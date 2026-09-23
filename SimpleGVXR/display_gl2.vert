/*

Copyright (c) 2016, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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
 *  @file       display_gl2.vert
 *
 *  @brief      Vertex shader to display 3D objects with Phong shading.
 *
 *  @version    1.0
 *
 *  @date       31/12/2013
 *
 *  @author     Dr Franck P. Vidal
 *
 *  @section    License
 *              BSD 3-Clause License.
 *
 *              For details on use and redistribution please refer
 *              to http://opensource.org/licenses/BSD-3-Clause
 *
 *  @section    Copyright
 *              (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
 *              http://www.fpvidal.net/, Dec 2013, 2013, version 1.0,
 *              BSD 3-Clause License
 *
 ********************************************************************************
 */


//******************************************************************************
//  OpenGL 2.0
//******************************************************************************
#version 110


//******************************************************************************
//  Varying variables
//******************************************************************************
varying vec3 g_texture_coordinates;
varying vec3 g_normal;
varying vec4 g_object_position;


//******************************************************************************
//  Uniform variables
//******************************************************************************
uniform mat4 g_projection_matrix;
uniform mat4 g_modelview_matrix;
uniform mat3 g_normal_matrix;


//-------------
void main(void)
//-------------
{
    g_object_position = g_modelview_matrix * vec4(gl_Vertex.xyz, 1.0);
    g_normal = normalize(g_normal_matrix * gl_Normal);
    
    g_texture_coordinates = gl_MultiTexCoord0.xyz;

    //gl_FrontColor = gl_Color;
    gl_Position = g_projection_matrix * g_object_position;
}
