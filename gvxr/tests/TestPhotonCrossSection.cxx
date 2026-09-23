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
 *   @file       TestPhotonCrossSection.cxx
 *
 *   @brief      Unit testing: Make sure the code in gvxr/src/Utilities/PhotonCrossSection.cxx is tested.
 *
 *
 *   @version    1.0
 *
 *   @date       09/04/2024
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
#include <cstdlib>
#include <cmath>

#include "gtest/gtest.h"

#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#ifndef GVXR_PHOTON_CROSS_SECTION_H
#include "gVirtualXRay/PhotonCrossSection.h"
#endif

#ifndef GVXR_MIXTURE_H
#include "gVirtualXRay/Mixture.h"
#endif


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;


//--------------------------------------------------
TEST(TestPhotonCrossSection, TestDefaultConstructor)
//--------------------------------------------------
{
    
    PhotonCrossSection photon_cross_section;

    ASSERT_EQ(photon_cross_section.useHU(), false);
    ASSERT_EQ(photon_cross_section.useHounsfieldUnit(), false);
    ASSERT_EQ(photon_cross_section.useHounsfieldValue(), false);
    ASSERT_EQ(photon_cross_section.useElement(), false);
    ASSERT_EQ(photon_cross_section.useMixture(), false);
    ASSERT_EQ(photon_cross_section.useMassAttenuationCoefficient(), false);
    ASSERT_EQ(photon_cross_section.useLinearAttenuationCoefficient(), false);

    ASSERT_EQ(photon_cross_section.getLabel().size() == 0, true) << "The label is not empty";

    try
    {
        double density = photon_cross_section.getDensity(); // Use a variable to get rid of the compilation  warning
        density *= 1.0; // Get rid of the compilation warning

        FAIL() << "Expected gVirtualXRay::Exception";
    }
    catch(const Exception& err)
    {
        std::string expected_message("Error: No properties set.");
        ASSERT_EQ(std::string(err.what()).substr(0, expected_message.size()), expected_message);
    }
    catch(...)
    {
        FAIL() << "Expected gVirtualXRay::Exception";
    }
}


//--------------------------------------------------
TEST(TestPhotonCrossSection, TestElementConstructor)
//--------------------------------------------------
{
    
    PhotonCrossSection photon_cross_section(82);

    ASSERT_EQ(photon_cross_section.useHU(), false);
    ASSERT_EQ(photon_cross_section.useHounsfieldUnit(), false);
    ASSERT_EQ(photon_cross_section.useHounsfieldValue(), false);
    ASSERT_EQ(photon_cross_section.useElement(), true);
    ASSERT_EQ(photon_cross_section.useMixture(), false);
    ASSERT_EQ(photon_cross_section.useMassAttenuationCoefficient(), false);
    ASSERT_EQ(photon_cross_section.useLinearAttenuationCoefficient(), false);

    ASSERT_EQ(photon_cross_section.getLabel().size() > 0, true) << "The label is empty";
    ASSERT_EQ(photon_cross_section.getLabel(), "Element: Lead") << "The label is not set";

    ASSERT_EQ(photon_cross_section.getDensity() > 0.0, true) << "The density is equal to 0.0";
    ASSERT_EQ(photon_cross_section.getMolarMass() > 0.0, true) << "The molar mass is equal to 0.0";

    double density(photon_cross_section.getDensity() / (g/cm3));
    double relative_error(abs(density - 11.35) / 11.35);
    ASSERT_NEAR(relative_error, 0.0, 0.01) << "The density is not set";

    ASSERT_NEAR(photon_cross_section.getMolarMass(), 207.2, 1e-9) << "The density is not set";
}


//--------------------------------------------------
TEST(TestPhotonCrossSection, TestMixtureConstructor)
//--------------------------------------------------
{
    std::map<std::string, double> weight_set;

    weight_set["Ti"] = 90;
    weight_set["Al"] = 6;
    weight_set["V"] = 4;

    double expected_density = 4.43 * g / cm3;
    double expected_molar_mass = 46.736852 * g / mole;

    Mixture mixture(weight_set);
    mixture.setDensity(expected_density);

    PhotonCrossSection photon_cross_section(mixture);

    ASSERT_EQ(photon_cross_section.useHU(), false);
    ASSERT_EQ(photon_cross_section.useHounsfieldUnit(), false);
    ASSERT_EQ(photon_cross_section.useHounsfieldValue(), false);
    ASSERT_EQ(photon_cross_section.useElement(), false);
    ASSERT_EQ(photon_cross_section.useMixture(), true);
    ASSERT_EQ(photon_cross_section.useMassAttenuationCoefficient(), false);
    ASSERT_EQ(photon_cross_section.useLinearAttenuationCoefficient(), false);

    ASSERT_EQ(photon_cross_section.getLabel().size() > 0, true) << "The label is empty";
    ASSERT_EQ(photon_cross_section.getLabel(), "Mixture: Al6Ti90V4") << "The label is not set";

    ASSERT_NEAR(photon_cross_section.getDensity(), expected_density, 1e-9) << "The density is set";

    double molar_mass(photon_cross_section.getMolarMass() / (g/mole));
    double relative_error(abs(molar_mass - expected_molar_mass) / expected_molar_mass);
    ASSERT_NEAR(relative_error, 0.0, 0.01) << "The molar mass is not set properly, " << expected_molar_mass << " g/mole was expected, " << molar_mass << " g/mole was computed.";
}


