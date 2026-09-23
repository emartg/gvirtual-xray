/*

Copyright (c) 2025-present, Prof Franck P. Vidal (franck.vidal@stfc.ac.uk),
UK Research and Innovation, All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

3. Neither the name of the UK Research and Innovation nor the names of its contributors
may be used to endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/


/**
********************************************************************************
*
*   @file       TestDetector.cxx
*
*   @brief      Unit testing: test the functions of detector.h.
*
*   @date       01/12/2025
*
*   @author     Prof Franck P. Vidal
*
*   @section    License
*               BSD 3-Clause License.
*
*               For details on use and redistribution please refer
*               to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, Dec 2025-present, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <cmath>
#include <vector>
#include <ranges>
#include <tuple>
// #include <algorithm>

#include "gtest/gtest.h"

#ifndef GVXR_EXCEPTION_H
#include "gVirtualXRay/Exception.h"
#endif

#ifndef SGVXR_SimpleGVXR_h
#include "SimpleGVXR.h"
#endif

// #ifndef GVXR_LOGGER_H
// #include "gVirtualXRay/Logger.h"
// #endif

// #ifndef SGVXR_SimpleGVXR_h
// #include "SimpleGVXR.h"
// #endif
//
#ifndef SGVXR_Detector_h
#include "detector.h"
#endif

#ifndef SGVXR_Utils_h
#include "Utils.h"
#endif

#ifdef HAS_VALGRIND
#include <valgrind/valgrind.h>
#else
#define RUNNING_ON_VALGRIND 0
#endif


//******************************************************************************
//  Global variables
//******************************************************************************


//******************************************************************************
//  Const
//******************************************************************************
const short HU_bone(1330);
const short HU_brain(37);
const short HU_gas(-998);
const short HU_soft_tissue(52);
const short HU_water(0);


//******************************************************************************
//  Function declaration
//******************************************************************************



//-----------------------------------
TEST(TestDetector, SettersAndGetters)
//-----------------------------------
{
    // Expect an exception as the SDD is not known
    EXPECT_THROW(
        setDetectorSize(200, 100, "mm"),
        gVirtualXRay::Exception
    );

    setDetectorNumberOfPixels(400, 200);

    EXPECT_NO_THROW(setDetectorSize(200, 100, "mm"));
    std::vector<double> detector_size = getDetectorSize("mm");
    ASSERT_NEAR(detector_size[0], 200, 0.05);
    ASSERT_NEAR(detector_size[1], 100, 0.05);

    std::vector<double> detector_pixel_pitch = getDetectorPixelSpacing("mm");
    ASSERT_NEAR(detector_pixel_pitch[0], 0.5, 0.05);
    ASSERT_NEAR(detector_pixel_pitch[1], 0.5, 0.05);

    EXPECT_NO_THROW(setDetectorSize(400, 200, "mm"));
    detector_pixel_pitch = getDetectorPixelSpacing("mm");
    ASSERT_NEAR(detector_pixel_pitch[0], 1.0, 0.05);
    ASSERT_NEAR(detector_pixel_pitch[1], 1.0, 0.05);
}


//---------------------------------
TEST(TestDetector, MaxHardwareSize)
//---------------------------------
{
    // Create an OpenGL context
#ifdef HAS_OPENGL
#ifdef HAS_EGL
    createWindow(-1, 0,
        "EGL",
        3, 2,
        0); // Half-precision floating-point numbers is enough
#else // HAS_EGL
#ifdef __APPLE__
 // OpenGL 4.2 and above are not supported on MacOS (at least on my compute with MacOS Big Sur)
    createWindow(-1, 0,
        "OPENGL",
        4, 1,
        0); // Half-precision floating-point numbers is enough
#else // __APPLE__
    createWindow(-1, 0,
        "OPENGL",
        4, 6,
        0); // Half-precision floating-point numbers is enough
#endif // __APPLE__
#endif // HAS_EGL
#else // HAS_OPENGL
    createWindow(-1, 0, "OPENMP");
#endif // HAS_OPENGL

    std::vector<int> largest_possible_number_of_pixels = getLargestPossibleDetectorNumberOfPixels();
    ASSERT_EQ(largest_possible_number_of_pixels[0] > 0, true);
    ASSERT_EQ(largest_possible_number_of_pixels[1] > 0, true);
}


//------------------------------------------------------------------------------------------------------
void compare(const std::vector<float>& aReferenceRow, const std::vector<std::vector<float>>& aTestImage)
//------------------------------------------------------------------------------------------------------
{
    for (const auto& test_row: aTestImage) {

        for (std::tuple<const float&, const float&> elem : std::views::zip(aReferenceRow, test_row)) {
            ASSERT_EQ(std::get<0>(elem), std::get<1>(elem));
        }
    }
}


//------------------------
TEST(TestDetector, Resize)
//------------------------
{
    setDetectorPosition(40.0, 0.0, 0.0, "cm");
    setDetectorUpVector(0, 1, 0);

    setSourcePosition(-80.0, 0.0, 0.0, "cm");
    setMonoChromaticPerPixelAtSDD(80, "keV", 15000);
    useParallelBeam();

    makeCylinder("cyl", 10, 210, 50, "mm");
    setCompound("cyl", "H2O");
    setDensity("cyl", 1, "g/cm3");
    addPolygonMeshAsInnerSurface("cyl");

    ASSERT_NEAR(400, getDetectorNumberOfPixels()[0], 0.05);
    ASSERT_NEAR(200, getDetectorNumberOfPixels()[1], 0.05);

    ASSERT_NEAR(400.0, getDetectorSize("mm")[0], 0.05);
    ASSERT_NEAR(200.0, getDetectorSize("mm")[1], 0.05);

    ASSERT_NEAR(1.0, getDetectorPixelSpacing("mm")[0], 0.05);
    ASSERT_NEAR(1.0, getDetectorPixelSpacing("mm")[1], 0.05);

    const auto xray_image1(computeXRayImage());
    const auto lbuffer1(getLastLBuffer());

    const auto& ref_row_xray_image = xray_image1[0];
    const auto& ref_row_lbuffer = lbuffer1[0];

    compare(ref_row_lbuffer, lbuffer1);
    compare(ref_row_xray_image, xray_image1);

    setDetectorNumberOfPixels(400, 150);
    setDetectorSize(400, 200, "mm");

    ASSERT_NEAR(400, getDetectorNumberOfPixels()[0], 0.05);
    ASSERT_NEAR(150, getDetectorNumberOfPixels()[1], 0.05);

    ASSERT_NEAR(400.0, getDetectorSize("mm")[0], 0.05);
    ASSERT_NEAR(200.0, getDetectorSize("mm")[1], 0.05);

    ASSERT_NEAR(1.0, getDetectorPixelSpacing("mm")[0], 0.05);
    ASSERT_NEAR(200.0 / 150.0, getDetectorPixelSpacing("mm")[1], 0.05);

    const auto xray_image2(computeXRayImage());
    const auto lbuffer2(getLastLBuffer());

    compare(ref_row_lbuffer, lbuffer2);
    compare(ref_row_xray_image, xray_image2);

    setDetectorNumberOfPixels(400, 250);
    setDetectorSize(400, 200, "mm");

    ASSERT_NEAR(400, getDetectorNumberOfPixels()[0], 0.05);
    ASSERT_NEAR(250, getDetectorNumberOfPixels()[1], 0.05);

    ASSERT_NEAR(400.0, getDetectorSize("mm")[0], 0.05);
    ASSERT_NEAR(200.0, getDetectorSize("mm")[1], 0.05);

    ASSERT_NEAR(1.0, getDetectorPixelSpacing("mm")[0], 0.05);
    ASSERT_NEAR(200.0 / 250.0, getDetectorPixelSpacing("mm")[1], 0.05);

    const auto xray_image3(computeXRayImage());
    const auto lbuffer3(getLastLBuffer());

    compare(ref_row_lbuffer, lbuffer3);
    compare(ref_row_xray_image, xray_image3);

    destroy();
}


// //-------------------------------------------------------
// TEST(TestDetector, WhiteImageWithBiasAndGainWithoutNoise)
// //-------------------------------------------------------
// {
//
// }
//
//
// //----------------------------------------------------
// TEST(TestDetector, WhiteImageWithBiasAndGainWithNoise)
// //----------------------------------------------------
// {
//
// }
