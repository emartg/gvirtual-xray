#
#
# Copyright (c) 2019, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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



if(${CMAKE_VERSION} VERSION_GREATER_EQUAL  "3.13.0")
    cmake_policy(SET CMP0075 NEW)
endif()

FIND_PACKAGE( Threads REQUIRED )


if (USE_SYSTEM_GTEST)
    find_package(GTest REQUIRED)
endif ()


if (USE_LIBTIFF AND USE_SYSTEM_LIBTIFF)
    find_package(TIFF REQUIRED)
    #find_package(BZip2 REQUIRED)
endif ()

if (USE_assimp AND USE_SYSTEM_ASSIMP)
    find_package(assimp REQUIRED)
endif ()

IF (USE_GLEW)
    SET (GLEW_USE_STATIC_LIBS ON)
    if (USE_SYSTEM_GLEW)
        find_package(GLEW REQUIRED)
        SET (requiredLibs GLEW::glew_s ${requiredLibs})
    ELSE ()
        SET (requiredLibs ${GLEW_LIBRARY} ${requiredLibs})
    endif ()
ENDIF (USE_GLEW)

IF (USE_GLFW)
    IF (USE_SYSTEM_GLFW)
        FIND_PACKAGE(glfw3 REQUIRED)
    ENDIF (USE_SYSTEM_GLFW)

    SET (requiredLibs ${glfw3_LIBRARIES} ${requiredLibs})
ENDIF (USE_GLFW)


# Enable unit testing
enable_testing()
include_directories(${GTest_INCLUDE_DIRS})
#link_directories(${GTest_LIBS_DIR})

# Functions needed
# does this system provide the log and exp functions?
SET(CMAKE_REQUIRED_LIBRARIES m)

include(CheckIncludeFileCXX)
CHECK_INCLUDE_FILE_CXX("cmath" HAVE_CMATH_H)

include(CheckFunctionExists)
check_function_exists(cos HAVE_COS)
check_function_exists(sin HAVE_SIN)
check_function_exists(log HAVE_LOG)
check_function_exists(exp HAVE_EXP)
check_function_exists(pow HAVE_POW)
check_function_exists(sqrt HAVE_SQRT)
check_function_exists(abs HAVE_ABS)
check_function_exists(cbrt HAVE_CBRT)


# Valiation_02
add_executable(SGVXR_TestDensity                     TestDensity.cxx)

# Validation_01
add_executable(SGVXR_TestMassAttenuationCoefficients TestMassAttenuationCoefficients.cxx)
add_executable(SGVXR_TestScintillator  TestScintillator.cxx)

#add_executable(SGVXR_TestCubeWithIndex    TestCubeWithIndex.cxx)
#add_executable(SGVXR_TestCubeWithoutIndex TestCubeWithoutIndex.cxx)

# Validation_??
#add_executable(SGVXR_TestLBuffer TestLBuffer.cxx)

# Validation_03
add_executable(SGVXR_TestBeerLambertLawMonoChromatic TestBeerLambertLawMonoChromatic.cxx)

# Validation_05
add_executable(SGVXR_TestBeerLambertLawPolyChromatic TestBeerLambertLawPolyChromatic.cxx)

# Validation_08
add_executable(SGVXR_TestSinogramFromMesh            TestSinogramFromMesh.cxx)

# Validation_??
#add_executable(SGVXR_TestRadonTransformFromVoxels   TestRadonTransformFromVoxels.cxx)

# Validation_??
#add_executable(SGVXR_TestSinogramFromVoxels          TestSinogramFromVoxels.cxx)

# Validation_06
#add_executable(SGVXR_TestPointSource                 TestPointSource.cxx)

# Validation_07
add_executable(SGVXR_TestCubeSource                  TestCubeSource.cxx)

# Validation_09
#add_executable(SGVXR_TestUncenteredSource            TestUncenteredSource.cxx)

# Validation_??
#add_executable(SGVXR_TestPhantomMonoChromatic        TestPhantomMonoChromatic.cxx)

# Validation_04
#add_executable(SGVXR_TestRadonTransformFromMesh       TestRadonTransformFromMesh.cxx)

#add_executable(SGVXR_TestOrtho                        TestOrtho.cxx)

