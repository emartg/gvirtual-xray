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
*   @file       Context.cxx
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


//******************************************************************************
//  Include
//******************************************************************************
#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <iostream>

#include <algorithm> // For capitalising a string

#include <filesystem> // To list the list of devices in /dev/dri
#include <vector>
#include <sys/stat.h>

#ifndef WIN32
#include <fcntl.h>
#include <unistd.h>
#endif

#ifndef GVXR_OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

// #define GL_GLEXT_PROTOTYPES 1
// #include <GLFW/glfw3.h>
//
// #ifdef __APPLE__
// #include <OpenGL/gl.h>
// #else
// #include <GL/gl.h>
// #include <GL/glext.h>
// #endif
//
#ifdef HAS_EGL
#ifdef HAS_GBM
#include <fcntl.h>
#include <gbm.h>
#endif
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif

#ifdef HAS_GLFW
#define GLFW_INCLUDE_NONE 1
#include <GLFW/glfw3.h>
#endif

// #ifdef HAS_EGL
// #define GLFW_EXPOSE_NATIVE_EGL 1
// #include <GLFW/glfw3native.h>
// #endif

#ifndef GVXR_CONTEXT_H
#include "gVirtualXRay/Context.h"
#endif

#ifndef GVXR_LOGGER_H
#include <gVirtualXRay/Logger.h>
#endif

#ifndef GVXR_RESOURCE_MANAGER_H
#include "gVirtualXRay/ResourceManager.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
using namespace gVirtualXRay;


int g_number_of_EGL_devices = 0;

#ifdef HAS_EGL
PFNEGLQUERYDEVICESEXTPROC eglQueryDevicesEXT = nullptr;
PFNEGLGETPLATFORMDISPLAYEXTPROC eglGetPlatformDisplayEXT = nullptr;
PFNEGLQUERYDEVICESTRINGEXTPROC eglQueryDeviceStringEXT = nullptr;
PFNEGLQUERYDEVICEATTRIBEXTPROC eglQueryDeviceAttribEXT = nullptr;

//typedef const char* eglGetDisplayDriverName_t(EGLDisplay disp);
//typedef char* eglGetDisplayDriverconfig_t(EGLDisplay disp);

//eglGetDisplayDriverName_t eglGetDisplayDriverName = nullptr;
//eglGetDisplayDriverconfig_t eglGetDisplayDriverconfig = nullptr;

void initEGLFunctions()
{
    // eglQueryDevicesEXT
    if (eglQueryDevicesEXT == nullptr)
    {
        eglQueryDevicesEXT = (PFNEGLQUERYDEVICESEXTPROC)
            eglGetProcAddress("eglQueryDevicesEXT");
    }

    if (eglQueryDevicesEXT == nullptr)
    {
        throw gVirtualXRay::Exception(
            __FILE__,
            __FUNCTION__,
            __LINE__,
            "eglQueryDevicesEXT() could not be loaded.");
    }

    // eglGetPlatformDisplayEXT
    if (eglGetPlatformDisplayEXT == nullptr)
    {
        eglGetPlatformDisplayEXT = (PFNEGLGETPLATFORMDISPLAYEXTPROC)
            eglGetProcAddress("eglGetPlatformDisplayEXT");
    }

    if (eglGetPlatformDisplayEXT == nullptr)
    {
        throw gVirtualXRay::Exception(
            __FILE__,
            __FUNCTION__,
            __LINE__,
            "eglGetPlatformDisplayEXT() could not be loaded.");
    }

    // eglQueryDeviceStringEXT
    if (eglQueryDeviceStringEXT == nullptr)
    {
        eglQueryDeviceStringEXT = (PFNEGLQUERYDEVICESTRINGEXTPROC)
            eglGetProcAddress("eglQueryDeviceStringEXT");
    }

    if (eglQueryDeviceStringEXT == nullptr)
    {
        throw gVirtualXRay::Exception(
            __FILE__,
            __FUNCTION__,
            __LINE__,
            "eglQueryDeviceStringEXT() could not be loaded.");
    }

    // eglQueryDeviceAttribEXT
    if (eglQueryDeviceAttribEXT == nullptr)
    {
        eglQueryDeviceAttribEXT = (PFNEGLQUERYDEVICEATTRIBEXTPROC)
            eglGetProcAddress("eglQueryDeviceAttribEXT");
    }

    if (eglQueryDeviceAttribEXT == nullptr)
    {
        throw gVirtualXRay::Exception(
            __FILE__,
            __FUNCTION__,
            __LINE__,
            "eglQueryDeviceAttribEXT() could not be loaded.");
    }
}

struct device_property_t {
    EGLint name;
    const char* displayName;
    const char* extension;
    enum Type {
        String,
        Attribute
    } type;
};


static const device_property_t deviceProperties[] {
#ifdef EGL_DRM_DEVICE_FILE_EXT
    { EGL_DRM_DEVICE_FILE_EXT, "DRM device file", "EGL_EXT_device_drm", device_property_t::String },
#endif

#ifdef EGL_CUDA_DEVICE_NV
    { EGL_CUDA_DEVICE_NV, "CUDA device", "EGL_NV_device_cuda", device_property_t::Attribute }
#endif
};

static const int devicePropertiesSize = sizeof(deviceProperties) / sizeof(device_property_t);
#endif


//-----------------
Context::Context():
//-----------------
		m_renderer(""),
		m_major_version(0),
		m_minor_version(0),

#ifdef HAS_GLFW
		m_p_window(nullptr),
#endif

#ifdef HAS_EGL
		m_display(EGL_NO_DISPLAY),
		m_config(nullptr),
		m_context(nullptr),
		m_surface(nullptr),
        m_device_id(-1),
#endif

#ifdef HAS_GLX
        m_glx_display(nullptr),
        m_glx_cmap(0),
        m_glx_context(nullptr),
        m_glx_window(0),
#endif

		m_ready(false)
//--------------------------------------------
{
	ResourceManager::getInstance().setGraphicsContext(this);
}


//--------------------------------------------
Context::Context(const std::string& aRenderer,
				 int aRendererMajorVersion,
				 int aRendererMinorVersion):
//--------------------------------------------
		m_major_version(0),
		m_minor_version(0),

#ifdef HAS_GLFW
		m_p_window(nullptr),
#endif

#ifdef HAS_EGL
		m_display(EGL_NO_DISPLAY),
		m_config(nullptr),
		m_context(nullptr),
		m_surface(nullptr),
        m_device_id(-1),
#endif

#ifdef HAS_GLX
        m_glx_display(nullptr),
        m_glx_cmap(0),
        m_glx_context(nullptr),
        m_glx_window(0),
#endif

		m_ready(false)
//--------------------------------------------
{
	ResourceManager::getInstance().setGraphicsContext(this);
	create(aRenderer, aRendererMajorVersion, aRendererMinorVersion);
}


//-----------------
Context::~Context()
//-----------------
{
	ResourceManager::getInstance().destroyAndRemoveContext();
	release();
	destroy();
}


//------------------------------------------------
void Context::create(const std::string& aRenderer,
					 int aRendererMajorVersion,
					 int aRendererMinorVersion)
