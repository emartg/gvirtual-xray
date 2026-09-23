//******************************************************************************
//  Include
//******************************************************************************
#include <iostream>
#include <iomanip>

#include <fstream>
#include <vector>
#include <cmath>
#include <numeric>

#include "gtest/gtest.h"

#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#ifndef GVXR_IMAGE_H
#include "gVirtualXRay/Image.h"
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

/*
#ifndef GVXR_INVALID_INTERNAL_FORMAT_EXCEPTION_H
#include "gVirtualXRay/InvalidInternalFormalException.h"
#endif

#ifndef GVXR_PHOTON_CROSS_SECTION_H
#include "gVirtualXRay/PhotonCrossSection.h"
#endif

#ifndef GVXR_INVALID_IMAGE_SIZE_EXCEPTION_H
#include "gVirtualXRay/InvalidImageSizeException.h"
#endif

#ifndef GVXR_NO_DETECTOR_EXCEPTION_H
#include "gVirtualXRay/NoDetectorException.h"
#endif

#ifndef GVXR_NO_BEAM_EXCEPTION_H
#include "gVirtualXRay/NoBeamException.h"
#endif
*/
/*
#ifndef GVXR_utils_h
#include "utils.h"
#endif
*/

//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//  Function declaration
//******************************************************************************


//------------------------------------
TEST(TestAABB, TestAABB)
//------------------------------------
{
    // Set the geometry
    SceneGraphNode scene_graph("root", NULL);
    scene_graph.addChild("inside", CylinderMesh<float>(20, 3.0 * cm, 1.0 * cm));
    scene_graph.addChild("outside", CubeMesh<float>(5.0 * cm));

    SceneGraphNode& cylinder_node = scene_graph.getChild("inside");
    // SceneGraphNode& cube_node = scene_graph.getChild("outside");

    scene_graph.setLocalTransformation(MATRIX4::buildRotationMatrix(70, 1, 0, 0));
    //cylinder_node.setLocalTransformation(MATRIX4::buildRotationMatrix(-70, 0, 1, 0));
    Image<float> image(100, 100);
    for (int x = 0; x < 100; ++x)
    {
        for (int z = 0; z < 100; ++z)
        {
            std::map<RATIONAL_NUMBER, VEC3> p_intersection_set = cylinder_node.rayIntersectAABB(
                VEC3((-5. + 10. * float(x) / 99.) * cm, -10.0 * cm, (-5. + 10. * float(z) / 99.) * cm),
                VEC3(0.0, 1.0, 0.0));

            for (std::map<RATIONAL_NUMBER, VEC3>::const_iterator ite = p_intersection_set.begin();
                ite != p_intersection_set.end();
                ++ite)
            {
                image(x, z, 0) = ite->first;
                //                std::cout << ite->first << "\t" << ite->second << std::endl;
            }
        }
    }
    image.saveMHD("test.mhd");

    // Image();
    // Image(const Image<T>& anImage);
    // Image(const char* aFileName);
    // Image(const std::string& aFileName);
    // Image(const T* apData,
    //              unsigned int aWidth,
    //              unsigned int aHeight,
    //              unsigned int aNumberOfSlices = 1,
    //              const VEC3& aVoxelSize = VEC3(1, 1, 1));
    // Image(unsigned int aWidth,
    //              unsigned int aHeight,
    //              unsigned int aNumberOfSlices = 1,
    //              T aDefaultValue = 0);
}
