// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef VIEWPORTRENDERDATA_H
#define VIEWPORTRENDERDATA_H

#include "camerarenderdata.h"

#include <QColor>
#include <QMatrix4x4>
#include <QRect>

/**
 * @brief raw struct for storing viewport data used for rendering
 */
struct ViewportRenderData
{
  bool visible;
  float z;
  QRect rect;
  QColor clear_color;
  CameraRenderData camera;
  QMatrix4x4 view_matrix;
  QMatrix4x4 projection_matrix;
};

#endif // VIEWPORTRENDERDATA_H
