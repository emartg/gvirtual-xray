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
*   @file       TestBeerLambertLawMonoChromatic.cxx
*
*   @brief      Unit testing: Beer-Lambert Law, Monochromatism Case,
*               the unit test corresponds to validation_03,
*               last date of check: Passed test on 08 May 2019.
*
*   @version    1.0
*
*   @date       08/05/2019
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
#include "gtest/gtest.h"

#ifndef GVXR_gVXRLogger_h
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
int g_number_of_pixels[] = {int(600 / g_scaling_factor), 1};
double g_source_position_in_cm[]   = {0.0, 0.0, -10.0};
double g_detector_position_in_cm[] = {0.0, 0.0,  10.0};
double g_detector_up_vector[]      = {0.0, 1.0,  0.0};

double g_literature_result_in_keV(0.0);
double g_expected_result_in_keV(0.0);


//******************************************************************************
//  Function declaration
//******************************************************************************
void loadSTLFile();
void loadDetector();
void loadSource();


//--------------------------------------------------------------------
TEST(TestBeerLambertLawMonoChromatic, TestBeerLambertLawPolyChromatic)
//--------------------------------------------------------------------
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

    // Enable artefact filtering on GPU
    enableArtefactFilteringOnGPU();

    // Compute the X-ray image
    std::vector<std::vector<float> > x_ray_image = computeXRayImage();

    std::vector<float> row = x_ray_image[x_ray_image.size() / 2];
    float central_pixel = row[row.size() / 2];

    float simulated_result_in_keV = central_pixel / getUnitOfEnergy("keV");

    gVirtualXRay::LOGGER.logNow() << "On GPU, with half precision, the energy (in keV) transmitted "
        "orthogonally throw the middle of cube and "
        "cylinder actually is:" << std::endl;

    gVirtualXRay::LOGGER.logNow() << "\t\t           = " << simulated_result_in_keV << " keV" <<
        std::endl << std::endl;

    gVirtualXRay::LOGGER.logNow() << "The absolute error is: "
        "|I_expected - I_simulated| keV" << std::endl;

    gVirtualXRay::LOGGER.logNow() << "                     : |" <<
        g_expected_result_in_keV << " - " << simulated_result_in_keV <<
        "| keV" << std::endl;

    gVirtualXRay::LOGGER.logNow() << "                     : " <<
        std::fixed << std::abs(g_expected_result_in_keV -
            simulated_result_in_keV) << " keV" << std::endl << std::endl;

    gVirtualXRay::LOGGER.logNow() << "The relative error is: |I_expected - I_simulated| "
        "/ I_expected" << std::endl;

    gVirtualXRay::LOGGER.logNow() << "                     : " <<
        std::fixed << 100.0 * std::abs(g_expected_result_in_keV -
            simulated_result_in_keV) / g_expected_result_in_keV <<
        "%" << std::endl;

    destroyAllWindows();

    ASSERT_NEAR(1, simulated_result_in_keV / g_expected_result_in_keV, 0.01);

    // The absolute difference is less than 0.01 keV
    ASSERT_EQ(abs(g_expected_result_in_keV - simulated_result_in_keV) < 0.01, true);
}


