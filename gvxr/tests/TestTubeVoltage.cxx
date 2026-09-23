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
*   @date       20/09/2023
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
#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include "gtest/gtest.h"

#ifndef GVXR_UNITS_H
#include "gVirtualXRay/Units.h"
#endif

#ifndef GVXR_XRAY_BEAM_H
#include <gVirtualXRay/XRayBeam.h>
#endif

#ifndef GVXR_LOGGER_H
#include "gVirtualXRay/Logger.h"
#endif


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;
//using namespace std;


//******************************************************************************
//  Global variables
//******************************************************************************

double g_voltage = 100;


//******************************************************************************
//  Function declaration
//******************************************************************************
void runTest(XRayBeam& aBeam, const std::string& aFileName);



void runTest(XRayBeam& aBeam, const std::string& aFileName)
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
    while (reference_data_stream.good())
    {
        double ref_count, ref_energy;
        reference_data_stream >> ref_energy >> ref_count;

        if (ref_energy > 0.5 && ref_count > 1e-7)
        {
            SpectrumRecord record;

            if (has_swapped)
            {
                has_swapped = false;
                record = aBeam.getEnergyChannel(i - 1);
            } 
            else
                record = aBeam.getEnergyChannel(i);

            ref_energy *= keV;
            double test_count = record.getPhotonNumber();
            double test_energy = record.getPhotonEnergy();
            

            double relative_error_count  = 100 * abs((ref_count - test_count) / ref_count);
            double relative_error_energy = 100 * abs((ref_energy - test_energy) / ref_energy);

            if (relative_error_count > 0.01)
            {
                if (!has_swapped)
                {
                    record = aBeam.getEnergyChannel(i+1);

                    test_count = record.getPhotonNumber();
                    test_energy = record.getPhotonEnergy();

                    relative_error_count  = 100 * abs((ref_count - test_count) / ref_count);
                    relative_error_energy = 100 * abs((ref_energy - test_energy) / ref_energy);

                    has_swapped = true;
                }
            }

            i++;

            if (relative_error_count > 0.01 || relative_error_energy > 0.01)
            {
                LOGGER.logError("") << "*********************************" << std::endl;
                LOGGER.logError("") << i << "\t" << 
                    relative_error_energy << "\t" << 
                    relative_error_count << "\t" <<
                    ref_energy / keV << "\t" << 
                    test_energy / keV << "\t" << 
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
    XRayBeam xray_beam;

    // XRayBeam xray_beam;
    if (xray_beam.hasSpekpy())
    {
        std::string fname = "data/spekpy_unfiltered.dat";

        xray_beam.useSpekpy();
        xray_beam.setVoltage(g_voltage * kV);

        runTest(xray_beam, fname);
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
    XRayBeam xray_beam;

    if (xray_beam.hasXpecgen())
    {
        std::string fname = "data/xpecgen_unfiltered.dat";

        xray_beam.useXpecgen();
        xray_beam.setVoltage(g_voltage * kV);

        runTest(xray_beam, fname);
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
    XRayBeam xray_beam;

    if (xray_beam.hasSpekpy())
    {
        std::string fname = "data/spekpy_filtered.dat";

        xray_beam.useSpekpy();
        xray_beam.setVoltage(g_voltage * kV);
        xray_beam.clearFiltration();
        xray_beam.addFilter("Al", 10 * mm);
        xray_beam.addFilter("Cu", 5 * mm);

        runTest(xray_beam, fname);
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
    XRayBeam xray_beam;

    if (xray_beam.hasXpecgen())
    {
        std::string fname = "data/xpecgen_filtered.dat";

        xray_beam.useXpecgen();
        xray_beam.setVoltage(g_voltage * kV);
        xray_beam.clearFiltration();
        xray_beam.addFilter("Al", 10 * mm);
        xray_beam.addFilter("Cu", 5 * mm);

        runTest(xray_beam, fname);
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
    XRayBeam xray_beam;

    if (xray_beam.hasSpekpy())
    {
        std::string fname = "data/spekpy_filtered.dat";

        xray_beam.useSpekpy();
        xray_beam.setVoltage(g_voltage * kV);
        xray_beam.clearFiltration();
        xray_beam.addFilter(13, 10 * mm);
        xray_beam.addFilter(29, 5 * mm);

        runTest(xray_beam, fname);
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
    XRayBeam xray_beam;

    if (xray_beam.hasXpecgen())
    {
        std::string fname = "data/xpecgen_filtered.dat";

        xray_beam.useXpecgen();
        xray_beam.setVoltage(g_voltage * kV);
        xray_beam.clearFiltration();
        xray_beam.addFilter(13, 10 * mm);
        xray_beam.addFilter(29, 5 * mm);

        runTest(xray_beam, fname);
    }
    else
    {
        FAIL() << "Xpecgen is not installed.";
    }
}


