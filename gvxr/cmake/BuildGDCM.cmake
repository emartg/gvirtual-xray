macro(build_gdcm install_prefix staging_prefix temp_path)


    # make a custom GDCM configuration file

SET (GDCM_VERSION_STRING 2.8)
SET (GDCM_VERSION_MAJOR  2.8)
SET (GDCM_VERSION_MINOR  2)

set(CMAKE_OSX_EXTERNAL_PROJECT_ARGS)
if(APPLE)
  list(APPEND CMAKE_OSX_EXTERNAL_PROJECT_ARGS
    -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
    -DCMAKE_OSX_SYSROOT=${CMAKE_OSX_SYSROOT}
    -DMACOSX_RPATH:BOOL=ON
    -DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}
  )
endif()

SET (GDCM_INSTALL_DIR ${staging_prefix}/${install_prefix}-install)

IF (UNIX AND NOT APPLE)
    ExternalProject_Add(gdcm
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        URL  "https://downloads.sourceforge.net/project/gdcm/gdcm%202.x/GDCM%202.8.2/gdcm-2.8.2.tar.gz"
        URL_MD5 "878d389466d37cb5808efe24e6d17f9b"
        UPDATE_COMMAND ""
        SOURCE_DIR  ${temp_path}/${install_prefix}-source
        BINARY_DIR  ${temp_path}/${install_prefix}-build
        INSTALL_DIR ${GDCM_INSTALL_DIR}
        LIST_SEPARATOR :::
        CMAKE_GENERATOR ${CMAKE_GEN}
        CMAKE_ARGS
            -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
            -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
            -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
            -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
            -DCMAKE_EXE_LINKER_FLAGS:STRING=${LINKER_FLAGS}
            -DBUILD_SHARED_LIBS:BOOL=OFF
            -DCMAKE_TOOLCHAIN_FILE:FILEPATH=${CMAKE_TOOLCHAIN_FILE}
            -DCMAKE_INSTALL_PREFIX:PATH=${GDCM_INSTALL_DIR}
            -DGDCM_BUILD_DOCBOOK_MANPAGES:BOOL=OFF
            -DCMAKE_INSTALL_LIBDIR=lib64
    )
ELSE ()
    ExternalProject_Add(gdcm
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        URL  "https://downloads.sourceforge.net/project/gdcm/gdcm%202.x/GDCM%202.8.2/gdcm-2.8.2.tar.gz"
        URL_MD5 "878d389466d37cb5808efe24e6d17f9b"
        UPDATE_COMMAND ""
        SOURCE_DIR  ${temp_path}/${install_prefix}-source
        BINARY_DIR  ${temp_path}/${install_prefix}-build
        INSTALL_DIR ${GDCM_INSTALL_DIR}
        LIST_SEPARATOR :::
        CMAKE_GENERATOR ${CMAKE_GEN}
        CMAKE_ARGS
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
            -DCMAKE_INSTALL_PREFIX:PATH=${GDCM_INSTALL_DIR}
            -DGDCM_BUILD_DOCBOOK_MANPAGES:BOOL=OFF
        )
ENDIF ()

SET(GDCM_INCLUDE_DIR  ${GDCM_INSTALL_DIR}/include/gdcm-${GDCM_VERSION_MAJOR})
SET(GDCM_INCLUDE_DIRS ${GDCM_INCLUDE_DIR})
SET(GDCM_LIBRARY_DIR  ${GDCM_INSTALL_DIR}/lib)

