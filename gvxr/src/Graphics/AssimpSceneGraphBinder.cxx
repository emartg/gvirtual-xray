/**
********************************************************************************
*
*   @file       SceneGraphBinder.cxx
*
*   @brief      Class to handle scenegraphes using The Open-Asset-Importer-Lib
*   			(ASSIMP), see http://www.assimp.org/
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

#include <limits>
#include <fstream>
#include <cstdio>

// ASSIMP to load Collada files
#ifdef HAS_ASSIMP
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#endif

#ifndef GVXR_ASSIMP_SCENEGRAPH_BINDER_H
#include "gVirtualXRay/AssimpSceneGraphBinder.h"
#endif

// For types such as VEC3, MATRIX4
#ifndef GVXR_TYPES_H
#include "gVirtualXRay/Types.h"
#endif

// For exceptions
#ifndef GVXR_EXCEPTION_H
#include "gVirtualXRay/Exception.h"
#endif

#ifndef GVXR_FILE_DOES_NOT_EXIST_EXCEPTION_H
#include "gVirtualXRay/FileDoesNotExistException.h"
#endif


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;



//-----------------------------------------------
AssimpSceneGraphBinder::AssimpSceneGraphBinder():
//-----------------------------------------------
#ifdef HAS_ASSIMP
		SceneGraphBinder(),
		m_p_scene(0)
#else
		SceneGraphBinder()
#endif
//-----------------------------------------------
{
#ifndef HAS_ASSIMP
	throw Exception(__FILE__, __FUNCTION__, __LINE__, "ASSIMP is not supported in your implementation. To enable it, install ASSIMP and re-compile gVirtualXRay to support it.");
#endif
}


//----------------------------------------------------------------------------------------
AssimpSceneGraphBinder::AssimpSceneGraphBinder(const AssimpSceneGraphBinder& aSceneGraph):
//----------------------------------------------------------------------------------------
		SceneGraphBinder(aSceneGraph)
//----------------------------------------------------------------------------------------
{
#ifdef HAS_ASSIMP
	setSceneGraph(aSceneGraph.m_p_scene, m_unit_of_length);
#else
	throw Exception(__FILE__, __FUNCTION__, __LINE__, "ASSIMP is not supported in your implementation. To enable it, install ASSIMP and re-compile gVirtualXRay to support it.");
#endif
}


AssimpSceneGraphBinder::AssimpSceneGraphBinder(const char* aFileName,
	    									   RATIONAL_NUMBER aUnitOfLength)
{
	loadSceneGraph(aFileName, aUnitOfLength);
}


AssimpSceneGraphBinder::AssimpSceneGraphBinder(const std::string& aFileName,
	    									   RATIONAL_NUMBER aUnitOfLength)
{
	loadSceneGraph(aFileName.data(), aUnitOfLength);
}


//-----------------------------------------------
AssimpSceneGraphBinder::~AssimpSceneGraphBinder()
//-----------------------------------------------
{}


//--------------------------------------------------------------------------------------------------
AssimpSceneGraphBinder& AssimpSceneGraphBinder::operator=(const AssimpSceneGraphBinder& aSceneGraph)
//--------------------------------------------------------------------------------------------------
{
	SceneGraphBinder::operator=(aSceneGraph);

#ifdef HAS_ASSIMP
	setSceneGraph(aSceneGraph.m_p_scene, aSceneGraph.m_unit_of_length);
#endif

	return (*this);
}


#ifdef HAS_ASSIMP
//-----------------------------------------------------------------------
void AssimpSceneGraphBinder::setSceneGraph(const aiScene* apSceneGraph,
										   RATIONAL_NUMBER aUnitOfLength)
//-----------------------------------------------------------------------
{
	// Empty the scenegraph
	if (m_p_scene != apSceneGraph)
	{
		m_p_scene = const_cast<aiScene*>(apSceneGraph);
	}

	m_unit_of_length = aUnitOfLength;

	aiMatrix4x4 scaling_to_unit = aiMatrix4x4::Scaling(aiVector3D(m_unit_of_length, m_unit_of_length, m_unit_of_length), scaling_to_unit);

	recursivePolygonMeshBuild(m_p_scene->mRootNode, &m_root_node, scaling_to_unit);
}
#endif


//------------------------------------------------------------------------
void AssimpSceneGraphBinder::loadSceneGraph(const char* aFileName,
										    RATIONAL_NUMBER aUnitOfLength)
//------------------------------------------------------------------------
{
	// Check if file exists
	std::ifstream file_stream(aFileName);

	// The file exist
	if(!file_stream.fail())
	{
		file_stream.close();
	}
	// The file doest not exist
	else
	{
		// Throw an error
		throw FileDoesNotExistException(__FILE__, __FUNCTION__, __LINE__, aFileName);
	}

#ifdef HAS_ASSIMP
	// Load the file
	const aiScene* p_scene = m_importer.ReadFile( aFileName, aiProcessPreset_TargetRealtime_Quality);

	// The import failed
	if (!p_scene)
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, std::string("Cannot read file \"") + aFileName + "\". " + m_importer.GetErrorString());
	}

	// Load and set the scenegraph
	setSceneGraph(p_scene, aUnitOfLength);
#endif
}


#ifdef HAS_ASSIMP

void AssimpSceneGraphBinder::recursivePolygonMeshBuild(const aiNode* nd, SceneGraphNode* apParent, aiMatrix4x4 tStack)
{
	std::string node_label = nd->mName.C_Str();

	// Keep track of the transformation matrices of parents without geometry
	aiMatrix4x4 m =  nd->mTransformation ;
	tStack = tStack * m;
	m.Transpose();

	aiMatrix4x4 temp_matrix = tStack;
	temp_matrix.Transpose();

	MATRIX4 modelling_matrix(
			temp_matrix.a1, temp_matrix.a2, temp_matrix.a3, temp_matrix.a4,
			temp_matrix.b1, temp_matrix.b2, temp_matrix.b3, temp_matrix.b4,
			temp_matrix.c1, temp_matrix.c2, temp_matrix.c3, temp_matrix.c4,
			temp_matrix.d1, temp_matrix.d2, temp_matrix.d3, temp_matrix.d4);

	std::vector<float> p_vertex_set;
	std::vector<unsigned int> p_index_array;

	unsigned int index_offset = 0;

	for (unsigned int n=0; n < nd->mNumMeshes; ++n)
	{
		const aiMesh* mesh = m_p_scene->mMeshes[nd->mMeshes[n]];

		if (mesh->mPrimitiveTypes == aiPrimitiveType_TRIANGLE)
		{
			index_offset = p_vertex_set.size() / 3;

			for (unsigned int t = 0; t < mesh->mNumFaces; ++t)
			{
				const aiFace* face = &mesh->mFaces[t];

				for(unsigned int i = 0; i < face->mNumIndices; i++)
				{
					p_index_array.push_back(face->mIndices[i] + index_offset);
				}
			}

			for (unsigned int t = 0; t < mesh->mNumVertices; ++t)
			{
				aiVector3D vertex(mesh->mVertices[t]);

				p_vertex_set.push_back( (vertex.x));
				p_vertex_set.push_back( (vertex.y));
				p_vertex_set.push_back( (vertex.z));
			}
		}
	}

	SceneGraphNode* p_parent = apParent;

	if (p_vertex_set.size() && p_index_array.size())
	{
		apParent->addChild(node_label);
		SceneGraphNode& node = apParent->getChild(node_label);
		p_parent = &node;

		node.getPolygonMesh() = (PolygonMesh());

		node.getPolygonMesh().setInternalData(GL_TRIANGLES,
			&p_vertex_set,
			&p_index_array,
			true,
			GL_STATIC_DRAW);

		node.getPolygonMesh().computeNormalVectors();

		node.setLocalTransformation(modelling_matrix);

		// Keep track of the transformation matrices of parents without geometry
		// => the transformation has been taken into account
		// => reset the stack to the identity matrix
		tStack = aiMatrix4x4();
	}

	// all children
	for (unsigned int n=0; n < nd->mNumChildren; ++n)
	{
		recursivePolygonMeshBuild(nd->mChildren[n], p_parent, tStack);
	}
}



#endif
