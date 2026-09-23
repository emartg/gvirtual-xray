---
title: "gVirtualXray" # short title
search_exclude: true # set to false if you want this page to show up in search results (it's 'true for the template as we don't want that in search results)
description: "Virtual X-Ray Imaging Library on GPU" # a description of the page - A brief description of the exemplar or use case and which problem it solves and which domains (or if its cross domain) it applies to. 
contributors: [Prof Franck P. Vidal, Dr Andréas Meuleman, Dr Aaron Sújar, Mr Iwan T. Mitchell] # a comma separated list of contributors' names, as found in _data/CONTRIBUTORS.yml (add yourself to the files if you do not have an entry)
page_id: # unique page id, ideally lowercase words separated by underscore(s) - for example page_id of 'Galaxy' could be galaxy
type: research_software_story
---

<!-- Please keep all sections and fill them in.
If this is not possible for any reason - you may remove them (you might need to explain to the Editorial Board in your pull request why certain sections are not present).
The text describing what is needed in the sections can be removed.
All comment sections can be removed before your submit a pull request.
-->

<!-- Once you have completed your research community entry - please add it to _data/sidebars/main.yml under the Research Software Stories entry in alphabetical order.
This comment can be deleted in your final page.
-->


## The Problem <!-- do not delete this heading and write your text below it -->
<!-- ### Virtual X-Ray Imaging Library on GPU -->

