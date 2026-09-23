/**
********************************************************************************
*
*	@file		InternalOrganMesh.cxx
*
*	@brief      Class to handle a polygon mesh for an internal organ.
*
*	@version	1.0
*
*	@date		18/07/2016
*
*	@author		Andréas Meuleman
*
********************************************************************************
*/

//******************************************************************************
//	Include
//******************************************************************************
#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#define _USE_MATH_DEFINES
#include <cmath>

#include <cstdlib>
#include <map>
#include <algorithm>

#ifndef GVXR_UTILITIES_H
#include "gVirtualXRay/Utilities.h"
#endif

#ifndef GVXR_NOT_IMPLEMENTED_EXCEPTION_H
#include "gVirtualXRay/NotImplementedException.h"
#endif

#include "gVirtualXRay/InternalOrganMesh.h"

#include "bump_map_generation_gl2.vert.h"
#include "bump_map_generation_gl2.frag.h"

#include "bump_map_generation_gl3.vert.h"
#include "bump_map_generation_gl3.frag.h"

#include "bump_map_generation_gl4.vert.h"
#include "bump_map_generation_gl4.frag.h"

#include "3d_bump_map_generation_gl2.vert.h"
#include "3d_bump_map_generation_gl2.frag.h"

#include "3d_bump_map_generation_gl3.vert.h"
#include "3d_bump_map_generation_gl3.frag.h"

#include "3d_bump_map_generation_gl4.vert.h"
#include "3d_bump_map_generation_gl4.frag.h"


//******************************************************************************
//	constant variables
//******************************************************************************
//size of the arrays to initialize perlin's noise
const int TAB_SIZE = 64;

//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {

//----------------------------------------
InternalOrganMesh::InternalOrganMesh(bool optimize_texture_coordinates,
		unsigned int texture_2D_size, unsigned int texture_3D_size):
//----------------------------------------
		AnatomicalMesh(),
		m_bump_map_texture(0),
		m_bump_map_frame_buffer(0),
		m_has_tendon(0),
		m_is_lung(0),
		m_has_thin_bumps(0),
		m_use_3D_bump_map(0),
		m_texture_size_2d(texture_2D_size),
		m_texture_size_3d(texture_3D_size),
		m_need_initialize(true),
		m_generation_texture_coord_are_set(false),
		m_optimize_texture_coordinates(optimize_texture_coordinates)
//----------------------------------------
{
	//internal organs use bump mapping
	m_use_bump_map = 1;
	m_label = "unknown_internal_organ";
}

//----------------------------------------
InternalOrganMesh::~InternalOrganMesh()
//----------------------------------------
{
	// TODO Auto-generated destructor stub
}

//----------------------------------------
bool InternalOrganMesh::isTransparent() const
//----------------------------------------
{
	return false;
}

//----------------------------------------------
void InternalOrganMesh::defineDisplayShaderInput(int aShaderID)
//----------------------------------------------
{
	//initialize if needed
	if(m_need_initialize)
	{
		initialize();
	}

	GLuint handle(glGetUniformLocation(aShaderID, "g_use_3D_bump_map"));
	glUniform1iv(handle, 1, &m_use_3D_bump_map);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	if(m_use_3D_bump_map)
	{
		//Bind the texture for bump mapping in texture unit 3
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_3D, m_bump_map_texture);

		handle = glGetUniformLocation(aShaderID, "g_3D_bump_map");
		glUniform1i(handle,3);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}
	else
	{
		//Bind the texture for bump mapping in texture unit 1
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_bump_map_texture);

		handle = glGetUniformLocation(aShaderID, "g_bump_map");
		glUniform1i(handle,1);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

	AnatomicalMesh::defineDisplayShaderInput(aShaderID);
}

//----------------------------------------------
void InternalOrganMesh::splitFaces()
//----------------------------------------------
{
	throw NotImplementedException(__FILE__, __FUNCTION__, __LINE__);
}

//----------------------------------------------
void InternalOrganMesh::generate2DTexture(Shader const& shader,
		unsigned int& frame_buffer, ToCreate to_create)
