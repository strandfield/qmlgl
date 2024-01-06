// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "q3dmodelcontroller.h"

#include "modelloader.h"

Q3dModelController::Q3dModelController(Q3dModel& model, QObject* parent) : QObject(parent),
  m_model(model)
{

}

void Q3dModelController::openModel(const QUrl& path)
{
  ModelLoader loader;
  std::unique_ptr<Model> model = loader.tryLoad(path.toLocalFile());

  if (model)
  {
    m_model.setModel(std::move(model));
  }
}
