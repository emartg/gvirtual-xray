/*

Copyright (c) 2019-2023, Dr Franck P. Vidal, Bangor University, All rights reserved.
Copyright (c) 2023-present, Prof Franck P. Vidal (franck.vidal@stfc.ac.uk),
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
*   @file       TestXRayDetectorConstruction.cxx
*
*   @brief      Unit testing: creating and resizing the widgets of an XRayDetector
*               is cheap. Its constructor used to build two spheres of more than
*               20 million triangles each (1.1 s in Release, 4 s in Debug), which
*               made every new detector very slow.
*
*   @author     emartg
*
*   @section    License
*               BSD 3-Clause License.
*
*               For details on use and redistribution please refer
*               to http://opensource.org/licenses/BSD-3-Clause
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <chrono>

#include "gtest/gtest.h"

#ifndef GVXR_UNITS_H
#include "gVirtualXRay/Units.h"
#endif

#ifndef GVXR_XRAY_DETECTOR_H
#include "gVirtualXRay/XRayDetector.h"
#endif


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;


namespace
{
// The limit is far above what a detector costs now (about 1 ms) and far below what it cost before the change (1.1 s in
// Release and 4 s in Debug on a fast machine), so it separates the two without depending on the speed of the machine
const double MAXIMUM_MEAN_TIME_IN_MS = 250.0;

template<typename F> double meanTimeInMs(int aNumberOfRepetitions, F aFunction)
{
    const auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < aNumberOfRepetitions; ++i)
    {
        aFunction();
    }
    const std::chrono::duration<double, std::milli> elapsed = std::chrono::steady_clock::now() - start;
    return elapsed.count() / aNumberOfRepetitions;
}
}


//----------------------------------------------------------------
TEST(TestXRayDetectorConstruction, ConstructionIsCheap)
//----------------------------------------------------------------
{
    const double mean = meanTimeInMs(5, [] { XRayDetector detector; });
    ASSERT_LT(mean, MAXIMUM_MEAN_TIME_IN_MS) << "Creating an XRayDetector took " << mean << " ms on average";
}


//----------------------------------------------------------------
TEST(TestXRayDetectorConstruction, ResizingTheWidgetsIsCheap)
//----------------------------------------------------------------
{
    XRayDetector detector;
    const double mean = meanTimeInMs(5, [&detector] { detector.setWidgetLength(5 * mm); });
    ASSERT_LT(mean, MAXIMUM_MEAN_TIME_IN_MS) << "Resizing the widgets took " << mean << " ms on average";
}
