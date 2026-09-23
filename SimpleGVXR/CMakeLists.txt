#
#
# Copyright (c) 2019, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/version.cmake)

if(POLICY CMP0048)
    cmake_policy (SET CMP0048 NEW)
    PROJECT(SimpleGVXR
        VERSION ${SimpleGVXR_VERSION_STRING}
        #LANGUAGES CXX #CUDA
    )
else ()
    PROJECT(SimpleGVXR
        #LANGUAGES CXX #CUDA
    )
endif()
set(CMAKE_DIRECTORY_LABELS "SimpleGVXR")

INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/cmake/advanced_version.cmake)


# Support for the CTest dashboard testing system
INCLUDE(CTest)


# Initialise some states by default ############################################
SET (BUILD_SHARED_LIBS OFF)


# Minimum CMake version ########################################################
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 23) # C++23
set(CMAKE_CXX_STANDARD_REQUIRED ON) # C++23 is required (i.e. not optional)
set(CMAKE_CXX_EXTENSIONS OFF) # without compiler extensions like gnu++11

OPTION(USE_SYSTEM_gVirtualXRay   "Use System gVirtualXRay"      ON)

FIND_PACKAGE(Doxygen)
IF (Doxygen_FOUND)
    OPTION(BUILD_SimpleGVXR_DOCUMENTATION         "Build SimpleGVXR's documentation using Doxygen" ON)
ELSE (Doxygen_FOUND)
    OPTION(BUILD_SimpleGVXR_DOCUMENTATION         "Build SimpleGVXR's documentation using Doxygen" OFF)
ENDIF (Doxygen_FOUND)

IF (USE_GLEW)
    IF (USE_SYSTEM_GLEW)
        find_package(GLEW REQUIRED)
        IF (GLEW_USE_STATIC_LIBS)
            SET (requiredLibs GLEW::glew_s ${requiredLibs})
        ELSE (GLEW_USE_STATIC_LIBS)
            SET (requiredLibs GLEW::GLEW ${requiredLibs})
        ENDIF (GLEW_USE_STATIC_LIBS)
    ELSE (USE_SYSTEM_GLEW)
        SET (requiredLibs ${GLEW_LIBRARY} ${requiredLibs})
    ENDIF (USE_SYSTEM_GLEW)
ENDIF (USE_GLEW)

IF (USE_GLAD AND USE_SYSTEM_GLAD)
        FIND_PACKAGE( glad REQUIRED)
ENDIF (USE_GLAD AND USE_SYSTEM_GLAD)

IF (USE_GLFW)
    IF (USE_SYSTEM_GLFW)
        FIND_PACKAGE(glfw3 REQUIRED)
    ENDIF (USE_SYSTEM_GLFW)
    SET (requiredLibs ${glfw3_LIBRARIES} ${requiredLibs})
ENDIF (USE_GLFW)


# Set a default build type if none was specified
IF (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
      message(STATUS "Setting build type to 'Release' as none was specified.")
      SET (CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)

      # Set the possible values of build type for cmake-gui
      set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
ENDIF ()


# Enable the unit tests
if (BUILD_TESTING)
    ENABLE_TESTING()
endif()


# find_package(CUDAToolkit)
#IF (CUDAToolkit_FOUND)
#    add_compile_definitions(HAS_CUDA)
#    include_directories(${CUDAToolkit_INCLUDE_DIRS})
#    MESSAGE (STATUS "CUDAToolkit found, oversampling is going to be supported")
#    SET (optionalLibs CUDA::cudart_static CUDA::cudart ${optionalLibs})
#ELSE (CUDAToolkit_FOUND)
#    MESSAGE (WARNING "CUDAToolkit not found, oversampling is NOT going to be supported")
#ENDIF (CUDAToolkit_FOUND)


# for superbuild ###############################################################
SET (SimpleGVXR_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})


################################################################################
# gVirtualXRay uses KWStyle for checking the coding style
include(cmake/KWStyle/KWStyle.cmake)


# Doxygen documentation ########################################################
IF(BUILD_SimpleGVXR_DOCUMENTATION)
    ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/doc ${CMAKE_CURRENT_BINARY_DIR}/doc)
    MESSAGE(STATUS "Build documentation.")
ELSE (BUILD_SimpleGVXR_DOCUMENTATION)
       MESSAGE("Do not build documentation.")
ENDIF(BUILD_SimpleGVXR_DOCUMENTATION)


# Find gVirtualXRay ############################################################
IF (NOT USE_SUPERBUILD)
    IF (NOT USE_SYSTEM_gVirtualXRay)
    		SET (USE_SUPERBUILD ON)
    		INCLUDE(ExternalProject)
    ENDIF (NOT USE_SYSTEM_gVirtualXRay)

    FIND_PACKAGE(gVirtualXRay REQUIRED)
    INCLUDE(${gVirtualXRay_USE_FILE})
