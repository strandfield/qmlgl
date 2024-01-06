// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#include "qmlgl/viewport.h"

struct AppViewportRenderData : ViewportRenderData
{
  bool draw_world_frame = false;
  bool draw_camera_orienation_axes = false;

  AppViewportRenderData() = default;
  explicit AppViewportRenderData(Viewport* v);
};

class AppViewport : public Viewport
{
  Q_OBJECT
  Q_PROPERTY(bool drawWorldFrame READ drawWorldFrame WRITE setDrawWorldFrame NOTIFY drawWorldFrameChanged)
  Q_PROPERTY(bool drawCameraOrientationAxes READ drawCameraOrientationAxes WRITE setDrawCameraOrientationAxes NOTIFY drawCameraOrientationAxesChanged)
public:
  explicit AppViewport(QQuickItem* parent = nullptr);

  bool drawWorldFrame() const;
  void setDrawWorldFrame(bool on = true);

  bool drawCameraOrientationAxes() const;
  void setDrawCameraOrientationAxes(bool on = true);

  AppViewportRenderData renderData() const;

Q_SIGNALS:
  void drawWorldFrameChanged();
  void drawCameraOrientationAxesChanged();
  
private:
  bool m_draw_world_frame = true;
  bool m_draw_camera_orienation_axes = true;
};
