#
#
# Copyright (c) 2022, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
# http://www.fpvidal.net/
# All rights reserved.
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
# 3. Neither the name of the Bangor University nor the names of its contributors
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


if (USE_LIBTIFF AND USE_SYSTEM_LIBTIFF)
    find_package(TIFF REQUIRED)
    #find_package(BZip2 REQUIRED)
endif ()

if (USE_assimp AND USE_SYSTEM_ASSIMP)
    find_package(assimp REQUIRED)
endif ()

IF (USE_GLEW)
    IF (USE_SYSTEM_GLEW)
        find_package(GLEW REQUIRED)
        IF (GLEW_USE_STATIC_LIBS)
            SET (requiredLibs GLEW::glew_s ${requiredLibs})
        ELSE (GLEW_USE_STATIC_LIBS)
            SET (requiredLibs GLEW::GLEW ${requiredLibs})
        ENDIF (GLEW_USE_STATIC_LIBS)
    ELSE (USE_SYSTEM_GLEW)
        SET (requiredLibs ${GLEW_LIBRARY} ${requiredLibs})
    ENDIF (USE_SYSTEM_GLEW)
ENDIF (USE_GLEW)

IF (USE_GLAD AND USE_SYSTEM_GLAD)
        FIND_PACKAGE( glad REQUIRED)
ENDIF (USE_GLAD AND USE_SYSTEM_GLAD)

add_executable(Benchmark Benchmark.cxx)
target_link_libraries(Benchmark ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${optionalLibs} ${requiredLibs})

add_executable(DemoBeerLambertLawMonoChromatic DemoBeerLambertLawMonoChromatic.cxx)
target_link_libraries(DemoBeerLambertLawMonoChromatic ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${optionalLibs} ${requiredLibs})

add_executable(DemoDragon DemoDragon.cxx)
target_link_libraries(DemoDragon ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${optionalLibs} ${requiredLibs})

add_executable(DemoConvolutionKernel DemoConvolutionKernel.cxx)
target_link_libraries(DemoConvolutionKernel ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${optionalLibs} ${requiredLibs})

foreach(SGVXR_DEMO_TARGET Benchmark DemoBeerLambertLawMonoChromatic DemoDragon DemoConvolutionKernel)
    target_link_libraries(${SGVXR_DEMO_TARGET}
        "-Wl,--start-group"
        ${gVirtualXRay_LIBRARY} ${gVirtualXRay_LIBRARIES}
        ${optionalLibs} ${requiredLibs} ${ZLIB_LIBRARY}
        "-Wl,--end-group")
endforeach()

# Add dependencies
ADD_DEPENDENCIES(Benchmark   SimpleGVXR)
ADD_DEPENDENCIES(DemoBeerLambertLawMonoChromatic   SimpleGVXR)
ADD_DEPENDENCIES(DemoDragon   SimpleGVXR)
ADD_DEPENDENCIES(DemoConvolutionKernel   SimpleGVXR)

#FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/)
#FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/point_source)
#FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/SBP)
#FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/FBP)
#FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/ART)
#FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/MART)
#FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/SIRT)

# Copy the DLLs
IF (WIN32)
    FILE(COPY ${XRAYLIB_SHARED_LIB_PATH} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    install (FILES ${XRAYLIB_SHARED_LIB_PATH}
        DESTINATION ${INSTALL_DIR}/demos)
ENDIF (WIN32)

# add the install targets
install (TARGETS Benchmark
        DESTINATION ${INSTALL_DIR}/demos)

install (TARGETS DemoBeerLambertLawMonoChromatic
        DESTINATION ${INSTALL_DIR}/demos)

install (TARGETS DemoDragon
        DESTINATION ${INSTALL_DIR}/demos)

install (TARGETS DemoConvolutionKernel
        DESTINATION ${INSTALL_DIR}/demos)

install (FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../Wrappers/welsh-dragon-small.stl
        DESTINATION ${INSTALL_DIR}/demos)

FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../../Wrappers/welsh-dragon-small.stl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
