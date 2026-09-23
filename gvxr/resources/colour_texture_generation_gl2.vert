/**
********************************************************************************
*
*	@file		colour_texture_generation_gl3.vert
*
*	@brief      vertex shader to create a texture.
*
*	@version	1.0
*
*	@date		01/08/2016
*
*	@author		Andréas Meuleman
*
********************************************************************************
*/



//******************************************************************************
//	OpenGL 2.0
//******************************************************************************
#version 110


//******************************************************************************
//	Varying variables
//******************************************************************************
varying vec3 g_normal;
varying vec3 g_object_position;
varying vec2 g_texture_coordinates;


//******************************************************************************
//	Uniform variables
//******************************************************************************
uniform int g_use_3D_colour_texture;


//-------------
void main(void)
//-------------
{
	g_object_position = gl_Vertex.xyz;
	g_normal = gl_Normal.xyz;
	g_texture_coordinates = gl_MultiTexCoord0.xy;
	
	if(g_use_3D_colour_texture == 1)
	{
		gl_Position = vec4(gl_Vertex.xyz, 1.0) * 2.0 - 1.0;

	}
	else
	{
		gl_Position = vec4(gl_MultiTexCoord0.xy * 2.0 - 1.0, 0.0, 1.0);
	}
}

