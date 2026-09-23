/**
********************************************************************************
*
*   @file       MUx_Dx_gl4.frag
*
*   @brief      Fragment shader that computes Mu_x * Dx.
*               see:
*               F. P. Vidal, M. Garnier, N. Freud, J. M. Letang, and
*               N. W. John. Simulation of X-ray attenuation on the GPU. In
*               Proceedings of Theory and Practice of Computer Graphics 2009,
*               pages 25–32, Cardiff, UK, June 2009. Eurographics Association.
*               Winner of Ken Brodlie Prize for Best Paper.
*               The shader is written in GLSL version 450, and is designed for
*               OpenGL 4.5.
*
*   @version    1.3
*
*   @date       21/02/2020
*
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
//  OpenGL 4.5
//******************************************************************************
#version 450 core
precision highp float; // Video card drivers require this line to function properly


//******************************************************************************
//  Input
//******************************************************************************
layout(location=0) smooth in vec3 g_texture_coordinates;


//******************************************************************************
//  Output
//******************************************************************************
layout(location=0) out vec3 fragColor;


//******************************************************************************
//   Uniform variables
//******************************************************************************
layout(location=2) uniform sampler2D g_l_buffer_texture;
layout(location=3) uniform float     g_attenuation_coefficient;


//-------------
void main(void)
//-------------
{
    // Get the L-buffer value
    float intensity = texture(g_l_buffer_texture, g_texture_coordinates.st).r;

    // Compute L-buffer * g_attenuation_coefficient
    fragColor = vec3(max(0.0, intensity) * g_attenuation_coefficient, 0.0, 0.0);
}
