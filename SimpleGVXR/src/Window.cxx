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
*   @file       Window.cxx
*
*   @brief      Window is a virtual class to handle windows. The plan is to use
*               GLFW to support any OpenGL version as well as Vulkan.
*
*   @version    1.0
*
*   @date       03/03/2020
*
*   @author     Dr Franck P. Vidal
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*   http://www.fpvidal.net/, Mar 2020, 2020, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <cmath>
#include <algorithm>

#ifdef HAS_GLFW
#define GLFW_INCLUDE_GLCOREARB 1
#include <GLFW/glfw3.h>
#endif

// To load a scenegraph
#ifndef GVXR_SCENEGRAPH_BINDER_H
#include "gVirtualXRay/SceneGraphBinder.h"
#endif

#ifndef GVXR_EXCEPTION_H
#include "gVirtualXRay/Exception.h"
#endif

// To perform the computations of the X-ray simulation
#ifndef GVXR_XRAY_RENDERER_H
#include "gVirtualXRay/XRayRenderer.h"
#endif

#ifndef GVXR_LOGGER_H
#include <gVirtualXRay/Logger.h>
#endif

#ifndef SGVXR_Window_h
#include "Window.h"
#endif


//******************************************************************************
//  Namespaces
//******************************************************************************


//******************************************************************************
//  Function declarations
//******************************************************************************
#ifdef HAS_GLFW
void keyCallback(GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods);

void mouseButtonCallback(GLFWwindow* apWindow,
    int aButton,
    int aButtonState,
    int aModifierKey);

void cursorPosCallback(GLFWwindow* apWindow, double x, double y);

void scrollCallback(GLFWwindow* apWindow, double xoffset, double yoffset);
#endif


//******************************************************************************
//  Static class members
//******************************************************************************
std::list<SimpleGVXR::Window*> SimpleGVXR::Window::m_p_window_set;


//******************************************************************************
//  Global variables
//******************************************************************************
extern double g_initial_fovy;
extern double g_initial_near;
extern double g_initial_far;
extern GLfloat g_zoom;
extern gVirtualXRay::Matrix4x4 < GLfloat > g_scene_rotation_matrix;
extern gVirtualXRay::Matrix4x4 < GLfloat > g_scene_translation_matrix;

extern bool g_run_loop;
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


//******************************************************************************
//  Function definitions
//******************************************************************************


//----------------------------------
SimpleGVXR::Window::Window(int aVisibilityFlag):
//----------------------------------
    m_title("gVirtualXRay -- Window"),
    m_visible(aVisibilityFlag),
    m_width(500),
    m_height(500),
    m_viewer_initialised(false),
	m_callback_registered(false),
    m_first_pass(false)
//----------------------------------
{
	// gVirtualXRay::LOGGER.logNow("Create window")  << std::endl;

    m_background_colour[0] = 0.5;
    m_background_colour[1] = 0.5;
    m_background_colour[2] = 0.5;

	m_p_window_set.push_back(this);
}


//---------------------------
SimpleGVXR::Window::~Window()
//---------------------------
{
	// Find the current window in the set of windows
	auto ite = std::find(m_p_window_set.begin(), m_p_window_set.end(), this);

	// If the window is found, remove it from the set
	if (ite != m_p_window_set.end())
	{
		m_p_window_set.erase(ite);
	}
}


//-------------------------------------------------------------
void SimpleGVXR::Window::setWindowSize(int aWidth, int aHeight)
//-------------------------------------------------------------
{
    // The size has changed
    if (m_width != aWidth || m_height != aHeight)
    {
        m_width  = aWidth;
        m_height = aHeight;

#ifdef HAS_GLFW
        if (m_context.getWindowHandle())
        {
        	glfwSetWindowSize(m_context.getWindowHandle(), aWidth, aHeight);
        }
#endif
    }
}


//-----------------------------------------------------------
std::pair<int, int> SimpleGVXR::Window::getWindowSize() const
//-----------------------------------------------------------
{
    return {m_width, m_height};
}


//-----------------------------------
void SimpleGVXR::Window::renderLoop()
//-----------------------------------
{
    g_run_loop = true;

#ifdef HAS_GLFW
    // Launch the event loop
    if (m_context.getWindowHandle())
    {
		while (!glfwWindowShouldClose(m_context.getWindowHandle()) &&
				g_run_loop)
        {
			// Process the event
			glfwPollEvents();

            // A close callback may have changed the loop state while polling.
            // Do not render with a context or shader that is being torn down.
            if (glfwWindowShouldClose(m_context.getWindowHandle()) || !g_run_loop)
            {
                break;
            }

			// Render here
			displayScene(true);
        }

        g_run_loop = false;
    }
#endif
}


