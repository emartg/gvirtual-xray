#ifdef HAS_GLEW
#include <GL/glew.h>
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


//-------------------
void initialiseGLEW()
//-------------------
{
#ifdef HAS_GLEW
    LOGGER.logNow("Initialise GLEW") << std::endl;

	glewExperimental = GL_TRUE;
    glGetError(); // Remove an error that occurs in Windows

	GLenum err = glewInit();
    glGetError(); // Remove an error that occurs in Windows

#ifdef HAS_EGL
	if (err != GLEW_OK && err != GLEW_ERROR_NO_GLX_DISPLAY)
#else
	if (err != GLEW_OK)
#endif
		{
		std::stringstream error_message;
		error_message << "ERROR: cannot initialise GLEW:\t";

		if (err == GLEW_ERROR_NO_GL_VERSION)
			error_message << "Missing GL version";
		else if (err == GLEW_ERROR_GL_VERSION_10_ONLY)
			error_message << "Need at least OpenGL 1.1";
		else if (err == GLEW_ERROR_GLX_VERSION_11_ONLY)
			error_message << "Need at least GLX 1.2";
#ifdef GLEW_ERROR_NO_GLX_DISPLAY		
		else if (err == GLEW_ERROR_NO_GLX_DISPLAY)
			error_message << "Need GLX display for GLX support";
#endif
		else
			error_message << glewGetErrorString(err);

        LOGGER.logError(error_message.str()) << std::endl;
        //throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
		}

	glGetError(); // Remove an error that occurs in Windows
#endif
}


};
