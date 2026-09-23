INCLUDE(CheckIncludeFileCXX)
check_include_file_cxx(filesystem HAVE_FILESYSTEM_HEADER)
IF (NOT HAVE_FILESYSTEM_HEADER)
    MESSAGE(STATUS filesystem C++ header file not found, try using experimental headers)
    SET (requiredLibs "${requiredLibs}" stdc++fs)
ELSE (NOT HAVE_FILESYSTEM_HEADER)
    SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_FILESYSTEM_HEADER")
ENDIF (NOT HAVE_FILESYSTEM_HEADER)


#find_package (Python3 COMPONENTS Development.Embed)
#IF (Python3_Development.Embed_FOUND)
#    SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_PythonLibs")
#    SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} ${Python3_INCLUDE_DIRS})
#    SET (optionalLibs "${optionalLibs}" "${Python3_LIBRARIES}")

#    MESSAGE ("Python3::Development was found. It will be possible to use Speckpy to generate X-ray tube spectrums from the C++ low API."e)

#ELSE (Python3_Development.Embed_FOUND)
#    MESSAGE (STATUS "Python3::Development was not found. It won't be possible to use Speckpy to generate X-ray tube spectrums, at least from the C++ low API.")
#ENDIF (Python3_Development.Embed_FOUND)


# glad #########################################################################
IF (USE_GLAD)
    # Use system library
    IF (USE_SYSTEM_GLAD)
        FIND_PACKAGE( glad REQUIRED)

        SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${glad_INCLUDE_DIRS}")
        SET (optionalLibs ${optionalLibs} ${glad_LIBRARIES})

    ENDIF (USE_SYSTEM_GLAD)

    MESSAGE(STATUS "Use GLAD.")
    SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_GLAD")

ENDIF(USE_GLAD)


# glad #########################################################################

#[[Comment the block to disable GLAD
IF (NOT USE_SYSTEM_GLAD)

    SET (GLAD_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glad/include)


    # Assume GNU/Linux
    IF (NOT WIN32 AND NOT APPLE)

        SET (GLAD_PATH
            ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glad/linux
        )

        SET (GLAD_LIBS
            ${GLAD_PATH}/libglad.a
        )
    ENDIF (NOT WIN32 AND NOT APPLE)

    IF (WIN32)

        SET (GLAD_PATH
            ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glad/windows

        )

        SET (GLAD_SHARED_LIB_PATH ${GLAD_PATH}/libglad-11.dll
            CACHE FILEPATH "Path glad's DLL")

        SET (GLAD_LIBS
            ${GLAD_PATH}/libglad-11.lib
        )

        IF(BUILD_TESTING)
            FILE(COPY ${GLAD_SHARED_LIB_PATH}
                DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/tests_bin)

            IF (BUILD_SIMPLEGVXR OR BUILD_WRAPPERS)
                FILE(COPY ${GLAD_SHARED_LIB_PATH}
                    DESTINATION ${CMAKE_BINARY_DIR}/SimpleGVXR/tests_bin)
            ENDIF (BUILD_SIMPLEGVXR OR BUILD_WRAPPERS)
        ENDIF(BUILD_TESTING)

    ENDIF (WIN32)

    IF (APPLE)
        SET (GLAD_PATH
            ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glad/apple
        )

        SET (GLAD_LIBS ${GLAD_PATH}/libglad.a)

    ENDIF (APPLE)

ENDIF (NOT USE_SYSTEM_GLAD)

SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} ${GLAD_INCLUDE_DIRS} ${GLAD_INCLUDE_DIR})
SET (requiredLibs ${requiredLibs} ${GLAD_LIBS})
SET (gVirtualXRay_GLAD_LIBS ${GLAD_LIBS})
SET (gVirtualXRay_GLAD_PATH ${GLAD_PATH})
SET (gVirtualXRay_LIBRARY_DIRS ${gVirtualXRay_GLAD_PATH} ${gVirtualXRay_LIBRARY_DIRS})
SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_GLAD")
]]


# ZLIB #########################################################################

# Use system library
IF (USE_SYSTEM_ZLIB)
    FIND_PACKAGE( ZLIB )
ENDIF(USE_SYSTEM_ZLIB)