//------------------------------------------------
{
	// The renderer has changed
	if (m_renderer != aRenderer ||
			m_major_version != aRendererMajorVersion ||
			m_minor_version != aRendererMinorVersion)
	{
		// Release the previous context if applicable
		release();

		// Store the context details
		m_renderer = aRenderer;
		m_major_version = aRendererMajorVersion;
		m_minor_version = aRendererMinorVersion;

		// Create the context if possible
		std::string renderer = m_renderer;
		std::transform(renderer.begin(), renderer.end(), renderer.begin(), ::toupper);

		if (renderer == "OPENGL") createGLWithGLFW();
		else if (renderer == "VULKAN") createVulkanWithGLFW();
		else if (renderer == "EGL") createGLWithEGL();
		else if (renderer == "GLX") createGLWithGLX();
		else if (renderer != "OPENMP") LOGGER.logWarning("Invalid context: ") << aRenderer <<
				" (" << aRendererMajorVersion << "." << aRendererMinorVersion << ")" << std::endl;
	}
}


//---------------------
void Context::release()
//---------------------
{
    ResourceManager::getInstance().releaseResources();

#ifdef HAS_EGL
    if (m_display != EGL_NO_DISPLAY && m_context)
    {
        eglDestroyContext(m_display, m_context);
        m_context = nullptr;
    }

    if (m_display != EGL_NO_DISPLAY)
    {
        eglTerminate(m_display);
        m_display = EGL_NO_DISPLAY;
    }

        m_device_id = -1;

#endif // HAS_EGL

#ifdef HAS_GLX
    if (m_glx_display)
    {
        glXMakeCurrent( m_glx_display, 0, nullptr );

        if (m_glx_context)
        {
            glXDestroyContext( m_glx_display, m_glx_context );
            m_glx_context = nullptr;
        }

        if (m_glx_window)
        {
            XDestroyWindow( m_glx_display, m_glx_window );
            m_glx_window = 0;
        }

        if (m_glx_cmap)
        {
            XFreeColormap( m_glx_display, m_glx_cmap );
            m_glx_cmap = 0;
        }

        XCloseDisplay( m_glx_display );
        m_glx_display = nullptr;
    }
#endif // HAS_GLX

#ifdef HAS_GLFW
    if (m_p_window)
        {
        glfwDestroyWindow(m_p_window);
        m_p_window = nullptr;
        }
#endif

    m_ready = false;
}


//---------------------
void Context::destroy()
//---------------------
{
    release();
}


//---------------------------
bool Context::isReady() const
//---------------------------
{
	return m_ready;
}


#ifdef HAS_GLFW
//------------------------------------
GLFWwindow* Context::getWindowHandle()
//------------------------------------
{
	return m_p_window;
}


//------------------------------------------------
const GLFWwindow* Context::getWindowHandle() const
//------------------------------------------------
{
	return m_p_window;
}
#endif


unsigned int Context::getNumberOfEGLDevices()
{
    g_number_of_EGL_devices = 0;

#ifdef HAS_EGL

#ifdef HAS_GLAD
    gVirtualXRay::initialiseGLAD("EGL");
#endif

    if (!g_number_of_EGL_devices)
        {
        initEGLFunctions();



        LOGGER.logNow("Query the number of EGL devices") << std::endl;

        EGLBoolean result = eglQueryDevicesEXT(0, nullptr, &g_number_of_EGL_devices);

        if (result == EGL_TRUE)
        {
            LOGGER.logNow("Success") << std::endl;
        }
        else
        {
            LOGGER.logWarning("Failed to query EGL devices") << std::endl;
            return 0;
        }

        if (g_number_of_EGL_devices > 0)
        {
            LOGGER.logNow("Detected ") << g_number_of_EGL_devices << " EGL devices." << std::endl;
        }
        else
        {
            LOGGER.logWarning("Failed to find an EGL device") << std::endl;
            return 0;
        }

        // Device extensions
        LOGGER.logNow("Print the details here of every EGL device.") << std::endl;
        std::vector<EGLDeviceEXT> egl_devices_set(g_number_of_EGL_devices);
        result = eglQueryDevicesEXT(g_number_of_EGL_devices, &egl_devices_set[0], &g_number_of_EGL_devices);

        if (result == EGL_TRUE)
        {
            LOGGER.logNow("Success") << std::endl;
        }
        else
        {
            LOGGER.logWarning("Failed to query EGL devices") << std::endl;
            return g_number_of_EGL_devices;
        }

        // bool software_rendering = false;
        for (int i = 0; i < g_number_of_EGL_devices; ++i)
        {
            LOGGER.logNow("Device ") << i + 1 << "/" << g_number_of_EGL_devices << ":" << std::endl;

            EGLDeviceEXT device = egl_devices_set[i];

            const char* p_extensions = eglQueryDeviceStringEXT(device, EGL_EXTENSIONS);
            if (p_extensions)
            {
                const std::string extensions = p_extensions;
                if (extensions.size())
                {
                    LOGGER.logNow("\tDevice Extensions: ") << extensions << std::endl;

                    if (extensions.find("EGL_MESA_device_software") != std::string::npos)
                    {
                        LOGGER.logWarning("\tSoftware rendering with Mesa will be used with this device") << std::endl;
                        // software_rendering = true;
                    }

                    if (extensions.find("EGL_EXT_device_drm") != std::string::npos ||
                            extensions.find("EGL_EXT_device_drm_render_node") != std::string::npos)
                    {
                        const char* p_device_file = eglQueryDeviceStringEXT(egl_devices_set[i], EGL_DRM_DEVICE_FILE_EXT);
                        if (p_device_file)
                        {
                            const std::string device_file = p_device_file;
                            if (device_file.size())
                                LOGGER.logNow("\tEGL DRM device file: ") << device_file << std::endl;
                            else
                                LOGGER.logNow("\tEGL DRM device file: NONE") << std::endl;
                        }
                        else
                        {
                            LOGGER.logNow("\tFailed to retrieve EGL DRM device file.") << std::endl;
                        }
                    }
                }
                else
                    LOGGER.logNow("\tDevice Extensions: NONE") << std::endl;
            }
            else
            {
                LOGGER.logNow("\tFailed to retrieve device extensions.") << std::endl;
            }

            // const char* p_renderer = eglQueryDeviceStringEXT(egl_devices_set[i], EGL_RENDERER_EXT);
            // if (p_renderer)
            // {
            //     const std::string renderer = p_renderer;
            //     if (renderer.size())
            //         LOGGER.logNow("\tDevice renderer: ") << renderer << std::endl;
            //     else
            //         LOGGER.logNow("\tDevice renderer: NONE") << std::endl;
            // }
            // else
            // {
            //     LOGGER.logNow("\tFailed to retrieve device renderer.") << std::endl;
            // }


            const char* p_vendor = eglQueryDeviceStringEXT(egl_devices_set[i], EGL_VENDOR);
            if (p_vendor) {
                const std::string vendor = p_vendor;
                if (vendor.size())
                    LOGGER.logNow("\tDevice vendor: ") << vendor << std::endl;
                else
                    LOGGER.logNow("\tDevice vendor: unknown") << std::endl;
            }
            else {
                LOGGER.logNow("\tFailed to retrieve device vendor.") << std::endl;
            }
        }
    }
#endif

    return g_number_of_EGL_devices;
}


