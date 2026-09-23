/*

Copyright (c) 2019, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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
*   @file       DemoConvolutionKernel.cxx
*
*   @brief      .
*
*   @version    1.0
*
*   @date       12/03/2026
*   @author     Nalin Gupta
*
*   @section    License
*               BSD 3-Clause License.
*
*               For details on use and redistribution please refer
*               to http://opensource.org/licenses/BSD-3-Clause
*
*   @section    Copyright
*               (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*               http://www.fpvidal.net/, Jul 2022, 2022, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifdef HAS_GLEW
#include <GL/glew.h>
#endif

// #include <cmath>

#ifdef HAS_FILESYSTEM_HEADER
#include <filesystem>
#else
#include <experimental/filesystem>
#endif

#ifndef __SimpleGVXR_h
#include "SimpleGVXR.h"
#endif

#ifndef SGVXR_Internal_h
#include "internal.h"
#endif

#ifndef GVXR_IMAGE_H
#include "gVirtualXRay/Image.h"
#endif

#ifndef GVXR_LOGGER_H
#include "gVirtualXRay/Logger.h"
#endif

//******************************************************************************
//  Namespaces
//******************************************************************************
#ifdef HAS_FILESYSTEM_HEADER
namespace fs = std::filesystem;
#else
namespace fs = std::experimental::filesystem;
#endif

//******************************************************************************
//  Global variables
//******************************************************************************
const fs::path g_output_path("demo-convolution-kernel");

int g_image_width(1280 / 2.0);
int g_image_height(800 / 2.0);

int g_square_width(200);
int g_square_height(100);

double g_sigma(10.0);
int g_kernel_size(2 * ceil(3 * g_sigma) + 1);

std::vector<float> float_1d_kernel({.2, .2, .2, .2, .2});


//--------
int main()
//--------
{
    try {
        // Check if g_output_path directory exists
        if (!fs::is_directory(g_output_path) || !fs::exists(g_output_path))
        {
            // Create the output directory if needed
            fs::create_directory(g_output_path);
        }

        useLogFile((g_output_path / fs::path("demo-convolution-kernel-log.txt")).string());

        gVirtualXRay::LOGGER.logNow("gVXR:\t") << getVersionOfCoreGVXR() << std::endl;
        gVirtualXRay::LOGGER.logNow("SimpleGVXR:\t") << getVersionOfSimpleGVXR() << std::endl;

        gVirtualXRay::Image<float> white_rectangle(g_image_width, g_image_height);

        gVirtualXRay::Image<float> white_rectangle_1D(g_image_width, g_image_height);
        gVirtualXRay::Image<float> white_rectangle_2D(g_image_width, g_image_height);

        for (int j = 0; j < int(g_square_height); ++j)
        {
            for (int i = 0; i < int(g_square_width); ++i)
            {
                int centered_index_i(i + (g_image_width-g_square_width) * 0.5);
                int centered_index_j(j + (g_image_height-g_square_height) * 0.5);

                white_rectangle.setPixel(centered_index_i, centered_index_j, 0, 1);
            }
        }

        gVirtualXRay::Image<float> float_2d_kernel = gVirtualXRay::gauss2D<float>(g_kernel_size, g_sigma).normalised(true);

        white_rectangle_1D = white_rectangle.convolution1D(float_1d_kernel);
        white_rectangle_1D.save((g_output_path / fs::path("white_rectangle_1D.tiff")).string());

        white_rectangle_2D = white_rectangle.convolution2D(float_2d_kernel);
        white_rectangle_2D.save((g_output_path / fs::path("white_rectangle_2D.tiff")).string());

        float_2d_kernel.save((g_output_path / fs::path("float_2d_kernel.tiff")).string());

        std::cout << "Convolution results written to: "
                  << fs::absolute(g_output_path).string() << std::endl;

        return 0;
    }
    catch (std::exception& e) {
        gVirtualXRay::LOGGER.logError("") << e.what() << std::endl;
        return 1;
    }
}