//-------------------------------------------------
TEST(TestPhotonCrossSection, TestMapIntConstructor)
//-------------------------------------------------
{
    std::map<int, double> weight_set;

    weight_set[22] = 90;
    weight_set[13] = 6;
    weight_set[23] = 4;

    double expected_density = 4.43 * g / cm3;
    double expected_molar_mass = 46.736852 * g / mole;

    PhotonCrossSection photon_cross_section(weight_set);
    photon_cross_section.setDensity(expected_density);

    ASSERT_EQ(photon_cross_section.useHU(), false);
    ASSERT_EQ(photon_cross_section.useHounsfieldUnit(), false);
    ASSERT_EQ(photon_cross_section.useHounsfieldValue(), false);
    ASSERT_EQ(photon_cross_section.useElement(), false);
    ASSERT_EQ(photon_cross_section.useMixture(), true);
    ASSERT_EQ(photon_cross_section.useMassAttenuationCoefficient(), false);
    ASSERT_EQ(photon_cross_section.useLinearAttenuationCoefficient(), false);

    ASSERT_EQ(photon_cross_section.getLabel().size() > 0, true) << "The label is empty";
    ASSERT_EQ(photon_cross_section.getLabel(), "Mixture: Al6Ti90V4") << "The label is not set";

    ASSERT_NEAR(photon_cross_section.getDensity(), expected_density, 1e-9) << "The density is set";

    double molar_mass(photon_cross_section.getMolarMass() / (g/mole));
    double relative_error(abs(molar_mass - molar_mass) / molar_mass);
    ASSERT_NEAR(relative_error, 0.0, 0.01) << "The molar mass is not set properly, " << expected_molar_mass << " g/mole was expected, " << molar_mass << " g/mole was computed.";
}


//-------------------------------------------------
TEST(TestPhotonCrossSection, TestMapStrConstructor)
//-------------------------------------------------
{
    std::map<std::string, double> weight_set;

    weight_set["Ti"] = 90;
    weight_set["Al"] = 6;
    weight_set["V"] = 4;

    double expected_density = 4.43 * g / cm3;
    double expected_molar_mass = 46.736852 * g / mole;

    PhotonCrossSection photon_cross_section(weight_set);
    photon_cross_section.setDensity(expected_density);

    ASSERT_EQ(photon_cross_section.useHU(), false);
    ASSERT_EQ(photon_cross_section.useHounsfieldUnit(), false);
    ASSERT_EQ(photon_cross_section.useHounsfieldValue(), false);
    ASSERT_EQ(photon_cross_section.useElement(), false);
    ASSERT_EQ(photon_cross_section.useMixture(), true);
    ASSERT_EQ(photon_cross_section.useMassAttenuationCoefficient(), false);
    ASSERT_EQ(photon_cross_section.useLinearAttenuationCoefficient(), false);

    ASSERT_EQ(photon_cross_section.getLabel().size() > 0, true) << "The label is empty";
    ASSERT_EQ(photon_cross_section.getLabel(), "Mixture: Al6Ti90V4") << "The label is not set";

    ASSERT_NEAR(photon_cross_section.getDensity(), expected_density, 1e-9) << "The density is set";

    double molar_mass(photon_cross_section.getMolarMass() / (g/mole));
    double relative_error(abs(molar_mass - expected_molar_mass) / expected_molar_mass);
    ASSERT_NEAR(relative_error, 0.0, 0.01) << "The molar mass is not set properly, " << expected_molar_mass << " g/mole was expected, " << molar_mass << " g/mole was computed.";
}


