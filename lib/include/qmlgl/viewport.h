// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "viewportrenderdata.h"

#include <QQuickItem>

#include <QRect>

class Camera;
class Window;

/**
 * @brief a QQuickItem that delimits where the GL scene is rendered
 *
 * A Viewport holds a camera() which is used to render the scene.
 *
 * It is possible to have multiple instances of this item in the same window
 * so that the OpenGLScene is rendered multiple times.
 * Since each Viewport can have its own camera, the OpenGLScene can therefore be
 * rendered from different viewing angle.
 * And although only one OpenGLScene is supported per window, it is also possible
 * to render a different "subscene" for each Viewport.
 */
class Viewport : public QQuickItem
{
  Q_OBJECT
  Q_PROPERTY(Camera* camera READ camera WRITE setCamera NOTIFY cameraChanged)
  Q_PROPERTY(QColor clearColor READ clearColor WRITE setClearColor NOTIFY clearColorChanged)
public:
  explicit Viewport(QQuickItem* parent = nullptr);

  Camera* camera() const;
  void setCamera(Camera* cam);

  QRect sceneRect() const;
  static float aspectRatio(const QRect& rect);

  const QColor& clearColor() const;
  void setClearColor(const QColor& color);

  ViewportRenderData renderData() const;

public Q_SLOTS:
  void update();

Q_SIGNALS:
  void cameraChanged();
  void clearColorChanged();

protected:
  void mousePressEvent(QMouseEvent* e) override;
  void mouseMoveEvent(QMouseEvent* e) override;
  void mouseReleaseEvent(QMouseEvent* e) override;

  void hoverMoveEvent(QHoverEvent* e) override;

  void keyPressEvent(QKeyEvent* e) override;
  void keyReleaseEvent(QKeyEvent* e) override;
  void wheelEvent(QWheelEvent* e) override;

protected:
  void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry) override;

private:
  void onWindowChanged();

private:
  Camera* m_camera = nullptr;
  Window* m_window = nullptr;
  QColor m_clear_color;
};

#endif // VIEWPORT_H
