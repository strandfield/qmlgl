// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>

#include <QMatrix4x4>
#include <QVector3D>

class CameraController;
class ViewFrustum;
class Viewport;

/**
 * @brief a camera object used to render the OpenGL scene
 *
 * This class stores the information used for the rendering of the OpenGL scene.
 * User interactions with the camera are better handled by a CameraController.
 *
 * https://learnopengl.com/Getting-started/Camera
 *
 * @sa class CameraController.
 */
class Camera : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
  Q_PROPERTY(QVector3D viewCenter READ viewCenter WRITE setViewCenter NOTIFY viewCenterChanged)
  Q_PROPERTY(QVector3D forwardVector READ forwardVector NOTIFY forwardVectorChanged)
  Q_PROPERTY(QVector3D upVector READ upVector NOTIFY upVectorChanged)
  Q_PROPERTY(QVector3D rightVector READ rightVector NOTIFY rightVectorChanged)
  Q_PROPERTY(ViewFrustum* frustum READ frustum WRITE setFrustum NOTIFY frustumChanged)
  Q_PROPERTY(CameraController* controller READ controller WRITE setController NOTIFY controllerChanged)
  Q_PROPERTY(Viewport* viewport READ viewport NOTIFY viewportChanged)
  Q_PROPERTY(QMatrix4x4 projectionMatrix READ projectionMatrix NOTIFY projectionMatrixChanged)
  Q_PROPERTY(QMatrix4x4 viewMatrix READ viewMatrix NOTIFY viewMatrixChanged)
public:
  explicit Camera(QObject* parent = nullptr);

  const QVector3D& position() const;
  void setPosition(const QVector3D& pos);

  const QVector3D& viewCenter() const;
  void setViewCenter(const QVector3D& pos);

  void reset(const QVector3D& position, const QVector3D& view_center, const QVector3D& up_vector);
  void reset(const Camera& other);

  float distanceToViewCenter() const;

  QVector3D forwardVector() const;
  QVector3D upVector() const;
  QVector3D rightVector() const;

  ViewFrustum* frustum() const;
  void setFrustum(ViewFrustum* f);

  bool isOrthographic() const;

  CameraController* controller() const;
  void setController(CameraController* camcon);

  Viewport* viewport() const;
  void setViewport(Viewport* vp);

  QMatrix4x4 projectionMatrix() const;
  QMatrix4x4 viewMatrix() const;

Q_SIGNALS:
  void positionChanged();
  void viewCenterChanged();
  void frustumChanged();
  void controllerChanged();
  void viewportChanged();
  void forwardVectorChanged();
  void upVectorChanged();
  void rightVectorChanged();
  void projectionMatrixChanged();
  void viewMatrixChanged();

private:
  QVector3D m_position;
  QVector3D m_view_center;
  QVector3D m_up_vector;
  ViewFrustum* m_frustum;
  CameraController* m_controller = nullptr;
  Viewport* m_viewport = nullptr;
};

void translate(Camera& camera, const QVector3D& v, bool move_view_center = true);
void translateWorld(Camera& camera, const QVector3D& v);
void rotate(Camera& camera, const QQuaternion& q);
void rotateAboutViewCenter(Camera& camera, const QQuaternion& q);
void pitch(Camera& camera, float angle);
void pitchAboutViewCenter(Camera& camera, float angle);
void yaw(Camera& camera, float angle, const QVector3D& axis = QVector3D(0, 0, 1));
void yawAboutViewCenter(Camera& camera, float angle, const QVector3D& axis = QVector3D(0,0,1));

#endif // CAMERA_H
