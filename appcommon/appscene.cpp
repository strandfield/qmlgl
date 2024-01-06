// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "appscene.h"

#include "qmlgl/viewport.h"
#include "qmlgl/window.h"

#include <QOpenGLContext>
#include <QOpenGLFunctions>

#include <algorithm>

AppOpenGLScene::~AppOpenGLScene()
{

}

void AppOpenGLScene::synchronize(Window* window)
{
  m_render_data.window_size = window->size();
  m_render_data.clear_color = window->clearColor();

  m_render_data.viewports.clear();

  for (Viewport* vp : window->viewports())
  {
    if (!vp->camera())
      continue;

    m_render_data.viewports.push_back(AppViewportRenderData(vp));
  }
}

void AppOpenGLScene::render(Window* window)
{
  QSize wsize = m_render_data.window_size;
  qreal pixel_ratio = window->devicePixelRatio();

  QOpenGLFunctions* gl = window->openglContext()->functions();
  gl->glViewport(0, 0, wsize.width() * pixel_ratio, wsize.height() * pixel_ratio);

  if (m_render_data.clear_color.alpha() > 0)
  {
    QColor cc = m_render_data.clear_color;
    gl->glClearColor(cc.redF(), cc.greenF(), cc.blueF(), 1.0f);
    gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  gl->glEnable(GL_DEPTH_TEST);

  std::sort(m_render_data.viewports.begin(), m_render_data.viewports.end(),
    [](const ViewportRenderData& lhs, const ViewportRenderData& rhs) { return lhs.z < rhs.z; });

  for (const AppViewportRenderData& v : m_render_data.viewports)
  {
    if (v.visible)
    {
      QColor c = v.clear_color;

      if (c.alpha() > 0)
      {
        // https://stackoverflow.com/questions/18830589/shouldnt-glclearcolor-obey-the-drawing-area-set-by-glviewport

        gl->glEnable(GL_SCISSOR_TEST);
        gl->glScissor(v.rect.x(), wsize.height() - v.rect.height() - v.rect.y(), v.rect.width(), v.rect.height());

        gl->glClearColor(c.redF(), c.greenF(), c.blueF(), c.alphaF());
        gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        gl->glDisable(GL_SCISSOR_TEST);
      }

      glViewport(gl, v.rect);

      renderViewport(window, v);
    }
  }

  window->resetOpenGLState();
}

void AppOpenGLScene::glViewport(QOpenGLFunctions* gl, const QRect& rect)
{
  GLint gl_y = m_render_data.window_size.height() - rect.height() - rect.y();
  gl->glViewport(rect.x(), gl_y, rect.width(), rect.height());
}
