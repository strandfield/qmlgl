// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef HEIGHTMAPCONTROLLER_H
#define HEIGHTMAPCONTROLLER_H

#include "heightmapobject.h"

class QJSEngine;

class HeightMapController : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString sourceCode READ sourceCode NOTIFY sourceCodeChanged)

public:
  HeightMapController(QJSEngine& jsengine, HeightMapObject& hm, QObject* parent = nullptr);

  QString sourceCode() const;
  void setSourceCode(const QString& code);

  Q_INVOKABLE void openDialog();

protected:
  void onDialogAccepted();
  void onDialogRejected();

private:
  void generateHeightmap(int nbrows, int nbcols, const QString& srccode);
  void generateDefaultHeightmap(int nbrows, int nbcols);

Q_SIGNALS:
  void sourceCodeChanged();

private:
  QJSEngine& m_jsengine;
  HeightMapObject& m_heightmap;
  QString m_source_code;
};

#endif // HEIGHTMAPCONTROLLER_H
