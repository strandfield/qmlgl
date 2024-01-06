// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#include <qmlgl/cameracontroller.h>

class OrthographicCameraController : public CameraController
{
  Q_OBJECT
  Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
  Q_PROPERTY(QVector3D forwardVector READ forwardVector WRITE setForwardVector NOTIFY forwardVectorChanged)
  Q_PROPERTY(QVector3D upVector READ upVector WRITE setUpVector NOTIFY upVectorChanged)
  Q_PROPERTY(float nearPlane READ nearPlane WRITE setNearPlane NOTIFY nearPlaneChanged)
  Q_PROPERTY(float farPlane READ farPlane WRITE setFarPlane NOTIFY farPlaneChanged)
  Q_PROPERTY(float minimumFrustumHeight READ minimumFrustumHeight WRITE setMinimumFrustumHeight NOTIFY minimumFrustumHeightChanged)
  Q_PROPERTY(float frustumHeight READ frustumHeight WRITE setFrustumHeight NOTIFY frustumHeightChanged)
public:
  explicit OrthographicCameraController(QObject* parent = nullptr);

  QVector3D position() const;
  void setPosition(const QVector3D& pos);

  QVector3D forwardVector() const;
  void setForwardVector(const QVector3D& vec);

  QVector3D upVector() const;
  void setUpVector(const QVector3D& vec);

  float nearPlane() const;
  void setNearPlane(float near);
  float farPlane() const;
  void setFarPlane(float far);

  float minimumFrustumHeight() const;
  void setMinimumFrustumHeight(float minh);

  float frustumHeight() const;
  void setFrustumHeight(float h);

  void setCamera(Camera* cam) override;

Q_SIGNALS:
  void positionChanged();
  void forwardVectorChanged();
  void upVectorChanged();
  void nearPlaneChanged();
  void farPlaneChanged();
  void minimumFrustumHeightChanged();
  void frustumHeightChanged();

protected:
  void wheelEvent(QWheelEvent* ev) override;

  void wheelZoom(float step);

  void configureCamera();

private:
  QVector3D m_position = QVector3D(1, 0, 0);
  QVector3D m_forward_vector = QVector3D(-1, 0, 0);
  QVector3D m_up_vector = QVector3D(0, 0, 1);
  float m_near_plane = -1.f;
  float m_far_plane = 1.f;
  float m_minimum_frustum_height = 0.1f;
  float m_frustum_height = 1.f;
};
