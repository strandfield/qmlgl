// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#include "model.h"
#include "ubershader.h"

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>

#include <map>

struct MeshRenderData
{
  std::unique_ptr<QOpenGLVertexArrayObject> m_vao;
  std::unique_ptr<QOpenGLBuffer> m_vertex_buffer;
  std::unique_ptr<QOpenGLBuffer> m_index_buffer;
  std::unique_ptr<QOpenGLBuffer> m_color_buffer;
  std::unique_ptr<QOpenGLBuffer> m_uv_buffer;
  std::unique_ptr<QOpenGLBuffer> m_normal_buffer;
};

class ModelRendererUberShader : public UberShader
{
public:
  ModelRendererUberShader();

  struct Config
  {
    bool has_colors = false;
    bool has_uv = false;
    bool has_normals = false;
    model::Material material;
  };

  QOpenGLShaderProgram* getProgram(Config conf);
};

class ModelRenderer
{
public:
  ModelRenderer();
  ~ModelRenderer();

  Model* model() const;
  void setModel(Model* m);

  void draw(QOpenGLFunctions* gl, const QMatrix4x4& projectionMatrix, const QMatrix4x4& viewMatrix);

  void releaseResources();

protected:
  void recursiveDraw(QOpenGLFunctions* gl, const QMatrix4x4& projectionMatrix, const QMatrix4x4& viewMatrix, const QMatrix4x4& modelMatrix, model::SceneNode* node);
  QOpenGLVertexArrayObject& get_vao(QOpenGLFunctions* gl, model::Mesh* mesh);
  ModelRendererUberShader::Config get_ubershader_conf(const model::Mesh& mesh) const;
  void bindShaderProgram(QOpenGLShaderProgram& shader_program);
  void releaseShaderProgram();
  QOpenGLTexture& get_texture(const QString& path);
  void bindTexture(QOpenGLTexture& texture);
  void releaseTexture();

private:
  Model* m_model = nullptr;
  std::map<model::Mesh*, std::unique_ptr<MeshRenderData>> m_mesh_render_data;
  std::map<QString, std::unique_ptr<QOpenGLTexture>> m_textures;
  ModelRendererUberShader m_ubershader;
  QOpenGLShaderProgram* m_current_shader_program = nullptr;
  QOpenGLTexture* m_current_texture = nullptr;
};
