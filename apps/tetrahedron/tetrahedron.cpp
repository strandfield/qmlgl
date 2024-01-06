// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tetrahedron.h"

#include "appcommon/coloredvertex.h"
#include "appcommon/openglbuffer.h"

Tetrahedron::Tetrahedron()
{

}

Tetrahedron::~Tetrahedron()
{
  releaseResources();
}

void Tetrahedron::draw(QOpenGLFunctions* gl, const QMatrix4x4& projectionMatrix, const QMatrix4x4& viewMatrix)
{
  QOpenGLVertexArrayObject& vao = get_vao(gl);
  vao.bind();

  QOpenGLShaderProgram& shader_program = get_shader_program();

  shader_program.bind();

  shader_program.setUniformValue("model_view_matrix", viewMatrix);
  shader_program.setUniformValue("projection_matrix", projectionMatrix);

  gl->glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, nullptr);

  shader_program.release();

  vao.release();
}

void Tetrahedron::releaseResources()
{
  m_shader_program.reset();
  m_vertex_buffer.reset();
  m_index_buffer.reset();
  m_vao.reset();
}

QOpenGLVertexArrayObject& Tetrahedron::get_vao(QOpenGLFunctions* gl)
{
  if (m_vao)
    return *m_vao;

  m_vao = std::make_unique<QOpenGLVertexArrayObject>();
  m_vao->create();

  m_vao->bind();

  {
    m_vertex_buffer = std::make_unique<QOpenGLBuffer>(QOpenGLBuffer::VertexBuffer);

    m_vertex_buffer->create();
    assert(m_vertex_buffer->isCreated());

    m_vertex_buffer->bind();
    m_vertex_buffer->setUsagePattern(QOpenGLBuffer::StaticDraw);

    float a = 2 * std::acos(-1.f) / 3;

    std::vector<ColoredVertex> vertexdata{
      { QVector3D(1, 0, 0), RgbColor(0xFF0000) },
      { QVector3D(std::cos(a), std::sin(a), 0), RgbColor(0x00FF00) },
      { QVector3D(std::cos(2 * a), std::sin(2 * a), 0), RgbColor(0x0000FF) },
      { QVector3D(0, 0, 1), RgbColor(0xFFD700) },
    };

    m_vertex_buffer->allocate(vertexdata.data(), static_cast<int>(vertexdata.size() * sizeof(ColoredVertex)));

    gl->glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(ColoredVertex), nullptr);
    gl->glVertexAttribPointer(2, 3, GL_UNSIGNED_BYTE, false, sizeof(ColoredVertex), (void*)sizeof(QVector3D));

    m_vertex_buffer->release();
  }

  {
    m_index_buffer = std::make_unique<QOpenGLBuffer>(QOpenGLBuffer::IndexBuffer);

    m_index_buffer->create();
    assert(m_index_buffer->isCreated());

    m_index_buffer->bind();
    m_index_buffer->setUsagePattern(QOpenGLBuffer::StaticDraw);

    std::vector<int> indexdata{ 0, 1, 3, 2, 0, 1 };

    m_index_buffer->allocate(indexdata.data(), static_cast<int>(indexdata.size() * sizeof(int)));
  }
 
  gl->glEnableVertexAttribArray(0);
  gl->glEnableVertexAttribArray(2);

  m_vao->release();

  return *m_vao;
}

QOpenGLShaderProgram& Tetrahedron::get_shader_program()
{
  if (m_shader_program)
    return *m_shader_program;

  m_shader_program = std::make_unique<QOpenGLShaderProgram>();

  m_shader_program->addShaderFromSourceFile(QOpenGLShader::Vertex, QString(":/shaders/vertexcolor.vert"));
  m_shader_program->addShaderFromSourceFile(QOpenGLShader::Fragment, QString(":/shaders/vertexcolor.frag"));

  m_shader_program->link();

  return *m_shader_program;
}
