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
*   @file       TestBeerLambertLawPolyChromatic.cxx
*
*   @brief      Unit testing: Beer-Lambert Law, Polychromatism Case,
*   			the unit test corresponds to validation_05.
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
//  Global variables
//******************************************************************************
Context g_GL_context;

Matrix4x4<GLfloat> g_scene_rotation_matrix;
Matrix4x4<GLfloat> g_detector_rotation_matrix;
Matrix4x4<GLfloat> g_sample_rotation_matrix;

GLfloat g_incident_energy_0(100.0 * keV);
GLfloat g_incident_energy_1(200.0 * keV);
GLfloat g_incident_energy_2(300.0 * keV);

GLfloat g_number_of_photon_in_energy_channel_0(10.0);
GLfloat g_number_of_photon_in_energy_channel_1(20.0);
GLfloat g_number_of_photon_in_energy_channel_2(10.0);

const GLfloat g_scaling_factor(2.0);
GLfloat g_resolution(0.15 * mm * g_scaling_factor); // 1mm per pixel
Vec2ui g_number_of_pixels(600 / g_scaling_factor, 1);
VEC3 g_source_position(   0.0, 0.0, -10.0 * cm);
VEC3 g_detector_position( 0.0, 0.0,  10.0 * cm);
VEC3 g_detector_up_vector(0.0, 1.0,   0.0);

SceneGraphNode g_scene_graph("root", NULL);

XRayBeam g_xray_beam;
XRayDetector g_xray_detector;
#ifdef HAS_OPENGL
XRayRenderer g_xray_half_precision_renderer;
XRayRenderer g_xray_full_precision_renderer;
#endif
XRayRenderer g_xray_openmp_renderer;

RATIONAL_NUMBER g_literature_result(0.0);
RATIONAL_NUMBER g_expected_result(0.0);

//******************************************************************************
//  Function declaration
//******************************************************************************
void loadSTLFile();
void loadDetector();
void loadSource();
void loadXRaySimulator();
void errorCallback(int error, const char* description);
void quit();


//---------------------------------------------------------------------
TEST (TestBeerLambertLawPolyChromatic, TestBeerLambertLawPolyChromatic)
//---------------------------------------------------------------------
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
#ifdef HAS_OPENGL
    g_xray_half_precision_renderer.addInnerSurface(&g_scene_graph.getChild("inside"));
    g_xray_half_precision_renderer.addOuterSurface(&g_scene_graph.getChild("outside"));
    g_xray_full_precision_renderer.addInnerSurface(&g_scene_graph.getChild("inside"));
    g_xray_full_precision_renderer.addOuterSurface(&g_scene_graph.getChild("outside"));
#endif
    g_xray_openmp_renderer.addInnerSurface(&g_scene_graph.getChild("inside"));
    g_xray_openmp_renderer.addOuterSurface(&g_scene_graph.getChild("outside"));

#ifdef HAS_OPENGL
    // Enable artefact filtering on GPU
    g_xray_half_precision_renderer.enableArtifactFilteringOnGPU();
    g_xray_full_precision_renderer.enableArtifactFilteringOnGPU();
#endif

    // Compute the X-ray image
#ifdef HAS_OPENGL
    g_xray_half_precision_renderer.computeImage(g_sample_rotation_matrix);
    g_xray_full_precision_renderer.computeImage(g_sample_rotation_matrix);
#endif
    g_xray_openmp_renderer.computeImage(g_sample_rotation_matrix);

    // Half float
    RATIONAL_NUMBER* energy_fluence = nullptr;
    RATIONAL_NUMBER central_pixel = 0.0;
    RATIONAL_NUMBER simulated_result = 0.0;
    RATIONAL_NUMBER relative_error = 0.0;

