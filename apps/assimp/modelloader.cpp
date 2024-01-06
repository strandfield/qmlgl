// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "modelloader.h"

#include <QDir>
#include <QFileInfo>

#include <assimp/Importer.hpp>  // C++ importer interface
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/scene.h>       // Output data structure

#include <iostream>
#include <stdexcept>

std::unique_ptr<Model> ModelLoader::load(const QString& file_path)
{
  m_model.setPath(file_path);

  Assimp::Importer importer;

  m_scene = importer.ReadFile(file_path.toStdString(), 
    aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType | aiProcess_GenBoundingBoxes);

  if (!m_scene)
  {
    throw std::runtime_error(std::string("Error loading file: (assimp:) ")
                             + importer.GetErrorString());
  }

  if (!m_scene->HasMeshes())
    throw std::runtime_error("No meshes found");

  for (unsigned int i(0); i < m_scene->mNumMaterials; ++i)
  {
    auto mat = processAiMaterial(m_scene->mMaterials[i]);
    m_model.appendMaterial(std::move(mat));
  }

  for (unsigned int i(0); i < m_scene->mNumMeshes; ++i)
  {
    auto mesh = processAiMesh(m_scene->mMeshes[i]);
    m_model.appendMesh(std::move(mesh));
  }

  std::unique_ptr<model::TransformNode> result = processAiNode(m_scene->mRootNode);

  // Assimp is y-up, so we need to apply a transform to be z-up.
  QMatrix4x4 m;
  m(1, 1) = 0;
  m(2,2) = 0;
  m(0, 0) = 1;
  m(1, 2) = -1;
  m(2, 1) = 1;
  result->setTransformMatrix(result->transformMatrix() * m);

  m_model.setRootNode(std::move(result));

  return std::make_unique<Model>(std::move(m_model));
}

std::unique_ptr<Model> ModelLoader::tryLoad(const QString& file_path) noexcept
{
  try
  {
    return load(file_path);
  }
  catch (const std::runtime_error& ex)
  {
    std::cerr << ex.what() << std::endl;
  }

  return nullptr;
}

static QVector3D convertVector(const aiVector3D& vec)
{
  return QVector3D(vec.x, vec.y, vec.z);
}

static RgbColor convertColor(const aiColor3D& c)
{
  return RgbColor(c.r, c.g, c.b);
}

static RgbColor convertColor(const aiColor4D& c)
{
  return RgbColor(c.r, c.g, c.b);
}

static QQuaternion convertQuaternion(const aiQuaternion& q)
{
  return QQuaternion(q.w, q.x, q.y, q.z);
}

static QMatrix4x4 convertTransform(const aiMatrix4x4& m)
{
  return QMatrix4x4(
    m.a1, m.a2, m.a3, m.a4, //
    m.b1, m.b2, m.b3, m.b4, //
    m.c1, m.c2, m.c3, m.c4, //
    m.d1, m.d2, m.d3, m.d4 //
    );
}

std::unique_ptr<model::Material> ModelLoader::processAiMaterial(const aiMaterial* ai_mat)
{
  if (ai_mat->GetTextureCount(aiTextureType_DIFFUSE) > 0)
  {
    aiString Path;

    if (ai_mat->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL)
        != AI_SUCCESS)
    {
      std::cerr << "wut?" << std::endl;
    }

    QString texturepath = QString(Path.C_Str());
    // TODO: handle embed textures ('texturepath' starts with '*')
    return std::make_unique<model::Material>(model::material::TextureMaterial(texturepath));
  }
  else
  {
    aiColor3D diffuse_color;

    if (ai_mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse_color) == AI_SUCCESS)
    {
      return std::make_unique<model::Material>(model::material::FlatColorMaterial(convertColor(diffuse_color)));
    }
    else
    {
      return std::make_unique<model::Material>(model::material::DefaultMaterial());
    }
  }
}

std::unique_ptr<model::Mesh> ModelLoader::processAiMesh(aiMesh* ai_mesh)
{
  auto mymesh = std::make_unique<model::Mesh>();

  if (ai_mesh->mNumVertices <= 0)
    throw std::runtime_error("No vertices in mesh");

  mymesh->vertices.reserve(ai_mesh->mNumVertices);

  // Get Vertices
  for (uint i = 0; i < ai_mesh->mNumVertices; ++i)
  {
    mymesh->vertices.push_back(convertVector(ai_mesh->mVertices[i]));
  }

  mymesh->indices.reserve(ai_mesh->mNumFaces * 3);

  // Get ai_mesh faces / indexes
  for (uint t = 0; t < ai_mesh->mNumFaces; ++t)
  {
    const aiFace& face = ai_mesh->mFaces[t];

    if (face.mNumIndices != 3)
    {
      std::cerr << "Mesh face with not a triangle, ignoring this primitive."
        << face.mNumIndices << std::endl;
      continue;
    }

    mymesh->indices.push_back(face.mIndices[0]);
    mymesh->indices.push_back(face.mIndices[1]);
    mymesh->indices.push_back(face.mIndices[2]);
  }

  // Colors
  if (ai_mesh->mColors[0])
  {
    mymesh->colors.reserve(mymesh->vertices.size());

    for (uint i = 0; i < ai_mesh->mNumVertices; ++i)
    {
      const aiColor4D& c = ai_mesh->mColors[0][i];
      mymesh->colors.push_back(convertColor(c));
    }
  }

  // Texture coordinates
  if (ai_mesh->HasTextureCoords(0))
  {
    mymesh->uv.reserve(ai_mesh->mNumVertices);

    for (uint i = 0; i < ai_mesh->mNumVertices; ++i)
    {
      const aiVector3D& tcoord = ai_mesh->mTextureCoords[0][i];
      mymesh->uv.push_back(QVector2D(tcoord.x, tcoord.y));
    }
  }

  // Normals
  if (ai_mesh->HasNormals())
  {
    mymesh->normals.reserve(ai_mesh->mNumVertices);

    for (uint i = 0; i < ai_mesh->mNumVertices; ++i)
    {
      mymesh->normals.push_back(convertVector(ai_mesh->mNormals[i]));
    }
  }

  mymesh->material = m_model.getMaterial(ai_mesh->mMaterialIndex);

  mymesh->boundingbox = AABB(convertVector(ai_mesh->mAABB.mMin), convertVector(ai_mesh->mAABB.mMax));

  return mymesh;
}

std::unique_ptr<model::TransformNode> ModelLoader::processAiNode(const aiNode* node)
{
  auto tr = std::make_unique<model::TransformNode>(convertTransform(node->mTransformation));

  for (uint i = 0; i < node->mNumMeshes; ++i)
  {
    model::Mesh* mesh = m_model.getMesh(node->mMeshes[i]);
    tr->appendChild(std::make_unique<model::MeshNode>(mesh));
  }

  for (uint i = 0; i < node->mNumChildren; ++i)
  {
    auto mnode = processAiNode(node->mChildren[i]);
    tr->appendChild(std::move(mnode));
  }

  return tr;
}