//-------------------------------------
TEST(TestPhotonCrossSection, TestReset)
//-------------------------------------
{
    std::map<std::string, double> weight_set;

    weight_set["Ti"] = 90;
    weight_set["Al"] = 6;
    weight_set["V"] = 4;

    double expected_density = 4.43 * g / cm3;
    // double expected_molar_mass = 46.736852 * g / mole;

    PhotonCrossSection photon_cross_section(weight_set);
    photon_cross_section.setDensity(expected_density);

    photon_cross_section.reset();

    ASSERT_EQ(photon_cross_section.useHU(), false);
    ASSERT_EQ(photon_cross_section.useHounsfieldUnit(), false);
    ASSERT_EQ(photon_cross_section.useHounsfieldValue(), false);
    ASSERT_EQ(photon_cross_section.useElement(), false);
    ASSERT_EQ(photon_cross_section.useMixture(), false);
    ASSERT_EQ(photon_cross_section.useMassAttenuationCoefficient(), false);
    ASSERT_EQ(photon_cross_section.useLinearAttenuationCoefficient(), false);

    ASSERT_EQ(photon_cross_section.getLabel().size() == 0, true) << "The label is not empty";

    try
    {
        double density = photon_cross_section.getDensity(); // Use a variable to get rid of the compilation  warning
        density *= 1.0; // Get rid of the compilation warning

        FAIL() << "Expected gVirtualXRay::Exception";
    }
    catch(const Exception& err)
    {
        std::string expected_message("Error: No properties set.");
        ASSERT_EQ(std::string(err.what()).substr(0, expected_message.size()), expected_message);
    }
    catch(...)
    {
        FAIL() << "Expected gVirtualXRay::Exception";
    }
}


//-------------------------------------------
TEST(TestPhotonCrossSection, TestUseElementZ)
//-------------------------------------------
{
    
    PhotonCrossSection photon_cross_section;
    photon_cross_section.useElement(82);

    ASSERT_EQ(photon_cross_section.useHU(), false);
    ASSERT_EQ(photon_cross_section.useHounsfieldUnit(), false);
    ASSERT_EQ(photon_cross_section.useHounsfieldValue(), false);
    ASSERT_EQ(photon_cross_section.useElement(), true);
    ASSERT_EQ(photon_cross_section.useMixture(), false);
    ASSERT_EQ(photon_cross_section.useMassAttenuationCoefficient(), false);
    ASSERT_EQ(photon_cross_section.useLinearAttenuationCoefficient(), false);

    ASSERT_EQ(photon_cross_section.getLabel().size() > 0, true) << "The label is empty";
    ASSERT_EQ(photon_cross_section.getLabel(), "Element: Lead") << "The label is not set";

    ASSERT_EQ(photon_cross_section.getDensity() > 0.0, true) << "The density is equal to 0.0";
    ASSERT_EQ(photon_cross_section.getMolarMass() > 0.0, true) << "The molar mass is equal to 0.0";

    double density(photon_cross_section.getDensity() / (g/cm3));
    double relative_error(abs(density - 11.35) / 11.35);
    ASSERT_NEAR(relative_error, 0.0, 0.01) << "The density is not set";

    ASSERT_NEAR(photon_cross_section.getMolarMass(), 207.2, 1e-9) << "The density is not set";

    try
    {
        photon_cross_section.useElement(10000);

        FAIL() << "Expected gVirtualXRay::Exception";
    }
    catch(const Exception& err)
    {
        std::string expected_message("Error: Element (atomic number: 10000) not found.");
        ASSERT_EQ(std::string(err.what()).substr(0, expected_message.size()), expected_message);
    }
    catch(...)
    {
        FAIL() << "Expected gVirtualXRay::Exception";
    }
}


//---------------------------------------------
TEST(TestPhotonCrossSection, TestUseElementStr)
//---------------------------------------------
{
    std::string symbol("Pb");

    PhotonCrossSection photon_cross_section;
    photon_cross_section.useElement(symbol);

    ASSERT_EQ(photon_cross_section.useHU(), false);
    ASSERT_EQ(photon_cross_section.useHounsfieldUnit(), false);
    ASSERT_EQ(photon_cross_section.useHounsfieldValue(), false);
    ASSERT_EQ(photon_cross_section.useElement(), true);
    ASSERT_EQ(photon_cross_section.useMixture(), false);
    ASSERT_EQ(photon_cross_section.useMassAttenuationCoefficient(), false);
    ASSERT_EQ(photon_cross_section.useLinearAttenuationCoefficient(), false);

    ASSERT_EQ(photon_cross_section.getLabel().size() > 0, true) << "The label is empty";
    ASSERT_EQ(photon_cross_section.getLabel(), "Element: " + symbol) << "The label is not set";

    ASSERT_EQ(photon_cross_section.getDensity() > 0.0, true) << "The density is equal to 0.0";
    ASSERT_EQ(photon_cross_section.getMolarMass() > 0.0, true) << "The molar mass is equal to 0.0";

    double density(photon_cross_section.getDensity() / (g/cm3));
    double relative_error(abs(density - 11.35) / 11.35);
    ASSERT_NEAR(relative_error, 0.0, 0.01) << "The density is not set";

    ASSERT_NEAR(photon_cross_section.getMolarMass(), 207.2, 1e-9) << "The density is not set";

    try
    {
        photon_cross_section.useElement("test");

        FAIL() << "Expected gVirtualXRay::Exception";
    }
    catch(const Exception& err)
    {
        std::string expected_message("Error: Element (name: test) not found.");
        ASSERT_EQ(std::string(err.what()).substr(0, expected_message.size()), expected_message);
    }
    catch(...)
    {
        FAIL() << "Expected gVirtualXRay::Exception";
    }
}


