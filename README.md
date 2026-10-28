# gVirtualXray (gVXR): Virtual X-Ray Imaging Library on GPU


## What does gVXR do?

gVirtualXRay (gVXR) is a C++ library to simulate X-ray imaging. 
It is based on the Beer-Lambert law to compute the absorption of light (i.e. photons) by 3D objects (here polygon meshes). 
It is implemented on the graphics processing unit (GPU) using the OpenGL Shading Language (GLSL).

SimpleGVXR is a smaller library build on the top of gVirtualXRay. 
It provides wrappers to Python, R, Ruby, Tcl, C#, Java, and GNU Octave.

## What can you do with gVXR?

X-ray simulations created with gVirtualXRay have been used in a wide range of applications, including 
- real-time medical simulators, 
- proposing a new densitometric radiographic modality in clinical imaging, 
- studying noise removal techniques in fluoroscopy, 
- teaching particle physics and X-ray imaging to undergraduate students in engineering, and XCT to masters students, 
- predicting image quality and artifacts in material science, etc. 
gVXR has also been used to produce a high number of realistic simulated images in optimisation problems and to train machine learning algorithms, with applications in oncology and radiology in medicine as well as material science.

Further details are available in three community review articles:
- The first one focuses on **X-ray Computed Tomography (XCT)** applications: ["X-ray simulations with gVXR as a useful tool for education, data analysis, set-up of CT scans, and scanner development"](https://doi.org/10.1117/12.3025315) was honoured with the **Best Paper Award of the SPIE CT Conference 2024 for "tomography outreach tools"**.
- The second one focuses on **medical and life science** applications: ["X-ray simulations with gVirtualXray in medicine and life sciences"](https://doi.org/10.2312/evm.20251974) was honoured with the **3rd place in the Dirk Bartz Prize for Visual Computing in Medicine and Life Sciences 2025** (Eurographics Medical Prize).
- The final is a **comprehensive review covering all applications**: ["X-ray simulations with gVXR in education, digital twining, experiment planning, and data analysis"](https://doi.org/10.1016/j.nimb.2025.165804).


## Who is it for?

gVXR is an application programming interface (API). 
It is for software developers who wish to simulate realistic X-ray images in realtime when photon scattering is negligible. 
gVXR's features can be used in C++, Python, R, Ruby, Tcl, C#, Java, and GNU Octave. 
To simplify the setting up of a simulation, a user-friendly JSON file format has been designed (note: for Python only at the moment).

If programming is not your thing, check out [WebCT](https://webct.io/), a feature-rich environment for previewing and simulating X-ray scans on the web browser.



## Is gVXR available for free?

Yes, gVXR is open source. Its source code is available under the BSD 3-Clause License. For details on use and redistribution please refer to [http://opensource.org/licenses/BSD-3-Clause](http://opensource.org/licenses/BSD-3-Clause).



## Credits and Third-Party Code

The credits of the project and the origin and license of the third-party code and resources it uses (xraylib, XrayPhysics, GLEW, Assimp, the Welsh dragon mesh, the embedded font, and more) are recorded in [docs/sources.md](docs/sources.md).



## Is gVXR available as a package that can be deployed without building it?

Python wheels are available on [Python Package Index (Pypi)](https://pypi.org/project/gvxr/). For other programming languages, you will have to build gVXR from the source code (see [docs/build_instructions.md](docs/build_instructions.md)).


## Is gVXR code publicly available to download?

Yes. It is hosted in an established third-party source code repository called [SourceForge](https://sourceforge.net/projects/gvirtualxray/).

## Git LFS requirement

Git LFS (Large File Storage) is required when cloning the source repository because the project contains large test data and documentation files.

Install Git LFS using the instructions for your platform:

- Windows: `winget install GitHub.GitLFS` or download it from [git-lfs.com](https://git-lfs.com/).
- macOS: `brew install git-lfs`
- Debian or Ubuntu: `sudo apt install git-lfs`

Then enable Git LFS and download the project files:

```bash
git lfs install
git clone <repository-url>
cd gVirtualXray
git lfs pull
```

## Installation

```bash
pip install gvxr
```

You may also install Numpy, tifffile and Matplotlib to run the test below.

```bash
pip install numpy matplotlib tifffile
```

## Usage

There are 6 main steps to simulate an X-ray image:

1. Create a renderer (OpenGL context): `gvxr.createOpenGLContext()`
2. X-ray source:
    - Position
        - x: -40.0 cm,
        - y: 0.0 cm,
        - z: 0.0 cm,
        - `gvxr.setSourcePosition(-40.0,  0.0, 0.0, "cm")`
    - Shape:
        - cone beam: `gvxr.usePointSource()`, or
        - Parallel (e.g. synchrotron): `gvxr.useParallelBeam();`
3. Spectrum:
    - monochromatic (0.08 MeV, i.e. 80 keV),
    - 1000 photons per ray,
    - `gvxr.setMonoChromatic(0.08, "MeV", 1000)`
4. Detector:
    - Position:
        - x: 10.0 cm,
        - y: 0.0 cm,
        - z: 0.0 cm,
        - `gvxr.setDetectorPosition(10.0, 0.0, 0.0, "cm")`
    - Orientation:
        - 0, 0, -1
        - `gvxr.setDetectorUpVector(0, 0, -1)`
    - Resolution:
        - $640 \times 320$ pixels
        - `gvxr.setDetectorNumberOfPixels(640, 320)`
    - Pixel spacing:
        - 0.5, 0.5, mm
        - `gvxr.setDetectorPixelSize(0.5, 0.5, "mm")`
5. Sample:
    - Welsh dragon in a STL file:
        - ID: "Dragon",
        - STL file: "input_data/welsh-dragon-small.stl",
        - Unit: mm,
        - `gvxr.loadMeshFile("Dragon", "input_data/welsh-dragon-small.stl", "mm")`
    - Material of the sample (ID = "Dragon"):
        - For a chemical element such as iron, you can use the Z number or symbol:
            - `gvxr.setElement("Dragon", 26)`, or
            - `gvxr.setElement("Dragon", "Fe")`
        - For a compound such as water, do not forget to specify the density:
            - `gvxr.setCompound("Dragon", "H2O")`
            - `gvxr.setDensity("Dragon", 1.0, "g/cm3")`
            - `gvxr.setDensity("Dragon", 1.0, "g.cm-3")`
        - For a mixture such as Titanium-Aluminum-Vanadium alloy, do not forget to specify the density:
            - `gvxr.setMixture("Dragon", "Ti90Al6V4")`
            - `gvxr.setMixture("Dragon", [22, 13, 23], [0.9, 0.06, 0.04])`
            - `gvxr.setMixture("Dragon", ["Ti", "Al", "V"], [0.9, 0.06, 0.04]) # Not yet implemented`
            - `gvxr.setDensity("Dragon", 4.43, "g/cm3")`
            - `gvxr.setDensity("Dragon", 4.43, "g.cm-3")`
6. Compute the corresponding X-ray image: `xray_image = gvxr.computeXRayImage()`

You can find the Jupyter Notebook of the example below at: [https://github.com/effepivi/gvxr-demos/blob/main/training-course/02-first_xray_simulation.ipynb](https://github.com/effepivi/gvxr-demos/blob/main/training-course/02-first_xray_simulation.ipynb).

```python
#!/usr/bin/env python3

# Import packages
import os
import numpy as np # Who does not use Numpy?

import matplotlib # To plot images
import matplotlib.pyplot as plt # Plotting
from matplotlib.colors import LogNorm # Look up table
from matplotlib.colors import PowerNorm # Look up table

# from tifffile import imwrite # Write TIFF files

from gvxrPython3 import gvxr # Simulate X-ray images

# Create an OpenGL context
print("Create an OpenGL context")
gvxr.createOpenGLContext();

# Create a source
print("Set up the beam")
gvxr.setSourcePosition(-40.0,  0.0, 0.0, "cm");
gvxr.usePointSource();
#  For a parallel source, use gvxr.useParallelBeam();

# Set its spectrum, here a monochromatic beam
# 1000 photons of 80 keV (i.e. 0.08 MeV) per ray
gvxr.setMonoChromatic(0.08, "MeV", 1000);
# The following is equivalent: gvxr.setMonoChromatic(80, "keV", 1000);

# Set up the detector
print("Set up the detector");
gvxr.setDetectorPosition(10.0, 0.0, 0.0, "cm");
gvxr.setDetectorUpVector(0, 0, -1);
gvxr.setDetectorNumberOfPixels(640, 320);
gvxr.setDetectorPixelSize(0.5, 0.5, "mm");

# Locate the sample STL file from the package directory
path = os.path.dirname(gvxr.__file__)
fname = path + "/welsh-dragon-small.stl"

# Load the sample data
if not os.path.exists(fname):
    raise IOError(fname)

print("Load the mesh data from", fname);
gvxr.loadMeshFile("Dragon", fname, "mm")

print("Move ", "Dragon", " to the centre");
gvxr.moveToCentre("Dragon");

# Material properties
print("Set ", "Dragon", "'s material");

# Iron (Z number: 26, symbol: Fe)
gvxr.setElement("Dragon", 26)
gvxr.setElement("Dragon", "Fe")

# Liquid water
gvxr.setCompound("Dragon", "H2O")
gvxr.setDensity("Dragon", 1.0, "g/cm3")
gvxr.setDensity("Dragon", 1.0, "g.cm-3")

# Titanium Aluminum Vanadium Alloy
gvxr.setMixture("Dragon", "Ti90Al6V4")
gvxr.setMixture("Dragon", [22, 13, 23], [0.9, 0.06, 0.04])
# gvxr.setMixture("Dragon", ["Ti", "Al", "V"], [0.9, 0.06, 0.04]) # Not yet implemented
gvxr.setDensity("Dragon", 4.43, "g/cm3")
gvxr.setDensity("Dragon", 4.43, "g.cm-3")

# Compute an X-ray image
# We convert the array in a Numpy structure and store the data using single-precision floating-point numbers.
print("Compute an X-ray image");
x_ray_image = np.array(gvxr.computeXRayImage()).astype(np.single)

# Update the visualisation window
gvxr.displayScene()

# Create the output directory if needed
if not os.path.exists("output_data"):
    os.mkdir("output_data")

# Save the X-ray image in a TIFF file and store the data using single-precision floating-point numbers.
gvxr.saveLastXRayImage('output_data/raw_x-ray_image-02.tif')

# The line below will also works
# imwrite('output_data/raw_x-ray_image-02.tif', x_ray_image)

# Save the L-buffer
gvxr.saveLastLBuffer('output_data/lbuffer-02.tif');

# Display the X-ray image
# using a linear colour scale
plt.figure(figsize=(10, 5))
plt.title("Image simulated using gVirtualXray\nusing a linear colour scale")
plt.imshow(x_ray_image, cmap="gray")
plt.colorbar(orientation='vertical');
plt.show()

# using a logarithmic colour scale
plt.figure(figsize=(10, 5))
plt.title("Image simulated using gVirtualXray\nusing a logarithmic colour scale")
plt.imshow(x_ray_image, cmap="gray", norm=LogNorm(vmin=x_ray_image.min(), vmax=x_ray_image.max()))
plt.colorbar(orientation='vertical');
plt.show()

# using a Power-law colour scale (gamma=0.5)
plt.figure(figsize=(10, 5))
plt.title("Image simulated using gVirtualXray\nusing a Power-law colour scale ($\gamma=0.5$)")
plt.imshow(x_ray_image, cmap="gray", norm=PowerNorm(gamma=1./2.))
plt.colorbar(orientation='vertical');
plt.show()

# Display the X-ray image and compare three different lookup tables
plt.figure(figsize=(17, 7.5))

plt.suptitle("Image simulated with gVirtualXray visualised", y=0.75)

plt.subplot(131)
plt.imshow(x_ray_image, cmap="gray")
plt.colorbar(orientation='horizontal')
plt.title("using a linear colour scale")

plt.subplot(132)
plt.imshow(x_ray_image, norm=LogNorm(), cmap="gray")
plt.colorbar(orientation='horizontal')
plt.title("using a logarithmic colour scale")

plt.subplot(133)
plt.imshow(x_ray_image, norm=PowerNorm(gamma=1./2.), cmap="gray")
plt.colorbar(orientation='horizontal');
plt.title("using a Power-law colour scale ($\gamma=0.5$)")

plt.tight_layout()

plt.savefig("output_data/projection-02.pdf", dpi=600);

# Change the sample's colour
# By default the object is white, which is not always pretty. Let's change it to purple.
red = 102 / 255
green = 51 / 255
blue = 153 / 255
gvxr.setColour("Dragon", red, green, blue, 1.0)

# This image can be used in a research paper to illustrate the simulation environment, in which case you may want to change the background colour to white with:
gvxr.setWindowBackGroundColour(1.0, 1.0, 1.0)

# Update the visualisation window
gvxr.displayScene()

# Take the screenshot and save it in a file
screenshot = gvxr.takeScreenshot()
plt.imsave("output_data/screenshot-02.png", np.array(screenshot))

# or display it using Matplotlib
plt.figure(figsize=(10, 10))
plt.imshow(screenshot)
plt.title("Screenshot of the X-ray simulation environment")
plt.axis('off');
plt.show()


# Interactive visualisation
# The user can rotate the 3D scene and zoom-in and -out in the visualisation window.

# - Keys are:
#     - Q/Escape: to quit the event loop (does not close the window)
#     - B: display/hide the X-ray beam
#     - W: display the polygon meshes in solid or wireframe
#     - N: display the X-ray image in negative or positive
#     - H: display/hide the X-ray detector
# - Mouse interactions:
#     - Zoom in/out: mouse wheel
#     - Rotation: Right mouse button down + move cursor```
gvxr.renderLoop()
```

## Awards &amp; accolades

1. **2<sup>nd</sup> place**
   *Eurographics 2009 - Medical Prize*
   for "ImaGiNe-S : Imaging Guided Needle Simulation".
   DOI: [10.2312/egm.20091024](https://doi.org/10.2312/egm.20091024)
2. **Winner of Ken Brodlie Prize for best paper**
   *Theory and Practice of Computer Graphics 2009 (TPCG 2009)*
   for "Simulation of X-ray Attenuation on the GPU".
   DOI: [10.2312/LocalChapterEvents/TPCG/TPCG09/025-032](https://doi.org/10.2312/LocalChapterEvents/TPCG/TPCG09/025-032)
3. **Winner of David Duce Prize for best short paper**
   *Computer Graphics and Visual Computing 2019 (CGVC 2019)*
   for "Projectional Radiography Simulator: an Interactive Teaching Tool".
   DOI: [10.2312/cgvc.20191267](https://doi.org/10.2312/cgvc.20191267)
4. **Best poster presentation**
   *6th Dimensional X-ray Computed Tomography Conference (dXCT 2022)*
   for "WebCT: Fully Featured Browser-Based Interactive X-Ray Simulations for Scan Planning and Training".
5. **Runner-up award for best student presentation**
   *Image-Based Simulation for Industry (IBSim-4i) 2023*
   for "Creating Functional Digital Shadows of X-ray systems".
6. **1<sup>st</sup> place**
   *CoSeC Impact Award 2023*
   for "Digital twins of industrial XCT scanners".
   URL: [https://www.cosec.ac.uk/impact/cosec-announces-impact-award-for-2023/](https://www.cosec.ac.uk/impact/cosec-announces-impact-award-for-2023/)
7. **Best Presentation Award**
   *International Symposium on Machinery and Mechatronics for Agriculture and Biosystems Engineering (ISMAB) 2024*
   for "Enhancing Material Decomposition in spectral CT Imaging via Deep Learning".
8. **Best paper award for tomography outreach tools**
   *Developments in X-Ray Tomography XV, SPIE Optics &amp; Photonics 2024*
   for "X-ray simulations with gVXR as a useful tool for education, data analysis, set-up of CT scans, and scanner development".
   DOI: [10.1117/12.3025315](https://doi.org/10.1117/12.3025315)
9. **3<sup>rd</sup> place**
   *Dirk Bartz Prize for Visual Computing in Medicine and Life Sciences 2025 (Eurographics Medical Prize)*
   for "X-ray simulations with gVirtualXray in medicine and life sciences".
   DOI: [10.2312/evm.20251974](https://doi.org/10.2312/evm.20251974)


## Build and test status of the trunk

gVirtualXRay may be built from source using [CMake](https://cmake.org). The status of the latest build can be found on [CDash](https://my.cdash.org/index.php?project=gVirtualXray). 

**gVXR is cross-platform**: it runs on 
- MS Windows(Intel architecture only, it might work on ARM architecture too), 
- GNU/Linux (Intel &amp; ARM architectures), and 
- MacOS computers (Intel architecture only).

It supports GPUs from any manufacturer. It can even run on platforms without GPUs (in this case, be patient as the CPU will be used).

**gVXR is scalable**: it runs on 
- Raspberry Pi,
- laptops,</li>
- desktop PCs,</li>
- supercomputers, and </li>
- cloud infrastructures, including:
    - Google Colab,
    - Code Ocean, and
    - STFC cloud.

Containerization using Docker is even possible.

It should be possible to run it on other platforms, but this has not been tested.


## How to cite

If you use gVXR in your own applications, particularly for research & development, I will be grateful if you could cite the articles as follows:

- **Seminal paper:** F.P. Vidal, M. Garnier, N. Freud, J.M. Létang, and N.W. John: Simulation of X-ray attenuation on the GPU. *Proceedings of Theory and Practice of Computer Graphics 2009*, Eurographics Association, Cardiff, UK (2009), pp. 25-32, <a href="https://doi.org/10.2312/LocalChapterEvents/TPCG/TPCG09/025-032">doi: 10.2312/LocalChapterEvents/TPCG/TPCG09/025-032 <Link /></a></li>
- **First reference to gVXR as an opensource software:** F.P. Vidal and P.-F. Villard: Development and validation of real-time simulation of X-ray imaging with respiratory motion. *Computerized Medical Imaging Graphics*, 49 (2016), pp. 1-15, <a href="https://doi.org/10.1016/j.compmedimag.2015.12.002">doi: 10.1016/j.compmedimag.2015.12.002 <Link /></a></li>
- **Clinical validation study:** J.L. Pointon, T. Wen, J. Tugwell-Allsup, A. Sújar, J.M. Létang, and F.P. Vidal: Simulation of X-ray projections on GPU: Benchmarking gVirtualXray with clinically realistic phantoms. *Computer Methods and Programs in Biomedicine*, 234 (2023), pp. 107500, <a href="https://doi.org/10.1016/j.cmpb.2023.107500">doi: 10.1016/j.cmpb.2023.107500 <Link /></a></li>
- **Comprehensive review paper:** Vidal, F. P., Afshari, S., Ahmed, S., Albiol, A., Albiol, F., Béchet, É., … Villard, P.-F. (2025).:
  X-ray simulations with gVXR in education, digital twining, experiment planning, and data analysis
        *Nuclear Instruments and Methods in Physics Research Section B: Beam Interactions with Materials and Atoms*, vol. 568, article 165804, [doi: 10.1016/j.nimb.2025.165804](https://doi.org/10.1016/j.nimb.2025.165804)

```bibtex
@article{POINTON2023107500,
    title = {{Simulation of X-ray projections on GPU: Benchmarking gVirtualXray with clinically realistic phantoms}},
    journal = {Computer Methods and Programs in Biomedicine},
    volume = {234},
    pages = {107500},
    year = {2023},
    issn = {0169-2607},
    doi = {10.1016/j.cmpb.2023.107500},
    author = {Jamie Lea Pointon and Tianci Wen and Jenna Tugwell-Allsup and 
        Aaron S\'ujar and Jean Michel L\'etang and Franck Patrick Vidal}
}

@article{Vidal2016ComputMedImagingGraph,
    author = "Franck P. Vidal and Pierre-Frédéric Villard",
    title = "Development and validation of real-time simulation of X-ray imaging
    with respiratory motion ",
    journal = "Computerized Medical Imaging and Graphics ",
    year = "2016",
    volume = "49",
    pages = "1-15",
    month = apr,
    doi = "10.1016/j.compmedimag.2015.12.002",
    publisher = {Elsevier},
}

@inproceedings{Vidal2009TPCG,
    author = {F. P. Vidal and M. Garnier and N. Freud and J. M. L\'etang and N. W. John},
    title = {Simulation of {X-ray} Attenuation on the {GPU}},
    booktitle = {Proceedings of Theory and Practice of Computer Graphics 2009},
    year = 2009,
    pages = {25-32},
    month = jun,
    address = {Cardiff, UK},
    annotation = {Jun~17--19, 2009},
    note = {Winner of Ken Brodlie Prize for Best Paper},
    doi = {10.2312/LocalChapterEvents/TPCG/TPCG09/025-032},
    publisher = {Eurographics Association},
}

@article{Vidal2025NIMB,
	author = {Vidal, Franck P. and Afshari, Shaghayegh and Ahmed, Sharif and Albiol, Alberto and 
	    Albiol, Francisco and B{\'e}chet, {\'E}ric and Bellot, Alberto Corb{\'\i} and Bosse, Stefan and 
	    Burkhard, Simon and Chahid, Younes and Chou, Cheng-Ying and Culver, Robert and Desbarats, Pascal and 
	    Dixon, Lewis and Friemann, Johan and Garbout, Amin and Garc{\'\i}a-Lorenzo, Marcos and 
	    Giovannelli, Jean-Fran{\c c}ois and Hanna, Ross and Hatton, Cl{\'e}mentine and Henry, Audrey and 
	    Kelly, Graham and Leblanc, Christophe and Leonardi, Alberto and L{\'e}tang, Jean Michel and 
	    Lipscomb, Harry and Manchester, Tristan and Meere, Bas and Michelet, Claire and Middleburgh, Simon and 
	    Mihail, Radu P. and Mitchell, Iwan and Perera, Liam and Puig, Mart{\'\i} and Racy, Malek and 
	    Rouwane, Ali and Seznec, Herv{\'e} and S{\'u}jar, Aaron and Tugwell-Allsup, Jenna and 
	    Villard, Pierre-Fr{\'e}d{\'e}ric},
	doi = {10.1016/j.nimb.2025.165804},
	journal = {Nuclear Instruments and Methods in Physics Research Section B: Beam Interactions with Materials and Atoms},
	keywords = {X-ray imaging; Computed tomography; Simulation; GPU programming; Digital twinning; Registration; Machine learning},
	pages = {165804},
	title = {{X-ray simulations with gVXR in education, digital twining, experiment planning, and data analysis}},
	volume = 568,
	year = 2025
}
```


## Scientific and Industrial Collaboration

If you are interested in any form of collaboration (e.g. to develop your own application), e.g. research papers or grant proposals, drop the package maintainer an email.


<small>&copy; Copyright 2011-2023, Dr Franck P. Vidal, School of Computer science and Electronic Engineering, Bangor University. All rights reserved</small>

<small>&copy; Copyright 2024-, Prof Franck P. Vidal, Computed Tomography, Scientific Computing, Science and Technology Facilities Council (STFC). All rights reserved</small>
