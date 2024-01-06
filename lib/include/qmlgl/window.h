// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef WINDOW_H
#define WINDOW_H

#include "dllexportimport.h"

#include "scene.h"

#include <QQuickWindow>

#include <QColor>
#include <QList>
#include <QQmlError>
#include <QUrl>

class QOpenGLFunctions;
class QQmlComponent;
class QQmlContext;
class QQuickItem;

class Viewport;

/**
 * @brief a replacement for QQuickView that makes it easy to render a GL scene under QML
 *
 * This class is abstract.
 * User should derive from this class and reimplement the createOpenGLScene() function
 * which is called when the OpenGLScene must be created.
 */
class QMLGL_API Window : public QQuickWindow
{
  Q_OBJECT
  Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
  Q_PROPERTY(Status status READ status NOTIFY statusChanged)
  Q_PROPERTY(QColor clearColor READ clearColor WRITE setClearColor NOTIFY clearColorChanged)
public:
  Window();
  ~Window();

  enum Status
  {
    Null,
    Ready,
    Loading,
    Error,
  };
  Q_ENUM(Status)

  Status status() const;

  const QUrl& source() const;
  void setSource(QUrl src);

  QQmlEngine* engine() const;
  QQmlContext* rootContext() const;

  QQuickItem* rootObject() const;

  const QColor& clearColor() const;
  void setClearColor(const QColor& c);

  const std::vector<Viewport*>& viewports() const;

  OpenGLScene* glScene() const;

signals:
  void statusChanged();
  void sourceChanged();
  void clearColorChanged();
  void glSceneDestroyed();

protected Q_SLOTS:
  void onSceneGraphInitialized();
  void onSceneGraphInvalidated();
  void onBeforeSynchronizing();
  void onBeforeRendering();

protected:
  virtual std::unique_ptr<OpenGLScene> createOpenGLScene() = 0;

protected:
  void resizeEvent(QResizeEvent* ev) override;

  void loadUi();
  void destroyUi();

protected:
  friend class Viewport;
  void addViewport(Viewport* v);
  void removeViewport(Viewport* v);

private:
  void onQmlComponentLoadingComplete();
  void resizeRootObjectToView();

private:
  Status m_status = Status::Null;
  QUrl m_source;
  QColor m_clear_color;
  QQmlComponent* m_qml_component = nullptr;
  QList<QQmlError> m_errors;
  QQuickItem* m_root_item = nullptr;
  QQmlEngine* m_qml_engine = nullptr;
  std::vector<Viewport*> m_viewports;
  std::unique_ptr<OpenGLScene> m_gl_scene;
};

#endif // WINDOW_H
