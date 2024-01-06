// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#include "q3dmodel.h"

#include <QObject>
#include <QUrl>

class Q3dModelController : public QObject
{
  Q_OBJECT
public:
  explicit Q3dModelController(Q3dModel& model, QObject* parent = nullptr);

  Q_INVOKABLE void openModel(const QUrl& path);

private:
  Q3dModel& m_model;
};
