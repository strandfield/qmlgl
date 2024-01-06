
The `qmlgl` library provides a `Window` class for rendering OpenGL under QML.
A scene is defined by deriving from `OpenGLScene`.

## API reference

### Camera

Defined in header `camera.h`.

A class that defines a camera for rendering a OpenGL scene.

It defines a position, forward vector, up vector, right vector and a view frustum.

This class is derived from `QObject` and usable in QML.

### CameraController

Defined in header `cameracontroller.h`.

An abstract base class allowing the user to control a `Camera` with the keyboard or
mouse.

### OpenGLScene

Defined in header `scene.h`.

An abstract base class for an OpenGL scene.

It must be subclassed and the following two methods must be implemented.

- `synchronize()`: synchronizes the scene with the application (accessing objects in the main thread is safe here)
- `render()`: actually renders the OpenGL scene

### ViewFrustum

Defined in header `viewfrustum.h`.

Defines a view frustum for rendering a OpenGL scene.

Stores whether the frustum is orthogonal or uses perspective, the near and far planes, the aspect ratio 
and vertical angle.

This class is derived from `QObject` and usable in QML.

### Viewport

Defined in header `viewport.h`.

Defines a viewport in which the OpenGL scene is to be rendered.

Contains a camera.

This allows the `OpenGLScene` to be rendered multiple times in the `Window` (with different cameras).

This class is derived from `QQuickItem`.

### Window

Defined in header `window.h`.

A replacement for `QQuickView` that allows rendering a QML scene on top of an 
OpenGL scene.

This is an abstract class that must be derived in order to provide an implementation 
for the `createOpenGLScene()` method.

This class is derived from `QQuickWindow`.
