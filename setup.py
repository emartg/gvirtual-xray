#
#
# Copyright 2011-2023, Dr Franck P. Vidal, School of Computer science and 
# Engineering, Bangor University. All rights reserved.
#
# Copyright 2024-, Prof Franck P. Vidal, Computed Tomography, 
# Scientific Computing, Science and Technology Facilities Council (STFC). 
# All rights reserved
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
# this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation and/or
# other materials provided with the distribution.
#
# 3. Neither the names of the Bangor University and STFC nor the names of its contributors
# may be used to endorse or promote products derived from this software without
# specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


import numpy as np
from skbuild import setup
import os
from pathlib import Path


cmake_args = [
    # Build type
    "-DCMAKE_BUILD_TYPE:STRING=Release",

    # Numpy headers
    "-DPython3_NumPy_INCLUDE_DIRS:PATH=" + np.get_include(),

    # Enable/disable functionalities
    "-DBUILD_TESTING:BOOL=OFF",
    "-DBUILD_SIMPLEGVXR:BOOL=ON",
    "-DBUILD_WRAPPER_PYTHON3:BOOL=ON",
    "-DBUILD_SimpleGVXR_DOCUMENTATION:BOOL=ON",
    "-DGLEW_USE_STATIC_LIBS:BOOL=ON",
    "-DBUILD_DEMOS:BOOL=OFF"
    "-DUSE_OPENGL:BOOL=ON",
    "-DUSE_assimp:BOOL=ON",
    "-DUSE_GLAD:BOOL=OFF",
    "-DUSE_GLFW:BOOL=ON",
    "-DUSE_GLEW:BOOL=ON",
    "-DUSE_FFTW3:BOOL=OFF",
    "-DUSE_FREETYPE:BOOL=OFF",
    "-DUSE_GDCM:BOOL=OFF",
    "-DUSE_LIBTIFF:BOOL=ON",
    ]

home = Path.home()
conan_toolchain_path = home / "conan-install/build/Release/generators/conan_toolchain.cmake"

if os.path.exists(conan_toolchain_path):
    print(f"Conan toolchain \"{conan_toolchain_path}\" exists, the toolchain will be used")
    cmake_args.append("-DCMAKE_TOOLCHAIN_FILE:FILEPATH=" + str(conan_toolchain_path))

    # Enable the use of system libraries
    cmake_args.append("-DUSE_SYSTEM_BOOST:BOOL=ON")
    cmake_args.append("-DUSE_SYSTEM_GLEW:BOOL=ON")
    cmake_args.append("-DUSE_SYSTEM_GLAD:BOOL=ON")
    cmake_args.append("-DUSE_SYSTEM_GLFW:BOOL=ON")
    cmake_args.append("-DUSE_SYSTEM_ZLIB:BOOL=ON")
    cmake_args.append("-DUSE_SYSTEM_FREETYPE:BOOL=OFF")
    cmake_args.append("-DUSE_SYSTEM_XRAYLIB:BOOL=OFF")
    cmake_args.append("-DUSE_SYSTEM_ASSIMP:BOOL=ON")
    cmake_args.append("-DUSE_SYSTEM_LIBTIFF:BOOL=ON")
    cmake_args.append("-DUSE_SYSTEM_GTEST:BOOL=ON")
    # cmake_args.append("-DUSE_SYSTEM_FLTK:BOOL=ON")
    # cmake_args.append("-DUSE_SYSTEM_GLUT:BOOL=ON")
    # cmake_args.append("-DUSE_SYSTEM_VCG:BOOL=ON")
    # cmake_args.append("-DUSE_SYSTEM_FFTW3:BOOL=ON")
    # cmake_args.append("-DUSE_SYSTEM_GDCM:BOOL=ON")
else:
    # Disable the use of system libraries
    cmake_args.append("-DUSE_SYSTEM_BOOST:BOOL=OFF")
    cmake_args.append("-DUSE_SYSTEM_GLEW:BOOL=OFF")
    cmake_args.append("-DUSE_SYSTEM_GLFW:BOOL=OFF")
    cmake_args.append("-DUSE_SYSTEM_ZLIB:BOOL=OFF")
    cmake_args.append("-DUSE_SYSTEM_FREETYPE:BOOL=OFF")
    cmake_args.append("-DUSE_SYSTEM_XRAYLIB:BOOL=OFF")
    cmake_args.append("-DUSE_SYSTEM_ASSIMP:BOOL=OFF")
    cmake_args.append("-DUSE_SYSTEM_LIBTIFF:BOOL=OFF")
    cmake_args.append("-DUSE_SYSTEM_FLTK:BOOL=OFF")
    cmake_args.append("-DUSE_SYSTEM_GLUT:BOOL=OFF")
    cmake_args.append("-DUSE_SYSTEM_VCG:BOOL=OFF")
    cmake_args.append("-DUSE_SYSTEM_FFTW3:BOOL=OFF")
    cmake_args.append("-DUSE_SYSTEM_GDCM:BOOL=OFF")


if os.name == "nt":
    cmake_args.append("-DCMAKE_GENERATOR_PLATFORM:STRING=x64")
    cmake_args.append("-DCMAKE_BUILD_PARALLEL_LEVEL:INT=32")

# GNU/Linux or macOS
elif os.name == "posix":
    # Not macOS
    if str(os.uname()).find("Darwin") < 0:
        cmake_args.append("-DCMAKE_EXE_LINKER_FLAGS:STRING=-lrt")
        cmake_args.append("-DCMAKE_MODULE_LINKER_FLAGS:STRING=-lrt")
        cmake_args.append("-DCMAKE_BUILD_PARALLEL_LEVEL:INT=32")


setup(name="gvxr",
    version="2.1.0",
    author="Prof. Franck P. Vidal",
    #description="gVXR bindings, allowing to use some of the gVirtualXray (gVXR) library in python.",
    description="gVirtualXray (gVXR) Bindings to simulate realist X-ray attenuation images in microseconds from triangle meshes.",
    long_description="The gVXR Bindings project allows to use of the gVirtualXray (gVXR), the Virtual X-Ray Imaging Library on GPU with python. It allows the user to simulate realist X-ray attenuation images in microseconds from triangle meshes. This project is still experimental. For more information, please visit https://gvirtualxray.sourceforge.io/",
    long_description_content_type="text/x-rst",    
    url="https://gvirtualxray.sourceforge.io/",
    python_requires=">=3.10",
    packages=["gvxrPython3"],
    include_package_data=True,
    cmake_install_dir="Wrappers/gvxrPython3",
    package_dir={"": "Wrappers"},
    package_data={"gvxrPython3": ["_gvxrPython3/*" , "tests/*.py" , "twins/*.py" , "twins/models/*.json5"]},
    cmake_install_target="install-python",
    cmake_args=cmake_args
)

    #package_data={"gvxrPython3": ["gvxrPython3/*.so", "gvxrPython3/*.dll", "gvxrPython3/*.py"]},
    #package_dir={"gvxrPython3": "gvxrWrapper/python3"},
