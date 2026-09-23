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
*   @file       TestSinogramFromMesh.cxx
*
*   @brief      Unit testing: Sinogram,
*   			the unit test corresponds to validation_08
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

#ifndef GVXR_CONTEXT_H
#include "gVirtualXRay/Context.h"
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
const float g_incident_energy(0.08 * MeV);
const VEC2 g_resolution(0.5 * mm, 1.0 * mm);
const Vec2ui g_number_of_pixels(367, 128);
const VEC3 g_source_position(   0.0,  10.0 * cm, 0.0);
const VEC3 g_detector_position( 0.0, -10.0 * cm, 0.0);
const VEC3 g_detector_up_vector(0.0, 0.0, 1.0);

const unsigned int g_angle_number(360);
const RATIONAL_NUMBER g_angle_step(0.5);


//******************************************************************************
//  Global variables
//******************************************************************************
Context g_GL_context;

Matrix4x4<GLfloat> g_sample_rotation_matrix;

std::vector<float> g_p_vertex_data;
std::vector<unsigned int> g_p_index_data;

SceneGraphNode g_polygon_mesh;

XRayBeam g_xray_beam;
XRayDetector g_xray_detector;
XRayRenderer g_xray_renderer;
Shader g_display_shader;


//******************************************************************************
//  Function declaration
//******************************************************************************
void createPhantom();

void quit();

void loadSTLFile();

void loadDetector();

void loadSource();

void loadXRaySimulator();

Vec3<GLfloat> getArcballVector(int x, int y);

void computeRotation(MATRIX4& aRotationMatrix);

void updateXRayImage();

void errorCallback(int error, const char* description);

void draw();


//----------------------------------------------
TEST(TestSinogramFromMesh, TestSinogramFromMesh)
//----------------------------------------------
{
    // Initialize random seed
    srand(time(0));

    // Register the exit callback
    atexit(quit);

    // Initialise OpenGL
    initGL(g_GL_context);

#ifdef HAS_OPENGL
    // Check the OpenGL version
    LOGGER.logNow("") << "GL:\t" << glGetString(GL_VERSION) << std::endl;

    // Check the GLSL version
    LOGGER.logNow("") << "GLSL:\t" <<
        glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
#endif

    // Check the current FBO
    checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Create the phantom
    createPhantom();

    // Display the mu coefficients
    LOGGER.logNow("") << "Linear attenuation coefficient at:\t" << g_incident_energy / keV << " keV (" << g_incident_energy / MeV << "MeV)" << std::endl;
    LOGGER.logNow("") << "Mu(HU(0):     \t" << g_xray_renderer.getMuFromHU(0, g_incident_energy) / (1.0 / cm)      << " cm-1" << std::endl;
    LOGGER.logNow("") << "Mu(HU(-1000): \t" << g_xray_renderer.getMuFromHU(-1000, g_incident_energy) / (1.0 / cm)      << " cm-1" << std::endl;

    // Load the data
    loadDetector();
    loadSource();
    loadXRaySimulator();

    // Add the geometry to the X-ray renderer
    loadSTLFile();
    g_xray_renderer.addInnerSurface(&g_polygon_mesh);

    LOGGER.logNow("") << "Angular step: " << g_angle_step << " degrees" << std::endl;
    LOGGER.logNow("") << "Angular span: [0, " <<
        g_angle_step * (g_angle_number - 1) <<
        "] degrees" << std::endl;

    // Enable artefact filtering on GPU
    g_xray_renderer.enableArtifactFilteringOnGPU();

    // Compute the sinogram
    // g_xray_renderer.computeSinogram(g_sample_rotation_matrix,
    //     VEC3(),
    //     g_angle_number,
    //     g_angle_step);

    // // Save the sinogram
    // g_xray_renderer.printSinogram("TestSinogram_simulation_sinogram.mha");

#ifdef HAS_OPENGL
    // Compute the projection set
    g_xray_renderer.computeCTAcquisition("", "",
        g_angle_number,
        0, true, g_angle_step * (g_angle_number - 1),
        0,
        VEC3(0, 0, 0),
        g_detector_up_vector,
        true,
        1);

    // Save the projection set
    g_xray_renderer.printProjectionSet("TestSinogram_simulation_projection_set.mha");

    // // Load reference sinogram (computed using GNU octave)
    // Image<XRayRenderer::PixelType> reference_sinogram("data/phantom_sinogram.mhd");

    // // Compare reference sinogram and simulated sinogram using NCC
    // ASSERT_NEAR(reference_sinogram.computeNCC(g_xray_renderer.getSinogram()), 1.0, 0.00001);

    // // Compare reference sinogram and simulated sinogram using RMSE
    // float RMSE = reference_sinogram.computeRMSE(g_xray_renderer.getSinogram());
    // float range = (reference_sinogram.getMaxValue() - reference_sinogram.getMinValue());
    // ASSERT_TRUE(RMSE < 0.01f * std::min(range, g_xray_renderer.getSinogram().getMaxValue() - g_xray_renderer.getSinogram().getMinValue()));

    // Load reference projections (computed using GNU octave)
    Image<XRayRenderer::PixelType> reference_projections("data/phantom_projections.mhd");

    // Load test projections (computed using GNU octave)
    Image<XRayRenderer::PixelType> test_projections(g_xray_renderer.getProjectionSet());
    test_projections /= g_xray_renderer.getTotalEnergyWithDetectorResponse(true);
    test_projections = test_projections.log();
    test_projections *= -1;

    // Compare reference projections and simulated projections using NCC
    ASSERT_NEAR(100 * reference_projections.computeNCC(test_projections), 100.0, 0.01);

    // Compare reference projections and simulated projections using RMSE
    float RMSE = reference_projections.computeRMSE(test_projections);
    float range = (reference_projections.getMaxValue() - reference_projections.getMinValue());
    ASSERT_TRUE(RMSE < 0.01f * std::min(range, test_projections.getMaxValue() - test_projections.getMinValue()));
#endif

    // Close the window and shut EGL
    quit();
}