//----------------------------------------------
{
	//render to the bump buffer
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
	glViewport(0, 0, m_texture_size_2d, m_texture_size_2d);

	// Enable back face culling
	pushEnableDisableState(GL_CULL_FACE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Enable the bump shader
	pushShaderProgram();
	shader.enable();

	GLint shader_id(shader.getProgramHandle());

	switch(to_create)
	{
	case TO_CREATE_BUMP_MAP:
		defineBumpMapShaderInput(shader_id);
		break;

	case TO_CREATE_COLOUR_TEXTURE:
		defineColourTextureShaderInput(shader_id);
		break;

	default:
		defineBumpMapShaderInput(shader_id);
	}

 	display();

	// Disable the shader
	popShaderProgram();

	// Restore the attributes
    popEnableDisableState();
}

//----------------------------------------------
void InternalOrganMesh::generate3DTexture(Shader const& shader,
		unsigned int& frame_buffer, unsigned int& texture, ToCreate to_create)
//----------------------------------------------
{
	PolygonMesh square_filler;
	std::vector<VEC3 > vertex_data;

	VEC3 v1(0, 0, 0);
	VEC3 v2(1, 0, 0);
	VEC3 v3(1, 1, 0);
	VEC3 v4(0, 1, 0);

	vertex_data.reserve(6);

	vertex_data.push_back(v1);
	vertex_data.push_back(v2);
	vertex_data.push_back(v3);

	vertex_data.push_back(v1);
	vertex_data.push_back(v3);
	vertex_data.push_back(v4);

	square_filler.setInternalData(GL_TRIANGLES, (std::vector<float>*)(&vertex_data), false, GL_STATIC_DRAW);

	//render to the bump buffer
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
	glViewport(0, 0, m_texture_size_3d, m_texture_size_3d);

	// Disable back face culling
	pushEnableDisableState(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);

	// Enable the bump shader
	pushShaderProgram();
	shader.enable();

    // Check the OpenGL error status
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    GLint shader_id(shader.getProgramHandle());

	switch(to_create)
	{
	case TO_CREATE_BUMP_MAP:
		defineBumpMapShaderInput(shader_id);
		break;
	case TO_CREATE_COLOUR_TEXTURE:
		defineColourTextureShaderInput(shader_id);
		break;
	default:
		defineBumpMapShaderInput(shader_id);
	}

	for(unsigned int i(0); i < m_texture_size_3d; i++)
	{
		//link the frame buffer and the texture
		glFramebufferTexture3D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_3D, texture, 0, i);

		//check the creation of the buffer
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot create the frame buffer.");
		}

		square_filler.display();

		square_filler.applyTranslation(VEC3(0,0,1.f/m_texture_size_3d));
	}

	// Check the OpenGL error status
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Disable the shader
	popShaderProgram();

	// Restore the attributes
    popEnableDisableState();

    // Check the OpenGL error status
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}

//----------------------------------------------
void InternalOrganMesh::defineBumpMapShaderInput(int shader_id)
//----------------------------------------------
{
	GLuint handle(glGetUniformLocation(shader_id, "g_has_tendon"));
	glUniform1iv(handle, 1, &m_has_tendon);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	handle = glGetUniformLocation(shader_id, "g_is_lung");
	glUniform1iv(handle, 1, &m_is_lung);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	handle = glGetUniformLocation(shader_id, "g_has_thin_bumps");
	glUniform1iv(handle, 1, &m_has_thin_bumps);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	handle = glGetUniformLocation(shader_id, "g_perm");
	glUniform1iv(handle, TAB_SIZE,&m_permutation[0]);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	handle = glGetUniformLocation(shader_id, "g_gradient_tab");
	glUniform3fv(handle, TAB_SIZE,(const GLfloat*) &m_gradient_tab[0]);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}


//----------------------------------------------
void InternalOrganMesh::initialize2DBuffer(unsigned int& aTexture, unsigned int& aFrameBuffer)
//----------------------------------------------
{
	//Intialize the buffer for the texture
	glGenTextures(1, &aTexture);
	glBindTexture(GL_TEXTURE_2D, aTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16, m_texture_size_2d, m_texture_size_2d, 0, GL_RGB, GL_FLOAT, 0);

	//Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//initalize the frame buffer
	glGenFramebuffers(1, &aFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, aFrameBuffer);

	//link the frame buffer and the texture
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, aTexture, 0);

	//check the creation of the buffer
	if(!(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE))
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot create the frame buffer.");
	}

	// Cleanup for now
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

//----------------------------------------------
void InternalOrganMesh::initialize3DBuffer(unsigned int& aTexture, unsigned int& aFrameBuffer, bool has_colours)
//----------------------------------------------
{
	//Intialize the buffer for the texture
	glGenTextures(1, &aTexture);
	glBindTexture(GL_TEXTURE_3D, aTexture);

	if(has_colours)
	{
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB16, m_texture_size_3d, m_texture_size_3d, m_texture_size_3d, 0, GL_RGB, GL_FLOAT, 0);
	}
	else
	{
		glTexImage3D(GL_TEXTURE_3D, 0, GL_R16, m_texture_size_3d, m_texture_size_3d, m_texture_size_3d, 0, GL_RED, GL_FLOAT, 0);
	}

	//Filtering
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	//initalize the frame buffer
	glGenFramebuffers(1, &aFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, aFrameBuffer);

	//link the frame buffer and the texture
	glFramebufferTexture3D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_3D, aTexture, 0, 0);

	//check the creation of the buffer
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot create the frame buffer.");
	}

	// Cleanup for now
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_3D, 0);
}

