/*

Copyright (c) 2019, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
http://www.fpvidal.net/
All rights reserved.

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
*   @file       TestCubeSource.cxx
*
*   @brief      Unit testing: Cube source.
*               A similar experimental set up is used with a validated
*               Monte Carlo simulation tool. We compare the results produce with
*               gVirtualXRay.
*   			The unit test corresponds to validation_07,
*   			last date of check: Passed test on 03 Apr 2019.
*
*   @version    1.0
*
*   @date       03/04/2019
*   @author     Dr Franck P. Vidal
*
*   @section    License
*               BSD 3-Clause License.
*
*               For details on use and redistribution please refer
*               to http://opensource.org/licenses/BSD-3-Clause
*
*   @section    Copyright
*               (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*               http://www.fpvidal.net/, Apr 2019, 2019, version 1.0,
*               BSD 3-Clause License
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

double g_source_size_in_cm(0.5);


//******************************************************************************
//  Function declaration
//******************************************************************************
void loadSTLFile();
void loadDetector();
void loadSource();


//----------------------------------
TEST(TestCubeSource, TestCubeSource)
//----------------------------------
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
    loadDetector();
    loadSource();

    // Add the geometry to the X-ray renderer
    loadSTLFile();
    addPolygonMeshAsInnerSurface("inside");
    addPolygonMeshAsOuterSurface("outside");
/*
    // Output the detector properties
    gVirtualXRay::LOGGER.logNow() << "Detector size: " << g_number_of_pixels.getX() << "x" << g_number_of_pixels.getY() << " pixels" << std::endl;
    gVirtualXRay::LOGGER.logNow() << std::fixed << "Pixel size:" << g_resolution / mm << "x" << g_resolution / mm << " mm2" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "Detector size: " << g_resolution / cm * g_number_of_pixels.getX() << "x" << g_resolution / cm * g_number_of_pixels.getY() << " cm2" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "Detector position: " << g_detector_position / cm << " (in cm)" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "Source position: "   << g_source_position   / cm << " (in cm)" << std::endl;

    RATIONAL_NUMBER tangent(
        ((g_resolution * g_number_of_pixels.getX()) / 2.0) /
        ((g_detector_position - g_source_position).length()));

    RATIONAL_NUMBER angle_in_radians(2.0 * std::atan(tangent));
    RATIONAL_NUMBER angle_in_degrees(180.0 * angle_in_radians / M_PI);
    gVirtualXRay::LOGGER.logNow() << "Field of view: " <<
        angle_in_degrees <<
        " degrees" << std::endl;
*/
    // Enable artefact filtering on GPU
    enableArtifactFilteringOnGPU();

    // Compute the projection set
    std::vector<std::vector<std::vector<float> > > x_ray_image;
    x_ray_image.push_back(computeXRayImage());

    gVirtualXRay::LOGGER.logNow() << getImageWidth(x_ray_image[0]) << " x " << getImageHeght(x_ray_image[0]) << std::endl;

    // Close the window
    destroyAllWindows();

    // Load reference image (computed using GEANT4/Gate)
    std::vector<std::vector<std::vector<float> > > reference_projection_set = loadImage3D("data/cube_source/gate_simulation.mhd");

    gVirtualXRay::LOGGER.logNow() << getImageWidth(reference_projection_set[0]) << " x " << getImageHeght(reference_projection_set[0]) << std::endl;
    //reference_projection_set = reference_projection_set.normalised() * g_incident_energy_in_keV;

    // Compare reference projection set and simulated projection set using NCC
    double ZNCC = computeZNCC(reference_projection_set, x_ray_image);
    ASSERT_NEAR(ZNCC, 1, 1e-2);

    // Compare reference projection set and simulated projection set using RMSE
    float RMSE = computeRMSE(reference_projection_set, x_ray_image);
    ASSERT_NEAR(RMSE, 0.0, 1e-2 * g_incident_energy_in_keV);
}


