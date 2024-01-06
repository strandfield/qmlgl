// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "assimpscene.h"

#include "q3dmodelcontroller.h"

#include "bboxsidecamera.h"

#include "appcommon/appwindow.h"

#include <QGuiApplication>

int main(int argc, char *argv[])
{
  QGuiApplication app{ argc, argv };
  
  qmlRegisterType<OrthographicCameraController>("Assimp", 1, 0, "OrthographicCameraController");
  qmlRegisterType<BboxSideCameraController>("Assimp", 1, 0, "BboxSideCameraController");

  AppWindow w{ OpenGLSceneFactory::factoryFor<AssimpScene>() };

  auto* model = new Q3dModel(&w);
  auto* controller = new Q3dModelController(*model, &w);

  w.exposeQObjectToQml(model, "q_3dmodel");
  w.exposeQObjectToQml(controller, "q_3dmodel_controller");

  w.setSource(QUrl("qrc:/qml/MainWindow.qml"));

  w.show();

  return app.exec();
}
