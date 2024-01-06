// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef HEIGHTMAPIMAGE_H
#define HEIGHTMAPIMAGE_H

#include "heightmapimagerenderer.h"
#include "heightmapobject.h"

#include <QUrl>

class HeightMapImage : public QObject
{
  Q_OBJECT
  Q_PROPERTY(HeightMapObject* heightmap READ heightmap WRITE setHeightMap NOTIFY heightMapChanged)
  Q_PROPERTY(QString imageFormat READ imageFormat WRITE setImageFormat NOTIFY imageFormatChanged)
  Q_PROPERTY(QUrl imageUrl READ imageUrl NOTIFY imageUrlChanged)
public:
  explicit HeightMapImage(QObject* parent = nullptr);
  ~HeightMapImage();

  HeightMapObject* heightmap() const;
  void setHeightMap(HeightMapObject* hm);

  QString imageFormat() const;
  void setImageFormat(const QString& fmt);

  QUrl imageUrl() const;

Q_SIGNALS: 
  void heightMapChanged();
  void imageFormatChanged();
  void imageUrlChanged();

protected Q_SLOTS:
  void onHeightMapChanged();

private:
  void deleteCurrentImage();

private:
  HeightMapObject* m_heightmap = nullptr;
  HeightMapImageRenderer m_image_renderer;
  QString m_image_format = "bmp";
  QUrl m_image_url;
};

#endif // HEIGHTMAPIMAGE_H
