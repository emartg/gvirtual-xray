/**
 *******************************************************************************
 *
 *	@file		cumulated_l_buffer_gl3.frag
 *
 *	@brief		Fragment shader that computes the cumulated L-buffer.
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
//	OpenGL 3.2
//******************************************************************************
#version 150
precision highp float; // Video card drivers require this line to function properly


//******************************************************************************
//	Input variables
//******************************************************************************
smooth in vec3 g_texture_coordinates;


//******************************************************************************
//	Output
//******************************************************************************
out vec3 fragColor;


//******************************************************************************
//   Uniform variables
//******************************************************************************
uniform sampler2D g_l_buffer_texture;


//-------------
void main(void)
//-------------
{
    // Get the L-buffer value
    float intensity = texture(g_l_buffer_texture, g_texture_coordinates.st).r;
    
	fragColor = vec3(intensity, 0.0, 0.0);
}

