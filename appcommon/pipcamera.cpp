// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "pipcamera.h"

#include <qmlgl/camera.h>
#include <qmlgl/viewfrustum.h>
#include <qmlgl/viewport.h>

#include <QDebug>

PictureInPictureCameraController::PictureInPictureCameraController(QObject* parent) : CameraController(parent)
{

}

Camera* PictureInPictureCameraController::sourceCamera() const
{
  return m_source_camera;
}

void PictureInPictureCameraController::setSourceCamera(Camera* cam)
{
  if (m_source_camera != cam)
  {
    if (m_source_camera)
    {
      disconnect(m_source_camera, nullptr, this, nullptr);
    }

    m_source_camera = cam;
    Q_EMIT sourceCameraChanged();

    if (m_source_camera)
    {
      connect(m_source_camera, &Camera::viewMatrixChanged, this, &PictureInPictureCameraController::onSourceCameraViewMatrixChanged);
      onSourceCameraViewMatrixChanged();
    }
  }
}

qreal PictureInPictureCameraController::distToViewCenter() const
{
  return m_dist_to_view_center;
}

void PictureInPictureCameraController::setDistToViewCenter(qreal d)
{
  if (!qFuzzyCompare(d, m_dist_to_view_center))
  {
    m_dist_to_view_center = d;
    Q_EMIT distToViewCenterChanged();

    onSourceCameraViewMatrixChanged();
  }
}

void PictureInPictureCameraController::onSourceCameraViewMatrixChanged()
{
  if (!camera() || !sourceCamera())
    return;

  camera()->reset(*sourceCamera());

  if (distToViewCenter() > 0)
  {
    camera()->setPosition(camera()->viewCenter() - distToViewCenter() * camera()->forwardVector());
  }
}
