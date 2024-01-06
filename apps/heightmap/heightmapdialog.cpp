// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "heightmapdialog.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>

#include <QFont>

HeightMapDialog::HeightMapDialog(QWidget* parent)
  : QDialog(parent)
{
  setMinimumSize(QSize(600, 400));

  m_heightmap_width_spinbox = new QSpinBox();
  m_heightmap_height_spinbox = new QSpinBox();
  m_heightmap_code_textedit = new QTextEdit();
  auto* ok_button = new QPushButton("OK");

  m_heightmap_width_spinbox->setRange(10, 1000);
  m_heightmap_height_spinbox->setRange(10, 1000);
  QFont font{ "Courier" };
  font.setPointSize(8);
  m_heightmap_code_textedit->document()->setDefaultFont(font);

  auto* layout = new QVBoxLayout(this);
  {
    {
      auto* sl = new QHBoxLayout();
      sl->addWidget(m_heightmap_width_spinbox);
      sl->addWidget(m_heightmap_height_spinbox);
      layout->addLayout(sl);
    }

    layout->addWidget(m_heightmap_code_textedit);

    layout->addWidget(ok_button, 0, Qt::AlignHCenter);
  }

  connect(ok_button, &QPushButton::clicked, this, &QDialog::accept);
}

int HeightMapDialog::heightmapWidth() const
{
  return m_heightmap_width_spinbox->value();
}

void HeightMapDialog::setHeightmapWidth(int w)
{
  m_heightmap_width_spinbox->setValue(w);
}

int HeightMapDialog::heightmapHeight() const
{
  return m_heightmap_height_spinbox->value();
}

void HeightMapDialog::setHeightmapHeight(int h)
{
  m_heightmap_height_spinbox->setValue(h);
}

QString HeightMapDialog::heightmapSourceCode() const
{
  return m_heightmap_code_textedit->toPlainText();
}

void HeightMapDialog::setHeightmapSourceCode(const QString& code)
{
  m_heightmap_code_textedit->setPlainText(code);
}
