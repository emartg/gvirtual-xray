#
#
# Copyright (c) 2022, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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





# Project name and version #####################################################
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.cmake)

if(POLICY CMP0048)
    cmake_policy (SET CMP0048 NEW)
    PROJECT(gVirtualXRay VERSION ${gVirtualXRay_VERSION_STRING})
else ()
    PROJECT(gVirtualXRay)
endif()

set(CMAKE_DIRECTORY_LABELS "gvxr-core")


cmake_policy(SET CMP0072 NEW) # OpenGL
cmake_policy(SET CMP0003 NEW)
cmake_policy(SET CMP0011 NEW)
cmake_policy(SET CMP0013 NEW)
cmake_policy(SET CMP0053 NEW)

IF (APPLE)
    cmake_policy(SET CMP0042 NEW)
ENDIF (APPLE)



SET (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/cmake/advanced_version.cmake)


# C++ version ##################################################################
# Use C++ 23
set(CMAKE_CXX_STANDARD 23) # C++23
set(CMAKE_CXX_STANDARD_REQUIRED ON) # C++23 is required (i.e. not optional)
set(CMAKE_CXX_EXTENSIONS OFF) # without compiler extensions like gnu++11

set (gVirtualXRay_CMAKE_CXX_STANDARD ${CMAKE_CXX_STANDARD} PARENT_SCOPE)


# Enable all compiler warnings #################################################
#IF (NOT WIN32)
#    SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-Wall")
#ENDIF (NOT WIN32)


# Define NOMINMAX ler warnings #################################################
IF (WIN32)
	SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DNOMINMAX")
ENDIF (WIN32)


# Libraries ####################################################################
SET (optionalLibs)

IF (WIN32)
    SET (requiredLibs)
ELSE (WIN32)
    SET (requiredLibs stdc++fs)
ENDIF (WIN32)


