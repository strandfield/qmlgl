// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef APPSCENE_H
#define APPSCENE_H

#include "qmlgl/scene.h"

#include "appviewport.h"

#include <vector>

class QOpenGLFunctions;

struct WindowRenderData
{
  QSize window_size;
  QColor clear_color;
  std::vector<AppViewportRenderData> viewports;
};

class AppOpenGLScene : public OpenGLScene
{
public:
  ~AppOpenGLScene();

  void synchronize(Window* window) override;
  void render(Window* window) override;

  virtual void renderViewport(Window* window, const AppViewportRenderData& view) = 0;

protected:
  void glViewport(QOpenGLFunctions* gl, const QRect& rect);

  const WindowRenderData& windowData() const;

private:
  WindowRenderData m_render_data;
};

inline const WindowRenderData& AppOpenGLScene::windowData() const
{
  return m_render_data;
}

#endif // APPSCENE_H
