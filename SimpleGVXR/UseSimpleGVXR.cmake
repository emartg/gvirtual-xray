# This file sets up include directories, link directories, and
# compiler settings for a project to use gVirtualXRay.  It should not be
# included directly, but rather through the gVirtualXRay_USE_FILE setting
# obtained from gVirtualXRayConfig.cmake.


# Find gVirtualXRay ############################################################
FIND_PACKAGE(gVirtualXRay REQUIRED)
include(${gVirtualXRay_USE_FILE})


# Add include directories needed to use gVirtualXRay
include_directories(BEFORE ${SimpleGVXR_INCLUDE_DIR})
include_directories(BEFORE ${SimpleGVXR_INCLUDE_DIR})


# Add link directories needed to use gVirtualXRay
link_directories(${SimpleGVXR_LIBRARY_DIRS})
