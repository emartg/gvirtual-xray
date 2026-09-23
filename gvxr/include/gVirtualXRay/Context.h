/*

Copyright (c) 2013-2023, Dr Franck P. Vidal, Bangor University, All rights reserved.
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
*   @file       Context.h
*
*   @brief      Create an OpenGL context using EGL or GLFW.
*
*   @version    1.0
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


#ifndef GVXR_CONTEXT_H
#define GVXR_CONTEXT_H


//******************************************************************************
//  defines
//******************************************************************************
#ifndef EGL_NO_X11
#define EGL_NO_X11
#endif

#ifndef MESA_EGL_NO_X11_HEADERS
#define MESA_EGL_NO_X11_HEADERS
#endif


//******************************************************************************
//  Include
//******************************************************************************
#include <string>

#include <vector>

#ifdef HAS_EGL
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif

#ifdef HAS_GLX
#include <GL/glx.h>
#endif

#ifdef HAS_GLFW
#ifndef USE_EMSCRIPTEN
#define GLFW_INCLUDE_GLCOREARB 1
#endif

#include <GLFW/glfw3.h>
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*   @class  Context
*   @brief  Context is a class to manage an OpenGL or Vulkan context.
*/
//==============================================================================
class Context
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default Constructor
    //--------------------------------------------------------------------------
	Context();


    //--------------------------------------------------------------------------
    /// Default Constructor
    /*
	*  @param aRenderer:   the type of context. Accepted values are "OpenGL", "Vulkan", and "EGL" (default: "OpenGL")
	*  @param aRendererMajorVersion: Select the major version of the renderer.
	*                    (default value: 3)
	*  @param aRendererMajorVersion: Select the minor version of the renderer.
	*                    (default value: 2)
	*/
    //--------------------------------------------------------------------------
	Context(const std::string& aRenderer,
			int aRendererMajorVersion,
			int aRendererMinorVersion);


    //--------------------------------------------------------------------------
    /// Destructor
    //--------------------------------------------------------------------------
	~Context();


    //--------------------------------------------------------------------------
    /// Create a new context
    /*
	*  @param aRenderer:   the type of context. Accepted values are "OpenGL", "Vulkan", and "EGL" (default: "OpenGL")
	*  @param aRendererMajorVersion: Select the major version of the renderer.
	*                    (default value: 3)
	*  @param aRendererMajorVersion: Select the minor version of the renderer.
	*                    (default value: 2)
	*/
    //--------------------------------------------------------------------------
	void create(const std::string& aRenderer = "OpenGL",
			int aRendererMajorVersion = 3,
			int aRendererMinorVersion = 2);


	//--------------------------------------------------------------------------
    /// Destroy the context
    //--------------------------------------------------------------------------
	void release();


    //--------------------------------------------------------------------------
    /// Destroy the context
    //--------------------------------------------------------------------------
	void destroy();


	[[nodiscard]] bool isReady() const;

#ifdef HAS_GLFW
	[[nodiscard]] GLFWwindow* getWindowHandle();
	[[nodiscard]] const GLFWwindow* getWindowHandle() const;
#endif


    //--------------------------------------------------------------------------
    /// Accessor on the number of EGL devices
    /*
    *  @return on the number of EGL devices
    */
    //--------------------------------------------------------------------------
    static unsigned int getNumberOfEGLDevices();


    //--------------------------------------------------------------------------
    /// Accessor on the list of supported EGL clients
    /*
    *  @return on the list of supported EGL clients
    */
    //--------------------------------------------------------------------------
    static std::vector<std::string> findEGLClients();


//******************************************************************************
private:
    //--------------------------------------------------------------------------
    /// Create an OpenGL context using GLX
    //--------------------------------------------------------------------------
    void createGLWithGLX();


    //--------------------------------------------------------------------------
    /// Create an OpenGL context using EGL
    //--------------------------------------------------------------------------
    void createGLWithEGL();


    //--------------------------------------------------------------------------
    /// Create an OpenGL context using GLFW
    /*
    *  @param aVisibilityFlag: flag controling if the window should be visible (1)
    *                          or hidden (0). (default value: 0)
    */
    //--------------------------------------------------------------------------
    void createGLWithGLFW(int aVisibilityFlag = 0);


	//--------------------------------------------------------------------------
    /// Create a Vulkan context using GLFW
    /*
    *  @param aVisibilityFlag: flag controling if the window should be visible (1)
    *                          or hidden (0). (default value: 0)
    */
    //--------------------------------------------------------------------------
    void createVulkanWithGLFW(int aVisibilityFlag = 0);


#ifdef HAS_EGL
    static const char* getDisplayExtensions(EGLDisplay aDisplay);
    void createDisplay(int aPlatform, const std::string& aPlatformStr, int aDeviceID = -1);
    void createEGLContextFromDisplay();
    bool initialiseEGL(std::vector<std::string>& anEGLClientSet);
#endif

#ifdef HAS_VULKAN
	struct {
		VkPhysicalDevice physical_device;
		VkDevice logical_device;
	} m_vk_main_device;

    // Indices (locations) of Queue Families (if they exist at all)
    struct QueueFamilyIndices {
        int graphics_family = -1;			// Location of Graphics Queue Family

        // Check if queue families are valid
        bool isValid()
        {
            return graphics_family >= 0;
        }
    };

    bool createVkInstance();
    bool createVkLogicalDevice();
    bool getVkPhysicalDevice();
    bool checkVkInstanceExtensionSupport(std::vector<const char*>* checkExtensions);

    QueueFamilyIndices getQueueFamilies(VkPhysicalDevice device);
#endif


	std::string m_renderer;
	int m_major_version;
	int m_minor_version;

#ifdef HAS_GLFW
    GLFWwindow* m_p_window;
#endif

#ifdef HAS_EGL
	EGLDisplay m_display;
    EGLConfig m_config;
    EGLContext m_context;
    EGLSurface m_surface;
    std::vector<EGLDeviceEXT> m_p_egl_devices_set;
    int m_device_id;
#endif

#ifdef HAS_GLX
	Display* m_glx_display;
    Colormap m_glx_cmap;
    GLXContext m_glx_context;
    Window m_glx_window;
#endif

#ifdef HAS_VULKAN
	VkInstance m_vk_instance;

	VkQueue m_vk_graphics_queue;
#endif

    bool m_ready;
};


} // namespace gVirtualXRay



#endif
