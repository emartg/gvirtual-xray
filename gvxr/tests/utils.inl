#include <iostream>
#include <string>

#include "gtest/gtest.h"

#ifndef Logger_h
#include "gVirtualXRay/Logger.h"
#endif

#ifndef OpenGLUtilities_h
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef Exception_h
#include <gVirtualXRay/Exception.h>
#endif


void initGL(gVirtualXRay::Context& aGLContext, int aMajorOpenGLVersion, int aMinorOpenGLVersion)
{
 #ifdef HAS_OPENGL
    if (!aGLContext.isReady())
    {
       gVirtualXRay::LOGGER.logNow("Try creating an OpenGL ") << aMajorOpenGLVersion << "." << aMinorOpenGLVersion << " context using GLFW" << std::endl;

        try
        {
            aGLContext.create("OpenGL", aMajorOpenGLVersion, aMinorOpenGLVersion);
        }
        catch (const gVirtualXRay::Exception& err)
        {
            std::string expected_message("Error: Cannot initialise GLFW.");
            ASSERT_EQ(std::string(err.what()).substr(0, expected_message.size()), expected_message);
        }
        catch(...)
        {
            FAIL() << "Expected gVirtualXRay::Exception";
        }
    }

    if (aGLContext.isReady())
    {
        gVirtualXRay::LOGGER.logNow("\tsuccess") << std::endl;
    }
    else
    {
        gVirtualXRay::LOGGER.logWarning("\tFailure") << std::endl;
    }

#ifdef HAS_EGL
    gVirtualXRay::LOGGER.logNow("EGL is supported by gVXR") << std::endl;
    if (!aGLContext.isReady())
    {
        gVirtualXRay::LOGGER.logNow("Try creating an OpenGL ") << aMajorOpenGLVersion << "." << aMinorOpenGLVersion << " context using EGL" << std::endl;
        aGLContext.create("EGL", aMajorOpenGLVersion, aMinorOpenGLVersion);
    }
#endif

    if (aGLContext.isReady())
    {
        gVirtualXRay::LOGGER.logNow("\tsuccess") << std::endl;
    }
    else
    {
        gVirtualXRay::LOGGER.logWarning("\tFailure") << std::endl;
    }

#ifdef HAS_GLX
    gVirtualXRay::LOGGER.logNow("GLX is supported by gVXR") << std::endl;
    if (!aGLContext.isReady())
    {
        gVirtualXRay::LOGGER.logNow("Try creating an OpenGL ") << aMajorOpenGLVersion << "." << aMinorOpenGLVersion << " context using GLX" << std::endl;
        aGLContext.create("GLX", aMajorOpenGLVersion, aMinorOpenGLVersion);
    }
#endif

    if (aGLContext.isReady())
    {
        gVirtualXRay::LOGGER.logNow("\tsuccess") << std::endl;
    }
    else
    {
        gVirtualXRay::LOGGER.logWarning("\tFailure") << std::endl;
    }
#endif

    // OpenGL is used
    if (aGLContext.isReady()) {
        gVirtualXRay::LOGGER.logNow("Use OpenGL as a compute backend") << std::endl;

        gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // Enable the Z-buffer
        glEnable(GL_DEPTH_TEST);
        gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // Set the background colour
        glClearColor(0.5, 0.5, 0.5, 1.0);
        gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        glEnable(GL_MULTISAMPLE);
        gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // Check the current FBO
        gVirtualXRay::checkFBOErrorStatus(__FILE__, __FUNCTION__, __LINE__);
    }
    // Somehow OpenGL could not be used, default back to OpenMP instead
    else {
        gVirtualXRay::LOGGER.logNow("Use OpenMP as a compute backend") << std::endl;

        aGLContext.create("OpenMP");
    }
}