//----------------------------------------------
void InternalOrganMesh::load2DBumpShader()
//----------------------------------------------
{
    char* p_vertex_shader(0);
    char* p_fragment_shader(0);

    int z_lib_return_code_vertex(0);
    int z_lib_return_code_fragment(0);

    std::string vertex_shader;
    std::string fragment_shader;

    // Bump map generation shader
    // Use OpenGL 4.5
    if (useOpenGL45())
    {
        z_lib_return_code_vertex   = inflate(g_bump_map_generation_gl4_vert, sizeof(g_bump_map_generation_gl4_vert),   &p_vertex_shader);
        z_lib_return_code_fragment = inflate(g_bump_map_generation_gl4_frag, sizeof(g_bump_map_generation_gl4_frag), &p_fragment_shader);
        m_bump_map_shader.setLabels("bump_map_generation_gl3.vert", "bump_map_generation_gl3.frag");
    }
    // Use OpenGL 3.2
    else if (useOpenGL32())
    {
        z_lib_return_code_vertex   = inflate(g_bump_map_generation_gl3_vert, sizeof(g_bump_map_generation_gl3_vert),   &p_vertex_shader);
        z_lib_return_code_fragment = inflate(g_bump_map_generation_gl3_frag, sizeof(g_bump_map_generation_gl3_frag), &p_fragment_shader);
        m_bump_map_shader.setLabels("bump_map_generation_gl3.vert", "bump_map_generation_gl3.frag");
    }
    // Fall back to OpenGL 2.x
    else
    {
				z_lib_return_code_vertex   = inflate(g_bump_map_generation_gl2_vert, sizeof(g_bump_map_generation_gl2_vert),   &p_vertex_shader);
				z_lib_return_code_fragment = inflate(g_bump_map_generation_gl2_frag, sizeof(g_bump_map_generation_gl2_frag), &p_fragment_shader);
				m_bump_map_shader.setLabels("bump_map_generation_gl2.vert", "bump_map_generation_gl2.frag");
        // throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot create shader programs for your chosen OpenGL context.");
    }

    vertex_shader   = p_vertex_shader;
    fragment_shader = p_fragment_shader;
    delete [] p_vertex_shader;     p_vertex_shader = 0;
    delete [] p_fragment_shader; p_fragment_shader = 0;

    if (z_lib_return_code_vertex <= 0 || z_lib_return_code_fragment <= 0 || !vertex_shader.size() || !fragment_shader.size())
    {
	    	throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot decode the shader using ZLib.");
    }
    m_bump_map_shader.loadSource(vertex_shader, fragment_shader);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}

//----------------------------------------------
void InternalOrganMesh::load3DBumpShader()
//----------------------------------------------
{
    char* p_vertex_shader(0);
    char* p_fragment_shader(0);

    int z_lib_return_code_vertex(0);
    int z_lib_return_code_fragment(0);

    std::string vertex_shader;
    std::string fragment_shader;

    // Bump map generation shader
    // Use OpenGL 4.5
    if (useOpenGL45())
    {
        z_lib_return_code_vertex   = inflate(g_3d_bump_map_generation_gl3_vert, sizeof(g_3d_bump_map_generation_gl3_vert),   &p_vertex_shader);
        z_lib_return_code_fragment = inflate(g_3d_bump_map_generation_gl3_frag, sizeof(g_3d_bump_map_generation_gl3_frag), &p_fragment_shader);
        m_bump_map_shader.setLabels("3d_bump_map_generation_gl3.vert", "3d_bump_map_generation_gl3.frag");
    }
    // Use OpenGL 3.2
    else if (useOpenGL32())
    {
        z_lib_return_code_vertex   = inflate(g_3d_bump_map_generation_gl3_vert, sizeof(g_3d_bump_map_generation_gl3_vert),   &p_vertex_shader);
        z_lib_return_code_fragment = inflate(g_3d_bump_map_generation_gl3_frag, sizeof(g_3d_bump_map_generation_gl3_frag), &p_fragment_shader);
        m_bump_map_shader.setLabels("3d_bump_map_generation_gl3.vert", "3d_bump_map_generation_gl3.frag");
    }
    // Fall back to OpenGL 2.x
    else
    {
				z_lib_return_code_vertex   = inflate(g_3d_bump_map_generation_gl2_vert, sizeof(g_3d_bump_map_generation_gl2_vert),   &p_vertex_shader);
				z_lib_return_code_fragment = inflate(g_3d_bump_map_generation_gl2_frag, sizeof(g_3d_bump_map_generation_gl2_frag), &p_fragment_shader);
				m_bump_map_shader.setLabels("3d_bump_map_generation_gl2.vert", "3d_bump_map_generation_gl2.frag");
        // throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot create shader programs for your chosen OpenGL context.");
    }

    vertex_shader   = p_vertex_shader;
    fragment_shader = p_fragment_shader;
    delete [] p_vertex_shader;     p_vertex_shader = 0;
    delete [] p_fragment_shader; p_fragment_shader = 0;

    if (z_lib_return_code_vertex <= 0 || z_lib_return_code_fragment <= 0 || !vertex_shader.size() || !fragment_shader.size())
    {
	    	throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot decode the shader using ZLib.");
    }
    m_bump_map_shader.loadSource(vertex_shader, fragment_shader);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}

