// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#include <qmlgl/cameracontroller.h>
#include <qmlgl/defs.h>

#include <optional>

class FreeflyCameraController : public CameraController
{
  Q_OBJECT
  Q_PROPERTY(float translationSpeed READ translationSpeed WRITE setTranslationSpeed NOTIFY translationSpeedChanged)
public:
  explicit FreeflyCameraController(QObject* parent = nullptr);

  float translationSpeed() const;
  void setTranslationSpeed(float speed);

  float rotationSpeed() const;
  void setRotationSpeed(float rs);

  Q_INVOKABLE void increaseTranslationSpeed();
  Q_INVOKABLE void decreaseTranslationSpeed();

Q_SIGNALS:
  void translationSpeedChanged();

protected:
  void update(qint64 frame_delta_time_ms) override;

  void mousePressEvent(QMouseEvent* ev) override;
  void mouseMoveEvent(QMouseEvent* ev) override;
  void mouseReleaseEvent(QMouseEvent* ev) override;

  void keyPressEvent(QKeyEvent* ev) override;
  void keyReleaseEvent(QKeyEvent* ev) override;

  void wheelEvent(QWheelEvent* ev) override;

  void rotateCamera(float yaw, float pitch);

private:
  float m_translation_speed = 20.f;
  float m_rotation_speed = 6.f;
  std::optional<QPoint> m_previous_move_pos;

  enum MovementFlag
  {
    RLEFT = 1,
    RRIGHT = 2,
    RUP = 4,
    RDOWN = 8,
    LEFT = 16,
    RIGHT = 32,
    FORWARD = 64,
    BACKWARD = 128,
    FORWARDCAM = 256,
    BACKWARDCAM = 512,
    ZUP = 1024,
    ZDOWN = 2048,
  };

  int m_keyboard_movement_flags = 0;
};
