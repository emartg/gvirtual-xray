cmake_minimum_required(VERSION 2.8.2)

if(POLICY CMP0048)
    cmake_policy (SET CMP0048 NEW)
    PROJECT(Wrappers-dow VERSION 1.0.0)
else ()
    PROJECT(Wrappers-dow)
endif()


include(ExternalProject)
ExternalProject_Add(Wrappers
    DOWNLOAD_COMMAND ""
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Wrappers
    CMAKE_ARGS
        ${LOCAL_CMAKE_BUILD_OPTIONS}
        ${ENABLED_WRAPPERS}
        -DgVirtualXRay_DIR:PATH=${gVirtualXRay_DIR}
        -DSimpleGVXR_DIR:PATH=${SimpleGVXR_DIR}
    PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
)