# The system library was not found
# Build Zlib instead
IF(NOT ZLIB_FOUND OR NOT USE_SYSTEM_ZLIB)
    MESSAGE (STATUS "Build Zlib")

    UNSET (USE_SYSTEM_ZLIB)
    SET (USE_SYSTEM_ZLIB OFF)
    SET (USE_SYSTEM_ZLIB OFF CACHE BOOL "Use system's zlib." FORCE )

    INCLUDE(BuildZLIB)
    build_zlib( zlib ${CMAKE_INSTALL_PREFIX}/third_party ${PROJECT_BINARY_DIR} )

    # Dependencies (really needed by Ninja, GNU Make and MS VC++ are not so picky)
    IF (UNIX OR APPLE)
        add_custom_target(zlib-libs
            BYPRODUCTS "${CMAKE_INSTALL_PREFIX}/third_party/zlib-install/lib/libz.a"
            COMMAND ""
            DEPENDS zlib
        )
    ENDIF ()

    IF (WINDOWS)
        add_custom_target(zlib-libs
            BYPRODUCTS "${CMAKE_INSTALL_PREFIX}/third_party/zlib-install/lib/zlib.lib"
            COMMAND ""
            DEPENDS zlib
        )
    ENDIF ()

    #ADD_DEPENDENCIES(gVirtualXRay zlib-libs)

    SET (BUILD_ZLIB ON)
ENDIF ()

IF (ZLIB_FOUND)
    SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${ZLIB_INCLUDE_DIR}")
    SET (gVirtualXRay_LIBRARY_DIRS ${gVirtualXRay_LIBRARY_DIRS} ${ZLIB_LIBRARY_DIR})
    SET (requiredLibs "${requiredLibs}" "${ZLIB_LIBRARY}")
ENDIF (ZLIB_FOUND)

## I commented the code below, not sure why one would use minizip
##FIND_PACKAGE( Minizip )
##IF (Minizip_FOUND)
##    SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${MINIZIP_INCLUDE_DIRS}")
##    SET (requiredLibs "${requiredLibs}" "${MINIZIP_LIBRARY}")
##ENDIF (Minizip_FOUND)


# XRayLib ######################################################################
IF (NOT USE_SYSTEM_XRAYLIB)

    SET (XRAYLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/xraylib/include/xraylib)

    #INSTALL (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/xraylib/include/
    #    DESTINATION ${INSTALL_DIR}/third_party/include
    #    COMPONENT Development
    #    FILES_MATCHING PATTERN "*.*"
    #)


    # Assume GNU/Linux
    IF (NOT WIN32 AND NOT APPLE)

        EXECUTE_PROCESS( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE )

        #INSTALL (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/xraylib/linux
        #    DESTINATION ${INSTALL_DIR}/third_party/lib64
        #    COMPONENT Development
        #    FILES_MATCHING PATTERN "*.*"
        #)

        #INSTALL (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/xraylib/linux
        #    DESTINATION ${INSTALL_DIR}/third_party/lib64
        #    COMPONENT Runtime
        #    FILES_MATCHING PATTERN "*.so"
        #)

        IF ( ${ARCHITECTURE} STREQUAL "aarch64" )
            SET (XRAYLIB_PATH
                ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/xraylib/linux-arm64-v8
            )
        ELSE ()
            IF ( ${ARCHITECTURE} STREQUAL "x86_64" )
                SET (XRAYLIB_PATH
                    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/xraylib/linux-amd64
                )
            ELSE ()
                MESSAGE ( FATAL_ERROR "Architecture: ${ARCHITECTURE} is not supported." )
            ENDIF ()
        ENDIF ()

        SET (XRAYLIB_LIBS
            ${XRAYLIB_PATH}/libxrl.a
        )
    ENDIF (NOT WIN32 AND NOT APPLE)

    IF (WIN32)
        #INSTALL (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/xraylib/windows
        #    DESTINATION ${INSTALL_DIR}/third_party/lib64
        #    COMPONENT Development
        #    FILES_MATCHING PATTERN "*.*"
        #)

        SET (XRAYLIB_PATH
            ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/xraylib/windows

        )

        SET (XRAYLIB_SHARED_LIB_PATH ${XRAYLIB_PATH}/libxrl-11.dll
            CACHE FILEPATH "Path xraylib's DLL")

        SET (XRAYLIB_LIBS
            ${XRAYLIB_PATH}/libxrl-11.lib
        )

        IF(BUILD_TESTING)
            FILE(COPY ${XRAYLIB_SHARED_LIB_PATH}
                DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/tests_bin)

            IF (BUILD_SIMPLEGVXR OR BUILD_WRAPPERS)
                FILE(COPY ${XRAYLIB_SHARED_LIB_PATH}
                    DESTINATION ${CMAKE_BINARY_DIR}/SimpleGVXR/tests_bin)
            ENDIF (BUILD_SIMPLEGVXR OR BUILD_WRAPPERS)
        ENDIF(BUILD_TESTING)

    ENDIF (WIN32)

    IF (APPLE)
        #INSTALL (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/xraylib/apple
        #    DESTINATION ${INSTALL_DIR}/third_party/lib64
        #    COMPONENT Development
        #    FILES_MATCHING PATTERN "*.*"
        #)

        SET (XRAYLIB_PATH
            ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/xraylib/apple
        )

        SET (XRAYLIB_LIBS
            ${XRAYLIB_PATH}/libxrl.la
            ${XRAYLIB_PATH}/libxrl.dylib
            ${XRAYLIB_PATH}/libxrl.11.dylib
        )

    ENDIF (APPLE)

