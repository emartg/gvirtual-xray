/**
********************************************************************************
*
*   @file       xray_display_gl4.vert
*
*   @brief      Vertex shader that display the energy fluence. The image can
*   			be in negative and the pixels can be filtered as follows:
*        		f(x) = ScaleFilter * (x + ShiftFilter)
*   			see:
*          		F. P. Vidal, M. Garnier, N. Freud, J. M. Letang, and
*          		N. W. John. Simulation of X-ray attenuation on the GPU. In
*          		Proceedings of Theory and Practice of Computer Graphics 2009,
*          		pages 25–32, Cardiff, UK, June 2009. Eurographics Association.
*          		Winner of Ken Brodlie Prize for Best Paper.
*               The shader is written in GLSL version 450, and is designed for
*               OpenGL 4.5.
*
*   @version	1.3
*
*   @date		25/02/2020
*
*   @author		Dr Franck P. Vidal
*
*   @section	License
*   			BSD 3-Clause License.
*
*   			For details on use and redistribution please refer
*   			to http://opensource.org/licenses/BSD-3-Clause
*
*   @section    Copyright
*               (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*               http://www.fpvidal.net/, Feb 2020, 2020, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
// OpenGL 4.5
//******************************************************************************
#version 450 core


//******************************************************************************
// Input
//******************************************************************************
layout(location=0) in vec3 in_Vertex;
layout(location=1) in vec3 in_TexCoord;


//******************************************************************************
// Output
//******************************************************************************
layout(location=0) smooth out vec3 g_texture_coordinates;


//******************************************************************************
// Uniform variables
//******************************************************************************
layout(location=0) uniform mat4 g_projection_matrix;
layout(location=1) uniform mat4 g_modelview_matrix;


//-------------
void main(void)
//-------------
{
    // Save the texture coordinates
	g_texture_coordinates = in_TexCoord;

    // Apply the modelling and viewing transformations
	gl_Position = g_projection_matrix * g_modelview_matrix * vec4(in_Vertex, 1.0);
}
