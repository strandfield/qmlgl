// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef CAMERARENDERDATA_H
#define CAMERARENDERDATA_H

#include "viewfrustumrenderdata.h"

#include <QVector3D>

/**
 * @brief raw struct storing camera data used for rendering
 */
struct CameraRenderData
{
  QVector3D position;
  QVector3D view_center;
  QVector3D up_vector;
  ViewFrustumRenderData viewfrustum;
};

#endif // CAMERARENDERDATA_H
