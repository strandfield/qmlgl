// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "appviewport.h"

AppViewportRenderData::AppViewportRenderData(Viewport* v)
{
  if (auto* appv = qobject_cast<AppViewport*>(v))
  {
    *this = appv->renderData();
  }
  else
  {
    static_cast<ViewportRenderData&>(*this) = v->renderData();
  }
}

AppViewport::AppViewport(QQuickItem* parent) : Viewport(parent)
{

}

bool AppViewport::drawWorldFrame() const
{
  return m_draw_world_frame;
}

void AppViewport::setDrawWorldFrame(bool on)
{
  if (m_draw_world_frame != on)
  {
    m_draw_world_frame = on;
    Q_EMIT drawWorldFrameChanged();

    update();
  }
}


bool AppViewport::drawCameraOrientationAxes() const
{
  return m_draw_camera_orienation_axes;
}

void AppViewport::setDrawCameraOrientationAxes(bool on)
{
  if (m_draw_camera_orienation_axes != on)
  {
    m_draw_camera_orienation_axes = on;
    Q_EMIT drawCameraOrientationAxesChanged();

    update();
  }
}


AppViewportRenderData AppViewport::renderData() const
{
  AppViewportRenderData r;
  static_cast<ViewportRenderData&>(r) = Viewport::renderData();
  r.draw_camera_orienation_axes = drawCameraOrientationAxes();
  r.draw_world_frame = drawWorldFrame();
  return r;
}
