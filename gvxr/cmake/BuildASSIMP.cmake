macro(build_assimp install_prefix staging_prefix temp_path)


# make a custom ASSIMP configuration file
SET (URL "https://github.com/assimp/assimp/archive/refs/tags/v5.4.0.zip")
SET (MD5 "321a963e1405ea38d98fdc18a15874ac")

SET (assimp_VERSION_STRING 5.4)
SET (assimp_VERSION_MAJOR  4)
SET (assimp_VERSION_MINOR  0)

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

SET(assimp_ROOT_DIR      ${INSTALL_DIR}          CACHE PATH "Where ASSIMP is."   FORCE)
SET(assimp_INCLUDE_DIRS  ${INSTALL_DIR}/include/ CACHE PATH "Where ASSIMP's headers are."   FORCE)
SET(assimp_LIBRARY_DIRS  ${INSTALL_DIR}/${INSTALL_LIBDIR}      CACHE PATH "Where ASSIMP's libraries are." FORCE)


IF (MSVC12)
    SET(assimp_MSVC_VERSION "vc120")
ELSEIF (MSVC14)
    SET(assimp_MSVC_VERSION "vc143")
ENDIF (MSVC12)

IF (MSVC_VERSION EQUAL 1950)
    SET(assimp_MSVC_VERSION "vc145")
ENDIF ()


