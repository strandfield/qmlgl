// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "freeflycamera.h"

#include <qmlgl/camera.h>
#include <qmlgl/viewfrustum.h>
#include <qmlgl/viewport.h>

FreeflyCameraController::FreeflyCameraController(QObject* parent) : CameraController(parent)
{

}

float FreeflyCameraController::translationSpeed() const
{
  return m_translation_speed;
}

void FreeflyCameraController::setTranslationSpeed(float speed)
{
  speed = std::max(speed, 0.f);

  if (!qFuzzyCompare(speed, m_translation_speed))
  {
    m_translation_speed = speed;
    Q_EMIT translationSpeedChanged();
  }
}

float FreeflyCameraController::rotationSpeed() const
{
  return m_rotation_speed;
}

void FreeflyCameraController::setRotationSpeed(float rs)
{
  m_rotation_speed = rs;
}

void FreeflyCameraController::increaseTranslationSpeed()
{
  setTranslationSpeed(translationSpeed() + 1.4f);
}

void FreeflyCameraController::decreaseTranslationSpeed()
{
  setTranslationSpeed(translationSpeed() - 1.4f);
}

void FreeflyCameraController::update(qint64 elapsed)
{
  const float d = translationSpeed() * 0.001f * elapsed;

  {
    auto delta = QVector3D(0,0,0);

    delta[2] += (m_keyboard_movement_flags & FORWARDCAM) ? 1.f : 0.f;
    delta[2] += (m_keyboard_movement_flags & BACKWARDCAM) ? -1.f : 0.f;

    if (!delta.isNull())
    {
      constexpr bool move_view_center = true;
      ::translate(*camera(), delta * d, move_view_center);
    }
  }

  {
    auto delta = QVector3D(0, 0, 0);

    const QVector3D fwd = camera()->forwardVector();
    const QVector3D ortho_vec = QVector3D::crossProduct(fwd, camera()->upVector());

    QVector3D fwd_2D = (fwd.x() != 0 || fwd.y() != 0) ? QVector3D(fwd.x(), fwd.y(), 0).normalized() : camera()->upVector();

    auto ortho_vec_2D = QVector3D(ortho_vec.x(), ortho_vec.y(), 0);
    ortho_vec_2D.normalize();

    delta += (m_keyboard_movement_flags & FORWARD) ? QVector3D(fwd_2D.x(), fwd_2D.y(), 0.f) : QVector3D{};
    delta += (m_keyboard_movement_flags & BACKWARD) ? QVector3D(-fwd_2D.x(), -fwd_2D.y(), 0.f) : QVector3D{};
    delta += (m_keyboard_movement_flags & LEFT) ? QVector3D(-ortho_vec_2D.x(), -ortho_vec_2D.y(), 0.f) : QVector3D{};
    delta += (m_keyboard_movement_flags & RIGHT) ? QVector3D(ortho_vec_2D.x(), ortho_vec_2D.y(), 0.f) : QVector3D{};

    if (!delta.isNull())
    {
      delta.normalize();
      ::translateWorld(*camera(), delta * d);
    }
  }

  {
    auto delta = QVector3D(0, 0, 0);

    delta += (m_keyboard_movement_flags & ZUP) ? QVector3D(0, 0, 1) : QVector3D{};
    delta += (m_keyboard_movement_flags & ZDOWN) ? QVector3D(0, 0, 1) : QVector3D{};

    if (!delta.isNull())
    {
      ::translateWorld(*camera(), delta * d);
    }
  }

  { // Keyboard rotate
    QPoint delta{};
    delta += m_keyboard_movement_flags & RRIGHT ? QPoint(1, 0) : QPoint(0, 0);
    delta += m_keyboard_movement_flags & RLEFT ? QPoint(-1, 0) : QPoint(0, 0);
    delta += m_keyboard_movement_flags & RDOWN ? QPoint(0, 1) : QPoint(0, 0);
    delta += m_keyboard_movement_flags & RUP ? QPoint(0, -1) : QPoint(0, 0);

    if (delta.x() != 0 || delta.y() != 0)
    {
      float rotation = m_rotation_speed * 0.001f * elapsed;
      rotateCamera(15 * delta.x() * rotation, 10 * delta.y() * rotation);
    }
  }
}