//----------------
void loadSTLFile()
//----------------
{
	// Set the geometry
  // Set the geometry
  makeCube("outside",
          3.0,
          "cm",
          "root");

  makeCylinder("inside",
          20,
          3.0,
          1.0,
          "cm",
          "root");

    // HU values
    double HU_water(0.0);
    double HU_bone(1329.885683);
    double HU_soft_tissue(51.714752);

    setHU( "inside", HU_bone);
    setHU("outside", HU_soft_tissue);

    gVirtualXRay::LOGGER.logNow() << "We run the simulation as follows:" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "- A cube, which has edge length of 3 cm, is made of soft tissue (HU = " << std::floor(HU_soft_tissue + 0.5) << "). " << std::endl;
    gVirtualXRay::LOGGER.logNow() << "- Inside, a cylinder is inserted. " <<
        "It is made of bone (HU = " << std::floor(HU_bone + 0.5) << "), its height is 3 cm and its diameter is 2 cm." << std::endl;
    gVirtualXRay::LOGGER.logNow() << "- We consider the energy of the incident beam is " << g_incident_energy_in_keV << " keV (i.e. " << g_incident_energy_in_keV / 1000 << " MeV). " << std::endl << std::endl;

    gVirtualXRay::LOGGER.logNow() << "From the literature, we know:" << std::endl;
    std::cout.precision(3);
    gVirtualXRay::LOGGER.logNow() <<
        "HU =    "     << std::fixed << HU_water << "\t" <<
        "rho = "    << std::scientific << 1.0 << " g/cm3,\t" <<
        "mu/rho = " << std::scientific << 1.837E-01 << " cm2/g,\t" <<
        "mu = "     << std::scientific << 1.837E-01 << " cm-1" << std::endl;

    gVirtualXRay::LOGGER.logNow() <<
        "HU = "     << std::fixed << HU_bone << "\t" <<
        "rho = "    << std::scientific << 1.920E+00 << " g/cm3,\t" <<
        "mu/rho = " << std::scientific << 2.229E-01 << " cm2/g,\t" <<
        "mu = "     << std::scientific << 4.280E-01 << " cm-1" << std::endl;

    gVirtualXRay::LOGGER.logNow() <<
        "HU =   "     << std::fixed << HU_soft_tissue << "\t" <<
        "rho = "    << std::scientific << 1.060E+00 << " g/cm3,\t" <<
        "mu/rho = " << std::scientific << 1.823E-01 << " cm2/g,\t" <<
        "mu = "     << std::scientific << 1.932E-01 << " cm-1" << std::endl << std::endl;

    gVirtualXRay::LOGGER.logNow() << "To obtain linear attenuation coefficients (mu values) in our implementation, densities (rho values) and mass attenuation coefficients (mu/rho values) are estimated as follows:" << std::endl;
    gVirtualXRay::LOGGER.logNow() <<
        "HU =    "     << std::fixed << HU_water << "\t" <<
        "rho = "    << std::scientific << getDensityFromHU(HU_water) << " g/cm3,\t" <<
        "mu/rho = " << std::scientific << getMassAttenuationFromHU(HU_water, g_incident_energy_in_keV, "keV") << " cm2/g,\t" <<
        "mu = "     << std::scientific << getMuFromHU(HU_water, g_incident_energy_in_keV, "keV") << " cm-1" << std::endl;

    gVirtualXRay::LOGGER.logNow() <<
        "HU = "     << std::fixed << HU_bone << "\t" <<
        "rho = "    << std::scientific << getDensityFromHU(HU_bone) << " g/cm3,\t" <<
        "mu/rho = " << std::scientific << getMassAttenuationFromHU(HU_bone, g_incident_energy_in_keV, "keV") << " cm2/g,\t" <<
        "mu = "     << std::scientific << getMuFromHU(HU_bone, g_incident_energy_in_keV, "keV") << " cm-1" << std::endl;

    gVirtualXRay::LOGGER.logNow() <<
        "HU =   "     << std::fixed << HU_soft_tissue << "\t" <<
        "rho = "    << std::scientific << getDensityFromHU(HU_soft_tissue) << " g/cm3,\t" <<
        "mu/rho = " << std::scientific << getMassAttenuationFromHU(HU_soft_tissue, g_incident_energy_in_keV, "keV") << " cm2/g,\t" <<
        "mu = "     << std::scientific << getMuFromHU(HU_soft_tissue, g_incident_energy_in_keV, "keV") << " cm-1" << std::endl << std::endl;
/*
    g_scene_graph.getChild("outside").getPolygonMesh().splitFaces(false);
    g_scene_graph.getChild("outside").getPolygonMesh().splitFaces(false);
    g_scene_graph.getChild("outside").getPolygonMesh().splitFaces(false);
    g_scene_graph.getChild("outside").getPolygonMesh().splitFaces(false);
*/
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

    // Set the source position and focal spot
    setFocalSpot(g_source_position_in_cm[0],
        g_source_position_in_cm[1],
        g_source_position_in_cm[2],
        g_source_size_in_cm, "cm",
        25);

    /*usePointSource();
    setSourcePosition(g_source_position_in_cm[0],
        g_source_position_in_cm[1],
        g_source_position_in_cm[2],
        "cm");
*/
}