ENDIF (NOT USE_SYSTEM_XRAYLIB)

SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} ${XRAYLIB_INCLUDE_DIRS} ${XRAYLIB_INCLUDE_DIR})
SET (requiredLibs ${requiredLibs} ${XRAYLIB_LIBS})
SET (gVirtualXRay_XRAYLIB_LIBS ${XRAYLIB_LIBS})
SET (gVirtualXRay_XRAYLIB_PATH ${XRAYLIB_PATH})
SET (gVirtualXRay_LIBRARY_DIRS ${gVirtualXRay_XRAYLIB_PATH} ${gVirtualXRay_LIBRARY_DIRS})


# ASSIMP #######################################################################
IF (USE_assimp)
    # Use system library
    IF (USE_SYSTEM_ASSIMP)
        FIND_PACKAGE( assimp REQUIRED)
    ENDIF (USE_SYSTEM_ASSIMP)

    # The system library was not found
    # Build ASSIMP instead
    IF (NOT assimp_FOUND OR NOT USE_SYSTEM_ASSIMP)
        MESSAGE (STATUS "Build ASSIMP")

        UNSET (USE_SYSTEM_ASSIMP)
        SET (USE_SYSTEM_ASSIMP OFF)
        SET (USE_SYSTEM_ASSIMP OFF CACHE BOOL "Use system's ASSIMP." FORCE )

        INCLUDE(BuildASSIMP)
        build_assimp( assimp ${CMAKE_INSTALL_PREFIX}/third_party ${PROJECT_BINARY_DIR} )
    ENDIF (NOT assimp_FOUND OR NOT USE_SYSTEM_ASSIMP)
ENDIF(USE_assimp)

IF (assimp_FOUND)
    MESSAGE(STATUS "Use ASSIMP.")

  	SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${assimp_INCLUDE_DIRS}")
  	SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_ASSIMP")

    IF (assimp_LIBRARIES)
        SET (optionalLibs ${optionalLibs} ${assimp_LIBRARIES})
    ENDIF (assimp_LIBRARIES)

    # Legacy support
    IF (ASSIMP_LIBRARIES)
        SET (optionalLibs ${optionalLibs} ${ASSIMP_LIBRARIES})
    ENDIF (ASSIMP_LIBRARIES)

ELSE (assimp_FOUND)
	MESSAGE (STATUS "ASSIMP not found, the use of ASSIMP will be disable.")
ENDIF (assimp_FOUND)


# OpenMP ######################################################################
FIND_PACKAGE(OpenMP COMPONENTS CXX)

IF (OpenMP_CXX_FOUND)
    MESSAGE (STATUS "Use OpenMP")

    #IF (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    #    MESSAGE (STATUS "MSVC detected")
    #    MESSAGE (STATUS "OpenMP enabled with  -openmp:llvm -openmp:experimental")   
    #    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -openmp:llvm -openmp:experimental" PARENT_SCOPE)
    #ELSE ()
    #    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}" PARENT_SCOPE)
    #    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}" PARENT_SCOPE)
    #ENDIF()

    SET (USE_OPENMP ON PARENT_SCOPE)
    SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_OPENMP")
    SET (requiredLibs ${requiredLibs} OpenMP::OpenMP_CXX)

ELSE (OpenMP_CXX_FOUND)
    MESSAGE (STATUS "OpenMP not found, the use of OpenMP will be disable.")
    SET (USE_OPENMP OFF)
ENDIF (OpenMP_CXX_FOUND)


