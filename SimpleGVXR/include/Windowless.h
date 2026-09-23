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


#ifndef SGVXR_Windowless_h
#define SGVXR_Windowless_h


/**
********************************************************************************
*
*   @file       Windowless.h
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
//  Define
//******************************************************************************
#ifdef HAS_EGL
#ifndef EGL_NO_X11
#define EGL_NO_X11
#endif

#ifndef MESA_EGL_NO_X11_HEADERS
#define MESA_EGL_NO_X11_HEADERS
#endif
#endif // HAS_EGL


//******************************************************************************
//  Include
//******************************************************************************
#include "gVirtualXRay/framebufferObject.h"

#include <memory>

#ifndef SGVXR_Window_h
#include "Window.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace SimpleGVXR {


//==============================================================================
/**
*   @class  Windowless
*   @brief  Windowless is a class to handle windows that are not visible.
*           They are useful to generate 3D visualisations even on 
*           supercomputers or the cloud.
*/
//==============================================================================
class Windowless: public Window
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Create a window
    /**
     *  @param aBackend:    The backend to use. 
     *                      Supported values are: 
     *                          "OPENGL",
     *                          "EGL",
     *                          "GLX", and
     *                          "VULKAN".
     *  @param anOpenGLMajorVersion: OpenGL's major version. (default value: 4)
     *  @param anOpenGLMinorVersion: OpenGL's major version. (default value: 3)
     */
    //--------------------------------------------------------------------------
    explicit Windowless(const std::string& aBackend,
        int anOpenGLMajorVersion = 4,
        int anOpenGLMinorVersion = 3);


    //------------------------------------------------------------------------------
    /// Destructor
    //------------------------------------------------------------------------------
    virtual ~Windowless();


    //------------------------------------------------------------------------------
    /// Create an Vulkan context and display it in a window
    /**
     *  @param aWidth:      the number of pixels along the X-axis
     *  @param aHeight:     the number of pixels along the Y-axis
     */
     //------------------------------------------------------------------------------
    virtual void setWindowSize(int aWidth, int aHeight);


    //------------------------------------------------------------------------------
    /// Render the 3D visualisation once.
    /**
     *  @param aSceneRotationFlag:  if true, the rotation transformation will be applied, else it won't be
     */
     //------------------------------------------------------------------------------
    virtual void displayScene(bool aSceneRotationFlag);


    //------------------------------------------------------------------------------
    /// Take a screenshot of the 3D visualisation.
    //------------------------------------------------------------------------------
    virtual void takeScreenshot();


    //--------------------------------------------------------------------------
    /// Set window background colour.
    /**
     *  @param R: the red colour [0, 1]
     *  @param G: the green colour [0, 1]
     *  @param B: the blue colour [0, 1]
     */
     //--------------------------------------------------------------------------
    virtual void setWindowBackGroundColour(float R, float G, float B);


    //--------------------------------------------------------------------------
    /// Set window background colour.
    /**
     *  @param R: the red colour [0, 1]
     *  @param G: the green colour [0, 1]
     *  @param B: the blue colour [0, 1]
     */
     //--------------------------------------------------------------------------
    virtual void setWindowBackGroundColor(float R, float G, float B);


//******************************************************************************
protected:
    //--------------------------------------------------------------------------
    /// Create a window.
    /**
     *  @param aVisibilityFlag: if true, the window will be visible,
     *                          else it won't be
     */
    //--------------------------------------------------------------------------
    virtual void createWindow(bool aVisibilityFlag);

    //--------------------------------------------------------------------------
    /// Initialise the shaders.
    //--------------------------------------------------------------------------
    virtual void initShader();


    //--------------------------------------------------------------------------
    /// Perform the rendering of the 3D visualisation.
    /**
     *  @param aSceneRotationFlag:  if true, the rotation transformation will be applied, else it won't be
     */
    //--------------------------------------------------------------------------
    virtual void render(bool aSceneRotationFlag);


    /// The texture names associated with the FBO (see below)
    unsigned int m_texture_name_set[2]{};


    /// Pointer on the FBO used for offscreen rendering used for the 3D visualisation
    std::unique_ptr<FramebufferObject> m_p_offscreen_fbo;


    /// Flag set to true when the window is instanciated. It is set to false when the textures and FBO for offscreen rendering are created. 
    bool m_first_initialisation;
};


};


#endif
