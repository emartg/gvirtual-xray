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
*   @file       TestRadonTransformFromMesh.cxx
*
*   @brief      Unit testing: Radon transform.
*               We compare the results produced with gVirtualXRay and GNU Octave.
*   			The unit test corresponds to parts of validation_04.
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
#include <iostream>
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

#ifndef GVXR_CUBE_MESH_H
#include "gVirtualXRay/CubeMesh.h"
#endif

#ifndef GVXR_XRAY_BEAM_H
#include "gVirtualXRay/XRayBeam.h"
#endif

#ifndef GVXR_XRAY_DETECTOR_H
#include "gVirtualXRay/XRayDetector.h"
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

#ifndef GVXR_SINOGRAM_H
#include "gVirtualXRay/Sinogram.h"
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
Vec2ui g_number_of_pixels(600 / g_scaling_factor, 1);
VEC3 g_source_position(   0.0, 0.0, -10.0 * cm);
VEC3 g_detector_position( 0.0, 0.0,  10.0 * cm);
VEC3 g_detector_up_vector(0.0, 1.0,   0.0);

SceneGraphNode g_scene_graph("root", NULL);

XRayBeam g_xray_beam;
XRayDetector g_xray_detector;
XRayRenderer g_xray_renderer;

RATIONAL_NUMBER g_literature_result(0.0);
RATIONAL_NUMBER g_expected_result(0.0);

unsigned int g_angle_number(360);
RATIONAL_NUMBER g_angle_step(0.5);


//******************************************************************************
//  Function declaration
//******************************************************************************
void loadSTLFile();
void loadDetector();
void loadSource();
void loadXRaySimulator();
void errorCallback(int error, const char* description);
void quit();