ELSE ()

    # C++ version
    set (CMAKE_CXX_STANDARD "${gVirtualXRay_CMAKE_CXX_STANDARD}")

    # Add include directories needed to use gVirtualXRay
    include_directories(${gVirtualXRay_INCLUDE_DIR})
    include_directories(${gVirtualXRay_INCLUDE_DIRS})

    # Add link directories needed to use gVirtualXRay
    link_directories(${gVirtualXRay_LIBRARY_DIRS})

    ADD_DEFINITIONS(${gVirtualXRay_ADD_DEFINITIONS})
ENDIF ()


# Create the RESOURCES #########################################################
FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Bin2C.cmake DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/cmake)
SET (RESOURCES_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/display_gl2.vert"
    "${CMAKE_CURRENT_SOURCE_DIR}/display_gl2.frag"
    "${CMAKE_CURRENT_SOURCE_DIR}/display_gl3.vert"
    "${CMAKE_CURRENT_SOURCE_DIR}/display_gl3.frag"
    "${CMAKE_CURRENT_SOURCE_DIR}/display_gl4.vert"
    "${CMAKE_CURRENT_SOURCE_DIR}/display_gl4.frag"
)

INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/cmake/CreateHeaderFiles.cmake)
CreateHeaderFiles("cmake" "BinaryWrapper" "${RESOURCES_SOURCES}")


# Headers of the project #######################################################
SET (SimpleGVXR_BASE_DIR       ${CMAKE_CURRENT_SOURCE_DIR})
SET (SimpleGVXR_INCLUDE_DIR    ${CMAKE_CURRENT_SOURCE_DIR}/include)
SET (SimpleGVXR_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR})

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories("${CMAKE_CURRENT_BINARY_DIR}")


ADD_DEFINITIONS("-DSimpleGVXR_VERSION_MAJOR=${SimpleGVXR_VERSION_MAJOR}")
ADD_DEFINITIONS("-DSimpleGVXR_VERSION_MINOR=${SimpleGVXR_VERSION_MINOR}")
ADD_DEFINITIONS("-DSimpleGVXR_VERSION_PATCH=${SimpleGVXR_VERSION_PATCH}")
ADD_DEFINITIONS("-DSimpleGVXR_VERSION_STRING=\"${SimpleGVXR_LONG_VERSION}\"")


# Create the library ###########################################################
#IF (CUDAToolkit_FOUND)
#    add_library(SimpleGVXR
#        SimpleGVXR.cxx
#        SimpleGVXR.cu
#        Window.cxx
#        Windowless.cxx
#        OpenGLWindow.cxx
#        VulkanWindow.cxx
#    )
#ELSE (CUDAToolkit_FOUND)
    add_library(SimpleGVXR
        src/internal.cxx
        src/acquisition.cxx
        src/detector.cxx
        src/ImplicitSurface.cxx
        src/IsoSurface.cxx
        src/sample.cxx
        src/source.cxx
        src/Utils.cxx
        src/visualisation.cxx
        src/SimpleGVXR.cxx
        src/Window.cxx
        src/Windowless.cxx
        src/OpenGLWindow.cxx
        src/VulkanWindow.cxx
    )
#ENDIF (CUDAToolkit_FOUND)


target_link_libraries(SimpleGVXR ${gVirtualXRay_LIBRARY} ${gVirtualXRay_LIBRARIES})

#IF (CUDA_FOUND)
#    target_link_libraries(SimpleGVXR ${gVirtualXRay_LIBRARY} ${gVirtualXRay_LIBRARIES} ${CUDA_LIBRARIES})
#    MESSAGE(FATAL_ERROR ${CUDA_LIBRARIES})
#ELSE (CUDA_FOUND)
    target_link_libraries(SimpleGVXR ${gVirtualXRay_LIBRARY} ${gVirtualXRay_LIBRARIES})
#ENDIF (CUDA_FOUND)
ADD_DEPENDENCIES(SimpleGVXR BinaryWrapper)

IF (USE_SUPERBUILD)
    ADD_DEPENDENCIES(SimpleGVXR   gVirtualXRay)
ENDIF ()




SET (SimpleGVXR_LIBRARY ${PROJECT_BINARY_DIR}/libSimpleGVXR.a)


# OpenGL ######################################################################
IF (USE_OPENGL)
    IF (WIN32 OR APPLE)
        FIND_PACKAGE(OpenGL REQUIRED)
        SET (requiredLibs ${requiredLibs} ${OPENGL_LIBRARIES})
    ELSE (WIN32 OR APPLE)
        FIND_PACKAGE(OpenGL COMPONENTS OpenGL)

        IF (OpenGL_OpenGL_FOUND)
            FIND_PACKAGE(OpenGL COMPONENTS EGL GLX)
            SET (requiredLibs ${requiredLibs} OpenGL::OpenGL)

            IF (OpenGL_GLX_FOUND)
                SET (optionalLibs ${optionalLibs} OpenGL::GLX)
            ENDIF (OpenGL_GLX_FOUND)

            IF (OpenGL_EGL_FOUND)
                SET (optionalLibs ${optionalLibs} OpenGL::EGL)
            ENDIF (OpenGL_EGL_FOUND)
        ENDIF (OpenGL_OpenGL_FOUND)
    ENDIF (WIN32 OR APPLE)
