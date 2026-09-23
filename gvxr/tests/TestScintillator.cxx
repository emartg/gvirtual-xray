/*

Copyright (c) 2023-2023, Dr Franck P. Vidal, Bangor University, All rights reserved.
Copyright (c) 2023-present, Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), 
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
*   @file       TestScintilator.cxx
*
*   @brief      Unit testing: scintillator and energy response,
*               Last date of check: Passed test on 27 Jun 2023.
*
*   @date       18/04/2024
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
*   (c) by Dr Franck P. Vidal, Bangor University, Jun 2023-Sept 2023, version 1.1, BSD 3-Clause License
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, Oct 2023-present, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <string>
#include <fstream>
#include <vector>
#include <cmath>
#include <numeric>

#include "gtest/gtest.h"

#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#ifndef GVXR_SCINTILLATOR_H
#include "gVirtualXRay/Scintillator.h"
#endif

#ifndef GVXR_CONTEXT_H
#include "gVirtualXRay/Context.h"
#endif

#ifndef GVXR_XRAY_RENDERER_H
#include "gVirtualXRay/XRayRenderer.h"
#endif

#ifndef GVXR_XRAY_BEAM_H
#include "gVirtualXRay/XRayBeam.h"
#endif

#ifndef GVXR_XRAY_DETECTOR_H
#include "gVirtualXRay/XRayDetector.h"
#endif

#ifndef GVXR_CUBOID_MESH_H
#include "gVirtualXRay/CuboidMesh.h"
#endif

#ifndef GVXR_utils_h
#include "utils.h"
#endif


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//  Global variables
//******************************************************************************
Context g_GL_context;

GLfloat g_incident_energy(80.0 * keV);
unsigned int g_photon_count = 1800;
Vec2ui g_number_of_pixels(288, 288);
VEC2 g_detector_size(432 * mm, 432 * mm);
VEC2 g_resolution(g_detector_size[0] / g_number_of_pixels[0],
    g_detector_size[1] / g_number_of_pixels[1]);
VEC3 g_source_position(   0.0, 0.0, -188.0 * mm);
VEC3 g_detector_position( 0.0, 0.0,  562.0 * mm);
VEC3 g_detector_up_vector(0.0, 1.0,   0.0);

SceneGraphNode g_scene_graph("root", NULL);

XRayBeam g_xray_beam;
XRayDetector g_xray_detector;
XRayRenderer g_xray_renderer;


//******************************************************************************
//  Function declaration
//******************************************************************************
void runLookTableTest(const char* aFileName,
    const std::vector<std::pair<RATIONAL_NUMBER, RATIONAL_NUMBER> >& response);

void runImageTest(const char* aFileName,
    const Image<XRayRenderer::PixelType> anImage);

void loadSample();
void loadDetector();
void loadSource();
void loadXRaySimulator();
void errorCallback(int error, const char* description);
void quit();

double getSum(const std::vector<double>& aVector);
double getAverage(const std::vector<double>& aVector);
double getVariance(const std::vector<double>& aVector);
double getStdDev(const std::vector<double>& aVector);
double getNCC(const std::vector<double>& aVector1, const std::vector<double>& aVector2);
double maxRelativeError(const std::vector<double>& aVector1, const std::vector<double>& aVector2);
double meanRelativeError(const std::vector<double>& aVector1, const std::vector<double>& aVector2);


//----------------------------------------------------------------------------------------------
void runLookTableTest(const char* aFileName,
                      const std::vector<std::pair<RATIONAL_NUMBER, RATIONAL_NUMBER> >& response)
//----------------------------------------------------------------------------------------------
{
    // Open the files
    std::ifstream reference_data_stream(aFileName);

    // The file is not open
    if (!reference_data_stream.is_open())
    {
        std::string error_message;
        error_message += "\"";
        error_message += aFileName;
        error_message += "\" does not exist";

        FAIL() << error_message;
    }

    std::vector<double> reference_vector;
    std::vector<double> test_vector;

    double current_ref_energy = 0;
    double last_ref_energy    = 0;
    double ref_response = 0;

    int i = 0;

    while (reference_data_stream.good())
    {
        reference_data_stream >> current_ref_energy >> ref_response;

        if (i == 0 || last_ref_energy != current_ref_energy)
        {
            reference_vector.push_back(ref_response);
            test_vector.push_back(response[i].second);
        }

        last_ref_energy = current_ref_energy;

        ++i;
    }

    // Tolerence 0.5%
    ASSERT_NEAR(100.0 * getNCC(reference_vector, test_vector), 100.0, 0.5);

    // Tolerence 0.5%
    ASSERT_NEAR(100.0 * maxRelativeError(reference_vector, test_vector), 0.0, 0.5);
}


//---------
void quit()
//---------
{
    g_xray_renderer.release();
    g_scene_graph.clear();
    g_GL_context.release();
}


//---------------
void loadSample()
//---------------
{
	// Set the geometry
    g_scene_graph.addChild( "Object 1", CuboidMesh<GLdouble>(43.0 * mm,
        32.0 * mm,
        25.0 * mm));

    // Material properties
    g_scene_graph.getChild( "Object 1").getPolygonMesh().setCompound("C");
    g_scene_graph.getChild( "Object 1").getPolygonMesh().setDensity(1.6 * g / cm3);

}


//-----------------
void loadDetector()
//-----------------
{
    g_xray_detector.setResolutionInUnitsOfLengthPerPixel(g_resolution);
    g_xray_detector.setNumberOfPixels(g_number_of_pixels);
    g_xray_detector.setDetectorPosition(g_detector_position);
    g_xray_detector.setUpVector(g_detector_up_vector);
}


//---------------
void loadSource()
//---------------
{
    // Set the energy
    g_xray_beam.initialise(g_incident_energy);
    g_xray_beam.setNumberOfPhotonsPerPixelAtSDD(g_photon_count, false);

    // Set the source position and size
    g_xray_detector.setXrayPointSource(g_source_position);
    
}


//----------------------
void loadXRaySimulator()
//----------------------
{
    // Initialise the X-ray renderer
#ifdef HAS_OPENGL
    g_xray_renderer.initialise(XRayRenderer::OPENGL,
        0, // Half-precision floating-point numbers is enough
        &g_xray_detector,
        &g_xray_beam);
#else
    g_xray_renderer.initialise(XRayRenderer::OPENMP,
        0,
        &g_xray_detector,
        &g_xray_beam);
#endif
}


//-----------------------------------------
TEST(TestScintilator, TestMonoEnergyImages)
//-----------------------------------------
{
    // Register the exit callback
    atexit(quit);

    // Initialise OpenGL
    initGL(g_GL_context);

    // Load the data
    loadDetector();
    loadSource();
    loadXRaySimulator();

    // Add the geometry to the X-ray renderer
    loadSample();
    g_xray_renderer.addInnerSurface(&g_scene_graph.getChild("Object 1"));

    // Compute the projection set
    g_xray_renderer.computeImage(MATRIX4(), true);

    // Load reference image (computed using GEANT4/Gate)
    Image<XRayRenderer::PixelType> ideal_detector_gate("data/scintillator-mono-80-keV/ideal_detector/old_gate/energy_fluence.mhd");
    Image<XRayRenderer::PixelType> CsI_0_6_gate("data/scintillator-mono-80-keV/CsI/0.6/old_gate_response/energy_fluence.mhd");
    Image<XRayRenderer::PixelType> CsI_1_2_gate("data/scintillator-mono-80-keV/CsI/1.2/old_gate_response/energy_fluence.mhd");
    Image<XRayRenderer::PixelType> Gadox_0_6_gate("data/scintillator-mono-80-keV/Gadox/0.6/old_gate_response/energy_fluence.mhd");
    Image<XRayRenderer::PixelType> Gadox_1_2_gate("data/scintillator-mono-80-keV/Gadox/1.2/old_gate_response/energy_fluence.mhd");

    Image<XRayRenderer::PixelType> ideal_detector_gVXR = g_xray_renderer.getEnergyFluence();

    g_xray_detector.setScintillator("CsI", 0.6 * cm);
    g_xray_renderer.computeImage(MATRIX4(), true);
    Image<XRayRenderer::PixelType> CsI_0_6_gVXR = g_xray_renderer.getEnergyFluence();

    g_xray_detector.setScintillator("CsI", 1.2 * cm);
    g_xray_renderer.computeImage(MATRIX4(), true);
    Image<XRayRenderer::PixelType> CsI_1_2_gVXR = g_xray_renderer.getEnergyFluence();

    g_xray_detector.setScintillator("Gadox", 0.6 * cm);
    g_xray_renderer.computeImage(MATRIX4(), true);
    Image<XRayRenderer::PixelType> Gadox_0_6_gVXR = g_xray_renderer.getEnergyFluence();

    g_xray_detector.setScintillator("Gadox", 1.2 * cm);
    g_xray_renderer.computeImage(MATRIX4(), true);
    Image<XRayRenderer::PixelType> Gadox_1_2_gVXR = g_xray_renderer.getEnergyFluence();

    // Compare reference projection set and simulated projection set using NCC
    ASSERT_NEAR(ideal_detector_gate.computeNCC(ideal_detector_gVXR), 1.0, 0.02);
    ASSERT_NEAR(CsI_0_6_gate.computeNCC(CsI_0_6_gVXR), 1.0, 0.02);
    ASSERT_NEAR(CsI_1_2_gate.computeNCC(CsI_1_2_gVXR), 1.0, 0.02);
    ASSERT_NEAR(Gadox_0_6_gate.computeNCC(Gadox_0_6_gVXR), 1.0, 0.02);
    ASSERT_NEAR(Gadox_1_2_gate.computeNCC(Gadox_1_2_gVXR), 1.0, 0.02);

    // // Compare reference projection set and simulated projection set using RMSE
    ASSERT_NEAR(ideal_detector_gate.computeRMSE(ideal_detector_gVXR), 0.0, ideal_detector_gate.getStandardDeviation());
    ASSERT_NEAR(CsI_0_6_gate.computeRMSE(CsI_0_6_gVXR), 0.0, CsI_0_6_gate.getStandardDeviation());
    ASSERT_NEAR(CsI_1_2_gate.computeRMSE(CsI_1_2_gVXR), 0.0, CsI_1_2_gate.getStandardDeviation());
    ASSERT_NEAR(Gadox_0_6_gate.computeRMSE(Gadox_0_6_gVXR), 0.0, Gadox_0_6_gate.getStandardDeviation());
    ASSERT_NEAR(Gadox_1_2_gate.computeRMSE(Gadox_1_2_gVXR), 0.0, Gadox_1_2_gate.getStandardDeviation());

    // Close the window and shut EGL
    // quit();
}


//-----------------------------------------
TEST(TestScintilator, TestPolyEnergyImages)
//-----------------------------------------
{
    // Register the exit callback
    atexit(quit);

    // Initialise OpenGL
    // initGL(g_GL_context);

    // Load the data
    loadDetector();
    loadSource();
    loadXRaySimulator();

    // Add the geometry to the X-ray renderer
    loadSample();
    g_xray_renderer.addInnerSurface(&g_scene_graph.getChild("Object 1"));

    // Compute the projection set
    g_xray_renderer.computeImage(MATRIX4(), true);

    // Load reference image (computed using GEANT4/Gate)
    Image<XRayRenderer::PixelType> ideal_detector_gate("data/scintillator-mono-80-keV/ideal_detector/old_gate/energy_fluence.mhd");
    Image<XRayRenderer::PixelType> CsI_0_6_gate("data/scintillator-mono-80-keV/CsI/0.6/old_gate_response/energy_fluence.mhd");
    Image<XRayRenderer::PixelType> CsI_1_2_gate("data/scintillator-mono-80-keV/CsI/1.2/old_gate_response/energy_fluence.mhd");
    Image<XRayRenderer::PixelType> Gadox_0_6_gate("data/scintillator-mono-80-keV/Gadox/0.6/old_gate_response/energy_fluence.mhd");
    Image<XRayRenderer::PixelType> Gadox_1_2_gate("data/scintillator-mono-80-keV/Gadox/1.2/old_gate_response/energy_fluence.mhd");

    Image<XRayRenderer::PixelType> ideal_detector_gVXR = g_xray_renderer.getEnergyFluence();

    g_xray_detector.setScintillator("CsI", 0.6 * cm);
    g_xray_renderer.computeImage(MATRIX4(), true);
    Image<XRayRenderer::PixelType> CsI_0_6_gVXR = g_xray_renderer.getEnergyFluence();

    g_xray_detector.setScintillator("CsI", 1.2 * cm);
    g_xray_renderer.computeImage(MATRIX4(), true);
    Image<XRayRenderer::PixelType> CsI_1_2_gVXR = g_xray_renderer.getEnergyFluence();

    g_xray_detector.setScintillator("Gadox", 0.6 * cm);
    g_xray_renderer.computeImage(MATRIX4(), true);
    Image<XRayRenderer::PixelType> Gadox_0_6_gVXR = g_xray_renderer.getEnergyFluence();

    g_xray_detector.setScintillator("Gadox", 1.2 * cm);
    g_xray_renderer.computeImage(MATRIX4(), true);
    Image<XRayRenderer::PixelType> Gadox_1_2_gVXR = g_xray_renderer.getEnergyFluence();

    // Compare reference projection set and simulated projection set using NCC
    ASSERT_NEAR(100 * ideal_detector_gate.computeNCC(ideal_detector_gVXR), 100.0, 2);
    ASSERT_NEAR(100 * CsI_0_6_gate.computeNCC(CsI_0_6_gVXR), 100.0, 2);
    ASSERT_NEAR(100 * CsI_1_2_gate.computeNCC(CsI_1_2_gVXR), 100.0, 2);
    ASSERT_NEAR(100 * Gadox_0_6_gate.computeNCC(Gadox_0_6_gVXR), 100.0, 2);
    ASSERT_NEAR(100 * Gadox_1_2_gate.computeNCC(Gadox_1_2_gVXR), 100.0, 2);

    // // Compare reference projection set and simulated projection set using RMSE
    ASSERT_NEAR(ideal_detector_gate.computeRMSE(ideal_detector_gVXR), 0.0, ideal_detector_gate.getStandardDeviation());
    ASSERT_NEAR(CsI_0_6_gate.computeRMSE(CsI_0_6_gVXR), 0.0, CsI_0_6_gate.getStandardDeviation());
    ASSERT_NEAR(CsI_1_2_gate.computeRMSE(CsI_1_2_gVXR), 0.0, CsI_1_2_gate.getStandardDeviation());
    ASSERT_NEAR(Gadox_0_6_gate.computeRMSE(Gadox_0_6_gVXR), 0.0, Gadox_0_6_gate.getStandardDeviation());
    ASSERT_NEAR(Gadox_1_2_gate.computeRMSE(Gadox_1_2_gVXR), 0.0, Gadox_1_2_gate.getStandardDeviation());

    // Close the window and shut EGL
    quit();
}


//---------------------------------------
TEST(TestScintilator, TestLookUpTableCsI)
//---------------------------------------
{
    const std::string& mat = "CsI";
    // double rho = 4.51; // g.cm^-3
    // double thickness = 0.06; // cm

    Scintillator scintillator;
    scintillator.setMaterial(mat);
    scintillator.setThickness(0.06 * cm);

    std::vector<std::pair<RATIONAL_NUMBER, RATIONAL_NUMBER> > response = scintillator.getEnergyResponse();

    for (std::vector<std::pair<RATIONAL_NUMBER, RATIONAL_NUMBER> >::iterator ite = response.begin();
        ite != response.end();
        ++ite)
    {
        ite->first /= keV;
        ite->second /= keV;
    }

    runLookTableTest("data/responseCsIDetector.tsv", response);
}


////-----------------------------------------
//TEST(TestScintilator, TestLookUpTableGdO2S)
////-----------------------------------------
//{
//    const std::string& mat = "Gd2O2S";
//    double rho = 7.34; // g.cm^-3
//    double thickness = 0.06; // cm
//
//    Scintillator scintillator;
//    scintillator.setMaterial(mat);
//    scintillator.setThickness(0.06 * cm);
//
//    std::vector<std::pair<RATIONAL_NUMBER, RATIONAL_NUMBER> > response = scintillator.getEnergyResponse();
//
//    for (std::vector<std::pair<RATIONAL_NUMBER, RATIONAL_NUMBER> >::iterator ite = response.begin();
//        ite != response.end();
//        ++ite)
//    {
//        ite->first /= keV;
//        ite->second /= keV;
//    }
//
//    runLookTableTest("data/responseGdO2SDetector.tsv", response);
//}


//-----------------------------------------------
double getSum(const std::vector<double>& aVector)
//-----------------------------------------------
{
    return (std::accumulate(aVector.begin(), aVector.end(), 0.0));
}


//---------------------------------------------------
double getAverage(const std::vector<double>& aVector)
//---------------------------------------------------
{
    return (getSum(aVector) / double(aVector.size()));
}


//----------------------------------------------------
double getVariance(const std::vector<double>& aVector)
//----------------------------------------------------
{
    double variance(0.0);
    double average(getAverage(aVector));

    for (std::vector<double>::const_iterator ite(aVector.begin());
            ite != aVector.end();
            ++ite)
    {
        variance += std::pow(*ite - average, 2);
    }

    return (variance / double(aVector.size()));
}


//--------------------------------------------------
double getStdDev(const std::vector<double>& aVector)
//--------------------------------------------------
{
    return (std::sqrt(getVariance(aVector)));
}


//------------------------------------------------
double getNCC(const std::vector<double>& aVector1,
              const std::vector<double>& aVector2)
//------------------------------------------------
{
    double ncc(0.0);

    double average1(getAverage(aVector1));
    double average2(getAverage(aVector2));

    double std_dev(getStdDev(aVector1) * getStdDev(aVector2));

    std::vector<double>::const_iterator ite1(aVector1.begin());
    std::vector<double>::const_iterator ite2(aVector2.begin());

    while (ite1 != aVector1.end())
    {
        ncc += (*ite1 - average1) * (*ite2 - average2);
        ++ite1;
        ++ite2;
    }

    ncc /= std_dev;
    ncc /= aVector1.size();

    return (ncc);
}


//----------------------------------------------------------
double maxRelativeError(const std::vector<double>& aVector1,
                        const std::vector<double>& aVector2)
//----------------------------------------------------------
{
    double max_relative_error(0.0);
    double relative_error(0.0);

    std::vector<double>::const_iterator ite1(aVector1.begin());
    std::vector<double>::const_iterator ite2(aVector2.begin());

    while (ite1 != aVector1.end())
    {
        relative_error = std::abs(*ite1 - *ite2) / *ite1;
        max_relative_error = std::max(relative_error, max_relative_error);

        ++ite1;
        ++ite2;
    }

    return (max_relative_error);
}


//-----------------------------------------------------------
double meanRelativeError(const std::vector<double>& aVector1,
                         const std::vector<double>& aVector2)
//-----------------------------------------------------------
{
    double mean_relative_error(0.0);

    std::vector<double>::const_iterator ite1(aVector1.begin());
    std::vector<double>::const_iterator ite2(aVector2.begin());

    while (ite1 != aVector1.end())
    {
        mean_relative_error += std::abs(*ite1 - *ite2) / *ite1;

        ++ite1;
        ++ite2;
    }

    return (mean_relative_error / aVector1.size());
}
