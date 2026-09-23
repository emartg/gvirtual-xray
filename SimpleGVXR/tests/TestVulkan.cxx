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
*   @file       TestMeshMemoryLeak.cxx
*
*   @brief      Unit testing: Test if Vulkan works.
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
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, Dec 2024-present, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <cmath>
#include <vector>
#include <numeric>

#ifdef __linux__
#include <sys/types.h>
#include <sys/sysinfo.h>
#elif WIN32
#include <windows.h>
#include <psapi.h>
#endif


#include "gtest/gtest.h"

#ifndef GVXR_LOGGER_H
#include "gVirtualXRay/Logger.h"
#endif

#ifndef SGVXR_SimpleGVXR_h
#include "SimpleGVXR.h"
#endif


//******************************************************************************
//  Global variables
//******************************************************************************
int g_main_window_width(1280 / 2.0);
int g_main_window_height(800 / 2.0);

// double g_incident_energy_in_keV(80.0);
// const double g_scaling_factor(2.0);
// double g_resolution_in_mm = (0.15 * g_scaling_factor); // 1mm per pixel
// int g_number_of_pixels[] = { int(600 / g_scaling_factor), 1 };
// double g_source_position_in_cm[] = { 0.0, 0.0, -10.0 };
// double g_detector_position_in_cm[] = { 0.0, 0.0,  10.0 };
// double g_detector_up_vector[] = { 0.0, 1.0,  0.0 };

// double g_literature_result_in_keV(0.0);
// double g_expected_result_in_keV(0.0);


//******************************************************************************
//  Function declaration
//******************************************************************************
// void loadSTLFile();
// void loadDetector();
// void loadSource();
// unsigned int processMemoryUsage();


//----------------------------
TEST(TestVulkan, TestVulkan)
//----------------------------
{
    // Create an OpenGL context
    createWindow(-1, 0, 
        "VULKAN", 1, 2, 
        0); // Half-precision floating-point numbers is enough

    setWindowSize(g_main_window_width, g_main_window_height);

    // Destroy the windows
    destroyAllWindows();

    // Kill the simulation engine
    destroy();
}
