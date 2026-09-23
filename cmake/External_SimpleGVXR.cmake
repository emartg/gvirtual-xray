cmake_minimum_required(VERSION 2.8.2)

if(POLICY CMP0048)
    cmake_policy (SET CMP0048 NEW)
    PROJECT(SimpleGVXR-dow VERSION 1.0.0)
else ()
    PROJECT(SimpleGVXR-dow)
endif()


include(ExternalProject)

IF (NOT BUILD_TESTING)
    ExternalProject_Add(SimpleGVXR
        DOWNLOAD_COMMAND ""
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SimpleGVXR
        CMAKE_ARGS
            ${LOCAL_CMAKE_BUILD_OPTIONS}
            -DgVirtualXRay_DIR:PATH=${gVirtualXRay_DIR}
        PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
    )
ELSE ()
    ExternalProject_Add(SimpleGVXR
        DOWNLOAD_COMMAND ""
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SimpleGVXR
        TEST_BEFORE_INSTALL ON
        CMAKE_ARGS
            ${LOCAL_CMAKE_BUILD_OPTIONS}
            -DgVirtualXRay_DIR:PATH=${gVirtualXRay_DIR}
        PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
    )
ENDIF ()



include (SimpleGVXR/version.cmake)
SET (SimpleGVXR_DIR ${CMAKE_INSTALL_PREFIX}/SimpleGVXR-${SimpleGVXR_VERSION_STRING})
