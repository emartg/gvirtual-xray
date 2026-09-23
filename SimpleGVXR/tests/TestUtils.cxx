/*

Copyright (c) 2025-present, Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), 
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
*   @file       TestUtils.cxx
*
*   @brief      Unit testing: Test if utilities works.
*
*   @date       17/03/2025
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
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, Mar 2025-present, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <vector>

#include "gtest/gtest.h"

#ifndef SGVXR_Utils_h
#include "Utils.h"
#endif


//******************************************************************************
//  Global variables
//******************************************************************************
int g_main_window_width(1280 / 2.0);
int g_main_window_height(800 / 2.0);


//******************************************************************************
//  Function declaration
//******************************************************************************


//------------------------------
TEST(TestUtils, TestTextureSize)
//------------------------------
{
    std::vector<unsigned int> max_texture_size;

    max_texture_size = getMax2DTextureSize();
    ASSERT_EQ(max_texture_size.size(), size_t(2));
    ASSERT_EQ(max_texture_size[0], size_t(0));
    ASSERT_EQ(max_texture_size[1], size_t(0));

    max_texture_size = getMax3DTextureSize();
    ASSERT_EQ(max_texture_size.size(), size_t(3));
    ASSERT_EQ(max_texture_size[0], size_t(0));
    ASSERT_EQ(max_texture_size[1], size_t(0));
    ASSERT_EQ(max_texture_size[2], size_t(0));

    // Create an OpenGL context
#ifdef HAS_OPENGL
#ifdef HAS_EGL
    createWindow(-1, 0,
        "EGL",
        3, 2,
        0); // Half-precision floating-point numbers is enough
#else // HAS_EGL
#ifdef __APPLE__
 // OpenGL 4.2 and above are not supported on MacOS (at least on my compute with MacOS Big Sur)
    createWindow(-1, 0,
        "OPENGL",
        4, 1,
        0); // Half-precision floating-point numbers is enough
#else // __APPLE__
    createWindow(-1, 0,
        "OPENGL",
        4, 6,
        0); // Half-precision floating-point numbers is enough
#endif // __APPLE__
#endif // HAS_EGL
#else // HAS_OPENGL
    createWindow(-1, 0, "OPENMP");
#endif // HAS_OPENGL

    setWindowSize(g_main_window_width, g_main_window_height);

    max_texture_size = getMax2DTextureSize();
    ASSERT_EQ(max_texture_size.size(), size_t(2));
    ASSERT_TRUE(max_texture_size[0] > size_t(0));
    ASSERT_TRUE(max_texture_size[1] > size_t(0));

    max_texture_size = getMax3DTextureSize();
    ASSERT_EQ(max_texture_size.size(), size_t(3));
    ASSERT_TRUE(max_texture_size[0] > size_t(0));
    ASSERT_TRUE(max_texture_size[1] > size_t(0));
    ASSERT_TRUE(max_texture_size[2] > size_t(0)); 

    // Destroy the windows
    destroyAllWindows();

    // Kill the simulation engine
    destroy();
}
