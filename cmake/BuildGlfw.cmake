# make a custom GLFW configuration file
macro(build_glfw install_prefix staging_prefix temp_path)

#    SET (URL "https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip")
#    SET (MD5 "7d1e90c5c0011eb10eac280b6093d0d0")

#    SET (glfw3_VERSION_STRING 3.4 FORCE)
#    SET (glfw3_VERSION_MAJOR  3.4 FORCE)
#    SET (glfw3_VERSION_MINOR  0   FORCE)

    SET (URL "https://github.com/glfw/glfw/releases/download/3.3.10/glfw-3.3.10.zip")
    SET (MD5 "ca85f61c76a041e8be1a9f6e76e189f6")

    SET (glfw3_VERSION_STRING 3.3 FORCE)
    SET (glfw3_VERSION_MAJOR  3.3 FORCE)
    SET (glfw3_VERSION_MINOR  10   FORCE)

    SET (INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/third_party)
    SET (INSTALL_LIBDIR lib)
    #SET (INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/${install_prefix}-install)

    SET (USE_SUPERBUILD ON)
    INCLUDE(ExternalProject)


    set(CMAKE_OSX_EXTERNAL_PROJECT_ARGS)
    if(APPLE)
        list(APPEND CMAKE_OSX_EXTERNAL_PROJECT_ARGS
            -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
            -DCMAKE_OSX_SYSROOT=${CMAKE_OSX_SYSROOT}
            -DMACOSX_RPATH:BOOL=ON
            -DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}
        )
    endif()

    enable_language(C)
    enable_language(CXX)

    # C++ version ##################################################################
    set(CMAKE_CXX_STANDARD 17) # C++17
    set(CMAKE_CXX_STANDARD_REQUIRED ON) # C++17 is required (I.e. not optional)
    set(CMAKE_CXX_EXTENSIONS ON) # without compiler extensions like gnu++11

    SET (glfw3_INCLUDE_DIR ${INSTALL_DIR}/include CACHE PATH "Where to find GLFW3 header files" FORCE)
    SET (glfw3_DIR         ${INSTALL_DIR}/${INSTALL_LIBDIR}/cmake/glfw3 CACHE PATH "Where to find Cmake files on GLFW3" FORCE)
    SET (glfw3_FOUND ON FORCE)
    SET (glfw3_LIBRARY_DIR "${INSTALL_DIR}/${INSTALL_LIBDIR}")
    SET (glfw3_LIBRARY     ${INSTALL_DIR}/${INSTALL_LIBDIR}/libglfw3.a CACHE FILEPATH "Path to the GLFW3 library" FORCE)

    IF (APPLE)
        SET (glfw3_LIBRARY     ${INSTALL_DIR}/${INSTALL_LIBDIR}/libglfw3.a CACHE FILEPATH "Path to the GLFW3 library" FORCE)
    ENDIF (APPLE)
    IF (WIN32)
        IF (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
            string(FIND ${CMAKE_GENERATOR} "Visual Studio" GENERATED_WITH_MSVC)
            IF (GENERATED_WITH_MSVC GREATER -1)
                SET(glfw3_LIBRARY     ${INSTALL_DIR}/${INSTALL_LIBDIR}/glfw3.lib CACHE FILEPATH "Path to the GLFW3 library" FORCE)
            ELSE ()
                SET(glfw3_LIBRARY     ${INSTALL_DIR}/${INSTALL_LIBDIR}/libglfw3.a CACHE FILEPATH "Path to the GLFW3 library" FORCE)
            ENDIF ()
        ENDIF ()
    ENDIF (WIN32)

        IF (UNIX AND NOT APPLE)

        # Use X11 if possible
        FIND_PACKAGE (X11)

        IF (X11_FOUND)
            SET (USE_X11 ON)
        ELSE (X11_FOUND ON)
            SET (USE_X11 OFF)
        ENDIF (X11_FOUND)

        ExternalProject_Add(glfw
            DOWNLOAD_EXTRACT_TIMESTAMP TRUE
            URL  ${URL}
            URL_MD5 ${MD5}
            #GIT_REPOSITORY https://github.com/glfw/glfw.git
            UPDATE_COMMAND ""
            SOURCE_DIR  ${temp_path}/${install_prefix}-source
            BINARY_DIR  ${temp_path}/${install_prefix}-build
            INSTALL_DIR ${INSTALL_DIR}
            LIST_SEPARATOR :::
            CMAKE_GENERATOR ${CMAKE_GEN}
            BUILD_BYPRODUCTS ${glfw3_LIBRARY}
            CMAKE_ARGS
                #-DGLFW_BUILD_POSIX_TIMER:BOOL=OFF
                #-DPOSIX_REQUIRED_STANDARD=199309L
                #-D_POSIX_C_SOURCE=199309L
                #-D_POSIX_SOURCE=199309L
                -DCMAKE_BUILD_TYPE:STRING=Release
                -DBUILD_SHARED_LIBS:BOOL=OFF
                -DCMAKE_INSTALL_PREFIX:PATH=${INSTALL_DIR}
                -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
                -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
                -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
                -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
                -DCMAKE_EXE_LINKER_FLAGS:STRING=${LINKER_FLAGS}
                -DBUILD_SHARED_LIBS:BOOL=OFF
                -DCMAKE_TOOLCHAIN_FILE:FILEPATH=${CMAKE_TOOLCHAIN_FILE}
                -DGLFW_BUILD_DOCS:BOOL=OFF
                -DGLFW_BUILD_EXAMPLES:BOOL=OFF
                -DGLFW_BUILD_TESTS:BOOL=OFF
                -DGLFW_USE_EGL:BOOL=ON
                -DGLFW_BUILD_WAYLAND:BOOL=OFF
                -DGLFW_BUILD_X11:BOOL=${USE_X11}
                -DUSE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF
                -DGLFW_USE_HYBRID_HPG:BOOL=ON
                -DCMAKE_INSTALL_LIBDIR=${INSTALL_LIBDIR}
            #INSTALL_COMMAND ""
        )
    ELSE ()
        ExternalProject_Add(glfw
            DOWNLOAD_EXTRACT_TIMESTAMP TRUE
            URL  ${URL}
            URL_MD5 ${MD5}
            UPDATE_COMMAND ""
            SOURCE_DIR  ${temp_path}/${install_prefix}-source
            BINARY_DIR  ${temp_path}/${install_prefix}-build
            INSTALL_DIR ${INSTALL_DIR}
            LIST_SEPARATOR :::
            CMAKE_GENERATOR ${CMAKE_GEN}
            BUILD_BYPRODUCTS ${glfw3_LIBRARY}
            CMAKE_ARGS
                -DCMAKE_BUILD_TYPE:STRING=Release
                -DBUILD_SHARED_LIBS:BOOL=OFF
                -DCMAKE_INSTALL_PREFIX:PATH=${INSTALL_DIR}
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
                -DGLFW_BUILD_DOCS:BOOL=OFF
                -DGLFW_BUILD_EXAMPLES:BOOL=OFF
                -DGLFW_BUILD_TESTS:BOOL=OFF
                -DDGLFW_USE_EGL:BOOL=ON
                -DUSE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF
                -DGLFW_USE_HYBRID_HPG:BOOL=ON
                -DCMAKE_INSTALL_LIBDIR=${INSTALL_LIBDIR}
            #INSTALL_COMMAND ""
        )
    ENDIF ()

    # install(DIRECTORY ${INSTALL_DIR}/include/ DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/include COMPONENT Development FILES_MATCHING PATTERN "*.*")
    # install(DIRECTORY ${INSTALL_DIR}/lib/     DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/lib     COMPONENT Development FILES_MATCHING PATTERN "*.*")
    # install(DIRECTORY ${INSTALL_DIR}/lib64/   DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/lib64   COMPONENT Development FILES_MATCHING PATTERN "*.*")
    # install(DIRECTORY ${INSTALL_DIR}/bin/     DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/bin     COMPONENT Development FILES_MATCHING PATTERN "*.*")
    # install(DIRECTORY ${INSTALL_DIR}/share/   DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/share   COMPONENT Development FILES_MATCHING PATTERN "*.*")
    # install(DIRECTORY ${INSTALL_DIR}/man/     DESTINATION ${CMAKE_INSTALL_PREFIX}/third_party/man     COMPONENT Development FILES_MATCHING PATTERN "*.*")


    # SET (CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${INSTALL_DIR}/${INSTALL_LIBDIR}/cmake/glfw3 ${INSTALL_DIR}/${INSTALL_LIBDIR}/cmake  ${INSTALL_DIR}/${INSTALL_LIBDIR})
    # SET (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${INSTALL_DIR}/${INSTALL_LIBDIR}/cmake/glfw3 ${INSTALL_DIR}/lib/cmake  ${INSTALL_DIR}/lib)



endmacro(build_glfw)
