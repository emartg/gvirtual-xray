#
#
# Copyright (c) 2020, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
# http://www.fpvidal.net/
# All rights reserved.
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
# 3. Neither the name of the Bangor University nor the names of its contributors
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

SET (SimpleGVXR_LICENSE )
SET (PROJECT_NAME         "SimpleGVXR")
SET (PROJECT_DESCRIPTION  "Simple Virtual X-Ray Imaging Library on GPU")
SET (EMAIL                "franck.p.vidal@fpvidal.net")
SET (MAIN_WEBSITE         "http://gvirtualxray.sourceforge.net/")
SET (MAIN_PACKAGE_NAME    "${PROJECT_NAME}")
SET (MAIN_PROJECT_VERSION ${SimpleGVXR_VERSION_STRING})
SET (FULL_PROJECT_DESCRIPTION "Welcome to SimpleGVXR, a simplified layer built on top of gVirtualXRay -- the open-source cross-platform library for the simulation of X-ray images from polygon meshes on the GPU. SimpleGVXR: i) is a C++ library, ii) provides a simplified, easy-to-use, and procedural interface to gVirtualXRay without templates, and iii) provides binders for other programming languages (Python 2 and 3, Ruby, R, TCL, Java, GNU Octave, Perl, and C#, and soon Matlab).")

STRING(TIMESTAMP TIMESTAMP)

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "AppleClang")
    SET (COMPILER "Apple Clang")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    SET (COMPILER "LLVM Clang")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Cray")
    SET (COMPILER "Cray Compiler")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Embarcadero")
    SET (COMPILER "Embarcadero")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Borland")
    SET (COMPILER "Embarcadero")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Embarcadero, Borland")
    SET (COMPILER "Embarcadero")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    SET (COMPILER "GNU g++")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "HP")
    SET (COMPILER "Hewlett-Packard Compiler")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    SET (COMPILER "Intel Compiler")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MIPSpro")
    SET (COMPILER "SGI MIPSpro")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    SET (COMPILER "Microsoft Visual Studio")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "PGI")
    SET (COMPILER "The Portland Group")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "PathScale")
    SET (COMPILER "PathScale")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "SDCC")
    SET (COMPILER "Small Device C Compiler")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "SunPro")
    SET (COMPILER "Oracle Solaris Studio")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "TI")
    SET (COMPILER "Texas Instruments")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "TinyCC")
    SET (COMPILER "Tiny C Compiler")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Watcom")
    SET (COMPILER "Open Watcom")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "XL")
    SET (COMPILER "IBM XL")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "VisualAge")
    SET (COMPILER "IBM XL")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "zOS")
    SET (COMPILER "IBM XL")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "XL, VisualAge, zOS")
    SET (COMPILER "IBM XL")
endif()

SET (SimpleGVXR_LONG_VERSION "SimpleGVXR ${SimpleGVXR_VERSION_MAJOR}.${SimpleGVXR_VERSION_MINOR}.${SimpleGVXR_VERSION_PATCH} (${TIMESTAMP}) [Compiler: ${COMPILER}] on ${CMAKE_SYSTEM_NAME}")

#SET (INSTALL_DIR "${PROJECT_NAME}-${SimpleGVXR_VERSION_MAJOR}.${SimpleGVXR_VERSION_MINOR}.${SimpleGVXR_VERSION_PATCH}")
SET (INSTALL_DIR "${PROJECT_NAME}")

SET (CPACK_PACKAGE_VERSION_MAJOR ${SimpleGVXR_VERSION_MAJOR})
SET (CPACK_PACKAGE_VERSION_MINOR ${SimpleGVXR_VERSION_MINOR})
SET (CPACK_PACKAGE_VERSION_PATCH ${SimpleGVXR_VERSION_PATCH})
SET (CPACK_PACKAGE_VERSION ${SimpleGVXR_VERSION_STRING})

SET (CPACK_PACKAGE_VENDOR "gVirtualXRay team")
SET (CPACK_PACKAGE_DESCRIPTION_SUMMARY "gVirtualXRay - Installation")
SET (CPACK_PACKAGE_INSTALL_DIRECTORY "gVirtualXRay")

SET (CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt)

SET (COPYRIGHT_STRING "(C) 2019 Dr Franck P. Vidal")
