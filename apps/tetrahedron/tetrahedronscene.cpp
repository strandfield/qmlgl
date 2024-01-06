// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tetrahedronscene.h"

TetrahedronScene::TetrahedronScene(QOpenGLContext* context) : QOpenGLFunctions(context)
{

}

void TetrahedronScene::synchronize(Window* window)
{
  AppOpenGLScene::synchronize(window);
}

void TetrahedronScene::renderViewport(Window* window, const AppViewportRenderData& view)
{
  if (view.draw_world_frame)
  {
    m_frameaxes.drawWorldFrameAxes(this, view.projection_matrix, view.view_matrix);
  }

  m_tetrahedron.draw(this, view.projection_matrix, view.view_matrix);

  if (view.draw_camera_orienation_axes)
  {
    m_frameaxes.drawCameraOrientationAxes(this, view.rect, windowData().window_size, view.view_matrix);
  }
}
