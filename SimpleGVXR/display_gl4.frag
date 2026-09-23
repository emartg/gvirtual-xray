/*

Copyright (c) 2020, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
http://www.fpvidal.net/
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

3. Neither the name of the Bangor University nor the names of its contributors
may be used to endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/


/**
********************************************************************************
*
*  @file       display_gl4.frag
*
*  @brief      Fragment shader to display 3D objects with Phong shading.
*              The shader is written in GLSL version 450, and is designed for
*              OpenGL 4.5.
*
*  @version    1.3
*
*  @date       29/02/2020
*
*  @author     Dr Franck P. Vidal
*
*  @section    License
*              BSD 3-Clause License.
*
*              For details on use and redistribution please refer
*              to http://opensource.org/licenses/BSD-3-Clause
*
*  @section    Copyright
*              (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*              http://www.fpvidal.net/, Feb 2020, 2020, version 1.0,
*              BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
// OpenGL 4.5
//******************************************************************************
#version 450 core
precision highp float; // Video card drivers require this line to function properly


//******************************************************************************
//  Input
//******************************************************************************
layout(location=0) smooth in vec3 g_texture_coordinates;
layout(location=1) smooth in vec3 g_normal;
layout(location=2) smooth in vec4 g_object_position;


//******************************************************************************
//  Output
//******************************************************************************
layout(location=0) out vec4 fragColor;


//******************************************************************************
//  Uniform variables
//******************************************************************************
layout(location=3) uniform int g_use_lighting;
layout(location=4) uniform vec4 light_global_ambient;
layout(location=5) uniform vec4 light_ambient;
layout(location=6) uniform vec4 light_diffuse;
layout(location=7) uniform vec4 light_specular;
layout(location=8) uniform vec4 light_position;
layout(location=9) uniform vec4 material_ambient;
layout(location=10) uniform vec4 material_diffuse;
layout(location=11) uniform vec4 material_specular;
layout(location=12) uniform float material_shininess;


//-------------
void main(void)
//-------------
{
    if (g_use_lighting  == 1)
        {
        vec3 view_direction  = vec3(g_object_position) / g_object_position.w;
        vec3 normal_vector   = normalize(g_normal);
        vec3 light_direction = normalize(vec3(light_position.xyz));

        vec3 half_vector     = normalize(view_direction + light_direction);

        vec4 global_ambient = light_global_ambient * material_ambient;
        vec4 ambient        = material_ambient  * light_ambient;
        vec4 diffuse        = material_diffuse  * light_diffuse  *
            max(0.0, dot(light_direction, normal_vector));
        vec4 specular       = material_specular * light_specular *
            pow(max(dot(normal_vector, half_vector), 0.0), material_shininess);

        fragColor = global_ambient + ambient + diffuse + specular;
        }
    else
        {
        fragColor = material_ambient;
        }
}