SET (gVirtualXRay_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
SET (gVirtualXRay_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
SET (gVirtualXRay_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/)

SET (gVirtualXRay_LIBRARY ${PROJECT_BINARY_DIR}/libgVirtualXRay.a)


IF (UNIX)
	IF (APPLE)

	    SET (requiredLibs ${requiredLibs} "-framework CoreFoundation")

	    IF(${CMAKE_GENERATOR} STREQUAL "Xcode")
	        SET (gVirtualXRay_LIBRARY
	            optimized ${PROJECT_BINARY_DIR}/Release/libgVirtualXRay.a
	            debug     ${PROJECT_BINARY_DIR}/Debug/libgVirtualXRay.a
	        )
	    ENDIF ()
	ELSE (APPLE)
            #FIND_PACKAGE( X11   REQUIRED )

            #IF (NOT X11_Xi_FOUND)
            #    MESSAGE(FATAL_ERROR "X11_Xi not found, you need to install libXi-devel")
            #ENDIF ()

            SET (requiredLibs ${requiredLibs}
                dl
                ${X11_Xi_L}
                rt
            )

	ENDIF (APPLE)
ENDIF ()

if (WIN32 AND ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    string(FIND ${CMAKE_GENERATOR} "Visual Studio" GENERATED_WITH_MSVC)
    if (GENERATED_WITH_MSVC GREATER -1)
        SET (gVirtualXRay_LIBRARY optimized ${PROJECT_BINARY_DIR}/Release/gVirtualXRay.lib
                debug     ${PROJECT_BINARY_DIR}/Debug/gVirtualXRay.lib
        )
    else (GENERATED_WITH_MSVC GREATER -1)
        SET (gVirtualXRay_LIBRARY ${PROJECT_BINARY_DIR}/gVirtualXRay.lib)
    endif (GENERATED_WITH_MSVC GREATER -1)
ENDIF ()


# Find dependencies
INCLUDE (cmake/dependencies.cmake)

        SET (gVirtualXRay_LIBRARY
            ${PROJECT_BINARY_DIR}/libgVirtualXRay.a)
SET (CXX_SOURCES
    # gVirtualXRay
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gVirtualXRay/PythonSingleton.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gVirtualXRay/XRayBeam.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gVirtualXRay/XRayDetector.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gVirtualXRay/XRayRenderer.cxx

    # Graphics
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/CoordinateSystemMesh.cxx
    #${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/CylinderMesh.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/framebufferObject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/Image.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/ImplicitSurface.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/OpenGL2VBO.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/OpenGL3VBO.cxx
    #${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/ParticleSystem.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/PolygonMesh.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/Shader.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/SphereMesh.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/StereoHelper.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/TextRenderer.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/VBO.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/ResourceManager.cxx

    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/SceneGraphNode.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/SceneGraphBinder.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/AssimpSceneGraphBinder.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/OpenSceneGraphBinder.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/GeodeFinder.cxx

    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/AnatomicalMesh.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/InternalOrganMesh.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/SoftTissueMesh.cxx

    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/BoneMesh.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/DiaphragmMesh.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/LiverMesh.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/LungsMesh.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/SkinMesh.cxx

    ${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics/DepthMap.cxx

    # Utilities
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Utilities/AtomicElement.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Utilities/Context.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Utilities/ElementSet.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Utilities/FBOException.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Utilities/initGLEW.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Utilities/initGLAD.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Utilities/Logger.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Utilities/MaterialSet.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Utilities/Mixture.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Utilities/PhotonCrossSection.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Utilities/OpenGLException.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Utilities/OpenGLUtilities.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Utilities/TissueMaterial.cxx
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Utilities/Utilities.cxx

    # XrayPhysics
    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/XrayPhysics/src/xsource.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/XrayPhysics/src/xsec.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/XrayPhysics/src/xsec_raw.cpp
)


IF (USE_GLAD AND NOT USE_SYSTEM_GLAD)
    SET (CXX_SOURCES ${CXX_SOURCES}
            ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glad/src/gl.c
    )

    IF (USE_GLAD AND OpenGL_EGL_FOUND)
        SET (CXX_SOURCES ${CXX_SOURCES}
                ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glad/src/egl.c
        )
    ENDIF (USE_GLAD AND OpenGL_EGL_FOUND)

    IF (USE_GLAD AND OpenGL_GLX_FOUND)
        SET (CXX_SOURCES ${CXX_SOURCES}
                ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glad/src/glx.c
        )
    ENDIF (USE_GLAD AND OpenGL_GLX_FOUND)

    IF (USE_GLAD AND Vulkan_FOUND)
        SET (CXX_SOURCES ${CXX_SOURCES}
                ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glad/src/vulkan.c
        )
    ENDIF (USE_GLAD AND Vulkan_FOUND)
ENDIF (USE_GLAD AND NOT USE_SYSTEM_GLAD)


# Add header files to the project
SET (HXX_SOURCES
    # gVirtualXRay
    ${CMAKE_CURRENT_BINARY_DIR}/gVirtualXRay/gVirtualXRayConfig.h
    
    #${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/filesystem.h

    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/AttenuationCoefficient.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/AttenuationCoefficient.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/PythonSingleton.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/PythonSingleton.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Scintillator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Scintillator.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/SpectrumRecord.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/SpectrumRecord.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/XRayBeam.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/XRayBeam.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/XRayDetector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/XRayDetector.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/XRayRenderer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/XRayRenderer.inl

    # Graphics
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/CoordinateSystemMesh.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/CoordinateSystemMesh.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/CylinderMesh.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/CylinderMesh.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/CubeMesh.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/CubeMesh.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/CuboidMesh.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/CuboidMesh.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/framebufferObject.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/FFT.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/FFT.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Image.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Image.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/ImplicitSurface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/ImplicitSurface.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/LineMesh.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/LineMesh.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Material.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Material.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/OpenGL2VBO.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/OpenGL2VBO.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/OpenGL3VBO.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/OpenGL3VBO.inl
    #${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/ParticleSystem.h
    #${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/ParticleSystem.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/PolygonMesh.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/PolygonMesh.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Shader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Shader.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Sinogram.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Sinogram.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/StepWedgeMesh.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/StepWedgeMesh.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/StereoHelper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/StereoHelper.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/SphereMesh.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/SphereMesh.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/VBO.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/VBO.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/TextRenderer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/TextRenderer.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/ResourceManager.h

    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/SceneGraphNode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/SceneGraphBinder.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/OpenSceneGraphBinder.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/AssimpSceneGraphBinder.h

    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/AnatomicalMesh.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/InternalOrganMesh.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/SoftTissueMesh.h

    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/BoneMesh.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/DiaphragmMesh.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/LiverMesh.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/LungsMesh.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/SkinMesh.h

    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/DepthMap.h

    # Math
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Matrix4x4.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Matrix4x4.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Vec2.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Vec2.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Vec3.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Vec3.inl

    # Utilities
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/AtomicElement.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/AtomicElement.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/ConstantValues.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Context.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/ElementSet.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/ElementSet.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Exception.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Exception.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/FBOException.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/FileDoesNotExistException.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/FileDoesNotExistException.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Logger.h
    #${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Logger.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/MaterialSet.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/MaterialSet.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Mixture.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Mixture.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/OpenGLException.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/OpenGLUtilities.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/OpenGLUtilities.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/OutOfBoundsException.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/OutOfBoundsException.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/OutOfMemoryException.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/OutOfMemoryException.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/PhotonCrossSection.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/PhotonCrossSection.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/TissueMaterial.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/TissueMaterial.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Types.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Units.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Utilities.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gVirtualXRay/Utilities.inl

    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/XrayPhysics/include/xsource.h
    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/XrayPhysics/include/xsec.h
    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/XrayPhysics/include/xsec_raw.h
)

IF (USE_GLAD AND NOT USE_SYSTEM_GLAD)
    SET (HXX_SOURCES ${HXX_SOURCES}
            ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glad/include/glad/gl.h
    )

    IF (USE_GLAD AND OpenGL_EGL_FOUND)
        SET (HXX_SOURCES ${HXX_SOURCES}
                ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glad/include/glad/egl.h
        )
    ENDIF (USE_GLAD AND OpenGL_EGL_FOUND)

    IF (USE_GLAD AND OpenGL_GLX_FOUND)
        SET (HXX_SOURCES ${HXX_SOURCES}
                ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glad/include/glad/glx.h
        )
    ENDIF (USE_GLAD AND OpenGL_GLX_FOUND)

    IF (USE_GLAD AND Vulkan_FOUND)
        SET (HXX_SOURCES ${HXX_SOURCES}
                ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glad/include/glad/vulkan.h
        )
    ENDIF (USE_GLAD AND Vulkan_FOUND)

    SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glad/include)
ENDIF (USE_GLAD AND NOT USE_SYSTEM_GLAD)

SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/XrayPhysics/include)


# Add source to the project ####################################################
IF (WIN32)
    IF (NOT CYGWIN)
        SET (SOURCES ${CXX_SOURCES} ${HXX_SOURCES})
    ELSE(NOT CYGWIN)
        SET (SOURCES ${CXX_SOURCES})
    ENDIF (NOT CYGWIN)
ELSE(WIN32)
    IF (APPLE)
        SET (SOURCES ${CXX_SOURCES} ${HXX_SOURCES})
    ELSE(APPLE)
        SET (SOURCES ${CXX_SOURCES})
    ENDIF (APPLE)
ENDIF (WIN32)


# Create the library ###########################################################
ADD_LIBRARY (gVirtualXRay STATIC ${SOURCES})


# Headers of the project #######################################################
SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${PROJECT_BINARY_DIR}")
SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${CMAKE_CURRENT_SOURCE_DIR}/include")

IF (NOT APPLE)
    SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${CMAKE_CURRENT_SOURCE_DIR}/include/OpenGL4")
ENDIF (NOT APPLE)

#SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "/usr/local/include")


# Boost ########################################################################
IF (NOT USE_SYSTEM_BOOST)
    include(FetchContent)

    Set(FETCHCONTENT_QUIET FALSE)
    set(BOOST_INCLUDE_LIBRARIES interprocess)
    set(BOOST_ENABLE_CMAKE ON)

    FetchContent_Declare(
      Boost
      GIT_PROGRESS TRUE
      #GIT_REPOSITORY https://github.com/boostorg/boost.git
      #GIT_TAG boost-1.87.0
      #URL https://github.com/boostorg/boost/releases/download/boost-1.84.0/boost-1.84.0.tar.xz
      #URL_MD5 893b5203b862eb9bbd08553e24ff146a
      URL https://archives.boost.io/release/1.87.0/source/boost_1_87_0.zip
      URL_MD5 78b0a6c31fc0cc78060455b6df38cf4a
      USES_TERMINAL_DOWNLOAD TRUE
      DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    FetchContent_MakeAvailable(Boost)

    SET (Boost_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/_deps/boost-src)

ELSE (NOT USE_SYSTEM_BOOST)
    FIND_PACKAGE (Boost COMPONENTS interprocess REQUIRED)
ENDIF (NOT USE_SYSTEM_BOOST)

SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_BOOST")
SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${Boost_INCLUDE_DIRS}")


# OpenCL ######################################################################
#FIND_PACKAGE(OpenCL REQUIRED)
#IF (OPENCL_FOUND)
#    SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${OpenCL_INCLUDE_DIRS}")
#    SET (requiredLibs ${requiredLibs} ${OpenCL_LIBRARIES})
#ELSE(OPENCL_FOUND)
#    MESSAGE(WARNING "OpenCL not found.")
#ENDIF (OPENCL_FOUND)





# OpenSceneGraph ###############################################################
IF (USE_OPENSCENEGRAPH)
    FIND_PACKAGE(OpenSceneGraph COMPONENTS osgViewer osgGA)
    IF (OPENSCENEGRAPH_FOUND)
	MESSAGE(STATUS "Use OpenSceneGraph.")
        SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${OPENSCENEGRAPH_INCLUDE_DIRS}")
        SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_OPENSCENEGRAPH")
        SET (optionalLibs ${optionalLibs} ${OPENSCENEGRAPH_LIBRARIES})
    ELSE (OPENSCENEGRAPH_FOUND)
        MESSAGE (WARNING "OpenSceneGraph not found, the use of OpenSceneGraph will be disable.")
    ENDIF (OPENSCENEGRAPH_FOUND)
ENDIF (USE_OPENSCENEGRAPH)

# Create the file
configure_file (
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/gVirtualXRayConfig.h.in"
    "${PROJECT_BINARY_DIR}/gVirtualXRay/gVirtualXRayConfig.h"
)

IF (USE_GLFW)
    SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_GLFW")
ENDIF (USE_GLFW)

# Add preprocessor definitions
ADD_DEFINITIONS(${gVirtualXRay_ADD_DEFINITIONS})
INCLUDE_DIRECTORIES(${gVirtualXRay_INCLUDE_DIRS})


# Add library paths
LINK_DIRECTORIES(${gVirtualXRay_LIBRARY_DIRS})


# Linkage of the target ########################################################
set_target_properties(gVirtualXRay PROPERTIES LINK_SEARCH_START_STATIC ON)
SET (gVirtualXRay_LIBRARIES ${requiredLibs} ${optionalLibs} ${gVirtualXRay_LIBRARY})
TARGET_LINK_LIBRARIES (gVirtualXRay ${requiredLibs} ${optionalLibs})


# Usage requirements of the target ###############################################
# The directory-scoped commands above only apply to this project. A project that links to the target gets the
# same headers and preprocessor definitions from here (the public headers include the ones of the libraries).
ADD_LIBRARY (gVirtualXRay::gVirtualXRay ALIAS gVirtualXRay)
TARGET_INCLUDE_DIRECTORIES (gVirtualXRay PUBLIC ${gVirtualXRay_INCLUDE_DIRS})
TARGET_COMPILE_DEFINITIONS (gVirtualXRay PUBLIC ${gVirtualXRay_ADD_DEFINITIONS})
TARGET_COMPILE_FEATURES    (gVirtualXRay PUBLIC cxx_std_${CMAKE_CXX_STANDARD})



# Create the RESOURCES ###########################################################
INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/cmake/CreateHeaderFiles.cmake)


SET (RESOURCES_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/clean_l_buffer_gl2.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/clean_l_buffer_gl2.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/clean_l_buffer_gl3.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/clean_l_buffer_gl3.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/clean_l_buffer_gl4.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/clean_l_buffer_gl4.vert

    ${CMAKE_CURRENT_SOURCE_DIR}/resources/cumulated_l_buffer_gl2.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/cumulated_l_buffer_gl2.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/cumulated_l_buffer_gl3.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/cumulated_l_buffer_gl3.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/cumulated_l_buffer_gl4.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/cumulated_l_buffer_gl4.vert

    ${CMAKE_CURRENT_SOURCE_DIR}/resources/l_buffer_gl2.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/l_buffer_gl2.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/l_buffer_gl3.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/l_buffer_gl3.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/l_buffer_gl4.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/l_buffer_gl4.vert

    ${CMAKE_CURRENT_SOURCE_DIR}/resources/MUx_Dx_gl2.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/MUx_Dx_gl2.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/MUx_Dx_gl3.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/MUx_Dx_gl3.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/MUx_Dx_gl4.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/MUx_Dx_gl4.vert

    ${CMAKE_CURRENT_SOURCE_DIR}/resources/outer_surface_l_buffer_gl2.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/outer_surface_l_buffer_gl2.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/outer_surface_l_buffer_gl3.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/outer_surface_l_buffer_gl3.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/outer_surface_l_buffer_gl4.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/outer_surface_l_buffer_gl4.vert

    ${CMAKE_CURRENT_SOURCE_DIR}/resources/xray_attenuation_gl2.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/xray_attenuation_gl2.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/xray_attenuation_gl3.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/xray_attenuation_gl3.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/xray_attenuation_gl4.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/xray_attenuation_gl4.vert

    ${CMAKE_CURRENT_SOURCE_DIR}/resources/xray_resample_gl2.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/xray_resample_gl2.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/xray_resample_gl3.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/xray_resample_gl3.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/xray_resample_gl4.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/xray_resample_gl4.vert

    ${CMAKE_CURRENT_SOURCE_DIR}/resources/xray_display_gl2.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/xray_display_gl2.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/xray_display_gl3.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/xray_display_gl3.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/xray_display_gl4.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/xray_display_gl4.vert

    ${CMAKE_CURRENT_SOURCE_DIR}/resources/correlation_CT_number_and_tissue_material.csv
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/mu_water.csv
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/element.csv

    ${CMAKE_CURRENT_SOURCE_DIR}/resources/text_display_gl2.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/text_display_gl2.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/text_display_gl3.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/text_display_gl3.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/text_display_gl4.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/text_display_gl4.vert

    ${CMAKE_CURRENT_SOURCE_DIR}/resources/display_realistic_gl2.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/display_realistic_gl2.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/display_realistic_gl3.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/display_realistic_gl3.vert

    ${CMAKE_CURRENT_SOURCE_DIR}/resources/depth_map_gl2.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/depth_map_gl2.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/depth_map_gl3.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/depth_map_gl3.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/depth_map_gl4.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/depth_map_gl4.vert

    ${CMAKE_CURRENT_SOURCE_DIR}/resources/bump_map_generation_gl2.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/bump_map_generation_gl2.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/bump_map_generation_gl3.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/bump_map_generation_gl3.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/bump_map_generation_gl4.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/bump_map_generation_gl4.vert

    ${CMAKE_CURRENT_SOURCE_DIR}/resources/3d_bump_map_generation_gl2.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/3d_bump_map_generation_gl2.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/3d_bump_map_generation_gl3.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/3d_bump_map_generation_gl3.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/3d_bump_map_generation_gl4.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/3d_bump_map_generation_gl4.vert

    ${CMAKE_CURRENT_SOURCE_DIR}/resources/colour_texture_generation_gl2.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/colour_texture_generation_gl2.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/colour_texture_generation_gl3.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/colour_texture_generation_gl3.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/colour_texture_generation_gl4.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/colour_texture_generation_gl4.vert

    ${CMAKE_CURRENT_SOURCE_DIR}/resources/getSpectrum.py
)

# Add the font file if necessary
IF (USE_FREETYPE)
    SET (RESOURCES_SOURCES ${RESOURCES_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/resources/FreeSans.ttf)
ENDIF (USE_FREETYPE)

FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Bin2C.cmake DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/cmake)
CreateHeaderFiles("cmake" "BinaryLibFiles" "${RESOURCES_SOURCES}")


# Make sure the source is generated before the executable builds.
ADD_DEPENDENCIES(gVirtualXRay BinaryLibFiles)


IF (USE_GLFW AND NOT USE_SYSTEM_GLFW)
        # ADD_DEPENDENCIES(glfw-libs glfw)
        ADD_DEPENDENCIES(gVirtualXRay glfw)
ENDIF (USE_GLFW AND NOT USE_SYSTEM_GLFW)

IF (NOT USE_SYSTEM_ZLIB)
    # ADD_DEPENDENCIES(zlib-libs zlib)
    ADD_DEPENDENCIES(gVirtualXRay zlib)
ENDIF (NOT USE_SYSTEM_ZLIB)

#IF (NOT USE_SYSTEM_VCG)
#    ADD_DEPENDENCIES(gVirtualXRay vcg)
#ENDIF (NOT USE_SYSTEM_VCG)

IF (USE_GLEW AND NOT USE_SYSTEM_GLEW)
    # ADD_DEPENDENCIES(glew-libs glew)
    ADD_DEPENDENCIES(gVirtualXRay glew)
ENDIF (USE_GLEW AND NOT USE_SYSTEM_GLEW)

IF (USE_FREETYPE AND NOT USE_SYSTEM_FREETYPE)
    #ADD_DEPENDENCIES(freetype-libs freetype)
    ADD_DEPENDENCIES(gVirtualXRay freetype)
ENDIF (USE_FREETYPE AND NOT USE_SYSTEM_FREETYPE)

IF (USE_assimp)
    IF (NOT USE_SYSTEM_ASSIMP)
        # ADD_DEPENDENCIES(assimp-libs assimp)
        ADD_DEPENDENCIES(gVirtualXRay assimp)
    ENDIF (NOT USE_SYSTEM_ASSIMP)

    # Make sure ZLIB is build before ASSIMP
    IF (NOT USE_SYSTEM_ZLIB AND NOT USE_SYSTEM_ASSIMP)
        ADD_DEPENDENCIES(assimp zlib)
    ENDIF (NOT USE_SYSTEM_ZLIB AND NOT USE_SYSTEM_ASSIMP)
ENDIF (USE_assimp)

IF (USE_LIBTIFF AND NOT USE_SYSTEM_LIBTIFF)
    # ADD_DEPENDENCIES(libtiff-libs libtiff)
    ADD_DEPENDENCIES(gVirtualXRay libtiff)
ENDIF (USE_LIBTIFF AND NOT USE_SYSTEM_LIBTIFF)

IF (USE_GDCM AND NOT USE_SYSTEM_GDCM)
    ADD_DEPENDENCIES(gdcm-libs gdcm)
    ADD_DEPENDENCIES(gVirtualXRay gdcm-libs)
ENDIF (USE_GDCM AND NOT USE_SYSTEM_GDCM)


# Installation variables #######################################################
if(NOT gVirtualXRay_INSTALL_RUNTIME_DIR)
  set(gVirtualXRay_INSTALL_RUNTIME_DIR bin)
endif()
if(NOT gVirtualXRay_INSTALL_LIBRARY_DIR)
    set(gVirtualXRay_INSTALL_LIBRARY_DIR lib)
endif()
if(NOT gVirtualXRay_INSTALL_ARCHIVE_DIR)
    set(gVirtualXRay_INSTALL_ARCHIVE_DIR lib)
endif()
if(NOT gVirtualXRay_INSTALL_INCLUDE_DIR)
    set(gVirtualXRay_INSTALL_INCLUDE_DIR
        include/gVirtualXRay-${gVirtualXRay_VERSION_MAJOR}.${gVirtualXRay_VERSION_MINOR})
endif()
if(NOT gVirtualXRay_INSTALL_DATA_DIR)
    set(gVirtualXRay_INSTALL_DATA_DIR
        share/gVirtualXRay-${gVirtualXRay_VERSION_MAJOR}.${gVirtualXRay_VERSION_MINOR})
endif()
if(NOT gVirtualXRay_INSTALL_DOC_DIR)
    set(gVirtualXRay_INSTALL_DOC_DIR
        share/doc/gVirtualXRay-${gVirtualXRay_VERSION_MAJOR}.${gVirtualXRay_VERSION_MINOR})
endif()
if(NOT gVirtualXRay_INSTALL_PACKAGE_DIR)
    set(gVirtualXRay_INSTALL_PACKAGE_DIR
        "lib/cmake/gVirtualXRay-${gVirtualXRay_VERSION_MAJOR}.${gVirtualXRay_VERSION_MINOR}")
endif()


# Installation #################################################################
IF (gVXR_INSTALL)
    INSTALL (TARGETS gVirtualXRay ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib64                  COMPONENT Development)
    INSTALL (FILES ${HXX_SOURCES}         DESTINATION ${CMAKE_INSTALL_PREFIX}/include/gVirtualXRay COMPONENT Development)
    INSTALL (FILES ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt DESTINATION ${CMAKE_INSTALL_PREFIX})
ENDIF (gVXR_INSTALL)


# Generate gVirtualXRayConfig.cmake ############################################
set(gVirtualXRay_USE_FILE
    "${CMAKE_INSTALL_PREFIX}/UsegVirtualXRay.cmake"
    PARENT_SCOPE)
configure_file(cmake/gVirtualXRayConfig.cmake.in CMakeFiles/gVirtualXRayConfig.cmake @ONLY)

IF (gVXR_INSTALL)
    INSTALL (FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/UsegVirtualXRay.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/gVirtualXRayConfig.cmake
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Bin2C.cmake
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/CreateHeaderFiles.cmake
        DESTINATION ${CMAKE_INSTALL_PREFIX}
        COMPONENT Development)
ENDIF (gVXR_INSTALL)




# Options ######################################################################



if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    #MESSAGE( "64 bits compiler detected" )
    SET( EX_PLATFORM 64 )
    SET( EX_PLATFORM_NAME "x64" )
else( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    #MESSAGE( "32 bits compiler detected" )
    SET( EX_PLATFORM 32 )
    SET( EX_PLATFORM_NAME "x86" )
endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )


################################################################################
# gVirtualXRay uses KWStyle for checking the coding style
include(cmake/KWStyle/KWStyle.cmake)


# Doxygen documentation ########################################################
IF(BUILD_gVXR_DOCUMENTATION)
    ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/doc/doxygen ${CMAKE_CURRENT_BINARY_DIR}/doc/doxygen)
    MESSAGE(STATUS "Build documentation.")
ELSE (BUILD_gVXR_DOCUMENTATION)
       MESSAGE("Do not build documentation.")
ENDIF(BUILD_gVXR_DOCUMENTATION)


# Tools ########################################################################
#IF (BUILD_TOOLS)
#    MESSAGE(STATUS "Build tools.")
#    ADD_SUBDIRECTORY (${CMAKE_CURRENT_SOURCE_DIR}/tools ${CMAKE_CURRENT_BINARY_DIR}/tools_bin)
#ELSE (BUILD_TOOLS)
#       MESSAGE("Do not build tools.")
#ENDIF (BUILD_TOOLS)


# Examples #####################################################################
#IF (BUILD_EXAMPLES)
#    MESSAGE(STATUS "Build examples.")
#    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples ${CMAKE_CURRENT_BINARY_DIR}/examples_bin)
#ELSE (BUILD_EXAMPLES)
#       MESSAGE("Do not build examples.")
#ENDIF (BUILD_EXAMPLES)


# Tutorials ####################################################################
#IF (BUILD_TUTORIALS)
#       MESSAGE(STATUS "Build tutorials.")
#    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tutorials ${CMAKE_CURRENT_BINARY_DIR}/tutorials_bin)
#ELSE (BUILD_TUTORIALS)
#       MESSAGE("Do not build tutorials.")
#ENDIF (BUILD_TUTORIALS)


# Unit tests ###################################################################
IF(BUILD_TESTING)
    MESSAGE(STATUS "Build unit tests.")
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests ${CMAKE_CURRENT_BINARY_DIR}/tests_bin)
ELSE (BUILD_TESTING)
       MESSAGE("Do not build unit tests.")
