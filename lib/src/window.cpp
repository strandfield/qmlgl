#include "window.h"

#include "init.h"
#include "scene.h"
#include "viewport.h"

#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QSurfaceFormat>

#include <QQmlComponent>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickItem>

#include <QRunnable>

#include <algorithm>

Window::Window()
  : m_clear_color(51, 51, 51)
{
  // we are rendering OpenGL so we need OpenGLSurface.
  setSurfaceType(QSurface::OpenGLSurface);

  QSurfaceFormat format;
  format.setMajorVersion(4);
  format.setMinorVersion(3);
  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);
  format.setSamples(4);
  setFormat(format);

  // since we are drawing our OpenGL under the QML, we don't want the window
  // to clear itself before it renders the QML
  // (that would erase our GL scene).
  // we would do the clear manually before rendering OpenGL.
  setClearBeforeRendering(false);

  // warning: calling create() will trigger a QResizeEvent
  create();

  m_qml_engine = new QQmlEngine(this);

  if (!m_qml_engine->incubationController())
  {
    m_qml_engine->setIncubationController(this->incubationController());
  }

  qmlgl::init();

  connect(this, &QQuickWindow::sceneGraphInitialized, this, &Window::onSceneGraphInitialized, Qt::DirectConnection);
  connect(this, &QQuickWindow::sceneGraphInvalidated, this, &Window::onSceneGraphInvalidated, Qt::DirectConnection);
  connect(this, &QQuickWindow::beforeSynchronizing, this, &Window::onBeforeSynchronizing, Qt::DirectConnection);
  connect(this, &QQuickWindow::beforeRendering, this, &Window::onBeforeRendering, Qt::DirectConnection);
}

Window::~Window()
{
  destroyUi();

  if (m_gl_scene)
  {
    // Cannot destroy the GL scene here as OpenGL context may
    // live in a different thread.

    struct DeleteSceneJob : QRunnable
    {
      OpenGLScene* m_scene;
      explicit DeleteSceneJob(OpenGLScene* s) : m_scene(s) { }
      void run() override { delete m_scene; }
    };
    scheduleRenderJob(new DeleteSceneJob(m_gl_scene.release()), BeforeSynchronizingStage);
  }

  disconnect(this, &QQuickWindow::sceneGraphInitialized, this, &Window::onSceneGraphInitialized);
  disconnect(this, &QQuickWindow::sceneGraphInvalidated, this, &Window::onSceneGraphInvalidated);
  disconnect(this, &QQuickWindow::beforeSynchronizing, this, &Window::onBeforeSynchronizing);
  disconnect(this, &QQuickWindow::beforeRendering, this, &Window::onBeforeRendering);
}

Window::Status Window::status() const
{
  return m_status;
}

const QUrl& Window::source() const
{
  return m_source;
}

void Window::setSource(QUrl src)
{
  if (m_source != src)
  {
    m_source = src;
    Q_EMIT sourceChanged();
    loadUi();
  }
}

QQmlEngine* Window::engine() const
{
    return m_qml_engine;
}

QQmlContext* Window::rootContext() const
{
  return engine()->rootContext();
}

const std::vector<Viewport*>& Window::viewports() const
{
  return m_viewports;
}

void Window::onQmlComponentLoadingComplete()
{
  if (m_qml_component->isLoading())
  {
    return;
  }

  if (m_qml_component->isError())
  {
     m_errors = m_qml_component->errors();

    for (const QQmlError& error : m_errors)
    {
      qDebug() << error.url() << error.line() << error;
    }

    m_status = Status::Error;
    emit statusChanged();

    qFatal("Unable to load QML file");
  }

  Q_ASSERT(m_qml_component->isReady());

  QObject* rootObject = m_qml_component->create();
  m_root_item = qobject_cast<QQuickItem*>(rootObject);

  if (!m_root_item)
  {
    qFatal("Did not load a Qt Quick scene");
  }

  m_root_item->setParentItem(this->contentItem());

  m_status = Status::Ready;
  emit statusChanged();

  m_qml_component->deleteLater();
  m_qml_component = nullptr;
}

void Window::resizeRootObjectToView()
{
  if (m_root_item)
  {
    m_root_item->setWidth(width());
    m_root_item->setHeight(height());
  }
}

void Window::loadUi()
{
  if (m_root_item)
  {
    m_root_item->deleteLater();
    m_root_item = nullptr;
  }

  if (source() == QUrl())
  {
    m_status = Status::Null;
    emit statusChanged();
    return;
  }

  m_status = Status::Loading;
  emit statusChanged();

  m_qml_component = new QQmlComponent(engine(), this);

  connect(m_qml_component, &QQmlComponent::statusChanged, this, &Window::onQmlComponentLoadingComplete);

  m_qml_component->loadUrl(source());
}

void Window::destroyUi()
{
  if (m_status != Status::Null)
  {
    m_status = Status::Null;
    Q_EMIT statusChanged();
  }

  if (m_root_item)
  {
    delete m_root_item;
    m_root_item = nullptr;
  }
  
  if (m_qml_component)
  {
    delete m_qml_component;
    m_qml_component = nullptr;
  }
}

void Window::addViewport(Viewport* v)
{
  m_viewports.push_back(v);

  update();
}

void Window::removeViewport(Viewport* v)
{
  auto it = std::find(m_viewports.begin(), m_viewports.end(), v);

  if (it != m_viewports.end())
  {
    m_viewports.erase(it);

    update();
  }
}

QQuickItem* Window::rootObject() const
{
  return m_root_item;
}

const QColor& Window::clearColor() const
{
  return m_clear_color;
}

void Window::setClearColor(const QColor& c)
{
  if (m_clear_color != c)
  {
    m_clear_color = c;
    Q_EMIT clearColorChanged();
    update();
  }
}

OpenGLScene* Window::glScene() const
{
  return m_gl_scene.get();
}

void Window::onSceneGraphInitialized()
{
  m_gl_scene = createOpenGLScene();
}

void Window::onSceneGraphInvalidated()
{
  if (m_gl_scene)
  {
    m_gl_scene.reset();
    Q_EMIT glSceneDestroyed();
  }
}

void Window::onBeforeSynchronizing()
{
  if (glScene())
  {
    glScene()->synchronize(this);
  }
}

void Window::onBeforeRendering()
{
  if (glScene())
  {
    glScene()->render(this);
  }
}

void Window::resizeEvent(QResizeEvent* ev)
{
  QQuickWindow::resizeEvent(ev);
  resizeRootObjectToView();
}
