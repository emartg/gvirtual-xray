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
*   @file       TestPhantomMonoChromatic.cxx
*
*   @brief      Unit testing: phantom with monochromatic sources (validation_??).
*               A similar experimental set up is used with a validated
*               Monte Carlo simulation tool. We compare the results produce with
*               gVirtualXRay.
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

#ifndef GVXR_CYLINDER_MESH_H
#include "gVirtualXRay/CylinderMesh.h"
#endif

#ifndef GVXR_SPHERE_MESH_H
#include "gVirtualXRay/SphereMesh.h"
#endif

#ifndef GVXR_CYLINDER_MESH_H
#include "gVirtualXRay/CylinderMesh.h"
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

#ifndef GVXR_LOGGER_H
#include "gVirtualXRay/Logger.h"
#endif


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;
//using namespace std;


//******************************************************************************
//  Constant variables
//******************************************************************************


//******************************************************************************
//  Global variables
//******************************************************************************
Context g_GL_context;

Matrix4x4<GLfloat> g_sample_rotation_matrix;

GLfloat g_incident_energy_33(33.0 * keV);
GLfloat g_incident_energy_44(43.9 * keV);

//const GLfloat g_scaling_factor(0.25);
const GLfloat g_scaling_factor(2);
GLfloat g_resolution(1.0 * mm * g_scaling_factor); // 1mm per pixel
Vec2ui g_number_of_pixels(300.0 / g_scaling_factor, 300.0 / g_scaling_factor);

VEC3 g_source_position(   0.0, 0.0, -50.0 * cm);
VEC3 g_detector_position( 0.0, 0.0,  50.0 * cm);
VEC3 g_detector_up_vector(0.0, 1.0,   0.0);

SceneGraphNode g_scene_graph("root", NULL);
CylinderMesh<GLdouble> g_cylinder;
SphereMesh g_sphere_0;
SphereMesh g_sphere_1;
SphereMesh g_sphere_2;
SphereMesh g_sphere_3;
SphereMesh g_sphere_4;
SphereMesh g_sphere_5;

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


//------------------------------------------------------
TEST(TestPhantomMonoChromatic, TestPhantomMonoChromatic)
//------------------------------------------------------
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

    // Output the detector properties
    LOGGER.logNow("") << "Detector size: " << g_number_of_pixels.getX() << "x" << g_number_of_pixels.getY() << " pixels" << std::endl;
    LOGGER.logNow("") << std::fixed << "Pixel size:" << g_resolution / mm << "x" << g_resolution / mm << " mm2" << std::endl;
    LOGGER.logNow("") << "Detector size: " << g_resolution / cm * g_number_of_pixels.getX() << "x" << g_resolution / cm * g_number_of_pixels.getY() << " cm2" << std::endl;
    LOGGER.logNow("") << "Detector position: " << g_detector_position / cm << " (in cm)" << std::endl;
    LOGGER.logNow("") << "Source position: "   << g_source_position   / cm << " (in cm)" << std::endl;

    // Set the energy
    g_xray_beam.initialise(g_incident_energy_33);

    // Enable artefact filtering on GPU
    g_xray_renderer.enableArtifactFilteringOnGPU();

    // Compute the projection set
    g_xray_renderer.computeProjectionSet(g_sample_rotation_matrix,
        VEC3(),
        180,
        1);

    g_xray_renderer.computeSinogram(g_sample_rotation_matrix,
        VEC3(),
        180,
        1);

    // Save the projection set
    g_xray_renderer.printProjectionSet("TestPhantomMonoChromatic_33keV_single_projection.mha");
    g_xray_renderer.printSinogram("TestPhantomMonoChromatic_33keV_single_sinogram.mha");

    float mu_water_at_33keV(g_xray_renderer.getMuFromHU(0, g_incident_energy_33));
    float mu_water_at_44keV(g_xray_renderer.getMuFromHU(0, g_incident_energy_44));

    Sinogram<float> sinogram = g_xray_renderer.getSinogram().getROI(
            0,
            0,
            g_xray_renderer.getSinogram().getDepth() / 2,
            g_xray_renderer.getSinogram().getWidth(),
            g_xray_renderer.getSinogram().getHeight(),
            1);