# OpenMP ######################################################################
FIND_PACKAGE(OpenMP COMPONENTS CUDA)

IF (OpenMP_CUDA_FOUND)
    MESSAGE (STATUS "Use OpenMP GPU offloading")

    SET (USE_OPENMP_GPU ON PARENT_SCOPE)
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CUDA_FLAGS}" PARENT_SCOPE)
    SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_OPENMP_GPU")
    SET (requiredLibs ${requiredLibs} ${OpenMP_EXE_LINKER_FLAGS} OpenMP::OpenMP_CUDA)

ELSE (OpenMP_CUDA_FOUND)
    MESSAGE (STATUS "OpenMP GPU offloading not found, the use of OpenMP GPU offloading will be disable.")
    SET (USE_OPENMP_GPU OFF)
ENDIF (OpenMP_CUDA_FOUND)


# Vulkan ######################################################################
FIND_PACKAGE(Vulkan)

IF (Vulkan_FOUND)
    MESSAGE (STATUS "Use VULKAN")

    SET (USE_VULKAN ON)
    SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${Vulkan_INCLUDE_DIRS}")
    SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_VULKAN" "-DGLFW_INCLUDE_VULKAN")

    SET (optionalLibs ${optionalLibs} ${Vulkan_LIBRARIES})
ELSE (Vulkan_FOUND)
    MESSAGE (STATUS "Vulkan not found, the use of Vulkan will be disable.")
    SET (USE_VULKAN OFF)
ENDIF (Vulkan_FOUND)


# GZIP ########################################################################
FIND_PACKAGE(GZIP)

IF (NOT GZIP_FOUND)
    IF (WIN32)

        file(DOWNLOAD http://gnuwin32.sourceforge.net/downlinks/gzip-bin-zip.php ${PROJECT_BINARY_DIR}/gzip/gzip-bin.zip)
		execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${PROJECT_BINARY_DIR}/gzip/gzip-bin.zip WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/gzip)
        SET (GZIP ${PROJECT_BINARY_DIR}/gzip/bin/gzip.exe)
        SET (GZIP_TOOL ${GZIP} CACHE FILEPATH "Where gzip.exe is." FORCE)

        FIND_PACKAGE(GZIP REQUIRED)
        IF (NOT GZIP_FOUND)
            MESSAGE(FATAL_ERROR "GZIP not found. You can find it on gzip's website at: http://www.gzip.org/")
        ENDIF (NOT GZIP_FOUND)

    ELSE (WIN32)
        MESSAGE(FATAL_ERROR "GZIP not found.")
    ENDIF (WIN32)
ENDIF (NOT GZIP_FOUND)


# OpenGL ######################################################################
IF (USE_OPENGL)
    SET (OpenGL_GL_PREFERENCE "GLVND")
    IF (WIN32 OR APPLE)
        FIND_PACKAGE(OpenGL)
        SET (requiredLibs ${requiredLibs} ${OPENGL_LIBRARIES})

    ELSE (WIN32 OR APPLE)
        FIND_PACKAGE(OpenGL COMPONENTS OpenGL)

        IF (OpenGL_OpenGL_FOUND)
            SET (requiredLibs ${requiredLibs} OpenGL::OpenGL)

            FIND_PACKAGE(OpenGL COMPONENTS GLX EGL GLU)
            IF (OpenGL_GLX_FOUND)
                SET (optionalLibs ${optionalLibs} OpenGL::GLX X11)
            ENDIF (OpenGL_GLX_FOUND)

            IF (OpenGL_EGL_FOUND)
                SET (optionalLibs ${optionalLibs} OpenGL::EGL)
            ENDIF (OpenGL_EGL_FOUND)

            IF (OpenGL_GLU_FOUND)
                SET (optionalLibs ${optionalLibs} OpenGL::GLU)
            ENDIF (OpenGL_GLU_FOUND)
        ENDIF (OpenGL_OpenGL_FOUND)
    ENDIF (WIN32 OR APPLE)

    SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DGLFW_INCLUDE_GLCOREARB")

    IF (OpenGL_OpenGL_FOUND)
        MESSAGE (STATUS "OpenGL found.")
        SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_OPENGL")
        SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${OPENGL_INCLUDE_DIR}")
    ENDIF (OpenGL_OpenGL_FOUND)

    IF (OpenGL_EGL_FOUND)
        MESSAGE (STATUS "EGL found.")
        SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_EGL")
    ENDIF (OpenGL_EGL_FOUND)

    IF (OpenGL_GLX_FOUND)
        MESSAGE (STATUS "EGL found.")
        SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_GLX")
    ENDIF (OpenGL_GLX_FOUND)

    IF (OpenGL_GLU_FOUND)
        MESSAGE (STATUS "GLU found.")
        SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_GLU")
    ENDIF (OpenGL_GLU_FOUND)