//----------------
void loadSTLFile()
//----------------
{
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
    gVirtualXRay::LOGGER.logNow() << "- A cube, which has edge length of 3 cm, "
        "is made of soft tissue (HU = " <<
        std::floor(HU_soft_tissue + 0.5) << "). " << std::endl;
    gVirtualXRay::LOGGER.logNow() << "- Inside, a cylinder is inserted. " <<
            "It is made of bone (HU = " << std::floor(HU_bone + 0.5) <<
            "), its height is 3 cm and its diameter is 2 cm." << std::endl;
    gVirtualXRay::LOGGER.logNow() << "- We consider the energy of the incident beam is " <<
        g_incident_energy_in_keV << " keV (i.e. " <<
        g_incident_energy_in_keV << " keV). " << std::endl << std::endl;

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
            "mu = "     << std::scientific << 1.932E-01 << " cm-1" <<
            std::endl << std::endl;

    g_literature_result_in_keV = g_incident_energy_in_keV *
        exp(-(4.280e-01 * 2 + 1.932e-01 * 1));

    gVirtualXRay::LOGGER.logNow() << "The energy (in keV) transmitted orthogonally throw "
        "the middle of cube and cylinder should be:" << std::endl;

    gVirtualXRay::LOGGER.logNow() << "\t\tI_reference = I_0 * exp(-Sum(mu * x))" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "\t\t            = " << std::fixed <<
        g_incident_energy_in_keV <<
        " * exp(-(mu_bone * 2 + mu_soft_tissue * 1))" << std::endl;

    gVirtualXRay::LOGGER.logNow() << "\t\t            = " << g_incident_energy_in_keV <<
        " * exp(-(4.280e-01 * 2 + 1.932e-01 * 1))" << std::endl;

    gVirtualXRay::LOGGER.logNow() << "\t\t            = " << g_literature_result_in_keV << " keV" <<
        std::endl << std::endl;

    gVirtualXRay::LOGGER.logNow() << "To obtain linear attenuation coefficients (mu values) in "
        "our implementation, densities (rho values) and mass attenuation "
        "coefficients (mu/rho values) are estimated as follows:" << std::endl;

    gVirtualXRay::LOGGER.logNow() <<
            "HU =    "  << std::fixed << HU_water << "\t" <<
            "rho = "    << std::scientific <<
                getDensityFromHU(HU_water) << " g/cm3,\t" <<
            "mu/rho = " << std::scientific <<
                getMassAttenuationFromHU(HU_water,
                    g_incident_energy_in_keV,
                    "keV") << " cm2/g,\t" <<
            "mu = "     << std::scientific << getMuFromHU(HU_water,
                g_incident_energy_in_keV,
                "keV") << " cm-1" << std::endl;

    gVirtualXRay::LOGGER.logNow() <<
            "HU = "     << std::fixed << HU_bone << "\t" <<
            "rho = "    << std::scientific << getDensityFromHU(HU_bone) <<
                " g/cm3,\t" <<
            "mu/rho = " << std::scientific << getMassAttenuationFromHU(HU_bone,
                g_incident_energy_in_keV,
                "keV") << " cm2/g,\t" <<
            "mu = "     << std::scientific << getMuFromHU(HU_bone,
                g_incident_energy_in_keV, "keV") << " cm-1" << std::endl;

    gVirtualXRay::LOGGER.logNow() <<
            "HU =   "     << std::fixed << HU_soft_tissue << "\t" <<
            "rho = "    << std::scientific <<
                getDensityFromHU(HU_soft_tissue) << " g/cm3,\t" <<
            "mu/rho = " << std::scientific << getMassAttenuationFromHU(
                HU_soft_tissue, g_incident_energy_in_keV, "keV") <<
                    " cm2/g,\t" <<
            "mu = "     << std::scientific << getMuFromHU(
                HU_soft_tissue, g_incident_energy_in_keV, "keV") <<
                    " cm-1" << std::endl << std::endl;

    g_expected_result_in_keV = g_incident_energy_in_keV *
        exp(-(getMuFromHU(HU_bone, g_incident_energy_in_keV, "keV") * 2 +
            getMuFromHU(HU_soft_tissue, g_incident_energy_in_keV, "keV") * 1));

    gVirtualXRay::LOGGER.logNow() << "The energy (in keV) transmitted orthogonally throw "
        "the middle of cube and cylinder should be:" << std::endl;

    gVirtualXRay::LOGGER.logNow() << "\t\tI_expected = I_0 * exp(-Sum(mu * x))" << std::endl;

    gVirtualXRay::LOGGER.logNow() << "\t\t           = " << std::fixed <<
        g_incident_energy_in_keV <<
        " * exp(-(mu_bone * 2 + mu_soft_tissue * 1))" << std::endl;

    gVirtualXRay::LOGGER.logNow() << "\t\t           = " << std::fixed <<
        g_incident_energy_in_keV <<
        " * exp(-(" << std::scientific <<
        getMuFromHU(HU_bone, g_incident_energy_in_keV, "keV") <<
        " * 2 + " <<
        getMuFromHU(HU_soft_tissue, g_incident_energy_in_keV, "keV") <<
        " * 1))" << std::endl;

    gVirtualXRay::LOGGER.logNow() << "\t\t           = " << std::fixed <<
        g_expected_result_in_keV << " keV" << std::endl << std::endl;

    gVirtualXRay::LOGGER.logNow() << "The absolute error is: |I_expected - I_reference| keV" <<
        std::endl;

    gVirtualXRay::LOGGER.logNow() << "                     : |" <<
        g_expected_result_in_keV << " - " << g_literature_result_in_keV <<
        "| keV" << std::endl;

    gVirtualXRay::LOGGER.logNow() << "                     : " << std::fixed <<
        std::abs(g_expected_result_in_keV - g_literature_result_in_keV) <<
        " keV" << std::endl << std::endl;

    gVirtualXRay::LOGGER.logNow() << "The relative error is: 100 * |I_expected - I_reference| / I_reference %" <<
        std::endl;

    gVirtualXRay::LOGGER.logNow() << "                     : 100 * |" <<
        g_expected_result_in_keV << " - " << g_literature_result_in_keV <<
        "| / " << g_literature_result_in_keV << " %"<< std::endl;

    gVirtualXRay::LOGGER.logNow() << "                     : " << std::fixed <<
        100.0 * std::abs(g_expected_result_in_keV - g_literature_result_in_keV) / g_literature_result_in_keV <<
        " %" << std::endl << std::endl;
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

    // Use a parallel source
    useParallelBeam();
}
