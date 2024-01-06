// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "heightmapimagerenderer.h"

#include <QBrush>
#include <QPainter>
#include <QPen>

HeightMapImageRenderer::HeightMapImageRenderer()
{
  QLinearGradient gradient{ QPointF(0., 0.5), QPointF(1., 0.5) };
  gradient.setCoordinateMode(QGradient::StretchToDeviceMode);

  /*
  gradient.setStops({ QGradientStop({ 0, QColor("red") }),
                      QGradientStop({ 0.35, QColor("orange") }),
                      QGradientStop({ 0.7, QColor("yellow") }),
                      QGradientStop({ 1, QColor("lime") }) });
                      */

  // Jet 
  gradient.setStops({ QGradientStop({ 0, QColor("#000080") }),
                      QGradientStop({ 0.125, QColor("#0000ff") }),
                      QGradientStop({ 0.25, QColor("#0080ff") }),
                      QGradientStop({ 0.375, QColor("#00ffff") }),
                      QGradientStop({ 0.5, QColor("#80ff80") }),
                      QGradientStop({ 0.625, QColor("#ffff00") }),
                      QGradientStop({ 0.75, QColor("#ff8000") }),
                      QGradientStop({ 0.875, QColor("#ff0000") }),
                      QGradientStop({ 1, QColor("#800000") }) });

  setGradient(gradient);
}

const QColor& HeightMapImageRenderer::invalidColor() const
{
  return m_invalid_color;
}

void HeightMapImageRenderer::setInvalidColor(const QColor& c)
{
  m_invalid_color = c;
}

const QImage& HeightMapImageRenderer::gradientImage() const
{
  return m_gradient_image;
}

void HeightMapImageRenderer::setGradient(const QLinearGradient& gradient, int nb_samples)
{
  if (m_gradient_image.width() != nb_samples)
  {
    m_gradient_image = QImage(nb_samples, 1, QImage::Format_RGB32);
  }

  QPainter painter{ &m_gradient_image };
  painter.setPen(Qt::NoPen);
  QBrush brush{ gradient };
  painter.setBrush(brush);
  painter.drawRect(m_gradient_image.rect());
}

QImage HeightMapImageRenderer::render(const HeightMap& hm)
{
  
  return render_heightmap_as_image(hm, [this](float x) { 
    return sample_color_image1d(gradientImage(), x);
    }, 
    invalidColor());
}
