// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#include <qmlgl/cameracontroller.h>
#include <qmlgl/defs.h>

#include <optional>

/**
 * @brief a controller that allows the user to rotate the camera around an object
 *
 * The point around which the camera is allowed to rotate is specified by target().
 *
 * The user is allowed to zoom in and out using the mouse-wheel.
 */
class OrbitalCameraController : public CameraController
{
  Q_OBJECT
  Q_PROPERTY(QVector3D target READ target WRITE setTarget NOTIFY targetChanged)
public:
  explicit OrbitalCameraController(QObject* parent = nullptr);

  bool xAxisInverted() const;
  bool yAxisInverted() const;

  void invertXAxis(bool on);
  void invertYAxis(bool on);

  float rotationSpeed() const;
  void setRotationSpeed(float rs);

  float zoomSpeed() const;
  void setZoomSpeed(float zs);

  QVector3D target() const;
  void setTarget(const QVector3D& pos);

  void setCamera(Camera* cam) override;

Q_SIGNALS:
  void targetChanged();

protected:
  void update(qint64 frame_delta_time_ms) override;

  void mousePressEvent(QMouseEvent* ev) override;
  void mouseMoveEvent(QMouseEvent* ev) override;
  void mouseReleaseEvent(QMouseEvent* ev) override;

  void keyPressEvent(QKeyEvent* ev) override;
  void keyReleaseEvent(QKeyEvent* ev) override;

  void wheelEvent(QWheelEvent* ev) override;

  void rotateCamera(const QPoint& mouse_delta);
  void rotateCamera(float yaw, float pitch);

  void zoom(float coeff);
  void wheelZoom(float amount);

private:
  bool m_invert_xaxis = false;
  bool m_invert_yaxis = false;
  float m_rotation_speed = float(M_PI) / 4.f;
  float m_zoom_speed = 10.f;
  QVector3D m_target_pos;

  std::optional<QPoint> m_previous_move_pos;

  enum MovementFlag
  {
    RLEFT = 1,
    RRIGHT = 2,
    RUP = 4,
    RDOWN = 8,
    ZOOMIN = 16,
    ZOOMOUT = 32,
  };

  int m_keyboard_movement_flags = 0;
};