//------------------------------------------
TEST(TestPhotonCrossSection, TestUseMixture)
//------------------------------------------
{
    std::map<std::string, double> weight_set;

    weight_set["Ti"] = 90;
    weight_set["Al"] = 6;
    weight_set["V"] = 4;

    double expected_density = 4.43 * g / cm3;
    double expected_molar_mass = 46.736852 * g / mole;

    Mixture mixture(weight_set);
    mixture.setDensity(expected_density);

    PhotonCrossSection photon_cross_section;
    photon_cross_section.useMixture(mixture);

    ASSERT_EQ(photon_cross_section.useHU(), false);
    ASSERT_EQ(photon_cross_section.useHounsfieldUnit(), false);
    ASSERT_EQ(photon_cross_section.useHounsfieldValue(), false);
    ASSERT_EQ(photon_cross_section.useElement(), false);
    ASSERT_EQ(photon_cross_section.useMixture(), true);
    ASSERT_EQ(photon_cross_section.useMassAttenuationCoefficient(), false);
    ASSERT_EQ(photon_cross_section.useLinearAttenuationCoefficient(), false);

    ASSERT_EQ(photon_cross_section.getLabel().size() > 0, true) << "The label is empty";
    ASSERT_EQ(photon_cross_section.getLabel(), "Mixture: Al6Ti90V4") << "The label is not set";

    ASSERT_NEAR(photon_cross_section.getDensity(), expected_density, 1e-9) << "The density is set";

    double molar_mass(photon_cross_section.getMolarMass() / (g/mole));
    double relative_error(abs(molar_mass - expected_molar_mass) / expected_molar_mass);
    ASSERT_NEAR(relative_error, 0.0, 0.01) << "The molar mass is not set properly, " << expected_molar_mass << " g/mole was expected, " << molar_mass << " g/mole was computed.";

    try
    {
        photon_cross_section.useMixture("Test");

        FAIL() << "Expected gVirtualXRay::Exception";
    }
    catch(const Exception& err)
    {
        std::string expected_message("Error: Element (name: Test) not found.");
        ASSERT_EQ(std::string(err.what()).substr(0, expected_message.size()), expected_message);
    }
    catch(...)
    {
        FAIL() << "Expected gVirtualXRay::Exception";
    }


    try
    {
        photon_cross_section.useMixture("10Test");

        FAIL() << "Expected gVirtualXRay::Exception";
    }
    catch(const Exception& err)
    {
        std::string expected_message("Error: Element (name: ) not found.");
        ASSERT_EQ(std::string(err.what()).substr(0, expected_message.size()), expected_message);
    }
    catch(...)
    {
        FAIL() << "Expected gVirtualXRay::Exception";
    }
}


//------------------------------------------------
TEST(TestPhotonCrossSection, TestUseMistureMapInt)
//------------------------------------------------
{
    std::map<int, double> weight_set;

    weight_set[22] = 90;
    weight_set[13] = 6;
    weight_set[23] = 4;

    double expected_density = 4.43 * g / cm3;
    double expected_molar_mass = 46.736852 * g / mole;

    PhotonCrossSection photon_cross_section;
    photon_cross_section.useMixture(weight_set);
    photon_cross_section.setDensity(expected_density);

    ASSERT_EQ(photon_cross_section.useHU(), false);
    ASSERT_EQ(photon_cross_section.useHounsfieldUnit(), false);
    ASSERT_EQ(photon_cross_section.useHounsfieldValue(), false);
    ASSERT_EQ(photon_cross_section.useElement(), false);
    ASSERT_EQ(photon_cross_section.useMixture(), true);
    ASSERT_EQ(photon_cross_section.useMassAttenuationCoefficient(), false);
    ASSERT_EQ(photon_cross_section.useLinearAttenuationCoefficient(), false);

    ASSERT_EQ(photon_cross_section.getLabel().size() > 0, true) << "The label is empty";
    ASSERT_EQ(photon_cross_section.getLabel(), "Mixture: Al6Ti90V4") << "The label is not set";

    ASSERT_NEAR(photon_cross_section.getDensity(), expected_density, 1e-9) << "The density is set";

    double molar_mass(photon_cross_section.getMolarMass() / (g/mole));
    double relative_error(abs(molar_mass - molar_mass) / molar_mass);
    ASSERT_NEAR(relative_error, 0.0, 0.01) << "The molar mass is not set properly, " << expected_molar_mass << " g/mole was expected, " << molar_mass << " g/mole was computed.";
}


