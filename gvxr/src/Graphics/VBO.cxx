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
*	@file		VBO.cxx
*
*	@brief		Class to handle vertex buffer objects (VBOs).
*
*	@version	1.0
*
*	@date		07/11/2013
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
#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <iostream>

#ifndef GVXR_OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef GVXR_CONTEXT_H
#include <gVirtualXRay/Context.h>
#endif

#ifndef GVXR_RESOURCE_MANAGER_H
#include <gVirtualXRay/ResourceManager.h>
#endif

#ifndef GVXR_VBO_h
#include "gVirtualXRay/VBO.h"
#endif


//******************************************************************************
//	Name space
//******************************************************************************
using namespace gVirtualXRay;


//----------------------------------------------
VBO::VBO():
//----------------------------------------------
        m_number_of_normals(0),
        m_number_of_compontents_per_normal(0),
        m_number_of_additional_data(0),
        m_number_of_compontents_per_additional_data(0),
        m_number_of_texture_coordinates(0),
        m_number_of_compontents_per_texture_coord(0),
        m_number_of_vertices(0),
        m_number_of_compontents_per_vertex(0),
        m_number_of_indices(0),
        m_normal_size(0),
        m_additional_data_size(0),
        m_texture_coordinate_size(0),
        m_vertex_size(0),
        m_index_size(0),
        m_normal_data_type(GL_NONE),
        m_additional_data_type(GL_NONE),
        m_texture_coordinate_data_type(GL_NONE),
        m_vertex_data_type(GL_NONE),
        m_index_data_type(GL_NONE),
        m_primitive_type(GL_NONE),
        m_storage_type(GL_DYNAMIC_DRAW),
        m_opengl_major_version(0)
//----------------------------------------------
{
    m_id_set[VERTEX_SET_ID]     = 0;
    m_id_set[TEXT_COORD_SET_ID] = 0;
    m_id_set[NORMAL_SET_ID]     = 0;
    m_id_set[ADDITIONAL_DATA_ID]= 0;
    m_id_set[INDEX_SET_ID]      = 0;
}


//-----------------
void VBO::destroy()
//-----------------
{
    Context* p_context = ResourceManager::getInstance().getGraphicsContext();
    if (p_context)
    {
        if (p_context->isReady())
        {
#ifdef HAS_OPENGL
            if (m_id_set[VERTEX_SET_ID])      glDeleteBuffers(1, &m_id_set[VERTEX_SET_ID]);
            if (m_id_set[TEXT_COORD_SET_ID])  glDeleteBuffers(1, &m_id_set[TEXT_COORD_SET_ID]);
            if (m_id_set[NORMAL_SET_ID])      glDeleteBuffers(1, &m_id_set[NORMAL_SET_ID]);
            if (m_id_set[ADDITIONAL_DATA_ID]) glDeleteBuffers(1, &m_id_set[ADDITIONAL_DATA_ID]);
            if (m_id_set[INDEX_SET_ID])       glDeleteBuffers(1, &m_id_set[INDEX_SET_ID]);
#endif
        }
    }

    m_id_set[VERTEX_SET_ID]     = 0;
    m_id_set[TEXT_COORD_SET_ID] = 0;
    m_id_set[NORMAL_SET_ID]     = 0;
    m_id_set[ADDITIONAL_DATA_ID]= 0;
    m_id_set[INDEX_SET_ID]      = 0;

    reset();
}


//---------------
void VBO::reset()
//---------------
{
    m_number_of_normals                       = 0;
    m_number_of_compontents_per_normal        = 0;

    m_number_of_additional_data               = 0;
    m_number_of_compontents_per_additional_data= 0;

    m_number_of_texture_coordinates           = 0;
    m_number_of_compontents_per_texture_coord = 0;

    m_number_of_vertices                      = 0;
    m_number_of_compontents_per_vertex        = 0;

    m_number_of_indices                       = 0;

    m_normal_size             = 0;
    m_additional_data_size    = 0;
    m_texture_coordinate_size = 0;
    m_vertex_size             = 0;
    m_index_size              = 0;

    m_normal_data_type             = GL_NONE;
    m_additional_data_type         = GL_NONE;
    m_texture_coordinate_data_type = GL_NONE;
    m_vertex_data_type             = GL_NONE;
    m_index_data_type              = GL_NONE;

    m_primitive_type = GL_NONE;

    m_storage_type = GL_DYNAMIC_DRAW;

    m_opengl_major_version = 0;
}
