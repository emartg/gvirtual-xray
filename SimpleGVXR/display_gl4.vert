/*

Copyright (c) 2020, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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
*  @file       display_gl4.vert
*
*  @brief      Vertex shader to display 3D objects with Phong shading.
*              The shader is written in GLSL version 450, and is designed for
*              OpenGL 4.5.
*
*  @version    1.3
*
*  @date       29/02/2020
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
*              http://www.fpvidal.net/, Feb 2020, 2020, version 1.0,
*              BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
// OpenGL 4.5
//******************************************************************************
#version 450 core
precision highp float; // Video card drivers require this line to function properly


//******************************************************************************
//  Input
//******************************************************************************
layout(location=0) in vec3 in_Vertex;
layout(location=1) in vec3 in_TexCoord;
layout(location=2) in vec3 in_Normal;


//******************************************************************************
//  Output
//******************************************************************************
layout(location=0) smooth out vec3 g_texture_coordinates;
layout(location=1) smooth out vec3 g_normal;
layout(location=2) smooth out vec4 g_object_position;


//******************************************************************************
//  Uniform variables
//******************************************************************************
layout(location=0) uniform mat4 g_projection_matrix;
layout(location=1) uniform mat4 g_modelview_matrix;
layout(location=2) uniform mat3 g_normal_matrix;


//-------------
void main(void)
//-------------
{
    g_object_position = g_modelview_matrix * vec4(in_Vertex, 1.0);
    g_normal = normalize(g_normal_matrix * in_Normal);

    g_texture_coordinates = in_TexCoord;

    gl_Position = g_projection_matrix * g_object_position;
}