//------------------------------------------------
TEST(TestPhotonCrossSection, TestUseMistureMapStr)
//------------------------------------------------
{
    std::map<std::string, double> weight_set;

    weight_set["Ti"] = 90;
    weight_set["Al"] = 6;
    weight_set["V"] = 4;

    double expected_density = 4.43 * g / cm3;
    double expected_molar_mass = 46.736852 * g / mole;

    PhotonCrossSection photon_cross_section;
    photon_cross_section.useMixture(weight_set);
    photon_cross_section.setDensity(expected_density);

    ASSERT_EQ(photon_cross_section.useHU(), false);
    ASSERT_EQ(photon_cross_section.useHounsfieldUnit(), false);
    ASSERT_EQ(photon_cross_section.useHounsfieldValue(), false);
    ASSERT_EQ(photon_cross_section.useElement(), false);
    ASSERT_EQ(photon_cross_section.useMixture(), true);
    ASSERT_EQ(photon_cross_section.useMassAttenuationCoefficient(), false);
    ASSERT_EQ(photon_cross_section.useLinearAttenuationCoefficient(), false);

    ASSERT_EQ(photon_cross_section.getLabel().size() > 0, true) << "The label is empty";
    ASSERT_EQ(photon_cross_section.getLabel(), "Mixture: Al6Ti90V4") << "The label is not set";

    ASSERT_NEAR(photon_cross_section.getDensity(), expected_density, 1e-9) << "The density is set";

    double molar_mass(photon_cross_section.getMolarMass() / (g/mole));
    double relative_error(abs(molar_mass - expected_molar_mass) / expected_molar_mass);
    ASSERT_NEAR(relative_error, 0.0, 0.01) << "The molar mass is not set properly, " << expected_molar_mass << " g/mole was expected, " << molar_mass << " g/mole was computed.";
}


//-------------------------------------------
TEST(TestPhotonCrossSection, TestUseCompound)
//-------------------------------------------
{
    double expected_density = 3.21 * g / cm3;
    double expected_molar_mass = 40.11 * g / mole;

    PhotonCrossSection photon_cross_section;
    photon_cross_section.useCompound("SiC");
    photon_cross_section.setDensity(expected_density);

    ASSERT_EQ(photon_cross_section.useHU(), false);
    ASSERT_EQ(photon_cross_section.useHounsfieldUnit(), false);
    ASSERT_EQ(photon_cross_section.useHounsfieldValue(), false);
    ASSERT_EQ(photon_cross_section.useElement(), false);
    ASSERT_EQ(photon_cross_section.useMixture(), true);
    ASSERT_EQ(photon_cross_section.useMassAttenuationCoefficient(), false);
    ASSERT_EQ(photon_cross_section.useLinearAttenuationCoefficient(), false);

    ASSERT_EQ(photon_cross_section.getLabel().size() > 0, true) << "The label is empty";
    ASSERT_EQ(photon_cross_section.getLabel(), "Compound: SiC") << "The label is not set";

    ASSERT_NEAR(photon_cross_section.getDensity(), expected_density, 1e-9) << "The density is set";

    double molar_mass(photon_cross_section.getMolarMass() / (g/mole));
    double relative_error(abs(molar_mass - expected_molar_mass) / expected_molar_mass);

    std::cout << molar_mass << "\t" << expected_molar_mass << std::endl;
    ASSERT_NEAR(relative_error, 0.0, 0.01) << "The molar mass is not set properly, " << expected_molar_mass << " g/mole was expected, " << molar_mass << " g/mole was computed.";

    try
    {
        photon_cross_section.useCompound("Test");

        FAIL() << "Expected gVirtualXRay::Exception";
    }
    catch(const Exception& err)
    {
        std::string expected_message("Error: Element (name: Test) not found.");
        ASSERT_EQ(std::string(err.what()).substr(0, expected_message.size()), expected_message);
    }
    catch(...)
    {
        FAIL() << "Expected gVirtualXRay::Exception";
    }

    try
    {
        photon_cross_section.useCompound("10Test");

        FAIL() << "Expected gVirtualXRay::Exception";
    }
    catch(const Exception& err)
    {
        std::string expected_message("Error: Element (name: Test) not found.");
        ASSERT_EQ(std::string(err.what()).substr(0, expected_message.size()), expected_message);
    }
    catch(...)
    {
        FAIL() << "Expected gVirtualXRay::Exception";
    }
}