ENDIF (USE_OPENGL)


# GLFW3 ########################################################################
IF (USE_GLFW)
    IF (USE_SYSTEM_GLFW)
        find_package(glfw3 REQUIRED)
    ENDIF ()

    INCLUDE (configureGlfw)
ENDIF (USE_GLFW)


# GLEW #########################################################################
IF (USE_GLEW AND OpenGL_OpenGL_FOUND)
    	SET (GLEW_USE_STATIC_LIBS ON)

    # Deals with cross-compilation using Mac OS X (building GLEW does not work in this case)
    IF (USE_SYSTEM_GLEW)
        find_package(GLEW REQUIRED)
    ENDIF ()


    # The system library was not found
    # Build GLEW instead
    IF (NOT GLEW_FOUND OR NOT USE_SYSTEM_GLEW)
        MESSAGE (STATUS "Build GLEW")

        UNSET (USE_SYSTEM_GLEW)
        SET (USE_SYSTEM_GLEW OFF)
        SET (USE_SYSTEM_GLEW OFF CACHE BOOL "Use system's GLEW." FORCE )

        INCLUDE(BuildGLEW)
        build_glew( glew ${CMAKE_INSTALL_PREFIX}/third_party ${PROJECT_BINARY_DIR} )

        # Dependencies (really needed by Ninja, GNU Make and MS VC++ are not so picky)
        IF (UNIX AND NOT APPLE)
            add_custom_target(glew-libs
                BYPRODUCTS "${CMAKE_INSTALL_PREFIX}/third_party/lib64/libGLEW.a"
                COMMAND ""
                DEPENDS glew
            )
        ENDIF (UNIX AND NOT APPLE)

        IF (APPLE)
            add_custom_target(glew-libs
                BYPRODUCTS "${CMAKE_INSTALL_PREFIX}/third_party/lib/libGLEW.a"
                COMMAND ""
                DEPENDS glew
            )
        ENDIF (APPLE)

        IF (WINDOWS)
            add_custom_target(glew-libs
                BYPRODUCTS "${CMAKE_INSTALL_PREFIX}/third_party/lib/GLEW.lib"
                COMMAND ""
                DEPENDS glew
            )
        ENDIF (WINDOWS)

        #ADD_DEPENDENCIES(gVirtualXRay glew-libs)

    ENDIF (NOT GLEW_FOUND OR NOT USE_SYSTEM_GLEW)

    IF (GLEW_FOUND)
        MESSAGE (STATUS "Use GLEW")

        SET (HAS_GLEW ON)
        SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_GLEW")
        SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${glew_INCLUDE_DIRS})
        SET (gVirtualXRay_LIBRARY_DIRS ${gVirtualXRay_LIBRARY_DIRS} ${GLEW_LIBRARY_DIR})

        IF (GLEW_USE_STATIC_LIBS)
            SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DGLEW_STATIC")
        ENDIF (GLEW_USE_STATIC_LIBS)

        IF (USE_SYSTEM_GLEW)
            IF (GLEW_USE_STATIC_LIBS)
                SET (requiredLibs GLEW::glew_s ${requiredLibs})
            ELSE (GLEW_USE_STATIC_LIBS)
                SET (requiredLibs GLEW::GLEW ${requiredLibs})
            ENDIF (GLEW_USE_STATIC_LIBS)
        ELSE (USE_SYSTEM_GLEW)
            SET (requiredLibs ${GLEW_LIBRARY} ${requiredLibs})
        ENDIF ()
    ENDIF (GLEW_FOUND)
ENDIF (USE_GLEW AND OpenGL_OpenGL_FOUND)