if (WIN32 AND ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    SET(GDCM_LIBRARIES
        ${GDCM_INSTALL_DIR}/lib/gdcmcharls.lib
        ${GDCM_INSTALL_DIR}/lib/gdcmCommon.lib
        ${GDCM_INSTALL_DIR}/lib/gdcmDICT.lib
        ${GDCM_INSTALL_DIR}/lib/gdcmDSED.lib
        ${GDCM_INSTALL_DIR}/lib/gdcmexpat.lib
        ${GDCM_INSTALL_DIR}/lib/gdcmgetopt.lib
        ${GDCM_INSTALL_DIR}/lib/gdcmIOD.lib
        ${GDCM_INSTALL_DIR}/lib/gdcmjpeg8.lib
        ${GDCM_INSTALL_DIR}/lib/gdcmjpeg12.lib
        ${GDCM_INSTALL_DIR}/lib/gdcmjpeg16.lib
        ${GDCM_INSTALL_DIR}/lib/gdcmMEXD.lib
        ${GDCM_INSTALL_DIR}/lib/gdcmMSFF.lib
        #${GDCM_INSTALL_DIR}/lib/gdcmopenjpeg.lib
        ${GDCM_INSTALL_DIR}/lib/gdcmzlib.lib
        ${GDCM_INSTALL_DIR}/lib/socketxx.lib
	#${GDCM_INSTALL_DIR}/bin/gdcmCommon.lib
	#${GDCM_INSTALL_DIR}/bin/gdcmDICT.lib
	#${GDCM_INSTALL_DIR}/bin/gdcmDSED.lib
	#${GDCM_INSTALL_DIR}/bin/gdcmIOD.lib
	#${GDCM_INSTALL_DIR}/bin/gdcmMEXD.lib
	#${GDCM_INSTALL_DIR}/bin/gdcmMSFF.lib
	Rpcrt4
	Ws2_32
    )
else ()
    SET(GDCM_LIBRARIES
        ${GDCM_INSTALL_DIR}/lib/libgdcmcharls.a
        ${GDCM_INSTALL_DIR}/lib/libgdcmCommon.a
        ${GDCM_INSTALL_DIR}/lib/libgdcmDICT.a
        ${GDCM_INSTALL_DIR}/lib/libgdcmDSED.a
        ${GDCM_INSTALL_DIR}/lib/libgdcmexpat.a
        ${GDCM_INSTALL_DIR}/lib/libgdcmIOD.a
        ${GDCM_INSTALL_DIR}/lib/libgdcmjpeg12.a
        ${GDCM_INSTALL_DIR}/lib/libgdcmjpeg16.a
        ${GDCM_INSTALL_DIR}/lib/libgdcmjpeg8.a
        ${GDCM_INSTALL_DIR}/lib/libgdcmMEXD.a
        ${GDCM_INSTALL_DIR}/lib/libgdcmMSFF.a
        ${GDCM_INSTALL_DIR}/lib/libgdcmopenjp2.a
        ${GDCM_INSTALL_DIR}/lib/libgdcmzlib.a
        ${GDCM_INSTALL_DIR}/lib/libsocketxx.a

        #${GDCM_INSTALL_DIR}/lib/libgdcmopenjpeg.a
    )

    IF (NOT WIN32)
        SET(GDCM_LIBRARIES
            ${GDCM_LIBRARIES}
            ${GDCM_INSTALL_DIR}/lib/libgdcmuuid.a
            dl
        )
    ELSE (NOT WIN32)
        SET(GDCM_LIBRARIES
            ${GDCM_LIBRARIES}
            ${GDCM_INSTALL_DIR}/lib/libgdcmcharls.a
            ${GDCM_INSTALL_DIR}/lib/libgdcmDICT.a
            ${GDCM_INSTALL_DIR}/lib/libgdcmexpat.a
            ${GDCM_INSTALL_DIR}/lib/libgdcmIOD.a
            ${GDCM_INSTALL_DIR}/lib/libgdcmjpeg16.a
            ${GDCM_INSTALL_DIR}/lib/libgdcmMEXD.a
            #${GDCM_INSTALL_DIR}/lib/libgdcmopenjpeg.a
            ${GDCM_INSTALL_DIR}/lib/libsocketxx.a
            ${GDCM_INSTALL_DIR}/lib/libgdcmCommon.a
            ${GDCM_INSTALL_DIR}/lib/libgdcmDSED.a
            ${GDCM_INSTALL_DIR}/lib/libgdcmgetopt.a
            ${GDCM_INSTALL_DIR}/lib/libgdcmjpeg12.a
            ${GDCM_INSTALL_DIR}/lib/libgdcmjpeg8.a
            ${GDCM_INSTALL_DIR}/lib/libgdcmMSFF.a
            ${GDCM_INSTALL_DIR}/lib/libgdcmzlib.a

	        rpcrt4
	        ws2_32
        )
    ENDIF (NOT WIN32)

endif ()

SET (GDCM_LIBRARY ${GDCM_LIBRARIES})

SET(GDCM_FOUND ON)


# copy libraries into install tree
install(DIRECTORY ${GDCM_INSTALL_DIR}/include/ DESTINATION ${INSTALL_DIR}/third_party/include COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${GDCM_INSTALL_DIR}/lib/     DESTINATION ${INSTALL_DIR}/third_party/lib     COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${GDCM_INSTALL_DIR}/lib64/   DESTINATION ${INSTALL_DIR}/third_party/lib64   COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${GDCM_INSTALL_DIR}/bin/     DESTINATION ${INSTALL_DIR}/third_party/bin     COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${GDCM_INSTALL_DIR}/share/   DESTINATION ${INSTALL_DIR}/third_party/share   COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${GDCM_INSTALL_DIR}/man/     DESTINATION ${INSTALL_DIR}/third_party/man     COMPONENT Development FILES_MATCHING PATTERN "*.*")

endmacro(build_gdcm)