//----------------------------------------------
void InternalOrganMesh::initialize2DTextureCoordinates()
//----------------------------------------------
{
	unsigned int n(int(sqrt(float(m_number_of_vertices)/6.))+2);

	if(m_optimize_texture_coordinates)
	{
		m_texture_coordinates_2d.resize(m_number_of_vertices);
		m_margin_direction.resize(m_number_of_vertices);
		std::vector<bool> has_texture_coordinates(m_number_of_vertices/3,false);
		unsigned int k(0);

		for (unsigned int i(0); i < n; i++) {
			for (unsigned int j(0); j < n; j++) {
				//extract four coordinates
				VEC2 v1(float(i)/n, float(j)/n);
				VEC2 v2(float(i + 1)/n, float(j)/n);
				VEC2 v3(float(i + 1)/n, float(j + 1)/n);
				VEC2 v4(float(i)/n, float(j + 1)/n);

				while(k<m_number_of_vertices/3 && has_texture_coordinates[k])
				{
					k++;
				}
				if(k<m_number_of_vertices/3)
				{
					unsigned int first_triangle_indice(3 * k);
					has_texture_coordinates[k] = true;
					k++;

					bool matching_face_not_found(true);
					unsigned int matching_face_indice(first_triangle_indice+3);
					VEC3 first_vertex(getVertex(first_triangle_indice));
					VEC3 second_vertex(getVertex(first_triangle_indice+1));
					VEC3 third_vertex(getVertex(first_triangle_indice+2));

					while(matching_face_not_found && matching_face_indice < m_number_of_vertices)
					{
						VEC3 first_matching_vertex(getVertex(matching_face_indice));
						VEC3 second_matching_vertex(getVertex(matching_face_indice+1));
						VEC3 third_matching_vertex(getVertex(matching_face_indice+2));

						if(!has_texture_coordinates[matching_face_indice/3])
						{
							if((third_matching_vertex==second_vertex)
									&& (first_matching_vertex==first_vertex))
								{
									//the first triangle
									m_texture_coordinates_2d[first_triangle_indice] = v3;
									m_texture_coordinates_2d[first_triangle_indice+1] = v1;
									m_texture_coordinates_2d[first_triangle_indice+2] = v2;

									//the second triangle
									m_texture_coordinates_2d[matching_face_indice] = v3;
									m_texture_coordinates_2d[matching_face_indice+1] = v4;
									m_texture_coordinates_2d[matching_face_indice+2] = v1;

									//Set the margin direction
									m_margin_direction[first_triangle_indice] = VEC2(-1., -1.);
									m_margin_direction[first_triangle_indice+1] = VEC2(1., 1.);
									m_margin_direction[first_triangle_indice+2] = VEC2(-1., 1.);

									m_margin_direction[matching_face_indice] = VEC2(-1., -1.);
									m_margin_direction[matching_face_indice+1] = VEC2(1., -1.);
									m_margin_direction[matching_face_indice+2] = VEC2(1., 1.);

									matching_face_not_found = false;
									has_texture_coordinates[matching_face_indice/3]=true;
								}
								else if((first_matching_vertex==second_vertex)
									&& (second_matching_vertex==first_vertex))
								{
									//the first triangle
									m_texture_coordinates_2d[first_triangle_indice] = v3;
									m_texture_coordinates_2d[first_triangle_indice+1] = v1;
									m_texture_coordinates_2d[first_triangle_indice+2] = v2;

									//the second triangle
									m_texture_coordinates_2d[matching_face_indice] = v1;
									m_texture_coordinates_2d[matching_face_indice+1] = v3;
									m_texture_coordinates_2d[matching_face_indice+2] = v4;

									//Set the margin direction
									m_margin_direction[first_triangle_indice] = VEC2(-1., -1.);
									m_margin_direction[first_triangle_indice+1] = VEC2(1., 1.);
									m_margin_direction[first_triangle_indice+2] = VEC2(-1., 1.);

									m_margin_direction[matching_face_indice] = VEC2(1., 1.);
									m_margin_direction[matching_face_indice+1] = VEC2(-1., -1.);
									m_margin_direction[matching_face_indice+2] = VEC2(1., -1.);

									matching_face_not_found = false;
									has_texture_coordinates[matching_face_indice/3]=true;
								}
								else if((second_matching_vertex == second_vertex)
									&& (third_matching_vertex == first_vertex))
								{
									//the first triangle
									m_texture_coordinates_2d[first_triangle_indice] = v3;
									m_texture_coordinates_2d[first_triangle_indice+1] = v1;
									m_texture_coordinates_2d[first_triangle_indice+2] = v2;

									//the second triangle
									m_texture_coordinates_2d[matching_face_indice] = v4;
									m_texture_coordinates_2d[matching_face_indice+1] = v1;
									m_texture_coordinates_2d[matching_face_indice+2] = v3;

									//Set the margin direction
									m_margin_direction[first_triangle_indice] = VEC2(-1., -1.);
									m_margin_direction[first_triangle_indice+1] = VEC2(1., 1.);
									m_margin_direction[first_triangle_indice+2] = VEC2(-1., 1.);

									m_margin_direction[matching_face_indice] = VEC2(1., -1.);
									m_margin_direction[matching_face_indice+1] = VEC2(1., 1.);
									m_margin_direction[matching_face_indice+2] = VEC2(-1., -1.);

									matching_face_not_found = false;
									has_texture_coordinates[matching_face_indice/3]=true;
								}
								else

								if((third_matching_vertex==third_vertex)
									&& (first_matching_vertex==second_vertex))
								{
									//the first triangle
									m_texture_coordinates_2d[first_triangle_indice] = v2;
									m_texture_coordinates_2d[first_triangle_indice+1] = v3;
									m_texture_coordinates_2d[first_triangle_indice+2] = v1;

									//the second triangle
									m_texture_coordinates_2d[matching_face_indice] = v3;
									m_texture_coordinates_2d[matching_face_indice+1] = v4;
									m_texture_coordinates_2d[matching_face_indice+2] = v1;

									//Set the margin direction
									m_margin_direction[first_triangle_indice] = VEC2(-1., 1.);
									m_margin_direction[first_triangle_indice+1] = VEC2(-1., -1.);
									m_margin_direction[first_triangle_indice+2] = VEC2(1., 1.);

									m_margin_direction[matching_face_indice] = VEC2(-1., -1.);
									m_margin_direction[matching_face_indice+1] = VEC2(1., -1.);
									m_margin_direction[matching_face_indice+2] = VEC2(1., 1.);

									matching_face_not_found = false;
									has_texture_coordinates[matching_face_indice/3]=true;
								}
								else if((first_matching_vertex==third_vertex)
									&& (second_matching_vertex==second_vertex))
								{
									//the first triangle
									m_texture_coordinates_2d[first_triangle_indice] = v2;
									m_texture_coordinates_2d[first_triangle_indice+1] = v3;
									m_texture_coordinates_2d[first_triangle_indice+2] = v1;

									//the second triangle
									m_texture_coordinates_2d[matching_face_indice] = v1;
									m_texture_coordinates_2d[matching_face_indice+1] = v3;
									m_texture_coordinates_2d[matching_face_indice+2] = v4;

									//Set the margin direction
									m_margin_direction[first_triangle_indice] = VEC2(-1., 1.);
									m_margin_direction[first_triangle_indice+1] = VEC2(-1., -1.);
									m_margin_direction[first_triangle_indice+2] = VEC2(1., 1.);

									m_margin_direction[matching_face_indice] = VEC2(1., 1.);
									m_margin_direction[matching_face_indice+1] = VEC2(-1., -1.);
									m_margin_direction[matching_face_indice+2] = VEC2(1., -1.);

									matching_face_not_found = false;
									has_texture_coordinates[matching_face_indice/3]=true;
								}
								else if((second_matching_vertex == third_vertex)
									&& (third_matching_vertex == second_vertex))
								{
									//the first triangle
									m_texture_coordinates_2d[first_triangle_indice] = v2;
									m_texture_coordinates_2d[first_triangle_indice+1] = v3;
									m_texture_coordinates_2d[first_triangle_indice+2] = v1;

									//the second triangle
									m_texture_coordinates_2d[matching_face_indice] = v4;
									m_texture_coordinates_2d[matching_face_indice+1] = v1;
									m_texture_coordinates_2d[matching_face_indice+2] = v3;

									//Set the margin direction
									m_margin_direction[first_triangle_indice] = VEC2(-1., 1.);
									m_margin_direction[first_triangle_indice+1] = VEC2(-1., -1.);
									m_margin_direction[first_triangle_indice+2] = VEC2(1., 1.);

									m_margin_direction[matching_face_indice] = VEC2(1., -1.);
									m_margin_direction[matching_face_indice+1] = VEC2(1., 1.);
									m_margin_direction[matching_face_indice+2] = VEC2(-1., -1.);

									matching_face_not_found = false;
									has_texture_coordinates[matching_face_indice/3]=true;
								}
								else

								if((third_matching_vertex==first_vertex)
									&& (first_matching_vertex==third_vertex))
								{
									//the first triangle
									m_texture_coordinates_2d[first_triangle_indice] = v1;
									m_texture_coordinates_2d[first_triangle_indice+1] = v2;
									m_texture_coordinates_2d[first_triangle_indice+2] = v3;

									//the second triangle
									m_texture_coordinates_2d[matching_face_indice] = v3;
									m_texture_coordinates_2d[matching_face_indice+1] = v4;
									m_texture_coordinates_2d[matching_face_indice+2] = v1;

									//Set the margin direction
									m_margin_direction[first_triangle_indice] = VEC2(1., 1.);
									m_margin_direction[first_triangle_indice+1] = VEC2(-1., 1.);
									m_margin_direction[first_triangle_indice+2] = VEC2(-1., -1.);

									m_margin_direction[matching_face_indice] = VEC2(-1., -1.);
									m_margin_direction[matching_face_indice+1] = VEC2(1., -1.);
									m_margin_direction[matching_face_indice+2] = VEC2(1., 1.);

									matching_face_not_found = false;
									has_texture_coordinates[matching_face_indice/3]=true;
								}
								else if((first_matching_vertex==first_vertex)
									&& (second_matching_vertex==third_vertex))
								{
									//the first triangle
									m_texture_coordinates_2d[first_triangle_indice] = v1;
									m_texture_coordinates_2d[first_triangle_indice+1] = v2;
									m_texture_coordinates_2d[first_triangle_indice+2] = v3;

									//the second triangle
									m_texture_coordinates_2d[matching_face_indice] = v1;
									m_texture_coordinates_2d[matching_face_indice+1] = v3;
									m_texture_coordinates_2d[matching_face_indice+2] = v4;

									//Set the margin direction
									m_margin_direction[first_triangle_indice] = VEC2(1., 1.);
									m_margin_direction[first_triangle_indice+1] = VEC2(-1., 1.);
									m_margin_direction[first_triangle_indice+2] = VEC2(-1., -1.);

									m_margin_direction[matching_face_indice] = VEC2(1., 1.);
									m_margin_direction[matching_face_indice+1] = VEC2(-1., -1.);
									m_margin_direction[matching_face_indice+2] = VEC2(1., -1.);

									matching_face_not_found = false;
									has_texture_coordinates[matching_face_indice/3]=true;
								}
								else if((second_matching_vertex == first_vertex)
									&& (third_matching_vertex == third_vertex))
								{
									//the first triangle
									m_texture_coordinates_2d[first_triangle_indice] = v1;
									m_texture_coordinates_2d[first_triangle_indice+1] = v2;
									m_texture_coordinates_2d[first_triangle_indice+2] = v3;

									//the second triangle
									m_texture_coordinates_2d[matching_face_indice] = v4;
									m_texture_coordinates_2d[matching_face_indice+1] = v1;
									m_texture_coordinates_2d[matching_face_indice+2] = v3;

									//Set the margin direction
									m_margin_direction[first_triangle_indice] = VEC2(1., 1.);
									m_margin_direction[first_triangle_indice+1] = VEC2(-1., 1.);
									m_margin_direction[first_triangle_indice+2] = VEC2(-1., -1.);

									m_margin_direction[matching_face_indice] = VEC2(1., -1.);
									m_margin_direction[matching_face_indice+1] = VEC2(1., 1.);
									m_margin_direction[matching_face_indice+2] = VEC2(-1., -1.);

									matching_face_not_found = false;
									has_texture_coordinates[matching_face_indice/3]=true;
								}
								else
								{
									matching_face_indice+=3;
								}
							}
							else
							{
								matching_face_indice+=3;
							}
					}
					//In case there is no matching triangle
					if(matching_face_indice >= m_number_of_vertices)
					{
						//only one triangle
						m_texture_coordinates_2d[first_triangle_indice] = v1;
						m_texture_coordinates_2d[first_triangle_indice+1] = v2;
						m_texture_coordinates_2d[first_triangle_indice+2] = v3;

						//Set the margin direction
						m_margin_direction[first_triangle_indice] = VEC2(2., 1.);
						m_margin_direction[first_triangle_indice+1] = VEC2(-1., 1.);
						m_margin_direction[first_triangle_indice+2] = VEC2(-1., -2.);
					}
				}
			}
		}
	}
	else
	{
		m_texture_coordinates_2d.reserve(m_number_of_vertices);
		m_margin_direction.reserve(m_number_of_vertices);

		for (unsigned int i(0); i < n; i++) {
			for (unsigned int j(0); j < n; j++) {
				//extract four coordinates
				VEC2 v1(float(i)/n, float(j)/n);
				VEC2 v2(float(i + 1)/n, float(j)/n);
				VEC2 v3(float(i + 1)/n, float(j + 1)/n);
				VEC2 v4(float(i)/n, float(j + 1)/n);

				//the first triangle
				m_texture_coordinates_2d.push_back(v1);
				m_texture_coordinates_2d.push_back(v2);
				m_texture_coordinates_2d.push_back(v3);

				//the second triangle
				m_texture_coordinates_2d.push_back(v1);
				m_texture_coordinates_2d.push_back(v3);
				m_texture_coordinates_2d.push_back(v4);

				m_margin_direction.push_back(VEC2(1., 1.));
				m_margin_direction.push_back(VEC2(-1., 1.));
				m_margin_direction.push_back(VEC2(-1., -1.));

				m_margin_direction.push_back(VEC2(1., 1.));
				m_margin_direction.push_back(VEC2(-1., -1.));
				m_margin_direction.push_back(VEC2(1., -1.));
			}
		}

	}

}

