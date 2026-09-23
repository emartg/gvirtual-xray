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


#ifndef SGVXR_Window_h
#define SGVXR_Window_h


/**
********************************************************************************
*
*   @file       Window.h
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
#include <string>
#include <list>
#include <utility>

// To create the OpenGL or Vulkan context
#ifndef GVXR_CONTEXT_H
#include "gVirtualXRay/Context.h"
#endif


//******************************************************************************
//  Function declarations
//******************************************************************************


//******************************************************************************
//  namespace
//******************************************************************************
namespace SimpleGVXR {

    
//==============================================================================
/**
*   @class    Window
*   @brief    Window is a virtual class to handle windows.
*/
//==============================================================================
class Window
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //------------------------------------------------------------------------------
    /// Default constructor
    /**
     *  @param aVisibilityFlag: flag controling if the window should be visible (1)
     *                          or hidden (0). (default value: 0)
     */
    //------------------------------------------------------------------------------
    explicit Window(int aVisibilityFlag = 0);


    //------------------------------------------------------------------------------
    /// Destructor
     //------------------------------------------------------------------------------
    virtual ~Window();


    //------------------------------------------------------------------------------
    /// Pure virtual method to set the size of the given window.
    /**
     *  @param aWidth:      the number of pixels along the X-axis
     *  @param aHeight:     the number of pixels along the Y-axis
     */
    //------------------------------------------------------------------------------
    virtual void setWindowSize(int aWidth, int aHeight);


    //------------------------------------------------------------------------------
    /// Accessor on the size of the window.
    /**
     *  @return the size of the window
     */
    //------------------------------------------------------------------------------
    [[nodiscard]] std::pair<int, int> getWindowSize() const;


    //------------------------------------------------------------------------------
    /// Initialise the viewer parameters.
    //------------------------------------------------------------------------------
    void initViewer();


    //------------------------------------------------------------------------------
    /// Pure virtual method to render the 3D visualisation once.
    /**
     *  @param aSceneRotationFlag:  if true, the rotation transformation will be applied, else it won't be
     */
     //------------------------------------------------------------------------------
    virtual void displayScene(bool aSceneRotationFlag);


    //------------------------------------------------------------------------------
    /// Pure virtual method to take a screenshot of the 3D visualisation.
    //------------------------------------------------------------------------------
    virtual void takeScreenshot();


    //------------------------------------------------------------------------------
    /// Virtual method to render the 3D visualisation in an interactive loop.
     //------------------------------------------------------------------------------
    virtual void renderLoop();


    //--------------------------------------------------------------------------
    /// Show the window.
    //--------------------------------------------------------------------------
    void showWindow();


    //--------------------------------------------------------------------------
    /// Set the native window title.
    //--------------------------------------------------------------------------
    void setWindowTitle(const std::string& aTitle);


    //--------------------------------------------------------------------------
    /// Hide the window.
    //--------------------------------------------------------------------------
    void hideWindow();


    //------------------------------------------------------------------------------
    /// Move a window.
    /**
    *  @param aPositionX:  the window position along the X axis of the screen
    *  @param aPositionY:  the window position along the Y axis of the screen
    */
    //------------------------------------------------------------------------------
    void setPosition(int aPositionX, int aPositionY);


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


    //--------------------------------------------------------------------------
    /// Destroy the window.
    //--------------------------------------------------------------------------
    void destroyWindow();


    //--------------------------------------------------------------------------
    /// Accessor on the rendering context of the window.
    /**
     *  @return the rendering context of the window
     */
    //--------------------------------------------------------------------------
    gVirtualXRay::Context& getContext();


#ifdef HAS_GLFW
    //--------------------------------------------------------------------------
    /// Accessor on the pointer of the Window instance given an GLFWwindow pointer.
    /**
     *  @param apGLFWwindow: the pointer on the GLFWwindow
     *  @return the pointer on the corresponding Window instance
     */
    //--------------------------------------------------------------------------
    static Window* getWindow(GLFWwindow* apGLFWwindow);
#endif


//******************************************************************************
protected:
    //--------------------------------------------------------------------------
    /// Virtual method to create a window.
    /**
     *  @param aVisibilityFlag: if true, the window will be visible, 
     *                          else it won't be
     */
    //--------------------------------------------------------------------------
    virtual void createWindow(bool aVisibilityFlag);


    //--------------------------------------------------------------------------
    /// Pure virtual method to initialise the shaders.
    //--------------------------------------------------------------------------
    virtual void initShader();


    //--------------------------------------------------------------------------
    /// Pure virtual method to perform the rendering of the 3D visualisation.
    /**
     *  @param aSceneRotationFlag:  if true, the rotation transformation will be applied, else it won't be
     */
    //--------------------------------------------------------------------------
    virtual void render(bool aSceneRotationFlag);


    //--------------------------------------------------------------------------
    /// Virtual method to register callbacks.
    //--------------------------------------------------------------------------
    virtual void registerCallbacks();


    /// Static list that register every window that is created. If a window is destroyed, it is removed from this list.
    static std::list<Window*> m_p_window_set;

    /// Rendering context associated with the window
    gVirtualXRay::Context m_context;

    /// Title of the window
    std::string m_title;

    /// Flag set to true if the window is visible.
    bool m_visible;

    ///  Width of the window in number of pixels
    int m_width;

    ///  Height of the window in number of pixels
    int m_height;

    ///  Flag initialised to false. It is set to true when the viewer is used the first time
    bool m_viewer_initialised;

    ///  Flag initialised to false. It is set to true when the callbacks are registered
    bool m_callback_registered;

    /// Background colour of the window in RGB with values between 0 and 1.
    float m_background_colour[3]{};

    bool m_first_pass;
};
};

#endif
