macro(build_glut install_prefix staging_prefix temp_path)


	# make a custom FreeGlut configuration file

SET (GLUT_VERSION_STRING 3.0)
SET (GLUT_VERSION_MAJOR  3.0)
SET (GLUT_VERSION_MINOR  0)

set(CMAKE_OSX_EXTERNAL_PROJECT_ARGS)
if(APPLE)
  list(APPEND CMAKE_OSX_EXTERNAL_PROJECT_ARGS
    -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
    -DCMAKE_OSX_SYSROOT=${CMAKE_OSX_SYSROOT}
    -DMACOSX_RPATH:BOOL=ON
    -DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}
  )
endif()

SET (GLUT_INSTALL_DIR ${staging_prefix}/${install_prefix}-install)

IF (UNIX AND NOT APPLE)
    ExternalProject_Add(glut
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        URL  "https://sourceforge.net/projects/freeglut/files/freeglut/3.0.0/freeglut-3.0.0.tar.gz"
        URL_MD5 "90c3ca4dd9d51cf32276bc5344ec9754"
        UPDATE_COMMAND ""
        SOURCE_DIR  ${temp_path}/${install_prefix}-source
        BINARY_DIR  ${temp_path}/${install_prefix}-build
        INSTALL_DIR ${GLUT_INSTALL_DIR}
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
            -DFREEGLUT_BUILD_DEMOS:BOOL=OFF
            -DFREEGLUT_BUILD_STATIC_LIBS:BOOL=ON
            -DFREEGLUT_BUILD_SHARED_LIBS:BOOL=OFF
            -DGLUT_BUILD_DEMOS:BOOL=OFF
            -DGLUT_BUILD_STATIC_LIBS:BOOL=ON
            -DGLUT_BUILD_SHARED_LIBS:BOOL=OFF
            -DCMAKE_INSTALL_PREFIX:PATH=${GLUT_INSTALL_DIR}
            -DINSTALL_PDB:BOOL=OFF
            -DCMAKE_INSTALL_LIBDIR=lib64
    )
ELSE ()
    ExternalProject_Add(glut
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        URL  "https://sourceforge.net/projects/freeglut/files/freeglut/3.0.0/freeglut-3.0.0.tar.gz"
        URL_MD5 "90c3ca4dd9d51cf32276bc5344ec9754"
        UPDATE_COMMAND ""
        SOURCE_DIR  ${temp_path}/${install_prefix}-source
        BINARY_DIR  ${temp_path}/${install_prefix}-build
        INSTALL_DIR ${GLUT_INSTALL_DIR}
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
            -DFREEGLUT_BUILD_DEMOS:BOOL=OFF
            -DFREEGLUT_BUILD_STATIC_LIBS:BOOL=ON
            -DFREEGLUT_BUILD_SHARED_LIBS:BOOL=OFF
            -DGLUT_BUILD_DEMOS:BOOL=OFF
            -DGLUT_BUILD_STATIC_LIBS:BOOL=ON
            -DGLUT_BUILD_SHARED_LIBS:BOOL=OFF
            -DCMAKE_INSTALL_PREFIX:PATH=${GLUT_INSTALL_DIR}
            -DINSTALL_PDB:BOOL=OFF
    )
ENDIF ()

SET(GLUT_INCLUDE_DIR  ${GLUT_INSTALL_DIR}/include)
SET(GLUT_INCLUDE_DIRS ${GLUT_INSTALL_DIR}/include)
SET(GLUT_LIBRARY_DIR  ${GLUT_INSTALL_DIR}/lib)

if (WIN32)
    if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
	SET(GLUT_LIBRARIES
            optimized ${GLUT_INSTALL_DIR}/lib/freeglut_static.lib
            debug     ${GLUT_INSTALL_DIR}/lib/freeglut_staticd.lib
        )
    ELSE ()
	SET(GLUT_LIBRARIES
           ${GLUT_INSTALL_DIR}/lib/libfreeglut_static.a
        )
    ENDIF ()
endif ()

IF (UNIX)

    FIND_PACKAGE (X11)

    IF (APPLE)

        FIND_LIBRARY(GL_LIBRARY GL)
        FIND_LIBRARY(AGL_LIBRARY AGL)
        FIND_LIBRARY(CARBON_LIBRARY Carbon)
        FIND_LIBRARY(COCOA_LIBRARY Cocoa)

        SET (GLUT_PLATFORM_DEPENDENT_LIBS
            ${GL_LIBRARY}
            ${AGL_LIBRARY}
            ${CARBON_LIBRARY}
            ${COCOA_LIBRARY}
            #pthread
            dl
            #SM
            #ICE
            ${X11_LIBRARIES}
            ${X11_X11_LIB}
	    ${X11_Xext_LIB}
            ${X11_Xinerama_LIB}
            ${X11_Xfixes_LIB}
            ${X11_Xcursor_LIB}
            ${X11_Xft_LIB}
            fontconfig
            m
            ${X11_Xxf86vm_LIB}
            ${X11_Xi_LIB}
            #Xrender
            ${X11_Xrandr_LIB}
        )

        SET (GLUT_LIBRARIES
            ${GLUT_INSTALL_DIR}/lib/libglut.a
            ${GLUT_PLATFORM_DEPENDENT_LIBS}
        )

    ELSE (APPLE)

        SET (GLUT_PLATFORM_DEPENDENT_LIBS
            #pthread
            dl
            #SM
            #ICE
            ${X11_LIBRARIES}
            ${X11_X11_LIB}
            ${X11_Xext_LIB}
            ${X11_Xinerama_LIB}
            ${X11_Xfixes_LIB}
            ${X11_Xcursor_LIB}
            ${X11_Xft_LIB}
            fontconfig
            #m
            ${X11_Xxf86vm_LIB}
            ${X11_Xi_LIB}
            #Xrender
            ${X11_Xrandr_LIB}
        )

        SET (GLUT_LIBRARIES
            ${GLUT_INSTALL_DIR}/lib64/libglut.a
            ${GLUT_PLATFORM_DEPENDENT_LIBS}
        )

    ENDIF (APPLE)
ENDIF (UNIX)

set (GLUT_LIBRARY      ${GLUT_LIBRARIES})
set (GLUT_INCLUDE_PATH ${GLUT_INCLUDE_DIR})

SET(GLUT_FOUND ON)


# copy libraries into install tree
install(DIRECTORY ${GLUT_INSTALL_DIR}/include/ DESTINATION ${INSTALL_DIR}/third_party/include COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${GLUT_INSTALL_DIR}/lib/     DESTINATION ${INSTALL_DIR}/third_party/lib     COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${GLUT_INSTALL_DIR}/lib64/   DESTINATION ${INSTALL_DIR}/third_party/lib64   COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${GLUT_INSTALL_DIR}/bin/     DESTINATION ${INSTALL_DIR}/third_party/bin     COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${GLUT_INSTALL_DIR}/share/   DESTINATION ${INSTALL_DIR}/third_party/share   COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${GLUT_INSTALL_DIR}/man/     DESTINATION ${INSTALL_DIR}/third_party/man     COMPONENT Development FILES_MATCHING PATTERN "*.*")

endmacro(build_glut)
