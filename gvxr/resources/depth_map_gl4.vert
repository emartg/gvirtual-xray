/**
********************************************************************************
*
*	@file		depth_map_gl3.vert
*
*	@brief      vertex shader to create a depth map.
*               The shader is written in GLSL version 450, and is designed for
*               OpenGL 4.5.
*
*   @date       28/02/2020
*
*   @author     Andréas Meuleman
*   @author     Dr Franck P. Vidal
*
*   @section    License
*               BSD 3-Clause License.
*
*               For details on use and redistribution please refer
*               to http://opensource.org/licenses/BSD-3-Clause
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
precision highp float; // Video card drivers require this line to function properly


//******************************************************************************
//	Input
//******************************************************************************
layout(location=0) in vec4 position;


//******************************************************************************
//	Uniform variables
//******************************************************************************
layout(location=0) uniform mat4 matrix;


//-------------
void main(void)
//-------------
{
	//output: the position of the vertex for the map
	gl_Position =  matrix * position;
}
