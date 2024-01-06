// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "qmlgl/window.h"

#include <memory>

/**
 * @brief abstract base class for a factory that constructs OpenGLScene
 */
class OpenGLSceneFactory
{
public:
  virtual ~OpenGLSceneFactory();

  virtual std::unique_ptr<OpenGLScene> create(QOpenGLContext* context) = 0;

  /**
   * @brief creates a factory for a specific OpenGLScene type
   * @tparam T  the OpenGLScene-derived type
   *
   * The template type argument T must be derived from OpenGLScene and
   * have a constructor that accepts a QOpenGLContext* as parameter.
   */
  template<typename T>
  static std::unique_ptr<OpenGLSceneFactory> factoryFor()
  {
    struct MyFactory : OpenGLSceneFactory
    {
      std::unique_ptr<OpenGLScene> create(QOpenGLContext* context) override
      {
        return std::unique_ptr<OpenGLScene>(new T(context));
      }
    };

    return std::unique_ptr<OpenGLSceneFactory>(new MyFactory);
  }

  template<typename F>
  static std::unique_ptr<OpenGLSceneFactory> makeFactory(F&& fun);
};

template<typename Callable>
class GenericOpenGLSceneFactory : public OpenGLSceneFactory
{
private:
  Callable m_callable;

public:
  explicit GenericOpenGLSceneFactory(Callable c) : m_callable(std::move(c))
  {

  }

  ~GenericOpenGLSceneFactory() = default;

  std::unique_ptr<OpenGLScene> create(QOpenGLContext* context) override
  {
    return m_callable(context);
  }
};

/**
 * @brief creates a scene factory using any callable object
 * @param fun
 *
 * The callable object @a fun must accept as parameter a pointer to an OpenGLContext
 * and return a unique pointer to an OpenGLScene.
 *
 * @code[cpp]
 * std::unique_ptr<OpenGLSceneFactory> myfun(QOpenGLContext* glContext);
 * @endcode
 */
template<typename F>
inline std::unique_ptr<OpenGLSceneFactory> OpenGLSceneFactory::makeFactory(F&& fun)
{
  return new GenericOpenGLSceneFactory<F>(std::forward<F>(fun));
}

/**
 * @brief an application window that renders a GL scene under QML
 */
class AppWindow : public Window
{
public:
  explicit AppWindow(std::unique_ptr<OpenGLSceneFactory> factory);

  /**
   * @brief specifies the behavior of exposeQObjectToQml()
   */
  enum ExposeOptions
  {
    ExposeObjectOnly,          ///< only the QObject is exposed to QML using its objectName()
    ExposeChildrenRecursively, ///< the QObject and its named children are exposed to QML
  };

  void exposeQObjectToQml(QObject* obj, ExposeOptions opts = ExposeObjectOnly);
  void exposeQObjectToQml(QObject* obj, const QString& name);

protected:
  std::unique_ptr<OpenGLScene> createOpenGLScene() override;

private:
  std::unique_ptr<OpenGLSceneFactory> m_factory;
};
