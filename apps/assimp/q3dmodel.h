// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#include "model.h"
#include "qboundingbox.h"

#include <QObject>

class Q3dModel : public QObject
{
  Q_OBJECT

  Q_PROPERTY(QString filePath READ filePath NOTIFY modelChanged)
  Q_PROPERTY(QVector3D modelCenter READ modelCenter NOTIFY modelChanged)
  Q_PROPERTY(QBoundingBox* boundingBox READ boundingBox NOTIFY modelChanged)
public:
  explicit Q3dModel(QObject* parent = nullptr);

  QString filePath() const;
  
  QVector3D modelCenter() const;

  void setModel(std::unique_ptr<Model> m);
  Model* model() const;

  QBoundingBox* boundingBox() const;

Q_SIGNALS:
  void modelChanged();

private:
  std::unique_ptr<Model> m_model;
  QBoundingBox* m_bbox = nullptr;
};
