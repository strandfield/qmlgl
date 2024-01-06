// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef HEIGHTMAPOBJECT_H
#define HEIGHTMAPOBJECT_H

#include "heightmap.h"

#include <QObject>

#include <QJsonValue>

class HeightMapObject : public QObject
{
  Q_OBJECT
  Q_PROPERTY(int rows READ rows NOTIFY sizeChanged)
  Q_PROPERTY(int cols READ cols NOTIFY sizeChanged)
  Q_PROPERTY(qreal altMin READ altMin WRITE setAltMin NOTIFY altMinChanged)
  Q_PROPERTY(qreal altMax READ altMax WRITE setAltMax NOTIFY altMaxChanged)
  Q_PROPERTY(QVector2D bottomLeft READ bottomLeft NOTIFY geometryChanged)
  Q_PROPERTY(QVector2D topRight READ topRight NOTIFY geometryChanged)
  Q_PROPERTY(qreal xRes READ xRes NOTIFY geometryChanged)
  Q_PROPERTY(qreal yRes READ yRes NOTIFY geometryChanged)
public:
  explicit HeightMapObject(QObject* parent = nullptr);

  const HeightMap& heightmap() const;

  int rows() const;
  int cols() const;

  qreal altMin() const;
  void setAltMin(qreal val);

  qreal altMax() const;
  void setAltMax(qreal val);

  QVector2D bottomLeft() const;
  QVector2D topRight() const;
  qreal xRes() const;
  qreal yRes() const;

  Q_INVOKABLE QJsonValue altitudeAt(int row, int col) const;
  Q_INVOKABLE float normalizedAltitudeAt(int row, int col) const;

  void fill(std::vector<float> zvalues, int nbcols);
  Q_INVOKABLE void fill(QJsonValue val);
  Q_INVOKABLE void setGeometry(QJsonValue val);

Q_SIGNALS:
  void sizeChanged();
  void altMinChanged();
  void altMaxChanged();
  void geometryChanged();
  void contentChanged();

private:
  HeightMap m_heightmap;
};

#endif // HEIGHTMAPOBJECT_H
