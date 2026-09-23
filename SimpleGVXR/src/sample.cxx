/*

Copyright (c) 2024-present, Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), 
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
*   @file       sample.cxx
*
*   @brief      Expose here all the functions of SimpleGVXR related to 
*               the sample. It includes its geometry and material composition.
*
*   @version    1.0
*
*   @date       10/07/2024
*
*   @author     Prof Franck P. Vidal
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, May 2024-present, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <iostream>
#include <memory>
#include <random>
#include <algorithm>

#ifndef GVXR_IMPLICIT_SURFACE_H
#include "gVirtualXRay/ImplicitSurface.h"
#endif

#ifndef GVXR_CYLINDER_MESH_H
#include "gVirtualXRay/CylinderMesh.h"
#endif

#ifndef GVXR_CUBE_MESH_H
#include "gVirtualXRay/CubeMesh.h"
#endif

#ifndef GVXR_CUBOID_MESH_H
#include "gVirtualXRay/CuboidMesh.h"
#endif

#ifndef GVXR_STEP_WEDGE_MESH_H
#include "gVirtualXRay/StepWedgeMesh.h"
#endif

#ifndef GVXR_EXCEPTION_H
#include "gVirtualXRay/Exception.h"
#endif

// To load a scenegraph
#ifndef GVXR_SCENEGRAPH_BINDER_H
#include "gVirtualXRay/SceneGraphBinder.h"
#endif

// To load a scenegraph using ASSIMP
#ifndef GVXR_ASSIMP_SCENEGRAPH_BINDER_H
#include "gVirtualXRay/AssimpSceneGraphBinder.h"
#endif

// To load a scenegraph using OpenSceneGraph
#ifndef GVXR_OpenSceneGraphBinder_h
#include "gVirtualXRay/OpenSceneGraphBinder.h"
#endif

#ifndef GVXR_SCENEGRAPH_NODE_H
#include "gVirtualXRay/SceneGraphNode.h"
#endif

#ifndef GVXR_XRAY_RENDERER_H
#include "gVirtualXRay/XRayRenderer.h"
#endif

#ifndef SGVXR_SimpleGVXR_h
#include "SimpleGVXR.h"
#endif

#ifndef SGVXR_Internal_h
#include "internal.h"
#endif

#ifndef SGVXR_Sample_h
#include "sample.h"
#endif


//******************************************************************************
//  Namespace
//******************************************************************************


//******************************************************************************
//  Function declarations
//******************************************************************************
double getCylinderVolume(const double& aRadius,
                         const double& aHeight);

double getSphereVolume(const double& aRadius);

bool isSpherePositionValid(double x, double y, double z,
                           double aRadius,
                           const std::vector<double>& apSpherePositionSet,
                           const std::vector<double>& apSphereRadiusSet);


//******************************************************************************
//  Global variables
//******************************************************************************
extern gVirtualXRay::XRayRenderer g_xray_renderer; // X-ray graphics engine
extern std::random_device g_random_device;  // Will be used to obtain a seed for the random number engine
extern std::mt19937 g_random_generator; // Standard mersenne_twister_engine seeded with rd()


//******************************************************************************
//  Function definitions
//******************************************************************************

//----------------------------------------------------
inline double getCylinderVolume(const double& aRadius,
                                const double& aHeight)
//----------------------------------------------------
{
    return M_PI * (aRadius * aRadius) * aHeight;
}


//--------------------------------------------------
inline double getSphereVolume(const double& aRadius)
//--------------------------------------------------
{
    return (4.0 / 3.0) * M_PI * (aRadius * aRadius * aRadius);
}


//-------------------------------------------------------------------------------
inline bool isSpherePositionValid(double x, double y, double z,
                                  double aRadius,
                                  const std::vector<double>& apSpherePositionSet,
                                  const std::vector<double>& apSphereRadiusSet)
//-------------------------------------------------------------------------------
{
    bool valid = true;
    
    for (unsigned int i = 0; i < apSphereRadiusSet.size(); ++i)
    {
        double distance = sqrt(
            pow(x - apSpherePositionSet[i * 3 + 0], 2) + 
            pow(y - apSpherePositionSet[i * 3 + 1], 2) + 
            pow(z - apSpherePositionSet[i * 3 + 2], 2)
        );
        
        if (distance <= (aRadius + apSphereRadiusSet[i]))
        {
            valid = false;
        }
    }

    return valid;
}


//-------------------------------------------------
void loadMeshFile(const std::string& aLabel,
                  const std::string& aFileName,
                  const std::string& aUnitOfLength,
				  bool addToRendererAsInnerSurface,
                  const std::string& aParent)
//-------------------------------------------------
{
    // Initialise the X-ray renderer if needed and if possible
    //initialiseXRayRenderer();

    gVirtualXRay::SceneGraphBinder* p_temp = g_p_scene_graph.get();
    if (!p_temp)
        {
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    // Add a new node to the scenegraph
    // Look for the parent
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aParent);

    // The parent does not exist
    if (!p_node)
    {
        throw gVirtualXRay::Exception(__FILE__,
                                        __FUNCTION__,
                                        __LINE__,
                                        "The parent node (" + aParent + ") does not exist.");
    }

    // Add the child
    p_node->addChild(aLabel);

    // Set the file name
    g_p_scene_graph->getNode(aLabel)->getPolygonMesh().setFileName(aFileName);

    // Load the file
    g_p_scene_graph->getNode(aLabel)->getPolygonMesh().loadSTLFile(false, true,
    true, false, getUnitOfLength(aUnitOfLength), GL_STATIC_DRAW);

    // Add the geometry to the X-ray renderer
    if (addToRendererAsInnerSurface)
    	g_xray_renderer.addInnerSurface(g_p_scene_graph->getNode(aLabel));
}


//----------------------------------------------------
std::string getMeshFilename(const std::string& aLabel)
//----------------------------------------------------
{
  // The scenegraph does not exist
  if (!g_p_scene_graph.get())
      {
      return std::string();
      }

  // Get the node
  gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

  // The node exists
  if (p_node)
      {
	  return p_node->getPolygonMesh().getFilename();
      }
  // The node does not exist
  else
      {
      return std::string();
      }
}


//--------------------------------------------------------
std::string getMeshUnitOfLength(const std::string& aLabel)
//--------------------------------------------------------
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
    {
        return std::string();
    }

    // Get the node
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // The node exists
    if (p_node)
    {
        double unit_of_length = p_node->getPolygonMesh().getUnitOfLength();

        if (unit_of_length == gVirtualXRay::um) return "um";
        if (unit_of_length == gVirtualXRay::mm) return "mm";
        if (unit_of_length == gVirtualXRay::cm) return "cm";
        if (unit_of_length == gVirtualXRay::dm) return "dm";
        if (unit_of_length == gVirtualXRay::m) return "m";
        if (unit_of_length == gVirtualXRay::dam) return "dam";
        if (unit_of_length == gVirtualXRay::hm) return "hm";
        if (unit_of_length == gVirtualXRay::km) return "km";
        return "N/A";
    }

    // The node does not exist
    return std::string();
}


//---------------------------------------------------
void loadSceneGraph(const std::string& aFileName,
                    const std::string& aUnitOfLength)
//---------------------------------------------------
{
    bool scene_graph_loaded = false;

#ifdef HAS_ASSIMP
    if (!scene_graph_loaded)
        {
        g_p_scene_graph.reset(new gVirtualXRay::AssimpSceneGraphBinder(
            aFileName,
            getUnitOfLength(aUnitOfLength)));

        scene_graph_loaded = true;
        }
#endif

#ifdef HAS_OPENSCENEGRAPH
    if (!scene_graph_loaded)
        {
        g_p_scene_graph.reset(new gVirtualXRay::OpenSceneGraphBinder(aFileName,
            getUnitOfLength(aUnitOfLength)));

        scene_graph_loaded = true;
        }
#endif

    if (!scene_graph_loaded)
        {
        gVirtualXRay::LOGGER.logError("gvxrWarning:\tGraphScene") <<
            aFileName <<
            " not loaded." <<
            std::endl;
        }
    // Add the geometry to the X-ray renderer
    else
        {
        // Look for all the meshes in the scenegraph
        std::vector<gVirtualXRay::SceneGraphNode*> p_temp_pull;
        p_temp_pull.push_back(g_p_scene_graph->getNode("root"));

        while (p_temp_pull.size())
            {
            // Get the last node
            gVirtualXRay::SceneGraphNode* p_node = p_temp_pull.back();

            // Remove the node
            p_temp_pull.pop_back();

            // Add its children
            for (unsigned int i = 0; i < p_node->getNumberOfChildren(); ++i)
                {
                p_temp_pull.push_back(&p_node->getChild(i));
                }

            // Add its geometry
            if (p_node->getPolygonMesh().getVertexNumber())
                {
                g_xray_renderer.addInnerSurface(p_node);
                }
            }
        }
}


//-----------------------------------------------------------
unsigned int getNumberOfPrimitives(const std::string& aLabel)
//-----------------------------------------------------------
{
  // The scenegraph does not exist
  if (!g_p_scene_graph.get())
      {
      return 0;
      }

  // Get the node
  gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

  // The node exists
  if (p_node)
      {
	  return p_node->getPolygonMesh().getTriangleNumber();
      }
  // The node does not exist
  else
      {
      return 0;
      }
}


//------------------------------------------------------------------
unsigned int getNumberOfFaceNormalVectors(const std::string& aLabel)
//------------------------------------------------------------------
{
  // The scenegraph does not exist
  if (!g_p_scene_graph.get())
      {
      return 0;
      }

  // Get the node
  gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

  // The node exists
  if (p_node)
      {
      return p_node->getPolygonMesh().getFaceNormalNumber();
      }
  // The node does not exist
  else
      {
      return 0;
      }
}


//--------------------------------------------------------------------
unsigned int getNumberOfVertexNormalVectors(const std::string& aLabel)
//--------------------------------------------------------------------
{
  // The scenegraph does not exist
  if (!g_p_scene_graph.get())
      {
      return 0;
      }

  // Get the node
  gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

  // The node exists
  if (p_node)
      {
      return p_node->getPolygonMesh().getVertexNormalNumber();
      }
  // The node does not exist
  else
      {
      return 0;
      }
}


//-----------------------------------------------------
unsigned int getVertexNumber(const std::string& aLabel)
//-----------------------------------------------------
{
  // The scenegraph does not exist
  if (!g_p_scene_graph.get())
      {
      return 0;
      }

  // Get the node
  gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

  // The node exists
  if (p_node)
      {
      return p_node->getPolygonMesh().getVertexNumber();
      }
  // The node does not exist
  else
      {
      return 0;
      }
}


//----------------------------------------------------
unsigned int getIndexNumber(const std::string& aLabel)
//----------------------------------------------------
{
  // The scenegraph does not exist
  if (!g_p_scene_graph.get())
      {
      return 0;
      }

  // Get the node
  gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

  // The node exists
  if (p_node)
      {
      return p_node->getPolygonMesh().getIndexNumber();
      }
  // The node does not exist
  else
      {
      return 0;
      }
}


//--------------------------------------------------------
std::vector<float> getVertexSet(const std::string& aLabel)
//--------------------------------------------------------
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
    {
        return std::vector<float>();
    }

    // Get the node
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // The node exists
    if (p_node)
    {
        unsigned int number_of_vertices = getVertexNumber(aLabel);
        std::vector<float> vertex_set(3 * number_of_vertices);

        gVirtualXRay::MATRIX4 transformation = p_node->getWorldTransformationMatrix();

        for (unsigned int i = 0; i < number_of_vertices; ++i)
        {
            gVirtualXRay::VEC3 vertex = transformation * p_node->getPolygonMesh().getVertex(i);

            vertex_set[i * 3 + 0] = vertex[0];
            vertex_set[i * 3 + 1] = vertex[1];
            vertex_set[i * 3 + 2] = vertex[2];
        }

        return vertex_set;
    }
    // The node does not exist
    else
    {
        return std::vector<float>();
    }
}


//------------------------------------------------------------------
std::vector<float> getFaceNormalVectorSet(const std::string& aLabel)
//------------------------------------------------------------------
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
    {
        return std::vector<float>();
    }

    // Get the node
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // The node exists
    if (p_node)
    {
        unsigned int number_of_normal_vectors = getNumberOfFaceNormalVectors(aLabel);
        std::vector<float> normal_vector_set(3 * number_of_normal_vectors);

        gVirtualXRay::MATRIX4 transformation = p_node->getWorldTransformationMatrix();

        for (unsigned int i = 0; i < number_of_normal_vectors; ++i)
        {
            gVirtualXRay::VEC3 normal_vector = transformation * p_node->getPolygonMesh().getFaceNormal(i);
            normal_vector_set[i * 3 + 0] = normal_vector[0];
            normal_vector_set[i * 3 + 1] = normal_vector[1];
            normal_vector_set[i * 3 + 2] = normal_vector[2];
        }

        return normal_vector_set;
    }
    // The node does not exist
    else
    {
        return std::vector<float>();
    }
}


//--------------------------------------------------------------------
std::vector<float> getVertexNormalVectorSet(const std::string& aLabel)
//--------------------------------------------------------------------
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
    {
        return std::vector<float>();
    }

    // Get the node
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // The node exists
    if (p_node)
    {
        unsigned int number_of_normal_vectors = getNumberOfVertexNormalVectors(aLabel);
        std::vector<float> normal_vector_set(3 * number_of_normal_vectors);

        gVirtualXRay::MATRIX4 transformation = p_node->getWorldTransformationMatrix();

        for (unsigned int i = 0; i < number_of_normal_vectors; ++i)
        {
        	gVirtualXRay::VEC3 normal_vector = transformation * p_node->getPolygonMesh().getVertexNormal(i);
            normal_vector_set[i * 3 + 0] = normal_vector[0];
            normal_vector_set[i * 3 + 1] = normal_vector[1];
            normal_vector_set[i * 3 + 2] = normal_vector[2];
        }

        return normal_vector_set;
    }
    // The node does not exist
    else
    {
        return std::vector<float>();
    }
}


std::vector<int> getIndexSet(const std::string& aLabel)
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
    {
        return std::vector<int>();
    }

    // Get the node
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // The node exists
    if (p_node)
    {
        unsigned int number_of_indices = getIndexNumber(aLabel);
        std::vector<int> index_set(number_of_indices);

        for (unsigned int i = 0; i < number_of_indices; ++i)
        {
            index_set[i] = p_node->getPolygonMesh().getIndex(i);
        }

        return index_set;
    }
    // The node does not exist
    else
    {
        return std::vector<int>();
    }
}


//--------------------------------------------
bool isInnerSurface(const std::string& aLabel)
//--------------------------------------------
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
    {
        return false;
    }

    // Get the node
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // The node exists
    if (p_node)
    {
        return g_xray_renderer.isInnerSurface(p_node);
    }
    // The node does not exist
    else
    {
        return false;
    }
}


//--------------------------------------------
bool isOuterSurface(const std::string& aLabel)
//--------------------------------------------
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
    {
        return false;
    }

    // Get the node
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // The node exists
    if (p_node)
    {
        return g_xray_renderer.isOuterSurface(p_node);
    }
    // The node does not exist
    else
    {
        return false;
    }
}


void emptyMesh(const std::string& aLabel, const std::string& aParent)
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    if (p_node)
        {
        p_node->setGeometry(gVirtualXRay::PolygonMesh());
        }
    else
        {
        g_p_scene_graph->addPolygonMesh(aLabel, gVirtualXRay::PolygonMesh(),
        gVirtualXRay::VEC3(), aParent);
        }
}


//---------------------------------------------------
void removeMeshAndChildren(const std::string& aLabel)
//---------------------------------------------------
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
    {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
    }

    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    if (aLabel == "root")
    {
        throw gVirtualXRay::Exception(__FILE__,
                            __FUNCTION__,
                            __LINE__,
                            "You cannot delete the root node.");
    }
    else
    {
        // The node exists
        if (p_node)
        {
            // Delete the node
            delete p_node;
        }
    }
}


void makeCube(const std::string& aLabel,
              double aLength,
              const std::string& aUnitOfLength,
              const std::string& aParent)
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    if (p_node)
        {
        p_node->setGeometry(gVirtualXRay::CubeMesh<double>(
            aLength * getUnitOfLength(aUnitOfLength),
            GL_UNSIGNED_INT));
        }
    else
        {
        g_p_scene_graph->addPolygonMesh(
            aLabel,
            gVirtualXRay::CubeMesh<double>(
                aLength * getUnitOfLength(aUnitOfLength),
                GL_UNSIGNED_INT),
            gVirtualXRay::VEC3(),
            aParent);
        }
}


//-----------------------------------------------
void makeCuboid(const std::string& aLabel,
                double aWidth,
                double aHeight,
                double aDepth,
                const std::string& aUnitOfLength,
                const std::string& aParent)
//-----------------------------------------------
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    if (p_node)
        {
        p_node->setGeometry(gVirtualXRay::CuboidMesh<double>(
            aWidth * getUnitOfLength(aUnitOfLength),
            aHeight * getUnitOfLength(aUnitOfLength),
            aDepth * getUnitOfLength(aUnitOfLength),
            GL_UNSIGNED_INT));
        }
    else
        {
        g_p_scene_graph->addPolygonMesh(
            aLabel,
            gVirtualXRay::CuboidMesh<double>(
                aWidth * getUnitOfLength(aUnitOfLength),
                aHeight * getUnitOfLength(aUnitOfLength),
                aDepth * getUnitOfLength(aUnitOfLength),
                GL_UNSIGNED_INT),
            gVirtualXRay::VEC3(),
            aParent);
        }
}


void makeCylinder(const std::string& aLabel,
                  unsigned int aNumberOfSectors,
                  double aHeight,
                  double aRadius,
                  const std::string& aUnitOfLength,
                  const std::string& aParent)
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    if (p_node)
        {
        p_node->setGeometry(gVirtualXRay::CylinderMesh<double>(
            aNumberOfSectors,
            aHeight * getUnitOfLength(aUnitOfLength),
            aRadius * getUnitOfLength(aUnitOfLength),
            GL_UNSIGNED_INT));
        }
    else
        {
        g_p_scene_graph->addPolygonMesh(
            aLabel,
            gVirtualXRay::CylinderMesh<double>(aNumberOfSectors,
                aHeight * getUnitOfLength(aUnitOfLength),
                aRadius * getUnitOfLength(aUnitOfLength),
                GL_UNSIGNED_INT),
            gVirtualXRay::VEC3(),
            aParent);
        }
}


//-----------------------------------------------
void makeSphere(const std::string& aLabel,
                unsigned int aNumberOfRings,
                unsigned int aNumberOfSectors,
                double aRadius,
                const std::string& aUnitOfLength,
                const std::string& aParent)
//-----------------------------------------------
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    if (p_node)
        {
        p_node->setGeometry(gVirtualXRay::SphereMesh(
            aNumberOfRings,
            aNumberOfSectors,
            aRadius * getUnitOfLength(aUnitOfLength),
            GL_UNSIGNED_INT));
        }
    else
        {
        g_p_scene_graph->addPolygonMesh(
            aLabel,
            gVirtualXRay::SphereMesh(
                aNumberOfRings,
                aNumberOfSectors,
                aRadius * getUnitOfLength(aUnitOfLength),
                GL_UNSIGNED_INT),
            gVirtualXRay::VEC3(),
            aParent);
        }
}


//--------------------------------------------------
void makeStepWedge(const std::string& aLabel,
                   unsigned int aNumberOfSteps,
                   float aTotalLength,
                   float aWidth,
                   float aFirstStepHeight,
                   float anOtherStepHeight,
                   const std::string& aUnitOfLength,
                   const std::string& aParent)
//--------------------------------------------------
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    if (p_node)
        {
        p_node->setGeometry(gVirtualXRay::StepWedgeMesh(
            aNumberOfSteps,
            aTotalLength * getUnitOfLength(aUnitOfLength),
            aWidth * getUnitOfLength(aUnitOfLength),
            aFirstStepHeight * getUnitOfLength(aUnitOfLength),
            anOtherStepHeight * getUnitOfLength(aUnitOfLength),
            GL_UNSIGNED_INT));
        }
    else
        {
        g_p_scene_graph->addPolygonMesh(
            aLabel,
            gVirtualXRay::StepWedgeMesh(
                aNumberOfSteps,
                aTotalLength * getUnitOfLength(aUnitOfLength),
                aWidth * getUnitOfLength(aUnitOfLength),
                aFirstStepHeight * getUnitOfLength(aUnitOfLength),
                anOtherStepHeight * getUnitOfLength(aUnitOfLength),
                GL_UNSIGNED_INT),
            gVirtualXRay::VEC3(),
            aParent);
        }
}


//----------------------------------------------------------
void makeIsoSurface(const std::string& aLabel,
                    short anIsoValue,
                    const std::vector<short>& aVoxelDataSet,
                    unsigned int aWidth,
                    unsigned int aHeight,
                    unsigned int aDepth,
                    double aSpacingX,
                    double aSpacingY,
                    double aSpacingZ,
                    const std::string& aUnitOfLength,
                    const std::string& aParent)
//----------------------------------------------------------
{
    gVirtualXRay::LOGGER.logError("Deprecated function: The function will be removed in future versions. ") <<
        "Please use the new functions provided in https://sourceforge.net/p/gvirtualxray/gitroot/ci/master/tree/SimpleGVXR/include/IsoSurface.h." <<
        std::endl;

    // Extract the isosurface
    float unit_of_length = getUnitOfLength(aUnitOfLength);
    gVirtualXRay::Image<short> voxel_data_set(
        &aVoxelDataSet[0],
        aWidth, aHeight, aDepth,
        gVirtualXRay::VEC3(aSpacingX * unit_of_length, 
            aSpacingY * unit_of_length, 
            aSpacingZ * unit_of_length)
    );

    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    if (p_node)
        {
        p_node->setGeometry(voxel_data_set.marchingCubes(anIsoValue));
        }
    else
        {
        g_p_scene_graph->addPolygonMesh(
            aLabel,
            voxel_data_set.marchingCubes(anIsoValue),
            gVirtualXRay::VEC3(),
            aParent);
        }
}


//-----------------------------------------------------------
void makeFoamInCuboid(const std::string& aPorosityName,
                      const std::string& aBoundingVolumeName,
                      double aCuboidWidth,
                      double aCuboidHeight,
                      double aCuboidDepth,
                      float aPorosityRadius,
                      float aPercentageOfVolume,
                      float aPercentageDeviation,
                      float aVoxelSpacing,
                      const std::string& aUnitOfLength,
                      unsigned char aVerboseLevel)
//-----------------------------------------------------------
{
    gVirtualXRay::LOGGER.logError("Deprecated function: The parameter \"aBoundingVolumeName\" is not used. The function will be removed in future versions.") <<
        std::endl;

    // Make sure that the foam pore meshes created earlier are destroyed
    destroyImplicitSurface(aPorosityName);
    createImplicitSurface(aPorosityName);

    // Get parent cube dimensions
    // double unit_of_length = getUnitOfLength(aUnitOfLength);

    // Define voxel parameters
    gVirtualXRay::Vec3ui number_of_voxels(aCuboidWidth / aVoxelSpacing,
        aCuboidHeight / aVoxelSpacing,
        aCuboidDepth / aVoxelSpacing);

    // Define pore parameters
    float standard_deviation = aPercentageDeviation * aPorosityRadius * 0.01;

    float pore_size_ratio = std::max(std::max(aCuboidWidth, aCuboidHeight), aCuboidDepth) / aPorosityRadius;
    unsigned int number_of_pores = (0.75 / M_PI) * (aPercentageOfVolume * 0.01) * (pow(pore_size_ratio, 3));

    // Randomly populate cube with pores
    std::uniform_real_distribution<float> distribution_x(-0.5 * aCuboidWidth, 0.5 * aCuboidWidth);
    std::uniform_real_distribution<float> distribution_y(-0.5 * aCuboidHeight, 0.5 * aCuboidHeight);
    std::uniform_real_distribution<float> distribution_z(-0.5 * aCuboidDepth, 0.5 * aCuboidDepth);

    std::normal_distribution<float> distribution_r(aPorosityRadius, standard_deviation);

    if (aVerboseLevel)
    {
        // gVirtualXRay::LOGGER.logNow("Bounding box lower corner: ") << min_corner << " " << aUnitOfLength << std::endl;
        // gVirtualXRay::LOGGER.logNow("Bounding box upper corner: ") << max_corner << " " << aUnitOfLength << std::endl;
        gVirtualXRay::LOGGER.logNow("Number of pores: ") << number_of_pores << std::endl;
        gVirtualXRay::LOGGER.logNow("Number of voxels: ") << number_of_voxels << std::endl;
    }

    for (unsigned int i = 0; i < number_of_pores; ++i)
    {
        gVirtualXRay::VEC3 pore_positions(distribution_x(g_random_generator),
            distribution_y(g_random_generator),
            distribution_z(g_random_generator));

        float pore_radius = distribution_r(g_random_generator);

        // Add metaball control points to create implicit surface
        addControlPoint(aPorosityName,
            pore_positions[0], pore_positions[1], pore_positions[2],
            aUnitOfLength,
            "META_BALL",
            1.0, pore_radius);
    }

    voxeliseImplicitSurface(aPorosityName,
        number_of_voxels[0], number_of_voxels[1], number_of_voxels[2],
        0, 0, 0,
        aVoxelSpacing, aVoxelSpacing, aVoxelSpacing,
        aUnitOfLength, aVerboseLevel);
}


//-----------------------------------------------------------
void makeTriangularMesh(const std::string& aLabel,
		                const std::vector<float>& aVertexSet,
						const std::string& aUnitOfLength,
						const std::string& aParent)
//-----------------------------------------------------------
{
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    if (p_node)
        {
        p_node->setGeometry(gVirtualXRay::PolygonMesh());
        }
    else
        {
        g_p_scene_graph->addPolygonMesh(
            aLabel,
			gVirtualXRay::PolygonMesh(),
            gVirtualXRay::VEC3(),
            aParent);

        p_node = g_p_scene_graph->getNode(aLabel);
        }

    p_node->getPolygonMesh().setInternalData(GL_TRIANGLES,
    		&aVertexSet,
             false,
			 GL_STATIC_DRAW);

    // Change scale and compute bounding box
    p_node->getPolygonMesh().applyScale(getUnitOfLength(aUnitOfLength));
}


//-------------------------------------------------------------------------
void makeTriangularMesh(const std::string& aLabel,
						const std::vector<float>& aVertexSet,
						const std::vector<int>& aTriangleIndexSet,
						const std::string& aUnitOfLength,
						const std::string& aParent)
//-------------------------------------------------------------------------
{
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    if (p_node)
        {
        p_node->setGeometry(gVirtualXRay::PolygonMesh());
        }
    else
        {
        g_p_scene_graph->addPolygonMesh(
            aLabel,
			gVirtualXRay::PolygonMesh(),
            gVirtualXRay::VEC3(),
            aParent);

        p_node = g_p_scene_graph->getNode(aLabel);
        }

    std::vector<unsigned int> p_temp;

    for (std::vector<int>::const_iterator ite(aTriangleIndexSet.begin());
    		ite != aTriangleIndexSet.end();
    		++ite)
    {
    	if (*ite < 0)
    	{
            throw gVirtualXRay::Exception(__FILE__,
                                          __FUNCTION__,
                                          __LINE__,
                                          "The index is negative.");
    	}

    	p_temp.push_back(*ite);
    }

    p_node->getPolygonMesh().setInternalData(GL_TRIANGLES,
			&aVertexSet,
			&p_temp,
			false,
			GL_STATIC_DRAW);

    // Change scale and compute bounding box
    p_node->getPolygonMesh().applyScale(getUnitOfLength(aUnitOfLength));
}


//-----------------------------------------------------------------------
unsigned int makeSpheresInCylinder(const std::string& aCylinderLabel,
                                   unsigned int aCylinderNumberOfSectors,
                                   double aCylinderRadius,
                                   double aCylinderHeight,
                                   const std::string& aSphereLabel,
                                   unsigned int aSphereNumberOfRings,
                                   unsigned int aSphereNumberOfSectors,
                                   double aSphereRadius,
                                   double aPercentVolume,
                                   const std::string& aUnitOfLength,
                                   const std::string& aParent)
//-----------------------------------------------------------------------
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
    {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
    }

    // Find the node in the scenegraph
    gVirtualXRay::SceneGraphNode* p_cylinder_node = g_p_scene_graph->getNode(aCylinderLabel);

    // The cylinder node exists, reset its geometry
    if (p_cylinder_node)
    {
        p_cylinder_node->setGeometry(gVirtualXRay::CylinderMesh<double>(
            aCylinderNumberOfSectors,
            aCylinderHeight * getUnitOfLength(aUnitOfLength),
            aCylinderRadius * getUnitOfLength(aUnitOfLength),
            GL_UNSIGNED_INT));
    }
    // The node does not exist, create a new geometry
    else
    {
        g_p_scene_graph->addPolygonMesh(
            aCylinderLabel,
            gVirtualXRay::CylinderMesh<double>(
                aCylinderNumberOfSectors,
                aCylinderHeight * getUnitOfLength(aUnitOfLength),
                aCylinderRadius * getUnitOfLength(aUnitOfLength),
                GL_UNSIGNED_INT),
            gVirtualXRay::VEC3(),
            aParent);

        p_cylinder_node = g_p_scene_graph->getNode(aCylinderLabel);
    }

    double cylinder_volume = getCylinderVolume(aCylinderRadius, aCylinderHeight);
    double sphere_volume = getSphereVolume(aSphereRadius);

    unsigned int targetted_number_of_spheres = ceil(aPercentVolume * cylinder_volume / sphere_volume);

    std::vector<double> p_kernel_position_set;
    std::vector<double> p_kernel_radius_set;

    std::uniform_real_distribution<> kernel_radius_distribution(0.8 * aSphereRadius, aSphereRadius);
    std::uniform_real_distribution<> angle_distribution(0.0, M_PI + M_PI);

    
    for (unsigned int i = 0; i < targetted_number_of_spheres; ++i)
    {
        bool reject = true;
        
        double x2, y2, z2;
        double kernel_radius;

        unsigned int count = 0;
        while (reject && count < 50)
        {
            kernel_radius = kernel_radius_distribution(g_random_generator);
            double angle = angle_distribution(g_random_generator);

            std::uniform_real_distribution<> distance_distribution(0.0, aCylinderRadius - kernel_radius);
            std::uniform_real_distribution<> height_distribution(kernel_radius, aCylinderHeight - kernel_radius);

            double d = distance_distribution(g_random_generator);

            double x = d;
            double y = 0;

            x2 = cos(angle) * x - sin(angle) * y;
            y2 = height_distribution(g_random_generator) - aCylinderHeight / 2.0;
            z2 = sin(angle) * x + cos(angle) * y;
            
            if (isSpherePositionValid(x2, y2, z2, kernel_radius, p_kernel_position_set, p_kernel_radius_set))
            {
                reject = false;
            }
            else
            {
                ++count;
            }
        }

        if (!reject)
        {
            p_kernel_position_set.push_back(x2);
            p_kernel_position_set.push_back(y2);
            p_kernel_position_set.push_back(z2);

            p_kernel_radius_set.push_back(kernel_radius);
        }
    }

    // Find the node in the scenegraph
    gVirtualXRay::SceneGraphNode* p_sphere_set_node = g_p_scene_graph->getNode(aSphereLabel);

    // There is at least one sphere
    if (p_kernel_radius_set.size() > 0)
    {
        // Add the 1st sphere
            
        // The sphere set node exists, reset its geometry
        if (p_sphere_set_node)
        {
            p_sphere_set_node->setGeometry(gVirtualXRay::SphereMesh(
                aSphereNumberOfRings,
                aSphereNumberOfSectors,
                aSphereRadius * getUnitOfLength(aUnitOfLength),
                GL_UNSIGNED_INT));
        }
        // The node does not exist, create a new geometry
        else
        {
            g_p_scene_graph->addPolygonMesh(
                aSphereLabel,
                gVirtualXRay::SphereMesh(
                    aSphereNumberOfRings,
                    aSphereNumberOfSectors,
                    aSphereRadius * getUnitOfLength(aUnitOfLength),
                    GL_UNSIGNED_INT),
                gVirtualXRay::VEC3(),
                aParent);

            p_sphere_set_node = g_p_scene_graph->getNode(aSphereLabel);
        }

        // Translate the sphere
        gVirtualXRay::VEC3 translation_vector(p_kernel_position_set[0], p_kernel_position_set[1], p_kernel_position_set[2]);
        p_sphere_set_node->getPolygonMesh().applyTranslation(translation_vector);

        // Add the remaining spheres
        for (unsigned int i = 1; i < p_kernel_radius_set.size(); ++i)
        {
            // Create the sphere
            gVirtualXRay::SphereMesh sphere(
                aSphereNumberOfRings,
                aSphereNumberOfSectors,
                aSphereRadius * getUnitOfLength(aUnitOfLength),
                GL_UNSIGNED_INT);

            // Translate the sphere
            gVirtualXRay::VEC3 translation_vector(p_kernel_position_set[i * 3 + 0],
                p_kernel_position_set[i * 3 + 1],
                p_kernel_position_set[i * 3 + 2]
            );

            sphere.applyTranslation(translation_vector);

            // Add the sphere to the set
            p_sphere_set_node->getPolygonMesh() += sphere;
        }

        // Subtract the sphere set from the cylinder
        p_cylinder_node->getPolygonMesh() -= p_sphere_set_node->getPolygonMesh();
    }

    return p_kernel_radius_set.size();
}


void addMesh(const std::string& aDestination, const std::string& aSource)
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
    {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
    }

    // Find the source. It must exist
    const gVirtualXRay::SceneGraphNode* p_source_node =
        g_p_scene_graph->getNode(aSource);

    // Find the destination. It must exist
    gVirtualXRay::SceneGraphNode* p_destination_node =
        g_p_scene_graph->getNode(aDestination);

    // The source does not exist
    if (!p_source_node)
    {
        throw gVirtualXRay::Exception(__FILE__,
                            __FUNCTION__,
                            __LINE__,
                            "The node (" + aSource + ") does not exist.");
    }

	// The destination does not exist
	if (!p_destination_node)
	{
	    throw gVirtualXRay::Exception(__FILE__,
                            __FUNCTION__,
                            __LINE__,
                            "The node (" + aDestination + ") does not exist.");
	}

	// Add the source to the destination
	p_destination_node->getPolygonMesh() += p_source_node->getPolygonMesh();
}


void subtractMesh(const std::string& aDestination, const std::string& aSource) {
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
    {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
    }

    // Find the source. It must exist
    const gVirtualXRay::SceneGraphNode* p_source_node =
        g_p_scene_graph->getNode(aSource);

    // Find the destination. It must exist
    gVirtualXRay::SceneGraphNode* p_destination_node =
        g_p_scene_graph->getNode(aDestination);

    // The source does not exist
    if (!p_source_node)
    {
        throw gVirtualXRay::Exception(__FILE__,
                            __FUNCTION__,
                            __LINE__,
                            "The node (" + aSource + ") does not exist.");
    }

	// The destination does not exist
	if (!p_destination_node)
	{
	    throw gVirtualXRay::Exception(__FILE__,
                            __FUNCTION__,
                            __LINE__,
                            "The node (" + aDestination + ") does not exist.");
	}

	// Subtract the source to the destination
	p_destination_node->getPolygonMesh() -= p_source_node->getPolygonMesh();
}


unsigned int getNumberOfChildren(const std::string& aLabel)
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    const gVirtualXRay::SceneGraphNode* p_node =
        g_p_scene_graph->getNode(aLabel);

    if (p_node)
        {
        return p_node->getNumberOfChildren();
        }
    else
        {
        throw gVirtualXRay::Exception(__FILE__,
                            __FUNCTION__,
                            __LINE__,
                            "The node (" + aLabel + ") does not exist.");
        }

    return 0;
}

std::string getChildLabel(const std::string& aLabel, unsigned int i)
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    const gVirtualXRay::SceneGraphNode* p_node =
        g_p_scene_graph->getNode(aLabel);

    if (p_node)
        {
        if (p_node->getNumberOfChildren())
            {
            return p_node->getChild(i).getLabel();
            }
        }

    return std::string();
}


//-------------------------------------------------------------------------
std::vector<double> getLocalTransformationMatrix(const std::string& aLabel)
//-------------------------------------------------------------------------
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    const gVirtualXRay::SceneGraphNode* p_node =
        g_p_scene_graph->getNode(aLabel);

    // The node exists
    if (p_node)
        {
        // Return its local transformation matrix
        return p_node->getLocalTransformationMatrix().getAsVector();
        }
    // The node does not exist
    else
        {
        throw gVirtualXRay::Exception(__FILE__,
                            __FUNCTION__,
                            __LINE__,
                            "The node (" + aLabel + ") does not exist.");
        }

    // Return the identity matrix
    return gVirtualXRay::MATRIX4().getAsVector();
}

//-------------------------------------------------------------------
void setLocalTransformationMatrix(const std::string& aLabel,
    const std::vector<double>& aMatrix)
//-------------------------------------------------------------------
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // The node exists
    if (p_node)
        {
        // Set its local transformation matrix
        p_node->setLocalTransformation(gVirtualXRay::MATRIX4(aMatrix));
        }
    // The node does not exist
    else
        {
        throw gVirtualXRay::Exception(__FILE__,
                            __FUNCTION__,
                            __LINE__,
                            "The node (" + aLabel + ") does not exist.");
        }
}


void applyCurrentLocalTransformation(const std::string& aLabel)
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // The node exists
    if (p_node)
        {
        // Process all the children first
        gVirtualXRay::MATRIX4 parent_transformation = p_node->getLocalTransformationMatrix();
        for (unsigned int i = 0; i < p_node->getNumberOfChildren(); ++i)
        {
            gVirtualXRay::SceneGraphNode& child = p_node->getChild(i);

            // Broadcast the parent's transformation first
            gVirtualXRay::MATRIX4 child_transformation = child.getLocalTransformationMatrix();
            child.setLocalTransformation(child_transformation * parent_transformation);

            // Resursively call the function
            applyCurrentLocalTransformation(child.getLabel());
        }

    	p_node->getPolygonMesh().applyTransform(parent_transformation);

        // Reset its local transformation matrix
        p_node->setLocalTransformation(gVirtualXRay::MATRIX4());
        }
    // The node does not exist
    else
        {
        throw gVirtualXRay::Exception(__FILE__,
                            __FUNCTION__,
                            __LINE__,
                            "The node (" + aLabel + ") does not exist.");
        }
}


//--------------------------------------
std::vector<double> getIdentityMatrix4()
//--------------------------------------
{
    std::vector<double> identity(16);

    identity[0] = 1.0;
    identity[1] = 0.0;
    identity[2] = 0.0;
    identity[3] = 0.0;

    identity[4] = 0.0;
    identity[5] = 1.0;
    identity[6] = 0.0;
    identity[7] = 0.0;

    identity[8] = 0.0;
    identity[9] = 0.0;
    identity[10] = 1.0;
    identity[11] = 0.0;

    identity[12] = 0.0;
    identity[13] = 0.0;
    identity[14] = 0.0;
    identity[15] = 1.0;

    return identity;
}


std::vector<double> getNodeAndChildrenBoundingBox(const std::string& aLabel,
    const std::string& aUnitOfLength)
{
    std::vector<double> bbox;

    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // The node exists
    if (p_node)
        {
        double unit_of_length = getUnitOfLength(aUnitOfLength);

        // Get its bounding box in world transformation
        gVirtualXRay::VEC3 min_corner =
        p_node->getNodeAndChildrenWorldMinCorner() / unit_of_length;
        gVirtualXRay::VEC3 max_corner =
        p_node->getNodeAndChildrenWorldMaxCorner() / unit_of_length;

        bbox.push_back(std::min(min_corner[0], max_corner[0]));
        bbox.push_back(std::min(min_corner[1], max_corner[1]));
        bbox.push_back(std::min(min_corner[2], max_corner[2]));

        bbox.push_back(std::max(min_corner[0], max_corner[0]));
        bbox.push_back(std::max(min_corner[1], max_corner[1]));
        bbox.push_back(std::max(min_corner[2], max_corner[2]));
        }
    // The node does not exist
    else
        {
        throw gVirtualXRay::Exception(__FILE__,
                            __FUNCTION__,
                            __LINE__,
                            "The node (" + aLabel + ") does not exist.");
        }

    return (bbox);
}

std::vector<double> getNodeOnlyBoundingBox(const std::string& aLabel,
                                           const std::string& aUnitOfLength)
{
    std::vector<double> bbox;

    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // The node exists
    if (p_node)
        {
        double unit_of_length = getUnitOfLength(aUnitOfLength);

        // Get its bounding box in world transformation
        gVirtualXRay::VEC3 min_corner =
            p_node->getNodeOnlyWorldMinCorner() / unit_of_length;

        gVirtualXRay::VEC3 max_corner =
            p_node->getNodeOnlyWorldMaxCorner() / unit_of_length;

        bbox.push_back(std::min(min_corner[0], max_corner[0]));
        bbox.push_back(std::min(min_corner[1], max_corner[1]));
        bbox.push_back(std::min(min_corner[2], max_corner[2]));

        bbox.push_back(std::max(min_corner[0], max_corner[0]));
        bbox.push_back(std::max(min_corner[1], max_corner[1]));
        bbox.push_back(std::max(min_corner[2], max_corner[2]));
        }
    // The node does not exist
    else
        {
        throw gVirtualXRay::Exception(__FILE__,
                            __FUNCTION__,
                            __LINE__,
                            "The node (" + aLabel + ") does not exist.");
        }

    return (bbox);
}


//---------------------------------------------------------------------------------------
std::vector<double> getNodeAndChildrenBoundingBoxCentre(const std::string& aLabel,
                                                        const std::string& aUnitOfLength)
//---------------------------------------------------------------------------------------
{
    std::vector<double> bbox = getNodeAndChildrenBoundingBox(aLabel, aUnitOfLength);

    std::vector<double> centre(3);
    centre[0] = bbox[0] + (bbox[3] - bbox[0]) / 2.0;
    centre[1] = bbox[1] + (bbox[4] - bbox[1]) / 2.0;
    centre[2] = bbox[2] + (bbox[5] - bbox[2]) / 2.0;

    return centre;
}

//---------------------------------------------------------------------------------------
std::vector<double> getNodeAndChildrenBoundingBoxCenter(const std::string& aLabel,
                                                        const std::string& aUnitOfLength)
//---------------------------------------------------------------------------------------
{
    return getNodeAndChildrenBoundingBoxCentre(aLabel, aUnitOfLength);

}

//--------------------------------------------------------------------------------
std::vector<double> getNodeOnlyBoundingBoxCentre(const std::string& aLabel,
                                                 const std::string& aUnitOfLength)
//--------------------------------------------------------------------------------
{
    std::vector<double> bbox = getNodeOnlyBoundingBox(aLabel, aUnitOfLength);

    std::vector<double> centre(3);
    centre[0] = bbox[0] + (bbox[3] - bbox[0]) / 2.0;
    centre[1] = bbox[1] + (bbox[4] - bbox[1]) / 2.0;
    centre[2] = bbox[2] + (bbox[5] - bbox[2]) / 2.0;

    return centre;
}


//--------------------------------------------------------------------------------
std::vector<double> getNodeOnlyBoundingBoxCenter(const std::string& aLabel,
                                                 const std::string& aUnitOfLength)
//--------------------------------------------------------------------------------
{
    return getNodeOnlyBoundingBoxCentre(aLabel, aUnitOfLength);
}


//-------------------------------------------------
void invertNormalVectors(const std::string& aLabel)
//-------------------------------------------------
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Invert the normal vectors
        p_node->getPolygonMesh().invertNormalVectors();
        }
    else
        {
        throw gVirtualXRay::Exception(__FILE__,
                            __FUNCTION__,
                            __LINE__,
                            "The node (" + aLabel + ") does not exist.");
        }
}


//------------------------------------------------------
bool hasNormalVectorsInverted(const std::string& aLabel)
//------------------------------------------------------
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Invert the normal vectors
        return (p_node->getPolygonMesh().hasNormalVectorsInverted());
        }
    else
        {
        throw gVirtualXRay::Exception(__FILE__,
                            __FUNCTION__,
                            __LINE__,
                            "The node (" + aLabel + ") does not exist.");
        }

    return false;
}


//-------------------------------------------------------------------------
void setColor(const std::string& aLabel, float R, float G, float B, float A)
//-------------------------------------------------------------------------
{
    setColour(aLabel, R, G, B, A);
}

//---------------------------------------------------------------------------
void setColour(const std::string& aLabel, float R, float G, float B, float A)
//---------------------------------------------------------------------------
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Set the colour
        p_node->getPolygonMesh().getMaterial().setAmbientColour(R, G, B, A);
        p_node->getPolygonMesh().getMaterial().setDiffuseColour(R, G, B, A);
        p_node->getPolygonMesh().getMaterial().setShininess(50.0);
        p_node->getPolygonMesh().getMaterial().setSpecularColour(
            1.0, 1.0, 1.0, 1.0);
        }
    else
        {
        throw gVirtualXRay::Exception(__FILE__,
                            __FUNCTION__,
                            __LINE__,
                            "The node (" + aLabel + ") does not exist.");
        }
}


//----------------------------------------------------------------------------------
void setAmbiantColour(const std::string& aLabel, float R, float G, float B, float A)
//----------------------------------------------------------------------------------
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Get the material's reference
        gVirtualXRay::Material& material = p_node->getPolygonMesh().getMaterial();
        material.setAmbientColour(R, G, B, A);
        }
    else
        {
        throw gVirtualXRay::Exception(__FILE__,
                            __FUNCTION__,
                            __LINE__,
                            "The node (" + aLabel + ") does not exist.");
        }
}


std::vector<float> getAmbientColour(const std::string& aLabel)
{
    std::vector<float> colour(4, 1.0);

    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Get the colour
        colour[0] = p_node->getPolygonMesh().getMaterial().
            getAmbientColour()[0];

        colour[1] = p_node->getPolygonMesh().getMaterial().
            getAmbientColour()[1];

        colour[2] = p_node->getPolygonMesh().getMaterial().
            getAmbientColour()[2];

        colour[3] = p_node->getPolygonMesh().getMaterial().
            getAmbientColour()[3];
        }
    else
        {
        throw gVirtualXRay::Exception(__FILE__,
                            __FUNCTION__,
                            __LINE__,
                            "The node (" + aLabel + ") does not exist.");
        }

    return colour;
}


//----------------------------------------------------------------------------------
void setDiffuseColour(const std::string& aLabel, float R, float G, float B, float A)
//----------------------------------------------------------------------------------
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Get the material's reference
        gVirtualXRay::Material& material = p_node->getPolygonMesh().getMaterial();
        material.setDiffuseColour(R, G, B, A);
        }
    else
        {
        throw gVirtualXRay::Exception(__FILE__,
                            __FUNCTION__,
                            __LINE__,
                            "The node (" + aLabel + ") does not exist.");
        }
}


std::vector<float> getDiffuseColour(const std::string& aLabel)
{
    std::vector<float> colour(4, 1.0);

    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Get the colour
        colour[0] = p_node->getPolygonMesh().getMaterial().
            getDiffuseColour()[0];

        colour[1] = p_node->getPolygonMesh().getMaterial().
            getDiffuseColour()[1];

        colour[2] = p_node->getPolygonMesh().getMaterial().
            getDiffuseColour()[2];

        colour[3] = p_node->getPolygonMesh().getMaterial().
            getDiffuseColour()[3];
        }
    else
        {
        throw gVirtualXRay::Exception(__FILE__,
                            __FUNCTION__,
                            __LINE__,
                            "The node (" + aLabel + ") does not exist.");
        }

    return colour;
}


//-----------------------------------------------------------------------------------
void setSpecularColour(const std::string& aLabel, float R, float G, float B, float A)
//-----------------------------------------------------------------------------------
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Get the material's reference
        gVirtualXRay::Material& material = p_node->getPolygonMesh().getMaterial();
        material.setSpecularColour(R, G, B, A);
        }
    else
        {
        throw gVirtualXRay::Exception(__FILE__,
                            __FUNCTION__,
                            __LINE__,
                            "The node (" + aLabel + ") does not exist.");
        }
}


std::vector<float> getSpecularColour(const std::string& aLabel)
{
    std::vector<float> colour(4, 1.0);

    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Get the colour
        colour[0] = p_node->getPolygonMesh().getMaterial().
            getSpecularColour()[0];

        colour[1] = p_node->getPolygonMesh().getMaterial().
            getSpecularColour()[1];

        colour[2] = p_node->getPolygonMesh().getMaterial().
            getSpecularColour()[2];

        colour[3] = p_node->getPolygonMesh().getMaterial().
            getSpecularColour()[3];
        }
    else
        {
        throw gVirtualXRay::Exception(__FILE__,
                            __FUNCTION__,
                            __LINE__,
                            "The node (" + aLabel + ") does not exist.");
        }

    return colour;
}


//-----------------------------------------------------------------
void setShininess(const std::string& aLabel, float aShininessValue)
//-----------------------------------------------------------------
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Get the material's reference
        gVirtualXRay::Material& material = p_node->getPolygonMesh().getMaterial();
        material.setShininess(aShininessValue);
        }
    else
        {
        throw gVirtualXRay::Exception(__FILE__,
                            __FUNCTION__,
                            __LINE__,
                            "The node (" + aLabel + ") does not exist.");
        }
}

float getShininess(const std::string& aLabel)
{
    float shininess = 0.0;

    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Get the colour
        shininess = p_node->getPolygonMesh().getMaterial().getShininess();
        }
    else
        {
        throw gVirtualXRay::Exception(__FILE__,
                            __FUNCTION__,
                            __LINE__,
                            "The node (" + aLabel + ") does not exist.");
        }

    return shininess;
}


//----------------------------------------------------------
void addPolygonMeshAsInnerSurface(const std::string& aLabel)
//----------------------------------------------------------
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Add the polygon mesh to the X-ray renderer
        g_xray_renderer.addInnerSurface(p_node);
        }
    else
        {
        throw gVirtualXRay::Exception(__FILE__,
                            __FUNCTION__,
                            __LINE__,
                            "The node (" + aLabel + ") does not exist.");
        }
}

//---------------------------------------------
void addPolygonMeshAsOuterSurface(const std::string& aLabel)
//---------------------------------------------
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Add the polygon mesh to the X-ray renderer
        g_xray_renderer.addOuterSurface(p_node);
        }
    else
        {
        throw gVirtualXRay::Exception(__FILE__,
                            __FUNCTION__,
                            __LINE__,
                            "The node (" + aLabel + ") does not exist.");
        }
}


//--------------------------------------
void removePolygonMeshesFromSceneGraph()
//--------------------------------------
{
    // The scenegraph exists
    if (g_p_scene_graph.get())
        {
        // Empty it
        g_p_scene_graph->clear();
        }

    removePolygonMeshesFromXRayRenderer();
}


//----------------------------------------
void removePolygonMeshesFromXRayRenderer()
//----------------------------------------
{
    g_xray_renderer.removeInnerSurfaces();
    g_xray_renderer.removeOuterSurface();
}


//--------------------------------------------
float getSOD(const std::string& aLabel,
             const std::string& aUnitOfLength)
//--------------------------------------------
{
    gVirtualXRay::VEC3 sample_position(convertVec3D(getNodeAndChildrenBoundingBoxCenter(aLabel, aUnitOfLength)));
    gVirtualXRay::VEC3 source_position(convertVec3D(getSourcePosition(aUnitOfLength)));

    return (source_position - sample_position).length();
}


//------------------------------------------------
float getBestSOD(const std::string& aLabel,
                 const std::string& aUnitOfLength,
                 float aMultiplicativeFactor)
//------------------------------------------------
{
    // Make sure it is at least one.
    aMultiplicativeFactor = std::max(aMultiplicativeFactor, 1.0f);

    // Compute the SDD.
    float sdd = getSDD(aUnitOfLength);

    // Compute the length of the sample's bounding box (i.e. the largest possible size of the sample).
    std::vector<double> bbox = getNodeAndChildrenBoundingBox(aLabel, aUnitOfLength);

    float diagonal_xyz = (gVirtualXRay::VEC3(bbox[3], bbox[4], bbox[5]) -
        gVirtualXRay::VEC3(bbox[0], bbox[1], bbox[2])).length();

    // Compute the ratios to apply the Thales's theorem.
    gVirtualXRay::VEC2 detector_size = convertVec2D(getDetectorSize(aUnitOfLength));
    float horizontal_ratio = diagonal_xyz / detector_size[0];
    float vertical_ratio = diagonal_xyz / detector_size[1];

    // Using the Thales's theorem, get the smallest theoretical SOD based on the bbox diagonal.
    float sod = std::max(horizontal_ratio * sdd, vertical_ratio * sdd);

    // Apply the multiplicative factor.
    // The sample will be further away from the source, i.e. closer to the detector,
    // i.e. looks smaller in the radiographs.
    sod *= aMultiplicativeFactor;

    // Make sure the detector cannot be in the middle of the sample.
    // Make sure the sample cannot be behind the detector.
    sod = std::min(sod, sdd - aMultiplicativeFactor * 0.5F * diagonal_xyz);

    return sod;
}


//---------------------------------------------
void applySOD(const std::string& aLabel,
              float aSourceObjectDistance,
              const std::string& aUnitOfLength)
//---------------------------------------------
{
    gVirtualXRay::VEC3 sample_centre(convertVec3D(getNodeAndChildrenBoundingBoxCentre(aLabel, aUnitOfLength)));
    gVirtualXRay::VEC3 source_position(convertVec3D(getSourcePosition(aUnitOfLength)));
    gVirtualXRay::VEC3 detector_position(convertVec3D(getDetectorPosition(aUnitOfLength)));

    gVirtualXRay::VEC3 view_vector = detector_position - source_position;
    view_vector.normalise();

    gVirtualXRay::VEC3 translation_vector = source_position + view_vector * aSourceObjectDistance;

    translateNode(aLabel,
        translation_vector[0], translation_vector[1], translation_vector[2],
        aUnitOfLength);
}


//-----------------
void moveToCentre()
//-----------------
{
    if (g_p_scene_graph.get())
    {
        g_p_scene_graph->moveToCentre();
    }
    else
    {
        throw gVirtualXRay::Exception(__FILE__,
                            __FUNCTION__,
                            __LINE__,
                            "The scenegraph is empty.");
    }
}

//-----------------
void moveToCenter()
//-----------------
{
    moveToCentre();
}

//-----------------------------------
void moveToCentre(const std::string& aLabel)
//-----------------------------------
{
    if (g_p_scene_graph.get())
    {
        g_p_scene_graph->moveToCentre(aLabel);
    }
    else
    {
        throw gVirtualXRay::Exception(__FILE__,
                            __FUNCTION__,
                            __LINE__,
                            "The scenegraph is empty.");
    }
}

//-----------------------------------
void moveToCenter(const std::string& aLabel)
//-----------------------------------
{
    moveToCentre(aLabel);
}

void scaleNode(const std::string& aLabel,
    double x,
    double y,
    double z)
{
    if (g_p_scene_graph.get())
    {
        // Find the mesh
        gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

        // It has been found
        if (p_node)
        {
            gVirtualXRay::MATRIX4 scaling_matrix =
                gVirtualXRay::MATRIX4::buildScaleMatrix(x, y, z);

            // Root node
            if (p_node->getLabel() == "root")
            {
                p_node->setLocalTransformation(
                    scaling_matrix
                    * p_node->getLocalTransformationMatrix());
            }
            else
            {
                p_node->setLocalTransformation(
                    p_node->getLocalTransformationMatrix()
                    * scaling_matrix);
            }
        }
        // It has not been found
        else
        {
            throw gVirtualXRay::Exception(__FILE__,
                                __FUNCTION__,
                                __LINE__,
                                "The node (" + aLabel + ") does not exist.");
        }
    }
    else
    {
        throw gVirtualXRay::Exception(__FILE__,
                            __FUNCTION__,
                            __LINE__,
                            "The scenegraph is empty.");
    }
}


void rotateNode(const std::string& aLabel,
    double angle,
    double x,
    double y,
    double z)
{
    if (g_p_scene_graph.get())
    {
        // Find the mesh
        gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

        // It has been found
        if (p_node)
        {
            gVirtualXRay::MATRIX4 rotation_matrix =
                gVirtualXRay::MATRIX4::buildRotationMatrix(angle, x, y, z);

            // Root node
            if (p_node->getLabel() == "root")
            {
                p_node->setLocalTransformation(
                    rotation_matrix
                    * p_node->getLocalTransformationMatrix());
            }
            else
            {
                p_node->setLocalTransformation(
                    p_node->getLocalTransformationMatrix() * rotation_matrix);
            }
        }
        // It has not been found
        else
        {
            throw gVirtualXRay::Exception(__FILE__,
                                __FUNCTION__,
                                __LINE__,
                                "The node (" + aLabel + ") does not exist.");
        }
    }
    else
    {
        throw gVirtualXRay::Exception(__FILE__,
                            __FUNCTION__,
                            __LINE__,
                            "The scenegraph is empty.");
    }
}


void rotateNode(const std::string& aLabel,
    double yaw,
    double pitch,
    double roll)
{
    if (g_p_scene_graph.get())
    {
        // Find the mesh
        gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

        // It has been found
        if (p_node)
        {
            gVirtualXRay::MATRIX4 rotation_matrix =
                gVirtualXRay::MATRIX4::buildYawPitchRollRotationMatrix(yaw, pitch, roll);

            // Root node
            if (p_node->getLabel() == "root")
            {
                p_node->setLocalTransformation(
                    rotation_matrix
                    * p_node->getLocalTransformationMatrix());
            }
            else
            {
                p_node->setLocalTransformation(
                    p_node->getLocalTransformationMatrix() * rotation_matrix);
            }
        }
        // It has not been found
        else
        {
            throw gVirtualXRay::Exception(__FILE__,
                                __FUNCTION__,
                                __LINE__,
                                "The node (" + aLabel + ") does not exist.");
        }
    }
    else
    {
        throw gVirtualXRay::Exception(__FILE__,
                            __FUNCTION__,
                            __LINE__,
                            "The scenegraph is empty.");
    }
}


void translateNode(const std::string& aLabel,
    double x,
    double y,
    double z,
    const std::string& aUnitOfLength)
{
    if (g_p_scene_graph.get())
    {
        // Find the mesh
        gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

        // It has been found
        if (p_node)
        {
            double unit_of_length = getUnitOfLength(aUnitOfLength);
            gVirtualXRay::MATRIX4 translation_matrix =
                gVirtualXRay::MATRIX4::buildTranslationMatrix(
                    x * unit_of_length, y * unit_of_length,
                    z * unit_of_length);

            // Root node
            if (p_node->getLabel() == "root")
            {
                p_node->setLocalTransformation(
                    translation_matrix
                    * p_node->getLocalTransformationMatrix());
            }
            else
            {
                p_node->setLocalTransformation(
                    p_node->getLocalTransformationMatrix()
                    * translation_matrix);
            }
        }
        // It has not been found
        else
        {
            throw gVirtualXRay::Exception(__FILE__,
                                __FUNCTION__,
                                __LINE__,
                                "The node (" + aLabel + ") does not exist.");
        }
    }
    else
    {
        throw gVirtualXRay::Exception(__FILE__,
                            __FUNCTION__,
                            __LINE__,
                            "The scenegraph is empty.");
    }
}


void shearNode(const std::string& aLabel,
    double yx,
    double zx,
    double xy,
    double zy,
    double xz,
    double yz)
{
    if (g_p_scene_graph.get())
    {
        // Find the mesh
        gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

        // It has been found
        if (p_node)
        {
            gVirtualXRay::MATRIX4 shear_matrix(
                1, yx, zx, 0,
                xy, 1, zy, 0,
                xz, yz, 1, 0,
                0, 0, 0, 1);

            // Root node
            if (p_node->getLabel() == "root")
            {
                p_node->setLocalTransformation(
                    shear_matrix *
                    p_node->getLocalTransformationMatrix());
            }
            else
            {
                p_node->setLocalTransformation(
                    p_node->getLocalTransformationMatrix() *
                    shear_matrix);
            }
        }
        // It has not been found
        else
        {
            throw gVirtualXRay::Exception(__FILE__,
                                __FUNCTION__,
                                __LINE__,
                                "The node (" + aLabel + ") does not exist.");
        }
    }
    else
    {
        throw gVirtualXRay::Exception(__FILE__,
                            __FUNCTION__,
                            __LINE__,
                            "The scenegraph is empty.");
    }
}
