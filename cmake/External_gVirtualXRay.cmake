cmake_minimum_required(VERSION 2.8.2)

if(POLICY CMP0048)
    cmake_policy (SET CMP0048 NEW)
    PROJECT(gVirtualXRay-dow VERSION 1.0.0)
else ()
    PROJECT(gVirtualXRay-dow)
endif()

get_filename_component(GVXR_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}" REALPATH)

include(ExternalProject)
include(${CMAKE_CURRENT_SOURCE_DIR}/gvxr/cmake/options.cmake)

IF (NOT BUILD_TESTING)
    ExternalProject_Add(gvxr
        DOWNLOAD_COMMAND ""
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/gvxr
        INSTALL_DIR ${GVXR_INSTALL_DIR}
        CMAKE_ARGS
            ${LOCAL_CMAKE_BUILD_OPTIONS}
        PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
    )
ELSE ()
    ExternalProject_Add(gvxr
        DOWNLOAD_COMMAND ""
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/gvxr
        INSTALL_DIR ${GVXR_INSTALL_DIR}
        TEST_BEFORE_INSTALL ON
        CMAKE_ARGS
            ${LOCAL_CMAKE_BUILD_OPTIONS}
            -DCMAKE_INSTALL_PREFIX:PATH=${GVXR_INSTALL_DIR}
        PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
    )
ENDIF ()


include (gvxr/cmake/version.cmake)
SET (gVirtualXRay_DIR ${CMAKE_INSTALL_PREFIX}/gVirtualXRay-${gVirtualXRay_VERSION_STRING})
