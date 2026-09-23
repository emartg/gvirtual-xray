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
*   @file       TestLBuffer.cxx
*
*   @brief      Unit testing: Test the L-buffer calculations,
*   			the unit test corresponds to validation_??,
*   			last date of check: Passed test on 01 Apr 2019.
*
*   @version    1.0
*
*   @date       01/04/2019
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

#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#define GLFW_INCLUDE_GLCOREARB 1
#include <GLFW/glfw3.h>

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

#ifndef GVXR_LOGGER_H
#include "gVirtualXRay/Logger.h"
#endif


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;
//using namespace std;


//******************************************************************************
//  Global variables
//******************************************************************************
GLsizei g_main_window_width(1280 / 2.0);
GLsizei g_main_window_height(800 / 2.0);
GLFWwindow* g_p_main_window_id(0);

Matrix4x4<GLfloat> g_scene_rotation_matrix;
Matrix4x4<GLfloat> g_detector_rotation_matrix;
Matrix4x4<GLfloat> g_sample_rotation_matrix;

GLfloat g_incident_energy(80.0 * keV);
const GLfloat g_scaling_factor(2.0);
GLfloat g_resolution(0.15 * mm * g_scaling_factor); // 1mm per pixel
Vec2ui g_number_of_pixels(600 / g_scaling_factor, 1);
VEC3 g_source_position(   0.0, 0.0, -10.0 * cm);
VEC3 g_detector_position( 0.0, 0.0,  10.0 * cm);
VEC3 g_detector_up_vector(0.0, 1.0,   0.0);

RATIONAL_NUMBER g_cube_side_in_cm(3.0);
RATIONAL_NUMBER g_cylinder_height_in_cm(3.0);
RATIONAL_NUMBER g_cylinder_radius_in_cm(1.0);

SceneGraphNode g_scene_graph("root", NULL);

XRayBeam g_xray_beam;
XRayDetector g_xray_detector;
XRayRenderer g_xray_half_precision_renderer;
XRayRenderer g_xray_full_precision_renderer;


//******************************************************************************
//  Function declaration
//******************************************************************************
void initGL();
void loadSTLFile();
void loadDetector();
void loadSource();
void loadXRaySimulator();
void errorCallback(int error, const char* description);
void quit();


