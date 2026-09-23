macro(build_fltk install_prefix staging_prefix temp_path)


# make a custom fltk configuration file

SET (fltk_VERSION_STRING 1.2 FORCE)
SET (fltk_VERSION_MAJOR  1.2 FORCE)
SET (fltk_VERSION_MINOR  8 FORCE)

set(CMAKE_OSX_EXTERNAL_PROJECT_ARGS)
if(APPLE)
  list(APPEND CMAKE_OSX_EXTERNAL_PROJECT_ARGS
    -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
    -DCMAKE_OSX_SYSROOT=${CMAKE_OSX_SYSROOT}
    -DMACOSX_RPATH:BOOL=ON
    -DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}
  )
endif()

SET (FLTK_INSTALL_DIR ${staging_prefix}/${install_prefix}-install FORCE)

SET (LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} FORCE)

IF (UNIX)
    IF (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
        SET (LINKER_FLAGS ${LINKER_FLAGS} -ldl)
    ENDIF()
ENDIF()

IF (UNIX AND NOT APPLE)
    ExternalProject_Add(fltk
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        URL  "http://fltk.org/pub/fltk/1.3.3/fltk-1.3.3-source.tar.gz"
        URL_MD5 "9ccdb0d19dc104b87179bd9fd10822e3"
        UPDATE_COMMAND ""
        SOURCE_DIR  ${temp_path}/${install_prefix}-source
        BINARY_DIR  ${temp_path}/${install_prefix}-build
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
            -DOPTION_BUILD_EXAMPLES:BOOL=OFF
            -DCMAKE_INSTALL_PREFIX:PATH=${FLTK_INSTALL_DIR}
            -DCMAKE_INSTALL_LIBDIR=lib64
    )
ELSE ()
    ExternalProject_Add(fltk
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        URL  "http://fltk.org/pub/fltk/1.3.3/fltk-1.3.3-source.tar.gz"
        URL_MD5 "9ccdb0d19dc104b87179bd9fd10822e3"
        UPDATE_COMMAND ""
        SOURCE_DIR  ${temp_path}/${install_prefix}-source
        BINARY_DIR  ${temp_path}/${install_prefix}-build
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
            -DOPTION_BUILD_EXAMPLES:BOOL=OFF
            -DCMAKE_INSTALL_PREFIX:PATH=${FLTK_INSTALL_DIR}
    )
ENDIF ()

#if(UNIX)
#  include(FindX11)
#  find_library(FLTK_MATH_LIBRARY m)
#  set( FLTK_PLATFORM_DEPENDENT_LIBS ${X11_LIBRARIES} ${FLTK_MATH_LIBRARY})
#endif()

if(APPLE)
    FIND_LIBRARY(AGL_LIBRARY AGL)
    FIND_LIBRARY(CARBON_LIBRARY Carbon)
    FIND_LIBRARY(COCOA_LIBRARY Cocoa)
    SET (FLTK_PLATFORM_DEPENDENT_LIBS ${FLTK_PLATFORM_DEPENDENT_LIBS} ${AGL_LIBRARY} ${CARBON_LIBRARY} ${COCOA_LIBRARY} FORCE)

#  set( FLTK_PLATFORM_DEPENDENT_LIBS  "-framework Carbon -framework Cocoa -framework ApplicationServices -lz")
endif()

IF (WIN32)
    SET (FLTK_FLUID_EXECUTABLE ${FLTK_INSTALL_DIR}/bin/fluid.exe FORCE)
ELSE (WIN32)
    SET (FLTK_FLUID_EXECUTABLE ${FLTK_INSTALL_DIR}/bin/fluid FORCE)
ENDIF (WIN32)

SET (FLTK_INCLUDE_DIR ${staging_prefix}/${install_prefix} FORCE)
set (FLTK_INCLUDE_DIRS ${FLTK_INSTALL_DIR} ${FLTK_INCLUDE_DIR} FORCE)
SET (FLTK_FOUND ON FORCE)

IF (UNIX)
    IF (NOT APPLE)
        FIND_PACKAGE (X11 REQUIRED)

        SET (FLTK_PLATFORM_DEPENDENT_LIBS
            #pthread
            dl
            #SM
            #ICE
            X11
            Xext
            Xinerama
            Xfixes
            Xcursor
            Xft
            fontconfig
            #m
        )
    ENDIF (NOT APPLE)

    SET (FLTK_LIBRARIES
        ${FLTK_INSTALL_DIR}/lib/libfltk.a
        ${FLTK_INSTALL_DIR}/lib/libfltk_forms.a
        ${FLTK_INSTALL_DIR}/lib/libfltk_gl.a
        ${FLTK_INSTALL_DIR}/lib/libfltk_images.a
        ${FLTK_PLATFORM_DEPENDENT_LIBS}
         FORCE
    )
ELSE (UNIX)
    IF (WIN32)
        SET (FLTK_LIBRARIES
            optimized ${FLTK_INSTALL_DIR}/lib/fltk.lib
            debug     ${FLTK_INSTALL_DIR}/lib/fltkd.lib

            optimized ${FLTK_INSTALL_DIR}/lib/fltk_forms.lib
            debug     ${FLTK_INSTALL_DIR}/lib/fltk_formsd.lib

            optimized ${FLTK_INSTALL_DIR}/lib/fltk_gl.lib
            debug     ${FLTK_INSTALL_DIR}/lib/fltk_gld.lib

            optimized ${FLTK_INSTALL_DIR}/lib/fltk_images.lib
            debug     ${FLTK_INSTALL_DIR}/lib/fltk_imagesd.lib
             FORCE
        )
    ENDIF(WIN32)
ENDIF (UNIX)


SET (FLTK_DIR ${staging_prefix}/${install_prefix} FORCE)

# copy libraries into install tree
install(DIRECTORY ${FLTK_INSTALL_DIR}/include/ DESTINATION ${INSTALL_DIR}/third_party/include COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${FLTK_INSTALL_DIR}/lib/     DESTINATION ${INSTALL_DIR}/third_party/lib     COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${FLTK_INSTALL_DIR}/lib64/   DESTINATION ${INSTALL_DIR}/third_party/lib64   COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${FLTK_INSTALL_DIR}/bin/     DESTINATION ${INSTALL_DIR}/third_party/bin     COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${FLTK_INSTALL_DIR}/share/   DESTINATION ${INSTALL_DIR}/third_party/share   COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${FLTK_INSTALL_DIR}/man/     DESTINATION ${INSTALL_DIR}/third_party/man     COMPONENT Development FILES_MATCHING PATTERN "*.*")


endmacro(build_fltk)
