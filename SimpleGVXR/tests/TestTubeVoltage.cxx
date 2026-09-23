/*

Copyright (c) 2023, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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
*   @file       TestTubeVoltage.cxx
*
*   @brief      Unit testing: test the tube voltage,
*   			last date of check: Passed test on 20th Sept 2023.
*
*   @version    1.0
*
*   @date       28/09/2023
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
*               http://www.fpvidal.net/, Sept 2023, 2023, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <cmath>
#include <fstream>
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
double g_voltage = 100;


//******************************************************************************
//  Function declaration
//******************************************************************************
void runTest(const std::string& aFileName);



void runTest(const std::string& aFileName)
{
    // Open the files
    std::ifstream reference_data_stream(aFileName);

    // The file is not open
    if (!reference_data_stream.is_open())
    {
        std::string error_message;
        error_message += "\"";
        error_message += aFileName;
        error_message += "\" does not exist";

        FAIL() << error_message;
    }

    double count, energy;
    int i = 0;
    bool has_swapped = false;
    while (reference_data_stream.good() && i < getPhotonCountEnergyBins().size())
    {
        double ref_count, ref_energy;
        reference_data_stream >> ref_energy >> ref_count;

        if (ref_energy > 0.5 && ref_count > 1e-7)
        {
            double test_count;
            double test_energy;
            if (has_swapped)
            {
                has_swapped = false;
                test_count = getPhotonCountEnergyBins()[i-1];
                test_energy = getEnergyBins("keV")[i-1];
            } 
            else
            {
                test_count = getPhotonCountEnergyBins()[i];
                test_energy = getEnergyBins("keV")[i];
            }
            

            double relative_error_count  = 100 * abs((ref_count - test_count) / ref_count);
            double relative_error_energy = 100 * abs((ref_energy - test_energy) / ref_energy);

            if (relative_error_count > 0.01)
            {
                if (!has_swapped)
                {
                    test_count = getPhotonCountEnergyBins()[i+1];
                    test_energy = getEnergyBins("keV")[i+1];

                    relative_error_count  = 100 * abs((ref_count - test_count) / ref_count);
                    relative_error_energy = 100 * abs((ref_energy - test_energy) / ref_energy);

                    has_swapped = true;
                }
            }
            i++;

            if (relative_error_count > 0.01 || relative_error_energy > 0.01)
            {
                gVirtualXRay::LOGGER.logNow() << i << "\t" << 
                    relative_error_energy << "\t" << 
                    relative_error_count << "\t" <<
                    ref_energy << "\t" << 
                    test_energy << "\t" << 
                    ref_count << "\t" << 
                    test_count << 
                std::endl;
            }

            ASSERT_NEAR(relative_error_count,  0.0, 0.01);
            ASSERT_NEAR(relative_error_energy, 0.0, 0.01);
        }
    }
}


//------------------------------------------
TEST (TestTubeVoltage, TestSpekpyUnfiltered)
//------------------------------------------
{
    if (hasSpekpy())
    {
        std::string fname = "data/spekpy_unfiltered.dat";

        useSpekpy();
        setVoltage(g_voltage, "kV");

        runTest(fname);
    }
    else
    {
        FAIL() << "Spekpy is not installed.";
    }
}


//-------------------------------------------
TEST (TestTubeVoltage, TestXpecgenUnfiltered)
//-------------------------------------------
{   
    if (hasXpecgen())
    {
        std::string fname = "data/xpecgen_unfiltered.dat";

        useXpecgen();
        setVoltage(g_voltage, "kV");

        runTest(fname);
    }
    else
    {
        FAIL() << "Xpecgen is not installed.";
    }
}


//-----------------------------------------
TEST (TestTubeVoltage, TestSpekpyFiltered1)
//-----------------------------------------
{
    if (hasSpekpy())
    {
        std::string fname = "data/spekpy_filtered.dat";

        useSpekpy();
        setVoltage(g_voltage, "kV");

        clearInherentFiltration();
        clearFiltration();
        addFilter("Al", 10, "mm");
        addFilter("Cu", 5, "mm");

        runTest(fname);

        clearInherentFiltration();
        clearFiltration();
        addInherentFilter("Al", 10, "mm");
        addInherentFilter("Cu", 5, "mm");

        runTest(fname);
    }
    else
    {
        FAIL() << "Spekpy is not installed.";
    }
}


//------------------------------------------
TEST (TestTubeVoltage, TestXpecgenFiltered1)
//------------------------------------------
{
    if (hasXpecgen())
    {
        std::string fname = "data/xpecgen_filtered.dat";

        useXpecgen();
        setVoltage(g_voltage, "kV");

        clearInherentFiltration();
        clearFiltration();
        addFilter("Al", 10, "mm");
        addFilter("Cu", 5, "mm");

        runTest(fname);
        clearInherentFiltration();
        clearFiltration();
        addInherentFilter("Al", 10, "mm");
        addInherentFilter("Cu", 5, "mm");

        runTest(fname);
    }
    else
    {
        FAIL() << "Xpecgen is not installed.";
    }
}

//-----------------------------------------
TEST (TestTubeVoltage, TestSpekpyFiltered2)
//-----------------------------------------
{
    if (hasSpekpy())
    {
        std::string fname = "data/spekpy_filtered.dat";

        useSpekpy();
        setVoltage(g_voltage, "kV");

        clearInherentFiltration();
        clearFiltration();
        addFilter(13, 10, "mm");
        addFilter(29, 5, "mm");

        clearInherentFiltration();
        clearFiltration();
        addInherentFilter(13, 10, "mm");
        addInherentFilter(29, 5, "mm");

        runTest(fname);
    }
    else
    {
        FAIL() << "Spekpy is not installed.";
    }
}


//------------------------------------------
TEST (TestTubeVoltage, TestXpecgenFiltered2)
//------------------------------------------
{
    if (hasXpecgen())
    {
        std::string fname = "data/xpecgen_filtered.dat";

        useXpecgen();
        setVoltage(g_voltage, "kV");

        clearInherentFiltration();
        clearFiltration();
        addFilter(13, 10, "mm");
        addFilter(29, 5, "mm");

        runTest(fname);

        clearInherentFiltration();
        clearFiltration();
        addInherentFilter(13, 10, "mm");
        addInherentFilter(29, 5, "mm");

        runTest(fname);
    }
    else
    {
        FAIL() << "Xpecgen is not installed.";
    }
}

