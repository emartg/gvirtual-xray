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

3. Neither the name of the Bangor University nor the names of its contributors
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
*   @file       TestFluxConversion.cxx
*
*   @brief      Unit testing: Test if the flux conversion function works.
*
*   @date       14/11/2025
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
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, Nov 2025-present, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <cmath>

#include "gtest/gtest.h"
#include "gVirtualXRay/XRayRenderer.h"

#ifndef SGVXR_Acquisition_h
#include "acquisition.h"
#endif

// #ifndef SGVXR_Source_h
// #include "gVirtualXRay/OpenGL2VBO.inl"
// #endif

#ifndef GVXR_IMAGE_H
#include "gVirtualXRay/Image.h"
#endif

#ifndef SGVXR_Source_h
#include "source.h"
#endif

#ifndef SGVXR_Detector_h
#include "detector.h"
#endif

#ifndef SGVXR_Utils_h
#include "Utils.h"
#endif


//******************************************************************************
//  Global variables
//******************************************************************************
int g_main_window_width(1280 / 2.0);
int g_main_window_height(800 / 2.0);

extern gVirtualXRay::XRayRenderer g_xray_renderer;


//******************************************************************************
//  Function declaration
//******************************************************************************
void compareImages(const gVirtualXRay::Image<float>& anImage0,
    const gVirtualXRay::Image<float>& anImage1,
    unsigned int aLineNumber);


//-------------------------------------------
TEST(TestFluxConversion, SDD1m_PixelArea1cm2)
//-------------------------------------------
{
    // SDD: 1m
    // pixel area: 1cm^2
    // Note: all the values should be the same
    double pixel_area = 1.0;
    setSourcePosition(0.5, 0, 0, "m");
    setDetectorPosition(-0.5, 0, 0, "m");
    setDetectorUpVector(0, 1, 0);
    setDetectorPixelSize(sqrt(pixel_area), sqrt(pixel_area), "cm");
    setDetectorNumberOfPixels(1, 1);

    int number_of_photons_per_pixel_at_SDD;
    int number_of_photons_per_cm2_at_1m;

    // Test monochromatic case
    // Set as per pixel at SDD
    number_of_photons_per_pixel_at_SDD = 10000;
    setMonoChromaticPerPixelAtSDD(14, "keV", number_of_photons_per_pixel_at_SDD);
    ASSERT_EQ(number_of_photons_per_pixel_at_SDD, getNumberOfPhotonsPerPixelAtSDD());

    number_of_photons_per_cm2_at_1m = getNumberOfPhotonsPerCm2At1m();
    ASSERT_EQ(number_of_photons_per_pixel_at_SDD, number_of_photons_per_cm2_at_1m);

    // Test monochromatic case
    // Set as per cm2 at 1m
    number_of_photons_per_cm2_at_1m = 10000;
    setMonoChromaticPerCm2At1m(14, "keV", number_of_photons_per_cm2_at_1m);
    ASSERT_EQ(number_of_photons_per_cm2_at_1m, getNumberOfPhotonsPerCm2At1m());

    number_of_photons_per_pixel_at_SDD = getNumberOfPhotonsPerPixelAtSDD();
    ASSERT_EQ(number_of_photons_per_pixel_at_SDD, number_of_photons_per_cm2_at_1m);

    // Polychromatic
    setVoltage(100, "kV");

    // Set as per pixel at SDD
    number_of_photons_per_pixel_at_SDD = 10000;
    setNumberOfPhotonsPerPixelAtSDD(number_of_photons_per_pixel_at_SDD);
    ASSERT_EQ(number_of_photons_per_pixel_at_SDD, getNumberOfPhotonsPerPixelAtSDD());

    number_of_photons_per_cm2_at_1m = getNumberOfPhotonsPerCm2At1m();
    ASSERT_EQ(number_of_photons_per_pixel_at_SDD, number_of_photons_per_cm2_at_1m);

    // Set as per cm2 at 1m
    number_of_photons_per_cm2_at_1m = 10000;
    setNumberOfPhotonsPerCm2At1m(number_of_photons_per_cm2_at_1m);
    ASSERT_EQ(number_of_photons_per_cm2_at_1m, getNumberOfPhotonsPerCm2At1m());

    number_of_photons_per_pixel_at_SDD = getNumberOfPhotonsPerPixelAtSDD();
    ASSERT_EQ(number_of_photons_per_pixel_at_SDD, number_of_photons_per_cm2_at_1m);
}


