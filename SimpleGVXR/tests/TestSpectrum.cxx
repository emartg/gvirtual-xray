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
*   @file       TestSpectrum.cxx
*
*   @brief      Unit testing: Make sure the spectrum is sorted in asending energy.
*   			The unit test corresponds to validation_09,
*   			last date of check: Passed test on 10 May 2023.
*
*   @version    1.0
*
*   @date       10/05/2023
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
*               http://www.fpvidal.net/, May 2023, 2023, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include "gtest/gtest.h"
#include <cmath>
#include <vector>
#include <fstream>

#ifndef GVXR_EXCEPTION_H
#include "gVirtualXRay/Exception.h"
#endif

#ifndef SGVXR_SimpleGVXR_h
#include "SimpleGVXR.h"
#endif

#ifndef GVXR_source_h
#include "source.h"
#endif


//--------------------------------
TEST(TestSpectrum, TestExceptions)
//--------------------------------
{
    resetBeamSpectrum();

    std::vector<std::pair<double, double> > p_spectrum = {
        std::pair<double, double>(80, 50),
        std::pair<double, double>(85, 40),
        std::pair<double, double>(90, 30)
    };

    // We test addEnergyBinToSpectrumPerCm2At1m() and 
    // exceptions when the conversion must be used in getPhotonCountsPerPixelAtSDD()
    addEnergyBinToSpectrumPerCm2At1m(p_spectrum[0].first, "keV", p_spectrum[0].second);
    addEnergyBinToSpectrumPerCm2At1m(p_spectrum[1].first, "keV", p_spectrum[1].second);
    addEnergyBinToSpectrumPerCm2At1m(p_spectrum[2].first, "keV", p_spectrum[2].second);

    std::vector<double> p_energy_bins = getEnergyBins("keV");
    for (unsigned int i = 0; i < p_spectrum.size(); ++i)
    {
        ASSERT_FLOAT_EQ(p_spectrum[i].first, p_energy_bins[i]);
    }

    std::vector<double> p_photon_count_per_cm2_at_1m;
    std::vector<double> p_photon_count_per_pixel_at_SDD;

    // Expect an exception as the SDD is not known
    EXPECT_THROW(
        p_photon_count_per_pixel_at_SDD = getPhotonCountsPerPixelAtSDD(),
        gVirtualXRay::Exception
    );

    EXPECT_NO_THROW(
        p_photon_count_per_cm2_at_1m = getPhotonCountsPerCm2At1m()
    );

    resetBeamSpectrum();

    // We test addEnergyBinToSpectrumPerPixelAtSDD() and 
    // exceptions when the conversion must be used in getPhotonCountsPerCm2At1m()
    addEnergyBinToSpectrumPerPixelAtSDD(p_spectrum[0].first, "keV", p_spectrum[0].second);
    addEnergyBinToSpectrumPerPixelAtSDD(p_spectrum[1].first, "keV", p_spectrum[1].second);
    addEnergyBinToSpectrumPerPixelAtSDD(p_spectrum[2].first, "keV", p_spectrum[2].second);

    p_energy_bins = getEnergyBins("keV");
    for (unsigned int i = 0; i < p_spectrum.size(); ++i)
    {
        ASSERT_FLOAT_EQ(p_spectrum[i].first, p_energy_bins[i]);
    }

    // Expect an exception as the SDD is not known
    EXPECT_THROW(
        p_photon_count_per_cm2_at_1m = getPhotonCountsPerCm2At1m(),
        gVirtualXRay::Exception
    );

    EXPECT_NO_THROW(
        p_photon_count_per_pixel_at_SDD = getPhotonCountsPerPixelAtSDD()
    );

    resetBeamSpectrum();
}


