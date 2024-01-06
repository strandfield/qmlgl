// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "assimpscene.h"

#include "q3dmodel.h"

#include "appcommon/appwindow.h"

AssimpScene::AssimpScene(QOpenGLContext* context) : QOpenGLFunctions(context)
{

}

void AssimpScene::synchronize(Window* window)
{
  AppOpenGLScene::synchronize(window);

  Q3dModel* model = window->findChild<Q3dModel*>();

  if (model && model->model() != m_model_renderer.model())
  {
    m_model_renderer.setModel(model->model());
  }
}

void AssimpScene::renderViewport(Window* window, const AppViewportRenderData& view)
{
  if (view.draw_world_frame)
  {
    m_frameaxes.drawWorldFrameAxes(this, view.projection_matrix, view.view_matrix);
  }

  m_model_renderer.draw(this, view.projection_matrix, view.view_matrix);

  if (view.draw_camera_orienation_axes)
  {
    m_frameaxes.drawCameraOrientationAxes(this, view.rect, windowData().window_size, view.view_matrix);
  }
}
