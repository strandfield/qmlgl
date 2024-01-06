// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "bboxsidecamera.h"

#include "qmlgl/camera.h"
#include "qmlgl/viewfrustum.h"

#include <algorithm>

BboxSideCameraController::BboxSideCameraController(QObject* parent) : OrthographicCameraController(parent),
  m_connections(*this)
{

}

QBoundingBox* BboxSideCameraController::boundingBox() const
{
  return m_bounding_box;
}

void BboxSideCameraController::setBoundingBox(QBoundingBox* bbox)
{
  if (m_bounding_box != bbox)
  {
    m_bounding_box = bbox;
    Q_EMIT boundingBoxChanged();
    onBoundingBoxChanged();
  }
}

BboxSideCameraController::Axis BboxSideCameraController::axis() const
{
  return m_axis;
}

void BboxSideCameraController::setAxis(Axis a)
{
  if (m_axis != a)
  {
    m_axis = a;
    Q_EMIT axisChanged();
    reconfigureOrthoCameraController();
  }
}

float BboxSideCameraController::idealFrustumHeight() const
{
  if (!boundingBox() || !camera() || !camera()->frustum())
    return 1.f;

  static const float margin_factor = 0.1f;

  QVector3D dim = boundingBox()->diagonal();

  float w, h;

  switch (axis())
  {
  break; case XAxis: std::tie(w, h) = std::make_pair(dim.y(), dim.z());
  break; case YAxis: std::tie(w, h) = std::make_pair(dim.x(), dim.z());
  break; case ZAxis: std::tie(w, h) = std::make_pair(dim.y(), dim.x());
  }


  float total_w = 2 * margin_factor * w + w;
  float total_h = 2 * margin_factor * h + h;
  return std::max(total_w / camera()->frustum()->aspectRatio(), total_h);
}

void BboxSideCameraController::onBoundingBoxChanged()
{
  //for (QMetaObject::Connection& c : m_connections)
  //{
  //  QObject::disconnect(c);
  //}

  //m_connections.clear();

  m_connections.disconnectAll();

  if (!boundingBox()) return;

  //m_connections.push_back(QObject::connect(boundingBox(), &QBoundingBox::bboxChanged, this, &BboxSideCameraController::configureOrthoCameraController));
  m_connections.connect(boundingBox(), &QBoundingBox::bboxChanged, &BboxSideCameraController::reconfigureOrthoCameraController);

  reconfigureOrthoCameraController();
}

void BboxSideCameraController::reconfigureOrthoCameraController()
{
  if (!boundingBox() || !camera())
    return;

  QBoundingBox& bbox = *boundingBox();

  switch (axis())
  {
  case XAxis:
  {
    QVector3D dim = bbox.diagonal();

    configureOrthoCameraController(
      QVector3D(dim.y(), dim.z(), dim.x()),
      QVector3D(-1, 0, 0),
      QVector3D(0, 0, 1));
  }
  break;
  case YAxis:
  {
    QVector3D dim = bbox.diagonal();

    configureOrthoCameraController(
      QVector3D(dim.x(), dim.z(), dim.y()),
      QVector3D(0, -1, 0),
      QVector3D(0, 0, 1));
  }
  break;
  case ZAxis:
  {
    QVector3D dim = bbox.diagonal();

    configureOrthoCameraController(
      QVector3D(dim.y(), dim.x(), dim.z()),
      QVector3D(0, 0, -1),
      QVector3D(-1, 0, 0));
  }
  break;
  default:
    break;
  }
}

void BboxSideCameraController::configureOrthoCameraController(const QVector3D& dimensions, const QVector3D& forward, const QVector3D& up)
{
  float near = 0.1f;

  QVector3D viewcenter = boundingBox()->center();
  // place the camera 1 meter in front of the "front" face
  float depth = dimensions.z();
  QVector3D campos = viewcenter - forward * (depth * 0.5f + 1.f + near);

  //float w = dimensions.x();
  //float h = dimensions.y();
  //float total_w = 2 * margin_factor * w + w;
  //float total_h = 2 * margin_factor * h + h;
  //float minh = std::max(total_w / camera()->frustum()->aspectRatio(), total_h);

  // the camera far plane should be 1 meters behind the bbox
  float far_plane = (depth + 2.f + near);

  setNearPlane(near);
  setFarPlane(far_plane);
  setPosition(campos);
  setForwardVector(forward);
  setUpVector(up);
  setFrustumHeight(idealFrustumHeight());
}