//-------------------------------------------
TEST(TestFluxConversion, SDD1m_PixelArea3cm2)
//-------------------------------------------
{
    // SDD: 1m
    // pixel area: 3cm^2
    // Note: the values per pixel at SDD should be 3x larger than per 1cm2 at 1m
    double pixel_area = 3.0;
    setSourcePosition(0.5, 0, 0, "m");
    setDetectorPosition(-0.5, 0, 0, "m");
    setDetectorUpVector(0, 1, 0);
    setDetectorPixelSize(sqrt(pixel_area), sqrt(pixel_area), "cm");
    setDetectorNumberOfPixels(1, 1);

    int number_of_photons_per_pixel_at_SDD;
    int number_of_photons_per_cm2_at_1m;
    float max_error_in_percent = 0.5 / 100.0;

    // Test monochromatic case
    // Set as per pixel at SDD
    number_of_photons_per_pixel_at_SDD = 10000;
    setMonoChromaticPerPixelAtSDD(14, "keV", number_of_photons_per_pixel_at_SDD);
    ASSERT_NEAR(number_of_photons_per_pixel_at_SDD, getNumberOfPhotonsPerPixelAtSDD(), 2);

    number_of_photons_per_cm2_at_1m = getNumberOfPhotonsPerCm2At1m();
    ASSERT_LT(abs(number_of_photons_per_pixel_at_SDD - round(pixel_area * number_of_photons_per_cm2_at_1m)) / number_of_photons_per_pixel_at_SDD, max_error_in_percent);

    // Test monochromatic case
    // Set as per cm2 at 1m
    number_of_photons_per_cm2_at_1m = 10000;
    setMonoChromaticPerCm2At1m(14, "keV", number_of_photons_per_cm2_at_1m);
    ASSERT_NEAR(number_of_photons_per_cm2_at_1m, getNumberOfPhotonsPerCm2At1m(), 2);

    number_of_photons_per_pixel_at_SDD = getNumberOfPhotonsPerPixelAtSDD();
    ASSERT_LT(abs(round(number_of_photons_per_pixel_at_SDD / pixel_area) - number_of_photons_per_cm2_at_1m) / number_of_photons_per_cm2_at_1m, max_error_in_percent);

    // Polychromatic
    setVoltage(100, "kV");

    // Set as per pixel at SDD
    number_of_photons_per_pixel_at_SDD = 10000;
    setNumberOfPhotonsPerPixelAtSDD(number_of_photons_per_pixel_at_SDD);
    ASSERT_NEAR(number_of_photons_per_pixel_at_SDD, getNumberOfPhotonsPerPixelAtSDD(), 2);

    number_of_photons_per_cm2_at_1m = getNumberOfPhotonsPerCm2At1m();
    ASSERT_NEAR(number_of_photons_per_pixel_at_SDD, round(pixel_area * number_of_photons_per_cm2_at_1m), 2);
    ASSERT_LT(abs(number_of_photons_per_pixel_at_SDD - round(pixel_area * number_of_photons_per_cm2_at_1m)) / number_of_photons_per_pixel_at_SDD, max_error_in_percent);

    // Set as per cm2 at 1m
    number_of_photons_per_cm2_at_1m = 10000;
    setNumberOfPhotonsPerCm2At1m(number_of_photons_per_cm2_at_1m);
    ASSERT_NEAR(number_of_photons_per_cm2_at_1m, getNumberOfPhotonsPerCm2At1m(), 2);

    number_of_photons_per_pixel_at_SDD = getNumberOfPhotonsPerPixelAtSDD();
    ASSERT_LT(abs(round(number_of_photons_per_pixel_at_SDD / pixel_area) - number_of_photons_per_cm2_at_1m) / number_of_photons_per_cm2_at_1m, max_error_in_percent);
}