//------------------------------------------------
std::vector<std::string> Context::findEGLClients()
//------------------------------------------------
{
    std::vector<std::string> client_set;

#ifdef HAS_EGL

    std::string client_extensions = getDisplayExtensions(EGL_NO_DISPLAY);

    if (client_extensions.find("EGL_EXT_platform_base") != std::string::npos)
    {
        if (client_extensions.find("EGL_MESA_platform_surfaceless") != std::string::npos)
        {
            client_set.push_back("Surfaceless platform");
        }

        if (client_extensions.find("EGL_EXT_platform_x11") != std::string::npos ||
            client_extensions.find("EGL_KHR_platform_x11") != std::string::npos)
        {
            client_set.push_back("X11 platform");
        }

        if (client_extensions.find("EGL_EXT_platform_wayland") != std::string::npos ||
            client_extensions.find("EGL_KHR_platform_wayland") != std::string::npos)
        {
            client_set.push_back("Wayland platform");
        }

        if (client_extensions.find("EGL_KHR_platform_android") != std::string::npos)
        {
            client_set.push_back("Android platform");
        }

#ifdef HAS_GBM
        if (client_extensions.find("EGL_KHR_platform_gbm") != std::string::npos) {
            client_set.push_back("EGL_KHR_platform_gbm");
        }

        if (client_extensions.find("EGL_MESA_platform_gbm") != std::string::npos) {
            client_set.push_back("EGL_MESA_platform_gbm");
        }
#endif // HAS_GBM

        if (client_extensions.find("EGL_EXT_platform_device") != std::string::npos ||
            client_extensions.find("EGL_KHR_platform_device") != std::string::npos)
        {
            client_set.push_back("Device platform");
        }
    }
#endif // HAS_EGL

    return client_set;
}


#ifdef HAS_EGL
//------------------------------------------------------------
const char* Context::getDisplayExtensions(EGLDisplay aDisplay)
//------------------------------------------------------------
{
    const char* extensions = nullptr;

    if (aDisplay == EGL_NO_DISPLAY)
    {
        LOGGER.logNow("EGL client extensions:") << std::endl;
    }
    else
    {
        LOGGER.logNow("EGL extensions:") << std::endl;
    }

    extensions = eglQueryString(aDisplay, EGL_EXTENSIONS);

    if (!extensions)
    {
        LOGGER.logWarning("\tRetrieving EGL extensions failed") << std::endl;
    }
    else
    {
        LOGGER.logNow(extensions) << std::endl;
    }

    return extensions;
}


//----------------------------------------------------------
void Context::createDisplay(int aPlatform,
                            const std::string& aPlatformStr,
                            int aDeviceID)