//---------------------------------
TEST(TestSpectrum, TestNoException)
//---------------------------------
{
    resetBeamSpectrum();

    setDetectorPixelPitch(1, 1, "cm");
    setDetectorPosition(0, 0, 0, "m");
    setSourcePosition(0, 0, 1, "m");

    std::vector<std::pair<double, double> > p_spectrum = {
        std::pair<double, double>(80, 50),
        std::pair<double, double>(85, 40),
        std::pair<double, double>(90, 30)
    };

    addEnergyBinToSpectrumPerCm2At1m(p_spectrum[0].first, "keV", p_spectrum[0].second);
    addEnergyBinToSpectrumPerCm2At1m(p_spectrum[1].first, "keV", p_spectrum[1].second);
    addEnergyBinToSpectrumPerCm2At1m(p_spectrum[2].first, "keV", p_spectrum[2].second);

    std::vector<double> p_energy_bins = getEnergyBins("keV");
    for (unsigned int i = 0; i < p_spectrum.size(); ++i)
    {
        ASSERT_FLOAT_EQ(p_spectrum[i].first, p_energy_bins[i]);
    }

    std::vector<double> p_photon_count_per_cm2_at_1m;
    std::vector<double> p_photon_count_per_pixel_at_SDD;

    EXPECT_NO_THROW(
        p_photon_count_per_pixel_at_SDD = getPhotonCountsPerPixelAtSDD()
    );

    EXPECT_NO_THROW(
        p_photon_count_per_cm2_at_1m = getPhotonCountsPerCm2At1m()
    );

    for (unsigned int i = 0; i < p_spectrum.size(); ++i)
    {
        ASSERT_FLOAT_EQ(p_spectrum[i].second, p_photon_count_per_cm2_at_1m[i]);
        ASSERT_FLOAT_EQ(p_spectrum[i].second, p_photon_count_per_pixel_at_SDD[i]);
    }

    resetBeamSpectrum();

    addEnergyBinToSpectrumPerPixelAtSDD(p_spectrum[0].first, "keV", p_spectrum[0].second);
    addEnergyBinToSpectrumPerPixelAtSDD(p_spectrum[1].first, "keV", p_spectrum[1].second);
    addEnergyBinToSpectrumPerPixelAtSDD(p_spectrum[2].first, "keV", p_spectrum[2].second);

    p_energy_bins = getEnergyBins("keV");
    for (unsigned int i = 0; i < p_spectrum.size(); ++i)
    {
        ASSERT_FLOAT_EQ(p_spectrum[i].first, p_energy_bins[i]);
    }

    EXPECT_NO_THROW(
        p_photon_count_per_cm2_at_1m = getPhotonCountsPerCm2At1m()
    );

    EXPECT_NO_THROW(
        p_photon_count_per_pixel_at_SDD = getPhotonCountsPerPixelAtSDD()
    );

    for (unsigned int i = 0; i < p_spectrum.size(); ++i)
    {
        ASSERT_FLOAT_EQ(p_spectrum[i].second, p_photon_count_per_cm2_at_1m[i]);
        ASSERT_FLOAT_EQ(p_spectrum[i].second, p_photon_count_per_pixel_at_SDD[i]);
    }

    resetBeamSpectrum();

    // Pretend the SDD is no longer known
    setDetectorPixelPitch(0, 0, "cm");
    setDetectorPosition(0, 0, 0, "m");
    setSourcePosition(0, 0, 0, "m");
}


