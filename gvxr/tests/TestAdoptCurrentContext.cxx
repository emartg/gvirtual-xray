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
*   @file       TestAdoptCurrentContext.cxx
*
*   @brief      Unit testing: a Context that adopts the OpenGL context that the
*               application has made current, instead of creating its own.
*               The X-ray image is computed on the adopted context and is
*               compared with the Beer-Lambert law (same set-up as
*               TestBeerLambertLawMonoChromatic).
*
*   @author     emartg
*
*   @section    License
*               BSD 3-Clause License.
*
*               For details on use and redistribution please refer
*               to http://opensource.org/licenses/BSD-3-Clause
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

#include "gtest/gtest.h"

#ifndef GVXR_TYPES_H
#include "gVirtualXRay/Types.h"
#endif

#ifndef GVXR_UNITS_H
#include "gVirtualXRay/Units.h"
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

#ifndef GVXR_XRAY_RENDERER_H
#include "gVirtualXRay/XRayRenderer.h"
#endif

#ifndef GVXR_RESOURCE_MANAGER_H
#include "gVirtualXRay/ResourceManager.h"
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
// The context of the application: it owns the window and the OpenGL context
Context g_application_context;

// The context that adopts the OpenGL context of the application
Context g_adopted_context;


//--------------------------------------------------
TEST(TestAdoptCurrentContext, NoCurrentContextFails)
//--------------------------------------------------
{
    // No OpenGL context has been made current on this thread yet
    Context context;
    ASSERT_FALSE(context.adoptCurrentContext());
    ASSERT_FALSE(context.isReady());
    ASSERT_FALSE(context.isExternal());
}


#ifdef HAS_OPENGL
//----------------------------------------------------------
TEST(TestAdoptCurrentContext, BeerLambertLawOnAdoptedContext)
//----------------------------------------------------------
{
    // The application makes an OpenGL context current
    initGL(g_application_context);
    if (!g_application_context.isReady())
    {
        GTEST_SKIP() << "No OpenGL context could be created";
    }
    ASSERT_FALSE(g_application_context.isExternal());

    // The library uses the adopted context
    ResourceManager::getInstance().setGraphicsContext(&g_adopted_context);
    ASSERT_FALSE(g_adopted_context.isReady());
    ASSERT_FALSE(g_adopted_context.isExternal());

    ASSERT_TRUE(g_adopted_context.adoptCurrentContext());
    ASSERT_TRUE(g_adopted_context.isReady());
    ASSERT_TRUE(g_adopted_context.isExternal());

    // Set-up: a cube of soft tissue (3 cm) with a cylinder of bone inside (3 cm high, 2 cm wide)
    const GLfloat incident_energy(80.0 * keV);
    const RATIONAL_NUMBER HU_bone(1329.885683);
    const RATIONAL_NUMBER HU_soft_tissue(51.714752);

    SceneGraphNode scene_graph("root", NULL);
    scene_graph.addChild( "inside", CylinderMesh<GLdouble>(20, 3.0 * cm, 1.0 * cm));
    scene_graph.addChild("outside", CubeMesh<GLdouble>(3.0 * cm));
    scene_graph.getChild( "inside").getPolygonMesh().setHounsfieldValue(HU_bone);
    scene_graph.getChild("outside").getPolygonMesh().setHounsfieldValue(HU_soft_tissue);

    XRayBeam beam;
    beam.initialise(incident_energy);

    XRayDetector detector;
    detector.setResolutionInUnitsOfLengthPerPixel(0.15 * mm * 2.0);
    detector.setNumberOfPixels(Vec2ui(301, 1));
    detector.setDetectorPosition(VEC3(0.0, 0.0, 10.0 * cm));
    detector.setUpVector(VEC3(0.0, 1.0, 0.0));
    detector.setXrayPointSource(VEC3(0.0, 0.0, -10.0 * cm));
    detector.setParallelBeam();

    // Compute the image on the GPU with full precision
    XRayRenderer renderer;
    renderer.initialise(XRayRenderer::OPENGL, 255, &detector, &beam);
    renderer.addInnerSurface(&scene_graph.getChild("inside"));
    renderer.addOuterSurface(&scene_graph.getChild("outside"));
    renderer.enableArtifactFilteringOnGPU();

    Matrix4x4<GLfloat> identity;
    renderer.computeImage(identity);

    // Beer-Lambert law: the beam crosses 2 cm of bone and 1 cm of soft tissue at the centre
    const RATIONAL_NUMBER mu_bone = renderer.getMuFromHU(HU_bone, incident_energy) / (1.0 / cm);
    const RATIONAL_NUMBER mu_soft_tissue = renderer.getMuFromHU(HU_soft_tissue, incident_energy) / (1.0 / cm);
    const RATIONAL_NUMBER expected = incident_energy * exp(-(mu_bone * 2 + mu_soft_tissue * 1));

    const RATIONAL_NUMBER* energy_fluence = renderer.getEnergyFluence().getRawData();
    const RATIONAL_NUMBER simulated = energy_fluence[detector.getNumberOfPixels().getX() / 2];

    LOGGER.logNow("") << "On the adopted context, the energy transmitted at the centre is " <<
            simulated / keV << " keV (expected: " << expected / keV << " keV)" << std::endl;
    ASSERT_NEAR(std::abs(expected - simulated) / expected, 0.0, 0.01);

    // Releasing the adopted context frees the objects of the library but destroys nothing
    renderer.release();
    scene_graph.clear();
    g_adopted_context.release();
    ASSERT_FALSE(g_adopted_context.isReady());
    ASSERT_FALSE(g_adopted_context.isExternal());
    ASSERT_TRUE(g_application_context.isReady());
    ASSERT_NE(glGetString(GL_VERSION), nullptr);

    // The context of the application can be adopted again
    ASSERT_TRUE(g_adopted_context.adoptCurrentContext());
    ASSERT_TRUE(g_adopted_context.isExternal());

    // The adopted context is released first, then the application destroys its own
    g_adopted_context.release();
    ResourceManager::getInstance().setGraphicsContext(nullptr);
    g_application_context.release();
}
#endif