//----------------------------------------------
void InternalOrganMesh::initialize3DTextureCoordinates()
//----------------------------------------------
{
	// Initialize the texture coordinates using the vertices position
    m_texture_coordinates_3d.reserve(m_number_of_vertices);

	for(unsigned int i(0); i < m_number_of_vertices; i++)
    {
    	m_texture_coordinates_3d.push_back(getVertex(i));
    }
}

#if 0
//----------------------------------------------
void InternalOrganMesh::addMargin(float margin)
//----------------------------------------------
{
	//Change the sampling coordinates to avoid artifacts at the boundary of the polygons
	unsigned int k(0);
	for (unsigned int i(0); i < n; i++) {
		for (unsigned int j(0); j < n; j++) {
			//the first triangle
			m_texture_coordinates_2d[k]+=VEC2(2*margin,margin);
			k++;
			m_texture_coordinates_2d[k]+=VEC2(-margin,margin);
			k++;
			m_texture_coordinates_2d[k]+=VEC2(-margin,-2*margin);
			k++;

			//the second triangle
			m_texture_coordinates_2d[k]+=VEC2(margin,2*margin);
			k++;
			m_texture_coordinates_2d[k]+=VEC2(-2*margin,-margin);
			k++;
			m_texture_coordinates_2d[k]+=VEC2(margin,-margin);
			k++;
		}
	}
}