//-------------------------------------------------------------
TEST(TestPhotonCrossSection, TestUseMassAttenuationCoefficient)
//-------------------------------------------------------------
{
    PhotonCrossSection photon_cross_section;
    photon_cross_section.useMassAttenuationCoefficient(1.0);

    ASSERT_EQ(photon_cross_section.useHU(), false);
    ASSERT_EQ(photon_cross_section.useHounsfieldUnit(), false);
    ASSERT_EQ(photon_cross_section.useHounsfieldValue(), false);
    ASSERT_EQ(photon_cross_section.useElement(), false);
    ASSERT_EQ(photon_cross_section.useMixture(), false);
    ASSERT_EQ(photon_cross_section.useMassAttenuationCoefficient(), true);
    ASSERT_EQ(photon_cross_section.useLinearAttenuationCoefficient(), false);

    ASSERT_EQ(photon_cross_section.getLabel().size() > 0, true) << "The label is empty";
    ASSERT_EQ(photon_cross_section.getLabel(), "mu/rho: 1.000000") << "The label is not set";
}


//---------------------------------------------------------------
TEST(TestPhotonCrossSection, TestUseLinearAttenuationCoefficient)
//---------------------------------------------------------------
{
    PhotonCrossSection photon_cross_section;
    photon_cross_section.useLinearAttenuationCoefficient(10.0 * (1.0 / cm));

    ASSERT_EQ(photon_cross_section.useHU(), false);
    ASSERT_EQ(photon_cross_section.useHounsfieldUnit(), false);
    ASSERT_EQ(photon_cross_section.useHounsfieldValue(), false);
    ASSERT_EQ(photon_cross_section.useElement(), false);
    ASSERT_EQ(photon_cross_section.useMixture(), false);
    ASSERT_EQ(photon_cross_section.useMassAttenuationCoefficient(), false);
    ASSERT_EQ(photon_cross_section.useLinearAttenuationCoefficient(), true);

    ASSERT_EQ(photon_cross_section.getLabel().size() > 0, true) << "The label is empty";
    ASSERT_EQ(photon_cross_section.getLabel(), "mu: 10.000000 cm-1") << "The label is not set";
}


//-------------------------------------
TEST(TestPhotonCrossSection, TestUseHU)
//-------------------------------------
{
    PhotonCrossSection photon_cross_section;
    photon_cross_section.useHU(1000);

    ASSERT_EQ(photon_cross_section.useHU(), true);
    ASSERT_EQ(photon_cross_section.useHounsfieldUnit(), true);
    ASSERT_EQ(photon_cross_section.useHounsfieldValue(), true);
    ASSERT_EQ(photon_cross_section.useElement(), false);
    ASSERT_EQ(photon_cross_section.useMixture(), false);
    ASSERT_EQ(photon_cross_section.useMassAttenuationCoefficient(), false);
    ASSERT_EQ(photon_cross_section.useLinearAttenuationCoefficient(), false);

    ASSERT_EQ(photon_cross_section.getLabel().size() > 0, true) << "The label is empty";
    ASSERT_EQ(photon_cross_section.getLabel(), "HU: 1000") << "The label is not set";
}


//-------------------------------------------------
TEST(TestPhotonCrossSection, TestUseHounsfieldUnit)
//-------------------------------------------------
{
    PhotonCrossSection photon_cross_section;
    photon_cross_section.useHounsfieldUnit(1000);

    ASSERT_EQ(photon_cross_section.useHU(), true);
    ASSERT_EQ(photon_cross_section.useHounsfieldUnit(), true);
    ASSERT_EQ(photon_cross_section.useHounsfieldValue(), true);
    ASSERT_EQ(photon_cross_section.useElement(), false);
    ASSERT_EQ(photon_cross_section.useMixture(), false);
    ASSERT_EQ(photon_cross_section.useMassAttenuationCoefficient(), false);
    ASSERT_EQ(photon_cross_section.useLinearAttenuationCoefficient(), false);

    ASSERT_EQ(photon_cross_section.getLabel().size() > 0, true) << "The label is empty";
    ASSERT_EQ(photon_cross_section.getLabel(), "HU: 1000") << "The label is not set";
}


//--------------------------------------------------
TEST(TestPhotonCrossSection, TestUseHounsfieldValue)
//--------------------------------------------------
{
    PhotonCrossSection photon_cross_section;
    photon_cross_section.useHounsfieldValue(1000);

    ASSERT_EQ(photon_cross_section.useHU(), true);
    ASSERT_EQ(photon_cross_section.useHounsfieldUnit(), true);
    ASSERT_EQ(photon_cross_section.useHounsfieldValue(), true);
    ASSERT_EQ(photon_cross_section.useElement(), false);
    ASSERT_EQ(photon_cross_section.useMixture(), false);
    ASSERT_EQ(photon_cross_section.useMassAttenuationCoefficient(), false);
    ASSERT_EQ(photon_cross_section.useLinearAttenuationCoefficient(), false);

    ASSERT_EQ(photon_cross_section.getLabel().size() > 0, true) << "The label is empty";
    ASSERT_EQ(photon_cross_section.getLabel(), "HU: 1000") << "The label is not set";
}


