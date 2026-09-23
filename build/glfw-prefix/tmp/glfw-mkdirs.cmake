# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "C:/Dev/gVirtualXray/build/glfw-source")
  file(MAKE_DIRECTORY "C:/Dev/gVirtualXray/build/glfw-source")
endif()
file(MAKE_DIRECTORY
  "C:/Dev/gVirtualXray/build/glfw-build"
  "C:/Dev/gVirtualXray/install/third_party"
  "C:/Dev/gVirtualXray/build/glfw-prefix/tmp"
  "C:/Dev/gVirtualXray/build/glfw-prefix/src/glfw-stamp"
  "C:/Dev/gVirtualXray/build/glfw-prefix/src"
  "C:/Dev/gVirtualXray/build/glfw-prefix/src/glfw-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Dev/gVirtualXray/build/glfw-prefix/src/glfw-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Dev/gVirtualXray/build/glfw-prefix/src/glfw-stamp${cfgdir}") # cfgdir has leading slash
endif()
