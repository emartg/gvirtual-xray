/**
********************************************************************************
*
*   @file       SceneGraphBinder.cxx
*
*   @brief      Class to handle scenegraphes.
*
*   @version    1.0
*
*   @date       16/07/2018
*
*   @author     Dr Franck P. Vidal
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*   http://www.fpvidal.net/, Jul 2018, 2018, version 1.0, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <algorithm> //For std::min and std::max Windows only
#include <cfloat> //For FLT_MAX

#ifndef GVXR_SCENEGRAPH_BINDER_H
#include "gVirtualXRay/SceneGraphBinder.h"
#endif

// For types such as VEC3, MATRIX4
#ifndef GVXR_TYPES_H
#include "gVirtualXRay/Types.h"
#endif

// To initialise GLEW, to define matrix stacks, to define state stacks, etc.
#ifndef GVXR_OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef GVXR_NOT_IMPLEMENTED_EXCEPTION_H
#include "gVirtualXRay/NotImplementedException.h"
#endif


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;









SceneGraphBinder::SceneGraphBinder():
		m_root_node("root"),
		m_unit_of_length(0.0)
{}





SceneGraphBinder::SceneGraphBinder(const SceneGraphBinder& aSceneGraph):
        m_root_node("root"),
		m_unit_of_length(aSceneGraph.m_unit_of_length)
{
}


SceneGraphBinder::SceneGraphBinder(const std::string& aFileName, RATIONAL_NUMBER aUnitOfLength):
        m_root_node("root")
{
	loadSceneGraph(aFileName.data(), aUnitOfLength);
}


SceneGraphBinder::SceneGraphBinder(const char* aFileName, RATIONAL_NUMBER aUnitOfLength):
		m_root_node("root")
{
	loadSceneGraph(aFileName, aUnitOfLength);
}


SceneGraphBinder::~SceneGraphBinder()
{}


SceneGraphBinder& SceneGraphBinder::operator=(const SceneGraphBinder& aSceneGraph)
{
	m_unit_of_length = aSceneGraph.m_unit_of_length;
    throw NotImplementedException(__FILE__, __FUNCTION__, __LINE__);

	return (*this);
}


void SceneGraphBinder::loadSceneGraph(const char* aFileName, RATIONAL_NUMBER aUnitOfLength)
{
	throw Exception(__FILE__, __FUNCTION__, __LINE__, "Do not call");
}


void SceneGraphBinder::loadSceneGraph(const std::string& aFileName, RATIONAL_NUMBER aUnitOfLength)
{
	loadSceneGraph(aFileName.data(), aUnitOfLength);
}


void SceneGraphBinder::clear()
{
	m_root_node.clear();
	// m_root_node = SceneGraphNode("root");
}

/*
const PolygonMesh& SceneGraphBinder::getPolygonMesh(const std::string& aLabel) const
{
	return (m_p_polygon_mesh_set[aLabel]);
}


const PolygonMesh& SceneGraphBinder::getPolygonMesh(const char* aLabel) const
{
	return (m_p_polygon_mesh_set[std::string(aLabel)]);
}


PolygonMesh& SceneGraphBinder::getPolygonMesh(const std::string& aLabel)
{
	return (m_p_polygon_mesh_set[aLabel]);
}


PolygonMesh& SceneGraphBinder::getPolygonMesh(const char* aLabel)
{
	return (m_p_polygon_mesh_set[std::string(aLabel)]);
}*/


void SceneGraphBinder::scale(const std::string& aNodeLabel,
		const VEC3& aScalingFactorSet)
{
	scale(aNodeLabel.data(), aScalingFactorSet);
}


//--------------------------------------------------------------------
void SceneGraphBinder::rotate(const std::string& aNodeLabel,
		                      float aRotationAngleInDegrees,
							  const VEC3& aRotationAxis)
//--------------------------------------------------------------------
{
	rotate(aNodeLabel.data(), aRotationAngleInDegrees, aRotationAxis);
}


void SceneGraphBinder::translate(const std::string& aNodeLabel,
									 const VEC3& aTranslationVector)
{
	translate(aNodeLabel.data(), aTranslationVector);
}


MATRIX4 SceneGraphBinder::getLocalTransformationMatrix(const std::string& aNodeLabel) const
{
	return (getLocalTransformationMatrix(aNodeLabel.data()));
}