/*unsigned int n(int(sqrt(float(m_number_of_vertices))/2.4));
m_texture_coordinates_2d.reserve(m_number_of_vertices);
m_margin_direction.reserve(m_number_of_vertices);

for (unsigned int i(0); i < n; i++) {
	for (unsigned int j(0); j < n; j++) {
		//extract four coordinates
		VEC2 v1(float(i)/n, float(j)/n);
		VEC2 v2(float(i + 1)/n, float(j)/n);
		VEC2 v3(float(i + 1)/n, float(j + 1)/n);
		VEC2 v4(float(i)/n, float(j + 1)/n);

		//the first triangle
		m_texture_coordinates_2d.push_back(v1);
		m_texture_coordinates_2d.push_back(v2);
		m_texture_coordinates_2d.push_back(v3);

		//the second triangle
		m_texture_coordinates_2d.push_back(v1);
		m_texture_coordinates_2d.push_back(v3);
		m_texture_coordinates_2d.push_back(v4);

		m_margin_direction.push_back(VEC2(1., 1.));
		m_margin_direction.push_back(VEC2(-1., 1.));
		m_margin_direction.push_back(VEC2(-1., -1.));

		m_margin_direction.push_back(VEC2(1., 1.));
		m_margin_direction.push_back(VEC2(-1., -1.));
		m_margin_direction.push_back(VEC2(1., -1.));
	}
}*/

