# This file sets up include directories, link directories, and
# compiler settings for a project to use gVirtualXRay.  It should not be
# included directly, but rather through the gVirtualXRay_USE_FILE setting
# obtained from gVirtualXRayConfig.cmake.


# Use shared library
SET(BUILD_SHARED_LIBS "${gVirtualXRay_BUILD_SHARED_LIBS}" PARENT_SCOPE)


# C++ version
set (CMAKE_CXX_STANDARD "${gVirtualXRay_CMAKE_CXX_STANDARD}" PARENT_SCOPE)


# C++ flags
SET(CMAKE_C_FLAGS   "${gVirtualXRay_CMAKE_C_FLAGS}" PARENT_SCOPE)
SET(CMAKE_CXX_FLAGS "${gVirtualXRay_CMAKE_CXX_FLAGS}" PARENT_SCOPE)


# Add preprocessor definitions
ADD_DEFINITIONS(${gVirtualXRay_ADD_DEFINITIONS})


# Add include directories needed to use gVirtualXRay
include_directories(BEFORE ${gVirtualXRay_INCLUDE_DIR})
include_directories(BEFORE ${gVirtualXRay_INCLUDE_DIRS})


# Add link directories needed to use gVirtualXRay
link_directories(${gVirtualXRay_LIBRARY_DIRS})

# Set gzip, using gVirtualXRay's configuration
SET(GZIP_TOOL ${gVirtualXRay_GZIP_TOOL} PARENT_SCOPE)
