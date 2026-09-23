/**
 ********************************************************************************
 *
 *   @file      text_display_gl4.vert
 *
 *   @brief     Vertex shader to display text.
 *              The shader is written in GLSL version 450, and is designed for
 *              OpenGL 4.5.
 *
 *   @version   1.3
 *
 *   @date      25/02/2020
 *
 *   @author    Dr Franck P. Vidal
 *
 *   @section   License
 *              BSD 3-Clause License.
 *
 *              For details on use and redistribution please refer
 *              to http://opensource.org/licenses/BSD-3-Clause
 *
 *   @section   Copyright
 *              (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
 *              http://www.fpvidal.net/, Feb 2020, 2020, version 1.0,
 *              BSD 3-Clause License
 *
 ********************************************************************************
 */


//******************************************************************************
//  OpenGL 4.5
//******************************************************************************
#version 450 core
precision highp float; // Video card drivers require this line to function properly


//******************************************************************************
//  Input
//******************************************************************************
layout(location=0) in vec3 in_Vertex;
layout(location=1) in vec3 in_TexCoord;


//******************************************************************************
//  Output
//******************************************************************************
layout(location=0) smooth out vec2 g_texture_coordinates;


//******************************************************************************
//  Uniform variables
//******************************************************************************
layout(location=0) uniform mat4 g_projection_matrix;
layout(location=1) uniform mat4 g_modelview_matrix;


//-------------
void main(void)
//-------------
{
    g_texture_coordinates = in_TexCoord.xy;
    gl_Position = g_projection_matrix * g_modelview_matrix * vec4(in_Vertex, 1.0);
}