#endif

struct VectorComparer
{
	// Return true if aLeftNode is less than aRightNode
    bool operator()(const VEC3 & aLeftNode, const VEC3 & aRightNode) const
    {
    	double epsilon(EPSILON);

    	if (fabs(aLeftNode.getX() - aRightNode.getX()) > epsilon)
    	{
    		if (aLeftNode.getX() < aRightNode.getX())
    		{
    			return (true);
    		}
    		else
    		{
    			return (false);
    		}
    	}

    	if (fabs(aLeftNode.getY() - aRightNode.getY()) > epsilon)
    	{
    		if (aLeftNode.getY() < aRightNode.getY())
    		{
    			return (true);
    		}
    		else
    		{
    			return (false);
    		}
    	}

    	if (fabs(aLeftNode.getZ() - aRightNode.getZ()) > epsilon)
    	{
    		if (aLeftNode.getZ() < aRightNode.getZ())
    		{
    			return (true);
    		}
    		else
    		{
    			return (false);
    		}
    	}

    	return (false);
    }
};


class TempRecord
{
public:
	inline const TempRecord& operator +=(const VEC3& aNormal)
	{
		m_normal += aNormal;

		return (*this);
	}


	inline const TempRecord& operator =(unsigned int anID)
	{
		m_id = anID;

		return (*this);
	}

	VEC3 m_normal;
	unsigned int m_id;
};

//----------------------------------------------
void InternalOrganMesh::shareNormalVectors()
//----------------------------------------------
{
	// Add the vertices to the hash table
	std::map<VEC3, TempRecord, VectorComparer> hash_table;

	//Compute the vertices normal vectors (sum of the adjacent faces normal vectors)
	for (unsigned int i(0); i < m_number_of_vertices; ++i)
	{
		VEC3 current_vertex(getVertex(i));
		VEC3 normal(getVertexNormal(i));

		hash_table[current_vertex].m_normal += normal;
	}

	//set the vertices normal data
	m_p_vertex_normal_set.clear();
	m_p_vertex_normal_set.resize(m_number_of_vertices * 3);

	for (unsigned int i(0); i < m_number_of_vertices; ++i)
	{
		VEC3 current_vertex(getVertex(i));
		VEC3 current_normal(hash_table[current_vertex].m_normal);

		m_p_vertex_normal_set[i*3] =  current_normal.getX();
		m_p_vertex_normal_set[i*3+1] =  current_normal.getY();
		m_p_vertex_normal_set[i*3+2] =  current_normal.getZ();
	}

	//m_p_face_normal_set.clear();

	normaliseNormals();
}

