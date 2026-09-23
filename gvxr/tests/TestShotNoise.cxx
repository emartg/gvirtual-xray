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
*   @file       TestShotNoise.cxx
*
*   @brief      Unit testing: shot noise:
*               Assess our own implementation of the Poisson noise.
*
*   @date       29/08/2025
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
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, Aug 2025-present, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <cmath>
#include <iostream>
#include <iomanip>
#include <memory>
#include <cstdlib>

#include "gtest/gtest.h"

#ifndef GVXR_TYPES_H
#include "gVirtualXRay/Types.h"
#endif

#ifndef GVXR_UNITS_H
#include "gVirtualXRay/Units.h"
#endif

#ifndef GVXR_SCENEGRAPH_NODE_H
#include "gVirtualXRay/SceneGraphNode.h"
#endif

#ifndef GVXR_CYLINDER_MESH_H
#include "gVirtualXRay/CylinderMesh.h"
#endif

#ifndef GVXR_CUBE_MESH_H
#include "gVirtualXRay/CubeMesh.h"
#endif

#ifndef GVXR_XRAY_BEAM_H
#include "gVirtualXRay/XRayBeam.h"
#endif

#ifndef GVXR_XRAY_DETECTOR_H
#include "gVirtualXRay/XRayDetector.h"
#endif

#ifndef OpenGLUtilities_h
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef GVXR_MATRIX_4X4_H
#include "gVirtualXRay/Matrix4x4.h"
#endif

#ifndef GVXR_XRAY_RENDERER_H
#include "gVirtualXRay/XRayRenderer.h"
#endif

#ifndef GVXR_SHADER_H
#include "gVirtualXRay/Shader.h"
#endif

#ifndef GVXR_UTILITIES_H
#include "gVirtualXRay/Utilities.h"
#endif

#ifndef GVXR_EXCEPTION_H
#include "gVirtualXRay/Exception.h"
#endif

#ifndef GVXR_utils_h
#include "utils.h"
#endif


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//  Constant variables
//******************************************************************************


//******************************************************************************
//  Global variables
//******************************************************************************
Context g_GL_context;

Matrix4x4<GLfloat> g_scene_rotation_matrix;
Matrix4x4<GLfloat> g_detector_rotation_matrix;
Matrix4x4<GLfloat> g_sample_rotation_matrix;

GLfloat g_incident_energy(80.0f * keV);
constexpr GLfloat g_scaling_factor(2.0f);
GLfloat g_resolution(0.15f * mm * g_scaling_factor); // 1mm per pixel
Vec2ui g_number_of_pixels(600 / g_scaling_factor, 600 / g_scaling_factor);
VEC3 g_source_position(0.0f, 0.0f, -10.0f * cm);
VEC3 g_detector_position(0.0f, 0.0f, 10.0f * cm);
VEC3 g_detector_up_vector(0.0f, 1.0f, 0.0f);

SceneGraphNode g_scene_graph("root", nullptr);

XRayBeam g_xray_beam;
XRayDetector g_xray_detector;
XRayRenderer g_xray_renderer;


//******************************************************************************
//  Function declaration
//******************************************************************************
void loadSTLFile();
void loadDetector();
void loadSource();
void loadXRaySimulator();
void errorCallback(int error, const char* description);
void quit();


