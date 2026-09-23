# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "C:/Dev/gVirtualXray/build/gvxr/LibTIFF-source")
  file(MAKE_DIRECTORY "C:/Dev/gVirtualXray/build/gvxr/LibTIFF-source")
endif()
file(MAKE_DIRECTORY
  "C:/Dev/gVirtualXray/build/gvxr/LibTIFF-build"
  "C:/Program Files (x86)/gVirtualXRay-Superbuild/third_party"
  "C:/Dev/gVirtualXray/build/gvxr/libtiff-prefix/tmp"
  "C:/Dev/gVirtualXray/build/gvxr/libtiff-prefix/src/libtiff-stamp"
  "C:/Dev/gVirtualXray/build/gvxr/libtiff-prefix/src"
  "C:/Dev/gVirtualXray/build/gvxr/libtiff-prefix/src/libtiff-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Dev/gVirtualXray/build/gvxr/libtiff-prefix/src/libtiff-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Dev/gVirtualXray/build/gvxr/libtiff-prefix/src/libtiff-stamp${cfgdir}") # cfgdir has leading slash
endif()