IF (FFTW3_LIBRARIES)
    # Validation_04
    add_executable(SGVXR_TestFBP                     TestFBP.cxx)
ENDIF()

add_executable(SGVXR_TestSpectrum       TestSpectrum.cxx)

IF (Python3_Development.Embed_FOUND)
    add_executable(SGVXR_TestTubeVoltage       TestTubeVoltage.cxx)
ENDIF()

add_executable(SGVXR_TestFocalSpot       TestFocalSpot.cxx)
add_executable(SGVXR_TestMemoryLeak       TestMemoryLeak.cxx)

FIND_FILE (VALGRIND_HEADER_FILE "valgrind/valgrind.h")

IF (VALGRIND_HEADER_FILE)
    target_compile_definitions(SGVXR_TestMemoryLeak PRIVATE HAS_VALGRIND)
ENDIF ()

add_executable(SGVXR_TestVulkan       TestVulkan.cxx)
add_executable(SGVXR_TestUtils       TestUtils.cxx)

add_executable(SGVXR_TestAcquisitionFile       TestAcquisitionFile.cxx)
add_executable(SGVXR_TestSampleFile       TestSampleFile.cxx)

add_executable(SGVXR_TestBeamFiltration       TestBeamFiltration.cxx)
add_executable(SGVXR_TestIsoSurface       TestIsoSurface.cxx)
add_executable(SGVXR_TestGain       TestGain.cxx)
add_executable(SGVXR_TestClipping       TestClipping.cxx)

add_executable(SGVXR_TestFluxConversion       TestFluxConversion.cxx)

add_executable(SGVXR_TestDetector       TestDetector.cxx)


list(APPEND GTest_LIBRARIES gtest gtest_main)
separate_arguments(GTest_LIBRARIES)

# MESSAGE(FATAL_ERROR "${SimpleGVXR_LIBRARIES}" )#${optionalLibs} ${requiredLibs} ${GTest_LIBRARIES} Threads::Threads)
target_link_libraries(SGVXR_TestDensity                     ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${optionalLibs} ${requiredLibs} ${GTest_LIBRARIES} Threads::Threads)
target_link_libraries(SGVXR_TestMassAttenuationCoefficients ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${optionalLibs} ${requiredLibs} ${GTest_LIBRARIES} Threads::Threads)
target_link_libraries(SGVXR_TestScintillator ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${optionalLibs} ${requiredLibs} ${GTest_LIBRARIES} Threads::Threads)

#target_link_libraries(SGVXR_TestCubeWithoutIndex            ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${optionalLibs} ${requiredLibs} ${GTest_LIBRARIES} Threads::Threads)
#target_link_libraries(SGVXR_TestCubeWithIndex               ${gVirtualXRay_LIBRARY} ${gVirtualXRay_LIBRARIES} ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${optionalLibs} ${requiredLibs} ${GTest_LIBRARIES} Threads::Threads)
#target_link_libraries(SGVXR_TestLBuffer                     ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${optionalLibs} ${requiredLibs} ${GTest_LIBRARIES} Threads::Threads)
target_link_libraries(SGVXR_TestBeerLambertLawMonoChromatic ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${optionalLibs} ${requiredLibs} ${GTest_LIBRARIES} Threads::Threads)
target_link_libraries(SGVXR_TestBeerLambertLawPolyChromatic ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${optionalLibs} ${requiredLibs} ${GTest_LIBRARIES} Threads::Threads)
target_link_libraries(SGVXR_TestSinogramFromMesh            ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${optionalLibs} ${requiredLibs} ${GTest_LIBRARIES} Threads::Threads)
#target_link_libraries(SGVXR_TestSinogramFromVoxels          ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${optionalLibs} ${requiredLibs} ${GTest_LIBRARIES} Threads::Threads)
#target_link_libraries(SGVXR_TestPointSource                 ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${optionalLibs} ${requiredLibs} ${GTest_LIBRARIES} Threads::Threads)
target_link_libraries(SGVXR_TestCubeSource                  ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${optionalLibs} ${requiredLibs} ${GTest_LIBRARIES} Threads::Threads)
#target_link_libraries(SGVXR_TestUncenteredSource            ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${optionalLibs} ${requiredLibs} ${GTest_LIBRARIES} Threads::Threads)
#target_link_libraries(SGVXR_TestPhantomMonoChromatic        ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${optionalLibs} ${requiredLibs} ${GTest_LIBRARIES} Threads::Threads)
#target_link_libraries(SGVXR_TestRadonTransformFromMesh      ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${optionalLibs} ${requiredLibs} ${GTest_LIBRARIES} Threads::Threads)
#target_link_libraries(SGVXR_TestOrtho                      ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${optionalLibs} ${requiredLibs} ${GTest_LIBRARIES} Threads::Threads)

