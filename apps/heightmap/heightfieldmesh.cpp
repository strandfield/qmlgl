// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "heightfieldmesh.h"

#include "heightfieldmodel.h"

#include <algorithm>

std::pair<std::vector<QVector3D>, std::vector<int>> generate_mesh(QSize mesh_size)
{
  std::vector<QVector3D> vertices;
  vertices.resize((mesh_size.width() + 1) * (mesh_size.height() + 1));

  float x = -0.5f * mesh_size.width();

  for (size_t i(0); i <= mesh_size.width(); i++)
  {
    float y = -0.5f * mesh_size.height();

    for (size_t j(0); j <= mesh_size.height(); j++)
    {
      vertices[i * (mesh_size.height() + 1) + j] = QVector3D(x, y, 0);

      y += 1.f;
    }

    x += 1.f;
  }

  std::vector<int> indices;
  size_t nblines = mesh_size.width() * (mesh_size.height() + 1) + mesh_size.height() * (mesh_size.width() + 1);
  indices.resize(2 * nblines);

  size_t w = 0;
  auto write_index = [&indices, &w](int index) {
    indices[w++] = index;
  };

  // horizontal (i.e. x-oriented) lines
  for (size_t i(0); i <= mesh_size.height(); i++)
  {
    int index = i;

    for (size_t j(0); j < mesh_size.width(); j++)
    {
      write_index(index);
      index += (mesh_size.height() + 1);
      write_index(index);
    }
  }

  // vertical (i.e. y-oriented) lines
  for (size_t i(0); i <= mesh_size.width(); i++)
  {
    int index = (mesh_size.height() + 1) * i;

    for (size_t j(0); j < mesh_size.height(); j++)
    {
      write_index(index);
      ++index;
      write_index(index);
    }
  }

  return { vertices, indices };
}

HeightFieldMesh::HeightFieldMesh()
{
  m_heightmap_rows = 10;
  m_heightmap_cols = 10;
  m_heightmap_bottomleft = QVector2D(-10, -10);
  m_heightmap_topright = QVector2D(10, 10);
  m_heightmap_altmin = 0;
  m_heightmap_altmax= 2;

  m_mesh_size = QSize(20, 20);
  m_mesh_dirty = true;
  m_mesh_resolution = 1.f;
  m_mesh_color = QColor("lime");
  m_zvalues = std::vector<float>(100, 0.f);
  /*m_zvalues[11] = 0.1f;
  m_zvalues[12] = 0.2f;
  m_zvalues[13] = 0.3f;*/
  for (size_t i(0); i < m_zvalues.size(); ++i)
  {
    m_zvalues[i] = (std::cos(i * 0.1f) + 1) * 0.5;
  }

  std::fill(m_zvalues.begin() + m_zvalues.size() / 3, m_zvalues.begin() + 2 * m_zvalues.size() / 3, -1.f);

  //for (size_t i(m_zvalues.size()/2); i < m_zvalues.size(); ++i)
  //{
  //  m_zvalues[i] = 1.f;
  //}
  m_zvalues_dirty = true;
}

HeightFieldMesh::~HeightFieldMesh()
{
  releaseResources();
}

void HeightFieldMesh::synchronize(const HeightFieldModel& model)
{
  if (model.heightmap())
  {
    HeightMapObject& hmo = *model.heightmap();

    if (model.heightmapRevision() != m_heightmap_revision)
    {
      m_zvalues = hmo.heightmap().zBuffer();
      m_zvalues_dirty = true;

      m_heightmap_rows = hmo.rows();
      m_heightmap_cols = hmo.cols();

      m_heightmap_revision = model.heightmapRevision();
    }

    m_heightmap_bottomleft = hmo.bottomLeft();
    m_heightmap_topright = hmo.topRight();
    m_heightmap_altmin = hmo.altMin();
    m_heightmap_altmax = hmo.altMax();
  }

  m_mesh_origin = model.origin();
  m_mesh_resolution = model.resolution();
  m_mesh_color = model.color();
  m_mesh_outside_color = model.outsideColor();

  if (m_mesh_size != model.size())
  {
    m_mesh_size = model.size();
    m_mesh_dirty = true;
  }
}

void HeightFieldMesh::draw(OpenGLFunctions* gl, const QMatrix4x4& projectionMatrix, const QMatrix4x4& viewMatrix)
{
  if (m_heightmap_rows * m_heightmap_cols == 0 || m_mesh_size.width() * m_mesh_size.height() == 0)
  {
    return;
  }

  udpate_buffers();

  QOpenGLVertexArrayObject& vao = get_vao(gl);

  vao.bind();

  QOpenGLShaderProgram& shader_program = get_shader_program();

  shader_program.bind();

  shader_program.setUniformValue("model_view_matrix", viewMatrix);
  shader_program.setUniformValue("projection_matrix", projectionMatrix);

  shader_program.setUniformValue("mesh_origin", m_mesh_origin);
  shader_program.setUniformValue("mesh_resolution", m_mesh_resolution);
  shader_program.setUniformValue("mesh_color", m_mesh_color);
  shader_program.setUniformValue("mesh_outside_color", m_mesh_outside_color);

  shader_program.setUniformValue("heightmap_rows", m_heightmap_rows);
  shader_program.setUniformValue("heightmap_cols", m_heightmap_cols);
  shader_program.setUniformValue("heightmap_bottomleft", m_heightmap_bottomleft);
  shader_program.setUniformValue("heightmap_topright", m_heightmap_topright);
  shader_program.setUniformValue("heightmap_altmin", m_heightmap_altmin);
  shader_program.setUniformValue("heightmap_altmax", m_heightmap_altmax);
  
  gl->glDrawElements(GL_LINES, m_mesh_nbindices, GL_UNSIGNED_INT, nullptr);

  shader_program.release();

  vao.release();
}