MATRIX4 SceneGraphBinder::getWorldTransformationMatrix(const std::string& aNodeLabel) const
{
	return (getWorldTransformationMatrix(aNodeLabel.data()));
}


void SceneGraphBinder::display(GLint aShaderID)
{
    throw NotImplementedException(__FILE__, __FUNCTION__, __LINE__);
	/*for (std::map<std::string, PolygonMesh>::iterator ite = getPolygonMeshSet().begin();
			ite != getPolygonMeshSet().end();
			++ite)
	{
		// Store the current transformation matrix
		pushModelViewMatrix();

		g_current_modelview_matrix *= ite->second.getTransformationMatrix();

		applyModelViewMatrix();

		// Get the material
		Material& material(ite->second.getMaterial());

		// Define colours inshader
		GLuint handle(glGetUniformLocation(aShaderID, "material_ambient"));
		glUniform4fv(handle, 1, &(material.getAmbientColour()[0]));
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		handle = glGetUniformLocation(aShaderID, "material_diffuse");
		glUniform4fv(handle, 1, &(material.getDiffuseColour()[0]));
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		handle = glGetUniformLocation(aShaderID, "material_specular");
		glUniform4fv(handle, 1, &(material.getSpecularColour()[0]));
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		handle = glGetUniformLocation(aShaderID, "material_shininess");
		glUniform1f(handle, material.getShininess());
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		ite->second.display();

		// Restore the current transformation matrix
		popModelViewMatrix();
	}*/
}


void SceneGraphBinder::scale(const char* aNodeLabel,
		const VEC3& aScalingFactorSet)
{
	SceneGraphNode* p_node = getNode(aNodeLabel);
	if (p_node)
	{
	    p_node->applyLocalTransformation(MATRIX4::buildScaleMatrix(aScalingFactorSet));
	}
}


void SceneGraphBinder::rotate(const char* aNodeLabel,
		float aRotationAngleInDegrees,
		const VEC3& aRotationAxis)
{
    SceneGraphNode* p_node = getNode(aNodeLabel);
    if (p_node)
    {
        p_node->applyLocalTransformation(MATRIX4::buildRotationMatrix(aRotationAngleInDegrees, aRotationAxis));
    }
}


void SceneGraphBinder::translate(const char* aNodeLabel,
		const VEC3& aTranslationVector)
{
    SceneGraphNode* p_node = getNode(aNodeLabel);
    if (p_node)
    {
        p_node->applyLocalTransformation(MATRIX4::buildTranslationMatrix(aTranslationVector));
    }
}


MATRIX4 SceneGraphBinder::getLocalTransformationMatrix(const char* aNodeLabel) const
{
    throw NotImplementedException(__FILE__, __FUNCTION__, __LINE__);
}


MATRIX4 SceneGraphBinder::getWorldTransformationMatrix(const char* aNodeLabel) const
{
    throw NotImplementedException(__FILE__, __FUNCTION__, __LINE__);
}


void SceneGraphBinder::moveToCentre(const std::string& aNodeLabel)
{
	moveToCentre(aNodeLabel.data());
}


void SceneGraphBinder::moveToCentre(const char* aNodeLabel)
{
    SceneGraphNode* p_node = getNode(aNodeLabel);
    if (p_node)
    {
    	VEC3 min_bbox = p_node->getNodeAndChildrenWorldMinCorner();
    	VEC3 max_bbox = p_node->getNodeAndChildrenWorldMaxCorner();

    	VEC3 translation_vector = -min_bbox;
    	translation_vector -= (max_bbox - min_bbox) / 2.0;
    	translate(aNodeLabel, translation_vector);
    }
}


void SceneGraphBinder::moveToCenter(const std::string& aNodeLabel)
{
	moveToCenter(aNodeLabel.data());
}


void SceneGraphBinder::moveToCenter(const char* aNodeLabel)
{
	moveToCentre(aNodeLabel);
}


void SceneGraphBinder::moveToCentre()
{
	moveToCentre("root");
}


void SceneGraphBinder::moveToCenter()
{
	moveToCentre();
}


MATRIX4 SceneGraphBinder::getRootNodeTransformationMatrix() const
{
    throw NotImplementedException(__FILE__, __FUNCTION__, __LINE__);
}


MATRIX4 SceneGraphBinder::getNodeLocalTransformationMatrix(const char* aNodeLabel) const
{
	return (getLocalTransformationMatrix(aNodeLabel));
}


