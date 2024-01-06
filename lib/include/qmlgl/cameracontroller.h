// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <QObject>

#include <QElapsedTimer>

#include <QQuaternion>
#include <QVector3D>

#include <QMouseEvent>
#include <QHoverEvent>
#include <QKeyEvent>
#include <QWheelEvent>

class Camera;

/**
 * @brief base class for controlling a camera through user inputs
 *
 * This is a base class that does not respond to user inputs.
 * Actual event handling is implemented differently by various subclasses.
 */
class CameraController : public QObject
{
  Q_OBJECT
  Q_PROPERTY(Camera* camera READ camera NOTIFY cameraChanged)
public:
  explicit CameraController(QObject* parent = nullptr);

  explicit CameraController(Camera& camera);

  Camera* camera() const;
  virtual void setCamera(Camera* cam);

  virtual void mousePressEvent(QMouseEvent* e);
  virtual void mouseMoveEvent(QMouseEvent* e);
  virtual void mouseReleaseEvent(QMouseEvent* e);

  virtual void hoverMoveEvent(QHoverEvent* e);

  virtual void keyPressEvent(QKeyEvent* e);
  virtual void keyReleaseEvent(QKeyEvent* e);

  virtual void wheelEvent(QWheelEvent* e);

Q_SIGNALS:
  void cameraChanged();

protected:
  void startMovement();
  virtual void update(qint64 elapsed);
  void endMovement();
  void timerEvent(QTimerEvent* ev) override;

private:
  Camera* m_camera = nullptr;
  int m_movement_timerid = -1;
  QElapsedTimer m_movement_elapsedtimer;
};

inline Camera* CameraController::camera() const
{
  return m_camera;
}

#endif // CAMERACONTROLLER_H
