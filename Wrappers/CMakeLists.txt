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
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.cmake)

if(POLICY CMP0048)
    cmake_policy (SET CMP0048 NEW)
    PROJECT(gvxrWrappers VERSION
        ${gvxrWrappers_VERSION_STRING}
        #LANGUAGES CXX CUDA
    )
else ()
    PROJECT(gvxrWrappers
      #  LANGUAGES CXX CUDA
    )
endif()
set(CMAKE_DIRECTORY_LABELS "gvxrWrappers")


# Support for the CTest dashboard testing system
INCLUDE(CTest)

# Initialise some states by default ############################################
SET (BUILD_SHARED_LIBS OFF)


# Minimum CMake version ########################################################
set(CMAKE_CXX_STANDARD 23) # C++23
set(CMAKE_CXX_STANDARD_REQUIRED ON) # C++23 is required (i.e. not optional)
set(CMAKE_CXX_EXTENSIONS OFF) # without compiler extensions like gnu++11


if(${CMAKE_VERSION} VERSION_LESS "3.13.0")
    message("Please consider to switch to CMake 3.13.0")
endif()


IF (MSVC)
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
ENDIF (MSVC)


if(${CMAKE_VERSION} VERSION_GREATER "3.13.0" OR ${CMAKE_VERSION} VERSION_EQUAL "3.13.0")
    cmake_policy(SET CMP0078 NEW) # SWIG
endif()


if(${CMAKE_VERSION} VERSION_GREATER "3.14.0" OR ${CMAKE_VERSION} VERSION_EQUAL "3.14.0")
    cmake_policy(SET CMP0086 NEW) # SWIG
endif()


OPTION(USE_SYSTEM_SimpleGVXR   "Use System SimpleGVXR"      ON)


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


# Find SimpleGVXR ############################################################
IF (NOT USE_SUPERBUILD)
    IF (NOT USE_SYSTEM_SimpleGVXR)
    		SET (USE_SUPERBUILD ON)
    		INCLUDE(ExternalProject)
    ENDIF (NOT USE_SYSTEM_SimpleGVXR)

    FIND_PACKAGE(SimpleGVXR REQUIRED)
    INCLUDE(${SimpleGVXR_USE_FILE})
ELSE ()

    # C++ version
    set (CMAKE_CXX_STANDARD "${gVirtualXRay_CMAKE_CXX_STANDARD}")

    # C++ flags
    SET(CMAKE_C_FLAGS   "${gVirtualXRay_CMAKE_C_FLAGS}")
    SET(CMAKE_CXX_FLAGS "${gVirtualXRay_CMAKE_CXX_FLAGS}")

    # Add include directories needed to use SimpleGVXR
    include_directories(${gVirtualXRay_INCLUDE_DIR})
    include_directories(${gVirtualXRay_INCLUDE_DIRS})
    include_directories(${SimpleGVXR_INCLUDE_DIR})
    include_directories(${NUMPY_INCLUDE_DIR})

    # Add link directories needed to use SimpleGVXR
    link_directories(${gVirtualXRay_LIBRARY_DIRS})
ENDIF ()


#find_package(CUDAToolkit)
#IF (CUDAToolkit_FOUND)
#    SET (optionalLibs CUDA::cudart_static CUDA::cudart ${optionalLibs})
#ENDIF (CUDAToolkit_FOUND)


# OpenMP ######################################################################
FIND_PACKAGE(OpenMP COMPONENTS CXX)

IF (OpenMP_CXX_FOUND)

    IF (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        MESSAGE (STATUS "MSVC detected")
        MESSAGE (STATUS "OpenMP enabled with  -openmp:llvm -openmp:experimental")   
        SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -openmp:llvm -openmp:experimental" PARENT_SCOPE)
    ELSE ()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}" PARENT_SCOPE)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}" PARENT_SCOPE)
    ENDIF()

    SET (optionalLibs ${optionalLibs} OpenMP::OpenMP_CXX  PARENT_SCOPE)
