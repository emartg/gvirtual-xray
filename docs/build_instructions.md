# Build Instructions

How to build gVirtualXRay from this repository, run its tests, and use SimpleGVXR and its demos, on Windows (MinGW-w64 GCC)
and on Linux x86_64 (GCC). What is not supported is in [limitations.md](limitations.md); how an application uses the library
is in [integration.md](integration.md).

## Prerequisites

- **CMake 3.26 or newer** and **Ninja** (Linux) or **MinGW Makefiles** (Windows).
- **A C++23 compiler:** GCC. On Windows, MinGW-w64 GCC (the commands below were run with GCC 16.2, UCRT); on Linux, the
  system GCC (run with GCC 15.2 on Ubuntu 26.04; the continuous integration uses the GCC of Ubuntu 24.04).
- **Git**, and an internet connection for the first configuration (the dependencies and the test data are downloaded).
- **For the tests that need an OpenGL context:** a graphics card with an OpenGL 3.2 driver or newer, or, on Linux without a
  display, a virtual display with Mesa's software renderer (see [Tests](#tests)).
- **On Linux,** the development files of X11 and OpenGL. On Ubuntu:

```
sudo apt-get install build-essential cmake ninja-build libgl-dev libglu1-mesa-dev libegl-dev \
    libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
```

## Getting the Sources

```
git clone https://github.com/emartg/gvirtual-xray.git
cd gvirtual-xray
```