//-----------------------------------
void SimpleGVXR::Window::showWindow()
//-----------------------------------
{
#ifdef HAS_GLFW
    // The window exists
    if (m_context.getWindowHandle())
    {
        // It is hidden
        if (!glfwGetWindowAttrib(m_context.getWindowHandle(),
            GLFW_VISIBLE))
        {
            glfwShowWindow(m_context.getWindowHandle());
            m_visible = true;
        }
    }
#endif
}


//------------------------------------------------------------
void SimpleGVXR::Window::setWindowTitle(const std::string& aTitle)
//------------------------------------------------------------
{
#ifdef HAS_GLFW
    if (m_context.getWindowHandle())
    {
        glfwSetWindowTitle(m_context.getWindowHandle(), aTitle.c_str());
    }
#else
    (void)aTitle;
#endif
}


//-----------------------------------
void SimpleGVXR::Window::hideWindow()
//-----------------------------------
{
#ifdef HAS_GLFW
    // The window exists
    if (m_context.getWindowHandle())
    {
        // It is shown
        if (glfwGetWindowAttrib(m_context.getWindowHandle(),
            GLFW_VISIBLE))
        {
            glfwHideWindow(m_context.getWindowHandle());
            m_visible = false;
        }
    }
#endif

    g_run_loop = false;
}


//------------------------------------------------------------------
void SimpleGVXR::Window::setPosition(int aPositionX, int aPositionY)
//------------------------------------------------------------------
{
#ifdef HAS_GLFW
    // The window exists
    if (m_context.getWindowHandle())
    {
        glfwSetWindowPos(m_context.getWindowHandle(), aPositionX, aPositionY);
    }
#endif
}


//---------------------------------------------------------------------------
void SimpleGVXR::Window::setWindowBackGroundColour(float R, float G, float B)
//---------------------------------------------------------------------------
{
    m_background_colour[0] = R;
    m_background_colour[1] = G;
    m_background_colour[2] = B;
}


//--------------------------------------------------------------------------
void SimpleGVXR::Window::setWindowBackGroundColor(float R, float G, float B)
//--------------------------------------------------------------------------
{
    setWindowBackGroundColour(R, G, B);
}


//--------------------------------------
void SimpleGVXR::Window::destroyWindow()
//--------------------------------------
{
    g_run_loop = false;
	m_context.release();
}


//-----------------------------------
void SimpleGVXR::Window::initViewer()
//-----------------------------------
{
    if (!m_viewer_initialised && !g_xray_detector.getSourcePositionSet().empty())
    {
        auto distance_source_object =
            static_cast<float>(g_xray_detector.getSourcePositionSet()[0].length());

        auto distance_detector_object =
            static_cast<float>(g_xray_detector.getDetectorPosition().length());

        const float distance = std::max(distance_detector_object,
                                  distance_source_object);

        const float half_fovy = (static_cast<float>(M_PI * g_initial_fovy) / 2.0f) / 180.0f;

        g_zoom = atan(half_fovy) * distance * 8;

        if (g_p_scene_graph)
        {
            g_initial_near = (g_p_scene_graph->getBoundingBox().second -
                g_p_scene_graph->getBoundingBox().first).length() / 5.0;

            m_viewer_initialised = true;
        }

        // Centre the rotation of the scence on the middle point between the source and the detector
        gVirtualXRay::VEC3 centre(g_xray_detector.getDetectorPosition() + g_xray_detector.getXraySourceCentre());
        centre /= 2.0;
        g_scene_translation_matrix = gVirtualXRay::Matrix4x4< GLfloat >::buildTranslationMatrix(-centre);

        // Check if the source_detector vector is parallel to the eye_lookat vector
        gVirtualXRay::VEC3 source_detector(g_xray_detector.getXraySourceCentre() - g_xray_detector.getDetectorPosition());
        source_detector.normalise();
        gVirtualXRay::VEC3 eye_lookat(0, 0, 1);

        // Get the angle between the two vectors in degrees
        float angle = static_cast<float>(gVirtualXRay::radianToDegree(acos(source_detector.dotProduct(eye_lookat))));

        // If   -10 < angle < 10,
        if (abs(angle) < 10 || abs(M_PI - angle)  < 10)
        {
        	// then rotate the scene so that we see it from a viewing direction
        	// Perpendicular to the source_detector vector
            g_scene_rotation_matrix *= gVirtualXRay::Matrix4x4<GLfloat>::buildRotationMatrix(90 - angle, g_xray_detector.getUpVector());

            // Slightly alter the rotation matrix so that it's a bit nicer
            g_scene_rotation_matrix *= gVirtualXRay::Matrix4x4<GLfloat>::buildRotationMatrix(-45, g_xray_detector.getUpVector());
            //g_scene_rotation_matrix *= gVirtualXRay::Matrix4x4<GLfloat>::buildRotationMatrix(0, source_detector);
            //g_scene_rotation_matrix *= gVirtualXRay::Matrix4x4<GLfloat>::buildRotationMatrix(0, 0, 0, 1);
        }

        g_initial_far = g_zoom * 100.0;

        gVirtualXRay::loadLookAtModelViewMatrix(0.0, 0.0, g_zoom,
                                                0.0, 0.0, 0.0,
                                                0.0, 1.0, 0.0);
    }
}


