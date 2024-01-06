// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "q3dmodel.h"

Q3dModel::Q3dModel(QObject* parent) : QObject(parent)
{

}

QString Q3dModel::filePath() const
{
  return m_model ? m_model->path() : QString();
}

QVector3D Q3dModel::modelCenter() const
{
  return m_model ? m_model->boundingBox().center() : QVector3D();
}

void Q3dModel::setModel(std::unique_ptr<Model> m)
{
  m_model = std::move(m);

  if (m_model)
  {
    if (!m_bbox)
    {
      m_bbox = new QBoundingBox(m_model->boundingBox(), this);
    }
    else
    {
      m_bbox->setAABB(m_model->boundingBox());
    }
  }
  else
  {
    if (m_bbox)
    {
      m_bbox->deleteLater();
      m_bbox = nullptr;
    }
  }

  Q_EMIT modelChanged();
}

Model* Q3dModel::model() const
{
  return m_model.get();
}

QBoundingBox* Q3dModel::boundingBox() const
{
  return m_bbox;
}