IF (FFTW3_LIBRARIES)
    target_link_libraries(SGVXR_TestFBP                     ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${optionalLibs} ${requiredLibs} ${GTest_LIBRARIES} Threads::Threads)
ENDIF()

target_link_libraries(SGVXR_TestSpectrum                      ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${optionalLibs} ${requiredLibs} ${GTest_LIBRARIES} Threads::Threads)
target_link_libraries(SGVXR_TestFocalSpot ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${optionalLibs} ${requiredLibs} ${GTest_LIBRARIES} Threads::Threads)

IF (Python3_Development.Embed_FOUND)
    target_link_libraries(SGVXR_TestTubeVoltage                      ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${optionalLibs} ${requiredLibs} ${GTest_LIBRARIES} Threads::Threads)
ENDIF()

target_link_libraries(SGVXR_TestMemoryLeak                      ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${optionalLibs} ${requiredLibs} ${GTest_LIBRARIES} Threads::Threads)
target_link_libraries(SGVXR_TestVulkan                      ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${optionalLibs} ${requiredLibs} ${GTest_LIBRARIES} Threads::Threads)
target_link_libraries(SGVXR_TestUtils                      ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${optionalLibs} ${requiredLibs} ${GTest_LIBRARIES} Threads::Threads)
target_link_libraries(SGVXR_TestAcquisitionFile                      ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${optionalLibs} ${requiredLibs} ${GTest_LIBRARIES} Threads::Threads)
target_link_libraries(SGVXR_TestSampleFile                      ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${optionalLibs} ${requiredLibs} ${GTest_LIBRARIES} Threads::Threads)

target_link_libraries(SGVXR_TestBeamFiltration                      ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${optionalLibs} ${requiredLibs} ${GTest_LIBRARIES} Threads::Threads)
target_link_libraries(SGVXR_TestIsoSurface                      ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${optionalLibs} ${requiredLibs} ${GTest_LIBRARIES} Threads::Threads)
target_link_libraries(SGVXR_TestGain                      ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES} ${optionalLibs}
        ${requiredLibs} ${GTest_LIBRARIES} Threads::Threads)
target_link_libraries(SGVXR_TestClipping                      ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES}
        ${optionalLibs}
        ${requiredLibs} ${GTest_LIBRARIES} Threads::Threads)
target_link_libraries(SGVXR_TestFluxConversion                      ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES}
        ${optionalLibs}
        ${requiredLibs} ${GTest_LIBRARIES} Threads::Threads)

target_link_libraries(SGVXR_TestDetector                      ${SimpleGVXR_LIBRARY} ${SimpleGVXR_LIBRARIES}
        ${optionalLibs}
        ${requiredLibs} ${GTest_LIBRARIES} Threads::Threads)

# Add dependencies
ADD_DEPENDENCIES(SGVXR_TestDensity   SimpleGVXR)
ADD_DEPENDENCIES(SGVXR_TestMassAttenuationCoefficients   SimpleGVXR)
ADD_DEPENDENCIES(SGVXR_TestScintillator                  SimpleGVXR)

#ADD_DEPENDENCIES(SGVXR_TestCubeWithoutIndex              SimpleGVXR)
#ADD_DEPENDENCIES(SGVXR_TestCubeWithIndex                 SimpleGVXR)
#ADD_DEPENDENCIES(SGVXR_TestLBuffer                       SimpleGVXR)
ADD_DEPENDENCIES(SGVXR_TestBeerLambertLawMonoChromatic   SimpleGVXR)
ADD_DEPENDENCIES(SGVXR_TestBeerLambertLawPolyChromatic   SimpleGVXR)
ADD_DEPENDENCIES(SGVXR_TestSinogramFromMesh              SimpleGVXR)
#ADD_DEPENDENCIES(SGVXR_TestSinogramFromVoxels            SimpleGVXR)
#ADD_DEPENDENCIES(SGVXR_TestPointSource                   SimpleGVXR)
ADD_DEPENDENCIES(SGVXR_TestCubeSource                    SimpleGVXR)
#ADD_DEPENDENCIES(SGVXR_TestUncenteredSource              SimpleGVXR)
#ADD_DEPENDENCIES(SGVXR_TestPhantomMonoChromatic          SimpleGVXR)
#ADD_DEPENDENCIES(SGVXR_TestRadonTransformFromMesh        SimpleGVXR)
#ADD_DEPENDENCIES(SGVXR_TestOrtho                         SimpleGVXR)

