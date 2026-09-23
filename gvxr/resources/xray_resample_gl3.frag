/**
********************************************************************************
*
*	@file		xray_resample_gl3.frag
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
//	OpenGL 3.2
//******************************************************************************
#version 150
precision highp float; // Video card drivers require this line to function properly


//******************************************************************************
//	Output
//******************************************************************************
smooth in vec3 g_texture_coordinates;


//******************************************************************************
//	Output
//******************************************************************************
out vec3 fragColor;


//******************************************************************************
//   Uniform variables
//******************************************************************************
uniform sampler2D g_texture;


//-------------
void main(void)
//-------------
{
    // Get the texel value
	  float intensity = texture(g_texture, g_texture_coordinates.st).r;


    // Save the fragment value
	  fragColor = vec3(intensity, intensity, intensity);
}
