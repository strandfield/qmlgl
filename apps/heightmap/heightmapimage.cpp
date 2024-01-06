// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "heightmapimage.h"

#include <QDir>
#include <QFile>
#include <QUuid>

HeightMapImage::HeightMapImage(QObject* parent) : QObject(parent)
{

}

HeightMapImage::~HeightMapImage()
{
  deleteCurrentImage();
}

HeightMapObject* HeightMapImage::heightmap() const
{
  return m_heightmap;
}

void HeightMapImage::setHeightMap(HeightMapObject* hm)
{
  if (m_heightmap != hm)
  {
    if (m_heightmap)
    {
      disconnect(m_heightmap, nullptr, this, nullptr);
    }

    m_heightmap = hm;
    Q_EMIT heightMapChanged();

    if (m_heightmap)
    {
      connect(m_heightmap, &HeightMapObject::contentChanged, this, &HeightMapImage::onHeightMapChanged);
    }

    onHeightMapChanged();
  }
}

QString HeightMapImage::imageFormat() const
{
  return m_image_format;
}

void HeightMapImage::setImageFormat(const QString& fmt)
{
  if (m_image_format != fmt)
  {
    m_image_format = fmt;
    Q_EMIT imageFormatChanged();
    
    onHeightMapChanged();
  }
}

QUrl HeightMapImage::imageUrl() const
{
  return m_image_url;
}

void HeightMapImage::onHeightMapChanged()
{
  deleteCurrentImage();

  if (!heightmap())
  {
    m_image_url = QUrl();
    Q_EMIT imageUrlChanged();
    return;
  }

  QImage img = m_image_renderer.render(heightmap()->heightmap());

  static QString appid = QUuid::createUuid().toString(QUuid::WithoutBraces).left(8);
  static int imcount = 0;
  QString savepath = QDir::tempPath() + QString("/hmimg-%1-%2.%3").arg(appid, QString::number(++imcount), imageFormat());
  img.save(savepath);

  m_image_url = QUrl::fromLocalFile(savepath);
  Q_EMIT imageUrlChanged();
}

void HeightMapImage::deleteCurrentImage()
{
  if (m_image_url != QUrl())
  {
    QFile::remove(m_image_url.toLocalFile());
  }
}
