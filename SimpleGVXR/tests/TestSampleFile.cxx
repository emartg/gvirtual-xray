/*

Copyright (c) 2026-present, Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), 
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
*   @file       TestSampleFile.cxx
*
*   @brief      Unit testing: Test the functions in ../src/sample.cxx.
*
*   @date       27/04/2026
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
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, Apr 2026-present, version 1.1,
*   BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <cmath>
#include <string>

#include "SimpleGVXR.h"
#include "gtest/gtest.h"

#ifndef SGVXR_Sample_h
#include "sample.h"
#endif


//******************************************************************************
//  Global variables
//******************************************************************************


//******************************************************************************
//  Function declaration
//******************************************************************************




//------------------------------------
TEST(TestSampleFile, elementMolarMass)
//------------------------------------
{
    // Create the sample
    emptyMesh("sample_element");

    setElement("sample_element", "Si");
    ASSERT_NEAR(getElementMolarMass(("Si")), 28.085F, 1e-3);
    ASSERT_NEAR(getEffectiveMolarMass(("sample_element")), 28.085, 1e-3);

    setElement("sample_element", "Fe");
    ASSERT_NEAR(getElementMolarMass(("Fe")), 55.845F, 1e-3);
    ASSERT_NEAR(getEffectiveMolarMass(("sample_element")), 55.845, 1e-3);
}


//----------------------------------------------
TEST(TestSampleFile, compoundEffectiveMolarMass)
//----------------------------------------------
{
    // Create the sample
    emptyMesh("sample_compound");

    // Water
    setCompound("sample_compound", "H2O");
    ASSERT_NEAR(getEffectiveMolarMass(("sample_compound")), 18.015, 1e-2);

    // Silicon carbide
    setCompound("sample_compound", "SiC");
    ASSERT_NEAR(getEffectiveMolarMass(("sample_compound")), 40.096, 1e-2);

    // Table salt
    setCompound("sample_compound", "NaCl");
    ASSERT_NEAR(getEffectiveMolarMass(("sample_compound")), 58.44, 1e-2);

    // Acetic acid
    setCompound("sample_compound", "CH3COOH");
    ASSERT_NEAR(getEffectiveMolarMass(("sample_compound")), 60.052, 1e-2);

    // Sodium bicarbonate
    setCompound("sample_compound", "NaHCO3");
    ASSERT_NEAR(getEffectiveMolarMass(("sample_compound")), 84.0066, 1e-2);
}


//---------------------------------------
TEST(TestSampleFile, elementAtomicNumber)
//---------------------------------------
{
    // Create the sample
    emptyMesh("sample_element");

    setElement("sample_element", "Si");
    ASSERT_EQ(getElementAtomicNumber(("Si")), 14);
    ASSERT_EQ(std::round(getEffectiveAtomicNumber(("sample_element"))), 14);

    setElement("sample_element", "Fe");
    ASSERT_EQ(getElementAtomicNumber(("Fe")), 26);
    ASSERT_EQ(std::round(getEffectiveAtomicNumber(("sample_element"))), 26);
}


//-------------------------------------------------
TEST(TestSampleFile, compoundEffectiveAtomicNumber)
//-------------------------------------------------
{
    // Create the sample
    emptyMesh("sample_compound");

    // See: https://en.wikipedia.org/wiki/Effective_atomic_number_(compounds_and_mixtures)
    setCompound("sample_compound", "H2O");
    ASSERT_NEAR(getEffectiveAtomicNumber(("sample_compound")), 7.42, 1e-2);
}


//------------------------------------------------
TEST(TestSampleFile, mixtureEffectiveAtomicNumber)
//------------------------------------------------
{
    // Create the sample
    emptyMesh("sample_mixture");

    // See table 1 of
    // MURTY, R. Effective Atomic Numbers of Heterogeneous Materials. Nature 207, 398–399
    // (1965). https://doi.org/10.1038/207398a0

    // Plain extruded leaded brass (copper, 80; zinc, 20)
    setMixture("sample_mixture", "Cu80Zn20");
    float Z_extruded_leaded_brass = getEffectiveAtomicNumber(("sample_mixture"));

    // Phosphor-bronze (copper, 89.75; tin, 1O; phosphorus, 0.25)
    std::vector<std::string> p_symbol_set = {"Cu", "Sn", "P"};
    std::vector<double> p_weight_set = {89.75, 10, 0.25};
    setMixture("sample_mixture", p_symbol_set, p_weight_set);
    float Z_phosphor_bronze = getEffectiveAtomicNumber(("sample_mixture"));

    // Carbon steel (iron, 98.95; carbon, 1.05)
    p_symbol_set = {"Fe", "C"};
    p_weight_set = {98.95, 1.05};
    setMixture("sample_mixture", p_symbol_set, p_weight_set);
    float Z_carbon_steel = getEffectiveAtomicNumber(("sample_mixture"));

    // Monel metal (nickel, 60; copper, 6.5; iron, 0.5; manganese, 33)
    p_symbol_set = {"Ni", "Cu", "Fe", "Mn"};
    p_weight_set = {60, 6.5, 0.5, 33};
    setMixture("sample_mixture", p_symbol_set, p_weight_set);
    float Z_monel_metal = getEffectiveAtomicNumber(("sample_mixture"));

    // Concrete (hydrogen, 0.56; oxygen, 49.56; magnesium, 0.24; sodium, 1.71; aluminium, 4.56;
    // silicon, 31.35; sulphur, 0.12; potassium, 1.92; calcium, 8.26; iron, 1.22)
    p_symbol_set = {"H", "O", "Mg", "Na", "Al", "Si", "S", "K", "Ca", "Fe"};
    p_weight_set = {0.56, 49.56, 0.24, 1.71, 4.56, 31.35, 0.12, 1.92, 8.26, 1.22};
    setMixture("sample_mixture", p_symbol_set, p_weight_set);
    float Z_concrete = getEffectiveAtomicNumber(("sample_mixture"));

    // Perspex (carbon, 59.97; oxygen, 31.97; hydrogen, 8.05)
    p_symbol_set = {"C", "O", "H"};
    p_weight_set = {59.97, 31.97, 8.05};
    setMixture("sample_mixture", p_symbol_set, p_weight_set);
    float Z_perspex = getEffectiveAtomicNumber(("sample_mixture"));

    // Sodium iodide (sodium, 15.34; iodine, 84.66)
    p_symbol_set = {"Na", "I"};
    p_weight_set = {15.34, 84.66};
    setMixture("sample_mixture", p_symbol_set, p_weight_set);
    float Z_sodium_iodide = getEffectiveAtomicNumber(("sample_mixture"));

    ASSERT_EQ(Z_extruded_leaded_brass >= 29.20 && Z_extruded_leaded_brass <= 29.21, true);
    ASSERT_EQ(Z_phosphor_bronze >= 30.06 && Z_phosphor_bronze <= 34.36, true);
    ASSERT_EQ(Z_carbon_steel >= 25.79 && Z_carbon_steel <= 25.98, true);
    ASSERT_EQ(Z_monel_metal >= 27.07 && Z_monel_metal <= 27.22, true);
    ASSERT_EQ(Z_concrete >= 11.52 && Z_concrete <= 2914.76, true);
    ASSERT_EQ(Z_perspex >= 6.24 && Z_perspex <= 6.94, true);
    ASSERT_EQ(Z_sodium_iodide >= 46.56 && Z_sodium_iodide <= 53.13, true);
}
