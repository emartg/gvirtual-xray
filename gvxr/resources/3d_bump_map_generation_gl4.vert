/**
********************************************************************************
*
*   @file       bump_map_generation_gl4.vert
*
*   @brief      vertex shader to create a bump map.
*               The shader is written in GLSL version 450, and is designed for
*               OpenGL 4.5.
*
*   @version    1.3
*
*   @date       26/02/2020
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
// Input
//******************************************************************************
layout(location=0) in vec3 in_Vertex;
layout(location=1) in vec2 in_TexCoord;
layout(location=2) in vec3 in_Normal;


//******************************************************************************
// Output
//******************************************************************************
layout(location=0) smooth out vec3 g_object_position;


//-------------
void main(void)
//-------------
{
    g_object_position = in_Vertex;

    gl_Position = vec4(in_Vertex, 1.0) * 2 - 1;
}