#ifdef HAS_OPENGL
    // Half float
    energy_fluence = g_xray_half_precision_renderer.getEnergyFluence().getRawData();
    central_pixel = energy_fluence[g_number_of_pixels.getX() / 2];
    simulated_result = central_pixel;

    LOGGER.logNow("") << "On GPU, with half precision, the energy (in keV) transmitted orthogonally throw the middle of cube and cylinder actually is:" << std::endl;
    LOGGER.logNow("") << "\t\t           = " << /*std::scientific <<*/ simulated_result / keV << " keV" << std::endl << std::endl;

    LOGGER.logNow("") << "The absolute error is: |I_expected - I_GPU_half_precision| keV" << std::endl;
    LOGGER.logNow("") << "                     : |" << /*std::scientific <<*/ g_expected_result / keV << " - " << simulated_result / keV << "| keV" << std::endl;
    LOGGER.logNow("") << "                     : " << std::fixed << std::abs(g_expected_result - simulated_result) / keV << " keV" << std::endl << std::endl;

    LOGGER.logNow("") << "The relative error is: |I_expected - I_GPU_half_precision| / I_expected" << std::endl;
    LOGGER.logNow("") << "                     : " << std::fixed << 100.0 * std::abs(g_expected_result - simulated_result) / g_expected_result << "%" << std::endl;

    relative_error = abs(g_expected_result - simulated_result) / g_expected_result;
    ASSERT_NEAR(relative_error, 0.0, 0.01);

    // Full float
    energy_fluence = g_xray_full_precision_renderer.getEnergyFluence().getRawData();
    central_pixel = energy_fluence[g_number_of_pixels.getX() / 2 + g_number_of_pixels.getY() / 2 * g_number_of_pixels.getX()];
    simulated_result = central_pixel;

    LOGGER.logNow("") << "On GPU, with full precision, the energy (in keV) transmitted orthogonally throw the middle of cube and cylinder actually is:" << std::endl;
    LOGGER.logNow("") << "\t\t           = " << /*std::scientific <<*/ simulated_result / keV << " keV" << std::endl << std::endl;

    LOGGER.logNow("") << "The absolute error is: |I_expected - I_GPU_full_precision| keV" << std::endl;
    LOGGER.logNow("") << "                     : |" << /*std::scientific <<*/ g_expected_result / keV << " - " << simulated_result / keV << "| keV" << std::endl;
    LOGGER.logNow("") << "                     : " << std::fixed << std::abs(g_expected_result - simulated_result) / keV << " keV" << std::endl << std::endl;

    LOGGER.logNow("") << "The relative error is: |I_expected - I_GPU_half_precision| / I_expected" << std::endl;
    LOGGER.logNow("") << "                     : " << std::fixed << 100.0 * std::abs(g_expected_result - simulated_result) / g_expected_result << "%" << std::endl;

    relative_error = abs(g_expected_result - simulated_result) / g_expected_result;
    ASSERT_NEAR(relative_error, 0.0, 0.01);
#endif

    // CPU / OpenMP
    energy_fluence = g_xray_openmp_renderer.getEnergyFluence().getRawData();
    central_pixel = energy_fluence[g_number_of_pixels.getX() / 2 + g_number_of_pixels.getY() / 2 * g_number_of_pixels.getX()];
    simulated_result = central_pixel;

    // Close the window and shut EGL
    quit();

    LOGGER.logNow("") << "On CPU, with OpenMP, the energy (in keV) transmitted orthogonally throw the middle of cube and cylinder actually is:" << std::endl;
    LOGGER.logNow("") << "\t\t           = " << /*std::scientific <<*/ simulated_result / keV << " keV" << std::endl << std::endl;

    LOGGER.logNow("") << "The absolute error is: |I_expected - I_CPU| keV" << std::endl;
    LOGGER.logNow("") << "                     : |" << /*std::scientific <<*/ g_expected_result / keV << " - " << simulated_result / keV << "| keV" << std::endl;
    LOGGER.logNow("") << "                     : " << std::fixed << std::abs(g_expected_result - simulated_result) / keV << " keV" << std::endl << std::endl;

    LOGGER.logNow("") << "The relative error is: |I_expected - I_CPU| / I_expected" << std::endl;
    LOGGER.logNow("") << "                     : " << std::fixed << 100.0 * std::abs(g_expected_result - simulated_result) / g_expected_result << "%" << std::endl;

    relative_error = abs(g_expected_result - simulated_result) / g_expected_result;
    ASSERT_NEAR(relative_error, 0.0, 0.01);
}


