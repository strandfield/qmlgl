// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef MODEL_H
#define MODEL_H

#include "aabb.h"

#include "appcommon/color.h"

#include <QMatrix4x4>
#include <QVector2D>
#include <QVector3D>

#include <QString>

#include <memory>
#include <variant>
#include <vector>

namespace model
{

namespace material
{

struct DefaultMaterial
{

};

struct VertexColorMaterial
{

};

struct FlatColorMaterial
{
  RgbColor color;

public:
  explicit FlatColorMaterial(RgbColor c) : color(c) { }
};

struct TextureMaterial
{
  QString texture_path;

public:
  explicit TextureMaterial(const QString& path) : texture_path(path) { }
};

} // namespace material

class Material
{
public:

  using Variant = std::variant<
    material::DefaultMaterial,
    material::VertexColorMaterial, 
    material::FlatColorMaterial, 
    material::TextureMaterial
  >;
  
  Material() = default;
  Material(const Material&) = default;
  explicit Material(Variant v) : m_variant(std::move(v))  { }

  template<typename T>
  bool is() const
  {
    return std::holds_alternative<T>(m_variant);
  }  
  
  template<typename T>
  const T& as() const
  {
    return std::get<T>(m_variant);
  }

  Material& operator=(const Material&) = default;

private:
  Variant m_variant;
};

struct Mesh
{
  std::vector<QVector3D> vertices;
  std::vector<int> indices;
  std::vector<RgbColor> colors;
  std::vector<QVector2D> uv;
  std::vector<QVector3D> normals;
  Material* material = nullptr;
  AABB boundingbox;
};

class SceneNode
{
public:
  virtual ~SceneNode() = default;

  virtual bool isMeshNode() const;
  virtual bool isTranformNode() const;
};

class TransformNode : public SceneNode
{
private:
  QMatrix4x4 m_transform_matrix;
  std::vector<std::unique_ptr<SceneNode>> m_children;

public:
  explicit TransformNode(const QMatrix4x4& m = QMatrix4x4());

  bool isTranformNode() const override;

  const QMatrix4x4& transformMatrix() const;
  void setTransformMatrix(const QMatrix4x4& m);

  void appendChild(std::unique_ptr<SceneNode> n);
  const std::vector<std::unique_ptr<SceneNode>>& children() const;
};

class MeshNode : public SceneNode
{
private:
  Mesh* m_mesh = nullptr;

public:
  MeshNode() = default;

  explicit MeshNode(Mesh* m);

  bool isMeshNode() const override;

  Mesh* mesh() const;
  void setMesh(Mesh* m);
};

} // namespace model

class Model
{
public:
  Model() = default;

  const QString& path() const;
  void setPath(const QString& path);

  model::SceneNode* rootNode() const;
  void setRootNode(std::unique_ptr<model::SceneNode> rn);

  void appendMaterial(std::unique_ptr<model::Material> m);
  model::Material* getMaterial(int index) const;

  void appendMesh(std::unique_ptr<model::Mesh> m);
  model::Mesh* getMesh(int index) const;

  AABB boundingBox() const;

private:
  QString m_path;
  std::vector<std::unique_ptr<model::Mesh>> m_meshes;
  std::vector<std::unique_ptr<model::Material>> m_materials;
  std::unique_ptr<model::SceneNode> m_root_node;
};

#endif // MODEL_H