void InternalOrganMesh::deleteIndices()
{
	if(m_number_of_indices)
	{
		// Reset the normal per face set
		m_p_vertex_normal_set.clear();

		// Reset the normal per face set
		//m_p_face_normal_set.clear();

		// Store the new vertices
		std::vector<double> p_new_vertex_set;

		// Process every triangle
		unsigned int triangle_number(getTriangleNumber());
		for (unsigned int i(0); i < triangle_number; ++i)
		{
			// Get the current vertices
			int id_0(getIndex(i * 3 + 0));
			int id_1(getIndex(i * 3 + 1));
			int id_2(getIndex(i * 3 + 2));

			VEC3 vertex_0(getVertex(id_0));
			VEC3 vertex_1(getVertex(id_1));
			VEC3 vertex_2(getVertex(id_2));

			// Compute the normal vector
			VEC3 normal((vertex_2 - vertex_0)^(vertex_2 - vertex_1));
			normal.normalize();

			// Add the first triangle
			p_new_vertex_set.push_back(vertex_0.getX());
			p_new_vertex_set.push_back(vertex_0.getY());
			p_new_vertex_set.push_back(vertex_0.getZ());
			p_new_vertex_set.push_back(vertex_1.getX());
			p_new_vertex_set.push_back(vertex_1.getY());
			p_new_vertex_set.push_back(vertex_1.getZ());
			p_new_vertex_set.push_back(vertex_2.getX());
			p_new_vertex_set.push_back(vertex_2.getY());
			p_new_vertex_set.push_back(vertex_2.getZ());

			for (unsigned int j(0); j < 3; ++j)
			{
				m_p_vertex_normal_set.push_back(normal.getX());
				m_p_vertex_normal_set.push_back(normal.getY());
				m_p_vertex_normal_set.push_back(normal.getZ());
			}
		}

		// Set the index in the hash table and create the list of vertices
		destroyIndexData();
		destroyVertexData();
		m_number_of_vertices = p_new_vertex_set.size() / 3;

		GLfloat* p_float_vertex_set(0);
		GLdouble* p_double_vertex_set(0);
		switch (m_vertex_data_type)
		{
		case GL_FLOAT:
			m_p_vertex_set = new GLfloat[m_number_of_vertices * 3];
			p_float_vertex_set = reinterpret_cast<GLfloat*>(m_p_vertex_set);
			break;

		case GL_DOUBLE:
			m_p_vertex_set = new GLdouble[m_number_of_vertices * 3];
			p_double_vertex_set = reinterpret_cast<GLdouble*>(m_p_vertex_set);
			break;

		default:
			throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid data type.");
		}

		// Save the result
		for (unsigned int id(0); id < p_new_vertex_set.size(); ++id)
		{
			if (p_float_vertex_set)
			{
				p_float_vertex_set[id] = p_new_vertex_set[id];
			}
			if (p_double_vertex_set)
			{
				p_double_vertex_set[id] = p_new_vertex_set[id];
			}
		}

		computeNormalVectors();
	}
}

//----------------------------------------------
void InternalOrganMesh::initGradientTab()
//----------------------------------------------
{
	float z, r, theta;

	for(unsigned int i(0); i<TAB_SIZE; i++)
	{
		z= 1 - 2*(float(rand()) / RAND_MAX);
		r= sqrt(1-z*z);
		theta=2 * Pi * (float(rand()) / RAND_MAX);

		m_gradient_tab.push_back(VEC3(
				r*cos(theta),
				r*sin(theta),
				z
			));
	}
}

//----------------------------------------------
void InternalOrganMesh::initPermutation()
//----------------------------------------------
{
	for(unsigned int i(0); i<TAB_SIZE; i++)
	{

		//generate random number until we can add it to the array
		bool nothing_added(true);
		while(nothing_added)
		{

			//generate a random number between 0 and 255
			int random_number(rand()%TAB_SIZE);

			//if the random number isn't already in the array
			if(std::find(m_permutation.begin(), m_permutation.end(), random_number) == m_permutation.end())
			{
				//add it to the array
				m_permutation.push_back(random_number);
				nothing_added = false;
			}
		}
	}
}


//---------------------------------------------------------------------
void InternalOrganMesh::copyFrom(const InternalOrganMesh& aPolygonMesh)
//---------------------------------------------------------------------
{
    AnatomicalMesh::copyFrom(aPolygonMesh);

    m_gradient_tab = aPolygonMesh.m_gradient_tab;
    m_permutation = aPolygonMesh.m_permutation;
    m_texture_coordinates_2d = aPolygonMesh.m_texture_coordinates_2d;
    m_texture_coordinates_3d = aPolygonMesh.m_texture_coordinates_3d;
    m_margin_direction = aPolygonMesh.m_margin_direction;
    m_bump_map_texture = aPolygonMesh.m_bump_map_texture;
    m_bump_map_frame_buffer = aPolygonMesh.m_bump_map_frame_buffer;
    m_bump_map_shader = aPolygonMesh.m_bump_map_shader;
    m_has_tendon = aPolygonMesh.m_has_tendon;
    m_is_lung = aPolygonMesh.m_is_lung;
    m_has_thin_bumps = aPolygonMesh.m_has_thin_bumps;
    m_use_3D_bump_map = aPolygonMesh.m_use_3D_bump_map;
    m_texture_size_2d = aPolygonMesh.m_texture_size_2d;
    m_texture_size_3d = aPolygonMesh.m_texture_size_3d;
    m_need_initialize = aPolygonMesh.m_need_initialize;
    m_generation_texture_coord_are_set = aPolygonMesh.m_generation_texture_coord_are_set;
    m_optimize_texture_coordinates = aPolygonMesh.m_optimize_texture_coordinates;
}


//------------------------------------------------------------------------------------
InternalOrganMesh& InternalOrganMesh::operator=(const InternalOrganMesh& aPolygonMesh)
//------------------------------------------------------------------------------------
{
    InternalOrganMesh::copyFrom(aPolygonMesh);

    return (*this);
}


} // namespace gVirtualXRay