ENDIF (OpenMP_CXX_FOUND)


# SWIG ########################################################################
FIND_PACKAGE(SWIG)

# It has not been found
IF (NOT SWIG_FOUND)
    # It is a Windows system, download its corresponding binaries
    IF (WIN32)
        file(DOWNLOAD https://sourceforge.net/projects/swig/files/swigwin/swigwin-4.4.1/swigwin-4.4.1.zip ${PROJECT_BINARY_DIR}/swigwin-4.4.1.zip)

        # Unzip swigwin-4.4.1.zip
        execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${PROJECT_BINARY_DIR}/swigwin-4.4.1.zip
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR})

        # Set the corresponding variables for CMake
        SET (SWIG_EXECUTABLE ${PROJECT_BINARY_DIR}/swigwin-4.4.1/swig.exe CACHE FILEPATH "Where swig.exe is" FORCE)
        SET (SWIG_DIR        ${PROJECT_BINARY_DIR}/swigwin-4.4.1/Lib      CACHE PATH     "Where Swig is"     FORCE)

        # Try again, but make it required this time
        FIND_PACKAGE(SWIG REQUIRED)

    # It is not a Windows system, install it
    ELSE (WIN32)
        MESSAGE(WARNING "SWIG not found, please install it first.")
    ENDIF (WIN32)
ELSE (NOT SWIG_FOUND)

    string(COMPARE LESS "${SWIG_VERSION}" "3.0.0" SWIG_VERSION_ERROR)

    if(SWIG_VERSION_ERROR)
	    MESSAGE(FATAL_ERROR "Swig version must be 3.0.0 or greater! (You have ${SWIG_VERSION})")
    ENDIF ()
ENDIF (NOT SWIG_FOUND)


file(WRITE
    ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/runAll.sh
    "echo Run all the tests.\n"
)


# OpenGL ######################################################################
FIND_PACKAGE(OpenGL REQUIRED COMPONENTS OpenGL)
FIND_PACKAGE(OpenGL COMPONENTS GLX EGL)

if (USE_SYSTEM_LIBTIFF)
    find_package(TIFF REQUIRED)
endif ()

if (USE_SYSTEM_ASSIMP)
    find_package(assimp REQUIRED)
endif ()

