macro(build_glew install_prefix staging_prefix temp_path)

FIND_PACKAGE(OpenGL REQUIRED)

IF (NOT OPENGL_GLU_FOUND)
	MESSAGE (FATAL_ERROR "Glu not found, GLEW cannot be built.")
ENDIF ()

SET (URL "https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0.tgz")
SET (MD5 "3579164bccaef09e36c0af7f4fd5c7c7")

#SET (GLEW_VERSION_STRING 2.2)
#SET (GLEW_VERSION_MAJOR  2.2)
#SET (GLEW_VERSION_MINOR  0)


SET (INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/third_party)
if (WIN32)
    SET (INSTALL_LIBDIR lib)
ELSE (WIN32)
    SET (INSTALL_LIBDIR lib64)
ENDIF (WIN32)

ExternalProject_Add(getGLEW
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
  URL  ${URL}
  URL_MD5 ${MD5}
  UPDATE_COMMAND ""
  SOURCE_DIR  ${temp_path}/${install_prefix}-source
  LIST_SEPARATOR :::
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  CMAKE_GENERATOR ""
  INSTALL_COMMAND ""
)

IF (UNIX AND NOT APPLE)
    ExternalProject_Add(glew
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        DOWNLOAD_COMMAND ""
        UPDATE_COMMAND ""
        SOURCE_DIR  ${temp_path}/${install_prefix}-source/build/cmake
        BINARY_DIR  ${temp_path}/${install_prefix}-build
        INSTALL_DIR ${INSTALL_DIR}
        LIST_SEPARATOR :::
        #BUILD_IN_SOURCE 1
        CMAKE_GENERATOR ${CMAKE_GEN}
        CMAKE_ARGS
            ${LOCAL_CMAKE_BUILD_OPTIONS}
            -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
            -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
            -DCMAKE_BUILD_TYPE:STRING=Release
            -DCMAKE_INSTALL_PREFIX:PATH=${INSTALL_DIR}
            -DCMAKE_INSTALL_LIBDIR=${INSTALL_LIBDIR}
            -DCMAKE_POLICY_VERSION_MINIMUM=3.5
    )
ELSE ()
    ExternalProject_Add(glew
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        DOWNLOAD_COMMAND ""
        UPDATE_COMMAND ""
        SOURCE_DIR  ${temp_path}/${install_prefix}-source/build/cmake
        BINARY_DIR  ${temp_path}/${install_prefix}-build
        INSTALL_DIR ${INSTALL_DIR}
        LIST_SEPARATOR :::
        #BUILD_IN_SOURCE 1
        CMAKE_GENERATOR ${CMAKE_GEN}
        CMAKE_ARGS
            ${LOCAL_CMAKE_BUILD_OPTIONS}
            -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
            -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
            -DCMAKE_BUILD_TYPE:STRING=Release
            -DCMAKE_INSTALL_PREFIX:PATH=${INSTALL_DIR}
            -DCMAKE_INSTALL_LIBDIR=${INSTALL_LIBDIR}
            -DCMAKE_POLICY_VERSION_MINIMUM=3.5
    )
ENDIF ()

ADD_DEPENDENCIES(glew getGLEW)

SET(GLEW_INCLUDE_DIR ${INSTALL_DIR}/include  ${INSTALL_DIR}/include CACHE PATH "Where to find GLEW header files" FORCE)
SET(GLEW_INCLUDE_DIRS ${INSTALL_DIR}/include  ${INSTALL_DIR}/include CACHE PATH "Where to find GLEW header files" FORCE)
SET(GLEW_DIR         ${INSTALL_DIR} CACHE PATH "Where to find GLEW" FORCE)
SET(GLEW_FOUND ON)

IF (UNIX)
    SET(GLEW_LIBRARY     ${INSTALL_DIR}/${INSTALL_LIBDIR}/libGLEW.a   CACHE FILEPATH "GLEW library" FORCE)
ENDIF (UNIX)

IF (APPLE)
    SET(GLEW_LIBRARY     ${INSTALL_DIR}/${INSTALL_LIBDIR}/libGLEW.a  CACHE FILEPATH "GLEW library" FORCE)
ENDIF (APPLE)

if (WIN32)
    if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
        string(FIND ${CMAKE_GENERATOR} "Visual Studio" GENERATED_WITH_MSVC)
        if (GENERATED_WITH_MSVC GREATER -1)
            SET(GLEW_LIBRARY
                    optimized ${INSTALL_DIR}/${INSTALL_LIBDIR}/libglew32.lib
                    debug     ${INSTALL_DIR}/${INSTALL_LIBDIR}/libglew32d.lib
                    CACHE FILEPATH "GLEW library" FORCE
            )
        else (GENERATED_WITH_MSVC GREATER -1)
            SET(GLEW_LIBRARY
                    ${INSTALL_DIR}/${INSTALL_LIBDIR}/libglew32.lib
                    CACHE FILEPATH "GLEW library" FORCE
            )
        endif (GENERATED_WITH_MSVC GREATER -1)
    ELSE ()
        SET(GLEW_LIBRARY
            ${INSTALL_DIR}/${INSTALL_LIBDIR}/libglew32.a
            CACHE FILEPATH "GLEW library" FORCE
        )
    ENDIF ()
endif (WIN32)


# copy libraries into install tree
# install(DIRECTORY ${INSTALL_DIR}/include/ DESTINATION ${INSTALL_DIR}/third_party/include COMPONENT Development FILES_MATCHING PATTERN "*.*")
# install(DIRECTORY ${INSTALL_DIR}/lib/     DESTINATION ${INSTALL_DIR}/third_party/lib     COMPONENT Development FILES_MATCHING PATTERN "*.*")
# install(DIRECTORY ${INSTALL_DIR}/lib64/   DESTINATION ${INSTALL_DIR}/third_party/lib64   COMPONENT Development FILES_MATCHING PATTERN "*.*")
# install(DIRECTORY ${INSTALL_DIR}/bin/     DESTINATION ${INSTALL_DIR}/third_party/bin     COMPONENT Development FILES_MATCHING PATTERN "*.*")
# install(DIRECTORY ${INSTALL_DIR}/share/   DESTINATION ${INSTALL_DIR}/third_party/share   COMPONENT Development FILES_MATCHING PATTERN "*.*")
# install(DIRECTORY ${INSTALL_DIR}/man/     DESTINATION ${INSTALL_DIR}/third_party/man     COMPONENT Development FILES_MATCHING PATTERN "*.*")

# install(DIRECTORY ${INSTALL_DIR}/lib/     DESTINATION ${INSTALL_DIR}/third_party/lib     COMPONENT Runtime FILES_MATCHING PATTERN "*.so*")
# install(DIRECTORY ${INSTALL_DIR}/lib64/   DESTINATION ${INSTALL_DIR}/third_party/lib     COMPONENT Runtime FILES_MATCHING PATTERN "*.so*")

endmacro(build_glew)