void FreeflyCameraController::rotateCamera(float yaw, float pitch)
{
  ::yaw(*camera(), -yaw);

  float a = std::acos(QVector3D::dotProduct(camera()->forwardVector(), QVector3D(0, 1, 0)));
  float old_pitch = rad2deg(a);
  float new_pitch = old_pitch - (-pitch);

  if (new_pitch >= 0.0f && new_pitch <= 180.0f)
  {
    ::pitch(*camera(), -pitch);
  }
}

void FreeflyCameraController::mousePressEvent(QMouseEvent* ev)
{
  if ((ev->buttons() & Qt::RightButton) || (ev->buttons() & Qt::MiddleButton))
  {
    m_previous_move_pos = ev->pos();
    ev->accept();
  }
}

void FreeflyCameraController::mouseMoveEvent(QMouseEvent* ev)
{
  if (m_previous_move_pos.has_value() && ev->buttons() == Qt::RightButton)
  {
    QPoint mouse_delta = ev->pos() - m_previous_move_pos.value();
    rotateCamera(0.1f * mouse_delta.x(), 0.1f * mouse_delta.y());
    m_previous_move_pos = ev->pos();
    ev->accept();
  }
}

void FreeflyCameraController::mouseReleaseEvent(QMouseEvent* ev)
{
  m_previous_move_pos.reset();
}

void FreeflyCameraController::keyPressEvent(QKeyEvent* ev)
{
  if (ev->modifiers() & Qt::ShiftModifier)
  {
    if (ev->key() == Qt::Key_Left)
      m_keyboard_movement_flags |= RLEFT;
    else if (ev->key() == Qt::Key_Right)
      m_keyboard_movement_flags |= RRIGHT;
    else if (ev->key() == Qt::Key_Up)
      m_keyboard_movement_flags |= RUP;
    else if (ev->key() == Qt::Key_Down)
      m_keyboard_movement_flags |= RDOWN;
  }
  
  if (ev->key() & Qt::AltModifier)
  {
    if (ev->key() == Qt::Key_Up)
      m_keyboard_movement_flags |= ZUP;
    else if (ev->key() == Qt::Key_Down)
      m_keyboard_movement_flags |= ZDOWN;
  }

  if (ev->modifiers() == Qt::NoModifier)
  {
    if (ev->key() == Qt::Key_Left)
      m_keyboard_movement_flags |= LEFT;
    else if (ev->key() == Qt::Key_Right)
      m_keyboard_movement_flags |= RIGHT;
    else if (ev->key() == Qt::Key_Up)
      m_keyboard_movement_flags |= FORWARD;
    else if (ev->key() == Qt::Key_Down)
      m_keyboard_movement_flags |= BACKWARD;
  }

  if (m_keyboard_movement_flags)
  {
    startMovement();
  }
}

void FreeflyCameraController::keyReleaseEvent(QKeyEvent* ev)
{
  if (ev->key() == Qt::Key_Left)
    m_keyboard_movement_flags &= ~(RLEFT | LEFT);
  else if (ev->key() == Qt::Key_Right)
    m_keyboard_movement_flags &= ~(RRIGHT | RIGHT);
  else if (ev->key() == Qt::Key_Up)
    m_keyboard_movement_flags &= ~(RUP | FORWARD | ZUP);
  else if (ev->key() == Qt::Key_Down)
    m_keyboard_movement_flags &= ~(RDOWN | BACKWARD | ZDOWN);

  if (!m_keyboard_movement_flags)
  {
    endMovement();
  }
}

void FreeflyCameraController::wheelEvent(QWheelEvent* ev)
{
  int wheel_angle = ev->angleDelta().y();

  constexpr bool move_view_center = true;
  translate(*camera(), QVector3D(0.f, wheel_angle / 120.f, 0.f), move_view_center);

  ev->accept();
}
