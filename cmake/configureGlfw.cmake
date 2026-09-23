#
#
# Copyright (c) 2014, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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


# Set the policy
cmake_policy(SET CMP0011 NEW)


# Fix linkage problem with windows and cross-compilation #######################
IF (WIN32)
    ADD_DEFINITIONS(-DGLEW_STATIC)
    ADD_DEFINITIONS(-DFREEGLUT_STATIC)
    #SET (CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -lwinmm")
ENDIF (WIN32)


# GLFW ########################################################################



IF (glfw3_FOUND)
    MESSAGE(STATUS "Use GLFW3")

    LINK_DIRECTORIES(${glfw3_LIBRARY_DIR})
    #SET(optionalLibs ${optionalLibs} glfw3)

    IF (APPLE)
        FIND_LIBRARY(COCOA_LIBRARY Cocoa)
        FIND_LIBRARY(IOKIT_LIBRARY IOKit)
        FIND_LIBRARY(QUARTZ_LIBRARY Quartz)
        FIND_LIBRARY(CORE_VIDEO_LIBRARY CoreVideo)

        SET (X11_ROOT /usr/local)
        SET (CMAKE_PREFIX_PATH ${X11_ROOT} ${CMAKE_PREFIX_PATH})

        SET(optionalLibs ${optionalLibs} ${COCOA_LIBRARY} ${IOKIT_LIBRARY} ${QUARTZ_LIBRARY} ${CORE_VIDEO_LIBRARY})
    ENDIF(APPLE)

    IF (UNIX)
        FIND_PACKAGE (X11)

        #set(THREADS_PREFER_PTHREAD_FLAG ON)
        #find_package(Threads REQUIRED)

        #SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")

        SET(optionalLibs dl  ${optionalLibs})

        IF (X11_FOUND)
            INCLUDE_DIRECTORIES(${X11_INCLUDE_DIRS})
            SET(optionalLibs ${optionalLibs} ${X11_X11_LIB} ${X11_Xrandr_LIB} ${X11_Xinput_LIB} ${X11_Xcursor_LIB} ${X11_Xinerama_LIB})
        ENDIF (X11_FOUND)

        #SET(optionalLibs ${optionalLibs} ${CMAKE_THREAD_LIBS_INIT})
    ENDIF(UNIX)

    INCLUDE_DIRECTORIES(${glfw3_INCLUDE_DIR})
    SET(optionalLibs ${optionalLibs} ${glfw3_LIBRARY})
ENDIF(glfw3_FOUND)


#SET(optionalLibs ${optionalLibs} PARENT_SCOPE)