//---------------------------------------------
TEST(TestPhotonCrossSection, TestEqualOperator)
//---------------------------------------------
{
    std::map<std::string, double> weight_set;

    weight_set["Ti"] = 90;
    weight_set["Al"] = 6;
    weight_set["V"] = 4;

    double expected_density = 4.43 * g / cm3;

    PhotonCrossSection mixture1;
    mixture1.useMixture(weight_set);
    mixture1.setDensity(expected_density);

    PhotonCrossSection mixture2(mixture1);
    PhotonCrossSection mixture3;
    mixture3 = mixture1;

    PhotonCrossSection mixture4;

    PhotonCrossSection mixture5;
    mixture5.useHU(1000);

    PhotonCrossSection mixture6;
    mixture6.useElement(1);

    ASSERT_EQ(mixture1 == mixture1, true) << "The cross sections are not the same";
    ASSERT_EQ(mixture1 == mixture2, true) << "The cross sections are not the same";
    ASSERT_EQ(mixture1 == mixture3, true) << "The cross sections are not the same";
    ASSERT_EQ(mixture1 == mixture4, false) << "The cross sections are the same";
    ASSERT_EQ(mixture1 == mixture5, false) << "The cross sections are the same";
    ASSERT_EQ(mixture1 == mixture6, false) << "The cross sections are the same";

    ASSERT_EQ(mixture2 == mixture1, true) << "The cross sections are not the same";
    ASSERT_EQ(mixture2 == mixture2, true) << "The cross sections are not the same";
    ASSERT_EQ(mixture2 == mixture3, true) << "The cross sections are not the same";
    ASSERT_EQ(mixture2 == mixture4, false) << "The cross sections are the same";
    ASSERT_EQ(mixture2 == mixture5, false) << "The cross sections are the same";
    ASSERT_EQ(mixture2 == mixture6, false) << "The cross sections are the same";

    ASSERT_EQ(mixture3 == mixture1, true) << "The cross sections are not the same";
    ASSERT_EQ(mixture3 == mixture2, true) << "The cross sections are not the same";
    ASSERT_EQ(mixture3 == mixture3, true) << "The cross sections are not the same";
    ASSERT_EQ(mixture3 == mixture4, false) << "The cross sections are the same";
    ASSERT_EQ(mixture3 == mixture5, false) << "The cross sections are the same";
    ASSERT_EQ(mixture3 == mixture6, false) << "The cross sections are the same";

    ASSERT_EQ(mixture4 == mixture1, false) << "The cross sections are the same";
    ASSERT_EQ(mixture4 == mixture2, false) << "The cross sections are the same";
    ASSERT_EQ(mixture4 == mixture3, false) << "The cross sections are the same";
    ASSERT_EQ(mixture4 == mixture4, true) << "The cross sections are not the same";
    ASSERT_EQ(mixture4 == mixture5, false) << "The cross sections are the same";
    ASSERT_EQ(mixture4 == mixture6, false) << "The cross sections are the same";

    ASSERT_EQ(mixture5 == mixture1, false) << "The cross sections are the same";
    ASSERT_EQ(mixture5 == mixture2, false) << "The cross sections are the same";
    ASSERT_EQ(mixture5 == mixture3, false) << "The cross sections are the same";
    ASSERT_EQ(mixture5 == mixture4, false) << "The cross sections are the same";
    ASSERT_EQ(mixture5 == mixture5, true) << "The cross sections are not the same";
    ASSERT_EQ(mixture5 == mixture6, false) << "The cross sections are the same";

    ASSERT_EQ(mixture6 == mixture1, false) << "The cross sections are the same";
    ASSERT_EQ(mixture6 == mixture2, false) << "The cross sections are the same";
    ASSERT_EQ(mixture6 == mixture3, false) << "The cross sections are the same";
    ASSERT_EQ(mixture6 == mixture4, false) << "The cross sections are the same";
    ASSERT_EQ(mixture6 == mixture5, false) << "The cross sections are the same";
    ASSERT_EQ(mixture6 == mixture6, true) << "The cross sections are not the same";
}


