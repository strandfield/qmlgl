// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "appwindow.h"

#include "appviewport.h"
#include "orbitalcamera.h"
#include "pipcamera.h"

#include <QQmlContext>

void init_qmlgl_appcommon_resources()
{
  Q_INIT_RESOURCE(qmlglappcommon);
}

OpenGLSceneFactory::~OpenGLSceneFactory()
{

}

/**
 * @brief constructs a window
 * @param factory  pointer used to construct the GL scene
 *
 * The scene is constructed when the OpenGLContext becomes available.
 */
AppWindow::AppWindow(std::unique_ptr<OpenGLSceneFactory> factory) : Window(),
  m_factory(std::move(factory))
{
  init_qmlgl_appcommon_resources();

  qmlRegisterType<AppViewport>("QmlGL", 1, 0, "AppViewport");
  qmlRegisterType<OrbitalCameraController>("QmlGL", 1, 0, "OrbitalCameraController");
  qmlRegisterType<PictureInPictureCameraController>("QmlGL", 1, 0, "PictureInPictureCameraController");
}

/**
 * @brief exposes a QObject to QML
 * @param obj   pointer to the QObject
 * @param opts  options
 *
 * The QObject becomes accessible in any QML document under is objectName().
 *
 * If @a opts is ExposeChildrenRecursively, this function is called recursively
 * with the children of @a obj.
 */
void AppWindow::exposeQObjectToQml(QObject* obj, ExposeOptions opts)
{
  if (opts == ExposeChildrenRecursively)
  {
    for (QObject* child : obj->children())
    {
      exposeQObjectToQml(child, opts);
    }
  }

  if (!obj->objectName().isEmpty())
  {
    rootContext()->setContextProperty(obj->objectName(), obj);
  }
}

/**
 * @brief exposes a QObject to QML
 * @param obj   pointer to the object
 * @param name  the name used to reference the object in QML
 */
void AppWindow::exposeQObjectToQml(QObject* obj, const QString& name)
{
  rootContext()->setContextProperty(name, obj);
}

std::unique_ptr<OpenGLScene> AppWindow::createOpenGLScene()
{
  return m_factory->create(openglContext());
}
