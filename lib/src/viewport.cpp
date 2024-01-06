// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "viewport.h"

#include "window.h"

#include "qmlgl/camera.h"
#include "qmlgl/cameracontroller.h"
#include "qmlgl/viewfrustum.h"

Viewport::Viewport(QQuickItem* parent) : QQuickItem(parent),
  m_clear_color(0.06f * 255, 0.06f * 255, 0.118f * 255)
{
  setAcceptedMouseButtons(Qt::AllButtons);
  setAcceptHoverEvents(true);
  setFlags(QQuickItem::ItemAcceptsInputMethod | QQuickItem::ItemIsFocusScope | QQuickItem::ItemClipsChildrenToShape);

  connect(this, &QQuickItem::windowChanged, this, &Viewport::onWindowChanged);
}

Camera* Viewport::camera() const
{
  return m_camera;
}

void Viewport::setCamera(Camera* cam)
{
  if (m_camera != cam)
  {
    if (m_camera)
    {
      disconnect(m_camera, nullptr, this, nullptr);
      m_camera->setViewport(nullptr);
    }

    m_camera = cam;
    Q_EMIT cameraChanged();

    if (m_camera)
    {
      connect(m_camera, &Camera::viewMatrixChanged, this, &Viewport::update);
      connect(m_camera, &Camera::projectionMatrixChanged, this, &Viewport::update);
      m_camera->setViewport(this);
    }
  }
}

/**
 * \brief returns the viewport rectangle in window coordinates
 */
QRect Viewport::sceneRect() const
{
  return mapRectToScene({ 0, 0, width(), height() }).toRect();
}

float Viewport::aspectRatio(const QRect& rect)
{
  return rect.width() / float(rect.height());
}

const QColor& Viewport::clearColor() const
{
  return m_clear_color;
}

void Viewport::setClearColor(const QColor& color)
{
  if (m_clear_color != color)
  {
    m_clear_color = color;
    Q_EMIT clearColorChanged();
  }
}

ViewportRenderData Viewport::renderData() const
{
  ViewportRenderData r;

  r.visible = isVisible();
  r.z = z();
  r.clear_color = clearColor();
  r.rect = sceneRect();
  r.camera.position = camera()->position();
  r.camera.up_vector = camera()->upVector();
  r.camera.view_center = camera()->viewCenter();
  r.camera.viewfrustum.aspect_ratio = camera()->frustum()->aspectRatio();
  r.camera.viewfrustum.far_plane = camera()->frustum()->farPlane();
  r.camera.viewfrustum.near_plane = camera()->frustum()->nearPlane();
  r.camera.viewfrustum.orhtographic = camera()->frustum()->isOrthographic();
  r.camera.viewfrustum.vertical_angle = camera()->frustum()->verticalAngle();
  r.view_matrix = camera()->viewMatrix();
  r.projection_matrix = camera()->projectionMatrix();

  return r;
}

void Viewport::update()
{
  if (window())
  {
    window()->update();
  }
}

void Viewport::mousePressEvent(QMouseEvent* e)
{
  forceActiveFocus();

  if (camera() && camera()->controller())
  {
    camera()->controller()->mousePressEvent(e);
  }
}

void Viewport::mouseMoveEvent(QMouseEvent* e)
{
  if (camera() && camera()->controller())
  {
    camera()->controller()->mouseMoveEvent(e);
  }
}

void Viewport::mouseReleaseEvent(QMouseEvent* e)
{
  if (camera() && camera()->controller())
  {
    camera()->controller()->mouseReleaseEvent(e);
  }
}

void Viewport::hoverMoveEvent(QHoverEvent* e)
{
  if (camera() && camera()->controller())
  {
    camera()->controller()->hoverMoveEvent(e);
  }
}

void Viewport::keyPressEvent(QKeyEvent* e)
{
  if (camera() && camera()->controller())
  {
    camera()->controller()->keyPressEvent(e);
  }
}

void Viewport::keyReleaseEvent(QKeyEvent* e)
{
  if (camera() && camera()->controller())
  {
    camera()->controller()->keyReleaseEvent(e);
  }
}

void Viewport::wheelEvent(QWheelEvent* e)
{
  if (camera() && camera()->controller())
  {
    camera()->controller()->wheelEvent(e);
  }
}

void Viewport::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry)
{
  QQuickItem::geometryChanged(newGeometry, oldGeometry);

  if (camera())
  {
    camera()->frustum()->setAspectRatio(aspectRatio(newGeometry.toRect()));
  }
}

void Viewport::onWindowChanged()
{
  if (m_window)
  {
    m_window->removeViewport(this);
  }

  m_window = nullptr;

  m_window = qobject_cast<Window*>(QQuickItem::window());

  if (m_window)
  {
    m_window->addViewport(this);
  }
}