//------------------------------
TEST (TestLBuffer, TestLBuffer1)
//------------------------------
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

    // Set an error callback
    glfwSetErrorCallback(errorCallback);

    // Initialize GLFW
    ASSERT_EQ(glfwInit(), GLFW_TRUE) << "Cannot initialise GLFW.";

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, false);

    // Enable anti-aliasing
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Create a windowed mode window and its OpenGL context
    g_p_main_window_id = glfwCreateWindow(g_main_window_width,
        g_main_window_height,
        "gVirtualXRay -- Unit testing: Beer-Lambert Law, Monochromatism Case.",
        0,
        0);

    // Window cannot be created
    if (!g_p_main_window_id)
        {
        glfwTerminate();
        }
    ASSERT_TRUE(g_p_main_window_id) << "Cannot create a GLFW windowed mode window and its OpenGL context.";

    // Make the window's context current
    glfwMakeContextCurrent(g_p_main_window_id);

    // Initialise GLAD
    initialiseGLAD("OPENGL");

    // Initialise OpenGL
    initGL();

    // Check the current FBO
    checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Load the data
    loadDetector();
    loadSource();
    loadXRaySimulator();

    // Add the geometry to the X-ray renderer
    loadSTLFile();
    g_xray_half_precision_renderer.addInnerSurface(&g_scene_graph.getChild("inside"));
    g_xray_half_precision_renderer.addOuterSurface(&g_scene_graph.getChild("outside"));
    g_xray_full_precision_renderer.addInnerSurface(&g_scene_graph.getChild("inside"));
    g_xray_full_precision_renderer.addOuterSurface(&g_scene_graph.getChild("outside"));

    // Set the projection matrix
    GLint width(0);
    GLint height(0);
    glfwGetFramebufferSize(g_p_main_window_id, &width, &height);

    // Enable artefact filtering on GPU
    g_xray_half_precision_renderer.enableArtifactFilteringOnGPU();
    g_xray_full_precision_renderer.enableArtifactFilteringOnGPU();

    // Compute the X-ray image
    g_xray_half_precision_renderer.computeLBuffer(g_scene_graph.getChild("outside"),
                                                  VEC3(),
                                                  g_sample_rotation_matrix);

    g_xray_full_precision_renderer.computeLBuffer(g_scene_graph.getChild("outside"),
                                                  VEC3(),
                                                  g_sample_rotation_matrix);

    g_xray_half_precision_renderer.printLBuffer("g_xray_half_precision_renderer.txt");
    g_xray_full_precision_renderer.printLBuffer("g_xray_full_precision_renderer.txt");


    RATIONAL_NUMBER* l_buffer(
        g_xray_half_precision_renderer.getLBuffer().getRawData());

    RATIONAL_NUMBER central_pixel(l_buffer[g_number_of_pixels.getX() / 2]);

    RATIONAL_NUMBER expected_value = g_cube_side_in_cm;
    RATIONAL_NUMBER simulated_result(central_pixel);
    ASSERT_FLOAT_EQ(expected_value, simulated_result);

    gVirtualXRay::LOGGER.logNow() << "On GPU, with half precision, the L-Buffer (in mm) computed orthogonally throw the middle of cube is:" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "\t\t           = " << simulated_result * cm / mm<< " mm" << std::endl << std::endl;

    gVirtualXRay::LOGGER.logNow() << "The absolute error is: |I_expected - I_GPU_half_precision| cm" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "                     : |" << expected_value * cm / mm << " - " << simulated_result * cm / mm << "| mm" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "                     : " << std::fixed << std::abs(expected_value - simulated_result) * cm / mm << " mm" << std::endl << std::endl;

    gVirtualXRay::LOGGER.logNow() << "The relative error is: |I_expected - I_GPU_half_precision| / I_expected" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "                     : " << std::fixed << 100.0 * std::abs(expected_value - simulated_result) / expected_value << " \%" << std::endl;

    l_buffer =
            g_xray_full_precision_renderer.getLBuffer().getRawData();

    central_pixel = l_buffer[g_number_of_pixels.getX() / 2];

    simulated_result = central_pixel;
    ASSERT_FLOAT_EQ(expected_value, simulated_result);

    gVirtualXRay::LOGGER.logNow() << "On GPU, with full precision, the L-Buffer (in mm) computed orthogonally throw the middle of cube and cylinder actually is:" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "\t\t           = " << simulated_result * cm / mm << " mm" << std::endl << std::endl;

    gVirtualXRay::LOGGER.logNow() << "The absolute error is: |I_expected - I_GPU_full_precision| cm" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "                     : |" << expected_value * cm / mm << " - " << simulated_result * cm / mm << "| mm" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "                     : " << std::fixed << std::abs(expected_value - simulated_result) * cm / mm << " mm" << std::endl << std::endl;

    gVirtualXRay::LOGGER.logNow() << "The relative error is: |I_expected - I_GPU_half_precision| / I_expected" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "                     : " << std::fixed << 100.0 * std::abs(expected_value - simulated_result) / expected_value << " \%" << std::endl;
}


