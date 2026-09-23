<meta charset="UTF-8">


gVirtualXRay: Virtual X-Ray Imaging Library on GPU
======================================================


Links
-----

* [Homepage](http://gvirtualxray.sourceforge.net/)
* [Download](http://gvirtualxray.sourceforge.net/downloads.php)
* [Demos](http://gvirtualxray.sourceforge.net/demos.php)
* [Programming documentation](http://gvirtualxray.sourceforge.net/documentation.php)
* [Quantitative validation](http://gvirtualxray.sourceforge.net/validation/validation_tests.php)
* [Videos](http://gvirtualxray.sourceforge.net/videos.php)
* [Mailing List](https://sourceforge.net/projects/gvirtualxray/lists/gvirtualxray-discuss)
* [Publications](http://gvirtualxray.sourceforge.net/publications.php)
* [Submit an issue](ISSUE_TEMPLATE.md)


About
-----

This project is focused on developing new software technologies for simulating X-ray images on the graphics processor unit (GPU) using OpenGL. The library is written in C++ and the OpenGL Shading Language (GLSL). It is portable and works on a wide range of computers and operating systems. Wrappers to other popular languages, including C#, Java, GNU Octave, Perl, Python 2, Python 3, R, Ruby and Tcl. A Matlab wrapper is in preparation too. The toolkit may be built from source using [CMake](https://cmake.org). The source code is available on SourceForge (https://sourceforge.net/projects/gvirtualxray/) under the BSD 3-Clause License.

Monte Carlo (MC) simulation is known as the state-of-the-art technique in term of accuracy. However, weeks of computation may be required to simulate a noise-free X-ray projection at high resolution, which limits the application of MC simulations in many applications where real-time performance is a requirement. Deterministic simulation provides a good compromise between speed and accuracy [[1](#cite1)] and can be implemented on GPUs for a further increase of speed [[2](#cite2)]. It allows [gVirtualXRay] to provide simulated X-ray projections in realtime without any further compromise on accuracy [[3](#cite3)]. For more realism, it is possible to take into account the imaging acquisition chain parameters, e.g. finite size of the X-ray tube, spectrum of the radiation beam, and the X-ray detector’s impulse response [[3](#cite3),[4](#cite4)]

It can be used in various application contexts, including

- __real-time medical simulations__ for training purposes [[5](#cite4),[6](#cite6),[7](#cite7)],
- __medical physics__, e.g. realistic data acquisition simulation in medical computed tomography (CT) with patient respiration [[8](#cite8)], and enhancing real clinical X-ray radiographs to provide densitometric images [[9](#cite9)], and
- __material science__ to investigate the source of artefacts in CT in dense metallic objects [[1](#cite1),[10](#cite10)].
- in __teaching__ X-ray imaging principles to students in mechanics (for the use of X- and γ-rays in non-destructive testing) and in medical physics.

The use of GPU computing opens up new perspectives. A simulated X-ray projection can be simulated in a few microseconds. It makes it possible to combine [gVirtualXRay] with an optimisation and machine learning framework, which would not have been possible a few years ago. For example, this approach can be used in 3D volume to 3D meshes registration [[10](#cite10)].


Copyright
---------

Dr Franck P. Vidal and Bangor University hold the copyright of this software. [gVirtualXRay] is available under the [3-Clause BSD License](http://opensource.org/licenses/BSD-3-Clause).
See [LICENSE](https://sourceforge.net/p/gvirtualxray/code/HEAD/tree/trunk/gvxr/LICENSE.txt). For details on use and redistribution please refer to the [3-Clause BSD License](http://opensource.org/licenses/BSD-3-Clause).


Dependencies
------------
All you need to build [gVirtualXRay] is:

- __[CMake]__ and a modern C++ compiler that have access to the __[OpenGL API]__. Note that OpenGL 2.x, 3.x and 4.x are supported.
- a __computer with a GPU__ to run programs that make use of [gVirtualXRay]. The software has been tested on MS Windows, MacOSX, and Linux using graphics cards from Intel, AMD and NVIDIA.
- __[ASSIMP]__ to load polygon mesh models and scenegraphs.
- __[GTest]__ and __[GLFW3]__ to build the quantitative validation programs.
- [zlib] to include compressed resources (mostly the shader programs and the material properties) into the binaries. This way you don't have to worry about where the files might be at runtime.

There are also some optional packages that are supported:

- __[NIST XCOM]__, a photon cross sections database, to produce physically accurate X-ray projections. Its use is optional but highly recommended.
- __[GLEW]__ to find OpenGL's core and extension functionalities. Its use is optional but highly recommended.
- __[ITK]__ to load and save volume datasets in many file formats.
- __[FFTW3]__ to perform the fast Fourier transform (FFT) and inverse fast Fourier transform (IFFT) used in our implementation of the filter-backproject (FBP) tomography reconstruction algorithm. It is not supported on MS Windows with MS VC++ compiler.
- __[FreeType]__ to load font files and display text in OpenGL.
- __[GDCM]__ to save DICOM files.
- __[LibTIFF]__ to save 16-bit TIFF files.

You don't have to worry about downloading and installing any of the software dependencies mentioned above ([NIST XCOM],
[ASSIMP],
[GTest],
[GLFW3],
[zlib],
[GLEW],
[ITK],
[FFTW3],
[FreeType],
[GDCM], and
[LibTIFF]).
[CMake] can automatically download and compile them.


How to cite
-----------

Details about (most of) the methods used and implemented in [gVirtualXRay] can
be found in these papers:

- Franck P. Vidal, and Pierre-Frédéric Villard. "Development and validation of real-time
simulation of X-ray imaging with respiratory motion".
_Computerized Medical Imaging and Graphics_, 49:1-15, April 2016.
DOI: [10.1016/j.compmedimag.2015.12.002](http://dx.doi.org/10.1016/j.compmedimag.2015.12.002).
PMID: [26773644](https://www.ncbi.nlm.nih.gov/m/pubmed/26773644)
- Franck P. Vidal, Manuel Garnier, Nicolas Freud, Jean Michel Létang, and Nigel W. John. "Accelerated deterministic simulation of X-ray attenuation using graphics hardware". In _Eurographics 2010 - Poster_, page Poster 5011, Norrköping, Sweden, May 2010. Eurographics Association. DOI: [10.2312/egp.20101026](http://dx.doi.org/10.2312/egp.20101026).
- Franck P. Vidal, Manuel Garnier, Nicolas Freud, Jean Michel Létang, and Nigel W. John. "Simulation of X-ray attenuation on the GPU". In _Proceedings of Theory and Practice of Computer Graphics 2009_, pages 25-32, Cardiff (UK), June 2009. Eurographics Association. __Winner of Ken Brodlie Prize for Best Paper.__ DOI: [10.2312/LocalChapterEvents/TPCG/TPCG09/025-032](http://dx.doi.org/10.2312/LocalChapterEvents/TPCG/TPCG09/025-032).

There are other scientific papers of interest. For more details, visit our [http://gvirtualxray.sourceforge.net/publications.php](publication page)


Scientific collaborations
-------------------------

If you'd like to discuss any project idea, grant application, etc. based on gVirtualXRay, just drop me an email at f DOT vidal AT bangor DOT ac DOT uk.


References
----------

<a id="cite1"/>[1] F. P. Vidal, J. M. Létang, G. Peix, and P. Clœtens. "Investigation of artefact sources in synchrotron microtomography via virtual X-ray imaging". _Nuclear Instruments and Methods in Physics Research B_, 234(3):333-348, June 2005. DOI: [10.1016/j.nimb.2005.02.003](http://dx.doi.org/10.1016/j.nimb.2005.02.003).  

<a id="cite2"/>[2] F. P. Vidal, M. Garnier, N. Freud, J. M. Létang, and N. W. John. "Simulation of X-ray attenuation on the GPU". In _Proceedings of Theory and Practice of Computer Graphics 2009_, pages 25-32, Cardiff (UK), June 2009. Eurographics Association. __Winner of Ken Brodlie Prize for Best Paper.__ DOI: [10.2312/LocalChapterEvents/TPCG/TPCG09/025-032](http://dx.doi.org/10.2312/LocalChapterEvents/TPCG/TPCG09/025-032).

<a id="cite3"/>[3] F. P. Vidal, and P.-F. Villard. "Development and validation of real-time simulation of X-ray imaging with respiratory motion". _Computerized Medical Imaging and Graphics_, 49:1-15, April 2016. DOI: [10.1016/j.compmedimag.2015.12.002](http://dx.doi.org/10.1016/j.compmedimag.2015.12.002).

<a id="cite4"/>[4] F. P. Vidal, M. Garnier, N. Freud, J. M. Létang, and N. W. John. "Accelerated deterministic simulation of X-ray attenuation using graphics hardware". In _Eurographics 2010 - Poster_, page Poster 5011, Norrköping, Sweden, May 2010. Eurographics Association. DOI: [10.2312/egp.20101026](http://dx.doi.org/10.2312/egp.20101026).

<a id="cite5"/>[5] P.-F. Villard, F. P. Vidal, C. Hunt, F. Bello, N. W. John, S. Johnson, and D. A. Gould. "Simulation of percutaneous transhepatic cholangiography training simulator with real-time breathing motion". _International Journal of Computer Assisted Radiology and Surgery_, 4(9):571-578, November 2009. DOI: [10.1007/s11548-009-0367-1](http://dx.doi.org/10.1007/s11548-009-0367-1).

<a id="cite6"/>[6] A. Sujar, A. Meuleman, P.-F. Villard, M. García, F. P. Vidal. "gVirtualXRay: Virtual X-Ray Imaging Library on GPU". In _Proceedings of Computer Graphics and Visual Computing (CGVC) 2017_, pages 61-68, Manchester, UK, September 2017. Eurographics Association. DOI: [10.2312/cgvc.20171279](http://dx.doi.org/10.2312/cgvc.20171279).

<a id="cite7"/>[7] Z. Zuo, W. Y. Qian, X. Liao, and P.-A. Heng. "Position based catheterization and angiography simulation". In _Proceedings of 2018 IEEE 6th International Conference on Serious Games and Applications for Health (SeGAH)_, Vienna (Austria), May 2018. IEEE. DOI: [10.1109/SeGAH.2018.8401369](http://dx.doi.org/10.1109/SeGAH.2018.8401369).

<a id="cite8"/>[8] F. P. Vidal, and P.-F. Villard. "Simulated Motion Artefact in Computed Tomography". In _Eurographics Workshop on Visual Computing for Biology and Medicine_, pages 213-214, Chester (UK), September 2015. Eurographics Association. DOI: [10.2312/vcbm.20151228](http://dx.doi.org/10.2312/vcbm.20151228).

<a id="cite9"/>[9] F. Albiol, A. Corbi, and A. Albiol. "
Densitometric Radiographic Imaging With Contour Sensors". _IEEE Access_, 7:18902-18914, January 2019. DOI: [10.1109/ACCESS.2019.2895925](http://dx.doi.org/10.1109/ACCESS.2019.2895925).

<a id="cite10"/>[10] F. P. Vidal, "gVirtualXRay – Fast X-ray simulation on GPU", in _Workshop on Image-Based Finite Element Method for Industry 2018 (IBFEM-4i 2018)_, Invited talk, Swansea (UK), September 2018. DOI: [10.5281/zenodo.1452506](http://dx.doi.org/10.5281/zenodo.1452506).



------------

Copyright (c) 2013-2019 Dr Franck P. Vidal, Bangor University, f DOT vidal AT bangor DOT ac DOT uk


[gVirtualXRay]: http://gvirtualxray.sourceforge.net/
[zlib]: https://www.zlib.net/
[GLEW]: http://glew.sourceforge.net/
[ITK]: https://itk.org/
[FFTW3]: http://www.fftw.org/
[FreeType]: https://www.freetype.org/
[GDCM]: http://gdcm.sourceforge.net/
[LibTIFF]: http://www.libtiff.org/
[CMake]: http://www.cmake.org
[OpenGL API]: https://www.opengl.org/documentation/
[NIST XCOM]: https://dx.doi.org/10.18434/T48G6X
[ASSIMP]: http://www.assimp.org/
[GTest]: https://github.com/google/googletest
[GLFW3]: https://www.glfw.org/