IF (FFTW3_LIBRARIES)
 ADD_DEPENDENCIES(SGVXR_TestFBP                           SimpleGVXR)
ENDIF()

ADD_DEPENDENCIES(SGVXR_TestSpectrum                         SimpleGVXR)
ADD_DEPENDENCIES(SGVXR_TestFocalSpot SimpleGVXR)

IF (Python3_Development.Embed_FOUND)
ADD_DEPENDENCIES(SGVXR_TestTubeVoltage                         SimpleGVXR)
ENDIF()

ADD_DEPENDENCIES(SGVXR_TestMemoryLeak SimpleGVXR)
ADD_DEPENDENCIES(SGVXR_TestVulkan SimpleGVXR)
ADD_DEPENDENCIES(SGVXR_TestUtils SimpleGVXR)
ADD_DEPENDENCIES(SGVXR_TestAcquisitionFile SimpleGVXR)
ADD_DEPENDENCIES(SGVXR_TestSampleFile SimpleGVXR)
ADD_DEPENDENCIES(SGVXR_TestBeamFiltration SimpleGVXR)
ADD_DEPENDENCIES(SGVXR_TestIsoSurface SimpleGVXR)
ADD_DEPENDENCIES(SGVXR_TestGain SimpleGVXR)
ADD_DEPENDENCIES(SGVXR_TestClipping SimpleGVXR)
ADD_DEPENDENCIES(SGVXR_TestFluxConversion SimpleGVXR)
ADD_DEPENDENCIES(SGVXR_TestDetector SimpleGVXR)

IF (NOT USE_SYSTEM_GTEST)
    ADD_DEPENDENCIES(SGVXR_TestDensity   googletest)
    ADD_DEPENDENCIES(SGVXR_TestMassAttenuationCoefficients   googletest)
    ADD_DEPENDENCIES(SGVXR_TestScintillator                  googletest)

    #ADD_DEPENDENCIES(SGVXR_TestCubeWithoutIndex              googletest)
    #ADD_DEPENDENCIES(SGVXR_TestCubeWithIndex                 googletest)
    #ADD_DEPENDENCIES(SGVXR_TestLBuffer                       googletest)
    ADD_DEPENDENCIES(SGVXR_TestBeerLambertLawMonoChromatic   googletest)
    ADD_DEPENDENCIES(SGVXR_TestBeerLambertLawPolyChromatic   googletest)
    ADD_DEPENDENCIES(SGVXR_TestSinogramFromMesh              googletest)
    #ADD_DEPENDENCIES(SGVXR_TestSinogramFromVoxels            googletest)
    #ADD_DEPENDENCIES(SGVXR_TestPointSource                   googletest)
    ADD_DEPENDENCIES(SGVXR_TestCubeSource                    googletest)
    #ADD_DEPENDENCIES(SGVXR_TestUncenteredSource              googletest)
    #ADD_DEPENDENCIES(SGVXR_TestPhantomMonoChromatic          googletest)
    #ADD_DEPENDENCIES(SGVXR_TestRadonTransformFromMesh        googletest)
    #ADD_DEPENDENCIES(SGVXR_TestOrtho                         googletest)

    IF (FFTW3_LIBRARIES)
        ADD_DEPENDENCIES(SGVXR_TestFBP                           googletest)
    ENDIF()

    ADD_DEPENDENCIES(SGVXR_TestSpectrum                         googletest)
    ADD_DEPENDENCIES(SGVXR_TestFocalSpot googletest)

    IF (Python3_Development.Embed_FOUND)
        ADD_DEPENDENCIES(SGVXR_TestTubeVoltage                         googletest)
    ENDIF()

    ADD_DEPENDENCIES(SGVXR_TestMemoryLeak googletest)
    ADD_DEPENDENCIES(SGVXR_TestVulkan googletest)
    ADD_DEPENDENCIES(SGVXR_TestUtils googletest)
    ADD_DEPENDENCIES(SGVXR_TestAcquisitionFile googletest)
    ADD_DEPENDENCIES(SGVXR_TestSampleFile googletest)
    ADD_DEPENDENCIES(SGVXR_TestBeamFiltration googletest)
    ADD_DEPENDENCIES(SGVXR_TestIsoSurface googletest)
    ADD_DEPENDENCIES(SGVXR_TestGain googletest)
    ADD_DEPENDENCIES(SGVXR_TestClipping googletest)
    ADD_DEPENDENCIES(SGVXR_TestFluxConversion googletest)
    ADD_DEPENDENCIES(SGVXR_TestDetector googletest)