ENDIF (USE_OPENGL)


# OpenMP ######################################################################
FIND_PACKAGE(OpenMP COMPONENTS CXX)

IF (OpenMP_CXX_FOUND)

    #IF (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    #    MESSAGE (STATUS "MSVC detected")
    #    MESSAGE (STATUS "OpenMP enabled with  -openmp:llvm -openmp:experimental")   
    #    SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -openmp:llvm -openmp:experimental")
    #    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -openmp:llvm -openmp:experimental")
    #ELSE ()
    #    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    #    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
    #ENDIF()


    SET (optionalLibs ${optionalLibs} OpenMP::OpenMP_CXX)
ENDIF (OpenMP_CXX_FOUND)

if (USE_SYSTEM_LIBTIFF)
    find_package(TIFF REQUIRED)
endif ()

if (USE_SYSTEM_ASSIMP)
    find_package(assimp REQUIRED)
endif ()

IF (UNIX)
	IF (APPLE)

	    SET (requiredLibs ${requiredLibs} "-framework CoreFoundation")

	    IF(${CMAKE_GENERATOR} STREQUAL "Xcode")
	        SET (SimpleGVXR_LIBRARY
	            optimized ${PROJECT_BINARY_DIR}/Release/libSimpleGVXR.a
	            debug     ${PROJECT_BINARY_DIR}/Debug/libSimpleGVXR.a
	        )
	    ENDIF ()
	ELSE (APPLE)
		SET (requiredLibs ${requiredLibs} dl)
	ENDIF (APPLE)
ENDIF ()




if (WIN32 AND ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    string(FIND ${CMAKE_GENERATOR} "Visual Studio" GENERATED_WITH_MSVC)
    if (GENERATED_WITH_MSVC GREATER -1)
        SET (SimpleGVXR_LIBRARY optimized ${PROJECT_BINARY_DIR}/Release/SimpleGVXR.lib
                                  debug     ${PROJECT_BINARY_DIR}/Debug/SimpleGVXR.lib
        )
    else (GENERATED_WITH_MSVC GREATER -1)
        SET (SimpleGVXR_LIBRARY ${PROJECT_BINARY_DIR}/SimpleGVXR.lib)
    endif (GENERATED_WITH_MSVC GREATER -1)
ENDIF ()

SET (SimpleGVXR_LIBRARIES
    ${optionalLibs}
    ${requiredLibs}
    ${SimpleGVXR_LIBRARY}
    ${gVirtualXRay_LIBRARY}
    ${gVirtualXRay_LIBRARIES}
    )


# Set up public headers for SimpleGVXR #########################################
set_target_properties(SimpleGVXR PROPERTIES PUBLIC_HEADER
    include/SimpleGVXR.h
    include/acquisition.h
    include/detector.h
    include/ImplicitSurface.h
    #include/IsoSurface.h  ## Not sure why we can't add this one
    include/sample.h
    include/source.h
    include/Utils.h
    include/visualisation.h
    include/SimpleGVXR.h
    include/Window.h
    include/Windowless.h
    include/OpenGLWindow.h
    include/VulkanWindow.h
)


# Generate SimpleGVXRConfig.cmake ############################################
set(SimpleGVXR_USE_FILE "${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/UseSimpleGVXR.cmake")
configure_file(SimpleGVXRConfig.cmake.in CMakeFiles/SimpleGVXRConfig.cmake @ONLY)

install(FILES
		${CMAKE_CURRENT_SOURCE_DIR}/UseSimpleGVXR.cmake
		${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/SimpleGVXRConfig.cmake
    DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}
    COMPONENT Development)




SET (SimpleGVXR_LIBRARY ${SimpleGVXR_LIBRARY} PARENT_SCOPE)
SET (SimpleGVXR_LIBRARIES ${SimpleGVXR_LIBRARIES} PARENT_SCOPE)
SET (SimpleGVXR_INCLUDE_DIR ${SimpleGVXR_INCLUDE_DIR} PARENT_SCOPE)
SET (SimpleGVXR_Doxy_XML ${PROJECT_BINARY_DIR}/doc/xml PARENT_SCOPE)


# Build demos ##################################################################
IF (BUILD_DEMOS)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/demos ${CMAKE_CURRENT_BINARY_DIR}/demos_bin)
ENDIF (BUILD_DEMOS)


# Unit tests ###################################################################
IF(BUILD_TESTING)
    MESSAGE(STATUS "Build unit tests.")
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests ${CMAKE_CURRENT_BINARY_DIR}/tests_bin)
ELSE (BUILD_TESTING)
       MESSAGE("Do not build unit tests.")
ENDIF(BUILD_TESTING)


# Installation #################################################################
install(TARGETS SimpleGVXR
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/bin
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/lib
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/lib
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/include)
