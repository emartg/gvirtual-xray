/**
*******************************************************************************
*
*  @file       DepthMap.cxx
*
*  @brief      Class to render to a buffer in order to create, for exemple, a shadow map.
*
*  @version    1.0
*
*  @date       19/07/2016
*
*  @author     Andréas Meuleman
*******************************************************************************
*/

//******************************************************************************
//	Include
//******************************************************************************
#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <iostream>
#include <fstream>

#ifndef GVXR_UTILITIES_H
#include "gVirtualXRay/Utilities.h"
#endif

#include "gVirtualXRay/DepthMap.h"

#include "depth_map_gl2.frag.h"
#include "depth_map_gl2.vert.h"

#include "depth_map_gl3.frag.h"
#include "depth_map_gl3.vert.h"

#include "depth_map_gl4.frag.h"
#include "depth_map_gl4.vert.h"


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {

//------------------------------------------------------------------------
DepthMap::DepthMap(int map_size) :
//------------------------------------------------------------------------
	m_is_initialized(false),
	m_map_frame_buffer(0),
	m_map_texture(0),
	m_map_size(map_size)
//------------------------------------------------------------------------
{
}


//------------------------------------------------------------------------
DepthMap::~DepthMap()
//------------------------------------------------------------------------
{
    release();
}


//----------------------
void DepthMap::release()
//----------------------
{
    m_is_initialized = false;

    m_depth_shader.release();

    if (m_map_frame_buffer)
        {
        glDeleteFramebuffers(1, &m_map_frame_buffer);
        m_map_frame_buffer = 0;
        }

    if (m_map_texture)
        {
        glDeleteTextures(1, &m_map_texture);
        m_map_texture = 0;
        }

    m_map_size = 0;
}


//------------------------------------------------------------------------
void DepthMap::initialize()
//------------------------------------------------------------------------
{
	//Intialize the buffer for the depth map texture and enable the sampling in the fragment shader as a sampler2DShadow
	glGenTextures(1, &m_map_texture);
	glBindTexture(GL_TEXTURE_2D, m_map_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, m_map_size, m_map_size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

	//initalize the frame buffer for the shadow map
	glGenFramebuffers(1, &m_map_frame_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_map_frame_buffer);

	//link the frame buffer and the texture
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_map_texture, 0);

	//Only depth
	glDrawBuffer(GL_NONE);

	//check the creation of the buffer
	if(!(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE))
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot create the frame buffer.");
	}

	// Cleanup for now
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	//load the shader
    char* p_vertex_shader(0);
    char* p_fragment_shader(0);

    int z_lib_return_code_vertex(0);
    int z_lib_return_code_fragment(0);

    std::string vertex_shader;
    std::string fragment_shader;

    // Use OpenGL 4.5
    if (useOpenGL45())
    {
        z_lib_return_code_vertex   = inflate(g_depth_map_gl4_vert, sizeof(g_depth_map_gl4_vert),   &p_vertex_shader);
        z_lib_return_code_fragment = inflate(g_depth_map_gl4_frag, sizeof(g_depth_map_gl4_frag), &p_fragment_shader);
        m_depth_shader.setLabels("depth_map_gl4.vert", "depth_map_gl4.frag");
    }
    // Use OpenGL 3.2
    else if (useOpenGL32())
    {
        z_lib_return_code_vertex   = inflate(g_depth_map_gl3_vert, sizeof(g_depth_map_gl3_vert),   &p_vertex_shader);
        z_lib_return_code_fragment = inflate(g_depth_map_gl3_frag, sizeof(g_depth_map_gl3_frag), &p_fragment_shader);
        m_depth_shader.setLabels("depth_map_gl3.vert", "depth_map_gl3.frag");
    }
    // Fall back to OpenGL 2.x
    else
    {
				z_lib_return_code_vertex   = inflate(g_depth_map_gl2_vert, sizeof(g_depth_map_gl2_vert),   &p_vertex_shader);
				z_lib_return_code_fragment = inflate(g_depth_map_gl2_frag, sizeof(g_depth_map_gl2_frag), &p_fragment_shader);
				m_depth_shader.setLabels("depth_map_gl2.vert", "depth_map_gl2.frag");
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
    m_depth_shader.loadSource(vertex_shader, fragment_shader);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	m_is_initialized = true;
}

//------------------------------------------------------------------------
void DepthMap::render(std::vector<PolygonMesh*>& mesh_set,
		Matrix4x4<GLfloat> const& proj_matrix)
//------------------------------------------------------------------------
{
	//Initialize if necessary
	if (!m_is_initialized)
	{
		initialize();
	}

	//render to the map buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_map_frame_buffer);
	glViewport(0, 0, m_map_size, m_map_size);

	// Clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable front face culling (to avoid shadow acne)
	pushEnableDisableState(GL_CULL_FACE);
	pushEnableDisableState(GL_DEPTH_TEST);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

	// Enable the shader
	pushShaderProgram();
	m_depth_shader.enable();
	GLint shader_id(m_depth_shader.getProgramHandle());

	// Handle for shader variables
	GLuint handle(0);

	//link the atribute to their IDs
	handle = glGetUniformLocation(shader_id, "matrix");
	glUniformMatrix4fv(handle, 1,  GL_FALSE, proj_matrix.get());
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	//render
	for (unsigned int i(0); i < mesh_set.size(); ++i)
	{
		mesh_set[i]->display();
	}

	// Disable the shader
	popShaderProgram();

    // Restore the attributes
    popEnableDisableState();
    popEnableDisableState();

	// Cleanup for now
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//------------------------------------------------------------------------
void DepthMap::render(std::vector<PolygonMesh*>& mesh_set, Matrix4x4<float> const& proj_matrix,
		std::vector< Matrix4x4<float> > const& model_matrix_set)
//------------------------------------------------------------------------
{
	if(model_matrix_set.size() != mesh_set.size())
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "The sizes of the matrix set and the polygon mesh set are different.");
	}

	//Initialize if necessary
	if (!m_is_initialized)
	{
		initialize();
	}

	//render to the map buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_map_frame_buffer);
	glViewport(0, 0, m_map_size, m_map_size);

	// Clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable back face culling
	pushEnableDisableState(GL_CULL_FACE);
	pushEnableDisableState(GL_DEPTH_TEST);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

	// Enable the shader
	pushShaderProgram();
	m_depth_shader.enable();
	GLint shader_id(m_depth_shader.getProgramHandle());

	// Handle for shader variables
	GLuint handle(0);

	//render
	for (unsigned int i(0); i < mesh_set.size(); ++i)
	{
		//compute the matrix
		//Matrix4x4<float> matrix(proj_matrix * model_matrix_set[i]);

		//link the atribute to their IDs
		handle = glGetUniformLocation(shader_id, "matrix");
		glUniformMatrix4fv(handle, 1,  GL_FALSE, proj_matrix.get());
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);


		mesh_set[i]->display();
	}

	// Disable the shader
	popShaderProgram();

    // Restore the attributes
    popEnableDisableState();
    popEnableDisableState();

	// Cleanup for now
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

//------------------------------------------------------------------------
GLuint DepthMap::getMapTexture() const
//------------------------------------------------------------------------
{
	return m_map_texture;
}


} // namespace gVirtualXRay
