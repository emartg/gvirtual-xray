# Notice

This repository is a downstream copy of gVirtualXRay, by Franck P. Vidal and the gVirtualXRay contributors. It is not the
upstream project and is not endorsed by its authors. This file states what it is based on and how it differs.

## The Upstream Base

| | |
|---|---|
| Project | gVirtualXRay (gVXR), [https://sourceforge.net/projects/gvirtualxray/](https://sourceforge.net/projects/gvirtualxray/) |
| Version | 2.1.0, upstream's tag `v2.1.0` (commit `53e5d7b`, 2026-06-09) |
| Imported as | `cee4003` (2026-09-23), the files of `v2.1.0` with Windows line endings |
| Copyright | Copyright (c) 2019, Dr Franck P. Vidal, as stated in [LICENSE](LICENSE); the authors are listed in [CONTRIBUTORS](CONTRIBUTORS) |
| License | BSD 3-Clause License ([LICENSE](LICENSE)), unchanged; this repository's own changes are under the same license |

The copyright notices of the upstream authors in the source files are kept as they are. Upstream gVirtualXRay has moved on
since 2.1.0; this repository does not follow it, and none of its later changes are included.

## The Differences

The X-ray computation and the physics are those of 2.1.0. The differences are listed by area; [CHANGELOG.md](CHANGELOG.md)
gives the details of each change.

### Build

- The build works with MinGW-w64 GCC on Windows: Windows is detected with `_WIN32` in the generated configuration header,
  the libraries are selected by their MinGW names (`libX.a`), circular static dependencies are linked in a group, the
  resource headers are written in one operation, the bundled Assimp does not treat its warnings as errors, and the xraylib
  DLL is copied beside the programs and installed. A small header, `mingw_fix.h`, is force-included with MinGW only.
- The core library can be used from another CMake project with `add_subdirectory`: the target `gVirtualXRay::gVirtualXRay`
  carries its include directories, preprocessor definitions, and C++ standard, and the tests and the installation rules
  are left out when the repository is not the top-level project (the option `gVXR_INSTALL`).
- The minimum version of CMake is 3.26; the library is compiled as C++23.
- The coverage flags of the Debug configuration are an option, `gVXR_COVERAGE`, off by default.
- Boost is found without its header-only component; zlib is downloaded from addresses that keep its version.
- A project can choose a shared GLEW (`GLEW_USE_STATIC_LIBS` off); the static GLEW stays the default.
- Vulkan is looked for only when GLFW is used, because its types come from the GLFW header.
- Git LFS is not needed: the two large reference files of `TestSinogramFromMesh` are downloaded from the release
  `test-data-1` of this repository, and the other test data are plain files.

### Library

- `Context::adoptCurrentContext()` and `Context::isExternal()` let an application that owns its OpenGL context use the
  library on it; `release()` and the destructor of such a context free the objects of the library but destroy no window,
  display, or context. Without a call to them, nothing changes.
- The widget spheres of `XRayDetector` are built with 3 subdivisions instead of 10, so that creating a detector takes
  under a millisecond instead of more than a second.
- `initialiseGLEW()` and `initializeGLEW()` take a verbose level, 0 by default, so that the line `Initialise GLEW` is no
  longer printed at each context.
- The logger writes to the console as well as to its file.
- On Windows, a context is requested from GLFW and OpenGL only; GLX and EGL stay the fallbacks on the other platforms.

### SimpleGVXR and Its Demos

- A window title can be set (`setWindowTitle`); `DemoDragon` names its three windows after their precision.
- Closing a window stops the render loop instead of opening the window again.
- `DemoConvolutionKernel` reports where it writes its results and returns a non-zero exit code when it fails.

### Tests and Continuous Integration

- The tests are labelled `cpu` (no OpenGL context needed) and `opengl`.
- New tests: `TestAdoptCurrentContext` (an application's context), `TestDetectorResize` (a renderer initialised again for
  a new number of pixels), and the consumer project (`gvxr/tests/consumer`, the library used with `add_subdirectory`).
- A continuous integration on GitHub Actions builds and tests the library on Linux (GCC, with the OpenGL tests on a virtual
  display) and Windows (MinGW GCC, the tests that need no OpenGL context), and builds the consumer project with the system packages of Linux and of MSYS2.

### Documentation

- The documentation of upstream gVirtualXRay is replaced by this repository's own: this file, the [README](README.md), and
  the documents of [docs/](docs/), with the credits in [docs/sources.md](docs/sources.md) and the diagrams in
  `docs/diagrams/`.

### What Was Removed

This repository keeps the C++ core library, SimpleGVXR, their tests and demos, and the Welsh dragon example. It does not
provide:

- the wrappers of SimpleGVXR (Python, R, Ruby, Tcl, C#, Java, Perl, and GNU Octave) and the Python package;
- the Python scripts and the wxPython interface (`gvxrGUI`);
- the Conan, Docker, Read the Docs, and CDash files;
- the prebuilt xraylib for macOS and for Linux on 64-bit ARM (the platforms of this repository are Windows and Linux on
  x86_64);
- the second copy of the test data in `SimpleGVXR/tests/data/`;
- the documentation of upstream gVirtualXRay (its documents, tutorials, posters, screenshots, and Doxygen
  configuration), its logos, and its code of conduct;
- the packaging with CPack.

## Reporting Problems

Problems of this repository go to its [issues](https://github.com/emartg/gvirtual-xray/issues); problems that are also in
upstream gVirtualXRay go to the [upstream project](https://sourceforge.net/projects/gvirtualxray/).