//----------------------------------------------------------
TEST(TestRadonTransformFromMesh, TestRadonTransformFromMesh)
//----------------------------------------------------------
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

    // Output the detector properties
    LOGGER.logNow("") << "Detector size: " <<
        g_number_of_pixels.getX() <<
        "x" <<
        g_number_of_pixels.getY() <<
        " pixels" << std::endl;

    LOGGER.logNow("") << std::fixed << "Pixel size:" <<
        g_resolution / mm <<
        "x" <<
        g_resolution / mm <<
        " mm2" << std::endl;

    LOGGER.logNow("") << "Detector size: " <<
        g_resolution / cm * g_number_of_pixels.getX() <<
        "x" <<
        g_resolution / cm * g_number_of_pixels.getY() <<
        " cm2" << std::endl;

    LOGGER.logNow("") << "Angular step: " << g_angle_step << " degrees" << std::endl;
    LOGGER.logNow("") << "Angular span: [0, " <<
        g_angle_step * (g_angle_number - 1) <<
        "] degrees" << std::endl;

    // Enable artefact filtering on GPU
    g_xray_renderer.enableArtifactFilteringOnGPU();
    // g_xray_renderer.disableArtifactFilteringOnGPU();
    // g_xray_renderer.disableArtifactFilteringOnCPU();

    // Compute the sinogram
    g_xray_renderer.computeSinogram(g_sample_rotation_matrix,
        VEC3(),
        g_angle_number,
        g_angle_step);

    // Save the sinogram
    g_xray_renderer.printSinogram("TestRadonTransformFromMesh_sinogram.mha");

    // Compute the projection set
    g_xray_renderer.computeProjectionSet(g_sample_rotation_matrix,
        VEC3(),
        g_angle_number,
        g_angle_step);

    // Save the projection set
    g_xray_renderer.printProjectionSet("TestRadonTransformFromMesh_projection_set.mha");

    // Close the window and shut EGL
    quit();


    /********** Sinogram **********/

    // Load reference image (computed using GNU Octave)
    Image<XRayRenderer::PixelType> reference_sinogram("data/CT_reconstruction/sinogram.mhd");

    // Compare reference projection set and simulated projection set using NCC
    double ZNCC = 100.0 * reference_sinogram.computeNCC(g_xray_renderer.getSinogram());
    ASSERT_NEAR(ZNCC, 100.0, 0.5);

    // Compare reference projection set and simulated projection set using RMSE
    double RMSE = reference_sinogram.computeRMSE(g_xray_renderer.getSinogram());
    ASSERT_NEAR(RMSE, 0.0, 0.1 * mm);




    /********** Projection set **********/

    // Load reference image (computed using GNU Octave)
    Image<XRayRenderer::PixelType> reference_projection_set("data/CT_reconstruction/projection_set.mhd");

    // Compare reference projection set and simulated projection set using NCC
    ZNCC = 100.0 * reference_projection_set.computeNCC(g_xray_renderer.getProjectionSet());
    ASSERT_NEAR(ZNCC, 100.0, 5);

    // Compare reference projection set and simulated projection set using RMSE
    // RMSE = reference_projection_set.computeRMSE(g_xray_renderer.getProjectionSet());
    //ASSERT_NEAR(RMSE, 0.0, 0.001 * mm);
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
    g_scene_graph.addChild( "inside", CylinderMesh<GLdouble>(20, 3.0 * cm, 1.0 * cm));
    g_scene_graph.addChild("outside", CubeMesh<GLdouble>(3.0 * cm));

    // HU values
    RATIONAL_NUMBER HU_water(0.0);
    RATIONAL_NUMBER HU_bone(1329.885683);
    RATIONAL_NUMBER HU_soft_tissue(51.714752);

    g_scene_graph.getChild( "inside").getPolygonMesh().setHounsfieldValue(HU_bone);
    g_scene_graph.getChild("outside").getPolygonMesh().setHounsfieldValue(HU_soft_tissue);

    LOGGER.logNow("") << "We run the simulation as follows:" << std::endl;
    LOGGER.logNow("") << "- A cube, which has edge length of 3 cm, is made of soft tissue (HU = " << std::floor(HU_soft_tissue + 0.5) << "). " << std::endl;
    LOGGER.logNow("") << "- Inside, a cylinder is inserted. " <<
            "It is made of bone (HU = " << std::floor(HU_bone + 0.5) << "), its height is 3 cm and its diameter is 2 cm." << std::endl;
    LOGGER.logNow("") << "- We consider the energy of the incident beam is " << g_incident_energy / keV << " keV (i.e. " << g_incident_energy / MeV << " MeV). " << std::endl << std::endl;

    LOGGER.logNow("") << "From the literature, we know:" << std::endl;
    std::cout.precision(3);
    LOGGER.logNow("") <<
            "HU =    "     << std::fixed << HU_water << "\t" <<
            "rho = "    << std::scientific << 1.0 << " g/cm3,\t" <<
            "mu/rho = " << std::scientific << 1.837E-01 << " cm2/g,\t" <<
            "mu = "     << std::scientific << 1.837E-01 << " cm-1" << std::endl;

    LOGGER.logNow("") <<
            "HU = "     << std::fixed << HU_bone << "\t" <<
            "rho = "    << std::scientific << 1.920E+00 << " g/cm3,\t" <<
            "mu/rho = " << std::scientific << 2.229E-01 << " cm2/g,\t" <<
            "mu = "     << std::scientific << 4.280E-01 << " cm-1" << std::endl;

    LOGGER.logNow("") <<
            "HU =   "     << std::fixed << HU_soft_tissue << "\t" <<
            "rho = "    << std::scientific << 1.060E+00 << " g/cm3,\t" <<
            "mu/rho = " << std::scientific << 1.823E-01 << " cm2/g,\t" <<
            "mu = "     << std::scientific << 1.932E-01 << " cm-1" << std::endl << std::endl;

    LOGGER.logNow("") << "To obtain linear attenuation coefficients (mu values) in our implementation, densities (rho values) and mass attenuation coefficients (mu/rho values) are estimated as follows:" << std::endl;
    LOGGER.logNow("") <<
            "HU =    "     << std::fixed << HU_water << "\t" <<
            "rho = "    << std::scientific << g_xray_renderer.getDensityFromHU(HU_water)  / (g/cm3) << " g/cm3,\t" <<
            "mu/rho = " << std::scientific << g_xray_renderer.getMassAttenuationFromHU(HU_water, g_incident_energy)  / (cm2/g) << " cm2/g,\t" <<
            "mu = "     << std::scientific << g_xray_renderer.getMuFromHU(HU_water, g_incident_energy) / (1.0 / cm) << " cm-1" << std::endl;

    LOGGER.logNow("") <<
            "HU = "     << std::fixed << HU_bone << "\t" <<
            "rho = "    << std::scientific << g_xray_renderer.getDensityFromHU(HU_bone)  / (g/cm3) << " g/cm3,\t" <<
            "mu/rho = " << std::scientific << g_xray_renderer.getMassAttenuationFromHU(HU_bone, g_incident_energy)  / (cm2/g) << " cm2/g,\t" <<
            "mu = "     << std::scientific << g_xray_renderer.getMuFromHU(HU_bone, g_incident_energy) / (1.0 / cm) << " cm-1" << std::endl;

    LOGGER.logNow("") <<
            "HU =   "     << std::fixed << HU_soft_tissue << "\t" <<
            "rho = "    << std::scientific << g_xray_renderer.getDensityFromHU(HU_soft_tissue)  / (g/cm3) << " g/cm3,\t" <<
            "mu/rho = " << std::scientific << g_xray_renderer.getMassAttenuationFromHU(HU_soft_tissue, g_incident_energy)  / (cm2/g) << " cm2/g,\t" <<
            "mu = "     << std::scientific << g_xray_renderer.getMuFromHU(HU_soft_tissue, g_incident_energy) / (1.0 / cm) << " cm-1" << std::endl << std::endl;

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
