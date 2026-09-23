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
*   @file       TestSinogramFromVoxels.cxx
*
*   @brief      Unit testing: Sinogram.
*
*   @version    1.0
*
*   @date       01/04/2019
*   @author     Dr Franck P. Vidal
*
*   @section    License
*               BSD 3-Clause License.
*
*               For details on use and redistribution please refer
*               to http://opensource.org/licenses/BSD-3-Clause
*
*   @section    Copyright
*               (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*               http://www.fpvidal.net/, Apr 2019, 2019, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <cmath>
#include <iomanip>
#include <memory>
#include <cstdlib>

#include "gtest/gtest.h"

#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#ifndef GVXR_EXCEPTION_H
#include "gVirtualXRay/Exception.h"
#endif

#ifndef GVXR_SINOGRAM_H
#include "gVirtualXRay/Sinogram.h"
#endif


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;
//using namespace std;


//******************************************************************************
//  Global variables
//******************************************************************************
double g_incident_energy = 80.0 * keV;


//******************************************************************************
//  Function declaration
//******************************************************************************


//--------------------------------------------------
TEST(TestSinogramFromVoxels, TestSinogramFromVoxels)
//--------------------------------------------------
{
    // Open CT data in HU
    Image<float> CT_data_set_in_HU("/home/fpvidal/Documents/MEDICAL_DATA/Visible Human/head/ct_data.mhd");

    // Convert HU to linear attenuation coefficients
    Image<float> CT_data_set_in_mu =  CT_data_set_in_HU.convertHU2mu(g_incident_energy);

    // Save a slice of the CT data set
    Image<float> middle_ct_slice_in_HU(CT_data_set_in_HU.setCanvasSize(512, 512, 1));
    Image<float> middle_ct_slice_in_mu(CT_data_set_in_mu.setCanvasSize(512, 512, 1));

    middle_ct_slice_in_HU.save("HU_ct_slice.mha");
    middle_ct_slice_in_mu.save("mu_ct_slice.mha");

    // Compute the Radon transform of the CT slice
    Sinogram<float> sinogram_in_HU(middle_ct_slice_in_HU.radonTransform(0, 1, 179));
    Sinogram<float> sinogram_in_mu(middle_ct_slice_in_mu.radonTransform(0, 1, 179));
    sinogram_in_HU.save("HU_test_sinogram_set.mha");
    sinogram_in_mu.save("mu_test_sinogram_set.mha");

    // Compute a projection set from the sinogram
    Image<float> projection_set_in_HU(sinogram_in_HU.getProjectionSet());
    Image<float> projection_set_in_mu(sinogram_in_mu.getProjectionSet());
    projection_set_in_HU.save("HU_test_projection_set.mha");
    projection_set_in_mu.save("mu_test_projection_set.mha");

#ifndef NDEBUG
    //CT_data_set.save("ct_data_80keV.mha");
    //sinogram.save("sinogram.mha");
    //projection_set.save("projection.mha");
#endif


	// Compare reference projection set and simulated projection set using NCC
	//BOOST_CHECK_CLOSE(reference_projection_set.computeNCC(g_xray_renderer.getProjectionSet()), 1.0, 0.1);

	// Compare reference projection set and simulated projection set using RMSE
    //float RMSE = reference_projection_set.computeRMSE(g_xray_renderer.getProjectionSet());
    //float range = (reference_projection_set.getMaxValue() - reference_projection_set.getMinValue());
    //BOOST_CHECK(RMSE < 0.01f * std::min(range, g_xray_renderer.getProjectionSet().getMaxValue() - g_xray_renderer.getProjectionSet().getMinValue()));
}
