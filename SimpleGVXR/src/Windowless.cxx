/*

Copyright (c) 2020-2023, Dr Franck P. Vidal, Bangor University, All rights reserved.
Copyright (c) 2023-present, Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), 
UK Research and Innovation, All rights reserved.


Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

3. Neither the name of Bangor University, UK Research and Innovation nor the 
names of its contributors may be used to endorse or promote products derived 
from this software without specific prior written permission.

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
*   @file       Windowless.cxx
*
*   @brief      Windowless is a class to handle an OpenGL context create
*   			using EGL or GLX.
*
*   @date       18/04/2024
*
*   @author     Prof Franck P. Vidal
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Dr Franck P. Vidal, Bangor University, Sept 2020-Sept 2023, version 1.1, BSD 3-Clause License
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, Oct 2023-present, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <iostream>

#ifdef HAS_GLFW
#include <GLFW/glfw3.h>
#endif

// To perform the computations of the X-ray simulation
#ifndef GVXR_XRAY_RENDERER_H
#include "gVirtualXRay/XRayRenderer.h"
#endif

// To initialise GLEW, to define matrix stacks, to define state stacks, etc.
#ifndef OpenGLUtilities_h
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef GVXR_UTILITIES_H
#include "gVirtualXRay/Utilities.h"
#endif

#ifndef GVXR_SHADER_H
#include "gVirtualXRay/Shader.h"
#endif

// To load a scenegraph
#ifndef GVXR_SCENEGRAPH_BINDER_H
#include "gVirtualXRay/SceneGraphBinder.h"
#endif

#ifndef GVXR_LOGGER_H
#include <gVirtualXRay/Logger.h>
#endif

#ifndef GVXR_COORDINATE_SYSTEM_MESH_H
#include "gVirtualXRay/CoordinateSystemMesh.h"
#endif

#ifndef SGVXR_Windowless_h
#include "Windowless.h"
#endif


// Shaders for rendering
#include "display_gl2.frag.h"
#include "display_gl2.vert.h"

#include "display_gl3.frag.h"
#include "display_gl3.vert.h"

#include "display_gl4.frag.h"
#include "display_gl4.vert.h"


//******************************************************************************
//  Namespaces
//******************************************************************************


//******************************************************************************
//  Global variables
//******************************************************************************
extern double g_initial_fovy;
extern double g_initial_near;
extern double g_initial_far;
extern GLfloat g_zoom;

extern bool g_display_beam;
extern bool g_display_wireframe;
extern bool g_use_lighing;
extern bool g_display_detector;
extern bool g_display_normal_vectors;

extern gVirtualXRay::XRayBeam g_xray_beam;         // X-ray beam
extern gVirtualXRay::XRayDetector g_xray_detector; // X-ray detector
extern gVirtualXRay::XRayRenderer g_xray_renderer; // X-ray graphics engine

extern gVirtualXRay::Shader g_display_shader;

extern std::unique_ptr<gVirtualXRay::SceneGraphBinder> g_p_scene_graph;

extern gVirtualXRay::Matrix4x4<GLfloat> g_scene_rotation_matrix;
extern gVirtualXRay::Matrix4x4<GLfloat> g_scene_translation_matrix;

// Rotate the scanned objects
extern gVirtualXRay::Matrix4x4<GLfloat> g_sample_rotation_matrix;

// Bias/gain of the shift/scale filter for the display of the X-ray image
extern double g_shift_filter;
extern double g_scale_filter;

extern std::vector<std::vector<std::vector< float> > > lastest_screenshot;

extern gVirtualXRay::CoordinateSystemMesh g_world_coordinate_system_mesh;


//---------------------------------------------
SimpleGVXR::Windowless::Windowless(const std::string& aBackend,
                                 int anOpenGLMajorVersion,
                                 int anOpenGLMinorVersion):
//---------------------------------------------
    Window(false),
    m_p_offscreen_fbo(nullptr),
	m_first_initialisation(true)
//---------------------------------------------
{
    m_callback_registered = true;
    m_texture_name_set[0] = m_texture_name_set[1] = 0;

    // Create the context
	  m_context.create(aBackend,
			anOpenGLMajorVersion,
	        anOpenGLMinorVersion);

    // Context has not been created
    if (!m_context.isReady())
    {
        // Display a warning
        gVirtualXRay::LOGGER.logWarning("Windowless cannot be created.") << std::endl;
    }
    // Context has been created
    else
    {
        // Create the FBO
        m_p_offscreen_fbo.reset(new FramebufferObject);

        // Initialise shaders
        SimpleGVXR::Windowless::initShader();

        SimpleGVXR::Windowless::setWindowSize(m_width, m_height);
    }
}


//---------------------------------
SimpleGVXR::Windowless::~Windowless()
//---------------------------------
{
    if (m_p_offscreen_fbo)
    {
        m_p_offscreen_fbo->UnattachAll();
        m_p_offscreen_fbo.reset(nullptr);
    }
}


//-----------------------------------------------------------------
void SimpleGVXR::Windowless::setWindowSize(int aWidth, int aHeight)
//-----------------------------------------------------------------
{
	// Change size if needed
	if (m_width != aWidth || m_height != aHeight || m_first_initialisation)
	{
		m_first_initialisation = false;

		m_width = aWidth;
		m_height = aHeight;

		gVirtualXRay::pushFBO();

		m_p_offscreen_fbo->Bind();

		// Unattach the texture from the FBO and delete it if needed
		m_p_offscreen_fbo->UnattachAll();

		if (m_texture_name_set[0] != 0)
		{
			glDeleteTextures(1, &m_texture_name_set[0]);
		}

		if (m_texture_name_set[1] != 0)
		{
			glDeleteTextures(1, &m_texture_name_set[1]);
		}

		// Store the texturing flags
		gVirtualXRay::pushTexture2D();

		// Generate the texture name.
		glGenTextures(2, m_texture_name_set);
		gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		// Set the texture properties
		glBindTexture(GL_TEXTURE_2D, m_texture_name_set[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		GLint width(0);
		glTexImage2D(GL_PROXY_TEXTURE_2D,
		  0,
		  GL_RGBA,
		  m_width,
		  m_height,
		  0,
		  GL_RGBA,
		   GL_UNSIGNED_BYTE,
		  nullptr);

		glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
		if (width != m_width)
		{
			throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot create a colour texture for offscreen rendering.");
		}

		glTexImage2D(GL_TEXTURE_2D,
		  0,
		  GL_RGBA,
		  m_width,
		  m_height,
		  0,
		  GL_RGBA,
		   GL_UNSIGNED_BYTE,
		  nullptr);



		  // Set the texture properties
		  glBindTexture(GL_TEXTURE_2D, m_texture_name_set[1]);
		  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		  glPixelStorei(GL_PACK_ALIGNMENT, 1);
		  gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		  glTexImage2D(GL_PROXY_TEXTURE_2D,
			  0,
			  GL_RGBA,
			  m_width,
			  m_height,
			  0,
			  GL_RGBA,
			   GL_UNSIGNED_BYTE,
			  nullptr);

		  glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
		  if (width != m_width)
		  {
			  throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot create a colour texture for offscreen rendering.");
		  }

		  glTexImage2D(GL_TEXTURE_2D,
			  0,
			  GL_DEPTH_COMPONENT32F,
			  m_width,
			  m_height,
			  0,
			  GL_DEPTH_COMPONENT,
			  GL_FLOAT,
			  nullptr);

		// Attach texture to FBO
		GLenum texTarget[] = {GL_TEXTURE_2D, GL_TEXTURE_2D};
		GLenum attachment[] = {GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT};

		m_p_offscreen_fbo->AttachTextures(2, texTarget, m_texture_name_set, attachment);

		// Change draw/read buffers
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		gVirtualXRay::popTexture();
		gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

		// Check the FBO
		if (!m_p_offscreen_fbo->IsValid())
		{
			throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__, "The FBO has not been successfully created.");
		}

    int x(0), y(0), w(m_width), h(m_height);

    if (m_height == 0) {
        // Prevent divide by 0
        m_height = 1;
    }

    double screen_aspect_ratio(static_cast<double>(m_width) / static_cast<double>(m_height));
    glViewport(x, y, w, h);

    initViewer();

    gVirtualXRay::loadPerspectiveProjectionMatrix(g_initial_fovy,
    screen_aspect_ratio, g_initial_near, g_initial_far);

    gVirtualXRay::loadLookAtModelViewMatrix(0.0, 0.0, g_zoom,
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0);

		gVirtualXRay::popFBO();
	}
}


//----------------------------------------------------
void SimpleGVXR::Windowless::displayScene(bool aSceneRotationFlag)
//----------------------------------------------------
{
  gVirtualXRay::pushFBO();

  m_p_offscreen_fbo->Bind();

  // Initialise the viewer if needed
	initViewer();

  // Clear the buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Display the 3D scene
  render(aSceneRotationFlag);

  // Make sure all the OpenGL code is done
  glFinish();

  gVirtualXRay::popFBO();
}


//------------------------------------------
void SimpleGVXR::Windowless::takeScreenshot()
//------------------------------------------
{
    gVirtualXRay::pushFBO();

    m_p_offscreen_fbo->Bind();

    std::vector<GLfloat> p_pixel_set(3 * m_width * m_height);

    if (!p_pixel_set.empty())
    {
        glReadPixels(0, 0, m_width, m_height, GL_RGB, GL_FLOAT, &p_pixel_set[0]);

        lastest_screenshot.clear();

        for (int j = 0; j < m_height; ++j)
        {
            lastest_screenshot.emplace_back( );

            for (int i = 0; i < m_width; ++i)
            {
                lastest_screenshot.back().emplace_back();

                lastest_screenshot.back().back().push_back(p_pixel_set[(m_height - 1 - j) * m_width * 3 + i * 3 + 0]);
                lastest_screenshot.back().back().push_back(p_pixel_set[(m_height - 1 - j) * m_width * 3 + i * 3 + 1]);
                lastest_screenshot.back().back().push_back(p_pixel_set[(m_height - 1 - j) * m_width * 3 + i * 3 + 2]);
            }
        }
    }
    
    gVirtualXRay::popFBO();
}


//------------------------------------------------------------------------------
void SimpleGVXR::Windowless::setWindowBackGroundColour(float R, float G, float B)
//------------------------------------------------------------------------------
{
    gVirtualXRay::pushFBO();
    m_p_offscreen_fbo->Bind();

    Window::setWindowBackGroundColour(R, G, B);
    glClearColor(m_background_colour[0], m_background_colour[1], m_background_colour[2], 1.0);

    gVirtualXRay::popFBO();
}


//------------------------------------------------------------------------------
void SimpleGVXR::Windowless::setWindowBackGroundColor(float R, float G, float B)
//------------------------------------------------------------------------------
{
    setWindowBackGroundColour(R, G, B);
}


//---------------------------------------------------
void SimpleGVXR::Windowless::createWindow(bool aVisibilityFlag)
//---------------------------------------------------
{
	// Do nothing
}


//--------------------------------------
void SimpleGVXR::Windowless::initShader()
//--------------------------------------
{
    gVirtualXRay::LOGGER.logNow("OpenGL vendor:") << glGetString(GL_VENDOR) << std::endl;
    gVirtualXRay::LOGGER.logNow("OpenGL renderer:") << glGetString(GL_RENDERER) << std::endl;
    gVirtualXRay::LOGGER.logNow("OpenGL version:") << glGetString(GL_VERSION) << std::endl;

    gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Enable the Z-buffer
    glEnable (GL_DEPTH_TEST);
    gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Set the background colour
    SimpleGVXR::Windowless::setWindowBackGroundColour(m_background_colour[0], m_background_colour[1], m_background_colour[2]);
    gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    glEnable (GL_MULTISAMPLE);
    gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Initialise the shaders
    char* p_vertex_shader(nullptr);
    char* p_fragment_shader(nullptr);

    int z_lib_return_code_vertex(0);
    int z_lib_return_code_fragment(0);

    // Display shader
    // OpenGL 4.5
    if (gVirtualXRay::useOpenGL45())
    {
        z_lib_return_code_vertex = gVirtualXRay::inflate(
            g_display_gl4_vert,
            sizeof(g_display_gl4_vert),
            &p_vertex_shader);

        z_lib_return_code_fragment = gVirtualXRay::inflate(
            g_display_gl4_frag,
            sizeof(g_display_gl4_frag),
            &p_fragment_shader);

        g_display_shader.setLabels("display_gl4.vert", "display_gl4.frag");
    }
    // OpenGL 3.2
    else if (gVirtualXRay::useOpenGL32())
    {
        z_lib_return_code_vertex = gVirtualXRay::inflate(
            g_display_gl3_vert,
            sizeof(g_display_gl3_vert),
            &p_vertex_shader);

        z_lib_return_code_fragment = gVirtualXRay::inflate(
            g_display_gl3_frag,
            sizeof(g_display_gl3_frag),
            &p_fragment_shader);

        g_display_shader.setLabels("display_gl3.vert", "display_gl3.frag");
    }
    else
    {
        z_lib_return_code_vertex = gVirtualXRay::inflate(
            g_display_gl2_vert,
            sizeof(g_display_gl2_vert),
            &p_vertex_shader);

        z_lib_return_code_fragment = gVirtualXRay::inflate(
            g_display_gl2_frag,
            sizeof(g_display_gl2_frag),
            &p_fragment_shader);

        g_display_shader.setLabels("display_gl2.vert", "display_gl2.frag");
        // throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot create shader programs for your chosen OpenGL context.");
    }

    std::string vertex_shader = p_vertex_shader;
    std::string fragment_shader = p_fragment_shader;

    delete[] p_vertex_shader;
    delete[] p_fragment_shader;

    p_vertex_shader = nullptr;
    p_fragment_shader = nullptr;

    if (z_lib_return_code_vertex <= 0 ||
        z_lib_return_code_fragment <= 0 ||
        vertex_shader.empty() ||
        fragment_shader.empty())
    {
        throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
            "Cannot decode the shader using ZLib.");
    }

    g_display_shader.loadSource(vertex_shader, fragment_shader);
    gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    initViewer();
}


void SimpleGVXR::Windowless::render(bool aSceneRotationFlag)
{
    try
    {
        // Enable back face culling
        gVirtualXRay::pushEnableDisableState (GL_CULL_FACE);
        glEnable(GL_CULL_FACE);
        glCullFace (GL_BACK);

        // Enable the shader
        gVirtualXRay::pushShaderProgram();
        g_display_shader.enable();
        GLuint shader_id(g_display_shader.getProgramHandle());

        // Check the status of OpenGL and of the current FBO
        gVirtualXRay::checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);
        gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // Store the current transformation matrix
        gVirtualXRay::pushModelViewMatrix();

        // Rotate the sample
        if (aSceneRotationFlag)
        {
            gVirtualXRay::g_current_modelview_matrix *= g_scene_rotation_matrix;
            gVirtualXRay::g_current_modelview_matrix *= g_scene_translation_matrix;
        }

        GLint handle(0);
        handle = glGetUniformLocation(shader_id, "g_projection_matrix");
        glUniformMatrix4fv(handle, 1, GL_FALSE,
        gVirtualXRay::g_current_projection_matrix.get());
        gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        handle = glGetUniformLocation(shader_id, "g_modelview_matrix");
        glUniformMatrix4fv(handle, 1, GL_FALSE,
        gVirtualXRay::g_current_modelview_matrix.get());
        gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        gVirtualXRay::MATRIX4 normal_matrix(
        gVirtualXRay::g_current_modelview_matrix);
        handle = glGetUniformLocation(shader_id, "g_normal_matrix");
        glUniformMatrix3fv(handle, 1, GL_FALSE, normal_matrix.get3x3());
        gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        GLint lighting;
        if (g_use_lighing)
        {
            lighting = 1;
        }
        else
        {
            lighting = 0;
        }
        handle = glGetUniformLocation(shader_id, "g_use_lighting");
        glUniform1iv(handle, 1, &lighting);
        gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // Store the current transformation matrix
        gVirtualXRay::pushModelViewMatrix();

        gVirtualXRay::g_current_modelview_matrix *= g_sample_rotation_matrix;

        // Look for all the meshes in the scenegraph
        std::vector<gVirtualXRay::SceneGraphNode*> p_temp_pull;
        p_temp_pull.push_back(g_p_scene_graph->getNode("root"));

        while (!p_temp_pull.empty())
        {
            // Get the last node
            gVirtualXRay::SceneGraphNode* p_node = p_temp_pull.back();

            // Remove the node
            p_temp_pull.pop_back();

            // Add its children
            for (unsigned int i = 0; i < p_node->getNumberOfChildren(); ++i)
            {
                p_temp_pull.push_back(&p_node->getChild(i));
            }

            // Add its geometry
            gVirtualXRay::PolygonMesh& mesh = p_node->getPolygonMesh();
            if (mesh.getVertexNumber())
            {
                // Store the current transformation matrix
                gVirtualXRay::pushModelViewMatrix();

                gVirtualXRay::g_current_modelview_matrix *=
                p_node->getWorldTransformationMatrix();

                gVirtualXRay::applyModelViewMatrix();

                // Set the colour of the sample
                handle = glGetUniformLocation(shader_id, "material_ambient");
                glUniform4fv(handle, 1, mesh.getMaterial().getAmbientColour());
                gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                     __FUNCTION__,
                                                     __LINE__);

                handle = glGetUniformLocation(shader_id, "material_diffuse");
                glUniform4fv(handle, 1, mesh.getMaterial().getDiffuseColour());
                gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                     __FUNCTION__,
                                                     __LINE__);

                handle = glGetUniformLocation(shader_id, "material_specular");
                glUniform4fv(handle, 1, mesh.getMaterial().getSpecularColour());
                gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                     __FUNCTION__,
                                                     __LINE__);

                handle = glGetUniformLocation(shader_id, "material_shininess");
                GLfloat shininess = mesh.getMaterial().getShininess();
                glUniform1fv(handle, 1, &shininess);
                gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                     __FUNCTION__,
                                                     __LINE__);

                // Use transparency
                gVirtualXRay::pushEnableDisableState(GL_BLEND);
                GLint blend_src_alpha;
                GLint blend_dst_alpha;
                glGetIntegerv(GL_BLEND_SRC_ALPHA, &blend_src_alpha);
                glGetIntegerv(GL_BLEND_DST_ALPHA, &blend_dst_alpha);

                if (mesh.getMaterial().getAmbientColour()[3] < 0.99)
                {
                    glEnable(GL_BLEND);
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                }

                // Display the sample
                if (g_display_wireframe)
                {
                    mesh.displayWireFrame();
                }
                else
                {
                    mesh.display();
                }

                // Restore states
                gVirtualXRay::popEnableDisableState();

                if (mesh.getMaterial().getAmbientColour()[3] < 0.99)
                {
                    glBlendFunc(blend_src_alpha, blend_dst_alpha);
                }

                if (g_display_normal_vectors)
                {
                    GLfloat line_width;
                    glGetFloatv(GL_LINE_WIDTH, &line_width);
                    glLineWidth(3.0f);

                    if (mesh.getVertexNormalNumber())
                    {
                        mesh.displayVertexNormal();
                    }

                    if (mesh.getFaceNormalNumber())
                    {
                        mesh.displayFaceNormal();
                    }
                    glLineWidth(line_width);
                }

                // Restore the current transformation matrix
                gVirtualXRay::popModelViewMatrix();
            }
        }

        // Restore the current transformation matrix
        gVirtualXRay::popModelViewMatrix();

        // Set the colour of the source
        {
            constexpr GLfloat material_ambient[] = {0.0, 0.39225, 0.39225, 1.0};
            constexpr GLfloat material_diffuse[] = {0.0, 0.70754, 0.70754, 1.0};
            constexpr GLfloat material_specular[] = {0.0, 0.708273, 0.708273, 1.0};
            constexpr GLfloat material_shininess = 50.2;

            handle = glGetUniformLocation(shader_id, "material_ambient");
            glUniform4fv(handle, 1, &material_ambient[0]);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);

            handle = glGetUniformLocation(shader_id, "material_diffuse");
            glUniform4fv(handle, 1, &material_diffuse[0]);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);

            handle = glGetUniformLocation(shader_id, "material_specular");
            glUniform4fv(handle, 1, &material_specular[0]);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);

            handle = glGetUniformLocation(shader_id, "material_shininess");
            glUniform1fv(handle, 1, &material_shininess);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);
        }

        {
            // Define the light colours here
            GLfloat light_global_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
            GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
            GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
            GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

            handle = glGetUniformLocation(shader_id, "light_global_ambient");
            glUniform4fv(handle, 1, &light_global_ambient[0]);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);

            handle = glGetUniformLocation(shader_id, "light_ambient");
            glUniform4fv(handle, 1, &light_ambient[0]);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);

            handle = glGetUniformLocation(shader_id, "light_diffuse");
            glUniform4fv(handle, 1, &light_diffuse[0]);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);

            handle = glGetUniformLocation(shader_id, "light_specular");
            glUniform4fv(handle, 1, &light_specular[0]);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);

            gVirtualXRay::VEC3 light_position(0, 0, g_zoom);
            handle = glGetUniformLocation(shader_id, "light_position");
            glUniform4f(handle, light_position.getX(), light_position.getY(),
            light_position.getZ(), 1.0);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);
        }

        // Display the source
        g_xray_detector.displaySource();

        // Draw the pixel of the image
        {
            constexpr GLfloat material_ambient[] = {0.0, 0.0, 1.0, 1.0};
            constexpr GLfloat material_diffuse[] = {0.0, 0.0, 1.0, 1.0};
            constexpr GLfloat material_specular[] = {0.0, 0.0, 1.0, 1.0};
            constexpr GLfloat material_shininess = 50.2;

            handle = glGetUniformLocation(shader_id, "material_ambient");
            glUniform4fv(handle, 1, &material_ambient[0]);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);

            handle = glGetUniformLocation(shader_id, "material_diffuse");
            glUniform4fv(handle, 1, &material_diffuse[0]);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);

            handle = glGetUniformLocation(shader_id, "material_specular");
            glUniform4fv(handle, 1, &material_specular[0]);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);

            handle = glGetUniformLocation(shader_id, "material_shininess");
            glUniform1fv(handle, 1, &material_shininess);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);

            g_xray_detector.displayFirstPixelPosition();
        }

        // Disable back face culling
        glDisable(GL_CULL_FACE);

        // Display the X-Ray image
        if (g_display_detector)
        {
            g_xray_renderer.display(true, true, false, g_shift_filter,
            g_scale_filter);
        }

        // Display the beam
        if (g_display_beam)
        {
            constexpr GLfloat material_ambient[] = { 0.75, 0, 0.5, 0.3 };
            handle = glGetUniformLocation(shader_id, "material_ambient");
            glUniform4fv(handle, 1, &material_ambient[0]);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__,
            __LINE__);

            lighting = 0;
            handle = glGetUniformLocation(shader_id, "g_use_lighting");
            glUniform1iv(handle, 1, &lighting);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__,
            __LINE__);

            g_xray_detector.displayBeam();
        }

        // Draw the pixel of the image
        {
            constexpr GLfloat material_ambient[] = {0.0, 0.0, 1.0, 1.0};
            constexpr GLfloat material_diffuse[] = {0.0, 0.0, 1.0, 1.0};
            constexpr GLfloat material_specular[] = {0.0, 0.0, 1.0, 1.0};
            constexpr GLfloat material_shininess = 50.2;

            handle = glGetUniformLocation(shader_id, "material_ambient");
            glUniform4fv(handle, 1, &material_ambient[0]);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);

            handle = glGetUniformLocation(shader_id, "material_diffuse");
            glUniform4fv(handle, 1, &material_diffuse[0]);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);

            handle = glGetUniformLocation(shader_id, "material_specular");
            glUniform4fv(handle, 1, &material_specular[0]);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);

            handle = glGetUniformLocation(shader_id, "material_shininess");
            glUniform1fv(handle, 1, &material_shininess);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);

            g_xray_detector.displayFirstPixelPosition();
        }

        // Draw the detector position
        {
            constexpr GLfloat material_ambient[] = {0.0, 0.0, 1.0, 1.0};
            constexpr GLfloat material_diffuse[] = {0.0, 0.0, 1.0, 1.0};
            constexpr GLfloat material_specular[] = {0.0, 0.0, 1.0, 1.0};
            constexpr GLfloat material_shininess = 50.2;

            handle = glGetUniformLocation(shader_id, "material_ambient");
            glUniform4fv(handle, 1, &material_ambient[0]);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);

            handle = glGetUniformLocation(shader_id, "material_diffuse");
            glUniform4fv(handle, 1, &material_diffuse[0]);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);

            handle = glGetUniformLocation(shader_id, "material_specular");
            glUniform4fv(handle, 1, &material_specular[0]);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);

            handle = glGetUniformLocation(shader_id, "material_shininess");
            glUniform1fv(handle, 1, &material_shininess);
            gVirtualXRay::checkOpenGLErrorStatus(__FILE__,
                                                 __FUNCTION__,
                                                 __LINE__);

            g_xray_detector.displayDetectorPosition();
        }

        lighting = 0;
        handle = glGetUniformLocation(shader_id, "g_use_lighting");
        glUniform1iv(handle, 1, &lighting);
        gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
        g_world_coordinate_system_mesh.display(shader_id);
        
        // Disable the shader
        gVirtualXRay::popShaderProgram();

        // Restore the current transformation matrix
        gVirtualXRay::popModelViewMatrix();

        // Restore the attributes
        gVirtualXRay::popEnableDisableState();

        // Check the status of OpenGL and of the current FBO
        gVirtualXRay::checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);
        gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
    }
    // Catch exception if any
    catch (const std::exception& error)
    {
        std::cerr << error.what() << std::endl;
    }
}