//----------------------------------------------------------
{
    LOGGER.logNow("*** Try to build an EGL display for ") << aPlatformStr << ". aDeviceID is: " <<
        aDeviceID << ". ***" << std::endl;

    if (aDeviceID < 0 && aPlatform != EGL_PLATFORM_GBM_MESA)
    {
        LOGGER.logNow("*** Use EGL_DEFAULT_DISPLAY on Platform ") << aPlatform << " ***" << std::endl;
        m_display = eglGetPlatformDisplayEXT(aPlatform, EGL_DEFAULT_DISPLAY, nullptr);
    }
#ifdef HAS_GBM
    else if (aPlatform == EGL_PLATFORM_GBM_MESA)
    {
        namespace fs = std::filesystem;

        std::vector<std::string> device_name_set;

        // Check all elements in "/dev/"
        LOGGER.logNow("\tLook for potential GBM devices in \"/dev/\":") << std::endl;
        for (const auto& entry : fs::directory_iterator("/dev/"))
        {
            std::filesystem::path filename_path = entry.path();
            std::string filename = filename_path.string();
            struct stat sb;

            // The element is a character device file
            if (stat(filename.c_str(), &sb) == 0 && sb.st_mode & S_IFCHR) {

                // This is a NVIDIA device
                size_t pos = filename.find("/dev/nvidia");
                if (pos == 0 && filename.size() == 12) {
                    device_name_set.push_back(filename);
                    LOGGER.logNow("\t\tFound :") << filename << std::endl;
                }
            }
        }

        // /dev/dri exists
        if (std::filesystem::is_directory("/dev/dri")) {
            // Check all elements in "/dev/dri/"
            LOGGER.logNow("\tLook for potential GBM devices in \"/dev/dri\":") << std::endl;
            for (const auto& entry : fs::directory_iterator("/dev/dri/"))
            {
                std::filesystem::path filename_path = entry.path();
                std::string filename = filename_path.string();
                struct stat sb;

                // The element is a character device file
                if (stat(filename.c_str(), &sb) == 0 && sb.st_mode & S_IFCHR) {
                    device_name_set.push_back(filename);
                    LOGGER.logNow("\t\tFound :") << filename << std::endl;
                }
            }
        }
        // /dev/dri does not exist
        else {
            LOGGER.logWarning("\t\"/dev/dri\" does not exist") << std::endl;
        }

        // Sort and reverse so that /dev/dri/renderD128 is before /dev/dri/card0
        std::sort (device_name_set.begin(), device_name_set.end());
        std::reverse(device_name_set.begin(), device_name_set.end());

        // const char* device_names[] = {"/dev/nvidia0", "/dev/dri/card0", "/dev/dri/card1", "/dev/dri/renderD128"};
        int fd = -1;
        gbm_device* p_gbm_device = nullptr;
        m_display = EGL_NO_DISPLAY;

        for (size_t i = 0; i < device_name_set.size(); ++i)
        {
            // No suitable device found
            if (fd < 0 || p_gbm_device == nullptr || m_display == EGL_NO_DISPLAY)
            {
                // Reinitialise the variables
                fd = -1;
                p_gbm_device = nullptr;
                m_display = EGL_NO_DISPLAY;

                // Open the device file
                fd = open(device_name_set[i].c_str(), O_RDWR);

                // The device file is not open
                if (fd < 0)
                {
                    LOGGER.logWarning("\tCannot open the device (") << device_name_set[i] <<
                        "). It's usualy a file permission issue. Check with your administrator." <<
                        std::endl;
                }
                // The device file is open
                else {
                    LOGGER.logNow("\tThe device (") << device_name_set[i] << ") is open." << std::endl;
                    LOGGER.logNow("\tOpen the corresponding GBM device.") << std::endl;

                    // Open the GBM device
                    p_gbm_device = gbm_create_device(fd);

                    // The GBM device is not open
                    if (!p_gbm_device)
                    {
                        LOGGER.logWarning("\tCannot open the corresponding GBM device.") << std::endl;
                    }
                    // The GBM device is open
                    else
                    {
                        // Get the display
                        m_display = eglGetPlatformDisplayEXT(EGL_PLATFORM_GBM_MESA,
                                                            p_gbm_device,
                                                            nullptr);

                        // The display could not be obtained
                        if (m_display == EGL_NO_DISPLAY)
                        {
                            LOGGER.logWarning("\tGBM was requested and it is enabled, but it failed to create a display. We'll try an alternative, but expect the unexpected!") << std::endl;
                        }
                    }
                }
            }
        }

        // There is still no display
        if (m_display == EGL_NO_DISPLAY)
        {
            // Try the default display
            LOGGER.logWarning("Try the default display") << std::endl;
            m_display = eglGetPlatformDisplayEXT(EGL_PLATFORM_DEVICE_EXT, EGL_DEFAULT_DISPLAY, nullptr);
        }
    }
#else // HAS_GBM
    else if (aPlatform == EGL_PLATFORM_GBM_MESA)
    {
        LOGGER.logWarning("\tGBM was requested but it was not enabled at the compilation time. We'll try an alternative, but expect the unexpected!") << std::endl;
        m_display = eglGetPlatformDisplayEXT(EGL_PLATFORM_DEVICE_EXT, EGL_DEFAULT_DISPLAY, NULL);
    }
#endif // HAS_GBM
    else
    {
        LOGGER.logNow("*** Use EGL_PLATFORM_DEVICE_EXT ***") << std::endl;

        // Query device extensions
        const std::string client_extensions = eglQueryDeviceStringEXT(m_p_egl_devices_set[aDeviceID], EGL_EXTENSIONS);

        LOGGER.logNow("Supported EGL extensions are: ") << client_extensions << std::endl;

        // Try to use a DRM device
        if (client_extensions.find("EGL_EXT_device_drm") != std::string::npos || client_extensions.find("EGL_EXT_device_drm_render_node") != std::string::npos) {
            const char* p_device_ID = eglQueryDeviceStringEXT(m_p_egl_devices_set[aDeviceID], EGL_DRM_DEVICE_FILE_EXT);

            // The device ID was provided
            if (p_device_ID != nullptr) {
                LOGGER.logNow("The device's ID is: ") << p_device_ID << std::endl;
                int fd = open(p_device_ID, O_RDWR, 0);

                if (fd < 0) {
                    LOGGER.logWarning("The device ") << p_device_ID << " cannot be open." << std::endl;
                }
                else {
                    EGLint attribs[] = {
                        EGL_DRM_MASTER_FD_EXT,
                        fd,
                        EGL_NONE
                    };

                    m_display = eglGetPlatformDisplayEXT(EGL_PLATFORM_DEVICE_EXT, m_p_egl_devices_set[aDeviceID], attribs);
                }
            }
            // The device ID was not provided
            else {
                LOGGER.logNow("The device's ID could not be queried: ") << std::endl;
            }
        }

        // This is a CUDA device
        if (client_extensions.find("EGL_CUDA_DEVICE_NV") != std::string::npos && m_display != EGL_NO_DISPLAY) {
            m_display = eglGetPlatformDisplayEXT(EGL_PLATFORM_DEVICE_EXT, m_p_egl_devices_set[aDeviceID], nullptr);
        }

        // Last try as an EGL_PLATFORM_DEVICE_EXT
        if (m_display != EGL_NO_DISPLAY) {
            m_display = eglGetPlatformDisplayEXT(EGL_PLATFORM_DEVICE_EXT, m_p_egl_devices_set[aDeviceID], nullptr);
        }
    }

    if (m_display != EGL_NO_DISPLAY)
    {
        LOGGER.logNow("\tSuccess.") << std::endl;

        EGLint egl_major_ver = 0;
        EGLint egl_minor_ver = 0;

        LOGGER.logNow("\tIntialising GLAD with EGL support...") << std::endl;
        initialiseGLAD("EGL", nullptr, 0, &m_display);

        LOGGER.logNow("\tIntialising EGL...") << std::endl;

        if (eglInitialize(m_display, &egl_major_ver, &egl_minor_ver) == EGL_FALSE)
        {
            LOGGER.logWarning("Failed to eglInitialize") << std::endl;

            EGLint error =  eglGetError();
            switch(error)
            {
                case EGL_BAD_DISPLAY:
                    LOGGER.logWarning("display is not an EGL display connection") << std::endl;
                    break;
                case EGL_NOT_INITIALIZED:
                    LOGGER.logWarning("display cannot be initialized") << std::endl;
                    break;
                default:
                    LOGGER.logWarning("unknown EGL error") << std::endl;
                    break;
            }

            m_display = EGL_NO_DISPLAY;
        }
        else
        {
            LOGGER.logNow("EGL version: ") << egl_major_ver << "." << egl_minor_ver << std::endl;

            const char* extensions = getDisplayExtensions(m_display);
            LOGGER.logNow("EGL extensions: ") << extensions << std::endl;
            LOGGER.logNow() << std::endl;
        }
    }
    else
    {
        LOGGER.logWarning("\tFailure.") << std::endl;
    }
}


//-------------------------------------------------------------------
bool Context::initialiseEGL(std::vector<std::string>& anEGLClientSet)
//-------------------------------------------------------------------
{
    // Initialise EGL
    LOGGER.logNow("Initialise EGL") << std::endl;

    LOGGER.logNow("Initialise EGL functions") << std::endl;
    initEGLFunctions();

    // Find all the EGL clients
    LOGGER.logNow("Find all the EGL clients") << std::endl;
    if (anEGLClientSet.size() == 0)
        anEGLClientSet = findEGLClients();

    LOGGER.logNow("Try to build an EGL display") << std::endl;
    while (anEGLClientSet.size())
    {
        std::string EGL_client = anEGLClientSet.back();
        anEGLClientSet.pop_back();

        LOGGER.logNow("for the ") << EGL_client << " client" << std::endl;

        if (m_display == EGL_NO_DISPLAY && EGL_client == "Android platform")
        {
            LOGGER.logNow("with Platform EGL_PLATFORM_ANDROID_KHR.") << std::endl;
            createDisplay(EGL_PLATFORM_ANDROID_KHR, EGL_client);
        }
        else if (m_display == EGL_NO_DISPLAY && EGL_client == "Wayland platform")
        {
            LOGGER.logNow("with Platform EGL_PLATFORM_WAYLAND_EXT.") << std::endl;
            createDisplay(EGL_PLATFORM_WAYLAND_EXT, EGL_client);
        }
        else if (m_display == EGL_NO_DISPLAY && EGL_client == "Surfaceless platform")
        {
            LOGGER.logNow("with Platform EGL_PLATFORM_SURFACELESS_MESA.") << std::endl;
            createDisplay(EGL_PLATFORM_SURFACELESS_MESA, EGL_client);
        }
#ifdef HAS_GBM
        else if (m_display == EGL_NO_DISPLAY && EGL_client == "EGL_KHR_platform_gbm")
        {
            LOGGER.logNow("with Platform EGL_KHR_platform_gbm.") << std::endl;
            createDisplay(EGL_PLATFORM_GBM_KHR, EGL_client);
        }
        else if (m_display == EGL_NO_DISPLAY && EGL_client == "GBM EGL_MESA_platform_gbm")
        {
            LOGGER.logNow("with Platform EGL_MESA_platform_gbm.") << std::endl;
            createDisplay(EGL_PLATFORM_GBM_MESA, EGL_client);
        }
#endif // HAS_GBM
        else if (m_display == EGL_NO_DISPLAY && EGL_client == "X11 platform")
        {
            LOGGER.logNow("with Platform EGL_PLATFORM_X11_EXT.") << std::endl;
            createDisplay(EGL_PLATFORM_X11_EXT, EGL_client);
        }
        else if (m_display == EGL_NO_DISPLAY && EGL_client == "Device platform")
        {
            LOGGER.logNow("Go through all the EGL devices to find a suitable display.") << std::endl;
            LOGGER.logNow("Surfaceless have the lowest priority.") << std::endl;

            EGLint number_of_egl_devices = getNumberOfEGLDevices();
            m_p_egl_devices_set.resize(number_of_egl_devices);
            eglQueryDevicesEXT(number_of_egl_devices, &m_p_egl_devices_set[0], &number_of_egl_devices);

            std::vector<int> p_priority_queue(number_of_egl_devices);
            int start_id = 0;
            int end_id = number_of_egl_devices - 1;

            for (int i = 0; i < number_of_egl_devices; ++i)
            {
                EGLDeviceEXT device = m_p_egl_devices_set[i];
                const std::string client_extensions = eglQueryDeviceStringEXT(device, EGL_EXTENSIONS);
                LOGGER.logNow("Device ") << i << "'s extensions: " << client_extensions << std::endl;

                if (client_extensions.find("EGL_MESA_platform_surfaceless") != std::string::npos)
                {
                    p_priority_queue[end_id--] = i;
                }
                else
                {
                    p_priority_queue[start_id++] = i;
                }
            }

            for (int i = 0; i < number_of_egl_devices; ++i)
            {
                if (m_display == EGL_NO_DISPLAY)
                {
                    std::string platform = "Platform device " + std::to_string(p_priority_queue[i] + 1) + "/" + std::to_string(number_of_egl_devices);
                    createDisplay(EGL_PLATFORM_DEVICE_EXT, platform, p_priority_queue[i]);
                }
            }
        }

        if (m_display != EGL_NO_DISPLAY) return true;
    }

    return false;
}
#endif


