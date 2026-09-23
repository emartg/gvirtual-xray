/*

Copyright (c) 2014, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
http://www.fpvidal.net/
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
*	@file		OpenGL2VBO.cxx
*
*	@brief		Class to handle vertex buffer objects (VBOs) in OpenGL 2.x.
*
*	@version	1.0
*
*	@date		07/08/2014
*
*	@author		Dr Franck P. Vidal
*
*	@section	License
*				BSD 3-Clause License.
*
*				For details on use and redistribution please refer
*				to http://opensource.org/licenses/BSD-3-Clause
*
*	@section	Copyright
*				(c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*				http://www.fpvidal.net/, Dec 2014, 2014, version 1.0,
*				BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
#ifdef HAS_GLEW
#include <GL/glew.h>
#endif

#ifndef GVXR_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <iostream>

#ifndef GVXR_OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef GVXR_RESOURCE_MANAGER_H
#include <gVirtualXRay/ResourceManager.h>
#endif

#ifndef OPENGL2_VBO_H
#include "gVirtualXRay/OpenGL2VBO.h"
#endif


//******************************************************************************
//	Name space
//******************************************************************************
using namespace gVirtualXRay;


//-----------------------
OpenGL2VBO::OpenGL2VBO():
//-----------------------
        VBO()
//-----------------------
{
    m_opengl_major_version = 2;

    ResourceManager::getInstance().addVBO(this);
}


//-----------------------
OpenGL2VBO::~OpenGL2VBO()
//-----------------------
{
	OpenGL2VBO::destroy();
    ResourceManager::getInstance().destroyAndRemoveVBO(this);
}


//------------------------------
void OpenGL2VBO::display() const
//------------------------------
{
#ifdef HAS_OPENGL
	if (m_primitive_type != GL_NONE)
	{
        if (m_number_of_normals)
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_id_set[NORMAL_SET_ID]);
            glEnableClientState(GL_NORMAL_ARRAY);
            glNormalPointer(m_normal_data_type, 0, NULL);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
        }

        if (m_number_of_additional_data)
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_id_set[ADDITIONAL_DATA_ID]);
            glEnableClientState(GL_COLOR_ARRAY);
            glNormalPointer(m_additional_data_type, 0, NULL);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
        }

        if (m_number_of_vertices)
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_id_set[VERTEX_SET_ID]);
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(m_number_of_compontents_per_vertex, m_vertex_data_type, 0, NULL);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
        }

        if (m_number_of_texture_coordinates)
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_id_set[TEXT_COORD_SET_ID]);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(m_number_of_compontents_per_texture_coord, m_texture_coordinate_data_type, 0, NULL);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
        }

		// Draw the polygon mesh
		if (!m_number_of_indices)
		{
			glDrawArrays(m_primitive_type, 0, m_number_of_vertices);
		}
		else
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id_set[INDEX_SET_ID]);
			glDrawElements(m_primitive_type, m_number_of_indices, m_index_data_type, 0);
		}
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        if (m_number_of_normals)
        {
            glDisableClientState(GL_NORMAL_ARRAY);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
        }

        if (m_number_of_additional_data)
        {
            glDisableClientState(GL_COLOR_ARRAY);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
        }

        if (m_number_of_vertices)
        {
            glDisableClientState(GL_VERTEX_ARRAY);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
        }

        if (m_number_of_texture_coordinates)
        {
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
        }

        if (m_number_of_indices)
        {
            glDisableClientState(GL_INDEX_ARRAY);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}
	else
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid primitive type.");
	}
#endif // HAS_OPENGL
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void OpenGL2VBO::setVertexData(int aPrimitiveType,
                               const unsigned int aNumberOfVertices,           unsigned int aNumberOfCompontentsPerVertex, int aVertexDataType, const void* vertices,
                               const unsigned int aNumberOfNormals,            unsigned int aNumberOfCompontentsPerNormal, int aNormalDataType, const void* apNormals,
                               const unsigned int aNumberOfTextureCoordinates, unsigned int aNumberOfCompontentsPerCoord,  int aCoordDataType,  const void* apTextureCoordinates,
							   const unsigned int aNumberOfAdditionalData,	   unsigned int aNumberOfCompontentsPerAdditionalData, int anAdditionalDataType, const void* apAdditionalData)
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
{
#ifdef HAS_OPENGL
	// Reset the internal data
	reset();

	// Add the VBO to the resource manager if needed
	ResourceManager::getInstance().addVBO(this);

	//Set the type to GL_NONE if use the default value (0)
	if(aVertexDataType == 0) aVertexDataType = GL_NONE;
	if(aNormalDataType == 0) aNormalDataType = GL_NONE;
	if(aCoordDataType == 0) aCoordDataType = GL_NONE;
	if(anAdditionalDataType == 0) anAdditionalDataType = GL_NONE;

	// Initialise class members
	m_primitive_type = aPrimitiveType;
	m_number_of_normals  = aNumberOfNormals;
    m_number_of_compontents_per_normal = aNumberOfCompontentsPerNormal;

    m_number_of_additional_data  = aNumberOfAdditionalData;
    m_number_of_compontents_per_additional_data = aNumberOfCompontentsPerAdditionalData;

	m_number_of_vertices = aNumberOfVertices;
    m_number_of_compontents_per_vertex = aNumberOfCompontentsPerVertex;

	m_number_of_texture_coordinates = aNumberOfTextureCoordinates;
	m_number_of_compontents_per_texture_coord = aNumberOfCompontentsPerCoord;

	m_normal_size             = m_number_of_normals             * aNumberOfCompontentsPerNormal;
	m_additional_data_size    = m_number_of_additional_data     * aNumberOfCompontentsPerAdditionalData;
	m_texture_coordinate_size = m_number_of_texture_coordinates * aNumberOfCompontentsPerCoord;
	m_vertex_size             = m_number_of_vertices            * aNumberOfCompontentsPerVertex;

	m_normal_data_type = aNormalDataType;
	m_additional_data_type = anAdditionalDataType;
	m_texture_coordinate_data_type = aCoordDataType;
	m_vertex_data_type = aVertexDataType;

	switch (m_normal_data_type)
	{
	case GL_FLOAT:
		m_normal_size *= sizeof(GLfloat);
		break;

	case GL_DOUBLE:
		m_normal_size *= sizeof(GLdouble);
		break;

	case GL_NONE:
		m_normal_size = 0;
		break;

	default:
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid data type.");
	}

	switch (m_additional_data_type)
	{
	case GL_FLOAT:
		m_additional_data_size *= sizeof(GLfloat);
		break;

	case GL_DOUBLE:
		m_additional_data_size *= sizeof(GLdouble);
		break;

	case GL_NONE:
		m_additional_data_size = 0;
		break;

	default:
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid data type.");
	}

	switch (m_texture_coordinate_data_type)
	{
	case GL_FLOAT:
		m_texture_coordinate_size *= sizeof(GLfloat);
		break;

	case GL_DOUBLE:
		m_texture_coordinate_size *= sizeof(GLdouble);
		break;

	case GL_NONE:
		m_texture_coordinate_size = 0;
		break;

	default:
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid data type.");
	}

	switch (m_vertex_data_type)
	{
	case GL_FLOAT:
		m_vertex_size *= sizeof(GLfloat);
		break;

	case GL_DOUBLE:
		m_vertex_size *= sizeof(GLdouble);
		break;

	default:
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid data type.");
	}

	// Load the data in the VBO
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// There is normal data
	if (apNormals)
	{
		// Create the VBO
		if (!m_id_set[NORMAL_SET_ID])
		{
			glGenBuffers(1, &m_id_set[NORMAL_SET_ID]);
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
		}

		// Bind the new VBO
		glBindBuffer(GL_ARRAY_BUFFER, m_id_set[NORMAL_SET_ID]);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		glBufferData(GL_ARRAY_BUFFER, m_normal_size, apNormals, m_storage_type);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

	// There is additional data
	if (apAdditionalData)
	{
		// Create the VBO
		if (!m_id_set[ADDITIONAL_DATA_ID])
		{
			glGenBuffers(1, &m_id_set[ADDITIONAL_DATA_ID]);
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
		}

		// Bind the new VBO
		glBindBuffer(GL_ARRAY_BUFFER, m_id_set[ADDITIONAL_DATA_ID]);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		glBufferData(GL_ARRAY_BUFFER, m_additional_data_size, apAdditionalData, m_storage_type);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

	// There are texture coordinates
	if (apTextureCoordinates)
	{
		// Create the VBO
		if (!m_id_set[TEXT_COORD_SET_ID])
		{
			glGenBuffers(1, &m_id_set[TEXT_COORD_SET_ID]);
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
		}

		// Bind the new VBO
		glBindBuffer(GL_ARRAY_BUFFER, m_id_set[TEXT_COORD_SET_ID]);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		glBufferData(GL_ARRAY_BUFFER, m_texture_coordinate_size, apTextureCoordinates, m_storage_type);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

	// There are vertex coordinates
	if (vertices)
	{
		// Create the VBO
		if (!m_id_set[VERTEX_SET_ID])
		{
			glGenBuffers(1, &m_id_set[VERTEX_SET_ID]);
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
		}

		// Bind the new VBO
		glBindBuffer(GL_ARRAY_BUFFER, m_id_set[VERTEX_SET_ID]);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		glBufferData(GL_ARRAY_BUFFER, m_vertex_size, vertices, m_storage_type);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

	if (m_id_set[VERTEX_SET_ID] != 0 && m_number_of_vertices == 0)
	{
		glDeleteBuffers(1, &m_id_set[VERTEX_SET_ID]);
		m_id_set[VERTEX_SET_ID] = 0;
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

	if (m_id_set[TEXT_COORD_SET_ID] != 0 && m_texture_coordinate_size == 0)
	{
		glDeleteBuffers(1, &m_id_set[TEXT_COORD_SET_ID]);
		m_id_set[TEXT_COORD_SET_ID] = 0;
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

	if (m_id_set[NORMAL_SET_ID] != 0 && m_normal_size == 0)
	{
		glDeleteBuffers(1, &m_id_set[NORMAL_SET_ID]);
		m_id_set[NORMAL_SET_ID] = 0;
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

	if (m_id_set[ADDITIONAL_DATA_ID] != 0 && m_additional_data_size == 0)
	{
		glDeleteBuffers(1, &m_id_set[ADDITIONAL_DATA_ID]);
		m_id_set[ADDITIONAL_DATA_ID] = 0;
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

	if (m_id_set[INDEX_SET_ID] != 0 && m_number_of_indices == 0)
	{
		glDeleteBuffers(1, &m_id_set[INDEX_SET_ID]);
		m_id_set[INDEX_SET_ID] = 0;
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

	// Unbind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
#endif // HAS_OPENGL
}


//----------------------------------------------------------------
void OpenGL2VBO::setIndexData(const unsigned int aNumberOfIndexes,
                              int anIndexDataType,
                              const void* apIndexSet)
//----------------------------------------------------------------
{
#ifdef HAS_OPENGL
	m_number_of_indices = aNumberOfIndexes;


	// Create the VBO
	if (!m_id_set[INDEX_SET_ID] && m_number_of_indices && apIndexSet)
	{
		glGenBuffers(1, &m_id_set[INDEX_SET_ID]);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

	m_index_size = aNumberOfIndexes;
	m_index_data_type = anIndexDataType;
	switch (m_index_data_type)
	{
	case GL_UNSIGNED_INT:
		m_index_size *= sizeof(GLuint);
		break;

	case GL_UNSIGNED_SHORT:
		m_index_size *= sizeof(GLushort);
		break;

	case GL_UNSIGNED_BYTE:
		m_index_size *= sizeof(GLubyte);
		break;

	default:
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid data type.");
	}

	// Load data
	if (m_number_of_indices && apIndexSet && m_index_size)
	{
		// Bind the new VBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id_set[INDEX_SET_ID]);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		// Load the data in the VBO
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_index_size, apIndexSet, m_storage_type);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		// Unbind the VBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}
	// There is no data to load
	else
	{
		// There is no need for a buffer
		if (m_id_set[INDEX_SET_ID])
		{
			glDeleteBuffers(1, &m_id_set[INDEX_SET_ID]);
			m_id_set[INDEX_SET_ID] = 0;
	        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
		}
	}
#endif // HAS_OPENGL
}