ENDIF(BUILD_TESTING)


# Validation tests #############################################################
#IF (BUILD_VALIDATION)
#       MESSAGE(STATUS "Build validation tests.")
#    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/validation ${CMAKE_CURRENT_BINARY_DIR}/validation_bin)
#ELSE (BUILD_VALIDATION)
#       MESSAGE("Do not build validation tests.")
#ENDIF (BUILD_VALIDATION)


# Installation packs ###########################################################
SET (CPACK_PACKAGE_FILE_NAME gVirtualXRay)
SET (CPACK_PACKAGE_ICON ${CMAKE_CURRENT_SOURCE_DIR}/doc/logo.ico)

IF (APPLE)
    SET (CPACK_BUNDLE_NAME gVirtualXRay)
    SET (CPACK_BUNDLE_ICON ${CMAKE_CURRENT_SOURCE_DIR}/examples/build/DragonDemo.icns)
    #SET (CPACK_BUNDLE_PLIST - path to a file that will become the bundle plist.
    #SET (CPACK_BUNDLE_STARTUP_COMMAND - path to a file that will be executed when the user opens the bundle. Could be a shell-script or a binary.
ENDIF (APPLE)


SET (CPACK_SOURCE_IGNORE_FILES "~$")
LIST( APPEND CPACK_SOURCE_INSTALLED_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/cmake     cmake)
LIST( APPEND CPACK_SOURCE_INSTALLED_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include   include)
LIST( APPEND CPACK_SOURCE_INSTALLED_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/src       src)
LIST( APPEND CPACK_SOURCE_INSTALLED_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/resources resources)
LIST( APPEND CPACK_SOURCE_INSTALLED_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/examples  examples)
LIST( APPEND CPACK_SOURCE_INSTALLED_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/tests      tests)
LIST( APPEND CPACK_SOURCE_INSTALLED_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/tutorials tutorials)
LIST( APPEND CPACK_SOURCE_INSTALLED_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/doc/dist_doc1    .)
LIST( APPEND CPACK_SOURCE_INSTALLED_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/doc/dist_doc2   doc)
LIST( APPEND CPACK_SOURCE_INSTALLED_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/doc/html        doc/html)
LIST( APPEND CPACK_SOURCE_INSTALLED_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/doc/TUTORIALS   doc/tutorials)
LIST( APPEND CPACK_SOURCE_INSTALLED_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/doc/screenshots doc/screenshots)
LIST( APPEND CPACK_SOURCE_INSTALLED_FILES       ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt      .)

