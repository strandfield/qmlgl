// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "heightmapobject.h"

#include <QJsonArray>
#include <QJsonObject>

#include <QDebug>

HeightMapObject::HeightMapObject(QObject* parent) : QObject(parent)
{

}

const HeightMap& HeightMapObject::heightmap() const
{
  return m_heightmap;
}

int HeightMapObject::rows() const
{
  return heightmap().rows();
}

int HeightMapObject::cols() const
{
  return heightmap().cols();
}

qreal HeightMapObject::altMin() const
{
  return heightmap().altMin();
}

void HeightMapObject::setAltMin(qreal val)
{
  if (!qFuzzyCompare(val, altMin()))
  {
    m_heightmap.setAltitudeRange(val, altMax());
    Q_EMIT altMinChanged();
  }
}

qreal HeightMapObject::altMax() const
{
  return heightmap().altMax();
}

void HeightMapObject::setAltMax(qreal val)
{
  if (!qFuzzyCompare(val, altMax()))
  {
    m_heightmap.setAltitudeRange(altMin(), val);
    Q_EMIT altMaxChanged();
  }
}

QVector2D HeightMapObject::bottomLeft() const
{
  return heightmap().bottomLeft();
}

QVector2D HeightMapObject::topRight() const
{
  return heightmap().topRight();
}

qreal HeightMapObject::xRes() const
{
  return heightmap().xRes();
}

qreal HeightMapObject::yRes() const
{
  return heightmap().yRes();
}

QJsonValue HeightMapObject::altitudeAt(int row, int col) const
{
  std::optional<float> z = heightmap().altitudeAt(row, col);
  return z.has_value() ? QJsonValue(z.value()) : QJsonValue();
}

float HeightMapObject::normalizedAltitudeAt(int row, int col) const
{
  return heightmap().normalizedAltitudeAt(row, col);
}

void HeightMapObject::fill(std::vector<float> zvalues, int nbcols)
{
  m_heightmap.fill(std::move(zvalues), nbcols);

  Q_EMIT sizeChanged();
  Q_EMIT contentChanged();
}

void HeightMapObject::fill(QJsonValue val)
{
  if (!val.isObject())
  {
    return;
  }

  QJsonObject obj = val.toObject();
  int nbcols = obj["cols"].toInt();

  if (nbcols <= 0)
  {
    qDebug() << "bad nbcols value: " << nbcols;
    return;
  }

  QJsonArray values = obj["zvalues"].toArray();

  std::vector<float> zvalues;
  zvalues.reserve(values.size());

  for (int i(0); i < values.size(); ++i)
  {
    zvalues.push_back(values.at(i).toDouble());
  }

  fill(std::move(zvalues), nbcols);
}

void HeightMapObject::setGeometry(QJsonValue val)
{
  if (!val.isObject())
    return;

  const QJsonObject obj = val.toObject();

  auto bottom_left = QVector2D(
    obj["bottom_left"]["x"].toDouble(),
    obj["bottom_left"]["y"].toDouble()
  );

  if (obj.contains("resolution"))
  {
    float resol = obj["resol"].toDouble();

    m_heightmap.setGeometry(bottom_left, resol);
  }
  else
  {
    auto top_right = QVector2D(
      obj["top_right"]["x"].toDouble(),
      obj["top_right"]["y"].toDouble()
    );

    m_heightmap.setGeometry(bottom_left, top_right);
  }

  Q_EMIT geometryChanged();
}
