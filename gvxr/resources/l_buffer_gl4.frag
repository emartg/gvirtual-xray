/**
********************************************************************************
*
*   @file        l_buffer_gl4.frag
*
*   @brief       Fragment shader that computes a L-buffer.
*                see: N. Freud, P. Duvauchelle, J. M. Letang, D. Babot,
*                Fast and robust ray casting algorithms for virtual X-ray
*                imaging, Nuclear Instruments and Methods in Physics Research
*                Section B: Beam Interactions with Materials and Atoms 248 (1)
*                (2006) 175-180.
*
*                F. P. Vidal, M. Garnier, N. Freud, J. M. Letang, and
*                N. W. John. Simulation of X-ray attenuation on the GPU. In
*                Proceedings of Theory and Practice of Computer Graphics 2009,
*                pages 25-32, Cardiff, UK, June 2009. Eurographics Association.
*                Winner of Ken Brodlie Prize for Best Paper.
*                The shader is written in GLSL version 450, and is designed for
*                OpenGL 4.5.
*
*   @version     1.3
*
*   @date        21/02/2020
*
*   @author      Dr Franck P. Vidal
*
*   @section     License
*                BSD 3-Clause License.
*
*                For details on use and redistribution please refer
*                to http://opensource.org/licenses/BSD-3-Clause
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
layout (location = 0) in TVertexData
{
    vec3 g_in_vertex;
    vec3 g_normal;
} g_input_data;


//******************************************************************************
//  Output
//******************************************************************************
layout (location = 0) out vec4 fragColor;


//******************************************************************************
//  Constants
//******************************************************************************
const bool PERSPECTIVE_PROJECTION = false;
const bool PARALLEL_PROJECTION    = true;
const float EPSILON = 0.00001;


//******************************************************************************
//  Uniform variables
//******************************************************************************
layout (location = 0) uniform vec4 g_detector_plane;
layout(location=1) uniform vec3 g_detector_center;
layout(location=10) uniform float g_to_cm_scale;
layout(location=6) uniform mat4 g_modelling_matrix;
layout(location=11) uniform mat3 g_normal_matrix;
layout(location=7) uniform bool g_projection_type; // 1 for parallel beam
layout(location=8) uniform vec3 g_ray_direction;
layout(location=9) uniform vec3 g_source_position;


//--------------
void main (void)
//--------------
{
	// Transform normal vectors
	vec3 normal = -normalize(g_normal_matrix * g_input_data.g_normal);

    // Transform the vertex
	vec4 transformed_vertex = g_modelling_matrix * vec4(g_input_data.g_in_vertex, 1.0);
	transformed_vertex.xyz /= transformed_vertex.w;
	transformed_vertex.w = 1.0;

    // Get the viewing direction
    vec3 viewing_vector;

	// Use parallel beam
	if (g_projection_type == PARALLEL_PROJECTION)
	{
		viewing_vector = -g_ray_direction;
	}
    // Use perspective projection
	else
	{
		viewing_vector = -normalize(transformed_vertex.xyz - g_source_position);
	}

	// Save the dot product
	//float dot_product = dot(normal, viewing_vector);
	//float dot_product_sign = sign(dot_product);

	float dot_product_sign = -sign(float(gl_FrontFacing) - 0.5);


    // The dot product value is too small
	/*if (abs(dot_product) < EPSILON)
	{
        // Alter the normal
		normal.x += 0.1;
		normal.y += 0.1;
		normal = normalize(normal);

        // Update the dot product
        dot_product = dot(normal, viewing_vector);
		dot_product_sign = sign(dot_product);
	}*/

    // Get the distance between the source and the vertex
    //float distance_source_vertex = distance(transformed_vertex.xyz, g_source_position);
	//float value = dot_product_sign * distance_source_vertex * g_to_cm_scale;

    // Get the distance between the detector pixel and the vertex
	float distance_intersection_detector = (dot(transformed_vertex.xyz, g_detector_plane.xyz) + g_detector_plane.w) / dot(-viewing_vector, g_detector_plane.xyz);
	float value = dot_product_sign * distance_intersection_detector * g_to_cm_scale;

    // Make sure the vertex is not on a detector pixel
	//if (abs(distance_source_vertex) < EPSILON) value = 0.0;
	//if (abs(distance_intersection_detector) < EPSILON) value = 0.0;
	value *= float(abs(distance_intersection_detector) >= EPSILON);

    // Apply clipping
	//float clipping = sign(dot(normalize(transformed_vertex.xyz - g_source_position.xyz),
	//    normalize(g_detector_plane.xyz)));
	//value *= float(clipping > 0.0);

	// In the red channel: the L-buffer value
	// In the green channel the sign of the dot product (useful to detect errors in the final L-buffer)
	// In the blue channel: 0.0 (unused)
	// In the alpha channel: 1.0 (unused)
	//fragColor = vec4(value, dot_product_sign, distance_source_vertex * g_to_cm_scale, 1.0);
	fragColor = vec4(value, dot_product_sign, distance_intersection_detector * g_to_cm_scale, 1.0);
}
