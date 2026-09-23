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
*   @file       TestDensity.cxx
*
*   @brief      Unit testing: density,
*               the unit test corresponds to validation_02,
*               Last date of check: Passed test on 08 May 2019.
*
*   @version    1.0
*
*   @date       08/05/2019
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
*               http://www.fpvidal.net/, Apr 201, 20179, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include "gtest/gtest.h"

#ifndef SGVXR_SimpleGVXR_h
#include "SimpleGVXR.h"
#endif


//******************************************************************************
//  Const
//******************************************************************************
const short HU_bone(1330);
const short HU_brain(37);
const short HU_gas(-998);
const short HU_soft_tissue(52);
const short HU_water(0);


//----------------------------
TEST(TestDensity, TestDensity)
//----------------------------
{
    double rho_reference;
    short HU;
    double rho_test;

    // Soft tissues
    HU = -741;
    rho_reference = 0.26;
    rho_test = getDensityFromHU(HU);

    ASSERT_NEAR(1, rho_test/ rho_reference, 0.05);

    HU = -98;
    rho_reference = 0.93;
    rho_test = getDensityFromHU(HU);

    ASSERT_NEAR(1, rho_test/ rho_reference, 0.05);

    HU = -77;
    rho_reference = 0.95;
    rho_test = getDensityFromHU(HU);

    ASSERT_NEAR(1, rho_test/ rho_reference, 0.05);

    HU = -55;
    rho_reference = 0.97;
    rho_test = getDensityFromHU(HU);

    ASSERT_NEAR(1, rho_test/ rho_reference, 0.05);

    HU = 36;
    rho_reference = 1.04;
    rho_test = getDensityFromHU(HU);

    ASSERT_NEAR(1, rho_test/ rho_reference, 0.05);

    HU = 40;
    rho_reference = 1.045;
    rho_test = getDensityFromHU(HU);

    ASSERT_NEAR(1, rho_test/ rho_reference, 0.05);

    HU = 46;
    rho_reference = 1.05;
    rho_test = getDensityFromHU(HU);

    ASSERT_NEAR(1, rho_test/ rho_reference, 0.05);

    // Soft tissues
    HU = 49;
    rho_reference = 1.07;
    rho_test = getDensityFromHU(HU);

    ASSERT_NEAR(1, rho_test/ rho_reference, 0.05);


    HU = 54;
    rho_reference = 1.06;
    rho_test = getDensityFromHU(HU);

    ASSERT_NEAR(1, rho_test/ rho_reference, 0.05);

    HU = 63;
    rho_reference = 1.07;
    rho_test = getDensityFromHU(HU);

    ASSERT_NEAR(1, rho_test/ rho_reference, 0.05);

    HU = 77;
    rho_reference = 1.09;
    rho_test = getDensityFromHU(HU);

    ASSERT_NEAR(1, rho_test/ rho_reference, 0.05);
    HU = 100;
    rho_reference = 1.12;
    rho_test = getDensityFromHU(HU);

    ASSERT_NEAR(1, rho_test/ rho_reference, 0.05);


    // Skeletal tissues
    HU = -49;
    rho_reference = 0.98;
    rho_test = getDensityFromHU(HU);

    ASSERT_NEAR(1, rho_test/ rho_reference, 0.05);

    HU = -22;
    rho_reference = 1.00;
    rho_test = getDensityFromHU(HU);

    ASSERT_NEAR(1, rho_test/ rho_reference, 0.05);

    HU = 11;
    rho_reference = 1.03;
    rho_test = getDensityFromHU(HU);

    ASSERT_NEAR(1, rho_test/ rho_reference, 0.05);

    HU = 102;
    rho_reference = 1.10;
    rho_test = getDensityFromHU(HU);

    ASSERT_NEAR(1, rho_test/ rho_reference, 0.05);

    HU = 385;
    rho_reference = 1.25;
    rho_test = getDensityFromHU(HU);

    ASSERT_NEAR(1, rho_test/ rho_reference, 0.05);

    HU = 454;
    rho_reference = 1.29;
    rho_test = getDensityFromHU(HU);

    ASSERT_NEAR(1, rho_test/ rho_reference, 0.05);

    HU = 466;
    rho_reference = 1.3;
    rho_test = getDensityFromHU(HU);

    ASSERT_NEAR(1, rho_test/ rho_reference, 0.05);

    HU = 1239;
    rho_reference = 1.75;
    rho_test = getDensityFromHU(HU);

    ASSERT_NEAR(1, rho_test/ rho_reference, 0.05);

    HU = 1524;
    rho_reference = 1.92;
    rho_test = getDensityFromHU(HU);

    ASSERT_NEAR(1, rho_test/ rho_reference, 0.05);
}