//---------------------------------
TEST(TestSpectrum, TestChangingSDD)
//---------------------------------
{
    resetBeamSpectrum();

    setDetectorPixelPitch(sqrt(0.5), sqrt(0.5), "cm");
    setDetectorPosition(0, 0, 0, "m");
    setSourcePosition(0, 0, 1, "m");

    std::vector<std::pair<double, double> > p_spectrum = {
        std::pair<double, double>(80, 50),
        std::pair<double, double>(85, 40),
        std::pair<double, double>(90, 30)
    };

    addEnergyBinToSpectrumPerCm2At1m(p_spectrum[0].first, "keV", p_spectrum[0].second);
    addEnergyBinToSpectrumPerCm2At1m(p_spectrum[1].first, "keV", p_spectrum[1].second);
    addEnergyBinToSpectrumPerCm2At1m(p_spectrum[2].first, "keV", p_spectrum[2].second);

    std::vector<double> p_energy_bins = getEnergyBins("keV");
    for (unsigned int i = 0; i < p_spectrum.size(); ++i)
    {
        ASSERT_FLOAT_EQ(p_spectrum[i].first, p_energy_bins[i]);
    }

    std::vector<double> p_photon_count_per_cm2_at_1m_before_move = getPhotonCountsPerCm2At1m();
    std::vector<double> p_photon_count_per_pixel_at_SDD_before_move = getPhotonCountsPerPixelAtSDD();

    setDetectorPosition(0, 0, 10, "m");
    setSourcePosition(0, 0, 15, "m");

    // Force the conversion
    std::vector<double> temp_1 = getPhotonCountsPerCm2At1m();
    std::vector<double> temp_2 = getPhotonCountsPerPixelAtSDD();

    // Restore the values
    setDetectorPosition(0, 0, 1, "m");
    setSourcePosition(0, 0, 0, "m");

    std::vector<double> p_photon_count_per_cm2_at_1m_after_move = getPhotonCountsPerCm2At1m();
    std::vector<double> p_photon_count_per_pixel_at_SDD_after_move = getPhotonCountsPerPixelAtSDD();


    for (unsigned int i = 0; i < p_spectrum.size(); ++i)
    {
        ASSERT_FLOAT_EQ(p_spectrum[i].second, p_photon_count_per_cm2_at_1m_after_move[i]);
        ASSERT_FLOAT_EQ(p_spectrum[i].second, p_photon_count_per_pixel_at_SDD_after_move[i] * 2.0);
    }

    resetBeamSpectrum();

    addEnergyBinToSpectrumPerPixelAtSDD(p_spectrum[0].first, "keV", p_spectrum[0].second);
    addEnergyBinToSpectrumPerPixelAtSDD(p_spectrum[1].first, "keV", p_spectrum[1].second);
    addEnergyBinToSpectrumPerPixelAtSDD(p_spectrum[2].first, "keV", p_spectrum[2].second);

    p_energy_bins = getEnergyBins("keV");
    for (unsigned int i = 0; i < p_spectrum.size(); ++i)
    {
        ASSERT_FLOAT_EQ(p_spectrum[i].first, p_energy_bins[i]);
    }

    p_photon_count_per_cm2_at_1m_after_move = getPhotonCountsPerCm2At1m();
    p_photon_count_per_pixel_at_SDD_after_move = getPhotonCountsPerPixelAtSDD();

    for (unsigned int i = 0; i < p_spectrum.size(); ++i)
    {
        ASSERT_FLOAT_EQ(p_spectrum[i].second, p_photon_count_per_cm2_at_1m_after_move[i] / 2.0);
        ASSERT_FLOAT_EQ(p_spectrum[i].second, p_photon_count_per_pixel_at_SDD_after_move[i]);
    }

    resetBeamSpectrum();

    // Pretend the SDD is no longer known
    setDetectorPixelPitch(0, 0, "cm");
    setDetectorPosition(0, 0, 0, "m");
    setSourcePosition(0, 0, 0, "m");
}


//-----------------------------------
TEST(TestSpectrum, TestSpectrumSort1)
//-----------------------------------
{
    resetBeamSpectrum();

    float p_energy_set[] = {80, 81, 81};

    addEnergyBinToSpectrumPerCm2At1m(p_energy_set[0], "keV", 1);
    addEnergyBinToSpectrumPerCm2At1m(p_energy_set[1], "keV", 10);
    addEnergyBinToSpectrumPerCm2At1m(p_energy_set[2], "keV", 5);

    std::vector<double> p_energy_bins = getEnergyBins("keV");

    for (unsigned int i = 0; i < p_energy_bins.size(); ++i)
    {
        ASSERT_FLOAT_EQ(p_energy_set[i], p_energy_bins[i]);
    }

    resetBeamSpectrum();
}