# TIFF #########################################################################
IF (USE_LIBTIFF)
    # Use system library
    IF (USE_SYSTEM_LIBTIFF)
        FIND_PACKAGE( TIFF )
    ENDIF ()

    # The system library was not found
    # Build LibTIFF instead
    IF (NOT TIFF_FOUND)
        MESSAGE (STATUS "Build LibTIFF")

        UNSET (USE_SYSTEM_LIBTIFF)
        SET (USE_SYSTEM_LIBTIFF OFF)
        SET (USE_SYSTEM_LIBTIFF OFF CACHE BOOL "Use system's LibTIFF." FORCE )

        INCLUDE(BuildLibTIFF)
        build_libtiff( LibTIFF ${CMAKE_INSTALL_PREFIX}/third_party ${PROJECT_BINARY_DIR} )

        # Dependencies (really needed by Ninja, GNU Make and MS VC++ are not so picky)
        IF (UNIX AND NOT APPLE)
            add_custom_target(libtiff-libs
                BYPRODUCTS "${CMAKE_INSTALL_PREFIX}/third_party/LibTIFF-install/lib64/libtiff.a"
                COMMAND ""
                DEPENDS libtiff
            )
        ENDIF (UNIX AND NOT APPLE)

        IF (APPLE)
            add_custom_target(libtiff-libs
                BYPRODUCTS "${CMAKE_INSTALL_PREFIX}/third_party/LibTIFF-install/lib/libtiff.a"
                COMMAND ""
                DEPENDS libtiff
            )
        ENDIF (APPLE)

        IF (WINDOWS)
            add_custom_target(libtiff-libs
                BYPRODUCTS "${CMAKE_INSTALL_PREFIX}/third_party/LibTIFF-install/lib/tiff.lib"
                COMMAND ""
                DEPENDS libtiff
            )
        ENDIF (WINDOWS)

        #ADD_DEPENDENCIES(libtiff libtiff-libs)
    ENDIF (NOT TIFF_FOUND)
ENDIF (USE_LIBTIFF)

IF (TIFF_FOUND)
    MESSAGE (STATUS "Use LibTIFF")

    SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_LIBTIFF" "-DHAS_TIFF" "-DTIFF_DISABLE_DEPRECATED")
    SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${TIFF_INCLUDE_DIRS}")
    SET (gVirtualXRay_LIBRARY_DIRS ${gVirtualXRay_LIBRARY_DIRS} ${TIFF_LIBRARY_DIR})
    SET (requiredLibs "${requiredLibs}" "${TIFF_LIBRARIES}")
ELSE (TIFF_FOUND)
    MESSAGE (STATUS "LibTIFF not found, the use of LibTIFF will be disable.")
ENDIF (TIFF_FOUND)

# GDCM #########################################################################
IF (USE_GDCM)
    # Use system library
    IF (USE_SYSTEM_GDCM)
        FIND_PACKAGE( GDCM )
    ENDIF(USE_SYSTEM_GDCM)

    # The system library was not found
    # Build GDCM instead
    IF (NOT GDCM_FOUND OR NOT USE_SYSTEM_GDCM)
        MESSAGE (STATUS "Build GDCM")

        UNSET (USE_SYSTEM_GDCM)
        SET (USE_SYSTEM_GDCM OFF)
        SET (USE_SYSTEM_GDCM OFF CACHE BOOL "Use system's GDCM." FORCE )

        INCLUDE(BuildGDCM)
        build_gdcm( gdcm ${CMAKE_INSTALL_PREFIX}/third_party ${PROJECT_BINARY_DIR} )
    ENDIF (NOT GDCM_FOUND OR NOT USE_SYSTEM_GDCM)
ENDIF(USE_GDCM)

IF (GDCM_FOUND)
    MESSAGE(STATUS "Use GDCM.")

    SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_GDCM")
    SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${GDCM_INCLUDE_DIRS}")
    SET (gVirtualXRay_LIBRARY_DIRS ${gVirtualXRay_LIBRARY_DIRS} ${GDCM_LIBRARY_DIR})
    SET (requiredLibs "${requiredLibs}" "${GDCM_LIBRARIES}")
ELSE (GDCM_FOUND)
    MESSAGE (STATUS "GDCM not found, the use of GDCM will be disable.")
ENDIF (GDCM_FOUND)


# VCG ##########################################################################
# I commented the section below as VCG is not used as yet
#[[http://vcglib.net/
IF (USE_VCG)
    # Use system library
    IF (USE_SYSTEM_VCG)
        FIND_PACKAGE( VCG )
    ENDIF(USE_SYSTEM_VCG)

    # The system library was not found
    # Build VCG instead
    IF(NOT VCG_FOUND OR NOT USE_SYSTEM_VCG)
        MESSAGE (STATUS "Build VCG")
        INCLUDE(BuildVCG)
        build_vcg( vcg ${CMAKE_INSTALL_PREFIX}/third_party ${PROJECT_BINARY_DIR} )
    ENDIF (NOT VCG_FOUND OR NOT USE_SYSTEM_VCG)

    IF (VCG_FOUND)
        MESSAGE (STATUS "USE VCG")
        SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${VCG_INCLUDE_DIR}")
    ELSE (VCG_FOUND)
        MESSAGE (STATUS "VCG not found, the use of VCG will be disable.")
    ENDIF (VCG_FOUND)
