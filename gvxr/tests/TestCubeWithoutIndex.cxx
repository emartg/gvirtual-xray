/*

Copyright (c) 2019-2023, Dr Franck P. Vidal, Bangor University, All rights reserved.
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
*   @file       TestCube.cxx
*
*   @brief      Unit testing: Test the implementation of CubeMesh.
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
*   (c) by Dr Franck P. Vidal, Bangor University, Apr 2019-Sept 2023, version 1.1, BSD 3-Clause License
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, Oct 2023-present, version 1.1, BSD 3-Clause License
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

#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#ifndef GVXR_TYPES_H
#include "gVirtualXRay/Types.h"
#endif

#ifndef GVXR_LOGGER_H
#include "gVirtualXRay/Logger.h"
#endif

#ifndef GVXR_UNITS_H
#include "gVirtualXRay/Units.h"
#endif

#ifndef GVXR_SCENEGRAPH_NODE_H
#include "gVirtualXRay/SceneGraphNode.h"
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

#ifndef GVXR_LOGGER_H
#include "gVirtualXRay/Logger.h"
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

Matrix4x4<GLfloat> g_scene_rotation_matrix;
Matrix4x4<GLfloat> g_detector_rotation_matrix;
Matrix4x4<GLfloat> g_sample_rotation_matrix;

GLfloat g_incident_energy(80.0 * keV);
const GLfloat g_scaling_factor(2.0);
GLfloat g_resolution(0.15 * mm * g_scaling_factor); // 1mm per pixel
Vec2ui g_number_of_pixels(600 / g_scaling_factor, 600 / g_scaling_factor);
VEC3 g_source_position(   0.0, 0.0, -10.0 * cm);
VEC3 g_detector_position( 0.0, 0.0,  10.0 * cm);
VEC3 g_detector_up_vector(0.0, 1.0,   0.0);

RATIONAL_NUMBER g_cube_side_in_cm(3.0);

SceneGraphNode g_scene_graph("root", NULL);

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


//--------------------------------------
TEST(TestCubeWithoutIndex, TestLBuffer1)
//--------------------------------------
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
    g_xray_renderer.addOuterSurface(&g_scene_graph.getChild("outside"));

    // Enable artefact filtering on GPU
    g_xray_renderer.enableArtifactFilteringOnGPU();

    // Compute the X-ray image
    g_xray_renderer.computeLBuffer(g_scene_graph.getChild("outside"),
                                                  VEC3(),
                                                  g_sample_rotation_matrix);
    // g_xray_renderer.printLBuffer("cube1_full_precision_renderer.mha");


    RATIONAL_NUMBER central_pixel(g_xray_renderer.getLBuffer().getPixel(g_number_of_pixels.getX
        () / 2, g_number_of_pixels.getY() / 2));

    RATIONAL_NUMBER expected_value = g_cube_side_in_cm;
    RATIONAL_NUMBER simulated_result(central_pixel);

#ifdef HAS_OPENGL
    LOGGER.logNow("") << "With OpenGL, the L-Buffer (in mm) computed orthogonally throw the middle of cube is:" <<
        std::endl;
#else
    LOGGER.logNow("") << "with OpenMP, the L-Buffer (in mm) computed orthogonally throw the middle of cube is:" <<
        std::endl;
#endif

    LOGGER.logNow("") << "\t\t           = " << simulated_result * cm / mm<< " mm" << std::endl << std::endl;

    LOGGER.logNow("") << "The absolute error is: |I_expected - I_simulated| cm" << std::endl;
    LOGGER.logNow("") << "                     : |" << expected_value * cm / mm << " - " << simulated_result * cm / mm << "| mm" << std::endl;
    LOGGER.logNow("") << "                     : " << std::fixed << std::abs(expected_value - simulated_result) * cm / mm << " mm" << std::endl << std::endl;

    LOGGER.logNow("") << "The relative error is: |I_expected - I_simulated| / I_expected" << std::endl;
    LOGGER.logNow("") << "                     : " << std::fixed << 100.0 * std::abs(expected_value - simulated_result) / expected_value << "%" << std::endl;

    ASSERT_NEAR(expected_value, simulated_result, 1e-9 / expected_value);
}


//--------------------------------------
TEST(TestCubeWithoutIndex, TestLBuffer2)
//--------------------------------------
{
    // Rotate by 90 deg
    g_sample_rotation_matrix = Matrix4x4<GLfloat>();
    g_sample_rotation_matrix.rotate(90.0, VEC3(1, 0, 0));

    // Compute the X-ray image
    g_xray_renderer.computeLBuffer(g_scene_graph.getChild("outside"),
                                                  VEC3(),
                                                  g_sample_rotation_matrix);

    RATIONAL_NUMBER central_pixel(g_xray_renderer.getLBuffer().getPixel(g_number_of_pixels.getX() / 2, g_number_of_pixels.getY() / 2));

    RATIONAL_NUMBER expected_value = g_cube_side_in_cm;
    RATIONAL_NUMBER simulated_result(central_pixel);

#ifdef HAS_OPENGL
    LOGGER.logNow("") << "With OpenGL, the L-Buffer (in mm) computed orthogonally throw the middle of cube is:" <<
        std::endl;
#else
    LOGGER.logNow("") << "with OpenMP, the L-Buffer (in mm) computed orthogonally throw the middle of cube is:" <<
        std::endl;
#endif
    LOGGER.logNow("") << "\t\t           = " << simulated_result * cm / mm<< " mm" << std::endl << std::endl;

    LOGGER.logNow("") << "The absolute error is: |I_expected - I_simulated| cm" << std::endl;
    LOGGER.logNow("") << "                     : |" << expected_value * cm / mm << " - " << simulated_result * cm / mm << "| mm" << std::endl;
    LOGGER.logNow("") << "                     : " << std::fixed << std::abs(expected_value - simulated_result) * cm / mm << " mm" << std::endl << std::endl;

    LOGGER.logNow("") << "The relative error is: |I_expected - I_simulated| / I_expected" << std::endl;
    LOGGER.logNow("") << "                     : " << std::fixed << 100.0 * std::abs(expected_value - simulated_result) / expected_value << "%" << std::endl;

    ASSERT_NEAR(expected_value, simulated_result, 1e-9 / expected_value);
}


//--------------------------------------
TEST(TestCubeWithoutIndex, TestLBuffer3)
//--------------------------------------
{
    // Rotate by 90 deg
    g_sample_rotation_matrix = Matrix4x4<GLfloat>();
    g_sample_rotation_matrix.rotate(90.0, VEC3(-1, 0, 0));

    // Compute the X-ray image
    g_xray_renderer.computeLBuffer(g_scene_graph.getChild("outside"),
                                                  VEC3(),
                                                  g_sample_rotation_matrix);

    RATIONAL_NUMBER central_pixel(g_xray_renderer.getLBuffer().getPixel(g_number_of_pixels.getX() / 2, g_number_of_pixels.getY() / 2));

    RATIONAL_NUMBER expected_value = g_cube_side_in_cm;
    RATIONAL_NUMBER simulated_result(central_pixel);

#ifdef HAS_OPENGL
    LOGGER.logNow("") << "With OpenGL, the L-Buffer (in mm) computed orthogonally throw the middle of cube is:" <<
        std::endl;
#else
    LOGGER.logNow("") << "with OpenMP, the L-Buffer (in mm) computed orthogonally throw the middle of cube is:" <<
        std::endl;
#endif
    LOGGER.logNow("") << "\t\t           = " << simulated_result * cm / mm<< " mm" << std::endl << std::endl;

    LOGGER.logNow("") << "The absolute error is: |I_expected - I_simulated| cm" << std::endl;
    LOGGER.logNow("") << "                     : |" << expected_value * cm / mm << " - " << simulated_result * cm / mm << "| mm" << std::endl;
    LOGGER.logNow("") << "                     : " << std::fixed << std::abs(expected_value - simulated_result) * cm / mm << " mm" << std::endl << std::endl;

    LOGGER.logNow("") << "The relative error is: |I_expected - I_simulated| / I_expected" << std::endl;
    LOGGER.logNow("") << "                     : " << std::fixed << 100.0 * std::abs(expected_value - simulated_result) / expected_value << "%" << std::endl;

    ASSERT_NEAR(expected_value, simulated_result, 1e-9 / expected_value);
}


//--------------------------------------
TEST(TestCubeWithoutIndex, TestLBuffer4)
//--------------------------------------
{
    // Rotate by 90 deg
    g_sample_rotation_matrix = Matrix4x4<GLfloat>();
    g_sample_rotation_matrix.rotate(90.0, VEC3(0, 1, 0));

    // Compute the X-ray image
    g_xray_renderer.computeLBuffer(g_scene_graph.getChild("outside"),
                                                  VEC3(),
                                                  g_sample_rotation_matrix);

    RATIONAL_NUMBER central_pixel(g_xray_renderer.getLBuffer().getPixel(g_number_of_pixels.getX() / 2, g_number_of_pixels.getY() / 2));

    RATIONAL_NUMBER expected_value = g_cube_side_in_cm;
    RATIONAL_NUMBER simulated_result(central_pixel);

#ifdef HAS_OPENGL
    LOGGER.logNow("") << "With OpenGL, the L-Buffer (in mm) computed orthogonally throw the middle of cube is:" <<
        std::endl;
#else
    LOGGER.logNow("") << "with OpenMP, the L-Buffer (in mm) computed orthogonally throw the middle of cube is:" <<
        std::endl;
#endif
    LOGGER.logNow("") << "\t\t           = " << simulated_result * cm / mm<< " mm" << std::endl << std::endl;

    LOGGER.logNow("") << "The absolute error is: |I_expected - I_simulated| cm" << std::endl;
    LOGGER.logNow("") << "                     : |" << expected_value * cm / mm << " - " << simulated_result * cm / mm << "| mm" << std::endl;
    LOGGER.logNow("") << "                     : " << std::fixed << std::abs(expected_value - simulated_result) * cm / mm << " mm" << std::endl << std::endl;

    LOGGER.logNow("") << "The relative error is: |I_expected - I_simulated| / I_expected" << std::endl;
    LOGGER.logNow("") << "                     : " << std::fixed << 100.0 * std::abs(expected_value - simulated_result) / expected_value << "%" << std::endl;

    ASSERT_NEAR(expected_value, simulated_result, 1e-9 / expected_value);
}


//--------------------------------------
TEST(TestCubeWithoutIndex, TestLBuffer5)
//--------------------------------------
{
    // Rotate by 90 deg
    g_sample_rotation_matrix = Matrix4x4<GLfloat>();
    g_sample_rotation_matrix.rotate(90.0, VEC3(0, -1, 0));

    // Compute the X-ray image
    g_xray_renderer.computeLBuffer(g_scene_graph.getChild("outside"),
                                                  VEC3(),
                                                  g_sample_rotation_matrix);

    RATIONAL_NUMBER central_pixel(g_xray_renderer.getLBuffer().getPixel(g_number_of_pixels.getX() / 2, g_number_of_pixels.getY() / 2));

    RATIONAL_NUMBER expected_value = g_cube_side_in_cm;
    RATIONAL_NUMBER simulated_result(central_pixel);

#ifdef HAS_OPENGL
    LOGGER.logNow("") << "With OpenGL, the L-Buffer (in mm) computed orthogonally throw the middle of cube is:" <<
        std::endl;
#else
    LOGGER.logNow("") << "with OpenMP, the L-Buffer (in mm) computed orthogonally throw the middle of cube is:" <<
        std::endl;
#endif
    LOGGER.logNow("") << "\t\t           = " << simulated_result * cm / mm<< " mm" << std::endl << std::endl;

    LOGGER.logNow("") << "The absolute error is: |I_expected - I_simulated| cm" << std::endl;
    LOGGER.logNow("") << "                     : |" << expected_value * cm / mm << " - " << simulated_result * cm / mm << "| mm" << std::endl;
    LOGGER.logNow("") << "                     : " << std::fixed << std::abs(expected_value - simulated_result) * cm / mm << " mm" << std::endl << std::endl;

    LOGGER.logNow("") << "The relative error is: |I_expected - I_simulated| / I_expected" << std::endl;
    LOGGER.logNow("") << "                     : " << std::fixed << 100.0 * std::abs(expected_value - simulated_result) / expected_value << "%" << std::endl;

    ASSERT_NEAR(expected_value, simulated_result, 1e-9 / expected_value);
}

//--------------------------------------
TEST(TestCubeWithoutIndex, TestLBuffer6)
//--------------------------------------
{
    // Rotate by 90 deg
    g_sample_rotation_matrix = Matrix4x4<GLfloat>();
    g_sample_rotation_matrix.rotate(90.0, VEC3(0, 0, 1));

    // Compute the X-ray image
    g_xray_renderer.computeLBuffer(g_scene_graph.getChild("outside"),
                                                  VEC3(),
                                                  g_sample_rotation_matrix);

    RATIONAL_NUMBER central_pixel(g_xray_renderer.getLBuffer().getPixel(g_number_of_pixels.getX() / 2, g_number_of_pixels.getY() / 2));

    RATIONAL_NUMBER expected_value = g_cube_side_in_cm;
    RATIONAL_NUMBER simulated_result(central_pixel);

#ifdef HAS_OPENGL
    LOGGER.logNow("") << "With OpenGL, the L-Buffer (in mm) computed orthogonally throw the middle of cube is:" <<
        std::endl;
#else
    LOGGER.logNow("") << "with OpenMP, the L-Buffer (in mm) computed orthogonally throw the middle of cube is:" <<
        std::endl;
#endif
    LOGGER.logNow("") << "\t\t           = " << simulated_result * cm / mm<< " mm" << std::endl << std::endl;

    LOGGER.logNow("") << "The absolute error is: |I_expected - I_simulated| cm" << std::endl;
    LOGGER.logNow("") << "                     : |" << expected_value * cm / mm << " - " << simulated_result * cm / mm << "| mm" << std::endl;
    LOGGER.logNow("") << "                     : " << std::fixed << std::abs(expected_value - simulated_result) * cm / mm << " mm" << std::endl << std::endl;

    LOGGER.logNow("") << "The relative error is: |I_expected - I_simulated| / I_expected" << std::endl;
    LOGGER.logNow("") << "                     : " << std::fixed << 100.0 * std::abs(expected_value - simulated_result) / expected_value << "%" << std::endl;

    ASSERT_NEAR(expected_value, simulated_result, 1e-9 / expected_value);
}


//--------------------------------------
TEST(TestCubeWithoutIndex, TestLBuffer7)
//--------------------------------------
{
    // Rotate by 90 deg
    g_sample_rotation_matrix = Matrix4x4<GLfloat>();
    g_sample_rotation_matrix.rotate(90.0, VEC3(0, 0, -1));

    // Compute the X-ray image
    g_xray_renderer.computeLBuffer(g_scene_graph.getChild("outside"),
                                                  VEC3(),
                                                  g_sample_rotation_matrix);


    RATIONAL_NUMBER central_pixel(g_xray_renderer.getLBuffer().getPixel(g_number_of_pixels.getX() / 2, g_number_of_pixels.getY() / 2));

    RATIONAL_NUMBER expected_value = g_cube_side_in_cm;
    RATIONAL_NUMBER simulated_result(central_pixel);

#ifdef HAS_OPENGL
    LOGGER.logNow("") << "With OpenGL, the L-Buffer (in mm) computed orthogonally throw the middle of cube is:" <<
        std::endl;
#else
    LOGGER.logNow("") << "with OpenMP, the L-Buffer (in mm) computed orthogonally throw the middle of cube is:" <<
        std::endl;
#endif
    LOGGER.logNow("") << "\t\t           = " << simulated_result * cm / mm<< " mm" << std::endl << std::endl;

    LOGGER.logNow("") << "The absolute error is: |I_expected - I_simulated| cm" << std::endl;
    LOGGER.logNow("") << "                     : |" << expected_value * cm / mm << " - " << simulated_result * cm / mm << "| mm" << std::endl;
    LOGGER.logNow("") << "                     : " << std::fixed << std::abs(expected_value - simulated_result) * cm / mm << " mm" << std::endl << std::endl;

    LOGGER.logNow("") << "The relative error is: |I_expected - I_simulated| / I_expected" << std::endl;
    LOGGER.logNow("") << "                     : " << std::fixed << 100.0 * std::abs(expected_value - simulated_result) / expected_value << "%" << std::endl;

    ASSERT_NEAR(expected_value, simulated_result, 1e-9 / expected_value);
}


//--------------------------------------
TEST(TestCubeWithoutIndex, TestLBuffer8)
//--------------------------------------
{
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
    g_scene_graph.addChild("outside", CubeMesh<GLdouble>(g_cube_side_in_cm * cm));

    // HU values
    RATIONAL_NUMBER HU_soft_tissue(51.714752);

    g_scene_graph.getChild("outside").getPolygonMesh().setHounsfieldValue(HU_soft_tissue);

    LOGGER.logNow("") << "We run the simulation as follows:" << std::endl;
    LOGGER.logNow("") << "- A cube, which has edge length of 3 cm, is made of soft tissue (HU = " << std::floor(HU_soft_tissue + 0.5) << "). " << std::endl;
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

    // Use a parallel source
    g_xray_detector.setParallelBeam();
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
