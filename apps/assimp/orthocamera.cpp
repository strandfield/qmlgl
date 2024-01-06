// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "orthocamera.h"

#include <qmlgl/camera.h>
#include <qmlgl/viewfrustum.h>
#include <qmlgl/viewport.h>

#include <QDebug>

#include <algorithm>

OrthographicCameraController::OrthographicCameraController(QObject* parent) : CameraController(parent)
{

}

QVector3D OrthographicCameraController::position() const
{
  return m_position;
}

void OrthographicCameraController::setPosition(const QVector3D& pos)
{
  if (m_position != pos)
  {
    m_position = pos;
    Q_EMIT positionChanged();

    if (camera())
    {
      configureCamera();
    }
  }
}

QVector3D OrthographicCameraController::forwardVector() const
{
  return m_forward_vector;
}

void OrthographicCameraController::setForwardVector(const QVector3D& vec)
{
  if (m_forward_vector != vec)
  {
    m_forward_vector = vec;
    Q_EMIT forwardVectorChanged();

    if (camera())
    {
      configureCamera();
    }
  }
}

QVector3D OrthographicCameraController::upVector() const
{
  return m_up_vector;
}

void OrthographicCameraController::setUpVector(const QVector3D& vec)
{
  if (m_up_vector != vec)
  {
    m_up_vector = vec;
    Q_EMIT upVectorChanged();

    if (camera())
    {
      configureCamera();
    }
  }
}

float OrthographicCameraController::nearPlane() const
{
  return m_near_plane;
}

void OrthographicCameraController::setNearPlane(float near)
{
  if (m_near_plane != near)
  {
    m_near_plane = near;
    Q_EMIT nearPlaneChanged();

    if (camera())
    {
      configureCamera();
    }
  }
}

float OrthographicCameraController::farPlane() const
{
  return m_far_plane;
}

void OrthographicCameraController::setFarPlane(float far)
{
  if (m_far_plane != far)
  {
    m_far_plane = far;
    Q_EMIT farPlaneChanged();

    if (camera())
    {
      configureCamera();
    }
  }
}

float OrthographicCameraController::minimumFrustumHeight() const
{
  return m_minimum_frustum_height;
}

void OrthographicCameraController::setMinimumFrustumHeight(float minh)
{
  if (!qFuzzyCompare(m_minimum_frustum_height, minh))
  {
    m_minimum_frustum_height = minh;
    Q_EMIT minimumFrustumHeightChanged();
    setFrustumHeight(frustumHeight());
  }
}

float OrthographicCameraController::frustumHeight() const
{
  return m_frustum_height;
}

void OrthographicCameraController::setFrustumHeight(float h)
{
  h = std::max(h, minimumFrustumHeight());

  if (!qFuzzyCompare(h, m_frustum_height))
  {
    m_frustum_height = h;
    Q_EMIT frustumHeightChanged();
    configureCamera();
  }
}

void OrthographicCameraController::setCamera(Camera* cam)
{
  CameraController::setCamera(cam);
  configureCamera();
}

void OrthographicCameraController::wheelZoom(float step) // TODO: change to int ?
{
  // +/- 10% for each step
  float new_frustum_height = frustumHeight() * std::pow(1.1f, step);
  setFrustumHeight(new_frustum_height);
}

void OrthographicCameraController::wheelEvent(QWheelEvent* event)
{
  int wheel_angle = event->angleDelta().y();

  wheelZoom(-wheel_angle / 120.f);
}

void OrthographicCameraController::configureCamera()
{
  if (!camera()) return;

  Camera& c = *camera();

  c.reset(position(), position() + forwardVector() * 0.5 * (nearPlane() + farPlane()), upVector());
  c.frustum()->setType(ViewFrustum::Ortho);
  c.frustum()->setFarPlane(farPlane());
  c.frustum()->setNearPlane(nearPlane());
  c.frustum()->setHeight(frustumHeight());
}