void HeightFieldMesh::releaseResources()
{
  m_shader_program.reset();
  m_vertex_buffer.reset();
  m_index_buffer.reset();
  m_vao.reset();
}

QOpenGLVertexArrayObject& HeightFieldMesh::get_vao(OpenGLFunctions* gl)
{
  if (m_vao)
  {
    return *m_vao; 
  }

  m_vao = std::make_unique<QOpenGLVertexArrayObject>();
  m_vao->create();

  m_vao->bind();

  {
    auto [vertex_data, index_data] = generate_mesh(m_mesh_size);
    m_mesh_nbindices = (int)index_data.size();

    {
      m_vertex_buffer = std::make_unique<QOpenGLBuffer>(QOpenGLBuffer::VertexBuffer);

      m_vertex_buffer->create();
      assert(m_vertex_buffer->isCreated());

      m_vertex_buffer->bind();
      m_vertex_buffer->setUsagePattern(QOpenGLBuffer::StaticDraw);

      m_vertex_buffer->allocate(vertex_data.data(), static_cast<int>(vertex_data.size() * sizeof(QVector3D)));

      gl->glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, nullptr);

      m_vertex_buffer->release();

      gl->glEnableVertexAttribArray(0);
    }

    {
      m_index_buffer = std::make_unique<QOpenGLBuffer>(QOpenGLBuffer::IndexBuffer);

      m_index_buffer->create();
      assert(m_index_buffer->isCreated());

      m_index_buffer->bind();
      m_index_buffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
      m_index_buffer->allocate(index_data.data(), static_cast<int>(index_data.size() * sizeof(int)));
    }

    m_mesh_dirty = false;
  }

  {
    m_texture_buffer = std::make_unique<QOpenGLBuffer>(QOpenGLBuffer::PixelUnpackBuffer);

    m_texture_buffer->create();
    assert(m_texture_buffer->isCreated());

    m_texture_buffer->bind();
    m_texture_buffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_texture_buffer->allocate(m_zvalues.data(), static_cast<int>(m_zvalues.size() * sizeof(float)));
    m_texture_buffer->release();

    m_texture = std::make_unique<QOpenGLTexture>(QOpenGLTexture::Target::TargetBuffer);
    m_texture->setFormat(QOpenGLTexture::TextureFormat::R32F);
    m_texture->bind();

    GLuint buffer_texture
      = m_texture->boundTextureId(QOpenGLTexture::BindingTarget::BindingTargetBuffer);
    gl->glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, m_texture_buffer->bufferId());
    gl->glActiveTexture(GL_TEXTURE0);
    gl->glBindTexture(GL_TEXTURE_BUFFER, buffer_texture);

    m_zvalues_dirty = false;
  }

  m_vao->release();

  return *m_vao;
}

void HeightFieldMesh::udpate_buffers()
{
  if (!m_vao)
  {
    // cannot update buffers that haven't been created yet!
    return;
  }

  if (m_mesh_dirty)
  {
    auto [vertex_data, index_data] = generate_mesh(m_mesh_size);
    m_mesh_nbindices = (int)index_data.size();

    m_vertex_buffer->bind();
    m_vertex_buffer->allocate(vertex_data.data(), static_cast<int>(vertex_data.size() * sizeof(QVector3D)));
    m_vertex_buffer->release();

    m_index_buffer->bind();
    m_index_buffer->allocate(index_data.data(), static_cast<int>(index_data.size() * sizeof(int)));
    m_vertex_buffer->release();

    m_mesh_dirty = false;
  }

  if (m_zvalues_dirty)
  {
    m_texture_buffer->bind();
    m_texture_buffer->allocate(m_zvalues.data(), static_cast<int>(m_zvalues.size() * sizeof(float)));
    m_texture_buffer->release();

    m_zvalues_dirty = false;
  }
}

QOpenGLShaderProgram& HeightFieldMesh::get_shader_program()
{
  if (m_shader_program)
    return *m_shader_program;

  m_shader_program = std::make_unique<QOpenGLShaderProgram>();

  m_shader_program->addShaderFromSourceFile(QOpenGLShader::Vertex, QString(":/shaders/heightfield.vert"));
  m_shader_program->addShaderFromSourceFile(QOpenGLShader::Fragment, QString(":/shaders/heightfield.frag"));

  m_shader_program->link();

  return *m_shader_program;
}
