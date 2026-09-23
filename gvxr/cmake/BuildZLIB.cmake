macro(build_zlib install_prefix staging_prefix temp_path)


# make a custom ZLIB configuration file
SET (URL "http://zlib.net/zlib-1.3.2.tar.gz")
SET (HASH "bb329a0a2cd0274d05519d61c667c062e06990d72e125ee2dfa8de64f0119d16")

SET (ZLIB_VERSION_STRING 1.3)
SET (ZLIB_VERSION_MAJOR  1.3)
SET (ZLIB_VERSION_MINOR  2)

set(CMAKE_OSX_EXTERNAL_PROJECT_ARGS)
if(APPLE)
  list(APPEND CMAKE_OSX_EXTERNAL_PROJECT_ARGS
    -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
    -DCMAKE_OSX_SYSROOT=${CMAKE_OSX_SYSROOT}
    -DMACOSX_RPATH:BOOL=ON
    -DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}
  )
endif()

SET (INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/third_party)
SET (INSTALL_LIBDIR lib)

SET(ZLIB_INCLUDE_DIR ${INSTALL_DIR}/include CACHE PATH "Path of zlib's header files." FORCE)
SET(ZLIB_DIR         ${INSTALL_DIR} CACHE PATH "Path of zlib's directory." FORCE)
SET(ZLIB_FOUND ON FORCE)

SET (ZLIB_LIBRARY     ${INSTALL_DIR}/${INSTALL_LIBDIR}/libz.a CACHE FILEPATH "Path of zlib's library." FORCE )
SET (ZLIB_LIBRARY_DEBUG     ${INSTALL_DIR}/${INSTALL_LIBDIR}/libz.a CACHE FILEPATH "Path of zlib's debug library." FORCE )
SET (ZLIB_LIBRARY_RELEASE     ${INSTALL_DIR}/${INSTALL_LIBDIR}/libz.a CACHE FILEPATH "Path of zlib's release library." FORCE )

if (WIN32)
    IF (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
        string(FIND ${CMAKE_GENERATOR} "Visual Studio" GENERATED_WITH_MSVC)
        if (GENERATED_WITH_MSVC GREATER -1)
            SET (ZLIB_LIBRARY
                    optimized ${INSTALL_DIR}/${INSTALL_LIBDIR}/zlibstatic.lib
                    debug     ${INSTALL_DIR}/${INSTALL_LIBDIR}/zlibstaticd.lib
                    CACHE FILEPATH "Path of zlib's library." FORCE
            )
        else (GENERATED_WITH_MSVC GREATER -1)
            SET (ZLIB_LIBRARY
                    ${INSTALL_DIR}/${INSTALL_LIBDIR}/zlibstatic.lib
                    CACHE FILEPATH "Path of zlib's library." FORCE
            )
        endif (GENERATED_WITH_MSVC GREATER -1)
    ELSE ()
        SET (ZLIB_LIBRARY     ${INSTALL_DIR}/${INSTALL_LIBDIR}/libzlibstatic.a CACHE FILEPATH "Path of zlib's library." FORCE)
        SET (ZLIB_LIBRARY_DEBUG     ${INSTALL_DIR}/${INSTALL_LIBDIR}/libzlibstatic.a CACHE FILEPATH "Path of zlib's debug library." FORCE)
        SET (ZLIB_LIBRARY_RELEASE     ${INSTALL_DIR}/${INSTALL_LIBDIR}/libzlibstatic.a CACHE FILEPATH "Path of zlib's release library." FORCE)
    ENDIF ()
endif (WIN32)


IF (UNIX AND NOT APPLE)
    ExternalProject_Add(zlib
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        URL  ${URL}
        URL_HASH SHA256=${HASH}
        UPDATE_COMMAND ""
        SOURCE_DIR  ${temp_path}/${install_prefix}-source
        BINARY_DIR  ${temp_path}/${install_prefix}-build
        INSTALL_DIR ${INSTALL_DIR}
        LIST_SEPARATOR :::
        CMAKE_GENERATOR ${CMAKE_GEN}
        BUILD_BYPRODUCTS ${ZLIB_LIBRARY}
        CMAKE_ARGS
            -DCMAKE_BUILD_TYPE:STRING=Release
            -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
            -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
            -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
            -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
            -DCMAKE_EXE_LINKER_FLAGS:STRING=${LINKER_FLAGS}
            -DBUILD_SHARED_LIBS:BOOL=OFF
            -DCMAKE_TOOLCHAIN_FILE:FILEPATH=${CMAKE_TOOLCHAIN_FILE}
            -DCMAKE_INSTALL_PREFIX:PATH=${INSTALL_DIR}
            -DCMAKE_INSTALL_LIBDIR=${INSTALL_LIBDIR}
    )
ELSE ()
    ExternalProject_Add(zlib
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        URL  ${URL}
        URL_HASH SHA256=${HASH}
        UPDATE_COMMAND ""
        SOURCE_DIR  ${temp_path}/${install_prefix}-source
        BINARY_DIR  ${temp_path}/${install_prefix}-build
        INSTALL_DIR ${INSTALL_DIR}
        LIST_SEPARATOR :::
        CMAKE_GENERATOR ${CMAKE_GEN}
        BUILD_BYPRODUCTS ${ZLIB_LIBRARY}
        CMAKE_ARGS
            -DCMAKE_BUILD_TYPE:STRING=Release
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
            -DCMAKE_INSTALL_PREFIX:PATH=${INSTALL_DIR}
            -DCMAKE_INSTALL_LIBDIR=${INSTALL_LIBDIR}
    )
ENDIF ()



# install(DIRECTORY ${INSTALL_DIR}/include/ DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/include COMPONENT Development FILES_MATCHING PATTERN "*.*")
# install(DIRECTORY ${INSTALL_DIR}/lib/     DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/lib     COMPONENT Development FILES_MATCHING PATTERN "*.*")
# install(DIRECTORY ${INSTALL_DIR}/lib64/   DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/lib64   COMPONENT Development FILES_MATCHING PATTERN "*.*")
# install(DIRECTORY ${INSTALL_DIR}/bin/     DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/bin     COMPONENT Development FILES_MATCHING PATTERN "*.*")
# install(DIRECTORY ${INSTALL_DIR}/share/   DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/share   COMPONENT Development FILES_MATCHING PATTERN "*.*")
# install(DIRECTORY ${INSTALL_DIR}/man/     DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/man     COMPONENT Development FILES_MATCHING PATTERN "*.*")

# install(DIRECTORY ${INSTALL_DIR}/lib/     DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/lib     COMPONENT Runtime FILES_MATCHING PATTERN "*.so*")
# install(DIRECTORY ${INSTALL_DIR}/lib64/   DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/lib     COMPONENT Runtime FILES_MATCHING PATTERN "*.so*")

endmacro(build_zlib)