//---------
void quit()
//---------
{
#ifdef HAS_OPENGL
    g_xray_full_precision_renderer.release();
    g_xray_half_precision_renderer.release();
#endif
    g_xray_openmp_renderer.release();
    g_scene_graph.clear();
    g_GL_context.release();
}


//----------------
void loadSTLFile()
//----------------
{
    // HU values
    double HU_bone(1329.885683);
    double HU_soft_tissue(51.714752);

    g_scene_graph.addChild( "inside", CylinderMesh<GLdouble>(20, 3.0 * cm, 1.0 * cm));
    g_scene_graph.addChild("outside", CubeMesh<GLdouble>(3.0 * cm));

    g_scene_graph.getChild( "inside").getPolygonMesh().setHounsfieldValue(HU_bone);
    g_scene_graph.getChild("outside").getPolygonMesh().setHounsfieldValue(HU_soft_tissue);

    LOGGER.logNow("") << "We run the simulation as follows:" << std::endl;
    LOGGER.logNow("") << "- A cube, which has edge length of 3 cm, is made of soft tissue (HU = " << std::floor(HU_soft_tissue + 0.5) << "). " << std::endl;
    LOGGER.logNow("") << "- Inside, a cylinder is inserted. " <<
            "It is made of bone (HU = " << std::floor(HU_bone + 0.5) << "), its height is 3 cm and its diameter is 2 cm." << std::endl;
    LOGGER.logNow("") << "- We consider the energy of the incident beam as follows: " << std::endl;

    double total_energy(0);
    for (unsigned int i(0); i < g_xray_beam.getEnergyChannelNumber(); ++i)
        {
        LOGGER.logNow("") << g_xray_beam.getEnergyChannel(i).getPhotonNumber() << " photons of " <<  g_xray_beam.getEnergyChannel(i).getPhotonEnergy() / keV << " keV (i.e. " << g_xray_beam.getEnergyChannel(i).getPhotonEnergy() / keV << " keV). " << std::endl;
        total_energy += g_xray_beam.getEnergyChannel(i).getPhotonNumber() * g_xray_beam.getEnergyChannel(i).getPhotonEnergy();
        }
    LOGGER.logNow("") << std::endl;

    LOGGER.logNow("") << "Total input energy = " << total_energy / keV << std::endl << std::endl;

    LOGGER.logNow("") << "From the literature, we know:" << std::endl;
    std::cout.precision(4);
    LOGGER.logNow("") <<
        "HU = "     << std::fixed      << HU_bone << "\t" <<
        "rho = "    << std::scientific << (g_xray_openmp_renderer.getDensityFromHU(HU_bone) / (g/cm3)) << " g/cm3,\t" <<
        "energy = " << std::fixed      << 1.000E-01 * MeV / keV<< " keV,\t" <<
        "mu/rho = " << std::scientific << 1.855E-01 << " cm2/g,\t" <<
        "mu = "     << std::scientific << (g_xray_openmp_renderer.getDensityFromHU(HU_bone) / (g/cm3)) * 1.855E-01 << " cm-1" << std::endl;

    LOGGER.logNow("") <<
        "HU =   "   << std::fixed      << HU_soft_tissue << "\t" <<
        "rho = "    << std::scientific << (g_xray_openmp_renderer.getDensityFromHU(HU_soft_tissue) / (g/cm3)) << " g/cm3,\t" <<
        "energy = " << std::fixed      << 1.000E-01 * MeV / keV << " keV,\t" <<
        "mu/rho = " << std::scientific << 1.693E-01 << " cm2/g,\t" <<
        "mu = "     << std::scientific << (g_xray_openmp_renderer.getDensityFromHU(HU_soft_tissue) / (g/cm3)) * 1.693E-01 << " cm-1" << std::endl << std::endl;

    LOGGER.logNow("") <<
        "HU = "     << std::fixed      << HU_bone << "\t" <<
        "rho = "    << std::scientific << (g_xray_openmp_renderer.getDensityFromHU(HU_bone) / (g/cm3)) << " g/cm3,\t" <<
        "energy = " << std::fixed      << 2.000E-01 * MeV / keV << " keV,\t" <<
        "mu/rho = " << std::scientific << 1.309E-01 << " cm2/g,\t" <<
        "mu = "     << std::scientific << (g_xray_openmp_renderer.getDensityFromHU(HU_bone) / (g/cm3)) * 1.309E-01 << " cm-1" << std::endl;

    LOGGER.logNow("") <<
        "HU =   "     << std::fixed    << HU_soft_tissue << "\t" <<
        "rho = "    << std::scientific << (g_xray_openmp_renderer.getDensityFromHU(HU_soft_tissue) / (g/cm3)) << " g/cm3,\t" <<
        "energy = " << std::fixed      << 2.000E-01 * MeV / keV << " keV,\t" <<
        "mu/rho = " << std::scientific << 1.358E-01 << " cm2/g,\t" <<
        "mu = "     << std::scientific << (g_xray_openmp_renderer.getDensityFromHU(HU_soft_tissue) / (g/cm3)) * 1.358E-01 << " cm-1" << std::endl << std::endl;

    LOGGER.logNow("") <<
        "HU = "     << std::fixed      << HU_bone << "\t" <<
        "rho = "    << std::scientific << (g_xray_openmp_renderer.getDensityFromHU(HU_bone) / (g/cm3)) << " g/cm3,\t" <<
        "energy = " << std::fixed      << 3.000E-01 * MeV / keV << " keV,\t" <<
        "mu/rho = " << std::scientific << 1.113E-01 << " cm2/g,\t" <<
        "mu = "     << std::scientific << (g_xray_openmp_renderer.getDensityFromHU(HU_bone) / (g/cm3)) * 1.113E-01 << " cm-1" << std::endl;

    LOGGER.logNow("") <<
        "HU =   "   << std::fixed      << HU_soft_tissue << "\t" <<
        "rho = "    << std::scientific << (g_xray_openmp_renderer.getDensityFromHU(HU_soft_tissue) / (g/cm3)) << " g/cm3,\t" <<
        "energy = " << std::fixed      << 3.000E-01 * MeV / keV << " keV,\t" <<
        "mu/rho = " << std::scientific << 1.175E-01 << " cm2/g,\t" <<
        "mu = "     << std::scientific << (g_xray_openmp_renderer.getDensityFromHU(HU_soft_tissue) / (g/cm3)) * 1.175E-01 << " cm-1" << std::endl << std::endl;

    //g_literature_result = g_incident_energy * exp(-(4.280e-01 * 2 + 1.932e-01 * 1));

    g_literature_result = 0.0;
    g_literature_result += (g_number_of_photon_in_energy_channel_0 * g_incident_energy_0) * exp(-(((g_xray_openmp_renderer.getDensityFromHU(HU_bone) / (g/cm3)) * 1.855E-01) * 2.0 + ((g_xray_openmp_renderer.getDensityFromHU(HU_soft_tissue) / (g/cm3)) * 1.693E-01) * 1.0));
    g_literature_result += (g_number_of_photon_in_energy_channel_1 * g_incident_energy_1) * exp(-(((g_xray_openmp_renderer.getDensityFromHU(HU_bone) / (g/cm3)) * 1.309E-01) * 2.0 + ((g_xray_openmp_renderer.getDensityFromHU(HU_soft_tissue) / (g/cm3)) * 1.358E-01) * 1.0));
    g_literature_result += (g_number_of_photon_in_energy_channel_2 * g_incident_energy_2) * exp(-(((g_xray_openmp_renderer.getDensityFromHU(HU_bone) / (g/cm3)) * 1.113E-01) * 2.0 + ((g_xray_openmp_renderer.getDensityFromHU(HU_soft_tissue) / (g/cm3)) * 1.175E-01) * 1.0));

    std::cout.precision(3);
    LOGGER.logNow("") << "The energy (in keV) transmitted orthogonally throw the middle of cube and cylinder should be:" << std::endl;
    LOGGER.logNow("") << "\t\tI_reference = I_0 * exp(-Sum(mu * x))" << std::endl;
    LOGGER.logNow("") << "\t\t            = " << std::scientific << g_literature_result / keV << " keV" << std::endl << std::endl;

    LOGGER.logNow("") << "To obtain linear attenuation coefficients (mu values) in our implementation, densities (rho values) and mass attenuation coefficients (mu/rho values) are estimated as follows:" << std::endl;

    std::cout.precision(4);
    for (unsigned int i(0); i < g_xray_beam.getEnergyChannelNumber(); ++i)
        {
        LOGGER.logNow("") <<
            "HU = "     << std::fixed      << HU_bone << "\t" <<
            "rho = "    << std::scientific << g_xray_openmp_renderer.getDensityFromHU(HU_bone)  / (g/cm3) << " g/cm3,\t" <<
            "energy = " << std::fixed      << g_xray_beam.getEnergyChannel(i).getPhotonEnergy() / keV << " keV,\t" <<
            "mu/rho = " << std::scientific << g_xray_openmp_renderer.getMassAttenuationFromHU(HU_bone, g_xray_beam.getEnergyChannel(i).getPhotonEnergy())  / (cm2/g) << " cm2/g,\t" <<
            "mu = "     << std::scientific << g_xray_openmp_renderer.getMuFromHU(HU_bone, g_xray_beam.getEnergyChannel(i).getPhotonEnergy()) / (1.0 / cm) << " cm-1" << std::endl;

        LOGGER.logNow("") <<
            "HU =   "   << std::fixed      << HU_soft_tissue << "\t" <<
            "rho = "    << std::scientific << g_xray_openmp_renderer.getDensityFromHU(HU_soft_tissue)  / (g/cm3) << " g/cm3,\t" <<
            "energy = " << std::fixed      << g_xray_beam.getEnergyChannel(i).getPhotonEnergy() / keV << " keV,\t" <<
            "mu/rho = " << std::scientific << g_xray_openmp_renderer.getMassAttenuationFromHU(HU_soft_tissue, g_xray_beam.getEnergyChannel(i).getPhotonEnergy())  / (cm2/g) << " cm2/g,\t" <<
            "mu = "     << std::scientific << g_xray_openmp_renderer.getMuFromHU(HU_soft_tissue, g_xray_beam.getEnergyChannel(i).getPhotonEnergy()) / (1.0 / cm) << " cm-1" << std::endl << std::endl;
        }

    g_expected_result = 0.0;
    for (unsigned int i(0); i < g_xray_beam.getEnergyChannelNumber(); ++i)
        {
        double photon_number(g_xray_beam.getEnergyChannel(i).getPhotonNumber());
        double incident_energy(g_xray_beam.getEnergyChannel(i).getPhotonEnergy());

        g_expected_result += (photon_number * incident_energy) * exp(-(g_xray_openmp_renderer.getMuFromHU(HU_bone, incident_energy) / (1.0 / cm) * 2 + g_xray_openmp_renderer.getMuFromHU(HU_soft_tissue, incident_energy) / (1.0 / cm) * 1));
        }

    LOGGER.logNow("") << "The energy (in keV) transmitted orthogonally throw the middle of cube and cylinder should be:" << std::endl;
    LOGGER.logNow("") << "\t\tI_expected = I_0 * exp(-Sum(mu * x))" << std::endl;
    LOGGER.logNow("") << "\t\tI_expected = " << "(" << std::fixed << g_xray_beam.getEnergyChannel(0).getPhotonEnergy() / keV << " x " << g_xray_beam.getEnergyChannel(0).getPhotonNumber() << ") x exp(-(" << g_xray_openmp_renderer.getMuFromHU(HU_bone, g_xray_beam.getEnergyChannel(0).getPhotonEnergy()) / (1.0 / cm) << " x 2 + " << g_xray_openmp_renderer.getMuFromHU(HU_soft_tissue, g_xray_beam.getEnergyChannel(0).getPhotonEnergy()) / (1.0 / cm) << " x 1)) +" << std::endl;
    LOGGER.logNow("") << "\t\t             " << "(" << std::fixed << g_xray_beam.getEnergyChannel(1).getPhotonEnergy() / keV << " x " << g_xray_beam.getEnergyChannel(1).getPhotonNumber() << ") x exp(-(" << g_xray_openmp_renderer.getMuFromHU(HU_bone, g_xray_beam.getEnergyChannel(1).getPhotonEnergy()) / (1.0 / cm) << " x 2 + " << g_xray_openmp_renderer.getMuFromHU(HU_soft_tissue, g_xray_beam.getEnergyChannel(1).getPhotonEnergy()) / (1.0 / cm) << " x 1)) +" << std::endl;
    LOGGER.logNow("") << "\t\t             " << "(" << std::fixed << g_xray_beam.getEnergyChannel(2).getPhotonEnergy() / keV << " x " << g_xray_beam.getEnergyChannel(2).getPhotonNumber() << ") x exp(-(" << g_xray_openmp_renderer.getMuFromHU(HU_bone, g_xray_beam.getEnergyChannel(2).getPhotonEnergy()) / (1.0 / cm) << " x 2 + " << g_xray_openmp_renderer.getMuFromHU(HU_soft_tissue, g_xray_beam.getEnergyChannel(2).getPhotonEnergy()) / (1.0 / cm) << " x 1))" << std::endl;
    std::cout.precision(3);
    LOGGER.logNow("") << "\t\tI_expected = " << (g_xray_beam.getEnergyChannel(0).getPhotonEnergy() / keV * g_xray_beam.getEnergyChannel(0).getPhotonNumber()) * exp(-(g_xray_openmp_renderer.getMuFromHU(HU_bone, g_xray_beam.getEnergyChannel(0).getPhotonEnergy()) / (1.0 / cm) * 2.0 + g_xray_openmp_renderer.getMuFromHU(HU_soft_tissue, g_xray_beam.getEnergyChannel(0).getPhotonEnergy()) / (1.0 / cm) * 1.0)) << " +" << std::endl;
    LOGGER.logNow("") << "\t\t             " << (g_xray_beam.getEnergyChannel(1).getPhotonEnergy() / keV * g_xray_beam.getEnergyChannel(1).getPhotonNumber()) * exp(-(g_xray_openmp_renderer.getMuFromHU(HU_bone, g_xray_beam.getEnergyChannel(1).getPhotonEnergy()) / (1.0 / cm) * 2.0 + g_xray_openmp_renderer.getMuFromHU(HU_soft_tissue, g_xray_beam.getEnergyChannel(1).getPhotonEnergy()) / (1.0 / cm) * 1.0)) << " +" << std::endl;
    LOGGER.logNow("") << "\t\t             " << (g_xray_beam.getEnergyChannel(2).getPhotonEnergy() / keV * g_xray_beam.getEnergyChannel(2).getPhotonNumber()) * exp(-(g_xray_openmp_renderer.getMuFromHU(HU_bone, g_xray_beam.getEnergyChannel(2).getPhotonEnergy()) / (1.0 / cm) * 2.0 + g_xray_openmp_renderer.getMuFromHU(HU_soft_tissue, g_xray_beam.getEnergyChannel(2).getPhotonEnergy()) / (1.0 / cm) * 1.0))         << std::endl;
    LOGGER.logNow("") << "\t\t           = " << /*std::scientific*/ std::fixed << g_expected_result / keV << " keV" << std::endl << std::endl;

    LOGGER.logNow("") << "The absolute error is: |I_expected - I_reference| keV" << std::endl;
    LOGGER.logNow("") << "                     : |" << /*std::scientific <<*/ g_expected_result / keV << " - " << g_literature_result / keV << "| keV" << std::endl;
    LOGGER.logNow("") << "                     : " << std::fixed << std::abs(g_expected_result - g_literature_result) / keV << " keV" << std::endl << std::endl;

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
    g_xray_beam.clear();

    g_xray_beam.addChannelPerPixelAtSDD(g_number_of_photon_in_energy_channel_0, g_incident_energy_0);
    g_xray_beam.addChannelPerPixelAtSDD(g_number_of_photon_in_energy_channel_1, g_incident_energy_1);
    g_xray_beam.addChannelPerPixelAtSDD(g_number_of_photon_in_energy_channel_2, g_incident_energy_2);

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
    g_xray_half_precision_renderer.initialise(XRayRenderer::OPENGL,
        0,
        &g_xray_detector,
        &g_xray_beam);

    g_xray_full_precision_renderer.initialise(XRayRenderer::OPENGL,
        255,
        &g_xray_detector,
        &g_xray_beam);
#endif

    g_xray_openmp_renderer.initialise(XRayRenderer::OPENMP,
        0,
        &g_xray_detector,
        &g_xray_beam);
}