SET (CPACK_COMPONENT_LIBRARIES_DISPLAY_NAME "Libraries")
SET (CPACK_COMPONENT_HEADERS_DISPLAY_NAME "C++ Headers")

SET (CPACK_COMPONENT_LIBRARIES_DESCRIPTION "Static libraries used to build programs with gVirtualXRay")
SET (CPACK_COMPONENT_HEADERS_DESCRIPTION "C/C++ header files for use with gVirtualXRay")

SET (CPACK_COMPONENT_LIBRARIES_GROUP "Development")
SET (CPACK_COMPONENT_HEADERS_GROUP "Development")

SET (CPACK_COMPONENT_GROUP_DEVELOPMENT_DESCRIPTION "All of the tools you'll ever need to develop software with gVirtualXRay")

SET (CPACK_COMPONENT_LIBRARIES_INSTALL_TYPES Developer Full)
SET (CPACK_COMPONENT_HEADERS_INSTALL_TYPES Developer Full)


IF (WIN32)
    SET (CPACK_GENERATOR NSIS)
    SET (CPACK_NSIS_PACKAGE_NAME ${MAIN_PACKAGE_NAME})
    SET (CPACK_NSIS_DISPLAY_NAME ${MAIN_PACKAGE_NAME})
    SET (CPACK_NSIS_CONTACT ${EMAIL})
    SET (CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${MAIN_PACKAGE_NAME}-${PROJECT_VERSION}")
    SET (CPACK_NSIS_MENU_LINKS "${MAIN_WEBSITE}" "Homepage for ${MAIN_PACKAGE_NAME}")
ENDIF (WIN32)


IF (gVXR_INSTALL AND NOT CPack_CMake_INCLUDED)
    INCLUDE(CPack)
ENDIF (gVXR_INSTALL AND NOT CPack_CMake_INCLUDED)

SET (gVirtualXRay_LIBRARY ${gVirtualXRay_LIBRARY} PARENT_SCOPE)

SET (gVirtualXRay_LIBRARIES ${gVirtualXRay_LIBRARIES} PARENT_SCOPE)

SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} PARENT_SCOPE)

SET (gVirtualXRay_LIBRARY_DIRS ${gVirtualXRay_LIBRARY_DIRS} PARENT_SCOPE)

SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} PARENT_SCOPE)

SET (gVirtualXRay_XRAYLIB_LIBS ${gVirtualXRay_XRAYLIB_LIBS} PARENT_SCOPE)
SET (gVirtualXRay_XRAYLIB_PATH ${gVirtualXRay_XRAYLIB_PATH} PARENT_SCOPE)


function(print_directory_variables dir)
    # Dump variables:
    get_property(_variableNames DIRECTORY ${dir} PROPERTY VARIABLES)
    list (SORT _variableNames)
    foreach (_variableName ${_variableNames})
        get_directory_property(_variableValue DIRECTORY ${dir} DEFINITION ${_variableName})
        message(STATUS "DIR ${dir}: ${_variableName}=${_variableValue}")
    endforeach()
endfunction(print_directory_variables)

#print_directory_variables(.)