//------------------------------
TEST (TestLBuffer, TestLBuffer2)
//------------------------------
{
    // Compute the X-ray image
    g_xray_half_precision_renderer.computeLBuffer(g_scene_graph.getChild("inside"),
                                                  VEC3(),
                                                  g_sample_rotation_matrix);

    g_xray_half_precision_renderer.printLBuffer("g_xray_half_precision_renderer.txt");
    g_xray_full_precision_renderer.computeLBuffer(g_scene_graph.getChild("inside"),
                                                  VEC3(),
                                                  g_sample_rotation_matrix);
    g_xray_full_precision_renderer.printLBuffer("g_xray_full_precision_renderer.txt");


    RATIONAL_NUMBER* l_buffer(
                              g_xray_half_precision_renderer.getLBuffer().getRawData());

    RATIONAL_NUMBER central_pixel(l_buffer[g_number_of_pixels.getX() / 2]);

    RATIONAL_NUMBER expected_value = 2.0 * g_cylinder_radius_in_cm;
    RATIONAL_NUMBER simulated_result(central_pixel);
    ASSERT_FLOAT_EQ(expected_value, simulated_result);

    gVirtualXRay::LOGGER.logNow() << "On GPU, with half precision, the L-Buffer (in mm) computed orthogonally throw the middle of cube is:" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "\t\t           = " << simulated_result * cm / mm<< " mm" << std::endl << std::endl;

    gVirtualXRay::LOGGER.logNow() << "The absolute error is: |I_expected - I_GPU_half_precision| cm" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "                     : |" << expected_value * cm / mm << " - " << simulated_result * cm / mm << "| mm" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "                     : " << std::fixed << std::abs(expected_value - simulated_result) * cm / mm << " mm" << std::endl << std::endl;

    gVirtualXRay::LOGGER.logNow() << "The relative error is: |I_expected - I_GPU_half_precision| / I_expected" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "                     : " << std::fixed << 100.0 * std::abs(expected_value - simulated_result) / expected_value << " \%" << std::endl;

    l_buffer =
    g_xray_full_precision_renderer.getLBuffer().getRawData();

    central_pixel = l_buffer[g_number_of_pixels.getX() / 2];

    simulated_result = central_pixel;
    ASSERT_FLOAT_EQ(expected_value, simulated_result);

    gVirtualXRay::LOGGER.logNow() << "On GPU, with full precision, the L-Buffer (in mm) computed orthogonally throw the middle of cube and cylinder actually is:" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "\t\t           = " << simulated_result * cm / mm << " mm" << std::endl << std::endl;

    gVirtualXRay::LOGGER.logNow() << "The absolute error is: |I_expected - I_GPU_full_precision| cm" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "                     : |" << expected_value * cm / mm << " - " << simulated_result * cm / mm << "| mm" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "                     : " << std::fixed << std::abs(expected_value - simulated_result) * cm / mm << " mm" << std::endl << std::endl;

    gVirtualXRay::LOGGER.logNow() << "The relative error is: |I_expected - I_GPU_half_precision| / I_expected" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "                     : " << std::fixed << 100.0 * std::abs(expected_value - simulated_result) / expected_value << " \%" << std::endl;
}


