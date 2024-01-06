// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "heightmapscene.h"

#include "heightfieldmodel.h"

#include "qmlgl/window.h"

HeightMapScene::HeightMapScene(QOpenGLContext* context) : QOpenGLFunctions(context)
{

}

void HeightMapScene::synchronize(Window* window)
{
  AppOpenGLScene::synchronize(window);

  auto* model = window->findChild<HeightFieldModel*>();

  if (model)
  {
    m_mesh.synchronize(*model);
  }
}

void HeightMapScene::renderViewport(Window* window, const AppViewportRenderData& view)
{
  if (view.draw_world_frame)
  {
    m_frameaxes.drawWorldFrameAxes(this, view.projection_matrix, view.view_matrix);
  }

  auto* gl = QOpenGLContext::currentContext()->versionFunctions<HeightFieldMesh::OpenGLFunctions>();
  m_mesh.draw(gl, view.projection_matrix, view.view_matrix);

  if (view.draw_camera_orienation_axes)
  {
    m_frameaxes.drawCameraOrientationAxes(this, view.rect, windowData().window_size, view.view_matrix);
  }
}