// Code taken from:
// https://www.khronos.org/opengl/wiki/Tutorial:_OpenGL_3.0_Context_Creation_(GLX)
#ifdef HAS_GLX
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
#endif


#ifdef HAS_OPENGL
// Code taken from:
// https://www.khronos.org/opengl/wiki/Tutorial:_OpenGL_3.0_Context_Creation_(GLX)
static bool isExtensionSupported(const char *extList, const char *extension)
{
  const char *start;
  const char *where, *terminator;
  
  /* Extension names should not have spaces. */
  where = strchr(extension, ' ');
  if (where || *extension == '\0')
    return false;

  /* It takes a bit of care to be fool-proof about parsing the
     OpenGL extensions string. Don't be fooled by sub-strings,
     etc. */
  for (start=extList;;) {
    where = strstr(start, extension);

    if (!where)
      break;

    terminator = where + strlen(extension);

    if ( where == start || *(where - 1) == ' ' )
      if ( *terminator == ' ' || *terminator == '\0' )
        return true;

    start = terminator;
  }

  return false;
}
#endif


#ifdef HAS_GLX
int handler(Display * d, XErrorEvent * e)
{
    char errorstring[1024];
    XGetErrorText(d, e->error_code, errorstring, 1023);

    LOGGER.logError("X error message: ") << errorstring << std::endl;
    return 0;
}
#endif


