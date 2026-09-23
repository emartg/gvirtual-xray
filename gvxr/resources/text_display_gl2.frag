/**
 ********************************************************************************
 *
 *	@file		text_display_gl2.frag
 *
 *	@brief		Fragment shader to display text.
 *
 *	@version	1.0
 *
 *	@date		17/11/2014
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
 *				http://www.fpvidal.net/, Dec 2013, 2013, version 1.0,
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
varying vec2 g_texture_coordinates;


//******************************************************************************
//	Uniform variables
//******************************************************************************
uniform sampler2D imageTexture;
uniform vec4 backgroundColour;
uniform vec4 fontColour;


//-------------
void main(void)
//-------------
{
    vec4 colour;
    if (texture2D(imageTexture, g_texture_coordinates.st).r < 0.5)
    {
        gl_FragColor = backgroundColour;
    }
    else
    {
        gl_FragColor = fontColour;
    }
}