//------------------------------
TEST (TestLBuffer, TestLBuffer3)
//------------------------------
{
    // Rotate by 45%
    g_sample_rotation_matrix.rotate(45.0, g_detector_up_vector);

    // Compute the X-ray image
    g_xray_half_precision_renderer.computeLBuffer(g_scene_graph.getChild("outside"),
                                                  VEC3(),
                                                  g_sample_rotation_matrix);

    g_xray_half_precision_renderer.printLBuffer("g_xray_half_precision_renderer.txt");
    g_xray_full_precision_renderer.computeLBuffer(g_scene_graph.getChild("outside"),
                                                  VEC3(),
                                                  g_sample_rotation_matrix);
    g_xray_full_precision_renderer.printLBuffer("g_xray_full_precision_renderer.txt");


    RATIONAL_NUMBER* l_buffer(
                              g_xray_half_precision_renderer.getLBuffer().getRawData());

    RATIONAL_NUMBER central_pixel(l_buffer[g_number_of_pixels.getX() / 2]);

    RATIONAL_NUMBER expected_value = std::sqrt(2.0 * (g_cube_side_in_cm * g_cube_side_in_cm));
    RATIONAL_NUMBER simulated_result(central_pixel);
    ASSERT_NEAR(expected_value, simulated_result, 0.1 / expected_value);

    gVirtualXRay::LOGGER.logNow() << "On GPU, with half precision, the L-Buffer (in mm) computed orthogonally throw the middle of cube is:" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "\t\t           = " << simulated_result * cm / mm<< " mm" << std::endl << std::endl;

    gVirtualXRay::LOGGER.logNow() << "The absolute error is: |I_expected - I_GPU_half_precision| cm" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "                     : |" << expected_value * cm / mm << " - " << simulated_result * cm / mm << "| mm" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "                     : " << std::fixed << std::abs(expected_value - simulated_result) * cm / mm << " mm" << std::endl << std::endl;

    gVirtualXRay::LOGGER.logNow() << "The relative error is: |I_expected - I_GPU_half_precision| / I_expected" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "                     : " << std::fixed << 100.0 * std::abs(expected_value - simulated_result) / expected_value << " \%" << std::endl;

    l_buffer =
    g_xray_full_precision_renderer.getLBuffer().getRawData();

    central_pixel = l_buffer[g_number_of_pixels.getX() / 2];

    simulated_result = central_pixel;
    ASSERT_FLOAT_EQ(expected_value, simulated_result);


    gVirtualXRay::LOGGER.logNow() << "On GPU, with full precision, the L-Buffer (in mm) computed orthogonally throw the middle of cube and cylinder actually is:" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "\t\t           = " << simulated_result * cm / mm << " mm" << std::endl << std::endl;

    gVirtualXRay::LOGGER.logNow() << "The absolute error is: |I_expected - I_GPU_full_precision| cm" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "                     : |" << expected_value * cm / mm << " - " << simulated_result * cm / mm << "| mm" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "                     : " << std::fixed << std::abs(expected_value - simulated_result) * cm / mm << " mm" << std::endl << std::endl;

    gVirtualXRay::LOGGER.logNow() << "The relative error is: |I_expected - I_GPU_half_precision| / I_expected" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "                     : " << std::fixed << 100.0 * std::abs(expected_value - simulated_result) / expected_value << " \%" << std::endl;
}


