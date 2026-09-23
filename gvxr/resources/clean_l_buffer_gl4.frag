/**
********************************************************************************
*
*   @file       clean_l_buffer_gl4.frag
*
*   @brief      Adaptive filtering to clean the L-buffer.
*               The shader is written in GLSL version 450, and is designed for
*               OpenGL 4.5.
*
*   @version    1.7
*
*   @date       04/07/2022
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
*               http://www.fpvidal.net/, Jul 2022, 2022, version 1.0,
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
layout(location=0) out vec3 fragColor;


//******************************************************************************
// Defines
//******************************************************************************
#define EPSILON         0.5


//******************************************************************************
// Uniform variables
//******************************************************************************
layout(location=2) uniform sampler2D g_l_buffer_texture;
layout(location=3) uniform float     g_x_offset;
layout(location=4) uniform float     g_y_offset;
layout(location=5) uniform float     g_cutoff;
layout(location=6) uniform int       g_max_radius_Lbuffer_artefact_filter;


//******************************************************************************
// Global variables
//******************************************************************************


//-----------------------------
bool isValid(vec3 lBufferValue)
//-----------------------------
{
    // Return true if lBufferValue is zero, false if it is smaller or greater than zero
    // Care is given to handle floating point numbers' inacuracy
    return ((abs(lBufferValue.g) < EPSILON) && (0.0 <= lBufferValue.r) && (lBufferValue.r <= g_cutoff));
}


//-------------
void main(void)
//-------------
{
    // Get the texel value
    vec3 texel_value = texture(g_l_buffer_texture, g_texture_coordinates.st).rgb;

    // Is the texel valid?
    bool is_texel_valid = isValid(texel_value);

    // If is_texel_valid == True; then intensity is equal to texel_value.r;
    // Else (is_texel_valid == False); then intensity is equal to 0.0.
    float intensity = texel_value.r * float(is_texel_valid);

    // If is_texel_valid == True; then accumulator is equal to 1;
    // Else (is_texel_valid == False); then accumulator is equal to 0.
    int accumulator = 1 * int(is_texel_valid);

    int temp_is_valid;
    float temp_intensity;
    int temp_accumulator;

    for (int radius = 1; radius <= g_max_radius_Lbuffer_artefact_filter; ++radius)
    {
        temp_intensity = 0.0;
        temp_accumulator = 0;

        for (int j = -radius; j <= radius; ++j)
        {
            for (int i = -radius; i <= radius; ++i)
            {
                vec2 offset = vec2( i * g_x_offset,  j * g_y_offset);

                texel_value = texture(g_l_buffer_texture,
                    g_texture_coordinates.st + offset).rgb;

                temp_is_valid = int(isValid(texel_value));
                temp_intensity += texel_value.r * temp_is_valid;
                temp_accumulator += 1 * temp_is_valid;
            }
        }

        intensity += float(accumulator == 0) * float(temp_accumulator > 0) * temp_intensity;
        accumulator += int(accumulator == 0) * int(temp_accumulator > 0) * temp_accumulator;
    }

    texel_value.r = intensity / max(1, accumulator);
    texel_value.g = 0.0;
    texel_value.b = 0.0;

    fragColor = texel_value;
}
