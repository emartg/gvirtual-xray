/*

Copyright (c) 2022, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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
 *   @file       TestScintillator.cxx
 *
 *   @brief      Unit testing: thest the implementation of the energy response of scintillators.
 *
 *
 *   @version    1.0
 *
 *   @date       28/10/2022
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
 *               http://www.fpvidal.net/, Oct 2022, 2022, version 1.0,
 *               BSD 3-Clause License
 *
 ********************************************************************************
 */


//******************************************************************************
//  Include
//******************************************************************************
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <numeric>

#include "gtest/gtest.h"

#ifndef GVXR_LOGGER_H
#include "gVirtualXRay/Logger.h"
#endif

#ifndef SGVXR_SimpleGVXR_h
#include "SimpleGVXR.h"
#endif


//******************************************************************************
//  Function declaration
//******************************************************************************
double getSum(const std::vector<double>& aVector);
double getAverage(const std::vector<double>& aVector);
double getVariance(const std::vector<double>& aVector);
double getStdDev(const std::vector<double>& aVector);
double getNCC(const std::vector<double>& aVector1, const std::vector<double>& aVector2);
double maxRelativeError(const std::vector<double>& aVector1, const std::vector<double>& aVector2);
double meanRelativeError(const std::vector<double>& aVector1, const std::vector<double>& aVector2);
void buildGnuplotHeader(std::ofstream& anOutputStream, const char* aTest);


//******************************************************************************
//  Const
//******************************************************************************


//-----------------------------------------------------------------------------
TEST(TestCsI, TestCsI)
//-----------------------------------------------------------------------------
{
    setScintillator("CsI", 600, "um");

    std::vector<std::vector<float> > energy_response = getEnergyResponse("keV");

    for (std::vector<std::vector<float> >::const_iterator ite = energy_response.begin();
        ite != energy_response.end();
        ++ite)
    {
        gVirtualXRay::LOGGER.logNow() << (*ite)[0] << "\t" << (*ite)[1] << std::endl;
    }
}





//-----------------------------------------------
double getSum(const std::vector<double>& aVector)
//-----------------------------------------------
{
    return (std::accumulate(aVector.begin(), aVector.end(), 0.0));
}


//---------------------------------------------------
double getAverage(const std::vector<double>& aVector)
//---------------------------------------------------
{
    return (getSum(aVector) / double(aVector.size()));
}


//----------------------------------------------------
double getVariance(const std::vector<double>& aVector)
//----------------------------------------------------
{
    double variance(0.0);
    double average(getAverage(aVector));

    for (std::vector<double>::const_iterator ite(aVector.begin());
            ite != aVector.end();
            ++ite)
    {
        variance += std::pow(*ite - average, 2);
    }

    return (variance / double(aVector.size()));
}


//--------------------------------------------------
double getStdDev(const std::vector<double>& aVector)
//--------------------------------------------------
{
    return (std::sqrt(getVariance(aVector)));
}


//------------------------------------------------
double getNCC(const std::vector<double>& aVector1,
        const std::vector<double>& aVector2)
//------------------------------------------------
{
    double ncc(0.0);

    double average1(getAverage(aVector1));
    double average2(getAverage(aVector2));

    double std_dev(getStdDev(aVector1) * getStdDev(aVector2));

    std::vector<double>::const_iterator ite1(aVector1.begin());
    std::vector<double>::const_iterator ite2(aVector2.begin());

    while (ite1 != aVector1.end())
    {
        ncc += (*ite1 - average1) * (*ite2 - average2);
        ++ite1;
        ++ite2;
    }

    ncc /= std_dev;
    ncc /= aVector1.size();

    return (ncc);
}


//----------------------------------------------------------
double maxRelativeError(const std::vector<double>& aVector1,
                        const std::vector<double>& aVector2)
//----------------------------------------------------------
{
    double max_relative_error(0.0);
    double relative_error(0.0);

    std::vector<double>::const_iterator ite1(aVector1.begin());
    std::vector<double>::const_iterator ite2(aVector2.begin());

    while (ite1 != aVector1.end())
    {
        relative_error = std::abs(*ite1 - *ite2) / *ite1;
        max_relative_error = std::max(relative_error, max_relative_error);

        ++ite1;
        ++ite2;
    }

    return (max_relative_error);
}


//-----------------------------------------------------------
double meanRelativeError(const std::vector<double>& aVector1,
        const std::vector<double>& aVector2)
//-----------------------------------------------------------
{
    double mean_relative_error(0.0);

    std::vector<double>::const_iterator ite1(aVector1.begin());
    std::vector<double>::const_iterator ite2(aVector2.begin());

    while (ite1 != aVector1.end())
    {
        mean_relative_error += std::abs(*ite1 - *ite2) / *ite1;

        ++ite1;
        ++ite2;
    }

    return (mean_relative_error / aVector1.size());
}


//-----------------------------------------------------------------------
void buildGnuplotHeader(std::ofstream& anOutputStream, const char* aTest)
//-----------------------------------------------------------------------
{
    anOutputStream << "#!/usr/bin/env gnuplot" << std::endl;
    anOutputStream << std::endl;
    anOutputStream << "set term post eps enhanced color 'Helvetica' 32 size 9in,7in" << std::endl;
    anOutputStream << "set output '" << aTest << ".eps'" << std::endl;
    anOutputStream << std::endl;
    anOutputStream << "set grid" << std::endl;
    anOutputStream << std::endl;
    anOutputStream << "#set key bottom" << std::endl;
    anOutputStream << std::endl;
    anOutputStream << "set log xy" << std::endl;
    anOutputStream << "set format xy '10^{%T}'" << std::endl;
    anOutputStream << std::endl;
    anOutputStream << "set title '" << aTest << "'" << std::endl;
    anOutputStream << "set ylabel 'Mass attenuation coefficient ({/Symbol m}/{/Symbol r}) in cm^2/g'" << std::endl;
    anOutputStream << "set xlabel 'Photon energy in MeV'" << std::endl;
    anOutputStream << "set xrange[0.001:1.0]" << std::endl;
    anOutputStream << std::endl;
    anOutputStream << "set key autotitle columnhead" << std::endl;
}
