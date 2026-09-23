/**
********************************************************************************
*
*	@file		bump_map_generation_gl3.vert
*
*	@brief      vertex shader to create a bump map.
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


//******************************************************************************
//	Uniform variables
//******************************************************************************
uniform int g_has_tendon;
uniform mat4 g_matrix;


//-------------
void main(void)
//-------------
{	
	if(g_has_tendon == 1)
	{
		g_normal = mat3(g_matrix[0].xyz, g_matrix[1].xyz, g_matrix[2].xyz) * gl_Normal;
		g_object_position = (g_matrix * vec4(gl_Vertex.xyz, 1.)).xyz;
	}
	else
	{
		g_normal = gl_Normal;
		g_object_position = gl_Vertex.xyz;
	}
		
	gl_Position = vec4(gl_MultiTexCoord0.xy * 2. - 1., 0., 1.);
}
