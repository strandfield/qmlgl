// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#include "model.h"

#include <assimp/matrix4x4.h>

#include <memory>

struct aiScene;
struct aiNode;
struct aiMesh;
struct aiMaterial;

class ModelLoader
{
public:
  std::unique_ptr<Model> load(const QString& filePath);
  std::unique_ptr<Model> tryLoad(const QString& filePath) noexcept;

private:
  std::unique_ptr<model::Material> processAiMaterial(const aiMaterial* ai_mat);
  std::unique_ptr<model::Mesh> processAiMesh(aiMesh* ai_mesh);

  std::unique_ptr<model::TransformNode> processAiNode(const aiNode* node);

private:
  Model m_model;
  const aiScene* m_scene = nullptr;
};
