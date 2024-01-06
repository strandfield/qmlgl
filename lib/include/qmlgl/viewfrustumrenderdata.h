// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef VIEWFRUSTUMRENDERDATA_H
#define VIEWFRUSTUMRENDERDATA_H

/**
 * @brief raw struct storing view frustum data used for rendering
 */
struct ViewFrustumRenderData
{
  bool orhtographic;
  float near_plane;
  float far_plane;
  float vertical_angle;
  float aspect_ratio;
};

#endif // VIEWFRUSTUMRENDERDATA_H
