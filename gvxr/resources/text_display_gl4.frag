/**
 ********************************************************************************
 *
 *   @file      text_display_gl4.frag
 *
 *   @brief     Fragment shader to display text.
 *              The shader is written in GLSL version 450, and is designed for
 *              OpenGL 4.5.
 *
 *   @version   1.3
 *
 *   @date      25/02/2020
 *
 *   @author    Dr Franck P. Vidal
 *
 *   @section   License
 *              BSD 3-Clause License.
 *
 *              For details on use and redistribution please refer
 *              to http://opensource.org/licenses/BSD-3-Clause
 *
 *   @section   Copyright
 *              (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
 *              http://www.fpvidal.net/, Feb 2020, 2020, version 1.0,
 *              BSD 3-Clause License
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
layout(location=0) smooth in vec2 g_texture_coordinates;


//******************************************************************************
//  Output
//******************************************************************************
layout(location=0) out vec4 fragColor;


//******************************************************************************
//  Uniform variables
//******************************************************************************
layout(location=2) uniform sampler2D imageTexture;
layout(location=3) uniform vec4 backgroundColour;
layout(location=4) uniform vec4 fontColour;


//-------------
void main(void)
//-------------
{
    vec4 colour;
    if (texture(imageTexture, g_texture_coordinates.st).r < 0.5)
    {
        fragColor = backgroundColour;
    }
    else
    {
        fragColor = fontColour;
    }
}
