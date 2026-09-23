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
*   @file       OpenGL3VBO.cxx
*
*   @brief      Class to handle vertex buffer objects (VBOs) in OpenGL 3.x or 4.x.
*
*   @version    1.0
*
*   @date       07/08/2014
*
*   @author     Dr Franck P. Vidal
*
*   @section    License
*               BSD 3-Clause License.
*
*               For details on use and redistribution please refer
*               to http://opensource.org/licenses/BSD-3-Clause
*
*   @section    Copyright
*               (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*               http://www.fpvidal.net/, Dec 2014, 2014, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <iostream>

#ifndef GVXR_OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef GVXR_RESOURCE_MANAGER_H
#include <gVirtualXRay/ResourceManager.h>
#endif

#ifndef GVXR_OPENGL3_VBO_H
#include "gVirtualXRay/OpenGL3VBO.h"
#endif


//******************************************************************************
//	Name space
//******************************************************************************
using namespace gVirtualXRay;


  //Define this somewhere in your header file
  #define BUFFER_OFFSET(i) ((void*)(i))

//----------------------------------------------
OpenGL3VBO::OpenGL3VBO():
//----------------------------------------------
		VBO(),
		m_vao_id(0)
//----------------------------------------------
{
    ResourceManager::getInstance().addVBO(this);

	m_id_set[VERTEX_SET_ID]     = 0;
	m_id_set[TEXT_COORD_SET_ID] = 0;
	m_id_set[NORMAL_SET_ID]     = 0;
    m_id_set[ADDITIONAL_DATA_ID]= 0;
    m_id_set[INDEX_SET_ID]      = 0;

    m_opengl_major_version = 3;
}


//-----------------------
OpenGL3VBO::~OpenGL3VBO()
//-----------------------
{
	OpenGL3VBO::destroy();
    ResourceManager::getInstance().destroyAndRemoveVBO(this);
}


//------------------------
void OpenGL3VBO::destroy()
//------------------------
{
    Context* p_context = ResourceManager::getInstance().getGraphicsContext();
    if (m_vao_id && p_context)
    {
#ifdef HAS_OPENGL
        if (p_context->isReady())
        {
            glDeleteVertexArrays(1, &m_vao_id);
        }
#endif // HAS_OPENGL

        m_vao_id = 0;
    }

    VBO::destroy();
}


//------------------------------
void OpenGL3VBO::display() const
//------------------------------
{
#ifdef HAS_OPENGL
	if (m_primitive_type != GL_NONE)
	{
	    // Bind the VAO
		glBindVertexArray(m_vao_id);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		// Draw the polygon mesh
		if (!m_number_of_indices)
		{
			glDrawArrays(m_primitive_type, 0, m_number_of_vertices);

		    // Check the OpenGL error status
		    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
		}
		else
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id_set[INDEX_SET_ID]);
			glDrawElements(m_primitive_type, m_number_of_indices, m_index_data_type, BUFFER_OFFSET(0));

		    // Check the OpenGL error status
		    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
		}

		// Unbind the VAO
		glBindVertexArray(0);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}
	else
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid primitive type.");
	}
