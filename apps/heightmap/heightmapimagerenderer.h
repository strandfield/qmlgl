// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef HEIGHTMAPIMAGERENDERER_H
#define HEIGHTMAPIMAGERENDERER_H

#include "heightmap.h"

#include <QImage>
#include <QLinearGradient>

#include <algorithm>

template<typename ColorFunc>
QImage render_heightmap_as_image(const HeightMap& hm, ColorFunc&& color, const QColor& invalid_color = Qt::black)
{
  QImage result{ hm.cols(), hm.rows(), QImage::Format_RGB32 };

  for (int x(0); x < hm.cols(); ++x)
  {
    for (int y(0); y < hm.rows(); ++y)
    {
      float val = hm.normalizedAltitudeAt(y, x);
      result.setPixelColor(x, y, val < 0 ? invalid_color : color(val));
    }
  }

  return result;
}

inline QColor sample_color_image1d(const QImage& img, float x)
{
  x = std::clamp(x, 0.f, 1.f);
  int col = std::round(x * (img.width() - 1));
  return img.pixelColor(col, 0);
}

class HeightMapImageRenderer
{
public:
  HeightMapImageRenderer();

  const QColor& invalidColor() const;
  void setInvalidColor(const QColor& c);

  const QImage& gradientImage() const;
  void setGradient(const QLinearGradient& gradient, int nb_samples = 256);

  QImage render(const HeightMap& hm);

private:
  QColor m_invalid_color = Qt::black;
  QImage m_gradient_image;
};

#endif // HEIGHTMAPIMAGERENDERER_H
