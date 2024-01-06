// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#include <qmlgl/camera.h>
#include <qmlgl/cameracontroller.h>
#include <qmlgl/defs.h>

/**
 * @brief a controller that can be used to implement a picture-in-picture viewport
 *
 * This controller moves its camera() so that it looks at the same thing, from
 * the same direction, as the sourceCamera().
 */
class PictureInPictureCameraController : public CameraController
{
  Q_OBJECT
  Q_PROPERTY(Camera* sourceCamera READ sourceCamera WRITE setSourceCamera NOTIFY sourceCameraChanged)
  Q_PROPERTY(qreal distToViewCenter READ distToViewCenter WRITE setDistToViewCenter NOTIFY distToViewCenterChanged)
public:
  explicit PictureInPictureCameraController(QObject* parent = nullptr);

  Camera* sourceCamera() const;
  void setSourceCamera(Camera* cam);

  qreal distToViewCenter() const;
  void setDistToViewCenter(qreal d);

Q_SIGNALS:
  void sourceCameraChanged();
  void distToViewCenterChanged();

protected Q_SLOTS:
  void onSourceCameraViewMatrixChanged();

private:
  Camera* m_source_camera = nullptr;
  qreal m_dist_to_view_center = -1;
};
