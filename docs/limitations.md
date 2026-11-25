# Limitations

What this repository does not support or does not provide, and the known issues of its current state. The build
instructions are in [build_instructions.md](build_instructions.md); the differences from upstream gVirtualXRay are in
[NOTICE.md](../NOTICE.md).

## Platforms and Compilers

- **Supported:** Windows with MinGW-w64 GCC, and Linux on x86_64 with GCC. Both are built and tested by the continuous
  integration.
- **MSVC is not supported.** The library compiles with MSVC, but the tests stop at a duplicate zlib symbol when they link.
- **Clang has not been tried.**
- **macOS and Linux on ARM are not supported:** the prebuilt xraylib of the repository is for Windows and Linux on x86_64 only,
  and the configuration stops on another platform with a message that says so.
- **CMake 3.26 or newer** is required, and the library is compiled as C++23.

## What Is Not Provided

- **No wrappers and no Python package:** the wrappers of SimpleGVXR (Python, R, Ruby, Tcl, C#, Java, Perl, and GNU Octave),
  the Python package of upstream gVirtualXRay, and its Python scripts and graphical interface are not part of this
  repository. It is for C++ applications.
- **No binary packages:** the library is built from source.
- **No published API reference:** the public headers carry the comments of each class and function.

## Known Issues

### The installed CMake package files do not work

**Effect.** After `cmake --install`, another project cannot use the library through `find_package(gVirtualXRay)` or
`find_package(SimpleGVXR)`: its configuration or its build stops. The Welsh dragon example of `SimpleGVXR-examples/`, which is
built that way, cannot be configured against an installation.

**Cause.** The package files are upstream's, written as variables rather than exported targets, and they do not describe the
installation: `gVirtualXRay_USE_FILE` is empty, the libraries and most include directories point to the build and source
folders, the imported targets that the link line names (`OpenGL::GLX`, `OpenMP::OpenMP_CXX`) are never created, and some
headers that the public headers include (those of SimpleGVXR other than `SimpleGVXR.h`, xraylib, XrayPhysics, and the
downloaded Boost) are not installed.

**Workaround.** Use the library with `add_subdirectory` and the target `gVirtualXRay::gVirtualXRay`
([integration.md](integration.md)); the consumer project of the continuous integration does it. To try SimpleGVXR by hand, use
its demos ([build_instructions.md](build_instructions.md#simplegvxr-and-its-demos)).

### `SGVXR_TestAcquisitionFile` fails at its cases `BHPI` and `distances`

**Effect.** On Windows with a graphics card, two cases of a SimpleGVXR test fail. In `BHPI`, the beam hardening index rises
between two thicknesses of aluminium where the test expects it to decrease (between 1.2 and 1.3 mm, from 0.4135 to 0.4150, in
the run of 2026-11-25; between 1.1 and 1.2 mm in an earlier run). In `distances`, the object-to-detector distance is not within
0.1 mm of the expected 1 mm. Under Mesa on Linux, the test passed in one run and failed at `distances` in another.

**Cause.** The physics of upstream gVirtualXRay 2.1.0, unchanged here; not investigated further.

**Workaround.** None needed to use the library; the core library's tests are not affected, and the continuous integration does
not build SimpleGVXR.

### Two SimpleGVXR tests fail under Mesa's software renderer

**Effect.** On Linux under Xvfb with Mesa 26.0.8 (llvmpipe), `SGVXR_TestBeerLambertLawMonoChromatic` and `SGVXR_TestIsoSurface`
fail. Neither reads a data file.

**Cause.** Not established; they fail in the same way on the previous state of the repository, so they come from the
environment (the software renderer), not from a change of the code.

**Workaround.** Run the SimpleGVXR tests on a graphics card.

### Two core tests need a graphics card

**Effect.** On Linux under Xvfb, `TestOpenGLContext` fails (its compute shader check reads back 0 where the shader wrote -1000)
and `TestCubeSource` takes about 5 minutes. The continuous integration leaves them out.

**Workaround.** Run them on a graphics card, where they pass.
