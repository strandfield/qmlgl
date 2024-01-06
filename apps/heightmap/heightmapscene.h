// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#include "heightfieldmesh.h"

#include <appcommon/appscene.h>
#include <appcommon/frameaxes.h>

#include <QOpenGLFunctions>

class HeightMapScene : public AppOpenGLScene, public QOpenGLFunctions
{
public:
  explicit HeightMapScene(QOpenGLContext* context);

protected:
  void synchronize(Window* window) override;
  void renderViewport(Window* window, const AppViewportRenderData& view) override;

private:
  FrameAxes m_frameaxes;
  HeightFieldMesh m_mesh;
};
