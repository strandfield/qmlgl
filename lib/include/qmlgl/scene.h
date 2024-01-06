// Copyright (C) 2024 Vincent Chambrin
// This file is part of the 'qmlgl' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef SCENE_H
#define SCENE_H

#include "dllexportimport.h"

class Window;

/**
 * @brief abstract base class for an OpenGL scene
 *
 * This abstract base class provides a synchronize() function
 * for synchronizing the scene with the application and
 * a render() function for actually rendering the scene.
 */
class QMLGL_API OpenGLScene
{
public:
  virtual ~OpenGLScene();

  /**
   * @brief synchronizes the scene with the application
   * @param window  pointer to the window in which the scene will be rendered
   *
   * This function is called in the render thread
   * (which may be the main thread, but is likely another thread).
   * The main thread is blocked during the execution of this
   * function allowing you to access objects living in the main
   * thread without producing data races.
   */
  virtual void synchronize(Window* window) = 0;

  /**
   * @brief renders the OpenGL scene
   * @param window  pointer to the window in which the scene is rendered
   *
   * This function is called in the render thread (which may be the main thread).
   * In case of threaded rendering, the main thread is NOT blocked during
   * the execution of this function making it generally unsafe to access objects
   * living in the main thread.
   * Ideally, the scene should have (by now) all the information necessary to render
   * the scene so that this function renders without accessing data in any other thread.
   */
  virtual void render(Window* window) = 0;
};

#endif // SCENE_H
