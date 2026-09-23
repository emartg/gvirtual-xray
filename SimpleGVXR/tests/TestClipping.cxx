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

3. Neither the name of UK Research and Innovation nor the names of its contributors
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
*   @file       TestClipping.cxx
*
*   @brief      Unit testing: test if vertices behind the source are clipped.
*
*   @version    1.0
*
*   @date       03/11/2025
*   @author     Prof Franck P. Vidal
*
*   @section    License
*               BSD 3-Clause License.
*
*               For details on use and redistribution please refer
*               to http://opensource.org/licenses/BSD-3-Clause
*
*   @section    Copyright
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk),
*   https://gvirtualxray.sourceforge.io/, Nov 2025, 2025, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <cmath>
#include <iomanip>
#include <memory>
#include <cstdlib>

#include "gtest/gtest.h"

#ifndef GVXR_LOGGER_H
#include "gVirtualXRay/Logger.h"
#endif

#ifndef SGVXR_SimpleGVXR_h
#include "SimpleGVXR.h"
#endif

#ifndef SGVXR_Utils_h
#include "Utils.h"
#endif

#ifndef SGVXR_Internal_h
#include "internal.h"
#endif


//******************************************************************************
//  Global variables
//******************************************************************************
int g_main_window_width(1280 / 2.0);
int g_main_window_height(800 / 2.0);

double g_incident_energy_in_keV(80.0);
const double g_scaling_factor(2.0);
double g_resolution_in_mm = (0.15 * g_scaling_factor); // 1mm per pixel
int g_number_of_pixels[] = {int(600 / g_scaling_factor), int(600 / g_scaling_factor)};
double g_source_position_in_cm[]   = {0.0, 0.0, -10.0};
double g_detector_position_in_cm[] = {0.0, 0.0,  10.0};
double g_detector_up_vector[]      = {0.0, 1.0,  0.0};

float g_100_um_in_cm = 1e-2;

double g_cylinder_radius = 1.0;
double g_cube_length = 3.0;


//******************************************************************************
//  Function declaration
//******************************************************************************
void loadSTLFile();
void loadDetector();
void loadSource();


//------------------------------
TEST(TestClipping, TestClipping)
//------------------------------
{
    // Make sure the detector has an odd number of columns
    if ((g_number_of_pixels[0] % 2) == 0)
    {
        g_number_of_pixels[0] = g_number_of_pixels[0] + 1;
    }

    // Make sure the detector has an odd number of rows
    if ((g_number_of_pixels[1] % 2) == 0)
      {
          g_number_of_pixels[1] = g_number_of_pixels[1] + 1;
      }

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

    setWindowSize(g_main_window_width, g_main_window_height);

    // Load the data
    loadSource();
    loadDetector();

    // Add the geometry to the X-ray renderer
    loadSTLFile();
    addPolygonMeshAsInnerSurface("inside");
    addPolygonMeshAsOuterSurface("outside");

    // Enable artefact filtering on GPU
    enableArtifactFilteringOnGPU();

    // Compute the X-ray image
    std::vector<std::vector<float> > x_ray_image = computeXRayImage();

    // Compute the L-buffer of the smallest part
    std::vector<std::vector<float> > lbuffer_inside1 = computeLBuffer("inside");
    float pixel_in_centre_inside1 = lbuffer_inside1[g_number_of_pixels[1] / 2][g_number_of_pixels[0] / 2];
    saveLastLBuffer("inside1.mha");

    // Compute the L-buffer of the largest part
    std::vector<std::vector<float> > lbuffer_outside1 = computeLBuffer("outside");
    float pixel_in_centre_outside1 = lbuffer_outside1[g_number_of_pixels[1] / 2][g_number_of_pixels[0] / 2];
    saveLastLBuffer("outside1.mha");

    // Move the source in the middle of the sample
    disableArtefactFiltering();
    setSourcePosition(0, 0, -g_cylinder_radius + 0.001, "cm");

    // Compute the L-buffer of the smallest part
    std::vector<std::vector<float> > lbuffer_inside2 = computeLBuffer("inside");
    // float pixel_in_centre_inside2 = lbuffer_inside2[g_number_of_pixels[1] / 2][g_number_of_pixels[0] / 2];
    saveLastLBuffer("inside2.mha");

    // Compute the L-buffer of the largest part
    std::vector<std::vector<float> > lbuffer_outside2 = computeLBuffer("outside");
    // float pixel_in_centre_outside2 = lbuffer_outside2[g_number_of_pixels[1] / 2][g_number_of_pixels[0] / 2];
    saveLastLBuffer("outside2.mha");


    // Close the window
    destroyAllWindows();

    std::cout<<g_number_of_pixels[1] / 2 << "\t" << g_number_of_pixels[0] / 2 <<std::endl;
    std::cout<<2.0f * g_cylinder_radius << "\t" << pixel_in_centre_inside1 <<std::endl;
    std::cout<<g_cube_length - 2.0f * g_cylinder_radius << "\t" << pixel_in_centre_outside1 <<std::endl;


    ASSERT_NEAR(2.0f * g_cylinder_radius,
        pixel_in_centre_inside1,
        g_100_um_in_cm);

    ASSERT_NEAR(g_cube_length - 2.0f * g_cylinder_radius,
        pixel_in_centre_outside1,
        g_100_um_in_cm);

/*
    ASSERT_NEAR(pixel_in_centre_inside1 / 2.0f,
        pixel_in_centre_inside2,
        g_100_um_in_cm);

    ASSERT_NEAR(pixel_in_centre_outside1 / 2.0f,
        pixel_in_centre_outside2,
        g_100_um_in_cm);*/
}


//----------------
void loadSTLFile()
//----------------
{
	// Set the geometry
  // Set the geometry
  makeCube("outside",
          g_cube_length,
          "cm",
          "root");

  makeCylinder("inside",
          20,
          3.0,
          g_cylinder_radius,
          "cm",
          "root");

    // HU values
    double HU_bone(1329.885683);
    double HU_soft_tissue(51.714752);

    setHU( "inside", HU_bone);
    setHU("outside", HU_soft_tissue);
}


//-----------------
void loadDetector()
//-----------------
{
    setDetectorPixelSize(g_resolution_in_mm, g_resolution_in_mm , "mm");

    setDetectorNumberOfPixels(g_number_of_pixels[0], g_number_of_pixels[1]);

    setDetectorPosition(
        g_detector_position_in_cm[0],
        g_detector_position_in_cm[1],
        g_detector_position_in_cm[2],
        "cm");

    setDetectorUpVector(
        g_detector_up_vector[0],
        g_detector_up_vector[1],
        g_detector_up_vector[2]);
}


//---------------
void loadSource()
//---------------
{
    // Set the energy
    setMonoChromatic(g_incident_energy_in_keV, "keV", 1);

    // Set the source position
    usePointSource();
    setSourcePosition(g_source_position_in_cm[0],
        g_source_position_in_cm[1],
        g_source_position_in_cm[2],
        "cm");
}
