// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "modelrenderer.h"

#include "appcommon/coloredvertex.h"
#include "appcommon/openglbuffer.h"

#include <QDir>
#include <QFileInfo>

ModelRendererUberShader::ModelRendererUberShader() : UberShader(":/shaders/model.vert", ":/shaders/model.frag")
{
  
}

QOpenGLShaderProgram* ModelRendererUberShader::getProgram(Config conf)
{
  glsl::PreprocessorDefines defines;

  if (conf.has_colors)
  {
    defines.emplace_back("MESH_HAS_COLORS");
  }

  if (conf.has_uv)
  {
    defines.emplace_back("MESH_HAS_UV");
  }

  if (conf.has_normals)
  {
    defines.emplace_back("MESH_HAS_NORMALS");
  }

  if (conf.material.is<model::material::FlatColorMaterial>())
  {
    defines.emplace_back("MATERIAL_FLAT_COLOR");
  }
  else if (conf.material.is<model::material::TextureMaterial>())
  {
    defines.emplace_back("MATERIAL_TEXTURE");
  }

  auto sp = UberShader::getProgram(defines, {});
  return sp.get();
}

ModelRenderer::ModelRenderer()
{

}

ModelRenderer::~ModelRenderer()
{
  releaseResources();
}

Model* ModelRenderer::model() const
{
  return m_model;
}

void ModelRenderer::setModel(Model* m)
{
  if (m_model != m)
  {
    releaseResources();
    m_model = m;
  }
}

void ModelRenderer::draw(QOpenGLFunctions* gl, const QMatrix4x4& projectionMatrix, const QMatrix4x4& viewMatrix)
{
  if (model() && model()->rootNode())
  {
    recursiveDraw(gl, projectionMatrix, viewMatrix, QMatrix4x4(), model()->rootNode());
    releaseTexture();
    releaseShaderProgram();
  }
}

void ModelRenderer::recursiveDraw(QOpenGLFunctions* gl, const QMatrix4x4& projectionMatrix, const QMatrix4x4& viewMatrix, const QMatrix4x4& modelMatrix, model::SceneNode* node)
{
  if (node->isTranformNode())
  {
    auto& trnode = static_cast<model::TransformNode&>(*node);
    QMatrix4x4 tr = modelMatrix * trnode.transformMatrix();
    
    for (const auto& child : trnode.children())
    {
      recursiveDraw(gl, projectionMatrix, viewMatrix, tr, child.get());
    }
  }
  else if (node->isMeshNode())
  {
    auto& meshnode = static_cast<model::MeshNode&>(*node);

    QOpenGLVertexArrayObject& vao = get_vao(gl, meshnode.mesh());

    ModelRendererUberShader::Config shadconf = get_ubershader_conf(*meshnode.mesh());
    QOpenGLShaderProgram* shader_program = m_ubershader.getProgram(shadconf);

    if (!shader_program)
    {
      return;
    }

    vao.bind();

    bindShaderProgram(*shader_program);

    shader_program->setUniformValue("model_matrix", modelMatrix);
    shader_program->setUniformValue("view_matrix", viewMatrix);
    shader_program->setUniformValue("projection_matrix", projectionMatrix);

    if (shadconf.material.is<model::material::FlatColorMaterial>())
    {
      auto& material = shadconf.material.as<model::material::FlatColorMaterial>();
      shader_program->setUniformValue("flat_color", QColor(material.color));
    }
    else if (shadconf.material.is<model::material::TextureMaterial>())
    {
      auto& material = shadconf.material.as<model::material::TextureMaterial>();
      QOpenGLTexture& texture = get_texture(material.texture_path);
      bindTexture(texture);
      shader_program->setUniformValue("texture_diffuse", 0);
    }

    gl->glDrawElements(GL_TRIANGLES, meshnode.mesh()->indices.size(), GL_UNSIGNED_INT, nullptr);

    // No neeed to release the shader program (the active one will be released when rendering is completed)
    // shader_program.release();

    vao.release();
  }
}

