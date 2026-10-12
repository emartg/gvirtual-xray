#
# CMakeLists.txt of the consumer test, a standalone project that uses the core library of gVirtualXRay through
# ADD_SUBDIRECTORY(), as a project does with this repository as a Git submodule. It verifies that the library
# is built alone (without SimpleGVXR, the wrappers, the tests, the installation rules, or the packaging), and
# that a target that links to gVirtualXRay::gVirtualXRay gets the headers, the preprocessor definitions, and the
# C++ standard of the library.
#
# Configure it with the options of the dependencies of the platform, for example on GNU/Linux with the system
# packages (the USE_SYSTEM_* options are described in gvxr/cmake/options.cmake):
#
#   cmake -S gvxr/tests/consumer -B build-consumer -DUSE_SYSTEM_GLEW=ON -DUSE_SYSTEM_ASSIMP=ON ...
#


CMAKE_MINIMUM_REQUIRED (VERSION 4.2 FATAL_ERROR)

PROJECT (gVirtualXRay_Consumer_Test LANGUAGES CXX)

# The consumer does not have to use the C++ standard of the library: the library requires its own
SET (CMAKE_CXX_STANDARD 20)
SET (CMAKE_CXX_STANDARD_REQUIRED ON)
SET (CMAKE_CXX_EXTENSIONS OFF)

# This project has tests, which must not turn on the tests of gVirtualXRay
INCLUDE (CTest)

# Build the core library alone, without a GLFW of its own: the application provides the OpenGL context
SET (BUILD_SIMPLEGVXR OFF)
SET (BUILD_WRAPPERS OFF)
SET (USE_GLFW OFF)
SET (USE_GLEW ON)

# Add gVirtualXRay from the root of this repository (a binary directory is required, since it is not a subdirectory)
ADD_SUBDIRECTORY (${CMAKE_CURRENT_SOURCE_DIR}/../../.. gVirtualXRay)

IF (NOT TARGET gVirtualXRay::gVirtualXRay)
    MESSAGE (FATAL_ERROR "The target gVirtualXRay::gVirtualXRay does not exist")
ENDIF ()

# The tests and SimpleGVXR must not be built when the library is used by another project
IF (TARGET SimpleGVXR OR TARGET TestMixture)
    MESSAGE (FATAL_ERROR "SimpleGVXR and the tests of gVirtualXRay must not be built for consumers")
ENDIF ()

# Create the consumer executable
ADD_EXECUTABLE (TestConsumer TestConsumer.cxx)
TARGET_LINK_LIBRARIES (TestConsumer PRIVATE gVirtualXRay::gVirtualXRay)

# xraylib is a shared library on Windows: copy its DLL beside the executable
IF (WIN32 AND XRAYLIB_SHARED_LIB_PATH)
    ADD_CUSTOM_COMMAND (TARGET TestConsumer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${XRAYLIB_SHARED_LIB_PATH} $<TARGET_FILE_DIR:TestConsumer>)
ENDIF ()

ADD_TEST (NAME TestConsumer COMMAND TestConsumer)
