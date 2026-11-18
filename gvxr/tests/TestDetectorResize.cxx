/*

Copyright (c) 2026-present, Eric Martínez Gamero, Universidad Rey Juan Carlos,
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
*   @file       TestDetectorResize.cxx
*
*   @brief      Unit testing: the number of pixels of a detector changes on a
*               renderer that is already in use. After
*               XRayDetector::setNumberOfPixels(), a second call to
*               XRayRenderer::initialise() on the same renderer, with the same
*               detector and beam, gives the image of a new renderer: the inner
*               surfaces stay registered, with the local transformations of
*               their nodes and their materials, and the beam and the detector
*               keep the values set before. The outer surface is forgotten by
*               initialise() and must be added again.
*
*   @author     Eric Martínez Gamero
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

#include <algorithm>
#include <chrono>
#include <cmath>
#include <vector>

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

#ifndef GVXR_SCENEGRAPH_NODE_H
#include "gVirtualXRay/SceneGraphNode.h"
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


#ifdef HAS_OPENGL
namespace
{
    const RATIONAL_NUMBER g_HU_bone(1329.885683);
    const RATIONAL_NUMBER g_HU_soft_tissue(51.714752);
    const RATIONAL_NUMBER g_HU_fat(-100.0);


    //------------------------------------------------------------------------
    // A detector of small pixels, a point source, and a monochromatic beam, as
    // an application sets them before its first image
    struct Scene
    //------------------------------------------------------------------------
    {
        SceneGraphNode scene_graph{"root", nullptr};
        XRayBeam beam;
        XRayDetector detector;

        Scene(const Vec2ui& aNumberOfPixels)
        {
            scene_graph.addChild("cube", CubeMesh<GLdouble>(4.0 * cm));
            scene_graph.addChild("cylinder", CylinderMesh<GLdouble>(20, 6.0 * cm, 1.0 * cm));
            scene_graph.addChild("wrapper", CubeMesh<GLdouble>(12.0 * cm));
            scene_graph.getChild("cube").getPolygonMesh().setHounsfieldValue(g_HU_soft_tissue);
            scene_graph.getChild("cylinder").getPolygonMesh().setHounsfieldValue(g_HU_bone);
            scene_graph.getChild("wrapper").getPolygonMesh().setHounsfieldValue(g_HU_fat);

            beam.initialise(60.0 * keV);

            detector.setResolutionInUnitsOfLengthPerPixel(0.5 * mm);
            detector.setNumberOfPixels(aNumberOfPixels);
            detector.setDetectorPosition(VEC3(0.0, 0.0, 40.0 * cm));
            detector.setUpVector(VEC3(0.0, 1.0, 0.0));
            detector.setXrayPointSource(VEC3(0.0, 0.0, -40.0 * cm));
        }

        const SceneGraphNode* node(const char* aLabel)
        {
            return &scene_graph.getChild(aLabel);
        }
    };


    //------------------------------------------------------------------------
    // The renderer as an application sets it up (the meshes are inner surfaces)
    void setUp(XRayRenderer& aRenderer, Scene& aScene, bool anOuterSurfaceFlag = false)
    //------------------------------------------------------------------------
    {
        aRenderer.initialise(XRayRenderer::OPENGL, 255, &aScene.detector, &aScene.beam);
        aRenderer.addInnerSurface(aScene.node("cube"));
        aRenderer.addInnerSurface(aScene.node("cylinder"));
        if (anOuterSurfaceFlag)
        {
            aRenderer.addOuterSurface(aScene.node("wrapper"));
        }
        aRenderer.enableArtifactFilteringOnGPU();
    }


    //------------------------------------------------------------------------
    // Compute an image and copy it (the renderer reuses its own buffer)
    std::vector<RATIONAL_NUMBER> computeImage(XRayRenderer& aRenderer, const XRayDetector& aDetector)
    //------------------------------------------------------------------------
    {
        aRenderer.computeImage(Matrix4x4<GLfloat>());
        const RATIONAL_NUMBER* p_data = aRenderer.getEnergyFluence().getRawData();
        const std::size_t size(std::size_t(aDetector.getNumberOfPixels().getX()) * aDetector.getNumberOfPixels().getY());
        return std::vector<RATIONAL_NUMBER>(p_data, p_data + size);
    }


    //------------------------------------------------------------------------
    // The largest difference between two images, relative to the largest value
    RATIONAL_NUMBER getRelativeDifference(const std::vector<RATIONAL_NUMBER>& anImage,
                                          const std::vector<RATIONAL_NUMBER>& aReference)
    //------------------------------------------------------------------------
    {
        if (anImage.size() != aReference.size() || aReference.empty())
        {
            return 1.0;
        }

        RATIONAL_NUMBER max_difference(0.0);
        RATIONAL_NUMBER max_value(0.0);
        for (std::size_t i(0); i < aReference.size(); ++i)
        {
            max_difference = std::max(max_difference, RATIONAL_NUMBER(std::abs(anImage[i] - aReference[i])));
            max_value = std::max(max_value, RATIONAL_NUMBER(std::abs(aReference[i])));
        }

        return (max_value > 0.0) ? max_difference / max_value : max_difference;
    }


    //------------------------------------------------------------------------
    // The image of a new renderer with the same detector, beam, meshes,
    // transformations, and materials
    std::vector<RATIONAL_NUMBER> computeReference(Scene& aScene, bool anOuterSurfaceFlag = false)
    //------------------------------------------------------------------------
    {
        XRayRenderer renderer;
        setUp(renderer, aScene, anOuterSurfaceFlag);
        std::vector<RATIONAL_NUMBER> image(computeImage(renderer, aScene.detector));
        renderer.release();
        return image;
    }


    //------------------------------------------------------------------------
    // Change the transformation and the material of the meshes in place, as an
    // application does between two images
    void changeInPlace(XRayRenderer& aRenderer, Scene& aScene)
    //------------------------------------------------------------------------
    {
        MATRIX4 cube_transformation;
        cube_transformation.translate(VEC3(1.5 * cm, -1.0 * cm, 0.0));
        cube_transformation.rotate(30.0, VEC3(0.0, 0.0, 1.0));
        aScene.scene_graph.getChild("cube").setLocalTransformation(cube_transformation);

        MATRIX4 cylinder_transformation;
        cylinder_transformation.rotate(60.0, VEC3(1.0, 0.0, 0.0));
        aScene.scene_graph.getChild("cylinder").setLocalTransformation(cylinder_transformation);

        aScene.scene_graph.getChild("cube").getPolygonMesh().setHounsfieldValue(g_HU_bone);
        aScene.scene_graph.getChild("cylinder").getPolygonMesh().setHounsfieldValue(g_HU_soft_tissue);
        aRenderer.needUpdateSurfacePerMaterial();
    }


    //------------------------------------------------------------------------
    // Resize the detector of a renderer in use and compare with a new renderer
    void checkResize(const Vec2ui& aFirstSize, const Vec2ui& aSecondSize)
    //------------------------------------------------------------------------
    {
        Scene scene(aFirstSize);
        XRayRenderer renderer;
        setUp(renderer, scene);
        changeInPlace(renderer, scene);
        const std::vector<RATIONAL_NUMBER> first_image(computeImage(renderer, scene.detector));
        ASSERT_EQ(getRelativeDifference(first_image, computeReference(scene)), 0.0);

        // The new number of pixels, then a second initialise() on the same renderer
        scene.detector.setNumberOfPixels(aSecondSize);
        renderer.initialise(XRayRenderer::OPENGL, 255, &scene.detector, &scene.beam);
        const std::vector<RATIONAL_NUMBER> resized_image(computeImage(renderer, scene.detector));
        ASSERT_EQ(resized_image.size(), std::size_t(aSecondSize.getX()) * aSecondSize.getY());

        const RATIONAL_NUMBER difference(getRelativeDifference(resized_image, computeReference(scene)));
        LOGGER.logNow("") << aFirstSize.getX() << "x" << aFirstSize.getY() << " -> " <<
                aSecondSize.getX() << "x" << aSecondSize.getY() <<
                ": difference with a new renderer " << difference << std::endl;
        ASSERT_EQ(difference, 0.0);

        // Back to the first size: the image of the first computation again
        scene.detector.setNumberOfPixels(aFirstSize);
        renderer.initialise(XRayRenderer::OPENGL, 255, &scene.detector, &scene.beam);
        ASSERT_EQ(getRelativeDifference(computeImage(renderer, scene.detector), first_image), 0.0);

        renderer.release();
    }
}


//-------------------------------------------
TEST(TestDetectorResize, CreateOpenGLContext)
//-------------------------------------------
{
    initGL(g_GL_context);
    if (!g_GL_context.isReady())
    {
        GTEST_SKIP() << "No OpenGL context could be created";
    }
}


//------------------------------------------
TEST(TestDetectorResize, LargerDetector)
//------------------------------------------
{
    if (!g_GL_context.isReady()) GTEST_SKIP() << "No OpenGL context";
    checkResize(Vec2ui(64, 32), Vec2ui(256, 128));
}


//-------------------------------------------
TEST(TestDetectorResize, SmallerDetector)
//-------------------------------------------
{
    if (!g_GL_context.isReady()) GTEST_SKIP() << "No OpenGL context";
    checkResize(Vec2ui(256, 128), Vec2ui(32, 32));
}


//-----------------------------------------------
TEST(TestDetectorResize, NonSquareDetectors)
//-----------------------------------------------
{
    if (!g_GL_context.isReady()) GTEST_SKIP() << "No OpenGL context";
    checkResize(Vec2ui(256, 128), Vec2ui(48, 200));
    checkResize(Vec2ui(32, 512), Vec2ui(512, 32));
}


//--------------------------------------------------
TEST(TestDetectorResize, BeamAndDetectorAreKept)
//--------------------------------------------------
{
    if (!g_GL_context.isReady()) GTEST_SKIP() << "No OpenGL context";

    Scene scene(Vec2ui(128, 64));
    scene.beam.clear();
    scene.beam.addChannel(1.0, 45.0 * keV);
    scene.detector.setXrayPointSource(VEC3(1.0 * cm, 0.5 * cm, -35.0 * cm));
    scene.detector.setDetectorPosition(VEC3(-1.0 * cm, 0.0, 45.0 * cm));
    scene.detector.setUpVector(VEC3(0.2, 1.0, 0.0).normal());

    XRayRenderer renderer;
    setUp(renderer, scene);
    computeImage(renderer, scene.detector);

    const VEC3 source(scene.detector.getXraySourceCentre());
    const VEC3 position(scene.detector.getDetectorPosition());
    const VEC3 up(scene.detector.getUpVector());
    const VEC2 pixel_size(scene.detector.getPixelSpacingInUnitOfLength());

    scene.detector.setNumberOfPixels(Vec2ui(200, 100));
    renderer.initialise(XRayRenderer::OPENGL, 255, &scene.detector, &scene.beam);

    // initialise() reads the detector and the beam, and changes neither
    ASSERT_EQ(scene.beam.getEnergyChannelNumber(), 1u);
    ASSERT_FLOAT_EQ(scene.beam.getEnergyChannel(0).getPhotonEnergy(), 45.0 * keV);
    ASSERT_EQ(scene.detector.getXraySourceCentre(), source);
    ASSERT_EQ(scene.detector.getDetectorPosition(), position);
    ASSERT_EQ(scene.detector.getUpVector(), up);
    ASSERT_FLOAT_EQ(scene.detector.getPixelSpacingInUnitOfLength().getX(), pixel_size.getX());
    ASSERT_FLOAT_EQ(scene.detector.getPixelSpacingInUnitOfLength().getY(), pixel_size.getY());

    // and the image is the one of a new renderer with these values
    ASSERT_EQ(getRelativeDifference(computeImage(renderer, scene.detector), computeReference(scene)), 0.0);
    renderer.release();
}


//------------------------------------------------------
TEST(TestDetectorResize, OuterSurfaceMustBeAddedAgain)
//------------------------------------------------------
{
    if (!g_GL_context.isReady()) GTEST_SKIP() << "No OpenGL context";

    Scene scene(Vec2ui(64, 64));
    XRayRenderer renderer;
    setUp(renderer, scene, true);
    computeImage(renderer, scene.detector);

    scene.detector.setNumberOfPixels(Vec2ui(96, 48));
    renderer.initialise(XRayRenderer::OPENGL, 255, &scene.detector, &scene.beam);

    // initialise() forgets the outer surface: the image is the one of the inner surfaces only
    ASSERT_EQ(getRelativeDifference(computeImage(renderer, scene.detector), computeReference(scene, false)), 0.0);

    // added again, the image is the one of a new renderer with the outer surface
    renderer.addOuterSurface(scene.node("wrapper"));
    ASSERT_EQ(getRelativeDifference(computeImage(renderer, scene.detector), computeReference(scene, true)), 0.0);
    renderer.release();
}


//-------------------------------------
TEST(TestDetectorResize, CostOfResize)
//-------------------------------------
{
    if (!g_GL_context.isReady()) GTEST_SKIP() << "No OpenGL context";

    Scene scene(Vec2ui(256, 128));
    XRayRenderer renderer;
    setUp(renderer, scene);
    computeImage(renderer, scene.detector);

    // A resize as the application does it: the new number of pixels and a second initialise()
    const unsigned int number_of_resizes(10);
    std::vector<double> times;
    for (unsigned int i(0); i < number_of_resizes; ++i)
    {
        const Vec2ui size((i % 2) ? Vec2ui(256, 128) : Vec2ui(255, 129));
        const auto start(std::chrono::steady_clock::now());
        scene.detector.setNumberOfPixels(size);
        renderer.initialise(XRayRenderer::OPENGL, 255, &scene.detector, &scene.beam);
        glFinish();
        times.push_back(std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - start).count());
    }
    std::sort(times.begin(), times.end());
    LOGGER.logNow("") << "Resize of a 256x128 detector (setNumberOfPixels() and initialise()): median " <<
            times[times.size() / 2] << " ms, min " << times.front() << " ms, max " << times.back() <<
            " ms (" << number_of_resizes << " resizes)" << std::endl;

    // The image is still the one of a new renderer
    ASSERT_EQ(getRelativeDifference(computeImage(renderer, scene.detector), computeReference(scene)), 0.0);
    renderer.release();
}
#endif
