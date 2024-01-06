// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

class Tetrahedron
{
public:
  Tetrahedron();
  ~Tetrahedron();

  void draw(QOpenGLFunctions* gl, const QMatrix4x4& projectionMatrix, const QMatrix4x4& viewMatrix);

  void releaseResources();

protected:
  QOpenGLVertexArrayObject& get_vao(QOpenGLFunctions* gl);
  QOpenGLShaderProgram& get_shader_program();

private:
  std::unique_ptr<QOpenGLVertexArrayObject> m_vao;
  std::unique_ptr<QOpenGLBuffer> m_vertex_buffer;
  std::unique_ptr<QOpenGLBuffer> m_index_buffer;
  std::unique_ptr<QOpenGLShaderProgram> m_shader_program;
};
