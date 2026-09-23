macro(build_freetype install_prefix staging_prefix temp_path)

SET (FREETYPE_VERSION_STRING 2.6)
SET (FREETYPE_VERSION_MAJOR  2.6)
SET (FREETYPE_VERSION_MINOR  5)

set(CMAKE_OSX_EXTERNAL_PROJECT_ARGS)
if(APPLE)
  list(APPEND CMAKE_OSX_EXTERNAL_PROJECT_ARGS
    -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
    -DCMAKE_OSX_SYSROOT=${CMAKE_OSX_SYSROOT}
    -DMACOSX_RPATH:BOOL=ON
    -DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}
  )
endif()

SET (FREETYPE_INSTALL_DIR ${staging_prefix}/${install_prefix}-install)
SET (FREETYPE_BINARY_DIR  ${temp_path}/${install_prefix}-build)

IF (UNIX AND NOT APPLE)
    ExternalProject_Add(freetype
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        URL  "https://sourceforge.net/projects/freetype/files/freetype2/2.14.1/freetype-2.14.1.tar.gz/download?use_mirror=vorboss&r=&use_mirror=vorboss"
        URL_MD5 "d8c8b0c3a6d4c7ed4c90e1a8c7171887"
        UPDATE_COMMAND ""
        SOURCE_DIR  ${temp_path}/${install_prefix}-source
        BINARY_DIR  ${temp_path}/${install_prefix}-build
        INSTALL_DIR ${FREETYPE_INSTALL_DIR}
        LIST_SEPARATOR :::
        CMAKE_GENERATOR ${CMAKE_GEN}
        CMAKE_ARGS
            -DDOWNLOAD_EXTRACT_TIMESTAMP:BOOL=ON
            -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
            -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
            -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
            -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
            -DCMAKE_EXE_LINKER_FLAGS:STRING=${LINKER_FLAGS}
            -DBUILD_SHARED_LIBS:BOOL=OFF
            -DCMAKE_TOOLCHAIN_FILE:FILEPATH=${CMAKE_TOOLCHAIN_FILE}
            -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
            -DCMAKE_OSX_SYSROOT=${CMAKE_OSX_SYSROOT}
            -DMACOSX_RPATH:BOOL=ON
            -DWITH_HarfBuzz:BOOL=OFF
            -DCMAKE_INSTALL_PREFIX:PATH=${FREETYPE_INSTALL_DIR}
            -DCMAKE_INSTALL_LIBDIR=lib64
    )
ELSE ()
    ExternalProject_Add(freetype
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        URL  "https://sourceforge.net/projects/freetype/files/freetype2/2.14.1/freetype-2.14.1.tar.gz/download?use_mirror=vorboss&r=&use_mirror=vorboss"
        #URL_MD5 "f2b84651cb70c0a7d64a7f427b6675cd"
        UPDATE_COMMAND ""
        SOURCE_DIR  ${temp_path}/${install_prefix}-source
        BINARY_DIR  ${temp_path}/${install_prefix}-build
        INSTALL_DIR ${FREETYPE_INSTALL_DIR}
        LIST_SEPARATOR :::
        CMAKE_GENERATOR ${CMAKE_GEN}
        CMAKE_ARGS
            -DDOWNLOAD_EXTRACT_TIMESTAMP:BOOL=ON
            -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
            -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
            -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
            -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
            -DCMAKE_EXE_LINKER_FLAGS:STRING=${LINKER_FLAGS}
            -DBUILD_SHARED_LIBS:BOOL=OFF
            -DCMAKE_TOOLCHAIN_FILE:FILEPATH=${CMAKE_TOOLCHAIN_FILE}
            -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
            -DCMAKE_OSX_SYSROOT=${CMAKE_OSX_SYSROOT}
            -DMACOSX_RPATH:BOOL=ON
            -DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}
            -DWITH_HarfBuzz:BOOL=OFF
            -DCMAKE_INSTALL_PREFIX:PATH=${FREETYPE_INSTALL_DIR}
    )
ENDIF ()

SET(FREETYPE_INCLUDE_DIRS ${FREETYPE_INSTALL_DIR}/include  ${FREETYPE_INSTALL_DIR}/include/freetype2)
SET(FREETYPE_DIR         ${FREETYPE_INSTALL_DIR})
SET(FREETYPE_FOUND ON)
SET (FREETYPE_built_type "${CMAKE_CFG_INTDIR} ${Configuration} ${CMAKE_CONFIGURATION_TYPES}")

SET(FREETYPE_LIBRARIES    ${FREETYPE_INSTALL_DIR}/lib64/libfreetype.a )

if (WIN32 AND ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    string(FIND ${CMAKE_GENERATOR} "Visual Studio" GENERATED_WITH_MSVC)
    if (GENERATED_WITH_MSVC GREATER -1)
        SET(FREETYPE_LIBRARIES
            optimized ${FREETYPE_INSTALL_DIR}/lib/freetype.lib
            debug     ${FREETYPE_INSTALL_DIR}/lib/freetyped.lib
        )
    else ()
        SET(FREETYPE_LIBRARIES ${FREETYPE_INSTALL_DIR}/lib/libfreetype.a)
    endif ()
endif()


# copy libraries into install tree
install(DIRECTORY ${FREETYPE_INSTALL_DIR}/include/ DESTINATION ${FREETYPE_INSTALL_DIR}/third_party/include COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${FREETYPE_INSTALL_DIR}/lib/     DESTINATION ${FREETYPE_INSTALL_DIR}/third_party/lib     COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${FREETYPE_INSTALL_DIR}/lib64/   DESTINATION ${FREETYPE_INSTALL_DIR}/third_party/lib64   COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${FREETYPE_INSTALL_DIR}/bin/     DESTINATION ${FREETYPE_INSTALL_DIR}/third_party/bin     COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${FREETYPE_INSTALL_DIR}/share/   DESTINATION ${FREETYPE_INSTALL_DIR}/third_party/share   COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${FREETYPE_INSTALL_DIR}/man/     DESTINATION ${FREETYPE_INSTALL_DIR}/third_party/man     COMPONENT Development FILES_MATCHING PATTERN "*.*")


endmacro(build_freetype)