//-----------------------------------------------------------------------------------
float inverseSquareLaw(float anOldIntensity, float anOldDistance, float aNewDistance)
//-----------------------------------------------------------------------------------
{
    // We know:
    // anOldIntensity * anOldDistance^2 = aNewIntensity * aNewDistance^2;

    // So
    // aNewIntensity = anOldIntensity * anOldDistance / aNewDistance

    return anOldIntensity * (anOldDistance * anOldDistance) / (aNewDistance * aNewDistance);
}


//------------------------------------------------------
TEST(TestFluxConversion, SDD0_861m_PixelArea0_000225cm2)
//------------------------------------------------------
{
    // SDD: 1m
    // pixel area: 0.015x0.015cm^2
    // Note: 249516.69 per cm2 @ 100cm
    //       5048.76 per pixel @ 86.1cm
    double pixel_area = 0.0150 * 0.0150;
    double SDD_in_cm = 86.1;
    setSourcePosition(SDD_in_cm / 2.0, 0, 0, "cm");
    setDetectorPosition(-SDD_in_cm / 2.0, 0, 0, "cm");
    setDetectorUpVector(0, 1, 0);
    setDetectorPixelSize(150, 150, "um");
    setDetectorNumberOfPixels(1, 1);

    int number_of_photons_per_pixel_at_SDD;
    int number_of_photons_per_cm2_at_1m;
    float max_error_in_percent = 0.5 / 100.0;
    int expected_result;

    // Test monochromatic case
    // Set as per cm2 at 1m
    number_of_photons_per_cm2_at_1m = 249516.69;
    setMonoChromaticPerCm2At1m(14, "keV", number_of_photons_per_cm2_at_1m);
    ASSERT_NEAR(number_of_photons_per_cm2_at_1m, getNumberOfPhotonsPerCm2At1m(), 2);

    number_of_photons_per_pixel_at_SDD = getNumberOfPhotonsPerPixelAtSDD();
    expected_result = inverseSquareLaw(number_of_photons_per_cm2_at_1m * pixel_area, 100, SDD_in_cm);
    ASSERT_LT(abs(number_of_photons_per_pixel_at_SDD - expected_result) / expected_result, max_error_in_percent);

    // Test monochromatic case
    // Set as per pixel at SDD
    number_of_photons_per_pixel_at_SDD = 75;
    setMonoChromaticPerPixelAtSDD(14, "keV", number_of_photons_per_pixel_at_SDD);
    ASSERT_NEAR(number_of_photons_per_pixel_at_SDD, getNumberOfPhotonsPerPixelAtSDD(), 2);

    number_of_photons_per_cm2_at_1m = getNumberOfPhotonsPerCm2At1m();
    expected_result = inverseSquareLaw(number_of_photons_per_pixel_at_SDD / pixel_area, SDD_in_cm, 100);
    ASSERT_LT(abs(expected_result - number_of_photons_per_cm2_at_1m) / expected_result, max_error_in_percent);

    // Polychromatic
    setVoltage(100, "kV");

    // Set as per pixel at SDD
    number_of_photons_per_pixel_at_SDD = 10000;
    setNumberOfPhotonsPerPixelAtSDD(number_of_photons_per_pixel_at_SDD);
    ASSERT_NEAR(number_of_photons_per_pixel_at_SDD, getNumberOfPhotonsPerPixelAtSDD(), 2);

    number_of_photons_per_cm2_at_1m = getNumberOfPhotonsPerCm2At1m();
    expected_result = inverseSquareLaw(number_of_photons_per_pixel_at_SDD / pixel_area, SDD_in_cm, 100);
    ASSERT_LT(abs(expected_result - number_of_photons_per_cm2_at_1m) / expected_result, max_error_in_percent);

    // Set as per cm2 at 1m
    number_of_photons_per_cm2_at_1m = 10000;
    setNumberOfPhotonsPerCm2At1m(number_of_photons_per_cm2_at_1m);
    ASSERT_NEAR(number_of_photons_per_cm2_at_1m, getNumberOfPhotonsPerCm2At1m(), 2);

    number_of_photons_per_pixel_at_SDD = getNumberOfPhotonsPerPixelAtSDD();
    expected_result = inverseSquareLaw(number_of_photons_per_cm2_at_1m * pixel_area, 100, SDD_in_cm);
    ASSERT_LT(abs(number_of_photons_per_pixel_at_SDD - expected_result) / expected_result, max_error_in_percent);
}