Git LFS is not needed (see [Tests](#tests) for the two files that the configuration downloads).

## Dependencies

The top-level `CMakeLists.txt` is a superbuild. At the first configuration it downloads and builds the dependencies that it is
not told to take from the system: GLEW, GLFW, Assimp, zlib, LibTIFF, FreeType, GoogleTest (for the tests), and the headers of
Boost. They are installed in `<install prefix>/third_party`, so **an install prefix that you can write to is required**:
without one, CMake uses a system folder (`C:\Program Files (x86)\...` on Windows, `/usr/local` on Linux) and the
configuration stops. The commands below put it inside the build folder.

xraylib (prebuilt, in `gvxr/dependencies/xraylib/`, for Windows and Linux on x86_64) and XrayPhysics (compiled into the
library) are part of the repository.

To take a dependency from the system instead, turn on its option: `USE_SYSTEM_GLEW`, `USE_SYSTEM_GLFW`, `USE_SYSTEM_ASSIMP`,
`USE_SYSTEM_ZLIB`, `USE_SYSTEM_LIBTIFF`, `USE_SYSTEM_FREETYPE`, `USE_SYSTEM_BOOST`, `USE_SYSTEM_GTEST`. The continuous
integration builds the core library (without GLFW and the tests) with the system packages of Ubuntu 24.04
(`libglew-dev libassimp-dev libtiff-dev libfreetype-dev zlib1g-dev libboost-dev libpng-dev libbz2-dev libvulkan-dev`) and
of MSYS2. A system GLEW is usually a shared library: add `-DGLEW_USE_STATIC_LIBS=OFF` with it.

## Building

The first configuration of a new build folder takes several minutes (the dependencies are built); the next ones do not build
them again. Use one build folder per compiler and per configuration.

### Windows (MinGW-w64)

In PowerShell. MinGW's `bin` folder must be on the `PATH` (for the compiler and, at run time, its libraries), but MinGW ships
a `cmake.exe` of its own that cannot check certificates, so the CMake from [cmake.org](https://cmake.org/download/) goes
first (see [Troubleshooting](#troubleshooting)). Replace `C:\mingw64` with the folder of your MinGW installation.

```powershell
$env:PATH = "C:\Program Files\CMake\bin;C:\mingw64\bin;" + $env:PATH
$prefix = "$PWD/build/install".Replace('\', '/')
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Release "-DCMAKE_INSTALL_PREFIX=$prefix" -DBUILD_SIMPLEGVXR=OFF -DASSIMP_WARNINGS_AS_ERRORS=OFF
cmake --build build --parallel 4
```

The install prefix is written with forward slashes because the build modules pass it on to the projects that they build.
`ASSIMP_WARNINGS_AS_ERRORS=OFF` lets the bundled Assimp build with a recent GCC. If a parallel build leaves compiler
processes behind that lock the build folder, build with `--parallel 1`.

### Linux

```
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="$PWD/build/install" \
    -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DBUILD_SIMPLEGVXR=OFF
cmake --build build --parallel 4
```

The C compiler is named because the top-level project declares only C++, and the build of GoogleTest needs a C compiler.

### Options

| Option | Default | Effect |
|---|---|---|
| `BUILD_TESTING` | `ON` | Build the tests (and download their large reference files) |
| `BUILD_SIMPLEGVXR` | `ON` | Build SimpleGVXR, its demos (`BUILD_DEMOS`, on), and its tests; it needs GLFW |
| `USE_GLFW` | `ON` | Use GLFW for the windows and the contexts that the library creates; off for an application that brings its own context |
| `GLEW_USE_STATIC_LIBS` | `ON` | Link the static GLEW; `OFF` for a shared GLEW (a system package) |
| `USE_SYSTEM_<NAME>` | `OFF` | Take a dependency from the system instead of building it (see [Dependencies](#dependencies)) |
| `gVXR_TEST_DATA_ARCHIVE` | in the build folder | The path of the archive of the test data (see [Tests](#tests)) |
| `gVXR_COVERAGE` | `OFF` | Compile the Debug configuration with the coverage flags (Linux) |
| `USE_GLAD` | `OFF` | Use the bundled glad loader instead of GLEW; leave it off in an application that has its own loader |

## SimpleGVXR and Its Demos

SimpleGVXR is a simpler interface to the library (free functions, which can open a window). To build it, its demos, and its
tests, configure the same build folder again with it turned on, and build:

```
cmake -S . -B build -DBUILD_SIMPLEGVXR=ON
cmake --build build --parallel 4
```

The demos are in `build/SimpleGVXR/demos_bin/`: `DemoDragon` (an interactive view of the Welsh dragon, in three windows: 32-bit,
16-bit, and mixed precision), `DemoBeerLambertLawMonoChromatic`, `DemoConvolutionKernel` (not interactive), and `Benchmark` (long
on purpose). On Windows, the build copies the xraylib DLL beside them. The Welsh dragon
example of `SimpleGVXR-examples/` is built against an installation and does not work yet (see
[limitations.md](limitations.md)).

## Tests

The tests use GoogleTest and CTest, with labels:

| Label | Tests | What they need |
|---|---|---|
| `cpu` | 8 | Nothing more (materials, mixtures, spectra, file reading) |
| `opengl` | 16 | An OpenGL context: a graphics card, or a virtual display with Mesa on Linux |
| `SimpleGVXR` | 18 | An OpenGL context; built only with `BUILD_SIMPLEGVXR=ON` |

```
ctest --test-dir build -L cpu --output-on-failure
ctest --test-dir build -L opengl --timeout 900 --output-on-failure
ctest --test-dir build -L SimpleGVXR --timeout 900 --output-on-failure
```

On a Linux machine without a display, install `xvfb libgl1-mesa-dri mesa-utils` and run the last two commands through a
virtual display, without the two tests that do not work with the software renderer (see
[Troubleshooting](#troubleshooting)), as the continuous integration does:

```
xvfb-run -a -s "-screen 0 1600x1000x24" ctest --test-dir build -L opengl -E "TestOpenGLContext|TestCubeSource" --timeout 900 --output-on-failure
```

**The test data.** The reference files of `TestSinogramFromMesh` (two files of 68 MB) are not in the repository. The first
configuration with the tests downloads the archive `gvxr-test-data-1.zip` (7.9 MB) from the release
[`test-data-1`](https://github.com/emartg/gvirtual-xray/releases/tag/test-data-1) of this repository, checks its SHA-256,
and extracts it in `build/test_data/`. On a machine without network access, download the archive elsewhere and give its path
with `-DgVXR_TEST_DATA_ARCHIVE=<path>`. A configuration with `-DBUILD_TESTING=OFF` needs no test data.

**The expected results** (2026-11-25): on Windows with a GeForce GTX 1660 SUPER, the 8 `cpu` and the 16 `opengl` tests pass;
with SimpleGVXR, 17 of its 18 tests pass, and `SGVXR_TestAcquisitionFile` fails at its cases `BHPI` and `distances`. On Linux with Mesa's
software renderer under Xvfb, the 8 `cpu` and 14 `opengl` tests pass (the two left out above), and 15 or 16 of the 18 SimpleGVXR
tests (see [limitations.md](limitations.md#known-issues)).

## Installation

```
cmake --install build
```

It copies the libraries, the headers, the demos, and the CMake package files into the install prefix. The installed CMake
package files do not work yet for another project (see [limitations.md](limitations.md)); a project uses the library with
`add_subdirectory` instead ([integration.md](integration.md)).

## Continuous Integration

The workflow `.github/workflows/ci.yml` runs on every pull request and on every push to `main`:

- **Linux** (GCC, the superbuild): builds the library and its tests, runs the `cpu` tests, and the `opengl` tests on a virtual
  display with Mesa (without the two that the software renderer cannot run).
- **Windows** (MinGW GCC, the superbuild): builds the library and its tests and runs the `cpu` tests (the runners cannot
  create the OpenGL context that the other tests need).
- **The consumer project** (`gvxr/tests/consumer`, the library used with `add_subdirectory`): on Linux with the system packages
  (the runner's CMake and CMake 3.26.4, and a leg with the shared GLEW), and on Windows with the MSYS2 packages and the shared
  GLEW.

SimpleGVXR is not built by the continuous integration. The build folder of the superbuild is cached: every run restores the
cache of `main`, and only the runs on `main` save it. The workflow `.github/workflows/diagrams.yml` checks that every diagram
image has its source ([diagrams/README.md](diagrams/README.md)).

## Troubleshooting

The failures of a configuration or a build, with the cause and what to do. The entries that have a title in common with the
build instructions of the other repositories of the project keep that title.

### A build folder holds the tools of another compiler

**Symptom.** The configuration stops with `Does not match the generator used previously`, or a build that worked with one
compiler fails after switching to another with errors that name tools or libraries of the first one.

**Cause.** CMake records the generator, the compilers, and the tools it found in `CMakeCache.txt` of the build folder, and the
superbuild also keeps the dependencies that it built with the first compiler in the build folder and in the install prefix.

**What to do.** Use one build folder per compiler (for example `build-gcc` and `build-mingw`), and delete the build folder, and
the install prefix that it filled, before switching a folder to another compiler or generator.

### A download fails with a certificate error (MinGW on the `PATH`)

**Symptom.** On Windows, the configuration stops while it downloads Boost or another dependency, with a message about the SSL
certificate.

**Cause.** The `cmake` found first in the `PATH` is the one that is shipped with MinGW-w64, which does not find the
certificates of the system. The one from [cmake.org](https://cmake.org/download/) does.

**What to do.** Put the `bin` folder of that CMake before the `bin` folder of MinGW in the `PATH`, check with
`Get-Command cmake` that it is the first one, and configure again in a new build folder.

### A download of gzip leaves an empty file (Windows)

**Symptom.** On Windows, when no `gzip` is installed, the build stops at the target `BinaryLibFiles` with
`The system cannot find the path specified.`, and nothing in the message names gzip.

**Cause.** The configuration downloads a gzip program when it finds none, extracts it, and records its path in
`CMakeCache.txt` without checking that the download worked. The download can leave an empty file, and the recorded path then
points to a file that does not exist.

**What to do.** Install a gzip that is in the `PATH` (the one of Git for Windows is in the `usr\bin` folder of its
installation), delete `CMakeCache.txt` (or the whole build folder), and configure again. A gzip in the `PATH` is found first and
nothing is downloaded. Deleting the folder `build\gvxr\gzip` and configuring again also downloads it again.

### The configuration stops at the test data

**Symptom.** With the tests on, the configuration stops with `The test data could not be downloaded from
https://github.com/emartg/gvirtual-xray/releases/download/test-data-1/gvxr-test-data-1.zip`, or with a message that the archive
`has the SHA-256 ... not fa0dcabf...`.

**Cause.** The reference data of `TestSinogramFromMesh` are not in the repository: `cmake/DownloadTestData.cmake` downloads the
archive `gvxr-test-data-1.zip` from the release `test-data-1` and checks its SHA-256. The download fails without network access
(or behind a proxy that CMake does not use); the check fails when the file at the path of the archive is another file.

**What to do.** Download the archive by hand (from the address of the message) and configure again with
`-DgVXR_TEST_DATA_ARCHIVE=<path of the file>`. For a wrong SHA-256, delete the file that the message names (by default
`test_data/gvxr-test-data-1.zip` in the build folder) and configure again. A configuration with `-DBUILD_TESTING=OFF` needs no
test data.

### `No rule to make target` after updating the repository

**Symptom.** `cmake --build build --target <name>` stops with `No rule to make target '<name>'` (or `unknown target`) just
after a `git pull`.

**Cause.** The update added a target, usually a test, and the build folder was generated before it existed.

**What to do.** Configure again before building: `cmake -S . -B build` (the dependencies are not built again), then
`cmake --build build --target <name>`.

### The tests that need an OpenGL context fail or never end

**Symptom.** On a machine without a GPU or a display (a server, a container, a continuous integration runner), the tests
labelled `opengl` fail at once, or two of them seem to hang.

**Cause.** They need an OpenGL context. On Linux, a virtual display and Mesa's software renderer provide one, but two tests do
not work there: `TestOpenGLContext` (its compute shader check reads back 0 where the shader wrote -1000 with the llvmpipe
renderer; it passes on a GPU) and `TestCubeSource` (about 5 minutes with Mesa 26, more than the timeout of 900 seconds of the
continuous integration).

**What to do.** On a machine with a GPU and its driver, run `ctest --test-dir build -L opengl --output-on-failure`. On a Linux
machine without a display, run the `xvfb-run` command of [Tests](#tests). The tests labelled `cpu` need no context.