//-----------------------------
void Context::createGLWithGLX()
//-----------------------------
{
    // Release the previous context if applicable
	release();

#ifdef HAS_GLX
    XSetErrorHandler(handler);

    // Code adapted from:
    // https://www.khronos.org/opengl/wiki/Tutorial:_OpenGL_3.0_Context_Creation_(GLX)

    m_ready = false;

    // Open the X display
    m_glx_display = XOpenDisplay(nullptr);

    if (!m_glx_display)
    {
        LOGGER.logWarning("Failed to open X display") << std::endl;
        return;
    }

    // Get a matching FB config
    static int visual_attribs[] =
    {
        GLX_X_RENDERABLE    , True,
        GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
        GLX_RENDER_TYPE     , GLX_RGBA_BIT,
        GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
        GLX_RED_SIZE        , 8,
        GLX_GREEN_SIZE      , 8,
        GLX_BLUE_SIZE       , 8,
        GLX_ALPHA_SIZE      , 8,
        GLX_DEPTH_SIZE      , 8,
        GLX_STENCIL_SIZE    , 8,
        GLX_DOUBLEBUFFER    , False,
        //GLX_SAMPLE_BUFFERS  , 1,
        //GLX_SAMPLES         , 4,
        None
    };

    int glx_major, glx_minor;
 
    // FBConfigs were added in GLX version 1.3.
    if ( !glXQueryVersion( m_glx_display, &glx_major, &glx_minor ) || 
        ( ( glx_major == 1 ) && ( glx_minor < 3 ) ) || ( glx_major < 1 ) )
    {
        LOGGER.logWarning("Invalid GLX version") << std::endl;
        return;
    }

    LOGGER.logNow("Getting matching framebuffer configs") << std::endl;
    int fbcount;
    GLXFBConfig* fbc = glXChooseFBConfig(m_glx_display, DefaultScreen(m_glx_display), visual_attribs, &fbcount);
    if (!fbc)
    {
        LOGGER.logWarning("Failed to retrieve a framebuffer config") << std::endl;
        return;
    }
    LOGGER.logNow( "Found ") << fbcount << " matching FB configs." << std::endl;

    // Pick the FB config/visual with the most samples per pixel
    LOGGER.logNow("Getting XVisualInfos") << std::endl;
    int best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;

    for (int i=0; i<fbcount; ++i)
    {
        XVisualInfo *vi = glXGetVisualFromFBConfig( m_glx_display, fbc[i] );
        if ( vi )
        {
            int samp_buf, samples;
            glXGetFBConfigAttrib( m_glx_display, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf );
            glXGetFBConfigAttrib( m_glx_display, fbc[i], GLX_SAMPLES       , &samples  );

            if ( (best_fbc < 0) || (samp_buf && (samples > best_num_samp)) )
                best_fbc = i, best_num_samp = samples;
            
            if ( worst_fbc < 0 || !samp_buf || samples < worst_num_samp )
                worst_fbc = i, worst_num_samp = samples;
        }
        XFree( vi );
    }

    GLXFBConfig bestFbc = fbc[ best_fbc ];

    // Be sure to free the FBConfig list allocated by glXChooseFBConfig()
    XFree( fbc );

    // Get a visual
    XVisualInfo *vi = glXGetVisualFromFBConfig( m_glx_display, bestFbc );

    LOGGER.logNow("Creating colormap") << std::endl;
    XSetWindowAttributes swa;

    m_glx_cmap = XCreateColormap( m_glx_display,
    RootWindow( m_glx_display, vi->screen ), vi->visual, AllocNone );
    swa.colormap = m_glx_cmap;
    swa.background_pixmap = None ;
    swa.border_pixel      = 0;
    swa.event_mask        = StructureNotifyMask;

    LOGGER.logNow("Creating window") << std::endl;
    m_glx_window = XCreateWindow( m_glx_display, RootWindow( m_glx_display, vi->screen ), 
        0, 0, 100, 100, 0, vi->depth, InputOutput, 
        vi->visual, 
        CWBorderPixel|CWColormap|CWEventMask, &swa );

    // Done with the visual info data
    XFree( vi );

    if ( !m_glx_window )
    {
        LOGGER.logWarning("Failed to create window") << std::endl;
        return;
    }

    XStoreName( m_glx_display, m_glx_window, "gVirtualXray" );

    LOGGER.logNow("Mapping window") << std::endl;
    XMapWindow( m_glx_display, m_glx_window );

    // Get the default screen's GLX extension list
    auto glx_screen = DefaultScreen( m_glx_display );
    const char *glxExts = glXQueryExtensionsString( m_glx_display, glx_screen);

    // NOTE: It is not necessary to create or make current to a context before
    // calling glXGetProcAddressARB
    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = nullptr;
    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)
        glXGetProcAddressARB( (const GLubyte *) "glXCreateContextAttribsARB" );

    // Install an X error handler so the application won't exit if GL 3.0
    // context allocation fails.
    //
    // Note this error handler is global.  All display connections in all threads
    // of a process use the same error handler, so be sure to guard against other
    // threads issuing X commands while this code is running.
    // ctxErrorOccurred = false;
    // int (*oldHandler)(Display*, XErrorEvent*) = XSetErrorHandler(&ctxErrorHandler);

    // Check for the GLX_ARB_create_context extension string and the function.
    // If either is not present, use GLX 1.3 context creation method.
    if ( !isExtensionSupported( glxExts, "GLX_ARB_create_context" ) || !glXCreateContextAttribsARB )
    {
        LOGGER.logWarning("glXCreateContextAttribsARB() not found ... using old-style GLX context") << std::endl;
        m_glx_context = glXCreateNewContext( m_glx_display, bestFbc, GLX_RGBA_TYPE, nullptr, True );
    }
    // If it does, try to get a GL m_major_version.m_minor_version context!
    else
    {
        int context_attribs[] =
        {
            GLX_CONTEXT_MAJOR_VERSION_ARB, m_major_version,
            GLX_CONTEXT_MINOR_VERSION_ARB, m_minor_version,
            //GLX_CONTEXT_FLAGS_ARB        , GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
            None
        };

        LOGGER.logNow("Creating context") << std::endl;
        m_glx_context = glXCreateContextAttribsARB( m_glx_display, bestFbc, nullptr,
            True, context_attribs );

        // Sync to ensure any errors generated are processed.
        XSync( m_glx_display, False );
        if (  m_glx_context )
            LOGGER.logNow("Creating GL ") << m_major_version << "." << m_minor_version << " context" << std::endl;
        else
        {
            LOGGER.logNow("Couldn't create GL ") << m_major_version << "." << m_minor_version << " context" << std::endl;
            return;
        }
    }

    // Sync to ensure any errors generated are processed.
    XSync( m_glx_display, False );

    // Verifying that context is a direct context
    if ( ! glXIsDirect ( m_glx_display, m_glx_context ) )
    {
        LOGGER.logNow("Indirect GLX rendering context obtained") << std::endl;
        printf( "Indirect GLX rendering context obtained\n" );
    }
    else
    {
        LOGGER.logNow("Direct GLX rendering context obtained") << std::endl;
    }

    LOGGER.logNow("Making context current") << std::endl;
    if (glXMakeCurrent( m_glx_display, m_glx_window, m_glx_context ))
    {
        // Initialise GLAD
        gVirtualXRay::initialiseGLAD("GLX", m_glx_display, glx_screen);

        // Initialise GLEW
        gVirtualXRay::initialiseGLEW();

        LOGGER.logNow("OpenGL version supported by this platform ") << glGetString(GL_VERSION) << std::endl;

        // Get rid of any OpenGL error at this stage
        glGetError();

        int major = 0;
        int minor = 0;
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);

        if ((major >= m_major_version)  || ((major == m_major_version) && (minor >= m_minor_version)))
        {
            m_ready = true;
        }
    }
    else
    {
        LOGGER.logError("Making context current failed") << std::endl;
    }    
#endif
}


