# Using the Library From an Application

How an application builds the core library of this repository with its own CMake project, lets it compute X-ray images on
the application's own OpenGL context, and what each step costs. The model is the consumer project,
[`gvxr/tests/consumer`](../gvxr/tests/consumer/CMakeLists.txt), which the continuous integration builds on Linux and
Windows. The diagrams of the [computation of an image](diagrams/sequence/compute_image.png) and of the
[core classes](diagrams/uml/core_classes.png) show the same in more detail.

## The CMake Project

Add the repository (for example as a Git submodule) with `add_subdirectory`, with the core library alone: no SimpleGVXR, no
GLFW (the application provides the context), and GLEW to load the OpenGL functions.

```cmake
set(BUILD_SIMPLEGVXR OFF)
set(USE_GLFW OFF)
set(USE_GLEW ON)

add_subdirectory(external/gvirtual-xray gvirtual-xray)

add_library(xray_adapter STATIC xray_adapter.cpp)
target_link_libraries(xray_adapter PRIVATE gVirtualXRay::gVirtualXRay)
```

- **What the target carries.** `gVirtualXRay::gVirtualXRay` gives the files that link to it the include directories, the
  preprocessor definitions (`HAS_GLEW`, `HAS_OPENGL`, `GLEW_STATIC` with the static GLEW, and the others of the
  configuration), and the requirement of C++23 (`cxx_std_23`), because the public headers include the headers of its
  dependencies and test those definitions.
- **C++23 for the files that include its headers.** The requirement is public: a target that links to the library is
  compiled as C++23. An application that stays at an older standard keeps the files that include the headers of the library
  in one library of its own (an adapter, as above) and links it privately.
- **What is left out.** When the repository is not the top-level project, its tests, its installation rules, and its
  packaging are not generated (`BUILD_TESTING` is off inside it, and `gVXR_INSTALL` defaults to off), and the build type is
  the application's.