<!-- Describe the problem(s) your software is trying to solve for your community. -->
[gVirtualXray](https://gvirtualxray.sourceforge.io/) (gVXR) is an open-source **application programming interface (API)** that extends the Beer–Lambert law to **simulate realistic X-ray images in realtime** on a graphics processor unit (GPU) using triangular meshes. 
Simulations generated with [gVXR](https://gvirtualxray.sourceforge.io/) have been benchmarked with anthropomophic phantoms (i.e. clinically realistic with complex structures and materials that mimic the human body) using Monte Carlo (MC) simulations, real radiographs, real digitally reconstructed radiographs (DRRs), and X-ray computed tomography (XCT). 


## The Community <!-- do not delete this heading and write your text below it -->
<!-- ### Add a short one-two liner subtitle about the community/communities that use or will potentially use this software -->

<!-- - Who are the users - how experienced are they in using your software? -->
[gVXR](https://gvirtualxray.sourceforge.io/) is **for software developers or scientists who wish to simulate realistic X-ray images in realtime** when photon scattering is negligible. 
It takes into account properties of the X-ray source, sample and detector. 
Note that [WebCT](https://webct.io/), a feature-rich and user-friendly environment for previewing and simulating XCT scans on the web browser, is built on top of gVXR.

<!-- Describe the community that uses your software -->
Today, [gVXR](https://gvirtualxray.sourceforge.io/) is **widely used across academia, national facilities, and industry** in a [wide range of applications](https://gvirtualxray.sourceforge.io/applications/), including real-time medical simulators, proposing a new densitometric radiographic modality in clinical imaging, studying noise removal techniques in fluoroscopy, teaching particle physics and X-ray imaging to undergraduate students in engineering, and XCT to masters students, predicting image quality and artifacts in material science, etc. It has also been used to produce high number of realistic simulated images in optimisation problems and to train machine learning algorithms. A review paper published in 2025 on [*X-ray simulations with gVXR in education, digital twining, experiment planning, and data analysis*](https://doi.org/10.1016/j.nimb.2025.165804) demonstrates its growing community and scientific impact.


<!-- - Describe how the development is organised (e.g. is it one person, many, how are they organised and/or governed) -->
[gVXR](https://gvirtualxray.sourceforge.io/) was created by Prof. Franck Vidal at [Bangor University](https://www.bangor.ac.uk/) in December 2013. 
Final year students, MSc students, PhD students and workplacement students at Bangor University have been contributing to [gVXR](https://gvirtualxray.sourceforge.io/) under his supervision. 
Since October 2023, he actively maintains the code and regularly add new features for the [Science and Technology Facilities Council (STFC)](https://www.stfc.ac.uk/) of the [UK Research and Innovation (UKRI)](https://www.ukri.org/). 
A research software engineer (RSE) joined the development team in February 2026.


## Technical Aspects <!-- do not delete this heading and write your text below it -->

<!-- - What is the tier(s) of software involved (e.g. analysis code, prototype tools, research software infrastructure (including services)) -->

### Software architecture
[gVXR](https://gvirtualxray.sourceforge.io/) is written in C++ using the OpenGL Shading Language (GLSL) to leverage the GPU. 
In March 2026, the entire code base was about 225,000 lines of code and 50,000 lines of comments. 
It is split in different components built on the top of each other:

1. [gvxr-core](https://sourceforge.net/p/gvirtualxray/gitroot/ci/master/tree/gvxr/) is the "low-level" object-oriented API. 
2. [SimpleGVXR](sourceforge.net/p/gvirtualxray/gitroot/ci/master/tree/SimpleGVXR) is a smaller library built on the top of gvxr-core. It is implemented as a state machine and expose [gvxr-core](https://sourceforge.net/p/gvirtualxray/gitroot/ci/master/tree/gvxr/)'s functionalities to provide a procedural API. This architecture was inspired by the [Insight Toolkit (ITK)](https://itk.org/) and [SimpleITK](https://simpleitk.org/). 
3. [Wrappers](https://sourceforge.net/p/gvirtualxray/gitroot/ci/master/tree/Wrappers/) of [SimpleGVXR](sourceforge.net/p/gvirtualxray/gitroot/ci/master/tree/SimpleGVXR) to Python, R, Ruby, Tcl, C#, Java, and GNU Octave are created using the [Simplified Wrapper and Interface Generator (SWIG)](https://swig.org/). 
4. To simplify the setting up of a simulation, a user-friendly JSON file format has been designed for Python users. 
5. A [digital twin framework](https://sourceforge.net/p/gvirtualxray/gitroot/ci/master/tree/Wrappers/gvxrPython3/twins/) has been added to make it easier to simulate X-ray images with twins of actual devices. It is only available in Python.
6. The above components are integrated in [WebCT](https://webct.io/) to provide a user-friendly graphical user interface (GUI).

### Hardware compatibility
[gVXR](https://gvirtualxray.sourceforge.io/) is cross-platform. 
It runs on:
- MS Windows (Intel architecture only), 
- GNU/Linux (Intel architecture & ARM architecture), and 
- MacOS computers (Intel architecture only). 
Other architectures may be compatible, but they have not been tested.

It supports GPUs from any manufacturer. 
It can even run on platforms without GPUs (in this case, be patient as the CPU will be used). 

[gVXR](https://gvirtualxray.sourceforge.io/) is scalable. 
It runs on:
- [Raspberry Pi](https://www.raspberrypi.com/),
- laptops,
- desktop PCs,
- supercomputers, and
- cloud infrastructures, including:
   - STFC Cloud,
   - [Google Colab](https://colab.research.google.com/), and
   - [Code Ocean](https://codeocean.com/capsule/0412345/tree/v1).

Containerization using [Docker](https://www.docker.com/) is even possible.

In March 2026, a ray-tracer implementation for hardware accelerators without graphics cores was added. 
It is implemented with OpenMP GPU offloading and tested on the AMD Accelerator Cloud (AAC) with <big>NAME HERE</big>accelerated processing units (APUs).

### Libraries and Systems <!-- do not delete this heading and write your text below it -->

<!-- - List any domain/community specific libraries and systems you make use of in your software solutions. -->

- Required:
   - [C++ 23](https://en.cppreference.com/w/cpp/17.html): is required to use std::ranges::views::zip.
   - [GLEW](http://glew.sourceforge.net/): 
   provides efficient run-time mechanisms for determining which OpenGL extensions are supported on the target platform, and exposes OpenGL core and extension functionality in a single header file.
   [[Licence]](https://github.com/nigels-com/glew/blob/master/LICENSE.txt) **CHECK WHAT TYPE IT IS**
   - [Glad](): 
   is an alternative to GLEW.
   [[Licence]]() **CHECK WHAT TYPE IT IS**
   - [GLFW3]():   **ADD URL**
   is a platform-independent API for creating windows and OpenGL contexts on MS Windows, macOS and GNU/Linux and other Unix-like systems. On GNU/Linux both X11 and Wayland are supported.
   [[zlib/libpng license]](https://www.glfw.org/license.html)
   - [GZIP](): **ADD URL**
   compresses the shader files to make the binary distributions smaller. 
   Note that we do not distribute it. It is not required to use gVXR, just to build it.
   [[GNU licence]]() **ADD LICENCE URL**
   - [OpenGL](https://www.opengl.org/):
   leverages GPU 
   [Open standard]
   - [XRAYLIB](): **ADD URL**
   To compute mass attenuation coefficients. 
   [[Licence]](https://github.com/tschoonj/xraylib/blob/master/license_all.txt) **CHECK WHAT TYPE IT IS**
   - [XRayphysics](): **ADD URL**
   generates spectra for X-ray tubes with a tungsten target. **CAN WE USE OTHER TARGETS?**
   [[MIT licence]](https://github.com/kylechampley/XrayPhysics/blob/main/LICENSE)
   - [ZLIB]():
   Decompress shaders at runtime.
   [[Licence]](https://www.zlib.net/zlib_license.html) **CHECK WHAT TYPE IT IS**

- Optional:
   - ASSIMP:
   to load polygon meshes from popular file formats.
   [[3-clause BSD licence]](https://github.com/assimp/assimp/blob/master/LICENSE)
   - Doxygen:
   to generate the technical documentation from the C++ header files.
   [[Licence]]() **ADD LICENCE URL**  **CHECK WHAT TYPE IT IS**
   - EGL:
   to create an OpenGL context on GNU/Linux.
    [[Licence]](https://gitlab.freedesktop.org/mesa/mesa/-/blob/main/src/egl/main/eglconfig.h)    **CHECK WHAT TYPE IT IS**
   - FREETYPE:
   to display text on top of the visualisation window.
   [[BSD-style license]](https://gitlab.freedesktop.org/freetype/freetype/-/blob/master/docs/FTL.TXT)
   - GDCM: 
   to save DICOM files.
   [[BSD license]](https://github.com/malaterre/GDCM/blob/master/README.Copyright.txt)
   - GDM:
   to create an OpenGL context on GNU/Linux.
   [[Licence]](https://gitlab.freedesktop.org/mesa/mesa/-/blob/main/src/gbm/main/gbm.h)
   - GLX:
   to create an OpenGL context on GNU/Linux.
   [[Licence]](https://gitlab.freedesktop.org/mesa/mesa/-/blob/main/src/glx/glxconfig.h)
   - Google Test (Gtest):
   to run unit tests.
   [[Licence]]() **ADD LICENCE URL**  **CHECK WHAT TYPE IT IS**
   - ITK:
   to perform some image filtering tasks.
   [[Apache licence]](https://github.com/InsightSoftwareConsortium/ITK/blob/master/LICENSE)
   - LibTIFF:
   to save TIFF files.
   [[Licence]](https://gitlab.com/libtiff/libtiff/-/blob/master/LICENSE.md)  **CHECK WHAT TYPE IT IS**
   - OpenMP:
   to speed up code running on CPU in parallel, typically the image processing filters and the implicit modelling.
   [Open standard]
   - [OpenSceneGraph](): **ADD URL**
   [[Licence]]() **ADD LICENCE URL**  **CHECK WHAT TYPE IT IS**
   - X11:
   to create an OpenGL context on GNU/Linux.
   [[Licence]](https://github.com/mirror/libX11?tab=License-1-ov-file#readme) **CHECK WHAT TYPE IT IS**



## Software Practices <!-- do not delete this heading and write your text below it -->

When we initiated the project, we adopted [SourceForge](https://www.sourceforge.net/) (leading FLOSS hosting service at the time). Subversion (SVN) was used for version control from 2013 to 2024. We migrated the project to Git in 2024.

Containerisation was a game changer. Before, software developpers had to install the development tools on their computers. 
We had to deal with compatibility issues due to different versions of g++ supporting different C++ standards, etc. 
Developping in a Docker container makes sure the same development environment is available to everybody on any computer.

Dependencies, e.g. C++ libraries, also had to be installed. For portability, superbuild were used with CMake, i.e. all the libraries needed by [gVXR](https://gvirtualxray.sourceforge.io/) would be downloaded by CMake and compiled during the build of gVXR. Using [Conan](https://conan.io/) made it possible to install the binaries of the dependencies and reuse them for every build.

A 3-Clause BSD license was chosen to maximise the outreach of the project beyond academic circles. [gVXR](https://gvirtualxray.sourceforge.io/) is used in academia for teaching and research, and in national laboratories. Companies have also adopted it in their routine commercial activities.




- What benefits have the developers gained in building their software by applying these software quality practices?


SWIG
Doxygen
Conan
auditwheel
twine
   - Sphinx:
   to format the technical documentation generated using Doxygen
   [[Licence]]() **ADD LICENCE URL**  **CHECK WHAT TYPE IT IS**




## Community <!-- do not delete this heading and write your text below it -->
### Add a short one-two liner subtitle about how the software is being adopted for usage and how a community is being built around it

- How do you onboard new developers?
- How do you onboard new users?
   - Explain your main user stories (the main types of users, what they want to achieve and what the benefit to them is) 

## Tools <!-- do not delete this heading and write your text below it -->
### Add a short one-two liner subtitle about the other tools that help in improving your research software  

- [CMake](https://cmake.org/)/[CTest](https://cmake.org/cmake/help/latest/manual/ctest.1.html)/[CDash](https://www.cdash.org/)
- [jenkins](https://www.jenkins.io/)
- [Clang-Tidy](https://clang.llvm.org/extra/clang-tidy/)
- [Valgrind](https://valgrind.org/)
- [Docker](https://www.docker.com/)
- [Manylinux](https://github.com/pypa/manylinux)

- List any tools that you use to help you improve or assess the software quality of your code and a short description of how they help your software. Please refer to [_data/tool_and_resource_list.yml](https://github.com/EVERSE-ResearchSoftware/RSQKit/blob/main/_data/tool_and_resource_list.yml) file in the RSQKit repository and add any ones that are missing.
- What has been the benefit to the project of using these tools?

## FAIR & Open <!-- do not delete this heading and write your text below it -->
### Add a short one-two liner subtitle about how your research software is aligned with FAIR and/or Open practices

 - FAIR: see the [FAIR software page in RSQKit](https://everse.software/RSQKit/fair_rs) and [Five recommendations for FAIR software](https://fair-software.eu/) for inspiration - e.g. are there a repository and a license, do you have a community registry/catalogue where the code is available, is it citable, do you have a “good software” checklist?

Since the inception of the project,  we have adopted the FAIR principles for software (FAIR4RS):
- Findable, 
- Accessible, 
- Interoperable, and 
- Reusable.
It helped us to maximize transparency, reproducibility, and impact. 
See Barker, M., Chue Hong, N.P., Katz, D.S. et al. Introducing the FAIR Principles for research software. Sci Data 9, 622 (2022). DOI: [10.1038/s41597-022-01710-x](https://doi.org/10.1038/s41597-022-01710-x).

[gVXR](https://gvirtualxray.sourceforge.io/) is accessible to most potential users: 
- Open-source: A flexible license, [3-Clause BSD License](https://opensource.org/license/BSD-3-clause), makes it possible for academic researchers to use for free; it also permits commercial use with minimal restrictions.
- Vendor agnostic: It almost runs on any computer. OpenGL makes it cross-platform.
   - It is suitable for all major operating systems, inc. Windows, GNU/Linux and macOS.
   - Both x86-64 and ARM CPU architectures are supported.
   - Users can use GPUs from AMD, Intel, and NVIDIA. This is an advantage over proprietary APIs that run on vendor-specific hardware.
   - Discrete and integrated GPUs are supported.
   - It even runs on computers without GPU.
   - It is scalable and runs on [Raspberry Pi 5](https://www.raspberrypi.com/products/raspberry-pi-5/), [Jetson Orin Nano](https://www.nvidia.com/en-us/autonomous-machines/embedded-systems/jetson-orin/nano-super-developer-kit/), laptops, desktops, supercomputers (e.g. [SCARF](https://www.sc.stfc.ac.uk/platforms-and-services/scarf/)) and cloud infrastructures ([Supercomputing Wales](https://www.supercomputing.wales/)).

The use of inclusive design is progressively being improved. 
Information is shared in different formats through our website. 
We adopted the [**Web Accessibility Initiative (WAI) Guidelines**](https://www.w3.org/WAI/). 
The "Clear Print Guidelines" will be followed too. It will benefit the whole community. This is particularly important for tutorials. 
Technical support is available via various communication channels, including video calls. 
We also offer in-person training opportunities. 
We plan to run online trainings too. 

We have adopted the [**Open Source Security Foundation (OpenSSF) best practices**](https://www.bestpractices.dev/en/projects/9895). [![OpenSSF Best Practices](https://www.bestpractices.dev/en/projects/9895/badge)](https://www.bestpractices.dev/en/projects/9895). It is a free, self-certification framework for open-source projects to demonstrate adoption of essential development and security practices. Key areas are basics (documentation, website, governence), change control and quality, security practices and sustainability.
To obtain a badge, one must complete a form and provide evidences. As answers are publicly available, it is not possible to cheat! When we first completed it, holes in our practices were identified. OpenSSF Best Practices helped us improve our practices as developpers and maintainers. For example, static and dynamic analisys tools are now fully integrated in our continuous integration (CI) workflow. It has also improved our documentation. We now include a security, contributors and code of conducts guidelines. We are now establishing our governance policies. It is fair to say that adopting OpenSSF best practices has improved the robustness of the project and we made us better software engineers. 

## Documentation <!-- do not delete this heading and write your text below it -->
### Add a short one-two liner subtitle about how documenting the research software helps the user community using the research software

 - How is the documentation for your software presented and maintained?

Research papers
Videos
Technical documentations
Tutorials

BUILD.md
CHANGELOG
CODE_OF_CONDUCT.md
CONTRIBUTING.m
CONTRIBUTIRS
README
SECURITY

doc/

## Sustainability <!-- do not delete this heading and write your text below it -->

The project is active. Since 2023, gVirtualXray is maintained by the Tomography & Imaging team of [**Scientific Computing**](https://www.sc.stfc.ac.uk/) the [**Science and Technology Facilities Council (STFC)**](https://www.ukri.org/councils/stfc/). It was previously maintained at the [**School of Computer Science & Engineering**](https://www.bangor.ac.uk/scse) of [**Bangor University**](https://www.bangor.ac.uk/).

Funding from [**Collaborative Computational Project in Tomographic Imaging (CCPi)**](https://ccpi.ac.uk/), [**UK's National Research Facilityfor lab-based X-ray Computed Tomography (NXCT)**](https://nxct.ac.uk/) and institutional support ensures ongoing development, maintenance, and future-proofing. 
New functionalities tailored for National Facilities (e.g. the [Diamond Light Source](https://www.diamond.ac.uk/) and [Extreme Photonics Applications Centre (EPAC)](https://www.clf.stfc.ac.uk/Pages/EPAC.aspx)) are supported by the [**Ada Lovelace Centre (ALC)**](https://www.adalovelacecentre.ac.uk/). 
We also explored other sources of funding.

Up to now, the governance model is informal and it blends oversight from the core team with contributions from the wider community.

**gVirtualXray leverages GPUs thanks to OpenGL**, an open stardard by the Khronos Group (network of 170 organisations focused on developing and publishing royalty‑free standards to enable interoperability in 3D graphics). 
No proprietary software and libraries are used in gVirtualXray to ensure interoperability. 

gVirtualXray faces a critical challenge: **since 2017 OpenGL been superseded by Vulkan**, a modern cross-platform API for high-performance graphics and computing which aims at tackling its shortcomings promising higher performance and flexibility
We have already adopted most recommendations about modernising our OpenGL code to ease the migration to a modern graphics API.

**Containerisation with Manylinux** eases the distribution of portable binary Python extensions on Linux. 

**Green computing via mixed-precision** is possible to further speedup the execution time without loss of accuracy.


## References <!-- do not delete this heading and write your text below it -->

If this page has been inspired or derived from other resources, make sure to reference them here.
