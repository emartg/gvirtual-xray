/*

Copyright (c) 2024-present, Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), 
UK Research and Innovation, All rights reserved.

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
*   @file       VulkanWindow.cxx
*
*   @brief      VulkanWindow is a class to handle windows. It uses
*               GLFW to support any Vulkan version.
*
*   @version    1.0
*
*   @date       16/12/2024
*
*   @author     Prof Franck P. Vidal
*
*   @section    License
*               BSD 3-Clause License.
*
*               For details on use and redistribution please refer
*               to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, Nov 2024-present, version 1.1, BSD 3-Clause License
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

#ifdef HAS_VULKAN
#define GLFW_INCLUDE_VULKAN 1
#include <GLFW/glfw3.h>
#endif

// // To perform the computations of the X-ray simulation
// #ifndef GVXR_XRAY_RENDERER_H
// #include "gVirtualXRay/XRayRenderer.h"
// #endif

// // To initialise GLEW, to define matrix stacks, to define state stacks, etc.
// #ifndef GVXR_VulkanUtilities_h
// #include "gVirtualXRay/VulkanUtilities.h"
// #endif

// #ifndef GVXR_UTILITIES_H
// #include "gVirtualXRay/Utilities.h"
// #endif

// #ifndef GVXR_SHADER_H
// #include "gVirtualXRay/Shader.h"
// #endif

// // To load a scenegraph
// #ifndef GVXR_SCENEGRAPH_BINDER_H
// #include "gVirtualXRay/SceneGraphBinder.h"
// #endif

// // To load a scenegraph using ASSIMP
// #ifndef GVXR_ASSIMP_SCENEGRAPH_BINDER_H
// #include "gVirtualXRay/AssimpSceneGraphBinder.h"
// #endif

// // To load a scenegraph using OpenSceneGraph
// #ifndef GVXR_OpenSceneGraphBinder_h
// #include "gVirtualXRay/OpenSceneGraphBinder.h"
// #endif

// // To perform the computations of the X-ray simulation
// #ifndef GVXR_XRAY_RENDERER_H
// #include "gVirtualXRay/XRayRenderer.h"
// #endif

#ifndef GVXR_LOGGER_H
#include <gVirtualXRay/Logger.h>
#endif

// #ifndef GVXR_COORDINATE_SYSTEM_MESH_H
// #include "gVirtualXRay/CoordinateSystemMesh.h"
// #endif

#ifndef SGVXR_VulkanWindow_h
#include "VulkanWindow.h"
#endif


// Shaders for rendering
// #include "display_gl2.frag.h"
// #include "display_gl2.vert.h"

// #include "display_gl3.frag.h"
// #include "display_gl3.vert.h"

// #include "display_gl4.frag.h"
// #include "display_gl4.vert.h"


//******************************************************************************
//  Namespaces
//******************************************************************************


//******************************************************************************
//  Function declarations
//******************************************************************************
#ifdef HAS_GLFW
void framebufferSizeVulkanCallback(GLFWwindow* apWindow, int aWidth, int aHeight);
#endif


//******************************************************************************
//  Global variables
//******************************************************************************
// extern double g_initial_fovy;
// extern double g_initial_near;
// extern double g_initial_far;
// extern GLfloat g_zoom;

// extern bool g_run_loop;
// extern bool g_display_beam;
// extern bool g_display_wireframe;
// extern bool g_use_lighing;
// extern bool g_display_detector;
// extern bool g_display_normal_vectors;

// extern gVirtualXRay::XRayBeam g_xray_beam;         // X-ray beam
// extern gVirtualXRay::XRayDetector g_xray_detector; // X-ray detector
// extern gVirtualXRay::XRayRenderer g_xray_renderer; // X-ray graphics engine


// extern gVirtualXRay::Shader g_display_shader;
// extern bool g_has_own_gl_context;

// extern std::unique_ptr<gVirtualXRay::SceneGraphBinder> g_p_scene_graph;

// extern gVirtualXRay::Matrix4x4<GLfloat> g_scene_rotation_matrix;
// extern gVirtualXRay::Matrix4x4<GLfloat> g_scene_translation_matrix;

// // Rotate the scanned objects
// extern gVirtualXRay::Matrix4x4<GLfloat> g_sample_rotation_matrix;

// // Bias/gain of the shift/scale filter for the display of the X-ray image
// extern double g_shift_filter;
// extern double g_scale_filter;

// extern std::vector<std::vector<std::vector< float> > > lastest_screenshot;

// extern gVirtualXRay::CoordinateSystemMesh g_world_coordinate_system_mesh;



//******************************************************************************
//  Function definitions
//******************************************************************************


//---------------------------------------------------------------
SimpleGVXR::VulkanWindow::VulkanWindow(int aVisibilityFlag,
                                       int anVulkanMajorVersion,
                                       int anVulkanMinorVersion):
//---------------------------------------------------------------
    Window(aVisibilityFlag)
//---------------------------------------------------------------
{
#ifdef HAS_VULKAN
    // Create the window
    m_context.create("Vulkan", anVulkanMajorVersion, anVulkanMinorVersion);

    // Set the window size
    if (m_context.getWindowHandle())
    {
        glfwSetWindowSize(m_context.getWindowHandle(), m_width, m_height);

    //     framebufferSizeVulkanCallback(m_context.getWindowHandle(), m_width, m_height);

    //     // Initialize GLFW callbacks
    //     registerCallbacks();

    //     // Initialise Vulkan
    //     initShader();

        uint32_t extension_count = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

        gVirtualXRay::LOGGER.logNow("Vulkan extension count: ") << extension_count << std::endl;
    }
#endif

    // Window has not been created
    if (!m_context.isReady())
    {
        // Display a warning
        std::cerr << "gvxrWarning:\tVulkanWindow " <<
            " cannot be created." <<
            std::endl;
    }
}


//-------------------------------------------------------------------
void SimpleGVXR::VulkanWindow::setWindowSize(int aWidth, int aHeight)
//-------------------------------------------------------------------
{
    // Window::setWindowSize(aWidth, aHeight);

    // if (m_context.getWindowHandle())
    // 	framebufferSizeVulkanCallback(m_context.getWindowHandle(), aWidth, aHeight);
}


//------------------------------------------------------------------
void SimpleGVXR::VulkanWindow::displayScene(bool aSceneRotationFlag)
//------------------------------------------------------------------
{
    // // Initialise the viewer if needed
    // initViewer();

    // // Show the window if needed
    // if (!g_has_own_gl_context && m_context.getWindowHandle())
    // {
    //     if (!glfwGetWindowAttrib(m_context.getWindowHandle(),
    //         GLFW_VISIBLE))
    //     {
    //         if (m_visible)
    //         {
    //             glfwShowWindow(m_context.getWindowHandle());
    //         }
    //         else
    //         {
    //             gVirtualXRay::LOGGER.logWarning("Rendering in an hidden window may work, it may not.") << std::endl;
    //         }
    //     }

    //     // Make the window's context current
    //     glfwMakeContextCurrent(m_context.getWindowHandle());

    //     // Clear the buffers
    //     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //     initViewer();
    // }

    // // Display the 3D scene
    // render(aSceneRotationFlag);

    // // Make sure all the Vulkan code is done
    // glFinish();

    // // Swap front and back buffers
    // if (!g_has_own_gl_context && m_context.getWindowHandle())
    // {
    //     glfwSwapBuffers(m_context.getWindowHandle());
    // }
}


//---------------------------------------------
void SimpleGVXR::VulkanWindow::takeScreenshot()
//---------------------------------------------
{
    // // Update the window size, just in case
    // glfwGetWindowSize(m_context.getWindowHandle(), &m_width, &m_height);

    // GLfloat* p_pixel_set = new GLfloat[3 * m_width * m_height];
    // glReadPixels(0, 0, m_width, m_height, GL_RGB, GL_FLOAT, p_pixel_set);

    // lastest_screenshot.clear();

    // for (unsigned int j = 0; j < m_height; ++j)
    // {
    //     lastest_screenshot.push_back(std::vector<std::vector< float> >() );

    //     for (unsigned int i = 0; i < m_width; ++i)
    //     {
    //         lastest_screenshot.back().push_back(std::vector< float>());

    //         lastest_screenshot.back().back().push_back(p_pixel_set[(m_height - 1 - j) * m_width * 3 + i * 3 + 0]);
    //         lastest_screenshot.back().back().push_back(p_pixel_set[(m_height - 1 - j) * m_width * 3 + i * 3 + 1]);
    //         lastest_screenshot.back().back().push_back(p_pixel_set[(m_height - 1 - j) * m_width * 3 + i * 3 + 2]);
    //     }
    // }

    // delete [] p_pixel_set;
}


//-----------------------------------------
void SimpleGVXR::VulkanWindow::renderLoop()
//-----------------------------------------
{
#ifdef HAS_GLFW
    if (m_context.getWindowHandle())
    {
        if (!glfwGetWindowAttrib(m_context.getWindowHandle(),
            GLFW_VISIBLE))
        {
            glfwShowWindow(m_context.getWindowHandle());
            m_visible = true;
        }

        SimpleGVXR::Window::renderLoop();
    }
#endif
}


//---------------------------------------------------------------------------------
void SimpleGVXR::VulkanWindow::setWindowBackGroundColour(float R, float G, float B)
//---------------------------------------------------------------------------------
{
    // Window::setWindowBackGroundColour(R, G, B);
    // glClearColor(m_background_colour[0], m_background_colour[1], m_background_colour[2], 1.0);
}


//--------------------------------------------------------------------------------
void SimpleGVXR::VulkanWindow::setWindowBackGroundColor(float R, float G, float B)
//--------------------------------------------------------------------------------
{
    // setWindowBackGroundColour(R, G, B);
}


//---------------------------------------------------
void SimpleGVXR::VulkanWindow::createWindow(bool aVisibilityFlag)
//---------------------------------------------------
{
	// Do nothing
}


//-----------------------------------------
void SimpleGVXR::VulkanWindow::initShader()
//-----------------------------------------
{
    // gVirtualXRay::LOGGER.logNow("Vulkan vendor: ") << glGetString(GL_VENDOR) << std::endl;
    // gVirtualXRay::LOGGER.logNow("Vulkan renderer: ") << glGetString(GL_RENDERER) << std::endl;
    // gVirtualXRay::LOGGER.logNow("Vulkan version: ") << glGetString(GL_VERSION) << std::endl;

    // gVirtualXRay::checkVulkanErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // // Enable the Z-buffer
    // glEnable (GL_DEPTH_TEST);
    // gVirtualXRay::checkVulkanErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // // Set the background colour
    // setWindowBackGroundColour(m_background_colour[0], m_background_colour[1], m_background_colour[2]);
    // gVirtualXRay::checkVulkanErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // glEnable (GL_MULTISAMPLE);
    // gVirtualXRay::checkVulkanErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // // Initialise the shaders
    // char* p_vertex_shader(0);
    // char* p_fragment_shader(0);

    // int z_lib_return_code_vertex(0);
    // int z_lib_return_code_fragment(0);

    // std::string vertex_shader;
    // std::string fragment_shader;

    // // Display shader
    // // Vulkan 4.5
    // if (gVirtualXRay::useVulkan45())
    // {
    //     z_lib_return_code_vertex = gVirtualXRay::inflate(
    //         g_display_gl4_vert,
    //         sizeof(g_display_gl4_vert),
    //         &p_vertex_shader);

    //     z_lib_return_code_fragment = gVirtualXRay::inflate(
    //         g_display_gl4_frag,
    //         sizeof(g_display_gl4_frag),
    //         &p_fragment_shader);

    //     g_display_shader.setLabels("display_gl4.vert", "display_gl4.frag");
    // }
    // // Vulkan 3.2
    // else if (gVirtualXRay::useVulkan32())
    // {
    //     z_lib_return_code_vertex = gVirtualXRay::inflate(
    //         g_display_gl3_vert,
    //         sizeof(g_display_gl3_vert),
    //         &p_vertex_shader);

    //     z_lib_return_code_fragment = gVirtualXRay::inflate(
    //         g_display_gl3_frag,
    //         sizeof(g_display_gl3_frag),
    //         &p_fragment_shader);

    //     g_display_shader.setLabels("display_gl3.vert", "display_gl3.frag");
    // }
    // else
    // {
    //     z_lib_return_code_vertex = gVirtualXRay::inflate(
    //         g_display_gl2_vert,
    //         sizeof(g_display_gl2_vert),
    //         &p_vertex_shader);

    //     z_lib_return_code_fragment = gVirtualXRay::inflate(
    //         g_display_gl2_frag,
    //         sizeof(g_display_gl2_frag),
    //         &p_fragment_shader);

    //     g_display_shader.setLabels("display_gl2.vert", "display_gl2.frag");
    //     // throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot create shader programs for your chosen Vulkan context.");
    // }

    // vertex_shader = p_vertex_shader;
    // fragment_shader = p_fragment_shader;

    // delete[] p_vertex_shader;
    // delete[] p_fragment_shader;

    // p_vertex_shader = 0;
    // p_fragment_shader = 0;

    // if (z_lib_return_code_vertex <= 0 ||
    //     z_lib_return_code_fragment <= 0 ||
    //     !vertex_shader.size() ||
    //     !fragment_shader.size())
    // {
    //     throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
    //         "Cannot decode the shader using ZLib.");
    // }

    // g_display_shader.loadSource(vertex_shader, fragment_shader);
    // gVirtualXRay::checkVulkanErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // initViewer();
}


//------------------------------------------------------------
void SimpleGVXR::VulkanWindow::render(bool aSceneRotationFlag)
//------------------------------------------------------------
{
    // try
    // {
    //     // Enable back face culling
    //     gVirtualXRay::pushEnableDisableState (GL_CULL_FACE);
    //     glEnable(GL_CULL_FACE);
    //     glCullFace (GL_BACK);

    //     // Enable the shader
    //     gVirtualXRay::pushShaderProgram();
    //     g_display_shader.enable();
    //     GLint shader_id(g_display_shader.getProgramHandle());

    //     // Check the status of Vulkan and of the current FBO
    //     gVirtualXRay::checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);
    //     gVirtualXRay::checkVulkanErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    //     // Store the current transformation matrix
    //     gVirtualXRay::pushModelViewMatrix();

    //     // Rotate the sample
    //     if (aSceneRotationFlag)
    //     {
    //         gVirtualXRay::g_current_modelview_matrix *= g_scene_rotation_matrix;
    //         gVirtualXRay::g_current_modelview_matrix *= g_scene_translation_matrix;
    //     }

    //     GLuint handle(0);
    //     handle = glGetUniformLocation(shader_id, "g_projection_matrix");
    //     glUniformMatrix4fv(handle, 1, GL_FALSE,
    //     gVirtualXRay::g_current_projection_matrix.get());
    //     gVirtualXRay::checkVulkanErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    //     handle = glGetUniformLocation(shader_id, "g_modelview_matrix");
    //     glUniformMatrix4fv(handle, 1, GL_FALSE,
    //     gVirtualXRay::g_current_modelview_matrix.get());
    //     gVirtualXRay::checkVulkanErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    //     gVirtualXRay::MATRIX4 normal_matrix(
    //     gVirtualXRay::g_current_modelview_matrix);
    //     handle = glGetUniformLocation(shader_id, "g_normal_matrix");
    //     glUniformMatrix3fv(handle, 1, GL_FALSE, normal_matrix.get3x3());
    //     gVirtualXRay::checkVulkanErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    //     GLint lighting;
    //     if (g_use_lighing)
    //     {
    //         lighting = 1;
    //     }
    //     else
    //     {
    //         lighting = 0;
    //     }
    //     handle = glGetUniformLocation(shader_id, "g_use_lighting");
    //     glUniform1iv(handle, 1, &lighting);
    //     gVirtualXRay::checkVulkanErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    //     // Store the current transformation matrix
    //     gVirtualXRay::pushModelViewMatrix();

    //     gVirtualXRay::g_current_modelview_matrix *= g_sample_rotation_matrix;

    //     // Look for all the meshes in the scenegraph
    //     std::vector<gVirtualXRay::SceneGraphNode*> p_temp_pull;
    //     p_temp_pull.push_back(g_p_scene_graph->getNode("root"));

    //     while (p_temp_pull.size())
    //     {
    //         // Get the last node
    //         gVirtualXRay::SceneGraphNode* p_node = p_temp_pull.back();

    //         // Remove the node
    //         p_temp_pull.pop_back();

    //         // Add its children
    //         for (unsigned int i = 0; i < p_node->getNumberOfChildren(); ++i)
    //         {
    //             p_temp_pull.push_back(&p_node->getChild(i));
    //         }

    //         // Add its geometry
    //         gVirtualXRay::PolygonMesh& mesh = p_node->getPolygonMesh();
    //         if (mesh.getVertexNumber())
    //         {
    //             // Store the current transformation matrix
    //             gVirtualXRay::pushModelViewMatrix();

    //             gVirtualXRay::g_current_modelview_matrix *=
    //             p_node->getWorldTransformationMatrix();

    //             gVirtualXRay::applyModelViewMatrix();

    //             // Set the colour of the sample
    //             handle = glGetUniformLocation(shader_id, "material_ambient");
    //             glUniform4fv(handle, 1, mesh.getMaterial().getAmbientColour());
    //             gVirtualXRay::checkVulkanErrorStatus(__FILE__,
    //                                                  __FUNCTION__,
    //                                                  __LINE__);

    //             handle = glGetUniformLocation(shader_id, "material_diffuse");
    //             glUniform4fv(handle, 1, mesh.getMaterial().getDiffuseColour());
    //             gVirtualXRay::checkVulkanErrorStatus(__FILE__,
    //                                                  __FUNCTION__,
    //                                                  __LINE__);

    //             handle = glGetUniformLocation(shader_id, "material_specular");
    //             glUniform4fv(handle, 1, mesh.getMaterial().getSpecularColour());
    //             gVirtualXRay::checkVulkanErrorStatus(__FILE__,
    //                                                  __FUNCTION__,
    //                                                  __LINE__);

    //             handle = glGetUniformLocation(shader_id, "material_shininess");
    //             GLfloat shininess = mesh.getMaterial().getShininess();
    //             glUniform1fv(handle, 1, &shininess);
    //             gVirtualXRay::checkVulkanErrorStatus(__FILE__,
    //                                                  __FUNCTION__,
    //                                                  __LINE__);

    //             // Use transparency
    //             gVirtualXRay::pushEnableDisableState(GL_BLEND);
    //             GLint blend_src_alpha;
    //             GLint blend_dst_alpha;
    //             glGetIntegerv(GL_BLEND_SRC_ALPHA, &blend_src_alpha);
    //             glGetIntegerv(GL_BLEND_DST_ALPHA, &blend_dst_alpha);

    //             if (mesh.getMaterial().getAmbientColour()[3] < 0.99)
    //             {
    //                 glEnable(GL_BLEND);
    //                 glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //             }

    //             // Display the sample
    //             if (g_display_wireframe)
    //             {
    //                 mesh.displayWireFrame();
    //             }
    //             else
    //             {
    //                 mesh.display();
    //             }

    //             // Restore states
    //             gVirtualXRay::popEnableDisableState();

    //             if (mesh.getMaterial().getAmbientColour()[3] < 0.99)
    //             {
    //                 glBlendFunc(blend_src_alpha, blend_dst_alpha);
    //             }

    //             if (g_display_normal_vectors)
    //             {
    //                 int line_width;
    //                 glGetIntegerv(GL_LINE_WIDTH, &line_width);
    //                 glLineWidth(3);

    //                 if (mesh.getVertexNormalNumber())
    //                 {
    //                     mesh.displayVertexNormal();
    //                 }

    //                 if (mesh.getFaceNormalNumber())
    //                 {
    //                     mesh.displayFaceNormal();
    //                 }
    //                 glLineWidth(line_width);
    //             }

    //             // Restore the current transformation matrix
    //             gVirtualXRay::popModelViewMatrix();
    //         }
    //     }

    //     // Restore the current transformation matrix
    //     gVirtualXRay::popModelViewMatrix();

    //     // Set the colour of the source
    //     {
    //         const GLfloat material_ambient[] = {0.0, 0.39225, 0.39225, 1.0};
    //         const GLfloat material_diffuse[] = {0.0, 0.70754, 0.70754, 1.0};
    //         const GLfloat material_specular[] = {0.0, 0.708273, 0.708273, 1.0};
    //         const GLfloat material_shininess = 50.2;

    //         handle = glGetUniformLocation(shader_id, "material_ambient");
    //         glUniform4fv(handle, 1, &material_ambient[0]);
    //         gVirtualXRay::checkVulkanErrorStatus(__FILE__,
    //                                              __FUNCTION__,
    //                                              __LINE__);

    //         handle = glGetUniformLocation(shader_id, "material_diffuse");
    //         glUniform4fv(handle, 1, &material_diffuse[0]);
    //         gVirtualXRay::checkVulkanErrorStatus(__FILE__,
    //                                              __FUNCTION__,
    //                                              __LINE__);

    //         handle = glGetUniformLocation(shader_id, "material_specular");
    //         glUniform4fv(handle, 1, &material_specular[0]);
    //         gVirtualXRay::checkVulkanErrorStatus(__FILE__,
    //                                              __FUNCTION__,
    //                                              __LINE__);

    //         handle = glGetUniformLocation(shader_id, "material_shininess");
    //         glUniform1fv(handle, 1, &material_shininess);
    //         gVirtualXRay::checkVulkanErrorStatus(__FILE__,
    //                                              __FUNCTION__,
    //                                              __LINE__);
    //     }

    //     {
    //         // Define the light colours here
    //         GLfloat light_global_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    //         GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
    //         GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    //         GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

    //         handle = glGetUniformLocation(shader_id, "light_global_ambient");
    //         glUniform4fv(handle, 1, &light_global_ambient[0]);
    //         gVirtualXRay::checkVulkanErrorStatus(__FILE__,
    //                                              __FUNCTION__,
    //                                              __LINE__);

    //         handle = glGetUniformLocation(shader_id, "light_ambient");
    //         glUniform4fv(handle, 1, &light_ambient[0]);
    //         gVirtualXRay::checkVulkanErrorStatus(__FILE__,
    //                                              __FUNCTION__,
    //                                              __LINE__);

    //         handle = glGetUniformLocation(shader_id, "light_diffuse");
    //         glUniform4fv(handle, 1, &light_diffuse[0]);
    //         gVirtualXRay::checkVulkanErrorStatus(__FILE__,
    //                                              __FUNCTION__,
    //                                              __LINE__);

    //         handle = glGetUniformLocation(shader_id, "light_specular");
    //         glUniform4fv(handle, 1, &light_specular[0]);
    //         gVirtualXRay::checkVulkanErrorStatus(__FILE__,
    //                                              __FUNCTION__,
    //                                              __LINE__);

    //         gVirtualXRay::VEC3 light_position(0, 0, g_zoom);
    //         handle = glGetUniformLocation(shader_id, "light_position");
    //         glUniform4f(handle, light_position.getX(), light_position.getY(),
    //         light_position.getZ(), 1.0);
    //         gVirtualXRay::checkVulkanErrorStatus(__FILE__,
    //                                              __FUNCTION__,
    //                                              __LINE__);
    //     }

    //     // Display the source
    //     g_xray_detector.displaySource();

    //     // Disable back face culling
    //     glDisable(GL_CULL_FACE);

    //     // Display the X-Ray image
    //     if (g_display_detector)
    //     {
    //         g_xray_renderer.display(true, true, false, g_shift_filter,
    //         g_scale_filter);
    //     }

    //     // Display the beam
    //     if (g_display_beam)
    //     {
    //         const GLfloat material_ambient[] = { 0.75, 0, 0.5, 0.3 };
    //         handle = glGetUniformLocation(shader_id, "material_ambient");
    //         glUniform4fv(handle, 1, &material_ambient[0]);
    //         gVirtualXRay::checkVulkanErrorStatus(__FILE__, __FUNCTION__,
    //         __LINE__);

    //         lighting = 0;
    //         handle = glGetUniformLocation(shader_id, "g_use_lighting");
    //         glUniform1iv(handle, 1, &lighting);
    //         gVirtualXRay::checkVulkanErrorStatus(__FILE__, __FUNCTION__,
    //         __LINE__);

    //         g_xray_detector.displayBeam();
    //     }

    //     // Draw the first pixel of the image
    //     {
    //         const GLfloat material_ambient[] = {0.0, 0.0, 1.0, 1.0};
    //         const GLfloat material_diffuse[] = {0.0, 0.0, 1.0, 1.0};
    //         const GLfloat material_specular[] = {0.0, 0.0, 1.0, 1.0};
    //         const GLfloat material_shininess = 50.2;

    //         handle = glGetUniformLocation(shader_id, "material_ambient");
    //         glUniform4fv(handle, 1, &material_ambient[0]);
    //         gVirtualXRay::checkVulkanErrorStatus(__FILE__,
    //                                              __FUNCTION__,
    //                                              __LINE__);

    //         handle = glGetUniformLocation(shader_id, "material_diffuse");
    //         glUniform4fv(handle, 1, &material_diffuse[0]);
    //         gVirtualXRay::checkVulkanErrorStatus(__FILE__,
    //                                              __FUNCTION__,
    //                                              __LINE__);

    //         handle = glGetUniformLocation(shader_id, "material_specular");
    //         glUniform4fv(handle, 1, &material_specular[0]);
    //         gVirtualXRay::checkVulkanErrorStatus(__FILE__,
    //                                              __FUNCTION__,
    //                                              __LINE__);

    //         handle = glGetUniformLocation(shader_id, "material_shininess");
    //         glUniform1fv(handle, 1, &material_shininess);
    //         gVirtualXRay::checkVulkanErrorStatus(__FILE__,
    //                                              __FUNCTION__,
    //                                              __LINE__);

    //         g_xray_detector.displayFirstPixelPosition();
    //     }

    //     // Draw the detector position
    //     {
    //         const GLfloat material_ambient[] = {0.0, 0.0, 1.0, 1.0};
    //         const GLfloat material_diffuse[] = {0.0, 0.0, 1.0, 1.0};
    //         const GLfloat material_specular[] = {0.0, 0.0, 1.0, 1.0};
    //         const GLfloat material_shininess = 50.2;

    //         handle = glGetUniformLocation(shader_id, "material_ambient");
    //         glUniform4fv(handle, 1, &material_ambient[0]);
    //         gVirtualXRay::checkVulkanErrorStatus(__FILE__,
    //                                              __FUNCTION__,
    //                                              __LINE__);

    //         handle = glGetUniformLocation(shader_id, "material_diffuse");
    //         glUniform4fv(handle, 1, &material_diffuse[0]);
    //         gVirtualXRay::checkVulkanErrorStatus(__FILE__,
    //                                              __FUNCTION__,
    //                                              __LINE__);

    //         handle = glGetUniformLocation(shader_id, "material_specular");
    //         glUniform4fv(handle, 1, &material_specular[0]);
    //         gVirtualXRay::checkVulkanErrorStatus(__FILE__,
    //                                              __FUNCTION__,
    //                                              __LINE__);

    //         handle = glGetUniformLocation(shader_id, "material_shininess");
    //         glUniform1fv(handle, 1, &material_shininess);
    //         gVirtualXRay::checkVulkanErrorStatus(__FILE__,
    //                                              __FUNCTION__,
    //                                              __LINE__);

    //         g_xray_detector.displayDetectorPosition();
    //     }

    //     lighting = 0;
    //     handle = glGetUniformLocation(shader_id, "g_use_lighting");
    //     glUniform1iv(handle, 1, &lighting);
    //     gVirtualXRay::checkVulkanErrorStatus(__FILE__, __FUNCTION__, __LINE__);
    //     g_world_coordinate_system_mesh.display(shader_id);

    //     // Disable the shader
    //     gVirtualXRay::popShaderProgram();

    //     // Restore the current transformation matrix
    //     gVirtualXRay::popModelViewMatrix();

    //     // Restore the attributes
    //     gVirtualXRay::popEnableDisableState();

    //     // Check the status of Vulkan and of the current FBO
    //     gVirtualXRay::checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);
    //     gVirtualXRay::checkVulkanErrorStatus(__FILE__, __FUNCTION__, __LINE__);
    // }
    // // Catch exception if any
    // catch (const std::exception& error)
    // {
    //     std::cerr << error.what() << std::endl;
    // }
}


//------------------------------------------------
void SimpleGVXR::VulkanWindow::registerCallbacks()
//------------------------------------------------
{
	// if (m_context.getWindowHandle() && !m_callback_registered)
	// {
	// 	Window::registerCallbacks();

    //     // Initialize GLFW callbacks
    //     glfwSetFramebufferSizeCallback(m_context.getWindowHandle(), framebufferSizeVulkanCallback);
	// }
}


#ifdef HAS_GLFW
//-----------------------------------------------------------------------------
void framebufferSizeVulkanCallback(GLFWwindow* apWindow, int width, int height)
//-----------------------------------------------------------------------------
{
    // if (height == 0) {
    //   // Prevent divide by 0
    //   height = 1;
    // }

    // int x(0), y(0), w(width), h(height);
    // double screen_aspect_ratio(double(width) / double(height));
    // glViewport(x, y, w, h);

    // SimpleGVXR::Window* p_window = SimpleGVXR::Window::getWindow(apWindow);
    // if (p_window)
    // {
    // 	p_window->initViewer();
    // }

    // gVirtualXRay::loadPerspectiveProjectionMatrix(g_initial_fovy,
    // screen_aspect_ratio, g_initial_near, g_initial_far);

    // gVirtualXRay::loadLookAtModelViewMatrix(0.0, 0.0, g_zoom,
    //     0.0, 0.0, 0.0,
    //     0.0, 1.0, 0.0);
}
#endif