- **The dependencies.** Either the system packages (`USE_SYSTEM_GLEW`, `USE_SYSTEM_ASSIMP`, `USE_SYSTEM_ZLIB`,
  `USE_SYSTEM_LIBTIFF`, `USE_SYSTEM_FREETYPE`, `USE_SYSTEM_BOOST`), or the ones that the superbuild builds, which are installed
  in `${CMAKE_INSTALL_PREFIX}/third_party` at the configuration (so the install prefix must be writable). See
  [build_instructions.md](build_instructions.md#dependencies).
- **GLEW.** The static GLEW is the default. With a shared GLEW (a system package, or the standard vcpkg triplets), set
  `GLEW_USE_STATIC_LIBS` to `OFF` before `add_subdirectory`: the target then links `GLEW::GLEW` and does not define
  `GLEW_STATIC`.
- **Do not enable `USE_GLAD`** in an application that has its own OpenGL loader: the bundled glad (version 2) clashes with
  another one, such as glad version 1.
- **xraylib on Windows** is a DLL: copy it beside the executable after the build, as the consumer project does with the
  variable `XRAYLIB_SHARED_LIB_PATH`:

```cmake
if(WIN32 AND XRAYLIB_SHARED_LIB_PATH)
    add_custom_command(TARGET my_application POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${XRAYLIB_SHARED_LIB_PATH} $<TARGET_FILE_DIR:my_application>)
endif()
```

## The OpenGL Context of the Application

The library can create its own context (with GLFW, EGL, or GLX), but an application that already has one, for its own
window, lets the library use it with `Context::adoptCurrentContext()`:

```cpp
#include "gVirtualXRay/gVirtualXRayConfig.h"
#include "gVirtualXRay/Context.h"
#include "gVirtualXRay/ResourceManager.h"

using namespace gVirtualXRay;

Context g_xray_context;

// The application's context is current on this thread
ResourceManager::getInstance().setGraphicsContext(&g_xray_context);
if (!g_xray_context.adoptCurrentContext())
{
    // no current context, or a version older than 3.2
}
```

- **The configuration header first.** `gVirtualXRay/gVirtualXRayConfig.h` (generated at the configuration) includes the
  headers of GLEW and OpenGL that the other headers of the library use without including them, so it comes before them in
  every file, as in the tests of the repository. Without it, GCC stops at `glGetUniformLocation` in `Image.inl`.
- **What it does.** It creates nothing: it releases what the library held, initialises GLEW on the current context, checks
  that its version is 3.2 or newer, and marks the object as ready. `isExternal()` then returns `true`. The OpenGL loader of the
  application (glad, for example) is not touched, and GLEW prints nothing (its verbose level is 0 by default).
- **Releasing.** `release()` and the destructor of an adopted context free the objects of the library (shaders, buffers,
  framebuffers) and destroy no window, display, or context. They make OpenGL calls, so release the renderer and the context of
  the library while the application's context is still current, before the application destroys it.
- **The OpenGL state.** The library changes the state of the context while it initialises and computes: framebuffer
  bindings, the current program, blending, and the depth test. It restores the viewport. An application that draws with the
  same context sets again the state that it relies on after a computation (or saves and restores it around the call).
- **One thread.** The calls of the library are made on the thread where the context is current.

## Computing an Image

A scene is a tree of `SceneGraphNode` objects, each with a polygon mesh and a material. The beam and the detector set the
geometry, and the renderer computes the image:

```cpp
#include "gVirtualXRay/gVirtualXRayConfig.h"
#include "gVirtualXRay/CubeMesh.h"
#include "gVirtualXRay/CylinderMesh.h"
#include "gVirtualXRay/SceneGraphNode.h"
#include "gVirtualXRay/Units.h"
#include "gVirtualXRay/XRayBeam.h"
#include "gVirtualXRay/XRayDetector.h"
#include "gVirtualXRay/XRayRenderer.h"

SceneGraphNode scene_graph("root", nullptr);
scene_graph.addChild("inside", CylinderMesh<GLdouble>(20, 3.0 * cm, 1.0 * cm));
scene_graph.addChild("outside", CubeMesh<GLdouble>(3.0 * cm));
scene_graph.getChild("inside").getPolygonMesh().setHounsfieldValue(1329.885683);
scene_graph.getChild("outside").getPolygonMesh().setHounsfieldValue(51.714752);

XRayBeam beam;
beam.initialise(80.0 * keV);

XRayDetector detector;
detector.setResolutionInUnitsOfLengthPerPixel(0.3 * mm);
detector.setNumberOfPixels(Vec2ui(301, 1));
detector.setDetectorPosition(VEC3(0.0, 0.0, 10.0 * cm));
detector.setUpVector(VEC3(0.0, 1.0, 0.0));
detector.setXrayPointSource(VEC3(0.0, 0.0, -10.0 * cm));
detector.setParallelBeam();

XRayRenderer renderer;
renderer.initialise(XRayRenderer::OPENGL, 255, &detector, &beam);
renderer.addInnerSurface(&scene_graph.getChild("inside"));
renderer.addOuterSurface(&scene_graph.getChild("outside"));

Matrix4x4<GLfloat> identity;
renderer.computeImage(identity);
const RATIONAL_NUMBER* energy_fluence = renderer.getEnergyFluence().getRawData();
```

This is the scene of the test `TestAdoptCurrentContext` (a cube of soft tissue with a cylinder of bone, at 80 keV), which
checks the result against the Beer-Lambert law.

- **What is read at every computation.** `computeImage()` reads the source and detector positions, the up vector, the pixel
  size, the kind of beam (point or parallel), the number of source samples, the spectrum, the transformations of the nodes,
  and the materials (after `needUpdateSurfacePerMaterial()`), so changing them needs no other call. This was checked in 14
  cases, each giving the image of a new renderer.
- **What is sized by `initialise()`.** The number of pixels of the detector sizes the framebuffers and the images. After
  `XRayDetector::setNumberOfPixels()`, call `initialise()` again on the same renderer: the inner surfaces, their
  transformations, and their materials are kept, and the beam and the detector stay as they were set, but the outer surface
  must be added again (the test `TestDetectorResize`). The number of energy channels is resized on its own at the next
  computation.

## What It Costs

Measured warm on a GeForce GTX 1660 SUPER, with a detector of 256 by 128 pixels, one energy channel, and four meshes
(Release; Debug in parentheses):

| Step | Time |
|---|---|
| Constructing an `XRayDetector` | 0.04 ms (0.2 ms) |
| `XRayRenderer::initialise()` | 4.3 ms (8.3 ms); about 20 ms for 512 by 512 pixels |
| `computeImage()` | 3.3 ms (median 9 ms) |
| Reading the image back | 0.15 ms (0.5 ms) |

`initialise()` compiles the shader programs and creates the framebuffers, so it belongs to the set-up of a scene or a change
of detector size, not to every frame; `computeImage()` can follow every change of the scene.
