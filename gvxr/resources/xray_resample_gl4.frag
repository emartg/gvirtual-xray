/**
********************************************************************************
*
*	@file		xray_resample_gl4.frag
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
//    OpenGL 4.5
//******************************************************************************
#version 450 core
precision highp float; // Video card drivers require this line to function properly


//******************************************************************************
//    Output
//******************************************************************************
layout(location=0) smooth in vec3 g_texture_coordinates;


//******************************************************************************
//    Output
//******************************************************************************
layout(location=0) out vec3 fragColor;


//******************************************************************************
//   Uniform variables
//******************************************************************************
layout(location=3) uniform sampler2D g_texture;


//-------------
void main(void)
//-------------
{
    // Get the texel value
    float intensity = texture(g_texture, g_texture_coordinates.st).r;

    // Save the fragment value
    fragColor = vec3(intensity, intensity, intensity);
}
