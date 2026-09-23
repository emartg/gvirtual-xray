/*

Copyright (c) 2024 - present, Prof Franck P. Vidal (franck.vidal@stfc.ac.uk),
United Kingdom Research and Innovation, all rights reserved.

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
*   @file       TestTissueMaterial.cxx
*
*   @brief      Unit testing: Make sure the code in gvxr/src/Utilities/TissueMaterial.cxx is tested.
*
*   @version    1.0
*
*   @date       10/04/2024
*
*   @author     Prof Franck P. Vidal
*
*   @section    License
*               BSD 3-Clause License.
*
*               For details on use and redistribution please refer
*               to http://opensource.org/licenses/BSD-3-Clause
*
*   @section    Copyright
*               (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk),
*               United Kingdom Research and Innovation, Apr 2024, 2024, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <iostream>
#include <map>

#include "gtest/gtest.h"

#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#ifndef GVXR_TISSUE_MATERIAL_H
#include "gVirtualXRay/TissueMaterial.h"
#endif


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;


//----------------------------------------------
TEST(TestTissueMaterial, TestDefaultConstructor)
//----------------------------------------------
{
    TissueMaterial tissue_material;
}


//---------------------------------------
TEST(TestTissueMaterial, TestConstructor)
//---------------------------------------
{
    TissueMaterial tissue_material(
        -1000, 1000,
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
        11, 12, 13, 14, 15, 16, 17, 18, 19
    );

    std::map<int, double> returned_weight_set = tissue_material.getMassWeightSet();

    ASSERT_EQ(returned_weight_set.size(), size_t(19)) << "The cross section size is not 19";

    double sum = 0;
    for (int i = 0; i < 19; ++i) sum += i + 1;

    ASSERT_NEAR(returned_weight_set[Z_H], 1.0 / sum, 1e-9);
    ASSERT_NEAR(returned_weight_set[Z_C], 2.0 / sum, 1e-9);
    ASSERT_NEAR(returned_weight_set[Z_N], 3.0 / sum, 1e-9);
    ASSERT_NEAR(returned_weight_set[Z_O], 4.0 / sum, 1e-9);
    ASSERT_NEAR(returned_weight_set[Z_Na], 5.0 / sum, 1e-9);
    ASSERT_NEAR(returned_weight_set[Z_Mg], 6.0 / sum, 1e-9);
    ASSERT_NEAR(returned_weight_set[Z_P], 7.0 / sum, 1e-9);
    ASSERT_NEAR(returned_weight_set[Z_S], 8.0 / sum, 1e-9);
    ASSERT_NEAR(returned_weight_set[Z_Cl], 9.0 / sum, 1e-9);
    ASSERT_NEAR(returned_weight_set[Z_Ar], 10.0 / sum, 1e-9);
    ASSERT_NEAR(returned_weight_set[Z_K], 11.0 / sum, 1e-9);
    ASSERT_NEAR(returned_weight_set[Z_Ca], 12.0 / sum, 1e-9);
    ASSERT_NEAR(returned_weight_set[Z_Ti], 13.0 / sum, 1e-9);
    ASSERT_NEAR(returned_weight_set[Z_Cu], 14.0 / sum, 1e-9);
    ASSERT_NEAR(returned_weight_set[Z_Zn], 15.0 / sum, 1e-9);
    ASSERT_NEAR(returned_weight_set[Z_Ag], 16.0 / sum, 1e-9);
    ASSERT_NEAR(returned_weight_set[Z_Sn], 17.0 / sum, 1e-9);
    ASSERT_NEAR(returned_weight_set[Z_Fe], 18.0 / sum, 1e-9);
    ASSERT_NEAR(returned_weight_set[Z_I], 19.0 / sum, 1e-9);

    ASSERT_EQ(tissue_material.getMinHUValue(), -1000);
    ASSERT_EQ(tissue_material.getMaxHUValue(), 1000);

    try
    {
        TissueMaterial tissue_material(1000, -1000);

        FAIL() << "Expected gVirtualXRay::Exception";
    }
    catch(const Exception& err)
    {
        std::string expected_message("Error: The min HU value is greater than the max HU value. This is not possible.");
        ASSERT_EQ(std::string(err.what()).substr(0, expected_message.size()), expected_message);
    }
    catch(...)
    {
        FAIL() << "Expected gVirtualXRay::Exception";
    }
}


//-------------------------------------------
TEST(TestTissueMaterial, TestCopyConstructor)
//-------------------------------------------
{
    TissueMaterial tissue_material1(
        -1000, 1000,
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
        11, 12, 13, 14, 15, 16, 17, 18, 19
    );

    TissueMaterial tissue_material2(tissue_material1);

    std::map<int, double> returned_weight_set = tissue_material1.getMassWeightSet();
    std::map<int, double> expected_weight_set = tissue_material2.getMassWeightSet();

    ASSERT_EQ(returned_weight_set.size(), size_t(19)) << "The cross section size is not 19";
    ASSERT_EQ(expected_weight_set.size(), size_t(19)) << "The cross section size is not 19";

    std::map<int, double>::iterator ite1 = returned_weight_set.begin();
    std::map<int, double>::iterator ite2 = expected_weight_set.begin();

    for (int i = 0; i < 19; ++i, ++ite1, ++ite2)
    {

        ASSERT_EQ(ite1->first, ite2->first);
        ASSERT_EQ(ite1->second, ite2->second);
    }

    ASSERT_EQ(tissue_material1.getMinHUValue(), tissue_material2.getMinHUValue());
    ASSERT_EQ(tissue_material1.getMaxHUValue(), tissue_material2.getMaxHUValue());
}


//---------------------------------
TEST(TestTissueMaterial, TestClear)
//---------------------------------
{
    TissueMaterial tissue_material(
        -1000, 1000,
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
        11, 12, 13, 14, 15, 16, 17, 18, 19
    );
    tissue_material.clear();

    std::map<int, double> returned_weight_set = tissue_material.getMassWeightSet();

    ASSERT_EQ(returned_weight_set.size(), size_t(0)) << "The cross section size is not 0";

    for (std::map<int, double>::const_iterator ite = returned_weight_set.begin();
        ite != returned_weight_set.end();
        ++ite)
    {
        ASSERT_NEAR(ite->second, 0.0, 1e-9);
    }

    ASSERT_EQ(tissue_material.getMinHUValue(), 0);
    ASSERT_EQ(tissue_material.getMaxHUValue(), 0);
}



//-----------------------------------------
TEST(TestTissueMaterial, TestEqualOperator)
//-----------------------------------------
{
    TissueMaterial tissue_material1(
        -1000, 1000,
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
        11, 12, 13, 14, 15, 16, 17, 18, 19
    );

    TissueMaterial tissue_material2(tissue_material1);

    TissueMaterial tissue_material3;
    tissue_material3 = tissue_material2;

    TissueMaterial tissue_material4;


    ASSERT_EQ(tissue_material1 == tissue_material1, true) << "The materials are not the same";
    ASSERT_EQ(tissue_material1 == tissue_material2, true) << "The materials are not the same";
    ASSERT_EQ(tissue_material1 == tissue_material3, true) << "The materials are not the same";
    ASSERT_EQ(tissue_material1 == tissue_material4, false) << "The materials are the same";

    ASSERT_EQ(tissue_material2 == tissue_material1, true) << "The materials are not the same";
    ASSERT_EQ(tissue_material2 == tissue_material2, true) << "The materials are not the same";
    ASSERT_EQ(tissue_material2 == tissue_material3, true) << "The materials are not the same";
    ASSERT_EQ(tissue_material2 == tissue_material4, false) << "The materials are the same";

    ASSERT_EQ(tissue_material3 == tissue_material1, true) << "The materials are not the same";
    ASSERT_EQ(tissue_material3 == tissue_material2, true) << "The materials are not the same";
    ASSERT_EQ(tissue_material3 == tissue_material3, true) << "The materials are not the same";
    ASSERT_EQ(tissue_material3 == tissue_material4, false) << "The materials are the same";

    ASSERT_EQ(tissue_material4 == tissue_material1, false) << "The materials are the same";
    ASSERT_EQ(tissue_material4 == tissue_material2, false) << "The materials are the same";
    ASSERT_EQ(tissue_material4 == tissue_material3, false) << "The materials are the same";
    ASSERT_EQ(tissue_material4 == tissue_material4, true) << "The materials are not the same";
}


//--------------------------------------------
TEST(TestTissueMaterial, TestNotEqualOperator)
//--------------------------------------------
{
    TissueMaterial tissue_material1(
        -1000, 1000,
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
        11, 12, 13, 14, 15, 16, 17, 18, 19
    );

    TissueMaterial tissue_material2(tissue_material1);

    TissueMaterial tissue_material3;
    tissue_material3 = tissue_material2;

    TissueMaterial tissue_material4;


    ASSERT_EQ(tissue_material1 != tissue_material1, false) << "The materials are not the same";
    ASSERT_EQ(tissue_material1 != tissue_material2, false) << "The materials are not the same";
    ASSERT_EQ(tissue_material1 != tissue_material3, false) << "The materials are not the same";
    ASSERT_EQ(tissue_material1 != tissue_material4, true) << "The materials are the same";

    ASSERT_EQ(tissue_material2 != tissue_material1, false) << "The materials are not the same";
    ASSERT_EQ(tissue_material2 != tissue_material2, false) << "The materials are not the same";
    ASSERT_EQ(tissue_material2 != tissue_material3, false) << "The materials are not the same";
    ASSERT_EQ(tissue_material2 != tissue_material4, true) << "The materials are the same";

    ASSERT_EQ(tissue_material3 != tissue_material1, false) << "The materials are not the same";
    ASSERT_EQ(tissue_material3 != tissue_material2, false) << "The materials are not the same";
    ASSERT_EQ(tissue_material3 != tissue_material3, false) << "The materials are not the same";
    ASSERT_EQ(tissue_material3 != tissue_material4, true) << "The materials are the same";

    ASSERT_EQ(tissue_material4 != tissue_material1, true) << "The materials are the same";
    ASSERT_EQ(tissue_material4 != tissue_material2, true) << "The materials are the same";
    ASSERT_EQ(tissue_material4 != tissue_material3, true) << "The materials are the same";
    ASSERT_EQ(tissue_material4 != tissue_material4, false) << "The materials are not the same";
}


//-----------------------------------
TEST(TestTissueMaterial, TestDensity)
//-----------------------------------
{
    short HU;
    double expected_density;
    double received_density;

    HU = -1001;
    expected_density = (1.21e-3) * g / cm3;
    received_density = TissueMaterial::getDensity(HU);
    ASSERT_EQ(expected_density, received_density);

    HU = -1000;
    expected_density = (1.21e-3) * g / cm3;
    received_density = TissueMaterial::getDensity(HU);
    ASSERT_EQ(expected_density, received_density);

    HU = -500;
    expected_density = (1.21e-3 + (HU - -1000) * (0.93 - 1.21e-3) / (-98 - -1000)) * g / cm3;
    received_density = TissueMaterial::getDensity(HU);
    ASSERT_EQ(expected_density, received_density);

    HU = -99;
    expected_density = (1.21e-3 + (HU - -1000) * (0.93 - 1.21e-3) / (-98 - -1000)) * g / cm3;
    received_density = TissueMaterial::getDensity(HU);
    ASSERT_EQ(expected_density, received_density);

    HU = -98;
    expected_density = (1.018 + 0.893e-3 * HU) * g / cm3;
    received_density = TissueMaterial::getDensity(HU);
    ASSERT_EQ(expected_density, received_density);

    HU = 14;
    expected_density = (1.018 + 0.893e-3 * HU) * g / cm3;
    received_density = TissueMaterial::getDensity(HU);
    ASSERT_EQ(expected_density, received_density);

    HU = 15;
    expected_density = (1.03) * g / cm3;
    received_density = TissueMaterial::getDensity(HU);
    ASSERT_EQ(expected_density, received_density);

    HU = 22;
    expected_density = (1.03) * g / cm3;
    received_density = TissueMaterial::getDensity(HU);
    ASSERT_EQ(expected_density, received_density);

    HU = 23;
    expected_density = (1.003 + 1.169e-3 * HU) * g / cm3;
    received_density = TissueMaterial::getDensity(HU);
    ASSERT_EQ(expected_density, received_density);

    HU = 60;
    expected_density = (1.003 + 1.169e-3 * HU) * g / cm3;
    received_density = TissueMaterial::getDensity(HU);
    ASSERT_EQ(expected_density, received_density);

    HU = 100;
    expected_density = (1.003 + 1.169e-3 * HU) * g / cm3;
    received_density = TissueMaterial::getDensity(HU);
    ASSERT_EQ(expected_density, received_density);

    HU = 101;
    expected_density = (1.017 + 0.592e-3 * HU) * g / cm3;
    received_density = TissueMaterial::getDensity(HU);
    ASSERT_EQ(expected_density, received_density);

    HU = 1000;
    expected_density = (1.017 + 0.592e-3 * HU) * g / cm3;
    received_density = TissueMaterial::getDensity(HU);
    ASSERT_EQ(expected_density, received_density);
}