//-----------------------------------
TEST(TestSpectrum, TestSpectrumSort2)
//-----------------------------------
{
    float p_energy_set[] = {80, 81, 81};

    addEnergyBinToSpectrumPerCm2At1m(p_energy_set[2], "keV", 1);
    addEnergyBinToSpectrumPerCm2At1m(p_energy_set[1], "keV", 10);
    addEnergyBinToSpectrumPerCm2At1m(p_energy_set[0], "keV", 5);

    std::vector<double> p_energy_bins = getEnergyBins("keV");

    for (unsigned int i = 0; i < p_energy_bins.size(); ++i)
    {
        ASSERT_FLOAT_EQ(p_energy_set[i], p_energy_bins[i]);
    }

    resetBeamSpectrum();
}


//-----------------------------------
TEST(TestSpectrum, TestSpectrumSort3)
//-----------------------------------
{
    resetBeamSpectrum();

    std::ifstream spectrum_data_file("data/55kV.tsv");

    float energy, photon_count;

    std::vector<float> p_energy_set1;
    std::vector<float> p_photon_count_set1;

    // *** Add in the same order ***
    while (spectrum_data_file >> energy >> photon_count)
    {
        if (photon_count * energy * getUnitOfEnergy("keV") >= 1 * getUnitOfEnergy("eV"))
        {
        	addEnergyBinToSpectrumPerCm2At1m(energy, "keV", photon_count);
            p_energy_set1.push_back(energy);
            p_photon_count_set1.push_back(photon_count);
        }
    }

    std::vector<double> p_energy_set2 = getEnergyBins("keV");
    std::vector<double> p_photon_count_set2 = getPhotonCountsPerCm2At1m();

    ASSERT_EQ(p_energy_set1.size(), p_energy_set2.size());
    ASSERT_EQ(p_photon_count_set1.size(), p_photon_count_set2.size());

    for (unsigned int i = 0; i < p_energy_set1.size(); ++i)
    {
        ASSERT_FLOAT_EQ(p_energy_set1[i], p_energy_set2[i]);
        ASSERT_FLOAT_EQ(p_photon_count_set1[i], p_photon_count_set2[i]);
    }

    resetBeamSpectrum();
}


//-----------------------------------
TEST(TestSpectrum, TestSpectrumSort4)
//-----------------------------------
{
    resetBeamSpectrum();

    std::ifstream spectrum_data_file("data/55kV.tsv");

    float energy, photon_count;

    std::vector<float> p_energy_set1;
    std::vector<float> p_photon_count_set1;

    while (spectrum_data_file >> energy >> photon_count)
    {
        if (photon_count * energy * getUnitOfEnergy("keV") >= 1 * getUnitOfEnergy("eV"))
        {
            p_energy_set1.push_back(energy);
            p_photon_count_set1.push_back(photon_count);
        }
    }

    // *** Add in revesrse order ***
    for (unsigned int i = 0; i < p_energy_set1.size(); ++i)
    {
    	addEnergyBinToSpectrumPerCm2At1m(p_energy_set1[p_energy_set1.size() - i - 1], "keV",
            p_photon_count_set1[p_photon_count_set1.size() - i - 1] );
    }

    std::vector<double> p_energy_set2 = getEnergyBins("keV");
    std::vector<double> p_photon_count_set2 = getPhotonCountsPerCm2At1m();

    ASSERT_EQ(p_energy_set1.size(), p_energy_set2.size());
    ASSERT_EQ(p_photon_count_set1.size(), p_photon_count_set2.size());

    for (unsigned int i = 0; i < p_energy_set1.size(); ++i)
    {
        ASSERT_FLOAT_EQ(p_energy_set1[i], p_energy_set2[i]);
        ASSERT_FLOAT_EQ(p_photon_count_set1[i], p_photon_count_set2[i]);
    }

    resetBeamSpectrum();
}


