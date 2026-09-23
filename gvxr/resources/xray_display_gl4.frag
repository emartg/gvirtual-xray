/**
********************************************************************************
*
*   @file       xray_display_gl4.frag
*
*   @brief      Fragment shader that displays the energy fluence. The image can
*               be in negative and the pixels can be filtered as follows:
*               f(x) = g_scale_filter * (x + g_shift_filter)
*               see:
*               F. P. Vidal, M. Garnier, N. Freud, J. M. Letang, and
*               N. W. John. Simulation of X-ray attenuation on the GPU. In
*               Proceedings of Theory and Practice of Computer Graphics 2009,
*               pages 25-32, Cardiff, UK, June 2009. Eurographics Association.
*               Winner of Ken Brodlie Prize for Best Paper.
*               The shader is written in GLSL version 450, and is designed for
*               OpenGL 4.5.
*
*   @version    1.3
*
*   @date       25/02/2020
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
// OpenGL 4.5
//******************************************************************************
#version 450 core
precision highp float; // Video card drivers require this line to function properly


//******************************************************************************
// Input
//******************************************************************************
layout(location=0) smooth in vec3 g_texture_coordinates;


//******************************************************************************
// Output
//******************************************************************************
layout(location=0) out vec4 fragColor;


//******************************************************************************
//   Uniform variables
//******************************************************************************
layout(location=2) uniform sampler2D g_attenuation;
layout(location=3) uniform bool      g_display_negative_image;
layout(location=4) uniform bool      g_use_log_scale;
layout(location=5) uniform bool      g_use_power_law;

layout(location=6) uniform float     g_shift_filter;
layout(location=7) uniform float     g_scale_filter;
layout(location=8) uniform float     log_epsilon;
layout(location=9) uniform float     g_gamma;
layout(location=10) uniform float     g_incident_energy;


//-------------
void main(void)
//-------------
{
    // Get the texel value
    float intensity = texture(g_attenuation, g_texture_coordinates.st).r;

    // See https://imagej.nih.gov/nih-image/more-docs/Engineering/ImgEngr.html
    //intensity = pow(10.0, -log10(g_incident_energy / intensity));

    // Use the log scale
    if (g_use_log_scale)
    {
        intensity = log(intensity + log_epsilon) / log(10.0);
    }

    // Use the power law
    if (g_use_power_law)
    {
        intensity = pow(intensity, g_gamma);
    }

    // Apply the shift/scale filter
    intensity = ((intensity) + g_shift_filter) * g_scale_filter;

    // Display the negative image
    if (g_display_negative_image)
    {
        intensity = 1.0 - intensity;
    }

    // Save the fragment value
    fragColor = vec4(intensity, intensity, intensity, 1.0);
}
