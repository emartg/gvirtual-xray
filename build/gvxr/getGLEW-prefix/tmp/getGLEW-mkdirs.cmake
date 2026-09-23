# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "C:/Dev/gVirtualXray/build/gvxr/glew-source")
  file(MAKE_DIRECTORY "C:/Dev/gVirtualXray/build/gvxr/glew-source")
endif()
file(MAKE_DIRECTORY
  "C:/Dev/gVirtualXray/build/gvxr/getGLEW-prefix/src/getGLEW-build"
  "C:/Dev/gVirtualXray/build/gvxr/getGLEW-prefix"
  "C:/Dev/gVirtualXray/build/gvxr/getGLEW-prefix/tmp"
  "C:/Dev/gVirtualXray/build/gvxr/getGLEW-prefix/src/getGLEW-stamp"
  "C:/Dev/gVirtualXray/build/gvxr/getGLEW-prefix/src"
  "C:/Dev/gVirtualXray/build/gvxr/getGLEW-prefix/src/getGLEW-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Dev/gVirtualXray/build/gvxr/getGLEW-prefix/src/getGLEW-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Dev/gVirtualXray/build/gvxr/getGLEW-prefix/src/getGLEW-stamp${cfgdir}") # cfgdir has leading slash
endif()