ENDIF (USE_VCG)
]]


# ITK #########################################################################
IF (USE_ITK)
    FIND_PACKAGE(ITK)
ENDIF (USE_ITK)

IF (ITK_FOUND)
    include(${ITK_USE_FILE})
    SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_ITK")
    SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${ITK_INCLUDE_DIRS}")
    SET (requiredLibs ${requiredLibs} ${ITK_LIBRARIES})
ELSE(ITK_FOUND)
    MESSAGE(STATUS "ITK not found, it will be disable.")
    SET (USE_ITK OFF)
ENDIF (ITK_FOUND)


# FFTW3 ########################################################################
IF (USE_FFTW3)

    # Use system library
    IF (USE_SYSTEM_FFTW3)
        FIND_LIBRARY(FFTW3_LIBRARIES NAMES fftw3 libfftw3 libfftw3_omp)
    ENDIF (USE_SYSTEM_FFTW3)

    # The system library was not found
    # Build FFTW3 instead (Unixes only)
    IF (NOT USE_SYSTEM_FFTW3 OR NOT FFTW3_LIBRARIES)
        IF (UNIX)
            MESSAGE (STATUS "Build FFTW3")

            UNSET (USE_SYSTEM_FFTW3)
            SET (USE_SYSTEM_FFTW3 OFF)
            SET (USE_SYSTEM_FFTW3 OFF CACHE BOOL "Use system's FFTW3." FORCE )

            INCLUDE(BuildFFTW3)
            build_fftw3( fftw3 ${CMAKE_INSTALL_PREFIX}/third_party ${PROJECT_BINARY_DIR} )
        ELSE (UNIX)
            MESSAGE (STATUS "Building FFTW3 on windows automatically is not supported. Either disable the Fast Fourier Transform, or build it by hand.")
        ENDIF (UNIX)
    ENDIF (NOT USE_SYSTEM_FFTW3 OR NOT FFTW3_LIBRARIES)

    IF (FFTW3_LIBRARIES)
        MESSAGE(STATUS "Use FFTW3.")

        SET (requiredLibs ${requiredLibs} ${FFTW3_LIBRARIES})
        SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_FFTW")
        SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${FFTW3_INCLUDE_DIR}")
        SET (gVirtualXRay_LIBRARY_DIRS ${gVirtualXRay_LIBRARY_DIRS} ${FFTW3_LIBRARY_DIR})
        SET (requiredLibs "${requiredLibs}" "${FFTW3_LIBRARY}")
    ELSE ()
        MESSAGE (STATUS "FFTW3 not found, the use of FFT will be disable.")
    ENDIF ()

ENDIF (USE_FFTW3)



# FREETYPE ####################################################################
IF (USE_FREETYPE)
    # Use system library
    IF (USE_SYSTEM_FREETYPE)
        FIND_PACKAGE(Freetype)
    ENDIF (USE_SYSTEM_FREETYPE)

    # The system library was not found
    # Build Freetype instead
    IF (NOT USE_SYSTEM_FREETYPE OR NOT FREETYPE_FOUND)
        MESSAGE (STATUS "Build Freetype")

        UNSET (USE_SYSTEM_FREETYPE)
        SET (USE_SYSTEM_FREETYPE OFF)
        SET (USE_SYSTEM_FREETYPE OFF CACHE BOOL "Use system's FreeType." FORCE )

        INCLUDE(BuildFreeType)
        build_freetype( freetype ${CMAKE_INSTALL_PREFIX}/third_party ${PROJECT_BINARY_DIR} )

        # Dependencies (really needed by Ninja, GNU Make and MS VC++ are not so picky)
        IF (UNIX AND NOT APPLE)
            add_custom_target(freetype-libs
                    BYPRODUCTS "${CMAKE_INSTALL_PREFIX}/third_party/freetype-install/lib64/libfreetype.a"
                    COMMAND ""
                    DEPENDS freetype
            )
        ENDIF (UNIX AND NOT APPLE)

        IF (APPLE)
            add_custom_target(freetype-libs
                    BYPRODUCTS "${CMAKE_INSTALL_PREFIX}/third_party/freetype-install/lib/libfreetype.a"
                    COMMAND ""
                    DEPENDS freetype
            )
        ENDIF (APPLE)

        IF (WINDOWS)
            add_custom_target(freetype-libs
                BYPRODUCTS "${CMAKE_INSTALL_PREFIX}/third_party/freetype-install/lib/freetype.lib"
                COMMAND ""
                DEPENDS freetype
            )
        ENDIF (WINDOWS)

    ENDIF (NOT USE_SYSTEM_FREETYPE OR NOT FREETYPE_FOUND)