//------------------------------------------
TEST(TestSpectrum, MergePhotonCountingBands)
//------------------------------------------
{
    resetBeamSpectrum();

    unsigned int number_of_bins = 12;
    unsigned int number_of_bands = 4;

    for (unsigned int i = 0; i < number_of_bins; ++i)
    {
    	addEnergyBinToSpectrumPerCm2At1m(i + 1, "keV", i * 10 + 1);
    }
    setPhotonCountingBands(0, number_of_bins+1e-6, "keV", number_of_bands);

    mergePhotonCountingBands();
    std::vector<double> p_energy_set = getEnergyBins("keV");
    std::vector<double> p_photon_count_set = getPhotonCountsPerCm2At1m();

    ASSERT_EQ(size_t(0), getNumberOfPhotonCountingBands());
    ASSERT_EQ(number_of_bins, p_energy_set.size());

    for (unsigned int i = 0; i < p_energy_set.size(); ++i)
    {
        ASSERT_FLOAT_EQ(p_energy_set[i], i + 1);
        ASSERT_FLOAT_EQ(p_photon_count_set[i], i * 10 + 1);
    }

    resetBeamSpectrum();
}


//-------------------------------------
TEST(TestSpectrum, Cm2At1mToPixelAtSDD)
//-------------------------------------
{
    resetBeamSpectrum();
    clearFiltration();

    addEnergyBinToSpectrumPerCm2At1m(15, "keV", 20);
    addEnergyBinToSpectrumPerCm2At1m(10, "keV", 25);

    ASSERT_FLOAT_EQ(getPhotonCountsPerCm2At1m()[0], 25);
    ASSERT_FLOAT_EQ(getPhotonCountsPerCm2At1m()[1], 20);
}


//------------------------------------------------
TEST(TestSpectrum, Cm2At1mToPixelAtSDD_Exceptions)
//------------------------------------------------
{
    resetBeamSpectrum();
    clearFiltration();

    addEnergyBinToSpectrumPerCm2At1m(15, "keV", 20);
    addEnergyBinToSpectrumPerCm2At1m(10, "keV", 25);

    // Tests that the expected exception is thrown
    EXPECT_THROW({
        try
        {
        	getPhotonCountsPerPixelAtSDD();
        }
        catch( const gVirtualXRay::Exception& e )
        {
            // Check the error message
        	ASSERT_EQ(std::string(e.what()).rfind("Error: The source-detector distance is null. You must initialise the source and detector before using this function.\n", 0), size_t(0));
            throw;
        }
    }, gVirtualXRay::Exception );
}


//------------------------------------------------
TEST(TestSpectrum, PixelAtSDDToCm2At1m_Exceptions)
//------------------------------------------------
{
    resetBeamSpectrum();
    clearFiltration();

    addEnergyBinToSpectrumPerPixelAtSDD(15, "keV", 20);
    addEnergyBinToSpectrumPerPixelAtSDD(10, "keV", 25);

    // Tests that the expected exception is thrown
    EXPECT_THROW({
        try
        {
        	getPhotonCountsPerCm2At1m();
        }
        catch( const gVirtualXRay::Exception& e )
        {
            // Check the error message
        	ASSERT_EQ(std::string(e.what()).rfind("Error: The source-detector distance is null. You must initialise the source and detector before using this function.\n", 0), size_t(0));
            throw;
        }
    }, gVirtualXRay::Exception );
}