# SWIG has been found
IF (SWIG_FOUND)

    SET (CPACK_COMPONENTS_ALL Demos)

    INCLUDE (UseSWIG)

    SET (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

    set(CMAKE_SWIG_FLAGS "")
    #include_directories(${CMAKE_CURRENT_SOURCE_DIR})
    #include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../SimpleGVXR)

    # The options ##################################################################
    INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/cmake/options.cmake)




    IF (BUILD_WRAPPER_PYTHON2 AND BUILD_WRAPPER_PYTHON3)
        MESSAGE(FATAL_ERROR "Both variables BUILD_WRAPPER_PYTHON2 and BUILD_WRAPPER_PYTHON3 are ON. Choose either Python2 or Python3, not both.")
    ENDIF (BUILD_WRAPPER_PYTHON2 AND BUILD_WRAPPER_PYTHON3)

    IF (BUILD_WRAPPER_CSHARP)
        add_subdirectory(csharp)

        file(APPEND
            ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/runAll.sh
            "echo Run C# test.\n"
            "cd csharp;./runMonoTest.sh;cd ..\n"
        )

    ENDIF (BUILD_WRAPPER_CSHARP)

    IF (BUILD_WRAPPER_LUA)
        MESSAGE(STATUS "Lua wrapper not implemented as yet, Franck is working on it.")
        #add_subdirectory(lua)
    ENDIF (BUILD_WRAPPER_LUA)

    IF (BUILD_WRAPPER_JAVA)
        add_subdirectory(java)

        file(APPEND
            ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/runAll.sh
            "echo Run Java test.\n"
            "cd java;./runJavaTest.sh;cd ..\n"
        )
    ENDIF (BUILD_WRAPPER_JAVA)

    IF (BUILD_WRAPPER_JAVASCRIPT)
        MESSAGE(STATUS "JavaScript wrapper not implemented as yet, Franck is working on it.")
        #add_subdirectory(js)
    ENDIF (BUILD_WRAPPER_JAVASCRIPT)

    IF (BUILD_WRAPPER_MATLAB)
        MESSAGE(STATUS "Matlab wrapper not implemented as yet, Franck is working on it.")
        #add_subdirectory(matlab)
    ENDIF (BUILD_WRAPPER_MATLAB)

    IF (BUILD_WRAPPER_OCTAVE)
        add_subdirectory(octave)

        file(APPEND
            ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/runAll.sh
            "echo Run GNU Octave test.\n"
            "cd octave;./runOctaveTest.sh;cd ..\n"
        )
    ENDIF (BUILD_WRAPPER_OCTAVE)

    IF (BUILD_WRAPPER_PERL)
        add_subdirectory(perl)

        file(APPEND
            ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/runAll.sh
            "echo Run Perl test.\n"
            "cd perl;./runPerlTest.sh;cd ..\n"
        )
    ENDIF (BUILD_WRAPPER_PERL)

    IF (BUILD_WRAPPER_PYTHON2)
        add_subdirectory(python2)

        file(APPEND
            ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/runAll.sh
            "echo Run Python 2 test.\n"
            "cd python2;./test.py;cd ..\n"
        )
    ENDIF (BUILD_WRAPPER_PYTHON2)

    IF (BUILD_WRAPPER_PYTHON3)
        add_subdirectory(gvxrPython3)

        file(APPEND
            ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/runAll.sh
            "echo Run Python 3 test.\n"
            "cd python3;./test.py;cd ..\n"
        )
    ENDIF (BUILD_WRAPPER_PYTHON3)

    IF (BUILD_WRAPPER_R)
        add_subdirectory(R)

        file(APPEND
            ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/runAll.sh
            "echo Run R test.\n"
            "cd R;./test.R;cd ..\n"
        )
    ENDIF (BUILD_WRAPPER_R)

    IF (BUILD_WRAPPER_RUBY)
        add_subdirectory(ruby)

        file(APPEND
            ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/runAll.sh
            "echo Run ruby test.\n"
            "cd ruby;./runRubyTest.sh;cd ..\n"
        )
    ENDIF (BUILD_WRAPPER_RUBY)

    IF (BUILD_WRAPPER_TCL)
        add_subdirectory(Tcl)

        file(APPEND
            ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/runAll.sh
            "echo Run Tcl test.\n"
            "cd Tcl;./test.tcl;cd ..\n"
        )
    ENDIF (BUILD_WRAPPER_TCL)
ENDIF (SWIG_FOUND)


IF (USE_SYSTEM_GLEW)
    find_package(GLEW REQUIRED)
    IF (GLEW_USE_STATIC_LIBS)
        SET (requiredLibs GLEW::glew_s ${requiredLibs})
    ELSE (GLEW_USE_STATIC_LIBS)
        SET (requiredLibs GLEW::GLEW ${requiredLibs})
    ENDIF (GLEW_USE_STATIC_LIBS)
ELSE (USE_SYSTEM_GLEW)
    SET (requiredLibs ${GLEW_LIBRARY} ${requiredLibs})
ENDIF ()


#install(FILES
#    welsh-dragon-small.stl
#    DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/data
#    COMPONENT Demos)

IF (UNIX)
    FILE(COPY ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/runAll.sh
    		DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
    		FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ
      			GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

#    install(FILES
#        ${CMAKE_CURRENT_BINARY_DIR}/runAll.sh
#        DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}
#        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
#        COMPONENT Demos)
ENDIF (UNIX)

INCLUDE(CPack.cmake)
