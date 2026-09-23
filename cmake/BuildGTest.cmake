macro(build_googletest install_prefix staging_prefix temp_path)


# make a custom GTest configuration file
SET (URL "https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip")
SET (HASH "40d4ec942217dcc84a9ebe2a68584ada7d4a33a8ee958755763278ea1c5e18ff")

SET (GTest_VERSION_STRING 1.17)
SET (GTest_VERSION_MAJOR  1.17)
SET (GTest_VERSION_MINOR  0)

set(GTest_DISABLE_PTHREADS ON)
set(GTest_FORCE_SHARED_CRT ON)

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

SET(GTest_INCLUDE_DIR ${INSTALL_DIR}/include CACHE PATH "Path of GTest's header files." FORCE)
SET(GTest_DIR         ${INSTALL_DIR} CACHE PATH "Path of GTest's directory." FORCE)
SET(GTest_FOUND ON FORCE)

SET (GTest_LIBRARIES     ${INSTALL_DIR}/${INSTALL_LIBDIR}/libgtest.a CACHE FILEPATH "Path of GTest's library."
        FORCE )
SET (GTest_LIBRARY_DEBUG     ${INSTALL_DIR}/${INSTALL_LIBDIR}/libgtest.a CACHE FILEPATH "Path of GTest's debug library." FORCE )
SET (GTest_LIBRARY_RELEASE     ${INSTALL_DIR}/${INSTALL_LIBDIR}/libgtest.a CACHE FILEPATH "Path of GTest's release library." FORCE )

if (WIN32)
    IF (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
        string(FIND ${CMAKE_GENERATOR} "Visual Studio" GENERATED_WITH_MSVC)
        if (GENERATED_WITH_MSVC GREATER -1)
            SET (GTest_LIBRARIES
                    optimized ${INSTALL_DIR}/${INSTALL_LIBDIR}/gtest.lib
                    debug     ${INSTALL_DIR}/${INSTALL_LIBDIR}/gtest.lib
                    CACHE FILEPATH "Path of GTest's library." FORCE
            )
        else (GENERATED_WITH_MSVC GREATER -1)
            SET (GTest_LIBRARIES
                ${INSTALL_DIR}/${INSTALL_LIBDIR}/libgtest.a
                    CACHE FILEPATH "Path of GTest's library." FORCE
            )
        endif (GENERATED_WITH_MSVC GREATER -1)
    ELSE ()
        SET (GTest_LIBRARIES     ${INSTALL_DIR}/${INSTALL_LIBDIR}/libgtest.a CACHE FILEPATH "Path of GTest's library." FORCE)
        SET (GTest_LIBRARY_DEBUG     ${INSTALL_DIR}/${INSTALL_LIBDIR}/libgtest.a CACHE FILEPATH "Path of GTest's debug library." FORCE)
        SET (GTest_LIBRARY_RELEASE     ${INSTALL_DIR}/${INSTALL_LIBDIR}/libgtest.a CACHE FILEPATH "Path of GTest's release library." FORCE)
    ENDIF ()
endif (WIN32)

IF (UNIX AND NOT APPLE)
    ExternalProject_Add(googletest
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        URL  ${URL}
        URL_HASH SHA256=${HASH}
        UPDATE_COMMAND ""
        SOURCE_DIR  ${temp_path}/${install_prefix}-source
        BINARY_DIR  ${temp_path}/${install_prefix}-build
        INSTALL_DIR ${INSTALL_DIR}
        LIST_SEPARATOR :::
        CMAKE_GENERATOR ${CMAKE_GEN}
        BUILD_BYPRODUCTS ${GTest_LIBRARIES}
        CMAKE_ARGS
            -DCMAKE_BUILD_TYPE:STRING=Release
            -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
#            -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
            -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
#            -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
            -DCMAKE_EXE_LINKER_FLAGS:STRING=${LINKER_FLAGS}
            -DBUILD_SHARED_LIBS:BOOL=OFF
            -DCMAKE_TOOLCHAIN_FILE:FILEPATH=${CMAKE_TOOLCHAIN_FILE}
            -DCMAKE_INSTALL_PREFIX:PATH=${INSTALL_DIR}
            -DCMAKE_INSTALL_LIBDIR=${INSTALL_LIBDIR}
            -Dgtest_force_shared_crt=${GTest_FORCE_SHARED_CRT}
            -Dgtest_disable_pthreads=${GTest_DISABLE_PTHREADS}
            -DBUILD_GTEST:BOOL=ON
            -DBUILD_GMOCK:BOOL=OFF
            -DBUILD_UTILS:BOOL=OFF
    )
ELSE ()
    ExternalProject_Add(googletest
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        URL  ${URL}
        URL_HASH SHA256=${HASH}
        UPDATE_COMMAND ""
        SOURCE_DIR  ${temp_path}/${install_prefix}-source
        BINARY_DIR  ${temp_path}/${install_prefix}-build
        INSTALL_DIR ${INSTALL_DIR}
        LIST_SEPARATOR :::
        CMAKE_GENERATOR ${CMAKE_GEN}
        BUILD_BYPRODUCTS ${GTest_LIBRARIES}
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
            -Dgtest_force_shared_crt=${GTEST_FORCE_SHARED_CRT}
            -Dgtest_disable_pthreads=${GTEST_DISABLE_PTHREADS}
            -DBUILD_GTEST:BOOL=ON
            -DBUILD_GMOCK:BOOL=OFF
            -DBUILD_UTILS:BOOL=OFF
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

endmacro(build_googletest)
