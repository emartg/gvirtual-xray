#
#
# Copyright 2011-2023, Dr Franck P. Vidal, School of Computer science and 
# Engineering, Bangor University. All rights reserved.
#
# Copyright 2024-, Prof Franck P. Vidal, Computed Tomography, 
# Scientific Computing, Science and Technology Facilities Council (STFC). 
# All rights reserved
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
# this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation and/or
# other materials provided with the distribution.
#
# 3. Neither the names of the Bangor University and STFC nor the names of its contributors
# may be used to endorse or promote products derived from this software without
# specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


# 3.26 is the oldest version that the build has been checked with, and the one that gvxr/cmake/Bin2C.cmake requires.
# A newer version sets the policies as 4.2 does
cmake_minimum_required(VERSION 3.26...4.2 FATAL_ERROR)

project(gVirtualXRay-Superbuild
        VERSION 2.1.0
        DESCRIPTION "gVirtualXRay is focused on developing new software technologies for simulating X-ray images on the graphics processor unit (GPU) using OpenGL. It supports ‘old’ OpenGL implementation as well as modern OpenGL core profile (OpenGL 3.2+). No deprecated function in OpenGL has been used. The library takes care of matrix transformations, matrix stacks, etc."
        HOMEPAGE_URL https://sourceforge.net/projects/gvirtualxray/
        LANGUAGES CXX
)

# Check for Git LFS
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CheckGitLFS.cmake)


# Use from another project #####################################################
# When another project adds this one with ADD_SUBDIRECTORY, that project owns the installation, the
# packaging, the build type, and the test driver. Link to the target gVirtualXRay::gVirtualXRay.
OPTION (gVXR_INSTALL "Generate the installation rules and the packaging of gVirtualXRay" ${PROJECT_IS_TOP_LEVEL})

# The Debug objects reference the coverage runtime when they are compiled with the coverage flags (GCC and Clang on
# Linux), so it is off by default. When it is on, the target adds the linker option for its consumers
OPTION (gVXR_COVERAGE "Compile the Debug configuration with the coverage flags on Linux" OFF)


