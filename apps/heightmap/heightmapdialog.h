// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef HEIGHTMAPDIALOG_H
#define HEIGHTMAPDIALOG_H

#include <QDialog>

class QPushButton;
class QSpinBox;
class QTextEdit;

class HeightMapDialog : public QDialog
{
  Q_OBJECT
public:
  explicit HeightMapDialog(QWidget* parent = nullptr);

  int heightmapWidth() const;
  void setHeightmapWidth(int w);

  int heightmapHeight() const;
  void setHeightmapHeight(int h);

  QString heightmapSourceCode() const;
  void setHeightmapSourceCode(const QString& code);

Q_SIGNALS:

private:
  QSpinBox* m_heightmap_width_spinbox = nullptr;
  QSpinBox* m_heightmap_height_spinbox = nullptr;
  QTextEdit* m_heightmap_code_textedit = nullptr;
};

#endif // HEIGHTMAPDIALOG_H
