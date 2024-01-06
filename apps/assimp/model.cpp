// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "model.h"

namespace model
{

bool SceneNode::isMeshNode() const
{
  return false;
}

bool SceneNode::isTranformNode() const
{
  return false;
}

TransformNode::TransformNode(const QMatrix4x4& m) :
  m_transform_matrix(m)
{

}

bool TransformNode::isTranformNode() const
{
  return true;
}

const QMatrix4x4& TransformNode::transformMatrix() const
{
  return m_transform_matrix;
}

void TransformNode::setTransformMatrix(const QMatrix4x4& m)
{
  m_transform_matrix = m;
}

void TransformNode::appendChild(std::unique_ptr<SceneNode> n)
{
  if (n)
  {
    m_children.push_back(std::move(n));
  }
}

const std::vector<std::unique_ptr<SceneNode>>& TransformNode::children() const
{
  return m_children;
}

MeshNode::MeshNode(Mesh* m) :
  m_mesh(m)
{

}

bool MeshNode::isMeshNode() const
{
  return true;
}

Mesh* MeshNode::mesh() const
{
  return m_mesh;
}

void MeshNode::setMesh(Mesh* m)
{
  m_mesh = m;
}

} // namespace model

const QString& Model::path() const
{
  return m_path;
}

void Model::setPath(const QString& path)
{
  m_path = path;
}

model::SceneNode* Model::rootNode() const
{
  return m_root_node.get();
}

void Model::setRootNode(std::unique_ptr<model::SceneNode> rn)
{
  m_root_node = std::move(rn);
}

void Model::appendMaterial(std::unique_ptr<model::Material> m)
{
  m_materials.push_back(std::move(m));
}

model::Material* Model::getMaterial(int index) const
{
  return m_materials.at(index).get();
}

void Model::appendMesh(std::unique_ptr<model::Mesh> m)
{
  m_meshes.push_back(std::move(m));
}

model::Mesh* Model::getMesh(int index) const
{
  return m_meshes.at(index).get();
}

namespace
{

void compute_bbox_recursive(AABB& bbox, const QMatrix4x4& modelMatrix, model::SceneNode* node)
{
  if (node->isTranformNode())
  {
    auto& trnode = static_cast<model::TransformNode&>(*node);
    QMatrix4x4 tr = modelMatrix * trnode.transformMatrix();

    for (const auto& child : trnode.children())
    {
      compute_bbox_recursive(bbox, tr, child.get());
    }
  }
  else if (node->isMeshNode())
  {
    auto& meshnode = static_cast<model::MeshNode&>(*node);
    const AABB& meshbbox = meshnode.mesh()->boundingbox;
    const AABB trbbox = meshbbox * modelMatrix;
    bbox = united(bbox, trbbox);
  }
}

}

AABB Model::boundingBox() const
{
  AABB result;
  compute_bbox_recursive(result, QMatrix4x4(), rootNode());
  return result;
}