/*    Image<float> reconstruction_in_mu_at_33keV = sinogram.filteredBackProjection(0, 1);
    Image<float> reconstruction_in_HU_at_33keV = (reconstruction_in_mu_at_33keV - mu_water_at_33keV) * (1000.0 / mu_water_at_33keV);

    reconstruction_in_mu_at_33keV.save("TestPhantomMonoChromatic_33keV_FBP_in_mu_at_33keV.mha");
    reconstruction_in_HU_at_33keV.save("TestPhantomMonoChromatic_33keV_FBP_in_HU_at_33keV.mha");
*/
    // Set the energy
   /* g_xray_beam.initialise(g_incident_energy_44);

    // Compute the projection set
    g_xray_renderer.computeProjectionSet(g_sample_rotation_matrix,
        VEC3(),
        180,
        1);

    g_xray_renderer.computeSinogram(g_sample_rotation_matrix,
        VEC3(),
        180,
        1);

    // Save the projection set
    g_xray_renderer.printProjectionSet("TestPhantomMonoChromatic_44keV_single_projection.mha");
    g_xray_renderer.printSinogram("TestPhantomMonoChromatic_44keV_single_sinogram.mha");
*/
    //Image<float> reconstruction_in_mu_at_44keV = g_xray_renderer.getSinogram().filteredBackProjection(0, 1);
    //Image<float> reconstruction_in_HU_at_44keV = (reconstruction_in_mu_at_33keV - mu_water_at_44keV) * (1000.0 / mu_water_at_44keV);

    //reconstruction_in_mu_at_44keV.save("TestPhantomMonoChromatic_33keV_FBP_in_mu_at_44keV.mha");
    //reconstruction_in_HU_at_44keV.save("TestPhantomMonoChromatic_33keV_FBP_in_HU_at_44keV.mha");


    // Close the window and shut EGL
    quit();
}


//---------
void quit()
//---------
{
    g_xray_full_precision_renderer.release();
    g_xray_half_precision_renderer.release();
    g_scene_graph.clear();
    g_GL_context.release();
}


//----------------
void loadSTLFile()
//----------------
{
    // HU values
    RATIONAL_NUMBER HU_air(-1000);
    RATIONAL_NUMBER HU_lung(-500);
    RATIONAL_NUMBER HU_fat( -100);
    RATIONAL_NUMBER HU_water(  0);
    RATIONAL_NUMBER HU_kidney(35);
    RATIONAL_NUMBER HU_liver( 50);
    RATIONAL_NUMBER HU_bone(1500);

    // Set geometry
    g_cylinder.create(3, 12.6 * cm, 21.6 / 2.0 * cm);
    g_cylinder.setHounsfieldValue(HU_water);

    g_sphere_0.setHounsfieldValue(HU_air);
    g_sphere_1.setHounsfieldValue(HU_lung);
    g_sphere_2.setHounsfieldValue(HU_fat);
    g_sphere_3.setHounsfieldValue(HU_kidney);
    g_sphere_4.setHounsfieldValue(HU_liver);
    g_sphere_5.setHounsfieldValue(HU_bone);

    g_sphere_0.create(5,  9.5 *mm);
    g_sphere_1.create(5, 12.7 *mm);
    g_sphere_2.create(5, 15.9 *mm);
    g_sphere_3.create(5, 19.1 *mm);
    g_sphere_4.create(5, 25.4 *mm);
    g_sphere_5.create(5, 31.8 *mm);

    g_sphere_0.translate(VEC3(6.0 * cm, 0, 0));
    g_sphere_1.translate(VEC3(6.0 * cm, 0, 0));
    g_sphere_2.translate(VEC3(6.0 * cm, 0, 0));
    g_sphere_3.translate(VEC3(6.0 * cm, 0, 0));
    g_sphere_4.translate(VEC3(6.0 * cm, 0, 0));
    g_sphere_5.translate(VEC3(6.0 * cm, 0, 0));

    //g_sphere_0.rotate(g_detector_up_vector,   0.0);
    g_sphere_1.rotate(g_detector_up_vector,  60.0);
    g_sphere_2.rotate(g_detector_up_vector, 120.0);
    g_sphere_3.rotate(g_detector_up_vector, 180.0);
    g_sphere_4.rotate(g_detector_up_vector, 240.0);
    g_sphere_5.rotate(g_detector_up_vector, 300.0);

 /*   g_xray_renderer.addInnerSurface(&g_sphere_0);
    g_xray_renderer.addInnerSurface(&g_sphere_1);
    g_xray_renderer.addInnerSurface(&g_sphere_2);
    g_xray_renderer.addInnerSurface(&g_sphere_3);
    g_xray_renderer.addInnerSurface(&g_sphere_4);
    g_xray_renderer.addInnerSurface(&g_sphere_5);*/
    g_xray_renderer.addOuterSurface(&g_cylinder);
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
    g_xray_beam.initialise(0);

    // Set the source position
    g_xray_detector.setXrayPointSource(g_source_position);
    g_xray_detector.setParallelBeam();
}


//----------------------
void loadXRaySimulator()
//----------------------
{
    // Initialise the X-ray renderer
#ifdef HAS_OPENGL
    g_xray_renderer.initialise(XRayRenderer::OPENGL,
        GL_RGB32F,
        &g_xray_detector,
        &g_xray_beam);
#else
    g_xray_renderer.initialise(XRayRenderer::OPENMP,
        0,
        &g_xray_detector,
        &g_xray_beam);
#endif
}
