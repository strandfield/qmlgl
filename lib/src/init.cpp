// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "init.h"

#include "camera.h"
#include "cameracontroller.h"
#include "viewfrustum.h"
#include "viewport.h"

#include <QQmlEngine>

namespace qmlgl
{

/**
 * @brief registers the qml types provided by the library
 *
 * This makes the following types available in the QmlGL module:
 * - Camera
 * - CameraController
 * - ViewFrustum
 * - Viewport
 *
 * It is safe to call this function multiple times; initialization
 * is only performed once.
 */
void init()
{
  static bool done = false;

  if (done) return;

  qmlRegisterType<Camera>("QmlGL", 1, 0, "Camera");
  qmlRegisterType<CameraController>("QmlGL", 1, 0, "CameraController");
  qmlRegisterType<ViewFrustum>("QmlGL", 1, 0, "ViewFrustum");
  qmlRegisterType<Viewport>("QmlGL", 1, 0, "Viewport");

  done = true;
}

} // namespace qmlgl