//------------------------------
TEST (TestLBuffer, TestLBuffer4)
//------------------------------
{
    // Compute the X-ray image
    g_xray_half_precision_renderer.computeLBuffer(g_scene_graph.getChild("inside"),
                                                  VEC3(),
                                                  g_sample_rotation_matrix);

    g_xray_half_precision_renderer.printLBuffer("g_xray_half_precision_renderer.txt");
    g_xray_full_precision_renderer.computeLBuffer(g_scene_graph.getChild("inside"),
                                                  VEC3(),
                                                  g_sample_rotation_matrix);
    g_xray_full_precision_renderer.printLBuffer("g_xray_full_precision_renderer.txt");


    RATIONAL_NUMBER* l_buffer(
                              g_xray_half_precision_renderer.getLBuffer().getRawData());

    RATIONAL_NUMBER central_pixel(l_buffer[g_number_of_pixels.getX() / 2]);

    RATIONAL_NUMBER expected_value = 2.0 * g_cylinder_radius_in_cm;
    RATIONAL_NUMBER simulated_result(central_pixel);
    ASSERT_NEAR(expected_value, simulated_result, 0.1 / expected_value);

    gVirtualXRay::LOGGER.logNow() << "On GPU, with half precision, the L-Buffer (in mm) computed orthogonally throw the middle of cube is:" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "\t\t           = " << simulated_result * cm / mm<< " mm" << std::endl << std::endl;

    gVirtualXRay::LOGGER.logNow() << "The absolute error is: |I_expected - I_GPU_half_precision| cm" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "                     : |" << expected_value * cm / mm << " - " << simulated_result * cm / mm << "| mm" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "                     : " << std::fixed << std::abs(expected_value - simulated_result) * cm / mm << " mm" << std::endl << std::endl;

    gVirtualXRay::LOGGER.logNow() << "The relative error is: |I_expected - I_GPU_half_precision| / I_expected" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "                     : " << std::fixed << 100.0 * std::abs(expected_value - simulated_result) / expected_value << " \%" << std::endl;

    l_buffer =
    g_xray_full_precision_renderer.getLBuffer().getRawData();

    central_pixel = l_buffer[g_number_of_pixels.getX() / 2];

    simulated_result = central_pixel;
    ASSERT_NEAR(expected_value, simulated_result, 0.1 / expected_value);

    gVirtualXRay::LOGGER.logNow() << "On GPU, with full precision, the L-Buffer (in mm) computed orthogonally throw the middle of cube and cylinder actually is:" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "\t\t           = " << simulated_result * cm / mm << " mm" << std::endl << std::endl;

    gVirtualXRay::LOGGER.logNow() << "The absolute error is: |I_expected - I_GPU_full_precision| cm" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "                     : |" << expected_value * cm / mm << " - " << simulated_result * cm / mm << "| mm" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "                     : " << std::fixed << std::abs(expected_value - simulated_result) * cm / mm << " mm" << std::endl << std::endl;

    gVirtualXRay::LOGGER.logNow() << "The relative error is: |I_expected - I_GPU_half_precision| / I_expected" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "                     : " << std::fixed << 100.0 * std::abs(expected_value - simulated_result) / expected_value << " \%" << std::endl;
}
/*
//------------------------------
TEST (TestLBuffer, TestLBuffer5)
//------------------------------
{
    // Rotate by 45%
    g_sample_rotation_matrix.rotate(45.0, VEC3(0.0, 0.0, 1.0));

    // Compute the X-ray image
    g_xray_half_precision_renderer.computeLBuffer(g_scene_graph.getChild("outside"),
                                                  VEC3(),
                                                  g_sample_rotation_matrix);

    g_xray_half_precision_renderer.printLBuffer("g_xray_half_precision_renderer.txt");
    g_xray_full_precision_renderer.computeLBuffer(g_scene_graph.getChild("outside"),
                                                  VEC3(),
                                                  g_sample_rotation_matrix);
    g_xray_full_precision_renderer.printLBuffer("g_xray_full_precision_renderer.txt");


    RATIONAL_NUMBER* l_buffer(
                              g_xray_half_precision_renderer.getLBuffer().getRawData());

    RATIONAL_NUMBER central_pixel(l_buffer[g_number_of_pixels.getX() / 2]);

    RATIONAL_NUMBER expected_value = std::sqrt(3.0 * (g_cube_side_in_cm * g_cube_side_in_cm));
    RATIONAL_NUMBER simulated_result(central_pixel);
    BOOST_CHECK_CLOSE(expected_value, simulated_result, 0.1);

    gVirtualXRay::LOGGER.logNow() << "On GPU, with half precision, the L-Buffer (in mm) computed orthogonally throw the middle of cube is:" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "\t\t           = " << simulated_result * cm / mm<< " mm" << std::endl << std::endl;

    gVirtualXRay::LOGGER.logNow() << "The absolute error is: |I_expected - I_GPU_half_precision| cm" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "                     : |" << expected_value * cm / mm << " - " << simulated_result * cm / mm << "| mm" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "                     : " << std::fixed << std::abs(expected_value - simulated_result) * cm / mm << " mm" << std::endl << std::endl;

    gVirtualXRay::LOGGER.logNow() << "The relative error is: |I_expected - I_GPU_half_precision| / I_expected" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "                     : " << std::fixed << 100.0 * std::abs(expected_value - simulated_result) / expected_value << " \%" << std::endl;

    l_buffer =
    g_xray_full_precision_renderer.getLBuffer().getRawData();

    central_pixel = l_buffer[g_number_of_pixels.getX() / 2];

    simulated_result = central_pixel;
    BOOST_CHECK_CLOSE(expected_value, simulated_result, 0.0001);


    gVirtualXRay::LOGGER.logNow() << "On GPU, with full precision, the L-Buffer (in mm) computed orthogonally throw the middle of cube and cylinder actually is:" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "\t\t           = " << simulated_result * cm / mm << " mm" << std::endl << std::endl;

    gVirtualXRay::LOGGER.logNow() << "The absolute error is: |I_expected - I_GPU_full_precision| cm" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "                     : |" << expected_value * cm / mm << " - " << simulated_result * cm / mm << "| mm" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "                     : " << std::fixed << std::abs(expected_value - simulated_result) * cm / mm << " mm" << std::endl << std::endl;

    gVirtualXRay::LOGGER.logNow() << "The relative error is: |I_expected - I_GPU_half_precision| / I_expected" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "                     : " << std::fixed << 100.0 * std::abs(expected_value - simulated_result) / expected_value << " \%" << std::endl;
}
*/