#endif // HAS_OPENGL
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void OpenGL3VBO::setVertexData(int aPrimitiveType,
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

	// There is no VAO
	if (!m_vao_id)
	{
		// Create a VAO
		glGenVertexArrays(1, &m_vao_id);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

	// Bind the VAO
	glBindVertexArray(m_vao_id);
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

		glEnableVertexAttribArray(2);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		glVertexAttribPointer(2, m_number_of_compontents_per_normal, m_normal_data_type, GL_FALSE, 0, 0);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}
	else
	{
		glDisableVertexAttribArray(2);
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

		glEnableVertexAttribArray(3);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		glVertexAttribPointer(3, m_number_of_compontents_per_additional_data, m_additional_data_type, GL_FALSE, 0, 0);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}
	else
	{
		glDisableVertexAttribArray(3);
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

		glEnableVertexAttribArray(1);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		glVertexAttribPointer(1, m_number_of_compontents_per_texture_coord, m_texture_coordinate_data_type, GL_FALSE, 0, 0);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}
	else
	{
		glDisableVertexAttribArray(1);
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

		glEnableVertexAttribArray(0);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		glVertexAttribPointer(0, m_number_of_compontents_per_vertex, m_vertex_data_type, GL_FALSE, 0, 0);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}
	else
	{
		glDisableVertexAttribArray(0);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

	if (m_id_set[VERTEX_SET_ID] != 0 && m_number_of_vertices == 0)
	{
		glDeleteBuffers(1, &m_id_set[VERTEX_SET_ID]);
		m_id_set[VERTEX_SET_ID] = 0;
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		glDisableVertexAttribArray(0);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

	if (m_id_set[TEXT_COORD_SET_ID] != 0 && m_texture_coordinate_size == 0)
	{
		glDeleteBuffers(1, &m_id_set[TEXT_COORD_SET_ID]);
		m_id_set[TEXT_COORD_SET_ID] = 0;
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		glDisableVertexAttribArray(1);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

	if (m_id_set[NORMAL_SET_ID] != 0 && m_normal_size == 0)
	{
		glDeleteBuffers(1, &m_id_set[NORMAL_SET_ID]);
		m_id_set[NORMAL_SET_ID] = 0;
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		glDisableVertexAttribArray(2);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

	if (m_id_set[ADDITIONAL_DATA_ID] != 0 && m_additional_data_size == 0)
	{
		glDeleteBuffers(1, &m_id_set[ADDITIONAL_DATA_ID]);
		m_id_set[ADDITIONAL_DATA_ID] = 0;
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		glDisableVertexAttribArray(3);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

	if (m_id_set[INDEX_SET_ID] != 0 && m_number_of_indices == 0)
	{
		glDeleteBuffers(1, &m_id_set[INDEX_SET_ID]);
		m_id_set[INDEX_SET_ID] = 0;
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

	// Unbind the VAO
	glBindVertexArray(0);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Unbind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
#endif // HAS_OPENGL
}



//----------------------------------------------------------------
void OpenGL3VBO::setVertexVBO(
	//----------------------------------------------------------------
	unsigned int number_of_vertices,
	unsigned int aVertexVBO,
	unsigned int aNormalVBO,
	unsigned int aTextureVBO
	)
{
#ifdef HAS_OPENGL
	// Reset the internal data
	reset();

	// Add the VBO to the resource manager if needed
	ResourceManager::getInstance().addVBO(this);

	m_primitive_type = GL_TRIANGLES;
	m_number_of_vertices = number_of_vertices;
	m_number_of_normals = number_of_vertices;
	m_number_of_texture_coordinates = number_of_vertices;


	m_number_of_compontents_per_normal = 3;
	m_number_of_compontents_per_vertex = 3;
	m_number_of_compontents_per_texture_coord = 2;

	m_number_of_additional_data = 0;
	m_number_of_compontents_per_additional_data = 0;

	m_normal_size = m_number_of_normals * 3;
	m_additional_data_size = m_number_of_additional_data * 3;
	m_texture_coordinate_size = m_number_of_texture_coordinates * 2;
	m_vertex_size = m_number_of_vertices * 3;

	m_normal_data_type = GL_FLOAT;
	m_additional_data_type = GL_FLOAT;
	m_texture_coordinate_data_type = GL_FLOAT;
	m_vertex_data_type = GL_FLOAT;

	m_normal_size *= sizeof(GLfloat);
	m_additional_data_size *= sizeof(GLfloat);
	m_texture_coordinate_size *= sizeof(GLfloat);
	m_vertex_size *= sizeof(GLfloat);


	// Load the data in the VBO
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// There is no VAO
	if (!m_vao_id)
	{
		// Create a VAO
		glGenVertexArrays(1, &m_vao_id);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

	// Bind the VAO
	glBindVertexArray(m_vao_id);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// There is normal data
	if (aNormalVBO)
	{
		// Create the VBO
		if (!m_id_set[NORMAL_SET_ID])
		{
			m_id_set[NORMAL_SET_ID] = aNormalVBO;
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_id_set[NORMAL_SET_ID]);

		glEnableVertexAttribArray(2);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}
	else
	{
		glDisableVertexAttribArray(2);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

	// There is additional data

	glDisableVertexAttribArray(3);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);


	// There are texture coordinates
	if (aTextureVBO)
	{
		// Create the VBO
		if (!m_id_set[TEXT_COORD_SET_ID])
		{
			m_id_set[TEXT_COORD_SET_ID] = aTextureVBO;
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_id_set[TEXT_COORD_SET_ID]);
		glEnableVertexAttribArray(1);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}
	else
	{
		glDisableVertexAttribArray(1);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

	// There are vertex coordinates
	if (aVertexVBO)
	{
		// Create the VBO
		if (!m_id_set[VERTEX_SET_ID])
		{
			m_id_set[VERTEX_SET_ID] = aVertexVBO;
			checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
		}


		glBindBuffer(GL_ARRAY_BUFFER, m_id_set[VERTEX_SET_ID]);

		glEnableVertexAttribArray(0);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}
	else
	{
		glDisableVertexAttribArray(0);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

	// Unbind the VAO
	glBindVertexArray(0);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Unbind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
#endif // HAS_OPENGL
}


//---------------------------------------------------------
void OpenGL3VBO::setIndexVBO(unsigned int aNumberOfIndexes,
							 int anIndexDataType,
							 unsigned int anIndexVBO)
//---------------------------------------------------------
{
#ifdef HAS_OPENGL
	m_number_of_indices = aNumberOfIndexes;
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

	// Bind the VAO
	glBindVertexArray(m_vao_id);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Create the VBO
	if (!m_id_set[INDEX_SET_ID] && m_number_of_indices)
	{
		m_id_set[INDEX_SET_ID] = anIndexVBO;
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id_set[INDEX_SET_ID]);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	m_index_size = aNumberOfIndexes;
	m_index_data_type = anIndexDataType;


	// Unbind the VAO
	glBindVertexArray(0);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
#endif // HAS_OPENGL
}


//----------------------------------------------------------------
void OpenGL3VBO::setIndexData(const unsigned int aNumberOfIndexes,
                              int anIndexDataType,
                              const void* apIndexSet)
//----------------------------------------------------------------
{
#ifdef HAS_OPENGL
	m_number_of_indices = aNumberOfIndexes;

	// Bind the VAO
	glBindVertexArray(m_vao_id);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

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

	// Unbind the VAO
	glBindVertexArray(0);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
#endif // HAS_OPENGL
}
