# gVirtualXRay (Downstream Copy)

[![Language](https://img.shields.io/badge/C%2B%2B-23-blue.svg)](https://isocpp.org/)
[![OpenGL](https://img.shields.io/badge/OpenGL-%E2%89%A53.2-red.svg)](https://www.opengl.org/)
[![CI](https://github.com/emartg/gvirtual-xray/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/emartg/gvirtual-xray/actions/workflows/ci.yml)
[![CMake](https://img.shields.io/badge/CMake-%E2%89%A53.26-brightgreen.svg)](https://cmake.org/)
[![License: BSD 3-Clause](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](LICENSE)
[![Based on gVirtualXRay 2.1.0](https://img.shields.io/badge/based%20on-gVirtualXRay%202.1.0-informational.svg)](https://sourceforge.net/projects/gvirtualxray/)

> **Notice.** This repository is a downstream copy of [gVirtualXRay](https://sourceforge.net/projects/gvirtualxray/) 2.1.0,
> by Franck P. Vidal and the gVirtualXRay contributors. It is not the upstream project and is not endorsed by its authors.
> It keeps the C++ library and its tests, with build fixes, a continuous integration, and a few changes for applications
> that use the library on their own OpenGL context; the parts that it does not need were removed. It is distributed under the BSD 3-Clause
> License of upstream gVirtualXRay ([LICENSE](LICENSE)). [NOTICE.md](NOTICE.md) states the base version and every difference.

## What It Does

gVirtualXRay computes X-ray images on the graphics card with OpenGL. A scene is made of polygon meshes, each with a
material (an element, a mixture, or a compound, with its density); an X-ray source (a point or a parallel beam, with a
monochromatic or a polychromatic spectrum) and a detector (its position, orientation, number of pixels, and pixel size) set
the geometry. For every pixel, the library measures the length of the path of the ray through each mesh on the GPU and
applies the Beer-Lambert law with the attenuation coefficients of the materials (from xraylib), energy by energy. The result
is the image that the detector records: 3.3 ms for a detector of 256 by 128 pixels and four meshes on a GeForce GTX 1660
SUPER, once the renderer is set up.

The library also offers the detector's energy response, scintillators, photon noise, the spectra of X-ray tubes (with
XrayPhysics), CT acquisitions (projections and sinograms), and the reading and writing of images (MetaImage, TIFF, and
others).

## Repository Structure

- `gvxr/`: the core library: its headers (`include/`), sources (`src/`), shaders and font (`resources/`), build modules
  (`cmake/`), bundled third-party code (`dependencies/`: the prebuilt xraylib for Windows and Linux on x86_64, XrayPhysics,
  and glad), and its tests (`tests/`, with their data and the consumer project, a standalone CMake project that uses the
  library with `add_subdirectory`).
- `SimpleGVXR/`: a simpler C++ interface to the core library (free functions, with a window when one is wanted), with its
  demos and tests.
- `SimpleGVXR-examples/`: the Welsh dragon example (see [docs/limitations.md](docs/limitations.md)).
- `cmake/`: the modules of the superbuild (the dependencies that it builds, and the download of the test data).
- `docs/`: the documentation (see [Documentation](#documentation)) and the diagrams (`docs/diagrams/`).
- `.github/workflows/`: the continuous integration (`ci.yml`) and the check of the diagrams (`diagrams.yml`).
- `CHANGELOG.md`, `NOTICE.md`, `LICENSE`, `CONTRIBUTORS`: the changes, the differences from upstream, the license, and the
  authors.

The folders `doc/`, `gvxr/doc/`, and `screenshots/`, and the logos, are the documentation of upstream gVirtualXRay, which
this repository does not maintain; they are removed in a later change.

## Getting the Sources

```
git clone https://github.com/emartg/gvirtual-xray.git
```

Git LFS is not needed. The two large reference files of one test are not in the repository: the first configuration with the
tests downloads them (7.9 MB) from the release [`test-data-1`](https://github.com/emartg/gvirtual-xray/releases/tag/test-data-1).

## Status

The library builds and passes its tests on Windows (MinGW-w64 GCC) and on Linux x86_64 (GCC), in the continuous integration
and on a desktop with a graphics card. Its X-ray computation and its physics are those of upstream gVirtualXRay 2.1.0; the
changes of this repository are listed in [NOTICE.md](NOTICE.md). MSVC, Clang, and macOS are not supported yet, and the
installed CMake package files do not work: see [docs/limitations.md](docs/limitations.md).

## Building and Testing in Short

The top-level `CMakeLists.txt` is a superbuild: it downloads and builds the dependencies that are not installed. On Linux:

```
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="$PWD/build/install" \
    -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DBUILD_SIMPLEGVXR=OFF
cmake --build build --parallel 4
ctest --test-dir build -L cpu --output-on-failure
ctest --test-dir build -L opengl -E "TestOpenGLContext|TestCubeSource" --timeout 900 --output-on-failure
```

The first configuration takes several minutes. The tests labelled `cpu` (8) need no OpenGL context; the ones labelled
`opengl` need a graphics card, or a virtual display with Mesa on Linux. The full instructions, for Windows too, with
SimpleGVXR and the Troubleshooting, are in [docs/build_instructions.md](docs/build_instructions.md).

## Using the Library

An application adds this repository with `add_subdirectory` and links the target `gVirtualXRay::gVirtualXRay`; it can create
its own OpenGL context and let the library use it. [docs/integration.md](docs/integration.md) explains how, with the options,
the context, and what each call costs.

## Documentation

| Document | Content |
|---|---|
| [docs/build_instructions.md](docs/build_instructions.md) | Prerequisites, building on Windows and Linux, the options, SimpleGVXR, the tests, the continuous integration, and the Troubleshooting |
| [docs/integration.md](docs/integration.md) | Using the core library from an application: the CMake setup, an OpenGL context of the application, and the computation of an image |
| [docs/limitations.md](docs/limitations.md) | What this repository does not support or does not provide, and the known issues |
| [docs/sources.md](docs/sources.md) | The credits: the authors, sources, and licenses of the project, its bundled code, its dependencies, and its resources |
| [docs/diagrams/README.md](docs/diagrams/README.md) | The diagrams (the computation of an image, the classes, the continuous integration, the release flow), how they are rendered and checked |
| [NOTICE.md](NOTICE.md) | The upstream base and the differences from it |
| [CHANGELOG.md](CHANGELOG.md) | The changes of this repository, then the releases of upstream gVirtualXRay up to 2.1.0 |

## Contributing and Reporting Problems

One person maintains this repository. Problems with the build, the tests,
or a change of this repository can be reported in its [issues](https://github.com/emartg/gvirtual-xray/issues). Problems that
are also in upstream gVirtualXRay (its physics, its rendering, its other interfaces) belong to the
[upstream project](https://sourceforge.net/projects/gvirtualxray/).

## License and Credits

gVirtualXRay is distributed under the BSD 3-Clause License; the copyright notice and the license are in [LICENSE](LICENSE),
and the authors in [CONTRIBUTORS](CONTRIBUTORS). The third-party code, libraries, and resources are credited, with their
authors, sources, and licenses, in [docs/sources.md](docs/sources.md).
