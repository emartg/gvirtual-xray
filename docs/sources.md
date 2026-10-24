# Sources and Attribution

This repository is a fork of gVirtualXRay. This file credits the project itself, the third-party code that
is bundled in it or that its build uses, and the resources that are part of it: who made each one, where it
comes from, and under which license. It sits in `docs/` (the folder of the maintainer's documents; `doc/` keeps the
documents of the library's own authors) and has the same name and place as the file of the other repositories of the
simulator project (`docs/sources.md`).

## How to Use This File

- Add an entry when a library or a resource becomes part of the project: its name and version, author,
  source link, and license. A temporary asset can be listed as a placeholder until it is kept or removed.
- Each entry has a status:
  - **Verified**: stated by the repository's own files (`LICENSE`, `CONTRIBUTORS`, license files next to
    the code) or by the official page of the source.
  - **Inferred**: deduced from file names or from the source's description, not stated in the repository.
  - **Unknown**: not established yet (see "Open Items").
- Checked on 2026-10-11.

## The Project

| What | Credit and license | Status |
|---|---|---|
| gVirtualXRay (all code, documentation, and logos not credited below) | Copyright (c) 2019, Dr Franck P. Vidal. BSD 3-Clause License (see `LICENSE`) | Verified |
| Developers | Prof Franck P. Vidal, Dr Andréas Meuleman, Dr Aaron Sújar, Mr Iwan T. Mitchell, Mr Nalin Gupta (see `CONTRIBUTORS`) | Verified |
| Institutions | United Kingdom Research and Innovation, Science & Technology Facilities Council (UKRI-STFC); Bangor University; Universidad Rey Juan Carlos (see `CONTRIBUTORS`) | Verified |

The fork's own changes (build fixes for MinGW and Windows, Git LFS, repository hygiene) are by the owner
of this fork, under the same license.

## Third-Party Code Bundled in the Repository (`gvxr/dependencies/`)

| Component | Author and source | License | Status |
|---|---|---|---|
| xraylib (headers and prebuilt libraries for Windows, Linux, and macOS, in `xraylib/`) | Bruno Golosio, Antonio Brunetti, Manuel Sanchez del Rio, Tom Schoonjans, and Teemu Ikonen ([tschoonj/xraylib](https://github.com/tschoonj/xraylib)) | Three-clause BSD license (`xraylib/license_all.txt`) | Verified |
| XrayPhysics (sources `xsource.cpp` and `xsec.cpp` compiled into the library) | Kyle Champley | MIT License (`XrayPhysics/LICENSE`) | Verified |
| glad (v2 loader, used only with `USE_GLAD`) | David Herberth ([Dav1dde/glad](https://github.com/Dav1dde/glad)) | MIT License for the glad source code (`glad/LICENSE`) | Verified |

## Third-Party Libraries Used by the Build

Downloaded or found by the build scripts (`cmake/` and `gvxr/cmake/`), with the versions they name. The
simulator builds only the core library, which needs a subset of them.

| Library | Version in the scripts | Author and source | License | Status |
|---|---|---|---|---|
| [GLEW](https://github.com/nigels-com/glew) | 2.2.0 | Milan Ikits and the GLEW contributors | The GLEW license (modified BSD), with Mesa (MIT-style) and Khronos (MIT-style) parts | Verified (the project's license file) |
| [Assimp](https://github.com/assimp/assimp) | 5.4.0 (the Conan file names 6.0.2) | The assimp team | BSD 3-Clause | Verified |
| [zlib](https://github.com/madler/zlib) | 1.3.2 | Jean-loup Gailly and Mark Adler | The zlib license | Verified |
| [GLFW](https://www.glfw.org) | 3.4 (SimpleGVXR windowing) | Marcus Geelnard and the GLFW contributors | zlib License | Verified |
| [GoogleTest](https://github.com/google/googletest) | 1.17.0 (tests) | Google | BSD 3-Clause | Verified |
| [LibTIFF](https://libtiff.gitlab.io/libtiff/) | 4.4.0 | Sam Leffler, Silicon Graphics, and contributors | The LibTIFF license (permissive) | Verified |
| [FreeType](https://freetype.org) | 2.6 | The FreeType Project | The FreeType License (BSD-style, with a credit clause) or the GNU GPL version 2, at the user's choice | Verified |
| [Boost](https://www.boost.org) (`interprocess`) | 1.90.0 in the Conan file | The Boost community | Boost Software License 1.0 | Verified |

The scripts also mention FFTW3, FLTK, GDCM, GLUT, VCG, FreeImage, and Minizip as optional dependencies;
they are credited here when the fork actually uses them.

## Resources

| Path | What | Author, source, and license | Status |
|---|---|---|---|
| `SimpleGVXR-examples/WelshDragon/welsh-dragon-small.stl` (and copies in `Wrappers/` and `doc/6-coding/`) | Mesh of a Welsh dragon, used by the examples and demos | Probably derived from the Welsh Dragon mesh released by Bangor University, UK, for Eurographics 2011 (scanned by the CAM Innovation Centre, Coleg Menai); Bangor [released it into the public domain](https://research.bangor.ac.uk/en/datasets/eg-2011-welsh-dragon-dataset-3d-model/) and asks that publications acknowledge: "The Welsh Dragon mesh model was released by Bangor University, UK, for Eurographics 2011" | Inferred (the repository does not state the origin, and the "small" mesh may be a reduction of it) |
| `gvxr/resources/FreeSans.ttf` | Font embedded in the library | Probably GNU FreeFont, FreeSans ([license](https://www.gnu.org/s/freefont/license.html): GPL version 3 or later with a font exception) | Inferred (from the file name; the repository has no license file for it) |
| `gvxr/tests/data/`, `SimpleGVXR/tests/data/` (Git LFS) | Reference data of the validation tests (scintillator responses, CT reconstruction) | Presumably produced by the gVXR developers (some folders are named `old_gate`, which suggests simulations with GATE) | Unknown |
| `Wrappers/gvxrPython3/CAD_models/` (`PMMA_block.stl`, `col_0.stl` to `col_4.stl`, `cube.stl`, `matrix.stl`) | Example meshes | Probably made by the gVXR developers | Unknown |
| `logo-pack/`, `Logo.png`, `Logo-transparent-*.png`, `doc/`, `gvxr/doc/`, `screenshots/` | Logos, documentation images, and screenshots | Presumably the gVXR developers, under the project's license | Unknown |

## Open Items

1. **Welsh dragon:** confirm with the gVXR documentation or its authors that the mesh derives from the
   Bangor dataset, and keep the acknowledgement above.
2. **FreeSans:** confirm that it is GNU FreeFont and add its license text next to the font, since the
   library embeds it in its binaries.
3. **License texts of the libraries the build downloads:** they are fetched at build time and carry their
   own license files; keep this table in step with `cmake/` and `gvxr/cmake/`.
4. **Everything marked Unknown:** ask the upstream project (the gVXR developers) or check its documentation.
