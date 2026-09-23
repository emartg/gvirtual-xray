// GLAD
#ifdef HAS_GLAD
#include <glad/gl.h>

#ifdef HAS_EGL
#include <glad/egl.h>
#endif // HAS_EGL

#ifdef HAS_GLX
#include <glad/glx.h>
#endif // HAS_GLX

#endif // HAS_GLAD

// GLFW
#ifdef HAS_GLFW
#include <GLFW/glfw3.h>
#endif

#include <sstream>

#ifndef GVXR_LOGGER_H
#include <gVirtualXRay/Logger.h>
#endif

#ifndef GVXR_EXCEPTION_H
#include "gVirtualXRay/Exception.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//-----------------------------------------------
    void initialiseGLAD(const std::string& anAPI,
                        void* aGLXDisplay,
                        int aGLXScreen,
                        void* anEGLDisplay)
//-----------------------------------------------
{
#ifdef HAS_OPENGL
#ifdef HAS_GLAD
    LOGGER.logNow("Initialise ") << anAPI << " using GLAD." << std::endl;

    bool error_has_occured = false;

    if (anAPI == "GL" || anAPI == "OPENGL" || anAPI == "GLFW")
    {
        if (!gladLoaderLoadGL())
        {
            LOGGER.logError("Cannot initialise OpenGL using GLAD");
            error_has_occured = true;
        }

#ifdef HAS_GLFW
        if (!gladLoadGL(glfwGetProcAddress))
        {
            LOGGER.logError("Cannot initialise OpenGL using GLAD with GLFW3");
            error_has_occured = true;
        }
#endif
    }
    else if (anAPI == "EGL")
    {
#ifdef HAS_EGL
        EGLDisplay* p_display = static_cast<EGLDisplay*>(anEGLDisplay);
        int egl_version = gladLoaderLoadEGL(p_display);
        if (!egl_version)
        {
            LOGGER.logError("Cannot initialise EGL using GLAD") << std::endl;
            error_has_occured = true;
        }
        else {
            LOGGER.logNow("Loaded EGL ") <<
                GLAD_VERSION_MAJOR(egl_version) << "." << GLAD_VERSION_MINOR(egl_version) << std::endl;
        }

        int gl_version = gladLoaderLoadGL();
        if (!gl_version)
        {
            LOGGER.logError("Cannot initialise OpenGL using GLAD");
            error_has_occured = true;
        }
        else {
            LOGGER.logNow("Loaded OpenGL ") <<
                GLAD_VERSION_MAJOR(gl_version) << "." << GLAD_VERSION_MINOR(gl_version) << std::endl;
        }

#else // HAS_EGL
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
            "Cannot initialise EGL as it is not supported on your system.");
#endif // HAS_EGL
    }
    else if (anAPI == "GLX")
    {
#ifdef HAS_GLX
        int glx_version = gladLoaderLoadGLX(static_cast<Display *>(aGLXDisplay), aGLXScreen);
        if (!glx_version)
        {
            LOGGER.logError("Cannot initialise GLX using GLAD") << std::endl;
            error_has_occured = true;
        }
        else {
            LOGGER.logNow("Loaded GLX ") <<
                GLAD_VERSION_MAJOR(glx_version) << "." << GLAD_VERSION_MINOR(glx_version) << std::endl;
        }

        int gl_version = gladLoaderLoadGL();
        if (!gl_version)
        {
            LOGGER.logError("Cannot initialise OpenGL using GLAD") << std::endl;
            error_has_occured = true;
        }
        else {
            LOGGER.logNow("Loaded OpenGL ") <<
                GLAD_VERSION_MAJOR(gl_version) << "." << GLAD_VERSION_MINOR(gl_version) << std::endl;
        }

#else // HAS_GLX
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
            "Cannot initialise GLX as it is not supported on your system.");
#endif // HAS_GLX
    }
    else
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot initialise an unknown API using GLAD");
    }

    if (!error_has_occured) {
        glGetError(); // Remove an error that occurs in Windows

        if (GLAD_GL_VERSION_3_0) LOGGER.logNow("Glad supports OpenGL 3.0") << std::endl;
        if (GLAD_GL_VERSION_3_1) LOGGER.logNow("Glad supports OpenGL 3.1") << std::endl;
        if (GLAD_GL_VERSION_3_2) LOGGER.logNow("Glad supports OpenGL 3.2") << std::endl;
        if (GLAD_GL_VERSION_3_3) LOGGER.logNow("Glad supports OpenGL 3.3") << std::endl;
        if (GLAD_GL_VERSION_4_0) LOGGER.logNow("Glad supports OpenGL 4.0") << std::endl;
        if (GLAD_GL_VERSION_4_1) LOGGER.logNow("Glad supports OpenGL 4.1") << std::endl;
        if (GLAD_GL_VERSION_4_2) LOGGER.logNow("Glad supports OpenGL 4.2") << std::endl;
        if (GLAD_GL_VERSION_4_3) LOGGER.logNow("Glad supports OpenGL 4.3") << std::endl;
        if (GLAD_GL_VERSION_4_4) LOGGER.logNow("Glad supports OpenGL 4.4") << std::endl;
        if (GLAD_GL_VERSION_4_5) LOGGER.logNow("Glad supports OpenGL 4.5") << std::endl;
        if (GLAD_GL_VERSION_4_6) LOGGER.logNow("Glad supports OpenGL 4.6") << std::endl;

    }
#else //HAS_GLAD
    LOGGER.logWarning("GLAD was not initialised as it has been turned off.") << std::endl;
#endif // HAS_GLAD
#endif // HAS_OPENGL
   }


};
