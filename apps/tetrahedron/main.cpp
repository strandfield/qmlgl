// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tetrahedronscene.h"

#include "appcommon/appwindow.h"

#include <QGuiApplication>

int main(int argc, char *argv[])
{
  QGuiApplication app{ argc, argv };

  AppWindow w{ OpenGLSceneFactory::factoryFor<TetrahedronScene>() };
  w.setSource(QUrl("qrc:/qml/MainWindow.qml"));
  w.show();

  return app.exec();
}