//------------------------------
TEST (TestLBuffer, TestLBuffer6)
//------------------------------
{
    // Close the window
    glfwSetWindowShouldClose(g_p_main_window_id, GL_TRUE);

    // Close the window and shut GLFW
    quit();
}

//----------------------------------------------------
void errorCallback(int error, const char* description)
//----------------------------------------------------
{
    gVirtualXRay::LOGGER.logError() << "GLFW error: " << description << std::endl;
}


//---------
void quit()
//---------
{
    if (g_p_main_window_id)
        {
        glfwDestroyWindow(g_p_main_window_id);
        g_p_main_window_id = 0;
        //glfwTerminate();
        }
}


//-----------
void initGL()
//-----------
{
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Enable the Z-buffer
    glEnable(GL_DEPTH_TEST);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Set the background colour
    glClearColor(0.5, 0.5, 0.5, 1.0);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    glEnable(GL_MULTISAMPLE);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}


//----------------
void loadSTLFile()
//----------------
{
	// Set the geometry
    g_scene_graph.addChild( "inside", CylinderMesh<GLdouble>(500, g_cylinder_height_in_cm * cm, g_cylinder_radius_in_cm * cm, GL_UNSIGNED_INT));
    g_scene_graph.addChild("outside", CubeMesh<GLdouble>(g_cube_side_in_cm * cm));


    // HU values
    RATIONAL_NUMBER HU_water(0.0);
    RATIONAL_NUMBER HU_bone(1329.885683);
    RATIONAL_NUMBER HU_soft_tissue(51.714752);

    g_scene_graph.getChild( "inside").getPolygonMesh().setHounsfieldValue(HU_bone);
    g_scene_graph.getChild("outside").getPolygonMesh().setHounsfieldValue(HU_soft_tissue);

    gVirtualXRay::LOGGER.logNow() << "We run the simulation as follows:" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "- A cube, which has edge length of 3 cm, is made of soft tissue (HU = " << std::floor(HU_soft_tissue + 0.5) << "). " << std::endl;
    gVirtualXRay::LOGGER.logNow() << "- Inside, a cylinder is inserted. " <<
            "It is made of bone (HU = " << std::floor(HU_bone + 0.5) << "), its height is 3 cm and its diameter is 2 cm." << std::endl;

    //g_scene_graph.getChild("outside").splitFaces(false);
    //g_scene_graph.getChild("outside").splitFaces(false);
    //g_scene_graph.getChild("outside").splitFaces(false);
    //g_scene_graph.getChild("outside").splitFaces(false);
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
    g_xray_half_precision_renderer.initialise(XRayRenderer::OPENGL,
        GL_RGB16F,
        &g_xray_detector,
        &g_xray_beam);

    g_xray_full_precision_renderer.initialise(XRayRenderer::OPENGL,
        GL_RGB32F,
        &g_xray_detector,
        &g_xray_beam);
}
