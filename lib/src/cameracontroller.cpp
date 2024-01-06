// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "cameracontroller.h"

#include "qmlgl/camera.h"

#include <QTimerEvent>

CameraController::CameraController(QObject* parent) : QObject(parent)
{

}

CameraController::CameraController(Camera& camera) : QObject(&camera),
  m_camera(&camera)
{

}

void CameraController::setCamera(Camera* cam)
{
  if (m_camera != cam)
  {
    m_camera = cam;
    Q_EMIT cameraChanged();
  }
}

void CameraController::mousePressEvent(QMouseEvent* e)
{

}

void CameraController::mouseMoveEvent(QMouseEvent* e)
{

}

void CameraController::mouseReleaseEvent(QMouseEvent* e)
{

}

void CameraController::hoverMoveEvent(QHoverEvent* e)
{

}

void CameraController::keyPressEvent(QKeyEvent* e)
{

}

void CameraController::keyReleaseEvent(QKeyEvent* e)
{

}

void CameraController::wheelEvent(QWheelEvent* e)
{

}

void CameraController::startMovement()
{
  if (m_movement_timerid == -1)
  {
    m_movement_timerid = startTimer(16);
    m_movement_elapsedtimer.restart();
  }
}

void CameraController::update(qint64 /* elapsed */)
{

}

void CameraController::endMovement()
{
  if (m_movement_timerid != -1)
  {
    killTimer(m_movement_timerid);
    m_movement_timerid = -1;
  }
}

void CameraController::timerEvent(QTimerEvent* ev)
{
  if (ev->timerId() == m_movement_timerid)
  {
    update(m_movement_elapsedtimer.elapsed());

    if (m_movement_timerid != -1)
    {
      m_movement_elapsedtimer.restart();
    }
  }
}