ENDIF (NOT USE_SYSTEM_GTEST)

add_test(SGVXR_TestDensity SGVXR_TestDensity)
add_test(SGVXR_TestMassAttenuationCoefficients SGVXR_TestMassAttenuationCoefficients)
add_test(SGVXR_TestScintillator SGVXR_TestScintillator)

#add_test(SGVXR_TestCubeWithoutIndex            SGVXR_TestCubeWithoutIndex)
#add_test(SGVXR_TestCubeWithIndex               SGVXR_TestCubeWithIndex)
#add_test(SGVXR_TestLBuffer                     SGVXR_TestLBuffer)
add_test(SGVXR_TestBeerLambertLawMonoChromatic SGVXR_TestBeerLambertLawMonoChromatic)
add_test(SGVXR_TestBeerLambertLawPolyChromatic SGVXR_TestBeerLambertLawPolyChromatic)
add_test(SGVXR_TestSinogramFromMesh            SGVXR_TestSinogramFromMesh)
##add_test(SGVXR_TestSinogramFromVoxels          SGVXR_TestSinogramFromVoxels)
#add_test(SGVXR_TestPointSource                 SGVXR_TestPointSource)
#add_test(SGVXR_TestCubeSource                  SGVXR_TestCubeSource)
#add_test(SGVXR_TestUncenteredSource            SGVXR_TestUncenteredSource)
##add_test(SGVXR_TestPhantomMonoChromatic        SGVXR_TestPhantomMonoChromatic)
##add_test(SGVXR_TestRadonTransformFromMesh      SGVXR_TestRadonTransformFromMesh)
#add_test(SGVXR_TestOrtho                       SGVXR_TestOrtho)

#IF (FFTW3_LIBRARIES)
#    add_test(SGVXR_TestFBP                     SGVXR_TestFBP)
#ENDIF()

add_test(SGVXR_TestSpectrum                       SGVXR_TestSpectrum)
add_test(SGVXR_TestFocalSpot  SGVXR_TestFocalSpot)

IF (Python3_Development.Embed_FOUND)
    add_test (SGVXR_TestTubeVoltage                       SGVXR_TestTubeVoltage)
ENDIF()

#add_test(SGVXR_TestMemoryLeak  SGVXR_TestMemoryLeak) # Do not add the test as the method is known to be unreliable.
add_test(SGVXR_TestVulkan  SGVXR_TestVulkan)
add_test(SGVXR_TestUtils  SGVXR_TestUtils)
add_test(SGVXR_TestAcquisitionFile  SGVXR_TestAcquisitionFile)
add_test(SGVXR_TestSampleFile  SGVXR_TestSampleFile)
add_test(SGVXR_TestBeamFiltration  SGVXR_TestBeamFiltration)
add_test(SGVXR_TestIsoSurface  SGVXR_TestIsoSurface)
add_test(SGVXR_TestGain  SGVXR_TestGain)
add_test(SGVXR_TestClipping  SGVXR_TestClipping)
add_test(SGVXR_TestFluxConversion  SGVXR_TestFluxConversion)
add_test(SGVXR_TestDetector  SGVXR_TestDetector)


FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/)
FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/point_source)
FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/SBP)
FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/FBP)
FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/ART)
FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/MART)
FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/SIRT)

