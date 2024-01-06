// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions_3_3_Core>


class HeightFieldModel;

class HeightFieldMesh
{
public:
  HeightFieldMesh();
  ~HeightFieldMesh();
  
  void synchronize(const HeightFieldModel& model);

  using OpenGLFunctions = QOpenGLFunctions_3_3_Core;

  void draw(OpenGLFunctions* gl, const QMatrix4x4& projectionMatrix, const QMatrix4x4& viewMatrix);

  void releaseResources();

protected:
  QOpenGLVertexArrayObject& get_vao(OpenGLFunctions* gl);
  void udpate_buffers();
  QOpenGLShaderProgram& get_shader_program();

private:
  int m_heightmap_revision = 0;
  int m_heightmap_rows = 0;
  int m_heightmap_cols = 0;
  QVector2D m_heightmap_bottomleft;
  QVector2D m_heightmap_topright;
  float m_heightmap_altmin;
  float m_heightmap_altmax;

  QVector3D m_mesh_origin;
  QSize m_mesh_size = QSize(20, 20);
  bool m_mesh_dirty = false;
  float m_mesh_resolution = 1.f;
  QColor m_mesh_color = QColor("lime");
  QColor m_mesh_outside_color = QColor("deepskyblue");
  std::vector<float> m_zvalues;
  bool m_zvalues_dirty = false;

  int m_mesh_nbindices = 0;

private:
  std::unique_ptr<QOpenGLVertexArrayObject> m_vao;
  std::unique_ptr<QOpenGLBuffer> m_vertex_buffer;
  std::unique_ptr<QOpenGLBuffer> m_index_buffer;
  std::unique_ptr<QOpenGLBuffer> m_texture_buffer;
  std::unique_ptr<QOpenGLTexture> m_texture;
  std::unique_ptr<QOpenGLShaderProgram> m_shader_program;
};
