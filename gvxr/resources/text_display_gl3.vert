/**
 ********************************************************************************
 *
 *	@file		text_display_gl3.vert
 *
 *	@brief		Fragment shader to display text.
 *
 *	@version	1.0
 *
 *	@date		17/11/2014
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
 *				http://www.fpvidal.net/, Dec 2013, 2013, version 1.0,
 *				BSD 3-Clause License
 *
 ********************************************************************************
 */


//******************************************************************************
//	OpenGL 3.2
//******************************************************************************
#version 150
precision highp float; // Video card drivers require this line to function properly


//******************************************************************************
//	Input
//******************************************************************************
in vec3 in_Vertex;
in vec3 in_TexCoord;


//******************************************************************************
//	Output
//******************************************************************************
smooth out vec2 g_texture_coordinates;


//******************************************************************************
//	Uniform variables
//******************************************************************************
uniform mat4 g_projection_matrix;
uniform mat4 g_modelview_matrix;


//-------------
void main(void)
//-------------
{
    g_texture_coordinates = in_TexCoord.xy;
	gl_Position = g_projection_matrix * g_modelview_matrix * vec4(in_Vertex, 1.0);
}