//---------------------------------------
TEST(TestFluxConversion, NoiseStatistics)
//---------------------------------------
{
    float initial_sdd = 861;

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

    setDetectorPosition(0, 0, 0, "mm");
    setDetectorUpVector(0, 1, 0);
    setDetectorNumberOfPixels(360, 360);
    setDetectorPixelSize(150, 150, "um");
    setSourcePosition(0, 0, -initial_sdd, "mm");
    enablePoissonNoise();
    usePointSource();

    // Use cm2
    float voltage = 160;
    float exposure = 0.167;
    float target_amperage = 300;
    float mAs = exposure * (target_amperage * 0.001);
    setVoltage(voltage, "kV");
    setmAs(mAs);

    // SDD: 861
    float initial_number_of_photons_per_pixel_at_SDD = 38240.79035851772; // Value was calibrated
    float initial_number_of_photons_per_cm2_at_1m = 125994225.85208292; // Value was calibrated

    // Set the flux using number of photons per pixel at SDD
    setNumberOfPhotonsPerPixelAtSDD(initial_number_of_photons_per_pixel_at_SDD);
    ASSERT_NEAR(initial_number_of_photons_per_pixel_at_SDD, getNumberOfPhotonsPerPixelAtSDD(), 1);
    ASSERT_NEAR(initial_number_of_photons_per_cm2_at_1m, getNumberOfPhotonsPerCm2At1m(), 0.01 * initial_number_of_photons_per_cm2_at_1m);

    gVirtualXRay::Image<float> image_0 = g_xray_renderer.getWhiteImage(true);

    // computeXRayImage(image_0.getRawData(),
    //     image_0.getHeight(),
    //     image_0.getWidth(),
    //     true);

    // Set the flux using number of photons per cm2 at 1m
    setNumberOfPhotonsPerCm2At1m(initial_number_of_photons_per_cm2_at_1m);
    ASSERT_NEAR(initial_number_of_photons_per_pixel_at_SDD, getNumberOfPhotonsPerPixelAtSDD(), 0.01 * initial_number_of_photons_per_pixel_at_SDD);
    ASSERT_NEAR(initial_number_of_photons_per_cm2_at_1m, getNumberOfPhotonsPerCm2At1m(), 1);

    // computeXRayImage(image_1.getRawData(),
    //     image_1.getHeight(),
    //     image_1.getWidth(),
    //     true);

    gVirtualXRay::Image<float> image_1 = g_xray_renderer.getWhiteImage(true);
    compareImages(image_0, image_1, __LINE__);

    // Change the SDD.
    float new_sdd = 900;
    setSourcePosition(0, 0, -new_sdd, "mm");

    // Apply the inverse square law
    float new_number_of_photons_per_pixel_at_SDD = initial_number_of_photons_per_pixel_at_SDD * (std::pow(initial_sdd, 2) / std::pow(new_sdd, 2));
    ASSERT_NEAR(new_number_of_photons_per_pixel_at_SDD, getNumberOfPhotonsPerPixelAtSDD(), 0.01 * new_number_of_photons_per_pixel_at_SDD);

    // Make sure the flux at the detector decreases when SDD increases
    if (new_sdd > initial_sdd) {
        ASSERT_TRUE(new_number_of_photons_per_pixel_at_SDD < initial_number_of_photons_per_pixel_at_SDD);
    }
    // Make sure the flux at the detector increases when SDD decreases
    else if (new_sdd < initial_sdd) {
        ASSERT_TRUE(new_number_of_photons_per_pixel_at_SDD > initial_number_of_photons_per_pixel_at_SDD);
    }

    // The flux using number of photons per cm2 at 1m should not change.
    ASSERT_NEAR(initial_number_of_photons_per_cm2_at_1m, getNumberOfPhotonsPerCm2At1m(), 0.01 * initial_number_of_photons_per_cm2_at_1m);

    image_0 = g_xray_renderer.getWhiteImage(true);

    // Set the flux using number of photons per pixel at SDD
    setNumberOfPhotonsPerPixelAtSDD(new_number_of_photons_per_pixel_at_SDD);
    ASSERT_NEAR(new_number_of_photons_per_pixel_at_SDD, getNumberOfPhotonsPerPixelAtSDD(), 1);

    // The flux using number of photons per cm2 at 1m should not change.
    ASSERT_NEAR(initial_number_of_photons_per_cm2_at_1m, getNumberOfPhotonsPerCm2At1m(), 0.01 * initial_number_of_photons_per_cm2_at_1m);

    image_1 = g_xray_renderer.getWhiteImage(true);
    compareImages(image_0, image_1, __LINE__);

    // Set the flux using number of photons per cm2 at 1m
    setNumberOfPhotonsPerCm2At1m(initial_number_of_photons_per_cm2_at_1m);
    ASSERT_NEAR(new_number_of_photons_per_pixel_at_SDD, getNumberOfPhotonsPerPixelAtSDD(), 0.01 * new_number_of_photons_per_pixel_at_SDD);
    ASSERT_NEAR(initial_number_of_photons_per_cm2_at_1m, getNumberOfPhotonsPerCm2At1m(), 1);

    // computeXRayImage(image_1.getRawData(),
    //     image_1.getHeight(),
    //     image_1.getWidth(),
    //     true);

    image_1 = g_xray_renderer.getWhiteImage(true);
    compareImages(image_0, image_1, __LINE__);
}


