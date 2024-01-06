
# `qmlgl` - Rendering a 3D OpenGL scene in a QML application

Short summary: the technique used here for rendering OpenGL under QML
is inspired by [Scene Graph - OpenGL Under QML](https://doc.qt.io/qt-5/qtquick-scenegraph-openglunderqml-example.html)
from the Qt examples; the difference is that the OpenGL scene is not associated with a 
`QQuickItem` in particular but is directly part of the `QQuickWindow`.

**Rendering OpenGL in a QtQuick application**

Before getting started, 
reading the [Qt Quick Scene Graph](https://doc.qt.io/qt-5/qtquick-visualcanvas-scenegraph.html) documentation 
is recommended for understanding the limitations of the various techniques presented here.

Qt provides several alternatives for manually rendering OpenGL in a Qt Quick application.

If you want to render OpenGL under the QML, the above example seems to show the way to go.

The process is as follow:
- make a `Qt::DirectConnection` to `QQuickWindow::beforeSynchronizing()` and `QQuickWindow::beforeRendering()`
- synchronize your OpenGL scene with the application in the slot connected to `beforeSynchronizing()`
- issue the OpenGL commands for the rendering in the slot connected to `beforeRendering()`.

Care must be taken as since we are using a direct connection, the slots are executed in the 
render thread, which is not be the main thread (unless single-threaded rendering is used).
Qt blocks the main thread while synchronizing so it is safe to access objects at this 
step, but it is generally unsafe to do so while rendering. 

If you want to render OpenGL as part of your QML interface, there are several possibilities.

One of them is showcased in the [Scene Graph - Rendering FBOs](https://doc.qt.io/qt-5/qtquick-scenegraph-fboitem-example.html) example, 
which relies on the `QQuickFramebufferObject` class (a `QQuickItem`).
The idea here is to render the OpenGL into a framebuffer object (FBO) and to 
let the item display it as part of the QML scene.

You may also use a `QSGImageNode` to display a texture. 
This is a scenegraph node that can be created using `QQuickWindow::createImageNode()`.
In order to use this technique, you will need to create your own `QQuickItem` and return 
the `QSGImageNode` from the `QQuickItem::updatePaintNode()` method.
Well, at least that's how I understand it. 
This technique is not very well documented and seems to offer less flexibility.

Alternatively, you may use a `QSGRenderNode` which will allow you to issue arbitrary 
OpenGL commands for rendering your `QQuickItem`.

In any case, whenever you are using OpenGL code while the QML scene graph is being rendered 
(e.g. with `QQuickFramebufferObject` or `QSGRenderNode`), care must be taken not 
to mess too much with the OpenGL context. 
It should be handed back in the state it was handed in otherwise rendering of the rest 
of the QML scene graph may get broken.

Rendering OpenGL under the QML is less risky as the QML scene graph is more likely to put
the OpenGL context back in a proper state before rendering its part.

**Use case covered by this project**

The code provided here may suit your need if:
- your code is primarily OpenGL and you want to design a user interface on top of it with QML
- you cannot or do not want to use `QQuickFramebufferObject` or `QSGRenderNode`
- you do not want the scene to associated with a `QQuickItem` (as done in [Scene Graph - OpenGL Under QML](https://doc.qt.io/qt-5/qtquick-scenegraph-openglunderqml-example.html))

In other words, this project is aimed at people who have OpenGL and want to put 
QML on top rather than people who have a QML application and want to add some 
OpenGL to it.

**My use case**

If time allows it, I will use this project for my experiments with OpenGL while 
reading the tutorials on https://learnopengl.com/.

## What's in this repository ?

This repository contains code to render an OpenGL scene under QML in a QtQuick application.

Library code is located in `lib`. The examples are in `apps` and rely on code 
in `appcommon`.

### the `qmlgl` library (in `lib`)

This library provides a `Window` class which is a replacement for `QQuickView`
suitable for rendering OpenGL under QML.

Please consult the README in the [lib](lib) folder for more information.

### the `appcommon` library (in `appcommon`)

A library built on top of `qmlgl` that provides useful classes for building 
a Qt Quick application with OpenGL under QML.

It is a common library used by the examples.

### the examples

More details may be available for each example in their respective folders.

**tetrahedron**

A basic OpenGL scene displaying a tetrahedron.

![Screenshot of the tetrahedron example app](apps/tetrahedron/doc/tetrahedron.png)