if (WIN32 AND ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    FILE (WRITE ${temp_path}/${install_prefix}-build/code/assimp-${assimp_MSVC_VERSION}-mtd.pdb)
    FILE (WRITE ${temp_path}/${install_prefix}-build/code/assimp-${assimp_MSVC_VERSION}-mt.pdb)

    string(FIND ${CMAKE_GENERATOR} "Visual Studio" GENERATED_WITH_MSVC)
    if (GENERATED_WITH_MSVC GREATER -1)
        SET(assimp_LIBRARIES
                optimized
                ${INSTALL_DIR}/${INSTALL_LIBDIR}/assimp-${assimp_MSVC_VERSION}-mt.lib
                debug
                ${INSTALL_DIR}/${INSTALL_LIBDIR}/assimp-${assimp_MSVC_VERSION}-mtd.lib
                CACHE FILEPATH "ASSIMP's libraries." FORCE
        )
    else (GENERATED_WITH_MSVC GREATER -1)
        SET(assimp_LIBRARIES
            ${INSTALL_DIR}/${INSTALL_LIBDIR}/libassimp.a
                CACHE FILEPATH "ASSIMP's libraries." FORCE
        )
    endif (GENERATED_WITH_MSVC GREATER -1)

    #install(FILES ${staging_prefix}/${install_prefix}-build/code/Debug/assimp-${assimp_MSVC_VERSION}-mt.lib
    #        CONFIGURATIONS Debug
    #        RUNTIME DESTINATION ${INSTALL_DIR}/third_party/${INSTALL_LIBDIR}/Debug)

    #install(FILES ${staging_prefix}/${install_prefix}-build/code/Release/assimp-${assimp_MSVC_VERSION}-mt.lib
    #        CONFIGURATIONS Release
    #        RUNTIME DESTINATION ${INSTALL_DIR}/third_party/${INSTALL_LIBDIR}/Release)
else ()

    SET(assimp_LIBRARIES
            ${INSTALL_DIR}/${INSTALL_LIBDIR}/libassimp.a
            CACHE FILEPATH "ASSIMP's libraries." FORCE
    )

endif ()

SET (assimp_LIBRARY ${assimp_LIBRARIES} CACHE FILEPATH "ASSIMP's libraries." FORCE)

IF (UNIX AND NOT APPLE)
    ExternalProject_Add(assimp
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        URL     ${URL}
        URL_MD5 ${MD5}
        UPDATE_COMMAND ""
        SOURCE_DIR  ${temp_path}/${install_prefix}-source
        BINARY_DIR  ${temp_path}/${install_prefix}-build
        INSTALL_DIR ${INSTALL_DIR}
        LIST_SEPARATOR :::
        CMAKE_GENERATOR ${CMAKE_GEN}
        BUILD_BYPRODUCTS ${assimp_LIBRARIES}
        CMAKE_ARGS
            ${LOCAL_CMAKE_BUILD_OPTIONS}
            -DASSIMP_WARNINGS_AS_ERRORS:BOOL=OFF
            -DCMAKE_BUILD_TYPE:STRING=Release
            -DCMAKE_INSTALL_PREFIX:PATH=${INSTALL_DIR}
            -DASSIMP_BUILD_TESTS:BOOL=OFF
            -DBUILD_SHARED_LIBS:BOOL=OFF
            -DBUILD_TESTING:BOOL=OFF
            -DASSIMP_BUILD_ASSIMP_TOOLS:BOOL=OFF
            -DASSIMP_BUILD_DOCBOOK_MANPAGES:BOOL=OFF
            -DCMAKE_INSTALL_LIBDIR=${INSTALL_LIBDIR}
            -DZLIB_INCLUDE_DIR:STRING=${ZLIB_INCLUDE_DIR}
            -DZLIB_DIR:STRING=${ZLIB_DIR}
            -DZLIB_LIBRARY:STRING=${ZLIB_LIBRARY}
            -DZLIB_LIBRARY_DEBUG:STRING=${ZLIB_LIBRARY_DEBUG}
            -DZLIB_LIBRARY_RELEASE:STRING=${ZLIB_LIBRARY_RELEASE}
            -DZLIB_LIBRARY_DBG:STRING=${ZLIB_LIBRARY_DEBUG}
            -DZLIB_LIBRARY_REL:STRING=${ZLIB_LIBRARY_RELEASE}
        #INSTALL_COMMAND ""
    )
ELSE (UNIX AND NOT APPLE)
    ExternalProject_Add(assimp
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        URL     ${URL}
        URL_MD5 ${MD5}
        UPDATE_COMMAND ""
        SOURCE_DIR  ${temp_path}/${install_prefix}-source
        BINARY_DIR  ${temp_path}/${install_prefix}-build
        INSTALL_DIR ${INSTALL_DIR}
        LIST_SEPARATOR :::
        CMAKE_GENERATOR ${CMAKE_GEN}
        BUILD_BYPRODUCTS ${assimp_LIBRARIES}
        CMAKE_ARGS
            ${LOCAL_CMAKE_BUILD_OPTIONS}
            -DASSIMP_WARNINGS_AS_ERRORS:BOOL=OFF
            -DCMAKE_BUILD_TYPE:STRING=Release
            -DCMAKE_INSTALL_PREFIX:PATH=${INSTALL_DIR}
            -DASSIMP_BUILD_TESTS:BOOL=OFF
            -DBUILD_SHARED_LIBS:BOOL=OFF
            -DBUILD_TESTING:BOOL=OFF
            -DASSIMP_BUILD_ASSIMP_TOOLS:BOOL=OFF
            -DASSIMP_BUILD_DOCBOOK_MANPAGES:BOOL=OFF
            -DCMAKE_INSTALL_LIBDIR=${INSTALL_LIBDIR}
            -DZLIB_INCLUDE_DIR:STRING=${ZLIB_INCLUDE_DIR}
            -DZLIB_DIR:STRING=${ZLIB_DIR}
            -DZLIB_LIBRARY:STRING=${ZLIB_LIBRARY}
            -DZLIB_LIBRARY_DEBUG:STRING=${ZLIB_LIBRARY_DEBUG}
            -DZLIB_LIBRARY_RELEASE:STRING=${ZLIB_LIBRARY_RELEASE}
            -DZLIB_LIBRARY_DBG:STRING=${ZLIB_LIBRARY_DEBUG}
            -DZLIB_LIBRARY_REL:STRING=${ZLIB_LIBRARY_RELEASE}
        #INSTALL_COMMAND ""
    )
ENDIF (UNIX AND NOT APPLE)


SET(assimp_FOUND ON)


# copy libraries into install tree
# install(DIRECTORY ${INSTALL_DIR}/include/ DESTINATION ${INSTALL_DIR}/third_party/include COMPONENT Development FILES_MATCHING PATTERN "*.*")
# install(DIRECTORY ${INSTALL_DIR}/${INSTALL_LIBDIR}     DESTINATION ${INSTALL_DIR}/third_party/${INSTALL_LIBDIR}     COMPONENT Development FILES_MATCHING PATTERN "*.*")
# install(DIRECTORY ${INSTALL_DIR}/bin/     DESTINATION ${INSTALL_DIR}/third_party/bin     COMPONENT Development FILES_MATCHING PATTERN "*.*")

# install(DIRECTORY ${INSTALL_DIR}/${INSTALL_LIBDIR}     DESTINATION ${INSTALL_DIR}/third_party/${INSTALL_LIBDIR}     COMPONENT Runtime FILES_MATCHING PATTERN "*.so*")
# install(DIRECTORY ${INSTALL_DIR}/${INSTALL_LIBDIR}/   DESTINATION ${INSTALL_DIR}/third_party/${INSTALL_LIBDIR}     COMPONENT Runtime FILES_MATCHING PATTERN "*.so*")

endmacro(build_assimp)
