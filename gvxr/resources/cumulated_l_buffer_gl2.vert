/**
 *******************************************************************************
 *
 *	@file		cumulated_l_buffer_gl2.vert
 *
 *	@brief		Vertex shader that computes the cumulated L-buffer.
 *				see:
 *          	F. P. Vidal, M. Garnier, N. Freud, J. M. Letang, and
 *          	N. W. John. Simulation of X-ray attenuation on the GPU. In
 *          	Proceedings of Theory and Practice of Computer Graphics 2009,
 *          	pages 25-32, Cardiff, UK, June 2009. Eurographics Association.
 *          	Winner of Ken Brodlie Prize for Best Paper.
 *
 *	@version	1.2
 *
 *	@date		27/12/2013
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
 *******************************************************************************
 */


//******************************************************************************
//	OpenGL 2.0
//******************************************************************************
#version 110


//******************************************************************************
//	Varying variables
//******************************************************************************
varying vec3 g_texture_coordinates;


//******************************************************************************
//	Uniform variables
//******************************************************************************
uniform mat4 g_projection_matrix;
uniform mat4 g_modelview_matrix;


//-------------
void main(void)
//-------------
{
    // Save the texture coordinates
	g_texture_coordinates = gl_MultiTexCoord0.xyz;
    
    // Apply the modelling and viewing transformations
	gl_Position = g_projection_matrix * g_modelview_matrix * vec4(gl_Vertex.xyz, 1.0);
}