//------------------
void createPhantom()
//------------------
{
    Image<short> phantom(256, 256, 128, -1000);
    phantom.setSpacing(0.5, 0.5, 1.0);

    // Set pixels in a square to one value
    unsigned int slice_size(phantom.getDepth() / 4);
    unsigned int start_slice((phantom.getDepth() - slice_size) / 2);

    unsigned int step_number(6);


    unsigned int step_height(((phantom.getWidth() - (phantom.getWidth() / 2) / 2)) / step_number);

    for(unsigned int s = start_slice; s < start_slice + slice_size; s++)
        {
        for (unsigned int step(0); step < step_number; ++step)
            {
            unsigned int row_size(phantom.getHeight() / 2);
            unsigned int start_row((phantom.getHeight()-row_size) / 4);
            start_row += step * step_height;

            for (unsigned int r(start_row);
                r < (start_row + step_height);
                ++r)
                {
                unsigned int column_size(phantom.getWidth() / 2);
                unsigned int start_column((phantom.getWidth()-column_size) / 4);

                unsigned int step_width(step_height * (1 + step));
                for (unsigned int c(start_column);
                    c < (start_column + step_width);
                    ++c)
                    {
                    phantom.setPixel(r, c, s, 0);
                    }
                }
            }
        }

    phantom.save("TestSinogram_phantom.mha");

    // Extract the isosurface
    g_polygon_mesh = SceneGraphNode("root", phantom.marchingCubes(-500), VEC3(), NULL);
    g_polygon_mesh.getPolygonMesh().moveToCentre();
    g_polygon_mesh.getPolygonMesh().setFileName("step_wedge.stl");
    g_polygon_mesh.getPolygonMesh().writeSTLFile(true);
}


//---------
void quit()
//---------
{
    g_xray_renderer.release();
    g_polygon_mesh.clear();
    g_xray_beam.clear();
    g_xray_detector.release();
    g_display_shader.release();
    g_GL_context.release();
}


//----------------
void loadSTLFile()
//----------------
{
    // Set geometry
    g_polygon_mesh.getPolygonMesh().setHounsfieldValue(0);

    float red((float(rand()) / (RAND_MAX)) + 1);
    float green((float(rand()) / (RAND_MAX)) + 1);
    float blue((float(rand()) / (RAND_MAX)) + 1);
    float alpha(1.0);

    VEC3 base_colour(red, green, blue);
    base_colour.normalise();

    g_polygon_mesh.getPolygonMesh().getMaterial().setAmbientColour(base_colour.getX() * 0.19225,
        base_colour.getY() * 0.19225,
        base_colour.getZ() * 0.19225,
        alpha);

    g_polygon_mesh.getPolygonMesh().getMaterial().setDiffuseColour(base_colour.getX() * 0.50754,
        base_colour.getY() * 0.50754,
        base_colour.getZ() * 0.50754,
        alpha);

    g_polygon_mesh.getPolygonMesh().getMaterial().setSpecularColour(base_colour.getX() * 0.50827,
        base_colour.getY() * 0.50827,
        base_colour.getZ() * 0.50827,
        alpha);

    g_polygon_mesh.getPolygonMesh().getMaterial().setShininess(50);

    // Compute the translation vector
    VEC3 translation_vector(g_polygon_mesh.getNodeAndChildrenWorldMinCorner() +
        (g_polygon_mesh.getNodeAndChildrenWorldMaxCorner() - g_polygon_mesh.getNodeAndChildrenWorldMinCorner()) / 2.0);

    g_sample_rotation_matrix =
        MATRIX4::buildTranslationMatrix(-translation_vector);
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