//-----------------------------------
TEST(TestSpectrum, ConversionAt100cm)
//-----------------------------------
{
    resetBeamSpectrum();
    clearFiltration();

    // SDD is 100 cm
    setDetectorPosition(0, 0, 50, "cm");
    setSourcePosition(0, 0, -50, "cm");

    setDetectorPixelPitch(1, 1, "cm");

    addEnergyBinToSpectrumPerCm2At1m(15, "keV", 20);
    addEnergyBinToSpectrumPerCm2At1m(10, "keV", 25);

    ASSERT_FLOAT_EQ(getPhotonCountsPerPixelAtSDD()[0], 25);
    ASSERT_FLOAT_EQ(getPhotonCountsPerPixelAtSDD()[1], 20);


    resetBeamSpectrum();
    clearFiltration();

    addEnergyBinToSpectrumPerPixelAtSDD(15, "keV", 20);
    addEnergyBinToSpectrumPerPixelAtSDD(10, "keV", 25);

    ASSERT_FLOAT_EQ(getPhotonCountsPerCm2At1m()[0], 25);
    ASSERT_FLOAT_EQ(getPhotonCountsPerCm2At1m()[1], 20);
}


//------------------------------------------------
TEST(TestSpectrum, ConversionCm2At1mToPixelAt50cm)
//------------------------------------------------
{
    resetBeamSpectrum();
    clearFiltration();

    // SDD is 100 cm
    setDetectorPosition(0, 0, 50, "cm");
    setSourcePosition(0, 0, 0, "cm");

    // Pixel area
    double pixel_area = 1.0;    setDetectorPixelPitch(1, 1, "cm");

    addEnergyBinToSpectrumPerCm2At1m(15, "keV", 20);
    addEnergyBinToSpectrumPerCm2At1m(10, "keV", 25);

    ASSERT_FLOAT_EQ(getPhotonCountsPerCm2At1m()[0] * ((100*100) / (50*50)) * pixel_area, getPhotonCountsPerPixelAtSDD()[0]);
    ASSERT_FLOAT_EQ(getPhotonCountsPerCm2At1m()[1] * ((100*100) / (50*50)) * pixel_area, getPhotonCountsPerPixelAtSDD()[1]);

    // Pixel area
    pixel_area = 5.0;
    setDetectorPixelPitch(sqrt(pixel_area), sqrt(pixel_area), "cm");

    ASSERT_FLOAT_EQ(getPhotonCountsPerCm2At1m()[0] * ((100*100) / (50*50)) * pixel_area, getPhotonCountsPerPixelAtSDD()[0]);
    ASSERT_FLOAT_EQ(getPhotonCountsPerCm2At1m()[1] * ((100*100) / (50*50)) * pixel_area, getPhotonCountsPerPixelAtSDD()[1]);
}


//------------------------------------------------
TEST(TestSpectrum, ConversionPixelAt50cmToCm2At1m)
//------------------------------------------------
{
    resetBeamSpectrum();
    clearFiltration();

    // SDD is 100 cm
    setDetectorPosition(0, 0, 50, "cm");
    setSourcePosition(0, 0, 0, "cm");

    // Pixel area
    double pixel_area = 1.0;
    setDetectorPixelPitch(1, 1, "cm");

    addEnergyBinToSpectrumPerPixelAtSDD(15, "keV", 20);
    addEnergyBinToSpectrumPerPixelAtSDD(10, "keV", 25);

    ASSERT_FLOAT_EQ(getPhotonCountsPerCm2At1m()[0] * ((100*100) / (50*50)) * pixel_area, getPhotonCountsPerPixelAtSDD()[0]);
    ASSERT_FLOAT_EQ(getPhotonCountsPerCm2At1m()[1] * ((100*100) / (50*50)) * pixel_area, getPhotonCountsPerPixelAtSDD()[1]);

    // Pixel area
    pixel_area = 5.0;
    setDetectorPixelPitch(sqrt(pixel_area), sqrt(pixel_area), "cm");

    ASSERT_FLOAT_EQ(getPhotonCountsPerCm2At1m()[0] * ((100*100) / (50*50)) * pixel_area, getPhotonCountsPerPixelAtSDD()[0]);
    ASSERT_FLOAT_EQ(getPhotonCountsPerCm2At1m()[1] * ((100*100) / (50*50)) * pixel_area, getPhotonCountsPerPixelAtSDD()[1]);
}