void ModelRenderer::releaseResources()
{
  m_ubershader.clearCache();
  m_mesh_render_data.clear();
  m_textures.clear();
}

QOpenGLVertexArrayObject& ModelRenderer::get_vao(QOpenGLFunctions* gl, model::Mesh* mesh)
{
  auto& entry = m_mesh_render_data[mesh];

  if (!entry)
  {
    entry = std::make_unique<MeshRenderData>();
  }

  if (entry->m_vao)
  {
    return *(entry->m_vao);
  }

  entry->m_vao = std::make_unique<QOpenGLVertexArrayObject>();
  entry->m_vao->create();

  entry->m_vao->bind();

  {
    BufferSpecs specs = BufferSpecsBuilder().index(0).tuplesize(3).type(GL_FLOAT);
    setup_buffer(entry->m_vertex_buffer, gl, buffer_data_from_vector(mesh->vertices), specs);
  }

  {
    setup_index_buffer(entry->m_index_buffer, gl, buffer_data_from_vector(mesh->indices));
  }

  if (!mesh->colors.empty())
  {
    BufferSpecs specs = BufferSpecsBuilder().index(2).tuplesize(3).type(GL_UNSIGNED_BYTE);
    setup_buffer(entry->m_color_buffer, gl, buffer_data_from_vector(mesh->colors), specs);
  }

  if (!mesh->uv.empty())
  {
    BufferSpecs specs = BufferSpecsBuilder().index(3).tuplesize(2).type(GL_FLOAT);
    setup_buffer(entry->m_uv_buffer, gl, buffer_data_from_vector(mesh->uv), specs);
  }

  if (!mesh->normals.empty())
  {
    BufferSpecs specs = BufferSpecsBuilder().index(4).tuplesize(3).type(GL_FLOAT);
    setup_buffer(entry->m_normal_buffer, gl, buffer_data_from_vector(mesh->normals), specs);
  }
 
  entry->m_vao->release();

  return *(entry->m_vao);
}

ModelRendererUberShader::Config ModelRenderer::get_ubershader_conf(const model::Mesh& mesh) const
{
  ModelRendererUberShader::Config conf;
  conf.has_colors = !mesh.colors.empty();
  conf.has_uv = !mesh.uv.empty();
  conf.has_normals = !mesh.normals.empty();
  conf.material = *(mesh.material);

  if (conf.material.is<model::material::DefaultMaterial>())
  {
    if (conf.has_colors)
    {
      conf.material = model::Material(model::material::VertexColorMaterial());
    }
    else
    {
      conf.material = model::Material(model::material::FlatColorMaterial(RgbColor(128, 128, 128)));
    }
  }

  return conf;
}

void ModelRenderer::bindShaderProgram(QOpenGLShaderProgram& shader_program)
{
  if (&shader_program != m_current_shader_program)
  {
    m_current_shader_program = &shader_program;
    shader_program.bind();
  }
}

void ModelRenderer::releaseShaderProgram()
{
  if (m_current_shader_program)
  {
    m_current_shader_program->release();
    m_current_shader_program = nullptr;
  }
}

QOpenGLTexture& ModelRenderer::get_texture(const QString& path)
{
  auto it = m_textures.find(path);

  if (it != m_textures.end())
  {
    return *(it->second);
  }

  auto& texture = m_textures[path];

  QString texturepath = QFileInfo(m_model->path()).dir().filePath(path);
  QImage image{ texturepath };

  texture = std::make_unique<QOpenGLTexture>(image.mirrored());
  
  return *texture;
}

void ModelRenderer::bindTexture(QOpenGLTexture& texture)
{
  if (m_current_texture != &texture)
  {
    texture.bind();
    m_current_texture = &texture;
  }
}

void ModelRenderer::releaseTexture()
{
  if (m_current_texture)
  {
    m_current_texture->release();
    m_current_texture = nullptr;
  }
}
