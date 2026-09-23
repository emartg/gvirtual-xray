/*

Copyright (c) 2024 - present, Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), United Kingdom Research and Innovation, all rights reserved.

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
 *   @file       TestMixture.cxx
 *
 *   @brief      Unit testing: Make sure the code in gvxr/src/Utilities/Mixture.cxx is tested.
 *
 *
 *   @version    1.0
 *
 *   @date       08/04/2024
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
#include <map>

#include "gtest/gtest.h"

#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#ifndef GVXR_MIXTURE_H
#include "gVirtualXRay/Mixture.h"
#endif

#ifndef GVXR_ELEMENT_SET_H
#include "gVirtualXRay/ElementSet.h"
#endif


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;


//---------------------------------------
TEST(TestMixture, TestDefaultConstructor)
//---------------------------------------
{
    Mixture mixture;

    ASSERT_NEAR(mixture.getMolarMass(), 0.0, 1e-9) << "The molar mass is not equal to 0.0";
    ASSERT_NEAR(mixture.getDensity(), -1.0, 1e-9) << "The density mass is not equal to -1.0";
    ASSERT_EQ(mixture.getLabel().size(), size_t(0)) << "The label is not empty";
}


//--------------------------------------
TEST(TestMixture, TestMapIntConstructor)
//--------------------------------------
{
    std::map<int, double> weight_set;

    weight_set[1] = 1.0;
    weight_set[2] = 2.0;
    weight_set[3] = 1.0;
    weight_set[4] = 2.0;

    Mixture mixture(weight_set);

    ASSERT_EQ(mixture.getMolarMass() > 0.0, true) << "The molar mass is equal to 0.0";
    ASSERT_EQ(mixture.getLabel().size() > 0, true) << "The label is empty";
    ASSERT_EQ(mixture.getLabel(), "H17He33Li17Be33") << "The label is empty";
}


//--------------------------------------
TEST(TestMixture, TestMapStrConstructor)
//--------------------------------------
{
    std::map<std::string, double> weight_set;

    weight_set[ElementSet::getInstance().getElement(1).getSymbol()] = 1.0;
    weight_set[ElementSet::getInstance().getElement(2).getSymbol()] = 2.0;
    weight_set[ElementSet::getInstance().getElement(3).getSymbol()] = 1.0;
    weight_set[ElementSet::getInstance().getElement(4).getSymbol()] = 2.0;

    Mixture mixture(weight_set);

    ASSERT_EQ(mixture.getMolarMass() > 0.0, true) << "The molar mass is equal to 0.0";
    ASSERT_EQ(mixture.getLabel().size() > 0, true) << "The label is empty";
    ASSERT_EQ(mixture.getLabel(), "H17He33Li17Be33") << "The label is empty";
}


//------------------------------------
TEST(TestMixture, TestCopyConstructor)
//------------------------------------
{
    std::map<int, double> weight_set;

    weight_set[1] = 1.0;
    weight_set[2] = 2.0;
    weight_set[3] = 1.0;
    weight_set[4] = 2.0;

    Mixture mixture1(weight_set);
    Mixture mixture2(mixture1);

    ASSERT_EQ(mixture1.getMolarMass(), mixture2.getMolarMass()) << "The molar masses are not equal";
    ASSERT_EQ(mixture1.computeMolarMass(), mixture2.computeMolarMass()) << "The molar masses are not equal";
    ASSERT_EQ(mixture1.getMolarMass(), mixture2.computeMolarMass()) << "The molar masses are not equal";
    ASSERT_EQ(mixture1.computeMolarMass(), mixture2.getMolarMass()) << "The molar masses are not equal";
    
    ASSERT_EQ(mixture1.getLabel(), mixture2.getLabel()) << "The labels are not equal";

    ASSERT_EQ(mixture1.getDensity(), mixture2.getDensity()) << "The labels are not equal";

    ASSERT_EQ(mixture1, mixture2) << "The mixtures are not equal";
}


//-------------------------------
TEST(TestMixture, TestAddElement)
//-------------------------------
{
    std::map<int, double> weight_set;

    weight_set[1] = 1.0;
    weight_set[2] = 2.0;
    weight_set[3] = 1.0;
    weight_set[4] = 2.0;

    Mixture mixture1(weight_set);
    Mixture mixture2;
    mixture2.addElement(ElementSet::getInstance().getElement(1).getSymbol(), 1.0);
    mixture2.addElement(ElementSet::getInstance().getElement(2).getSymbol(), 2.0);
    mixture2.addElement(ElementSet::getInstance().getElement(3).getSymbol(), 1.0);
    mixture2.addElement(ElementSet::getInstance().getElement(4).getSymbol(), 2.0);
    mixture2.normaliseWeightSet();


    ASSERT_EQ(mixture1.getMolarMass(), mixture2.getMolarMass()) << "The molar masses are not equal";
    ASSERT_EQ(mixture1.computeMolarMass(), mixture2.computeMolarMass()) << "The molar masses are not equal";
    ASSERT_EQ(mixture1.getMolarMass(), mixture2.computeMolarMass()) << "The molar masses are not equal";
    ASSERT_EQ(mixture1.computeMolarMass(), mixture2.getMolarMass()) << "The molar masses are not equal";
    
    ASSERT_EQ(mixture1.getLabel(), mixture2.getLabel()) << "The labels are not equal";

    ASSERT_EQ(mixture1.getDensity(), mixture2.getDensity()) << "The labels are not equal";

    ASSERT_EQ(mixture1, mixture2) << "The mixtures are not equal";
}


//--------------------------
TEST(TestMixture, TestClear)
//--------------------------
{
    std::map<int, double> weight_set;

    weight_set[1] = 1.0;
    weight_set[2] = 2.0;
    weight_set[3] = 1.0;
    weight_set[4] = 2.0;

    Mixture mixture(weight_set);
    mixture.clear();

    ASSERT_NEAR(mixture.getMolarMass(), 0.0, 1e-9) << "The molar mass is not equal to 0.0";
    ASSERT_NEAR(mixture.getDensity(), -1.0, 1e-9) << "The density mass is not equal to -1.0";
    ASSERT_EQ(mixture.getLabel().size(), size_t(0)) << "The label is not empty";
}


//----------------------------------
TEST(TestMixture, TestEqualOperator)
//----------------------------------
{
    std::map<int, double> weight_set;

    weight_set[1] = 1.0;
    weight_set[2] = 2.0;
    weight_set[3] = 1.0;
    weight_set[4] = 2.0;

    Mixture mixture1(weight_set);
    Mixture mixture2(mixture1);
    Mixture mixture3;

    ASSERT_EQ(mixture1 == mixture1, true) << "The mixtures are not the same";
    ASSERT_EQ(mixture2 == mixture2, true) << "The mixtures are not the same";
    ASSERT_EQ(mixture1 == mixture2, true) << "The mixtures are not the same";
    ASSERT_EQ(mixture2 == mixture1, true) << "The mixtures are not the same";

    ASSERT_EQ(mixture1 == mixture3, false) << "The mixtures are the same";
    ASSERT_EQ(mixture2 == mixture3, false) << "The mixtures are the same";
    ASSERT_EQ(mixture3 == mixture1, false) << "The mixtures are the same";
    ASSERT_EQ(mixture3 == mixture2, false) << "The mixtures are the same";
}


//-------------------------------------
TEST(TestMixture, TestNotEqualOperator)
//-------------------------------------
{
    std::map<int, double> weight_set;

    weight_set[1] = 1.0;
    weight_set[2] = 2.0;
    weight_set[3] = 1.0;
    weight_set[4] = 2.0;

    Mixture mixture1(weight_set);
    Mixture mixture2(mixture1);
    Mixture mixture3;

    ASSERT_EQ(mixture1 != mixture1, false) << "The mixtures are not the same";
    ASSERT_EQ(mixture2 != mixture2, false) << "The mixtures are not the same";
    ASSERT_EQ(mixture1 != mixture2, false) << "The mixtures are not the same";
    ASSERT_EQ(mixture2 != mixture1, false) << "The mixtures are not the same";

    ASSERT_EQ(mixture1 != mixture3, true) << "The mixtures are the same";
    ASSERT_EQ(mixture2 != mixture3, true) << "The mixtures are the same";
    ASSERT_EQ(mixture3 != mixture1, true) << "The mixtures are the same";
    ASSERT_EQ(mixture3 != mixture2, true) << "The mixtures are the same";
}


//-------------------------------------
TEST(TestMixture, TestGetMassWeightSet)
//-------------------------------------
{
    std::map<int, double> expected_weight_set;

    expected_weight_set[1] = 1.0;
    expected_weight_set[2] = 2.0;
    expected_weight_set[3] = 1.0;
    expected_weight_set[4] = 2.0;

    Mixture mixture(expected_weight_set);

    std::map<int, double> returned_weight_set = mixture.getMassWeightSet();

    ASSERT_EQ(returned_weight_set.size(), size_t(4)) << "The cross section size is not 4";

    ASSERT_NEAR(returned_weight_set[1], 1.0 / 6.0, 1e-9);
    ASSERT_NEAR(returned_weight_set[2], 2.0 / 6.0, 1e-9);
    ASSERT_NEAR(returned_weight_set[3], 1.0 / 6.0, 1e-9);
    ASSERT_NEAR(returned_weight_set[4], 2.0 / 6.0, 1e-9);
}