CONFIGURE_FILE (data/hydrogen.tsv   data/hydrogen.tsv   COPYONLY)
CONFIGURE_FILE (data/carbon.tsv     data/carbon.tsv   COPYONLY)
CONFIGURE_FILE (data/nitrogen.tsv   data/nitrogen.tsv   COPYONLY)
CONFIGURE_FILE (data/oxygen.tsv     data/oxygen.tsv   COPYONLY)
CONFIGURE_FILE (data/sodium.tsv	data/sodium.tsv		COPYONLY)
CONFIGURE_FILE (data/magnesium.tsv	data/magnesium.tsv	COPYONLY)
CONFIGURE_FILE (data/phosphor.tsv	data/phosphor.tsv	COPYONLY)
CONFIGURE_FILE (data/sulfur.tsv	data/sulfur.tsv	COPYONLY)
CONFIGURE_FILE (data/clorine.tsv	data/clorine.tsv	COPYONLY)
CONFIGURE_FILE (data/argon.tsv	data/argon.tsv	COPYONLY)
CONFIGURE_FILE (data/potassium.tsv	data/potassium.tsv	COPYONLY)
CONFIGURE_FILE (data/calcium.tsv	data/calcium.tsv	COPYONLY)
CONFIGURE_FILE (data/titanium.tsv	data/titanium.tsv	COPYONLY)
CONFIGURE_FILE (data/copper.tsv	data/copper.tsv	COPYONLY)
CONFIGURE_FILE (data/zinc.tsv	data/zinc.tsv	COPYONLY)
CONFIGURE_FILE (data/silver.tsv	data/silver.tsv	COPYONLY)
CONFIGURE_FILE (data/tin.tsv	data/tin.tsv	COPYONLY)
CONFIGURE_FILE (data/iron.tsv	data/iron.tsv	COPYONLY)
CONFIGURE_FILE (data/iodine.tsv	data/iodine.tsv	COPYONLY)
CONFIGURE_FILE (data/Ti90Al6V4.tsv	data/Ti90Al6V4.tsv	COPYONLY)
CONFIGURE_FILE (data/H2O.tsv	data/H2O.tsv	COPYONLY)
CONFIGURE_FILE (data/SiC.tsv	data/SiC.tsv	COPYONLY)
CONFIGURE_FILE (runGnuplot.sh    runGnuplot.sh		COPYONLY)

CONFIGURE_FILE (data/bone.csv   data/bone.tsv   COPYONLY)
CONFIGURE_FILE (data/brain.csv  data/brain.tsv  COPYONLY)
CONFIGURE_FILE (data/gas.csv    data/gas.tsv    COPYONLY)
CONFIGURE_FILE (data/tissue.csv data/tissue.tsv COPYONLY)
CONFIGURE_FILE (data/water.csv  data/water.tsv  COPYONLY)
CONFIGURE_FILE (data/phantom_sinogram.mhd      data/phantom_sinogram.mhd  COPYONLY)
CONFIGURE_FILE (data/phantom_sinogram.raw      data/phantom_sinogram.raw  COPYONLY)
CONFIGURE_FILE (data/phantom_projections.mhd   data/phantom_projections.mhd  COPYONLY)
CONFIGURE_FILE (data/phantom_projections.raw   data/phantom_projections.raw  COPYONLY)
CONFIGURE_FILE (data/point_source/gate_simulation.mhd       data/point_source/gate_simulation.mhd  COPYONLY)
CONFIGURE_FILE (data/point_source/gate_simulation.raw       data/point_source/gate_simulation.raw  COPYONLY)
CONFIGURE_FILE (data/cube_source/gate_simulation.mhd       data/cube_source/gate_simulation.mhd  COPYONLY)
CONFIGURE_FILE (data/cube_source/gate_simulation.raw       data/cube_source/gate_simulation.raw  COPYONLY)
CONFIGURE_FILE (data/uncentered_source/gate_simulation.mhd       data/uncentered_source/gate_simulation.mhd  COPYONLY)
CONFIGURE_FILE (data/uncentered_source/gate_simulation.raw       data/uncentered_source/gate_simulation.raw  COPYONLY)