//------------------------------------
TEST(TestShotNoise, TestShotNoise)
//------------------------------------
{
    // Make sure the detector has an odd number of columns
    if ((g_number_of_pixels.getX() % 2) == 0)
    {
        g_number_of_pixels.setX(g_number_of_pixels.getX() + 1);
    }

    // Make sure the detector has an odd number of rows
    if ((g_number_of_pixels.getY() % 2) == 0)
    {
        g_number_of_pixels.setY(g_number_of_pixels.getY() + 1);
    }

    // Register the exit callback
    atexit(quit);

    // Initialise OpenGL
    initGL(g_GL_context);

    // Load the data
    loadDetector();
    loadSource();
    loadXRaySimulator();

    // Add the geometry to the X-ray renderer
    loadSTLFile();
    g_xray_renderer.addInnerSurface(&g_scene_graph.getChild("inside"));
    g_xray_renderer.addOuterSurface(&g_scene_graph.getChild("outside"));

    std::vector<float> p_lambda_cutoff_set = { 49, 50, 51, 52 };
    std::vector<unsigned int> p_photon_number_set = { 49, 50, 51, 55, 65, 70, 500, 5000 };

    for (auto ite_lambda_cutoff : p_lambda_cutoff_set)
    {
        g_xray_beam.setLambdaCutoff(ite_lambda_cutoff);
        LOGGER.logNow("Use lambda cutoff value: ") << ite_lambda_cutoff << std::endl;

        for (auto ite_photon_number : p_photon_number_set)
        {
            LOGGER.logNow("Use ") << ite_photon_number << " photons per pixel" << std::endl;

            // Compute the projection set
            g_xray_beam.disablePoissonNoise();
            g_xray_beam.setNumberOfPhotonsPerPixelAtSDD(ite_photon_number, false);
            g_xray_renderer.computeImage(g_sample_rotation_matrix);

            // Load reference image
            Image<XRayRenderer::PixelType> image_fluence_no_noise = g_xray_renderer.getEnergyFluence();
            //image_fluence_no_noise.saveMHD("TestShotNoise-fluence-no_noise.mhd");

            Image<XRayRenderer::PixelType> image_photon_count_no_noise = image_fluence_no_noise / g_incident_energy;
            //image_photon_count_no_noise.saveMHD("TestShotNoise-photon_count-no_noise.mhd");

            g_xray_beam.enablePoissonNoise();
            g_xray_renderer.computeImage(g_sample_rotation_matrix);

            Image<XRayRenderer::PixelType> image_fluence_noise = g_xray_renderer.getEnergyFluence();
            //image_fluence_noise.saveMHD("TestShotNoise-fluence-noise.mhd");

            std::random_device rd;
            std::mt19937 gen(rd());

            Image<XRayRenderer::PixelType> image_photon_count_noise = image_photon_count_no_noise;

            for (size_t j = 0; j < image_photon_count_noise.getHeight(); ++j)
                for (size_t i = 0; i < image_photon_count_noise.getWidth(); ++i)
                {
                    unsigned int photon_number = image_photon_count_noise.getPixel(i, j, 0);
                    std::poisson_distribution<> d(photon_number);
                    image_photon_count_noise.setPixel(i, j, 0, d(gen));
                }

            Image<XRayRenderer::PixelType> image_fluence_noise_cpp = image_photon_count_noise * g_incident_energy;
            //image_fluence_noise_cpp.saveMHD("TestShotNoise-fluence-noise-C++.mhd");

            // Average value
            double reference_value = image_fluence_noise_cpp.getAverage();
            double test_value = image_fluence_noise.getAverage();
            double relative_error = 100.0 * std::abs(reference_value - test_value) / reference_value;
            double absolute_error = std::abs(reference_value - test_value);
            ASSERT_EQ(relative_error < 5 || absolute_error < 1, true) <<
                "Test: average value:" << std::endl <<
                "\tthe reference value is: " << reference_value << "; " << std::endl <<
                "\tthe tested value is: " << test_value << "; " << std::endl <<
                "\tthe relative error is: " << relative_error << "%;" << std::endl <<
                "\tthe absolute error is: " << absolute_error << "." << std::endl;

            // Stddev value
            reference_value = image_fluence_noise_cpp.getStandardDeviation();
            test_value = image_fluence_noise.getStandardDeviation();
            relative_error = 100.0 * std::abs(reference_value - test_value) / reference_value;
            absolute_error = std::abs(reference_value - test_value);
            ASSERT_EQ(relative_error < 5 || absolute_error < 1, true) <<
                "Test: standard deviation:" << std::endl <<
                "\tthe number of photons per pixel is: " << ite_photon_number << "; " << std::endl <<
                "\tthe lambda cutoff is: " << g_xray_beam.getLambdaCutoff() << "; " << std::endl <<
                "\tthe reference value is: " << reference_value << "; " << std::endl <<
                "\tthe tested value is: " << test_value << "; " << std::endl <<
                "\tthe relative error is: " << relative_error << "%;" << std::endl <<
                "\tthe absolute error is: " << absolute_error << "." << std::endl;

            // Max value
            reference_value = image_fluence_noise_cpp.getMaxValue();
            test_value = image_fluence_noise.getMaxValue();
            relative_error = 100.0 * std::abs(reference_value - test_value) / reference_value;
            absolute_error = std::abs(reference_value - test_value);
            ASSERT_EQ(relative_error < 5 || absolute_error < 1.5, true) <<
                "Test: max value:" << std::endl <<
                "\tthe reference value is: " << reference_value << "; " << std::endl <<
                "\tthe tested value is: " << test_value << "; " << std::endl <<
                "\tthe relative error is: " << relative_error << "%;" << std::endl <<
                "\tthe absolute error is: " << absolute_error << "." << std::endl;
        }
    }

    // Close the window and shut EGL
    quit();
}


//---------
void quit()
//---------
{
    g_xray_renderer.release();
    g_scene_graph.clear();
    g_GL_context.release();
}


//----------------
void loadSTLFile()
//----------------
{
    // Set the geometry
    g_scene_graph.addChild("inside", CylinderMesh<GLdouble>(20, 3.0 * cm, 1.0 * cm));
    g_scene_graph.addChild("outside", CubeMesh<GLdouble>(3.0 * cm));

    // HU values
    RATIONAL_NUMBER HU_bone(1329.885683);
    RATIONAL_NUMBER HU_soft_tissue(51.714752);

    g_scene_graph.getChild("inside").getPolygonMesh().setHounsfieldValue(HU_bone);
    g_scene_graph.getChild("outside").getPolygonMesh().setHounsfieldValue(HU_soft_tissue);

    g_scene_graph.getChild("outside").getPolygonMesh().splitFaces(false);
    g_scene_graph.getChild("outside").getPolygonMesh().splitFaces(false);
    g_scene_graph.getChild("outside").getPolygonMesh().splitFaces(false);
    g_scene_graph.getChild("outside").getPolygonMesh().splitFaces(false);
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

    // Set the source position
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
