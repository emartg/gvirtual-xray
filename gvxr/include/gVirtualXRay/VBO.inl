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
*	@file		VBO.inl
*
*	@brief		Class to handle vertex buffer objects (VBOs).
*
*	@version    1.0
*
*	@date		07/11/2013
*
*	@author		Dr Franck P. Vidal
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net), 
*   http://www.fpvidal.net/, Dec 2014, 2014, version 1.0, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
#include <cstring>

#ifndef GVXR_OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif


//******************************************************************************
//	define
//******************************************************************************
#define VBO_BUFFER_OFFSET(a) ((char*)NULL + (a))
#define VERTEX_SET_ID      0
#define TEXT_COORD_SET_ID  1
#define NORMAL_SET_ID      2
#define ADDITIONAL_DATA_ID 3
#define INDEX_SET_ID       4


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//----------------
inline VBO::~VBO()
//----------------
{
	VBO::destroy();
}


//------------------------------
inline bool VBO::isReady() const
//-----------------------------
{
	return ((m_number_of_vertices && m_id_set[0]) ? true : false);
}


//--------------------------------------------
inline void VBO::setBufferUsageHint(int aFlag)
//--------------------------------------------
{
	m_storage_type = aFlag;
}


//----------------------------------------
inline int VBO::getBufferUsageHint() const
//----------------------------------------
{
	return (m_storage_type);
}


//--------------------------------------
inline int VBO::getPrimitiveType() const
//--------------------------------------
{
	return (m_primitive_type);
}


//-----------------------------------------
inline VBO& VBO::operator=(const VBO& aVBO)
//-----------------------------------------
{
    // Prevent self assignment
    if (this != &aVBO)
	{
		memcpy(m_id_set, aVBO.m_id_set, sizeof(m_id_set));

		m_number_of_normals             = aVBO.m_number_of_normals;
		m_number_of_compontents_per_normal = aVBO.m_number_of_compontents_per_normal;
		m_number_of_additional_data     = aVBO.m_number_of_additional_data;
		m_number_of_compontents_per_additional_data     = aVBO.m_number_of_compontents_per_additional_data;
		m_number_of_texture_coordinates = aVBO.m_number_of_texture_coordinates;
		m_number_of_compontents_per_texture_coord = aVBO.m_number_of_compontents_per_texture_coord;
		m_number_of_vertices            = aVBO.m_number_of_vertices;
		m_number_of_compontents_per_vertex            = aVBO.m_number_of_compontents_per_vertex;
		m_number_of_indices             = aVBO.m_number_of_indices;

		m_normal_size             = aVBO.m_normal_size;
		m_additional_data_size    = aVBO.m_additional_data_size;
		m_texture_coordinate_size = aVBO.m_texture_coordinate_size;
		m_vertex_size             = aVBO.m_vertex_size;
		m_index_size              = aVBO.m_index_size;

		m_normal_data_type             = aVBO.m_normal_data_type;
		m_additional_data_type         = aVBO.m_additional_data_type;
		m_texture_coordinate_data_type = aVBO.m_texture_coordinate_data_type;
		m_vertex_data_type             = aVBO.m_vertex_data_type;
		m_index_data_type              = aVBO.m_index_data_type;

		m_primitive_type = aVBO.m_primitive_type;
		
		m_storage_type = aVBO.m_storage_type;

		m_opengl_major_version = aVBO.m_opengl_major_version;
	}
	
	return (*this);
}


//----------------------------------------------------
inline unsigned int VBO::getOpenglMajorVersion() const
//----------------------------------------------------
{
    return m_opengl_major_version;
}


} // namespace gVirtualXRay
