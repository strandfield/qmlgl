// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "heightmapscene.h"

#include "appcommon/appwindow.h"

#include "heightfieldmodel.h"
#include "heightmapcontroller.h"
#include "heightmapimage.h"

#include <QApplication>

#include <QQmlEngine>

int main(int argc, char *argv[])
{
  QApplication app{ argc, argv }; // QApplication needed to use Qt Widgets

  auto* model = new HeightFieldModel();

  AppWindow w{ OpenGLSceneFactory::factoryFor<HeightMapScene>() };

  model->setParent(&w);

  HeightMapController controller{ *w.engine(), *model->heightmap() };

  qmlRegisterType<HeightMapObject>("HeightMap", 1, 0, "HeightMapObject");
  qmlRegisterType<HeightMapImage>("HeightMap", 1, 0, "HeightMapImage");
  qmlRegisterUncreatableType<HeightFieldModel>("HeightMap", 1, 0, "HeightFieldModel", "HeightFieldModel is exposed as a singleton");

  w.exposeQObjectToQml(model, "heightfield_model");
  w.exposeQObjectToQml(&controller, "heightmap_controller");

  w.setSource(QUrl("qrc:/qml/MainWindow.qml"));
  w.show();

  return app.exec();
}
