/*

Copyright (c) 2014, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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
 *	@file		display_gl3.frag
 *
 *	@brief		Fragment shader to display 3D objects with Phong shading.
 *
 *	@version	1.0
 *
 *	@date		31/12/2013
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
//	OpenGL 3.2
//******************************************************************************
#version 150
precision highp float; // Video card drivers require this line to function properly

//******************************************************************************
//	constant variables
//******************************************************************************
//step for bump evaluation 
const float STEP=0.02;

//******************************************************************************
//	Input
//******************************************************************************
smooth in vec2 g_2D_texture_coordinates;
smooth in vec3 g_3D_texture_coordinates;
smooth in vec3 g_normal;
smooth in vec3 g_view_direction;
smooth in vec3 g_light_direction;
smooth in vec4 g_shadow_coord;

//******************************************************************************
//	Output
//******************************************************************************
out vec4 frag_colour;


//******************************************************************************
//	Uniform variables
//******************************************************************************
uniform int g_use_lighting;
uniform int g_use_bump_map;
uniform int g_use_texture;
uniform int g_use_3D_bump_map;
uniform int g_use_3D_colour_texture;
uniform vec3 light_direction;
uniform vec4 light_global_ambient;
uniform vec4 light_ambient;
uniform vec4 light_diffuse;
uniform vec4 light_specular;
uniform vec4 material_ambient;
uniform vec4 material_diffuse;
uniform vec4 material_specular;
uniform float material_shininess;
uniform float g_repeat_frequency;
uniform sampler2DShadow g_shadow_map;
uniform sampler2D g_bump_map;
uniform sampler2D g_colour_texture;
uniform sampler3D g_3D_bump_map;
uniform sampler3D g_3D_colour_texture;
uniform mat3 g_normal_matrix;

//change the texture coordinates to repeate a 3D texture
//-----------------------------------
float repeatCoordinate(float x)
//-----------------------------------
{
	float res = x - floor(x);
	if ((int(floor(x)) % 2) == 1)
	{
		res = 1.0 - res;
	}
	return res;
}

//calculate the bump value in the 3D bump map
//-----------------------------------
float computeBumpValue(vec3 texture_coord)
//-----------------------------------
{
	texture_coord = vec3(repeatCoordinate(texture_coord.x), repeatCoordinate(texture_coord.y), repeatCoordinate(texture_coord.z));
	return texture(g_3D_bump_map, texture_coord).r;
}

//calculate the texture colour
//-----------------------------------
vec4 computeTextureColour()
//-----------------------------------
{
	vec4 texture_colour;
	if(g_use_3D_colour_texture == 1)
   	{
		vec3 texture_coord = g_3D_texture_coordinates * g_repeat_frequency;
		
		texture_coord = vec3(repeatCoordinate(texture_coord.x), 
			repeatCoordinate(texture_coord.y), repeatCoordinate(texture_coord.z));
  		
  		texture_colour = texture(g_3D_colour_texture, texture_coord);
   	}
   	else
   	{
   		texture_colour = texture( g_colour_texture, g_2D_texture_coordinates);
   	}

	return texture_colour;
}

//-------------
void main(void)
//-------------
{
    if (g_use_lighting  == 1)
    {
   	 	vec3 normal_vector = g_normal;
    	
    	if(g_use_bump_map == 1)
	    {
	    	//modify the normal vector using the bump map   	
	    	vec3 deltaNormal;
	    	
	     	if (g_use_3D_bump_map == 1)
    		{
    			vec3 U = cross(g_normal,vec3(1,0,0));
				vec3 V = cross(g_normal,vec3(0,1,0));
				vec3 W = cross(g_normal,vec3(0,0,1));
	  				  				
	  			vec3 texture_coord = g_3D_texture_coordinates * g_repeat_frequency;
				deltaNormal = U*(computeBumpValue(texture_coord-STEP*U)
	   				- computeBumpValue(texture_coord+STEP*U))/(STEP);
	    	
				deltaNormal += V*(computeBumpValue(texture_coord-STEP*V)
	   				- computeBumpValue(texture_coord+STEP*V))/(STEP);
	
				deltaNormal += W*(computeBumpValue(texture_coord-STEP*W)
	  				- computeBumpValue(texture_coord+STEP*W))/(STEP);
       		}
    		else
    		{
    	    	deltaNormal = texture( g_bump_map, g_2D_texture_coordinates).rgb*2.-1.;
     		}
	    	
    		normal_vector += deltaNormal;
    	}
  		normal_vector = normalize(g_normal_matrix*normal_vector);
    
    	//cosinus of the light direction and the normal vector
    	float cosLightNormal = max(0., dot(light_direction, normal_vector));
    	  
		//change the shadow coordinates. They have to be in [0,1] for texture samlping
		vec3 shadow_changed_coord = g_shadow_coord.xyz/g_shadow_coord.w;
		shadow_changed_coord = shadow_changed_coord*0.5 + 0.5;
		    
    	//visibility equals 0.3 if the fragment if in the shadow and 1 if not (if it is on the back of the face, it is in the shadow).
       	float visibility = texture(g_shadow_map, vec3(shadow_changed_coord))*clamp(cosLightNormal-0.1, 0., 1.);
		visibility = visibility*0.7+0.4;
	      
	  	vec3 global_ambient;
	   	vec3 ambient;
	    vec3 diffuse;
	    
	    if(g_use_texture==1)
	    {
	    	vec4 texture_colour = computeTextureColour();
	    	
	    	global_ambient =vec3(light_global_ambient*texture_colour);
	   		ambient        = vec3(light_ambient * texture_colour);
	    	diffuse        =  vec3(texture_colour*light_diffuse) * cosLightNormal;
	    }
	    else
	    {
	    	global_ambient = vec3(light_global_ambient * material_ambient);
	   		ambient        = vec3(material_ambient  * light_ambient);
	    	diffuse        = vec3(material_diffuse  * light_diffuse) * cosLightNormal;
	    }

	    vec3 specular = vec3(material_specular * light_specular * pow(max(dot(g_view_direction, reflect(light_direction, normal_vector)), 0.), material_shininess));
	
    	frag_colour = visibility*vec4(global_ambient+ambient + diffuse + specular, 1.);
    }
    else
    {
	    if(g_use_texture==1)
	    {
	    	frag_colour = computeTextureColour();
	    }
	    else
	    {
	    	frag_colour = material_ambient;
	    }
    }
}