# Set CMake module path to include the cmake directory
SET (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


# Set a default build type if none was specified
IF (PROJECT_IS_TOP_LEVEL AND NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
      message(STATUS "Setting build type to 'Release' as none was specified.")
      SET (CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)

      # Set the possible values of build type for cmake-gui
      set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
ENDIF ()


# C++ version ##################################################################
set(CMAKE_CXX_STANDARD 23) # C++23
set(CMAKE_CXX_STANDARD_REQUIRED ON) # C++23 is required (i.e. not optional)
set(CMAKE_CXX_EXTENSIONS OFF) # without compiler extensions like gnu++11

# Disable warnings for unused variables and unused but set variables
if (CMAKE_COMPILER_IS_GNUCXX OR MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error -Wno-unused-but-set-variable")
endif()

# Treat ASSIMP warnings as errors
if (ASSIMP_WARNINGS_AS_ERRORS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
endif()

# MinGW std::byte collision fix
if (MINGW)
    add_compile_options(-include "${CMAKE_CURRENT_SOURCE_DIR}/mingw_fix.h")
endif()

# Flag required for shared libraries
SET (BUILD_SHARED_LIBS OFF)
IF (UNIX)
    IF (NOT APPLE)
        #IF ("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64")
            SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -Wall")
            SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wall")
            IF (gVXR_COVERAGE)
                SET (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
            ENDIF (gVXR_COVERAGE)

        #ENDIF ("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64")
    ENDIF (NOT APPLE)
ENDIF (UNIX)


IF (APPLE)
    SET (CMAKE_SHARED_LINKER_FLAGS "-undefined dynamic_lookup")
    SET (CMAKE_MODULE_LINKER_FLAGS "-undefined dynamic_lookup")
    SET (CMAKE_SHARED_MODULE_CREATE_CXX_FLAGS  "-bundle -Wl,-headerpad_max_install_names -undefined dynamic_lookup")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -undefined dynamic_lookup")
ENDIF (APPLE)

IF (MSVC)
    # Try adding parallel build on Windows
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")

    # Standard C++ exception handling: ISO-standard C++ exception handling
    # See: https://learn.microsoft.com/en-us/cpp/build/reference/eh-exception-handling-model
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")

    # Try fixing issue with OpenMP and Python wheels
    SET (CMAKE_INSTALL_OPENMP_LIBRARIES TRUE)
    INCLUDE (InstallRequiredSystemLibraries)
ENDIF (MSVC)




# Make include files in paths like /usr/local/include have preference before
# files in OSX frameworks.
SET (CMAKE_FIND_FRAMEWORK "LAST")


# For superbuild
SET (USE_SUPERBUILD ON)
INCLUDE(ExternalProject)
#INCLUDE(ConfigureExternalProjects)


# Options ######################################################################
INCLUDE (gvxr/cmake/options.cmake)
INCLUDE (SimpleGVXR/cmake/options.cmake)
INCLUDE (Wrappers/cmake/options.cmake)

# Enable SimpleGVXR if a wrapper is built
IF (BUILD_WRAPPERS)
    MESSAGE (STATUS "BUILD_WRAPPERS is ${BUILD_WRAPPERS}, SimpleGVXR will be built automatically.")
    SET (BUILD_SIMPLEGVXR ON)
# No wrapper, SimpleGVXR can still be built as an optional component
ELSE (BUILD_WRAPPERS)
    option(BUILD_SIMPLEGVXR    "Build SimpleGVXR module" ON)
    MESSAGE (STATUS "BUILD_WRAPPERS is ${BUILD_WRAPPERS}, you can still build SimpleGVXR if you'd like (default is ON)")
ENDIF (BUILD_WRAPPERS)


# CTest and Unit testing #######################################################
IF (PROJECT_IS_TOP_LEVEL)
    # Support for the CTest dashboard testing system
    INCLUDE(CTest)
ELSE ()
    # BUILD_TESTING belongs to the project that added this one: do not build the tests of gVirtualXRay
    SET (BUILD_TESTING OFF)
ENDIF ()

# Enable the unit tests
IF (BUILD_TESTING)
    ENABLE_TESTING()

    SET (OPTIONAL_TESTS "")

    OPTION (USE_CLANG_TIDY "Use clang-tidy if available. " OFF)
    OPTION (USE_CPPCHECK "Use cppcheck if available. " OFF)

    if (USE_CLANG_TIDY)
        find_program(CLANG_TIDY_COMMAND NAMES clang-tidy)
        if (CLANG_TIDY_COMMAND)
            set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
           # set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND};-p=${CMAKE_BINARY_DIR};${CLANGTIDY_EXTRA_ARGS}" CACHE STRING "" FORCE)

    #      add_custom_target(clang-tidy
    #           COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target clang-tidy
    #            )

            add_custom_target(clang-tidy
                #COMMAND sh -c "run-clang-tidy -header-filter=.* >lint.out 2>lint.err"
                COMMAND sh -c "run-clang-tidy  >lint.out 2>lint.err"
                COMMAND sh -c "grep warning: lint.out || true"
                COMMAND ls -lh ${CMAKE_BINARY_DIR}/lint.out
                VERBATIM
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                COMMENT "Running clang-tidy..."
            )
            
            MESSAGE (STATUS "Enable clang-tidy")

            FILE(WRITE ${PROJECT_BINARY_DIR}/_deps/.clang-tidy "
# Disable all checks in this folder.
Checks: '-*'"
            )
            
        ELSE (CLANG_TIDY_COMMAND)
            MESSAGE (WARNING "clang-tidy not found, static analysis will be disable")
        endif (CLANG_TIDY_COMMAND)
    endif (USE_CLANG_TIDY)


    if (USE_CPPCHECK)
        find_program(CPPCHECK_COMMAND NAMES cppcheck)
        if (CPPCHECK_COMMAND)
            set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
            add_custom_target(cppcheck
                COMMAND sh -c "cppcheck --force --enable=all . -I gvxr/include -I gvxr/include/gVirtualXRay -I SimpleGVXR/include -i ${CMAKE_BINARY_DIR}/_deps -i _skbuild -i third_party -i builds -i bin --disable=missingInclude >${CMAKE_BINARY_DIR}/cppcheck.out 2>${CMAKE_BINARY_DIR}/cppcheck.err"
                VERBATIM
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                COMMENT "Running cppcheck..."
            )
            
            MESSAGE (STATUS "Enable cppcheck")
            
        ELSE (CPPCHECK_COMMAND)
            MESSAGE (WARNING "cppcheck not found, static analysis will be disable")
        endif (CPPCHECK_COMMAND)
    endif (USE_CPPCHECK)

    IF (MSVC AND WIN32)
        find_program(MEMORYCHECK_COMMAND "drmemory.exe")
        IF (MEMORYCHECK_COMMAND)
            SET (MEMORYCHECK_TYPE "DrMemory")
            #set( MEMORYCHECK_COMMAND_OPTIONS "--trace-children=yes --leak-check=full --show-leak-kinds=all --track-origins=yes -s")
            #set( MEMORYCHECK_SUPPRESSIONS_FILE "${PROJECT_SOURCE_DIR}/cmake/valgrind_suppress.txt"
            #        CACHE STRING "File containing suppression rules for the MemoryCheckCommand tool")

            SET (OPTIONAL_TESTS "${OPTIONAL_TESTS} -D NightlyMemCheck")

            MESSAGE (STATUS "Enable DrMemory")

#            /Zi /Ob0 /Oy-
            #/Zi /MT /EHsc /Oy- /Ob0

        ELSE (MEMORYCHECK_COMMAND)
            MESSAGE (STATUS "DrMemory not found, dynamic analysis will be disable")
        ENDIF(MEMORYCHECK_COMMAND)
    ELSE(MSVC AND WIN32)
        find_program(MEMORYCHECK_COMMAND "valgrind")
        IF (MEMORYCHECK_COMMAND)
            set( MEMORYCHECK_COMMAND_OPTIONS "--gen-suppressions=all --trace-children=yes --leak-check=full
            --show-leak-kinds=all --track-origins=yes -s")
            set( MEMORYCHECK_SUPPRESSIONS_FILE "${PROJECT_SOURCE_DIR}/cmake/valgrind_suppress.txt"
                    CACHE STRING "File containing suppression rules for the MemoryCheckCommand tool")

            SET (OPTIONAL_TESTS "${OPTIONAL_TESTS} -D NightlyMemCheck")

            MESSAGE (STATUS "Enable valgirnd")

        ELSE (MEMORYCHECK_COMMAND)
            MESSAGE (WARNING "Valgirnd not found, dynamic analysis will be disable")
        ENDIF(MEMORYCHECK_COMMAND)
    ENDIF (MSVC AND WIN32)
	

    FILE(WRITE ${PROJECT_BINARY_DIR}/run_ctest.sh
"
# Not running on Manylinux
if ! command -v manylinux-interpreters &> /dev/null
then
    echo Not running on Manylinux
    ctest -D NightlyUpdate -D NightlyStart -D NightlyConfigure -D NightlyBuild -D NightlyTest -D NightlyCoverage ${OPTIONAL_TESTS} -D NightlySubmit

# Run on manylinux
else
    echo Run on Manylinux
    ctest -D NightlyStart -D NightlyConfigure -D NightlyBuild -D NightlyTest -D NightlyCoverage ${OPTIONAL_TESTS} -D NightlySubmit
fi
"
        )


    IF (CMAKE_VERSION VERSION_GREATER_EQUAL 3.19)
        FILE(CHMOD ${PROJECT_BINARY_DIR}/run_ctest.sh 
            FILE_PERMISSIONS
                OWNER_READ OWNER_WRITE OWNER_EXECUTE
                GROUP_READ GROUP_EXECUTE
                WORLD_READ WORLD_EXECUTE
        )
    ENDIF ()    
ENDIF (BUILD_TESTING)


# Google Test ##################################################################
OPTION (USE_SYSTEM_GTEST "Use an external version of GoogleTest. " OFF)
MARK_AS_ADVANCED (USE_SYSTEM_GTEST)
IF (BUILD_TESTING)

    # Use system library
    IF (USE_SYSTEM_GTEST)
        FIND_PACKAGE(GTest REQUIRED)
    ENDIF(USE_SYSTEM_GTEST)

    # The system library was not found
    # Build GTest instead
    IF(NOT GTest_FOUND OR NOT USE_SYSTEM_GTEST)
        MESSAGE (STATUS "Build GTest")
        SET (USE_SYSTEM_GTEST OFF)
        INCLUDE(BuildGTest)
        build_googletest( googletest ${CMAKE_INSTALL_PREFIX}/contrib ${PROJECT_BINARY_DIR})

        # # Dependencies (really needed by Ninja, GNU Make and MS VC++ are not so picky)
        # IF (UNIX OR APPLE)
        #     add_custom_target(googletest-libs
        #             BYPRODUCTS "${CMAKE_INSTALL_PREFIX}/contrib/googletest-install/lib/libgtest.a"
        #             COMMAND ""
        #             DEPENDS googletest
        #     )
        # ENDIF (UNIX OR APPLE)

        # IF (WINDOWS)
        #     add_custom_target(googletest-libs
        #             BYPRODUCTS "${CMAKE_INSTALL_PREFIX}/contrib/googletest-install/lib/gtest.lib"
        #             COMMAND ""
        #             DEPENDS googletest
        #     )
        # ENDIF (WINDOWS)

        #ADD_DEPENDENCIES(gVirtualXRay googletest-libs)

        SET (BUILD_GTEST ON)
    ENDIF (NOT GTest_FOUND OR NOT USE_SYSTEM_GTEST)


    IF (GTest_FOUND)
        SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${GTEST_INCLUDE_DIRS}")
        SET (requiredLibs "${requiredLibs}" "${GTEST_LIBRARIES}")
    ENDIF (GTest_FOUND)

ENDIF (BUILD_TESTING)


# GLFW ########################################################################
# GLFW3 is required for either testing, SimpleGVXR or Wrappers
IF (USE_GLFW AND NOT EMSCRIPTEN)
    IF (BUILD_SIMPLEGVXR OR BUILD_WRAPPERS OR BUILD_TESTING)
        # Use system library
        IF (USE_SYSTEM_GLFW)
            FIND_LIBRARY(GLFW3_LIBRARY NAMES glfw3)

            IF (GLFW3_LIBRARY)
                FIND_PACKAGE(glfw3)
            ENDIF (GLFW3_LIBRARY)
        ENDIF (USE_SYSTEM_GLFW)

        # The system library was not found
        # Build GLFW3 instead
        IF (NOT glfw3_FOUND OR NOT USE_SYSTEM_GLFW)
            MESSAGE (STATUS "Build GLFW3")
            #SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})
            SET (USE_SYSTEM_GLFW OFF)
            INCLUDE(cmake/BuildGlfw.cmake)
            build_glfw( glfw
                    ${CMAKE_INSTALL_PREFIX}/contrib  ${PROJECT_BINARY_DIR})

            # Dependencies (really needed by Ninja, GNU Make and MS VC++ are not so picky)
            # IF (UNIX AND NOT APPLE)
            #     add_custom_target(glfw-libs
            #         BYPRODUCTS "${CMAKE_INSTALL_PREFIX}/third_party/lib64/libglfw3.a"
            #         COMMAND ""
            #         DEPENDS glfw
            #     )
            # ENDIF (UNIX AND NOT APPLE)

            # IF (APPLE)
            #     add_custom_target(glfw-libs
            #         BYPRODUCTS "${CMAKE_INSTALL_PREFIX}/third_party/lib64/libglfw3.a"
            #         COMMAND ""
            #         DEPENDS glfw
            #     )
            # ENDIF (APPLE)

            # IF (WINDOWS)
            #     add_custom_target(glfw-libs
            #         BYPRODUCTS "${CMAKE_INSTALL_PREFIX}/third_party/lib/glfw3.lib"
            #         COMMAND ""
            #         DEPENDS glfw
            #     )
            # ENDIF (WINDOWS)

            #ADD_DEPENDENCIES(gvxr glfw-libs)

            SET (BUILD_GLFW ON)
        ENDIF (NOT glfw3_FOUND OR NOT USE_SYSTEM_GLFW)

        include(cmake/configureGlfw.cmake)
    ENDIF ()
ENDIF (USE_GLFW AND NOT EMSCRIPTEN)


ADD_SUBDIRECTORY (gvxr)

# Build SimpleGVXR
IF (BUILD_SIMPLEGVXR OR BUILD_WRAPPERS)
    ADD_SUBDIRECTORY (SimpleGVXR)
ENDIF (BUILD_SIMPLEGVXR OR BUILD_WRAPPERS)

# Build Wrappers
IF (BUILD_WRAPPERS)
    ADD_SUBDIRECTORY (Wrappers)
ENDIF (BUILD_WRAPPERS)

# Packaging support
set(CPACK_PACKAGE_VENDOR "Dr Franck P. Vidal @ Bangor University")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PROJECT_DESCRIPTION})
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENCE.txt")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

set(CPACK_SOURCE_GENERATOR "TGZ;ZIP")
set(CPACK_SOURCE_IGNORE_FILES
    /.git
    /.svn
    /dist
    /.*build.*
    /\\\\.DS_Store
    .swp$
    .*~
)
