// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef HEIGHTFIELDMODEL_H
#define HEIGHTFIELDMODEL_H

#include "heightmapobject.h"

#include <QColor>
#include <QSize>

class HeightFieldModel : public QObject
{
  Q_OBJECT
  Q_PROPERTY(int heightmapRevision READ heightmapRevision NOTIFY heightmapRevisionChanged)
  Q_PROPERTY(HeightMapObject* heightmap READ heightmap WRITE setHeightMap NOTIFY heightmapChanged)
  Q_PROPERTY(QVector3D origin READ origin WRITE setOrigin NOTIFY originChanged)
  Q_PROPERTY(QSize size READ size WRITE setSize NOTIFY sizeChanged)
  Q_PROPERTY(qreal resolution READ resolution WRITE setResolution NOTIFY resolutionChanged)
  Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
  Q_PROPERTY(QColor outsideColor READ outsideColor WRITE setOutsideColor NOTIFY outsideColorChanged)
public:
  explicit HeightFieldModel(QObject* parent = nullptr);

  int heightmapRevision() const;

  HeightMapObject* heightmap() const;
  void setHeightMap(HeightMapObject* hm);

  QVector3D origin() const; // center of the mesh
  void setOrigin(QVector3D pos);

  QSize size() const;
  void setSize(const QSize& s);

  qreal resolution() const;
  void setResolution(qreal res);

  QColor color() const;
  void setColor(const QColor& c);

  QColor outsideColor() const;
  void setOutsideColor(const QColor& c);

Q_SIGNALS:
  void heightmapRevisionChanged();
  void heightmapChanged();
  void originChanged();
  void sizeChanged();
  void resolutionChanged();
  void colorChanged();
  void outsideColorChanged();

protected Q_SLOTS:
  void incrHeightMapRevision();

private:
  int m_heightmap_revision = 0;
  HeightMapObject* m_heightmap = nullptr;
  QVector3D m_origin;
  QSize m_size = QSize(100, 100);
  qreal m_resolution = 1.f;
  QColor m_color = QColor("lime");
  QColor m_outside_color = QColor("deepskyblue");
};

#endif // HEIGHTFIELDMODEL_H