CONFIGURE_FILE (data/CT_reconstruction/ct_volume_in_mu_80keV.mhd   data/CT_reconstruction/ct_volume_in_mu_80keV.mhd  COPYONLY)
CONFIGURE_FILE (data/CT_reconstruction/ct_volume_in_mu_80keV.raw   data/CT_reconstruction/ct_volume_in_mu_80keV.raw  COPYONLY)
CONFIGURE_FILE (data/CT_reconstruction/projection_set.mhd          data/CT_reconstruction/projection_set.mhd         COPYONLY)
CONFIGURE_FILE (data/CT_reconstruction/projection_set.raw          data/CT_reconstruction/projection_set.raw         COPYONLY)
CONFIGURE_FILE (data/CT_reconstruction/sinogram.mhd                data/CT_reconstruction/sinogram.mhd               COPYONLY)
CONFIGURE_FILE (data/CT_reconstruction/sinogram.raw                data/CT_reconstruction/sinogram.raw               COPYONLY)

CONFIGURE_FILE (data/55kV.tsv                data/55kV.tsv               COPYONLY)

CONFIGURE_FILE (data/spekpy_filtered.dat      data/spekpy_filtered.dat      COPYONLY)
CONFIGURE_FILE (data/spekpy_unfiltered.dat    data/spekpy_unfiltered.dat    COPYONLY)
CONFIGURE_FILE (data/xpecgen_filtered.dat     data/xpecgen_filtered.dat     COPYONLY)
CONFIGURE_FILE (data/xpecgen_unfiltered.dat   data/xpecgen_unfiltered.dat   COPYONLY)


# add the install targets
install (DIRECTORY  ${CMAKE_CURRENT_BINARY_DIR}/data             DESTINATION ${INSTALL_DIR}/test)

install (TARGETS SGVXR_TestDensity          DESTINATION ${INSTALL_DIR}/test)
install (TARGETS SGVXR_TestMassAttenuationCoefficients          DESTINATION ${INSTALL_DIR}/test)
install (TARGETS SGVXR_TestScintillator          DESTINATION ${INSTALL_DIR}/test)

#install (TARGETS SGVXR_TestCubeWithoutIndex             DESTINATION ${INSTALL_DIR}/test)
#install (TARGETS SGVXR_TestCubeWithIndex             DESTINATION ${INSTALL_DIR}/test)
#install (TARGETS SGVXR_TestLBuffer             DESTINATION ${INSTALL_DIR}/test)
install (TARGETS SGVXR_TestBeerLambertLawMonoChromatic             DESTINATION ${INSTALL_DIR}/test)
install (TARGETS SGVXR_TestBeerLambertLawPolyChromatic             DESTINATION ${INSTALL_DIR}/test)
install (TARGETS SGVXR_TestSinogramFromMesh             DESTINATION ${INSTALL_DIR}/test)
#install (TARGETS SGVXR_TestSinogramFromVoxels             DESTINATION ${INSTALL_DIR}/test)
#install (TARGETS SGVXR_TestPointSource             DESTINATION ${INSTALL_DIR}/test)
install (TARGETS SGVXR_TestCubeSource             DESTINATION ${INSTALL_DIR}/test)
#install (TARGETS SGVXR_TestUncenteredSource             DESTINATION ${INSTALL_DIR}/test)
#install (TARGETS SGVXR_TestPhantomMonoChromatic             DESTINATION ${INSTALL_DIR}/test)
#install (TARGETS SGVXR_TestRadonTransformFromMesh            DESTINATION ${INSTALL_DIR}/test)
#install (TARGETS SGVXR_TestOrtho             DESTINATION ${INSTALL_DIR}/test)

IF (FFTW3_LIBRARIES)
    install (TARGETS SGVXR_TestFBP             DESTINATION ${INSTALL_DIR}/test)
ENDIF()

install (TARGETS SGVXR_TestSpectrum             DESTINATION ${INSTALL_DIR}/test)
install (TARGETS SGVXR_TestFocalSpot DESTINATION ${INSTALL_DIR}/test)

IF (Python3_Development.Embed_FOUND)
    install (TARGETS SGVXR_TestTubeVoltage            DESTINATION ${INSTALL_DIR}/test)
ENDIF()

install (TARGETS SGVXR_TestDetector             DESTINATION ${INSTALL_DIR}/test)
