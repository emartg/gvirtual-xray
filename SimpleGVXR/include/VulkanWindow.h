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

3. Neither the name of UK Research and Innovation nor the names of its contributors
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


#ifndef SGVXR_VulkanWindow_h
#define SGVXR_VulkanWindow_h


/**
********************************************************************************
*
*   @file       VulkanWindow.h
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
#ifndef SGVXR_Window_h
#include "Window.h"
#endif

//******************************************************************************
//  namespace
//******************************************************************************
namespace SimpleGVXR {


//==============================================================================
/**
*   @class  VulkanWindow
*   @brief  VulkanWindow is a class to handle windows that use Vulkan as a backend.
*           They are useful to generate 3D visualisations even on 
*           supercomputers or the cloud.
*/
//==============================================================================
class VulkanWindow: public Window
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Create a window
    /**
    *  @param aVisibilityFlag: flag controling if the window should be visible (1)
    *                          or hidden (0). (default value: 0)
    *  @param anVulkanMajorVersion: Vulkan's major version. (default value: 1)
    *  @param anVulkanMinorVersion: Vulkan's major version. (default value: 2)
     */
    //--------------------------------------------------------------------------
    explicit VulkanWindow(int aVisibilityFlag = 0,
        int anVulkanMajorVersion = 1,
        int anVulkanMinorVersion = 2);


    //------------------------------------------------------------------------------
    /// Create an Vulkan context and display it in a window
    /**
     *  @param aWidth:      the number of pixels along the X-axis
     *  @param aHeight:     the number of pixels along the Y-axis
     */
    //------------------------------------------------------------------------------
    virtual void setWindowSize(int aWidth, int aHeight);


    virtual void displayScene(bool aSceneRotationFlag);


    virtual void takeScreenshot();


    virtual void renderLoop();


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


protected:
    // Initialise GLEW
    virtual void createWindow(bool aVisibilityFlag);

    // Initialise Vulkan
    virtual void initShader();


    virtual void render(bool aSceneRotationFlag);


    virtual void registerCallbacks();
};
};

#endif