//-----------------------------
void Context::createGLWithEGL()
//-----------------------------
{
	// See https://developer.nvidia.com/blog/egl-eye-opengl-visualization-without-x-server/
	// For an example

	// Release the previous context if applicable
	release();

#ifdef HAS_EGL
    initEGLFunctions();
    unsigned int number_of_EGL_devices = getNumberOfEGLDevices();

    // Initialise GLAD
#ifdef HAS_GLAD
    gVirtualXRay::initialiseGLAD("EGL", nullptr, 0, &m_display);
#endif

	// Initialise EGL
    std::vector<std::string> EGL_client_set = findEGLClients();

    if (number_of_EGL_devices == 0) {
        LOGGER.logWarning("Could not find any EGL client supported by this host:") << std::endl;
        return;
    }
    LOGGER.logNow("EGL clients supported by this host:") << std::endl;
    for (auto i: EGL_client_set)
    {
        LOGGER.logNow("\t- ") << i << std::endl;
    }

    // Go through all the clients until one hopefully works
    m_ready = false;
    while (EGL_client_set.size() && m_ready == false)
    {
        if (initialiseEGL(EGL_client_set))
        {
            // Context attributes
            constexpr EGLint attrib_list[] =
                {
                EGL_RED_SIZE, 8,
                EGL_BLUE_SIZE, 8,
                EGL_GREEN_SIZE, 8,
                EGL_DEPTH_SIZE, 8,
                EGL_LUMINANCE_SIZE, 0,
                EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
                EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
                EGL_NONE, EGL_NONE
                };

            // Choose the config supporting the attributes set above
            EGLint    num_config;
            if (eglChooseConfig(m_display, attrib_list, &m_config, 1, &num_config) == EGL_FALSE)
            {
                LOGGER.logWarning("Failed to eglChooseConfig") << std::endl;
                m_ready = false;
            }
            else if(num_config < 1)
            {
                LOGGER.logWarning("No matching EGL frame buffer configuration") << std::endl;
                m_ready = false;
            }
            else
            {
                // Bind the OpenGL API to the context
                LOGGER.logNow("Bind the OpenGL API to EGL") << std::endl;
                if (eglBindAPI(EGL_OPENGL_API) == EGL_FALSE)
                {
                    LOGGER.logWarning("Failed to eglBindAPI(EGL_OPENGL_API)") << std::endl;
                    m_ready = false;
                }
                else
                {
                    // OpenGL context
                    const EGLint context_attrib[] =
                    {
                        EGL_CONTEXT_MAJOR_VERSION_KHR,  m_major_version,
                        EGL_CONTEXT_MINOR_VERSION_KHR,  m_minor_version,
                        EGL_CONTEXT_OPENGL_PROFILE_MASK_KHR,    EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT_KHR,
                        EGL_CONTEXT_FLAGS_KHR,          EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE_BIT_KHR
                #ifndef NDEBUG
                        | EGL_CONTEXT_OPENGL_DEBUG_BIT_KHR
                #endif
                        ,
                        EGL_NONE
                    };

                    // Create the context
                    LOGGER.logNow("Create the context") << std::endl;
                    m_context = eglCreateContext(m_display, m_config, EGL_NO_CONTEXT, context_attrib);
                    if (m_context == EGL_NO_CONTEXT)
                    {
                        m_context = eglCreateContext(m_display, m_config, EGL_NO_CONTEXT, nullptr);
                    }

                    // The context was not created
                    if (m_context == EGL_NO_CONTEXT)
                    {
                        LOGGER.logWarning("Failed to eglCreateContext") << std::endl;
                        EGLint error =  eglGetError();
                        switch(error)
                        {
                            case EGL_BAD_CONFIG:
                                LOGGER.logWarning("config is not an EGL frame buffer configuration, or does not support the current rendering API") << std::endl;
                                break;
                            case EGL_BAD_ATTRIBUTE:
                                LOGGER.logWarning("attrib_list contains an invalid context attribute or if an attribute is not recognized or out of range") << std::endl;
                                break;
                            default:
                                LOGGER.logWarning("Unknown error") << std::endl;
                                break;
                        }

                        m_ready = false;
                    }
                    else
                    {
                        // Create the surface
                        LOGGER.logNow("Create the surface") << std::endl;
                        m_surface = eglCreatePbufferSurface(m_display,
                                m_config,
                                nullptr);

                        // The surface was not created
                        if (!m_surface)
                        {
                            LOGGER.logWarning("Error: eglCreatePbufferSurface failed Error") << std::endl;
                            m_ready = false;
                        }
                        else
                        {
                            // Make current
                            LOGGER.logNow("Make the context current") << std::endl;
                            if (eglMakeCurrent(m_display,
                                    m_surface,
                                    m_surface,
                                    m_context) == EGL_FALSE)
                            {
                                LOGGER.logWarning("Failed to eglMakeCurrent") << std::endl;
                                m_ready = false;
                            }
                            else
                            {
                                // Initialise GLAD
                                gVirtualXRay::initialiseGLAD("EGL");

                                // Initialise GLEW
                                gVirtualXRay::initialiseGLEW();

                                // Still no error?
                                if(EGL_SUCCESS != eglGetError())
                                {
                                    m_ready = false;
                                }
                                else
                                {
                                    LOGGER.logNow("OpenGL version supported by this platform ") << glGetString(GL_VERSION) << std::endl;

                                    // Get rid of any OpenGL error at this stage
                                    glGetError();

                                    int major = 0;
                                    int minor = 0;
                                    glGetIntegerv(GL_MAJOR_VERSION, &major);
                                    glGetIntegerv(GL_MINOR_VERSION, &minor);

                                    if ((major >= m_major_version) || ((major == m_major_version) && (minor >= m_minor_version)))
                                    {
                                        m_ready = true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
#endif // HAS_EGL
}


//-------------------------------------------------
void Context::createGLWithGLFW(int aVisibilityFlag)
//-------------------------------------------------
{
    // Release the previous context if applicable
    release();

#ifdef HAS_GLFW
    std::string window_title = "gVirtualXRay -- OpenGL Window (GLFW)";

    LOGGER.logNow("Initialise GLFW") << std::endl;

    // Initialize GLFW
    if (glfwInit())
    {

        // Set the version of OpenGL that is required
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_major_version);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_minor_version);
        glfwWindowHint(GLFW_DEPTH_BITS, 24);

        if (((m_major_version == 3) && (m_minor_version >= 2)) || (m_major_version >= 4))
        {
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        }

        // Enable anti-aliasing
        glfwWindowHint(GLFW_SAMPLES, 4);

        // Set the visibility
        if (aVisibilityFlag)
        {
            glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
        }
        else
        {
            glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        }

        // Create a windowed mode window and its OpenGL context
        LOGGER.logNow("Create an OpenGL window with a ") << m_major_version << "." << m_minor_version << " context." << std::endl;
        m_p_window = glfwCreateWindow(1, 1, window_title.data(), nullptr, nullptr);

        // Window has be created
        if (m_p_window)
        {
            // Make the window's context current
            LOGGER.logNow("Make the window's context current") << std::endl;
            glfwMakeContextCurrent(m_p_window);
            glfwSetWindowUserPointer(m_p_window, this);

            // Initialise GLAD
            gVirtualXRay::initialiseGLAD("GLFW");

            // Initialise GLEW
            gVirtualXRay::initialiseGLEW();

            int major = 0;
            int minor = 0;
            glGetIntegerv(GL_MAJOR_VERSION, &major);
            glGetIntegerv(GL_MINOR_VERSION, &minor);

            if ((major >= m_major_version) || ((major == m_major_version) && (minor >= m_minor_version)))
            {
                m_ready = true;
            }
        }
    }
    else
    {
        LOGGER.logWarning("Cannot initialise GLFW.") << std::endl;
    }
#else
    LOGGER.logWarning("Cannot use GLFW to create a context, it was not enabled when gVXR was compiled.") << std::endl;
#endif
}


//-----------------------------------------------------
void Context::createVulkanWithGLFW(int aVisibilityFlag)
//-----------------------------------------------------
{
// Compiled with Vulkan
#ifdef HAS_VULKAN

	// Release the previous context if applicable
	release();

    std::string window_title = "gVirtualXRay -- Vulkan Window (GLFW)";

    LOGGER.logNow("Initialise GLFW") << std::endl;

    // Initialize GLFW
    if (glfwInit())
    {
        // Make sure no version is explicitly required
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        // Set the visibility
        if (aVisibilityFlag)
        {
            glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
        }
        else
        {
            glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        }

        // Create a windowed mode window and its OpenGL context
        LOGGER.logNow("Create a Vulkan window with a ") << m_major_version << "." << m_minor_version << " context." << std::endl;
        m_p_window = glfwCreateWindow(1, 1, window_title.data(), 0, 0);

        // Window has be created
        if (m_p_window)
        {
            // Make the window's context current
            LOGGER.logNow("Make the window's context current") << std::endl;
            // glfwMakeContextCurrent(m_p_window);
            glfwSetWindowUserPointer(m_p_window, this);

            // Vulkan is supported by the system
            if (glfwVulkanSupported())
            {
                // Vulkan is available, at least for compute
                gVirtualXRay::LOGGER.logNow("Vulkan is available, at least for compute") << std::endl;

                // Create the Vulkan instance
                if (createVkInstance())
                {
                    // Select the physical device
                    if (getVkPhysicalDevice())
                    {
                        // Create the Vulkan logical device
                        if (createVkLogicalDevice())
                        {
                            // All the initialisation stages have passed
                            m_ready = true;
                        }
                        else
                        {
                            LOGGER.logWarning("Failed to attach a Vulkan physical device") << std::endl;
                            m_ready = false;
                        }
                    }
                    else
                    {
                        LOGGER.logWarning("Failed to create a Vulkan logical device") << std::endl;
                        m_ready = false;
                    }
                }
                else
                {
                    LOGGER.logWarning("Failed to create a Vulkan Instance") << std::endl;
                    m_ready = false;
                }
            }
            // Vulkan is not supported by the system
            else
            {
                m_ready = false;
            }
        }
    }
    else
    {
        LOGGER.logWarning("Cannot initialise GLFW.") << std::endl;
    }

// Compiled without Vulkan
#else
    throw gVirtualXRay::Exception(
        __FILE__,
        __FUNCTION__,
        __LINE__,
        "ERROR: gVXR was built without Vulkan support.");
#endif
}


#ifdef HAS_VULKAN
//------------------------------
bool Context::createVkInstance()
//------------------------------
{
	// Information about the application itself
	// Most data here doesn't affect the program and is for developer convenience
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Vulkan App";					// Custom name of the application
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);		// Custom version of the application
	appInfo.pEngineName = "No Engine";							// Custom engine name
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);			// Custom engine version
	appInfo.apiVersion = VK_API_VERSION_1_0;					// The Vulkan Version

	// Creation information for a VkInstance (Vulkan Instance)
	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	// Create list to hold instance extensions
	std::vector<const char*> instanceExtensions = std::vector<const char*>();

	// Set up extensions Instance will use
	uint32_t glfwExtensionCount = 0;				// GLFW may require multiple extensions
	const char** glfwExtensions;					// Extensions passed as array of cstrings, so need pointer (the array) to pointer (the cstring)

	// Get GLFW extensions
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	// Add GLFW extensions to list of extensions
	for (size_t i = 0; i < glfwExtensionCount; i++)
	{
		instanceExtensions.push_back(glfwExtensions[i]);
	}

	// Check Instance Extensions supported...
	if (checkVkInstanceExtensionSupport(&instanceExtensions))
    {
        createInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
        createInfo.ppEnabledExtensionNames = instanceExtensions.data();

        // TODO: Set up Validation Layers that Instance will use
        createInfo.enabledLayerCount = 0;
        createInfo.ppEnabledLayerNames = nullptr;

        // Create instance
        VkResult result = vkCreateInstance(&createInfo, nullptr, &m_vk_instance);

        return result == VK_SUCCESS;
    }
	else
    {
        LOGGER.logWarning("VkInstance does not support required extensions") << std::endl;
        return false;
	}
}


//-----------------------------------
bool Context::createVkLogicalDevice()
//-----------------------------------
{
	//Get the queue family indices for the chosen Physical Device
	QueueFamilyIndices indices = getQueueFamilies(m_vk_main_device.physical_device);

	// Queue the logical device needs to create and info to do so (Only 1 for now, will add more later!)
	VkDeviceQueueCreateInfo queueCreateInfo = {};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = indices.graphics_family;					// The index of the family to create a queue from
	queueCreateInfo.queueCount = 1;												// Number of queues to create
	float priority = 1.0f;
	queueCreateInfo.pQueuePriorities = &priority;								// Vulkan needs to know how to handle multiple queues, so decide priority (1 = highest priority)

	// Information to create logical device (sometimes called "device")
	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount = 1;						// Number of Queue Create Infos
	deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;			// List of queue create infos so device can create required queues
	deviceCreateInfo.enabledExtensionCount = 0;						// Number of enabled logical device extensions
	deviceCreateInfo.ppEnabledExtensionNames = nullptr;				// List of enabled logical device extensions

	// Physical Device Features the Logical Device will be using
	VkPhysicalDeviceFeatures deviceFeatures = {};

	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;			// Physical Device features Logical Device will use

	// Create the logical device for the given physical device
	VkResult result = vkCreateDevice(m_vk_main_device.physical_device, &deviceCreateInfo, nullptr, &m_vk_main_device.logical_device);
	if (result != VK_SUCCESS)
	{
		return false;
	}
	// Queues are created at the same time as the device...
	// So we want handle to queues
	// From given logical device, of given Queue Family, of given Queue Index (0 since only one queue), place reference in given VkQueue
    else
    {
    	vkGetDeviceQueue(m_vk_main_device.logical_device, indices.graphics_family, 0, &m_vk_graphics_queue);
		return true;
	}
}


bool isDeviceSuitable(VkPhysicalDevice aDevice)
{
    VkPhysicalDeviceProperties device_properties;
    VkPhysicalDeviceFeatures device_features;

    vkGetPhysicalDeviceProperties(aDevice, &device_properties);
    vkGetPhysicalDeviceFeatures(aDevice, &device_features);

    return device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
           device_features.geometryShader;
           
}


//---------------------------------
bool Context::getVkPhysicalDevice()
//---------------------------------
{
    // Initialise the variable
    m_vk_main_device.physical_device = VK_NULL_HANDLE;

    // Get the number of Vulkan devices
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(m_vk_instance, &device_count, nullptr);
    LOGGER.logNow("Number of Vulkan devices found: ") << device_count << std::endl;

    if (device_count == 0)
    {
        LOGGER.logWarning("Failed to find GPUs with Vulkan support") << std::endl;
        return false;
    }

    // Check each device
    std::vector<VkPhysicalDevice> devices(device_count);
    vkEnumeratePhysicalDevices(m_vk_instance, &device_count, devices.data());

    for (const auto& device : devices) {
        if (isDeviceSuitable(device)) {
            m_vk_main_device.physical_device = device;
            break;
        }
    }

    if (m_vk_main_device.physical_device == VK_NULL_HANDLE)
    {
        LOGGER.logWarning("Failed to find a suitable device") << std::endl;
        return false;
    }

    return true;
}


bool Context::checkVkInstanceExtensionSupport(std::vector<const char*>* checkExtensions)
{
	// Need to get number of extensions to create array of correct size to hold extensions
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	// Create a list of VkExtensionProperties using count
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	// Check if given extensions are in list of available extensions
	for (const auto &checkExtension : *checkExtensions)
	{
		bool hasExtension = false;
		for (const auto &extension : extensions)
		{
			if (strcmp(checkExtension, extension.extensionName))
			{
				hasExtension = true;
				break;
			}
		}

		if (!hasExtension)
		{
			return false;
		}
	}

	return true;
}


Context::QueueFamilyIndices Context::getQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndices indices;

	// Get all Queue Family Property info for the given device
	uint32_t queueFamilyCount = 0;
    std::cout<<" b" <<  std::endl;

	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
std::cout<<" a" <<  std::endl;

	std::vector<VkQueueFamilyProperties> queueFamilyList(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyList.data());

std::cout<<" TTOTT" <<  std::endl;
	// Go through each queue family and check if it has at least 1 of the required types of queue
	int i = 0;
	for (const auto &queueFamily : queueFamilyList)
	{
std::cout<< i << " TTOTT" <<  std::endl;
		// First check if queue family has at least 1 queue in that family (could have no queues)
		// Queue can be multiple types defined through bitfield. Need to bitwise AND with VK_QUEUE_*_BIT to check if has required type
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphics_family = i;		// If queue family is valid, then get index
		}

		// Check if queue family indices are in a valid state, stop searching if so
		if (indices.isValid())
		{
			break;
		}

		i++;
	}

	return indices;
}


#endif
