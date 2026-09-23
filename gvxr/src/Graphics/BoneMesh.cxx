/**
********************************************************************************
*
*	@file		BoneMesh.cxx
*
*	@brief      Class to handle a polygon mesh for Bone.
*
*	@version	1.0
*
*	@date		18/07/2016
*
*	@author		Andréas Meuleman
*
********************************************************************************
*/

//******************************************************************************
//	Include
//******************************************************************************
#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#ifndef GVXR_BONE_MESH_H
#include "gVirtualXRay/BoneMesh.h"
#endif

#ifndef GVXR_UTILITIES_H
#include "gVirtualXRay/Utilities.h"
#endif

//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {

//----------------------------------------------
BoneMesh::BoneMesh(unsigned int texture_3D_size):
//----------------------------------------------
		InternalOrganMesh(false, 0, texture_3D_size)
//----------------------------------------------
{
	m_has_thin_bumps = 1;
	m_use_3D_bump_map = 1;
	getMaterial().setShininess(4);
	getMaterial().setSpecularColour(0.2,0.2,0.2,1.);
	getMaterial().setDiffuseColour(0.84,0.74,0.6,1.);
	getMaterial().setAmbientColour(0.84,0.74,0.6,1.);
	m_label = "bones";
}

//----------------------------------------
BoneMesh::~BoneMesh()
//----------------------------------------
{
	// TODO Auto-generated destructor stub
}

//----------------------------------------
bool BoneMesh::isSoftTissue() const
//----------------------------------------
{
	return false;
}

//----------------------------------------------
void BoneMesh::updateVBO(int aTypeOfVBO, int aTypeOfPrimitive)
//----------------------------------------------
{
	if (m_p_vertex_set && m_number_of_vertices)
	{
		// Create the VBO if needed
		if (!m_p_vbo.get())
		{
		    m_p_vbo.reset(createVBO());
		}

		// Set the type of VBO
		m_p_vbo->setBufferUsageHint(aTypeOfVBO);

		m_p_vbo->setVertexData(aTypeOfPrimitive, m_number_of_vertices, 3, m_vertex_data_type, m_p_vertex_set,
				getVertexNormalNumber(), 3, GL_FLOAT, &m_p_vertex_normal_set[0],
				0, 0, GL_NONE, 0,
				m_number_of_vertices, 3, GL_FLOAT, &m_texture_coordinates_3d[0]);

        if (m_p_index_set && m_number_of_indices &&
                m_index_data_type != GL_NONE)
            {
            m_p_vbo->setIndexData(m_number_of_indices,
                    m_index_data_type,
                    m_p_index_set);
            }
	}
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}

//----------------------------------------------
void BoneMesh::defineDisplayShaderInput(int aShaderID)
//----------------------------------------------
{
	float repeate_frequency(8.f/float(m_texture_size_3d));

	GLuint handle = glGetUniformLocation(aShaderID, "g_repeat_frequency");
	glUniform1fv(handle, 1,&repeate_frequency);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	InternalOrganMesh::defineDisplayShaderInput(aShaderID);
}

//----------------------------------------------
void BoneMesh::initialize()
//----------------------------------------------
{
	//initialize if needed
	if(m_need_initialize)
	{
		//load the shaders
		load3DBumpShader();

	    //initialize the buffers for the bump map
	    initialize3DBuffer(m_bump_map_texture, m_bump_map_frame_buffer, false);

		if(m_number_of_indices)
		{
			initialize3DTextureCoordinates();
		}
		else
		{
			mergeVertices();

			initialize3DTextureCoordinates();

			updateVBO(m_p_vbo->getBufferUsageHint(), m_p_vbo->getPrimitiveType());
		}

	    initGradientTab();
		initPermutation();
		generateTextures();

		m_need_initialize = false;

		// Cleanup for now
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}


//----------------------------------------------
void BoneMesh::generateTextures()
//----------------------------------------------
{
	generate3DTexture(m_bump_map_shader, m_bump_map_frame_buffer, m_bump_map_texture, TO_CREATE_BUMP_MAP);
}

//----------------------------------------------
void BoneMesh::defineBumpMapShaderInput(int shader_id)
//----------------------------------------------
{
	float bump_frequency(m_texture_size_3d);

	GLuint handle = glGetUniformLocation(shader_id, "g_bump_frequency");
	glUniform1fv(handle, 1,&bump_frequency);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	InternalOrganMesh::defineBumpMapShaderInput(shader_id);
}

//----------------------------------------------
void BoneMesh::defineColourTextureShaderInput(int shader_id)
//----------------------------------------------
{
	//Do nothing for the bones
}


} // namespace gVirtualXRay