MATRIX4 SceneGraphBinder::getNodeWorldTransformationMatrix(const char* aNodeLabel) const
{
	return (getWorldTransformationMatrix(aNodeLabel));
}


void SceneGraphBinder::setRootNodeTransformationMatrix(const MATRIX4& aTransformationMatrix)
{
    throw NotImplementedException(__FILE__, __FUNCTION__, __LINE__);
}


void SceneGraphBinder::setNodeLocalTransformationMatrix(const char* aNodeLabel, const MATRIX4& aTransformationMatrix)
{
    throw NotImplementedException(__FILE__, __FUNCTION__, __LINE__);
}


std::pair<VEC3, VEC3> SceneGraphBinder::getBoundingBox()
{
	VEC3 min_corner( FLT_MAX,  FLT_MAX,  FLT_MAX);
	VEC3 max_corner(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	// Create a waiting list
    std::vector<SceneGraphNode*> p_waiting_list;

    // Add the root node to the waiting list
    p_waiting_list.push_back(&m_root_node);

    // The waiting list is not empty
    while (p_waiting_list.size())
    {
    	// Get the last element of the list
    	SceneGraphNode* p_node = p_waiting_list.back();

    	// Remove the last element of the list
    	p_waiting_list.pop_back();

    	// It is not NULL
    	if (p_node)
    	{
    	    // Add its children to the waiting list
			for (unsigned int i = 0; i < p_node->getNumberOfChildren(); ++i)
			{
				p_waiting_list.push_back(&p_node->getChild(i));
			}

			// Process the node
			min_corner[0] = std::min(min_corner[0], p_node->getNodeAndChildrenWorldMinCorner()[0]);
			min_corner[1] = std::min(min_corner[1], p_node->getNodeAndChildrenWorldMinCorner()[1]);
			min_corner[2] = std::min(min_corner[2], p_node->getNodeAndChildrenWorldMinCorner()[2]);

			max_corner[0] = std::max(max_corner[0], p_node->getNodeAndChildrenWorldMaxCorner()[0]);
			max_corner[1] = std::max(max_corner[1], p_node->getNodeAndChildrenWorldMaxCorner()[1]);
			max_corner[2] = std::max(max_corner[2], p_node->getNodeAndChildrenWorldMaxCorner()[2]);
    	}
    }

	return std::pair<VEC3, VEC3>(min_corner, max_corner);
}


SceneGraphNode* SceneGraphBinder::getNode(const std::string& aLabel)
{
    SceneGraphNode* p_current_node = &m_root_node;
    std::vector<SceneGraphNode*> p_waiting_list;

    while (p_current_node)
    {
        if (p_current_node->getLabel() == aLabel)
        {
            return (p_current_node);
        }
        else if (p_current_node->getNumberOfChildren())
        {
            for (unsigned int i = 0; i < p_current_node->getNumberOfChildren(); ++i)
            {
                p_waiting_list.push_back(&p_current_node->getChild(i));
            }
        }

        p_current_node = 0;

        if (p_waiting_list.size())
        {
            p_current_node = p_waiting_list.back();
            p_waiting_list.pop_back();
        }
    }

    return 0;
}


const SceneGraphNode* SceneGraphBinder::getNode(const std::string& aLabel) const
{
    const SceneGraphNode* p_current_node = &m_root_node;
    std::vector<const SceneGraphNode*> p_waiting_list;

    while (p_current_node)
    {
        if (p_current_node->getLabel() == aLabel)
        {
            return (p_current_node);
        }
        else if (p_current_node->getNumberOfChildren())
        {
            for (unsigned int i = 0; i < p_current_node->getNumberOfChildren(); ++i)
            {
                p_waiting_list.push_back(&p_current_node->getChild(i));
            }
        }

        p_current_node = 0;

        if (p_waiting_list.size())
        {
            p_current_node = p_waiting_list.back();
            p_waiting_list.pop_back();
        }
    }

    return 0;
}


void SceneGraphBinder::addPolygonMesh(const std::string& aLabel,
                                      const PolygonMesh& aPolygonMesh,
                                      const VEC3& aPosition,
                                      const std::string& aParent)
{
    SceneGraphNode* p_parent = getNode(aParent);

    if (!p_parent)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "The parent node does not exist.");
    }

    p_parent->addChild(aLabel, aPolygonMesh, aPosition);
}
