// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "heightfieldmodel.h"

HeightFieldModel::HeightFieldModel(QObject* parent) : QObject(parent)
{
  setHeightMap(new HeightMapObject(this));
}

int HeightFieldModel::heightmapRevision() const
{
  return m_heightmap_revision;
}

HeightMapObject* HeightFieldModel::heightmap() const
{
  return m_heightmap;
}

void HeightFieldModel::setHeightMap(HeightMapObject* hm)
{
  if (m_heightmap != hm)
  {
    if (m_heightmap)
    {
      disconnect(m_heightmap, nullptr, this, nullptr);
    }

    m_heightmap = hm;

    if (m_heightmap)
    {
      connect(m_heightmap, &HeightMapObject::contentChanged, this, &HeightFieldModel::incrHeightMapRevision);
    }

    Q_EMIT heightmapChanged();

    incrHeightMapRevision();
  }
}

QVector3D HeightFieldModel::origin() const
{
  return m_origin;
}

void HeightFieldModel::setOrigin(QVector3D pos)
{
  if (m_origin != pos)
  {
    m_origin = pos;
    Q_EMIT originChanged();
  }
}

QSize HeightFieldModel::size() const
{
  return m_size;
}

void HeightFieldModel::setSize(const QSize& s)
{
  if (m_size != s)
  {
    m_size = s;
    Q_EMIT sizeChanged();
  }
}

qreal HeightFieldModel::resolution() const
{
  return m_resolution;
}

void HeightFieldModel::setResolution(qreal res)
{
  if (!qFuzzyCompare(res, m_resolution))
  {
    m_resolution = res;
    Q_EMIT resolutionChanged();
  }
}

QColor HeightFieldModel::color() const
{
  return m_color;
}

void HeightFieldModel::setColor(const QColor& c)
{
  if (m_color != c)
  {
    m_color = c;
    Q_EMIT colorChanged();
  }
}

QColor HeightFieldModel::outsideColor() const
{
  return m_outside_color;
}

void HeightFieldModel::setOutsideColor(const QColor& c)
{
  if (m_outside_color != c)
  {
    m_outside_color = c;
    Q_EMIT outsideColorChanged();
  }
}

void HeightFieldModel::incrHeightMapRevision()
{
  ++m_heightmap_revision;
  Q_EMIT heightmapRevisionChanged();
}