//------------------------------------------------------------
void compareImages(const gVirtualXRay::Image<float>& anImage0,
                   const gVirtualXRay::Image<float>& anImage1,
                   unsigned int aLineNumber)
//------------------------------------------------------------
{
    gVirtualXRay::LOGGER.logNow("Call from Line #") << aLineNumber << std::endl;
    ASSERT_NEAR(anImage0.getMinValue(), anImage1.getMinValue(), 0.02 * std::abs(anImage0.getMinValue()));
    ASSERT_NEAR(anImage0.getMaxValue(), anImage1.getMaxValue(), 0.02 * std::abs(anImage0.getMaxValue()));
    ASSERT_NEAR(anImage0.getSum(), anImage1.getSum(), 0.02 * std::abs(anImage0.getSum()));
    ASSERT_NEAR(anImage0.getAverage(), anImage1.getAverage(), 0.02 * std::abs(anImage0.getAverage()));
    ASSERT_NEAR(anImage0.getVariance(), anImage1.getVariance(), 0.02 * std::abs(anImage0.getVariance()));
    ASSERT_NEAR(anImage0.getStandardDeviation(), anImage1.getStandardDeviation(), 0.02 * std::abs(anImage0.getStandardDeviation()));
    ASSERT_NEAR(anImage0.computeMAE(anImage1), 0.0, 0.01 * std::abs(anImage0.getAverage()));
    ASSERT_NEAR(anImage0.computeRMSE(anImage1), 0.0, 0.01 * std::abs(anImage0.getAverage()));
}