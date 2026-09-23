/**
********************************************************************************
*
*	@file		xray_resample_gl2.frag
*
*	@brief		Fragment shader that resample the energy fluence.
*
*	@version	1.2
*
*	@date		11/03/2022
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
*				http://www.fpvidal.net/, Mar 2022, 2022, version 1.0,
*				BSD 3-Clause License
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
varying vec3 g_texture_coordinates;


//******************************************************************************
//   Uniform variables
//******************************************************************************
uniform sampler2D g_texture;


//-------------
void main(void)
//-------------
{
    // Get the texel value
	   float intensity = texture2D(g_texture, g_texture_coordinates.st).r;

    // Save the fragment value
	   gl_FragColor = vec4(intensity, intensity, intensity, 1.0);
}