ENDIF (USE_FREETYPE)

IF (USE_FREETYPE AND FREETYPE_FOUND)
    MESSAGE (STATUS "Use Freetype")

    SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_FREETYPE")
    SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${FREETYPE_INCLUDE_DIRS}")
    SET (optionalLibs "${optionalLibs}" "${FREETYPE_LIBRARIES}")

    FIND_PACKAGE(PNG)
    IF (PNG_FOUND)
        SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${PNG_INCLUDE_DIRS}")
        SET(optionalLibs ${optionalLibs} ${PNG_LIBRARIES})
    ENDIF (PNG_FOUND)

    FIND_PACKAGE(BZip2)
    IF (BZIP2_FOUND)
        SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${BZIP2_INCLUDE_DIR}")
        SET(optionalLibs ${optionalLibs} ${BZIP2_LIBRARIES})
    ENDIF (BZIP2_FOUND)

    #FIND_PACKAGE(HARFBUZZ)
    #IF (HARFBUZZ_FOUND)
    #    SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${HARFBUZZ_INCLUDE_DIRS}")
    #    SET(optionalLibs ${optionalLibs} ${HARFBUZZ_LIBRARIES})
    #ENDIF (HARFBUZZ_FOUND)

ELSE (USE_FREETYPE AND FREETYPE_FOUND)
    MESSAGE (STATUS "Freetype not found, the use of Freetype will be disable.")
ENDIF (USE_FREETYPE AND FREETYPE_FOUND)


# getopt ######################################################################
# IF (WIN32 AND MSVC)

#     FIND_PACKAGE(Git)

#     IF (GIT_FOUND)
#         INCLUDE (ExternalProject)
#         ExternalProject_Add(getopt
#             PREFIX ${CMAKE_CURRENT_BINARY_DIR}/getopt
#             #DOWNLOAD_DIR ${CMAKE_CURRENT_BINARY_DIR}/download/getopt
#             GIT_REPOSITORY "https://github.com/skandhurkat/Getopt-for-Visual-Studio.git"
#             CONFIGURE_COMMAND ""
#             UPDATE_COMMAND ""
#             BUILD_COMMAND ""
#             INSTALL_COMMAND ""
#         )

#         INCLUDE_DIRECTORIES (${CMAKE_CURRENT_BINARY_DIR}/getopt/src/getopt)
#         SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${CMAKE_CURRENT_BINARY_DIR}/getopt/src/getopt")

#         # copy libraries into install tree
#         INSTALL (DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/getopt/src/getopt
#             DESTINATION ${INSTALL_DIR}/third_party
#             COMPONENT Development)

#         SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${INSTALL_DIR}/third_party/getopt")

#         SET (USE_EXTERNAL_GETOPT ON)
#         SET (HAS_GETOPT ON)
#     ELSE (GIT_FOUND)
#         SET (HAS_GETOPT OFF)
#         MESSAGE (WARNING "Install Git to download Getopt (it is not available in MS Visual Studio). If you don't (which is fine) command line tools won't be built.")
#     ENDIF (GIT_FOUND)
# ELSE (WIN32 AND MSVC)
#     SET (HAS_GETOPT ON)
# ENDIF (WIN32 AND MSVC)


# GDM ##########################################################################
if(UNIX AND NOT APPLE)
    FIND_PACKAGE(GBM)
endif()

IF (GBM_FOUND)
    MESSAGE(STATUS "Use GBM.")

    SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_GBM")
    SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${GBM_INCLUDE_DIRS}")
    SET (requiredLibs "${requiredLibs}" "${GBM_LIBRARIES}")
ELSE (GDCM_FOUND)
    MESSAGE (STATUS "GBM not found, the use of GBM will be disable.")
ENDIF (GBM_FOUND)
