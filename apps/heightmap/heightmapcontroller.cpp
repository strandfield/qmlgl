// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "heightmapcontroller.h"

#include "heightmapdialog.h"

#include <QMessageBox>

#include <QFile>
#include <QJSEngine>
#include <QStandardPaths>
#include <QUuid>

#include <algorithm>

HeightMapController::HeightMapController(QJSEngine& jsengine, HeightMapObject& hm, QObject* parent)
  : QObject(parent)
  , m_jsengine(jsengine)
  , m_heightmap(hm)
{
}

QString HeightMapController::sourceCode() const
{
  return m_source_code;
}

void HeightMapController::setSourceCode(const QString& code)
{
  if (m_source_code != code)
  {
    m_source_code = code;
    Q_EMIT sourceCodeChanged();
  }
}

void HeightMapController::openDialog()
{
  auto* dialog = new HeightMapDialog();

  dialog->setHeightmapHeight(m_heightmap.heightmap().rows());
  dialog->setHeightmapWidth(m_heightmap.heightmap().cols());
  dialog->setHeightmapSourceCode(m_source_code);

  connect(dialog, &QDialog::accepted, this, &HeightMapController::onDialogAccepted);
  connect(dialog, &QDialog::rejected, this, &HeightMapController::onDialogRejected);

  dialog->open();
}

void HeightMapController::onDialogAccepted()
{
  auto* dialog = qobject_cast<HeightMapDialog*>(sender());

  if (!dialog)
    return;

  int nbrows = dialog->heightmapHeight();
  int nbcols = dialog->heightmapWidth();
  QString src = dialog->heightmapSourceCode();

  dialog->deleteLater();

  generateHeightmap(nbrows, nbcols, src);
}

void HeightMapController::onDialogRejected()
{
  auto* dialog = qobject_cast<HeightMapDialog*>(sender());

  if (!dialog)
    return;

  dialog->deleteLater();
}

namespace
{

QString write_temp_js_file(const QString& text)
{
  QString path = QString("%1/%2.js")
                   .arg(QStandardPaths::writableLocation(QStandardPaths::TempLocation),
                        QUuid::createUuid().toString(QUuid::StringFormat::WithoutBraces));

  QFile file{ path };
  file.open(QIODevice::WriteOnly);
  file.write(text.toUtf8());
  file.close();
  return path;
}

std::vector<float> convert_array(const QJSValue& val)
{
  int length = val.property("length").toInt();

  std::vector<float> result;
  result.reserve(length);

  for (int i(0); i < length; ++i)
  {
    result.push_back(val.property(i).toNumber());
  }

  return result;
}

} // namespace

void HeightMapController::generateHeightmap(int nbrows, int nbcols, const QString& srccode)
{
  QJSEngine js;

  QString modulepath = write_temp_js_file(srccode);

  QJSValue jsmodule = js.importModule(modulepath);

  if (jsmodule.isError())
  {
    QMessageBox::information(nullptr, "Error", "Error while compiling Javascript code");
    generateDefaultHeightmap(nbrows, nbcols);
    return;
  }

  QJSValue jsgenheightmap = jsmodule.property("generateHeightMap");
  QJSValue result = jsgenheightmap.call({ QJSValue(nbrows), QJSValue(nbcols) });

  if (jsmodule.isError())
  {
    QMessageBox::information(nullptr, "Error", "Error while evaluating Javascript code");
    generateDefaultHeightmap(nbrows, nbcols);
    return;
  }

  if (!result.isArray())
  {
    QMessageBox::information(nullptr, "Error", "Result javascript value is not an array");
    generateDefaultHeightmap(nbrows, nbcols);
    return;
  }

  std::vector<float> zvalues = convert_array(result);

  m_heightmap.fill(std::move(zvalues), nbcols);

  setSourceCode(srccode);

  QFile::remove(modulepath);
}

void HeightMapController::generateDefaultHeightmap(int nbrows, int nbcols)
{
  auto zvalues = std::vector<float>(nbrows, nbcols);
  std::fill(zvalues.begin(), zvalues.end(), 0.f);
  m_heightmap.fill(std::move(zvalues), nbcols);
}