#ifdef HAS_GLFW
SimpleGVXR::Window* SimpleGVXR::Window::getWindow(GLFWwindow* apGLFWwindow)
{
	for (auto& ite: m_p_window_set)
	{
		if (ite->m_context.getWindowHandle() == apGLFWwindow)
			return ite;
	}

	return nullptr;
}
#endif


//------------------------------------------------------------
void SimpleGVXR::Window::displayScene(bool aSceneRotationFlag)
//------------------------------------------------------------
{}


//---------------------------------------
void SimpleGVXR::Window::takeScreenshot()
//---------------------------------------
{}


//-----------------------------------------------------
gVirtualXRay::Context& SimpleGVXR::Window::getContext()
//-----------------------------------------------------
{
    return m_context;
}


#ifdef HAS_GLFW
//--------------------------------------------
void window_close_callback(GLFWwindow* window)
//--------------------------------------------
{
    g_run_loop = false;
    glfwSetWindowShouldClose(window, GLFW_TRUE);

    SimpleGVXR::Window* p_window = SimpleGVXR::Window::getWindow(window);
    if (p_window)
    {
        p_window->hideWindow();
    }
}


//----------------------------------
void keyCallback(GLFWwindow* window,
                 int key,
                 int scancode,
                 int action,
                 int mods)
//----------------------------------
{
    if (action == GLFW_PRESS)
    {
		switch (key)
		{
		case GLFW_KEY_Q:
		case GLFW_KEY_ESCAPE:
			g_run_loop = false;
			break;

		case GLFW_KEY_B:
			g_display_beam = !g_display_beam;
			break;

		case GLFW_KEY_W:
			g_display_wireframe = !g_display_wireframe;
			break;

		case GLFW_KEY_N:
			g_xray_renderer.useNegativeFilteringFlag(
					!g_xray_renderer.getNegativeFilteringFlag());
			break;

		case GLFW_KEY_L:
			g_use_lighing = !g_use_lighing;
			break;

		case GLFW_KEY_D:
			g_display_detector = !g_display_detector;
			break;

		case GLFW_KEY_V:
			g_display_normal_vectors = !g_display_normal_vectors;
			break;

		case GLFW_KEY_T:
		    SimpleGVXR::Window* p_window = SimpleGVXR::Window::getWindow(window);
		    p_window->takeScreenshot();
		    break;
		}
    }
}
#endif


//------------------------------------------
void SimpleGVXR::Window::registerCallbacks()
//------------------------------------------
{
#ifdef HAS_GLFW
	if (m_context.getWindowHandle() && !m_callback_registered)
	{
		m_callback_registered = true;

        glfwSetKeyCallback(m_context.getWindowHandle(), keyCallback);
        glfwSetMouseButtonCallback(m_context.getWindowHandle(), mouseButtonCallback);
        glfwSetCursorPosCallback(m_context.getWindowHandle(), cursorPosCallback);
        glfwSetScrollCallback(m_context.getWindowHandle(), scrollCallback);
        glfwSetWindowCloseCallback(m_context.getWindowHandle(), window_close_callback);
	}
#endif
}


//---------------------------------------------------------
void SimpleGVXR::Window::createWindow(bool aVisibilityFlag)
//---------------------------------------------------------
{}


//-----------------------------------
void SimpleGVXR::Window::initShader()
//-----------------------------------
{}


//------------------------------------------------------
void SimpleGVXR::Window::render(bool aSceneRotationFlag)
//------------------------------------------------------
{}