// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#include "orthocamera.h"
#include "qboundingbox.h"

#include <vector>

template<typename T>
class ConnectionList
{
private:
  T& m_receiver;
  std::vector<QMetaObject::Connection> m_handles;

public:
  explicit ConnectionList(T& recv) : m_receiver(recv) { }

  void disconnectAll()
  {
    for (QMetaObject::Connection& c : m_handles)
    {
      QObject::disconnect(c);
    }

    m_handles.clear();
  }

  template<typename Sender, typename Signal, typename Slot>
  void connect(Sender&& sender, Signal&& signal, Slot&& slot)
  {
    QMetaObject::Connection c = QObject::connect(
      std::forward<Sender>(sender),
      std::forward<Signal>(signal),
      &m_receiver,
      std::forward<Slot>(slot)
      );
  }
};

class BboxSideCameraController : public OrthographicCameraController
{
  Q_OBJECT
  Q_PROPERTY(QBoundingBox* boundingBox READ boundingBox WRITE setBoundingBox NOTIFY boundingBoxChanged)
  Q_PROPERTY(Axis axis READ axis WRITE setAxis NOTIFY axisChanged)
public:
  explicit BboxSideCameraController(QObject* parent = nullptr);

  enum Axis
  {
    XAxis,
    YAxis,
    ZAxis,
  };
  Q_ENUM(Axis);

  QBoundingBox* boundingBox() const;
  void setBoundingBox(QBoundingBox* bbox);

  Axis axis() const;
  void setAxis(Axis a);

  Q_INVOKABLE float idealFrustumHeight() const;

Q_SIGNALS:
  void boundingBoxChanged();
  void axisChanged();

protected:
  void onBoundingBoxChanged();
  void reconfigureOrthoCameraController();
  void configureOrthoCameraController(const QVector3D& dimensions, const QVector3D& forward, const QVector3D& up);

private:
  QBoundingBox* m_bounding_box = nullptr;
  Axis m_axis = XAxis;
  ConnectionList<BboxSideCameraController> m_connections;
};