//------------------------------------------------
TEST(TestPhotonCrossSection, TestNotEqualOperator)
//------------------------------------------------
{
    std::map<std::string, double> weight_set;

    weight_set["Ti"] = 90;
    weight_set["Al"] = 6;
    weight_set["V"] = 4;

    double expected_density = 4.43 * g / cm3;

    PhotonCrossSection mixture1;
    mixture1.useMixture(weight_set);
    mixture1.setDensity(expected_density);

    PhotonCrossSection mixture2(mixture1);
    PhotonCrossSection mixture3;
    mixture3 = mixture1;

    PhotonCrossSection mixture4;

    PhotonCrossSection mixture5;
    mixture5.useHU(1000);

    PhotonCrossSection mixture6;
    mixture6.useElement(1);

    ASSERT_EQ(mixture1 != mixture1, false) << "The cross sections are not the same";
    ASSERT_EQ(mixture1 != mixture2, false) << "The cross sections are not the same";
    ASSERT_EQ(mixture1 != mixture3, false) << "The cross sections are not the same";
    ASSERT_EQ(mixture1 != mixture4, true) << "The cross sections are the same";
    ASSERT_EQ(mixture1 != mixture5, true) << "The cross sections are the same";
    ASSERT_EQ(mixture1 != mixture6, true) << "The cross sections are the same";

    ASSERT_EQ(mixture2 != mixture1, false) << "The cross sections are not the same";
    ASSERT_EQ(mixture2 != mixture2, false) << "The cross sections are not the same";
    ASSERT_EQ(mixture2 != mixture3, false) << "The cross sections are not the same";
    ASSERT_EQ(mixture2 != mixture4, true) << "The cross sections are the same";
    ASSERT_EQ(mixture2 != mixture5, true) << "The cross sections are the same";
    ASSERT_EQ(mixture2 != mixture6, true) << "The cross sections are the same";

    ASSERT_EQ(mixture3 != mixture1, false) << "The cross sections are not the same";
    ASSERT_EQ(mixture3 != mixture2, false) << "The cross sections are not the same";
    ASSERT_EQ(mixture3 != mixture3, false) << "The cross sections are not the same";
    ASSERT_EQ(mixture3 != mixture4, true) << "The cross sections are the same";
    ASSERT_EQ(mixture3 != mixture5, true) << "The cross sections are the same";
    ASSERT_EQ(mixture3 != mixture6, true) << "The cross sections are the same";

    ASSERT_EQ(mixture4 != mixture1, true) << "The cross sections are the same";
    ASSERT_EQ(mixture4 != mixture2, true) << "The cross sections are the same";
    ASSERT_EQ(mixture4 != mixture3, true) << "The cross sections are the same";
    ASSERT_EQ(mixture4 != mixture4, false) << "The cross sections are not the same";
    ASSERT_EQ(mixture4 != mixture5, true) << "The cross sections are the same";
    ASSERT_EQ(mixture4 != mixture6, true) << "The cross sections are the same";

    ASSERT_EQ(mixture5 != mixture1, true) << "The cross sections are the same";
    ASSERT_EQ(mixture5 != mixture2, true) << "The cross sections are the same";
    ASSERT_EQ(mixture5 != mixture3, true) << "The cross sections are the same";
    ASSERT_EQ(mixture5 != mixture4, true) << "The cross sections are the same";
    ASSERT_EQ(mixture5 != mixture5, false) << "The cross sections are not the same";
    ASSERT_EQ(mixture5 != mixture6, true) << "The cross sections are the same";

    ASSERT_EQ(mixture6 != mixture1, true) << "The cross sections are the same";
    ASSERT_EQ(mixture6 != mixture2, true) << "The cross sections are the same";
    ASSERT_EQ(mixture6 != mixture3, true) << "The cross sections are the same";
    ASSERT_EQ(mixture6 != mixture4, true) << "The cross sections are the same";
    ASSERT_EQ(mixture6 != mixture5, true) << "The cross sections are the same";
    ASSERT_EQ(mixture6 != mixture6, false) << "The cross sections are not the same";
}


//---------------------------------------------------
TEST(TestPhotonCrossSection, TestGetMixtureWeightSet)
//---------------------------------------------------
{
    std::map<int, double> weight_set = PhotonCrossSection::getMixtureWeightSet("Ti90Al6V4");

    ASSERT_EQ(weight_set.size(), size_t(3)) << "The cross section size is not 3";

    ASSERT_EQ(weight_set[22], 90) << "The percentage of Ti is not 90";
    ASSERT_EQ(weight_set[13], 6) << "The percentage of Al is not 6";
    ASSERT_EQ(weight_set[23], 4) << "The percentage of V is not 4";
}


//----------------------------------------------------
TEST(TestPhotonCrossSection, TestGetCompoundWeightSet)
//----------------------------------------------------
{
    std::map<int, double> weight_set = PhotonCrossSection::getCompoundWeightSet("SiC");

    ASSERT_EQ(weight_set.size(), size_t(2)) << "The cross section size is not 3";

    ASSERT_EQ(round(100 * weight_set[6]), 30) << "The percentage of C is not 30";
    ASSERT_EQ(round(100 * weight_set[14]), 70) << "The percentage of Si is not 70";
}
