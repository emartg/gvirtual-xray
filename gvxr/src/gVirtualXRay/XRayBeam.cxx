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
*	@file		XRayBeam.cxx
*
*	@brief		Class to manage X-Ray beams. The beam spectrum is discretised
*				into energy channels.
*
*	@version    1.0
*
*	@date		19/09/2023
*
*	@author		Dr Franck P. Vidal
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*   http://www.fpvidal.net/, Sept 2023, 2023, version 1.0, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
#ifdef WIN32
#define _HAS_STD_BYTE 0
#endif

#include "xsource.h"

#ifndef GVXR_XRAY_BEAM_H
#include <gVirtualXRay/XRayBeam.h>
#endif


//******************************************************************************
//	namespace
//******************************************************************************
using namespace gVirtualXRay;


//-------------------------------------------------------
void XRayBeam::initialise(const char* aFileName,
						  const RATIONAL_NUMBER& aUnit,
						  unsigned int aMergeChannelFlag)
//-------------------------------------------------------
{
    LOGGER.logWarning(std::string(__FUNCTION__) + 
        " is deprecated. It will be removed in a future release. "
        "Choose initialisePerPixelAtSDD or initialisePerCm2At1m. "
        "Note that initialise and initialisePerPixelAtSDD are equivalent.") << std::endl;

    initialisePerPixelAtSDD(aFileName, aUnit, aMergeChannelFlag);
}


//---------------------------------------------------------------------
void XRayBeam::initialisePerPixelAtSDD(const char* aFileName,
						               const RATIONAL_NUMBER& aUnit,
						               unsigned int aMergeChannelFlag)
//--------------------------------------------------------------------
{
    loadSpectrumFromFile(aFileName, aUnit, aMergeChannelFlag, PER_PIXEL_AT_SDD);
}


//---------------------------------------------------------------------
void XRayBeam::initialisePerCm2At1m(const char* aFileName,
						            const RATIONAL_NUMBER& aUnit,
						            unsigned int aMergeChannelFlag)
//--------------------------------------------------------------------
{
    loadSpectrumFromFile(aFileName, aUnit, aMergeChannelFlag, PER_CM2_AT_1M);
}


//------------------------
void XRayBeam::normalise()
//------------------------
{
	// Compute the total incident energy
	RATIONAL_NUMBER total_energy(0);
	for (std::vector<SpectrumRecord>::const_iterator ite_beam_spectrum(m_current_beam_spectrum.begin());
		ite_beam_spectrum != m_current_beam_spectrum.end();
		++ite_beam_spectrum)
	{
		total_energy += ite_beam_spectrum->m_energy * ite_beam_spectrum->m_photon_number;
	}

	// The energy is positive
	if (total_energy > 0)
	{
		// Normalise the number of photons
		for (std::vector<SpectrumRecord>::iterator ite_beam_spectrum(m_current_beam_spectrum.begin());
			ite_beam_spectrum != m_current_beam_spectrum.end();
			++ite_beam_spectrum)
		{
			ite_beam_spectrum->m_photon_number /= total_energy;
		}
	}
}


//----------------------------------------------------------------
void XRayBeam::setPhotonCountingBands(RATIONAL_NUMBER aMinEnergy,
                                      RATIONAL_NUMBER aMaxEnergy,
                                      size_t aNumberOfBands)
//----------------------------------------------------------------
{
    // Reset the array
    if (m_beam_spectrum_for_photon_count_detector.size())
    {
        mergePhotonCountingBands();
    }

    // Ignore this functionality if there is one band only
    if (aNumberOfBands > 1)
    {
        // Get the min and max energies
        RATIONAL_NUMBER step = (aMaxEnergy - aMinEnergy) / aNumberOfBands;
        RATIONAL_NUMBER max_energy_current_band = aMinEnergy;

        std::vector<SpectrumRecord>::const_iterator next_record_ite = m_current_beam_spectrum.begin();

        // Process every channel in the spectrum
        for (unsigned int i = 0; i < aNumberOfBands; ++i)
        {
            // Add a new band
            m_beam_spectrum_for_photon_count_detector.push_back(std::vector<SpectrumRecord>());
            m_photon_count_bands.push_back(max_energy_current_band);

            max_energy_current_band += step;
            RATIONAL_NUMBER last_recorded_energy = 0;

            // Process all the channel of the energy band
            while (next_record_ite != m_current_beam_spectrum.end() && last_recorded_energy < max_energy_current_band)
            {
                if (next_record_ite->m_energy < max_energy_current_band)
                {
                    m_beam_spectrum_for_photon_count_detector.back().push_back(*next_record_ite);
                    last_recorded_energy = next_record_ite->m_energy;

                    ++next_record_ite;
                }
                // Exit the while loop
                else
                {
                    last_recorded_energy = max_energy_current_band;
                }
            }

            // The last band is empty
            if (m_beam_spectrum_for_photon_count_detector.back().size() == 0)
            {
                // Drop the last band as it's empty
                m_beam_spectrum_for_photon_count_detector.pop_back();
                m_photon_count_bands.pop_back();
            }
        }
    }
}


//---------------------------------------
void XRayBeam::mergePhotonCountingBands()
//---------------------------------------
{
    // The functionality is in use
    if (getNumberOfPhotonCountingBands())
    {
        // Empty the spectrum
        m_current_beam_spectrum.clear();

        // Process all the bands
        for (std::vector< std::vector<SpectrumRecord> >::const_iterator ite_band(m_beam_spectrum_for_photon_count_detector.begin());
            ite_band != m_beam_spectrum_for_photon_count_detector.end();
            ++ite_band)
        {
            // Process all the bins
            for (std::vector<SpectrumRecord>::const_iterator ite_beam_spectrum(ite_band->begin());
                ite_beam_spectrum != ite_band->end();
                ++ite_beam_spectrum)
            {
                // Add the bin
                m_current_beam_spectrum.push_back(*ite_beam_spectrum);
            }
        }

        // Destroy the bands
        m_beam_spectrum_for_photon_count_detector.clear();
        m_photon_count_bands.clear();
    }
}


//-----------------------------------
void XRayBeam::generateTubeSpectrum()
//-----------------------------------
{
	float voltage_in_kV(m_voltage / kV);
    if (voltage_in_kV > 0)
    {
        clearSpectrumOnly();

        if (m_p_xsec.get() == nullptr)
        {
            m_p_xsec.reset(new xsec);

            if (!m_p_xsec->init())
            {
                throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot initialise xsec");
            }
        }

        float bin_energy_in_keV(m_first_energy_bin / keV);
        float energy_step_in_keV = m_size_energy_bin / keV;

        std::vector<float> energy_bins;

        while (bin_energy_in_keV < voltage_in_kV + energy_step_in_keV / 2)
        {
            energy_bins.push_back(bin_energy_in_keV);
            bin_energy_in_keV += energy_step_in_keV;
        }

        xraySource spectrum_generator;
        if (!spectrum_generator.init(m_p_xsec.get()))
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot initialise xraySource");
        }

        float* p_photon_count_set = spectrum_generator.simulateSpectra(voltage_in_kV, m_tube_angle_in_degrees, 74, &energy_bins[0], energy_bins.size());

        // Normalise the total energy
        double total_energy = 0.0;
        for (unsigned int i = 0; i < energy_bins.size(); ++i)
        {
            total_energy += p_photon_count_set[i] * energy_bins[i];
        }

    	double multiplier = 1.0 / total_energy;

        // Apply mAs if needed
        if (m_mAs > 0.0)
        {
        	// Number calibrated using spekpy
            // double polyfit = voltage_in_kV * 3117714780.725846 - -229085432418.32672;
        	double polyfit = voltage_in_kV * voltage_in_kV * voltage_in_kV * -3.17500435e+03 +
				voltage_in_kV * voltage_in_kV * 6.87679955e+06 +
				voltage_in_kV * 3.46583485e+08 +
				-7.77731261e+09;

        	// polyfit = 1.0;

        	if (polyfit < 1.0)
        		polyfit = 1.0;

        	multiplier *= m_mAs * polyfit;
        }







        for (unsigned int i = 0; i < energy_bins.size(); ++i)
        {
            // Apply the inherent filter
            if (!m_inherent_filtration.empty())
            {
                double mu_times_d = 0.0;

                // Go through all the inherent filters
                for (std::vector<std::pair<unsigned int, RATIONAL_NUMBER> >::const_iterator ite_filtration(m_inherent_filtration.begin());
                    ite_filtration != m_inherent_filtration.end();
                    ++ite_filtration)
                {
                    // Set the attenuation coefficient
                    double mu = PhotonCrossSection(ite_filtration->first).getLinearAttenuationCoefficient
                	(energy_bins[i] * keV);

                    // Convert mu in cm-1
                    mu /= 1.0 / cm; // 1/cm because the unit of mu is cm-1

                    double d = ite_filtration->second / cm;

                    mu_times_d += mu * d;
                }
              
                // Apply the filter
                p_photon_count_set[i] *= exp(-mu_times_d);
            }

            addChannelPerCm2At1m(multiplier * p_photon_count_set[i], energy_bins[i] * keV);
        }


        free(p_photon_count_set);
        p_photon_count_set = nullptr;
    }
}


//-------------------------------------------------------------------
void XRayBeam::loadSpectrumFromFile(const char* aFileName,
                                    const RATIONAL_NUMBER& aUnit,
                                    unsigned int aMergeChannelFlag,
                                    PhotonCountType aPhotonCountType)
//-------------------------------------------------------------------
{
    m_photon_count_type = aPhotonCountType;

	// Empty the current beam spectrum
	m_current_beam_spectrum.clear();
	m_unfiltered_beam_spectrum.clear();

    // Clear the filtration
    m_filtration.clear();

	// Open the file
	std::ifstream beam_spectrum_stream(aFileName);

	// The file is not open
	if (!beam_spectrum_stream.is_open())
	{
		throw FileDoesNotExistException(__FILE__, __FUNCTION__, __LINE__, aFileName);
	}

	// Load the file
	SpectrumRecord temp;
	while (beam_spectrum_stream >> temp)
	{
		// The record is valid
		if (!beam_spectrum_stream.eof())
		{
			// Apply the unit
			temp.m_energy *= aUnit;

			// The number of photons is greater than the threshold
			if (temp.m_photon_number * temp.m_energy >= m_bin_total_energy_lower_threshold)
			{
				// Store the data
				m_current_beam_spectrum.push_back(temp);
				m_unfiltered_beam_spectrum.push_back(temp);
			}
		}
	}

	// Merge channels
	if (aMergeChannelFlag > 1)
	{
		// Merge channels
		unsigned int counter(0);
		temp.m_photon_number = 0;
		temp.m_energy = 0;

		// The set of new records
		std::vector<SpectrumRecord> new_beam_spectrum;

		// Process all the spectrum
		for (std::vector<SpectrumRecord>::iterator ite(m_current_beam_spectrum.begin());
			ite != m_current_beam_spectrum.end();
			++ite)
		{
			// Add the current 'old' bin to the 'new' bin
			temp.m_photon_number += ite->m_photon_number;
			temp.m_energy += ite->m_energy * ite->m_photon_number;
			counter++;

			// Store the new bin
			if (counter == aMergeChannelFlag)
			{
				// Compute the average the energy
				temp.m_energy /= temp.m_photon_number;

				// Add the bin
				new_beam_spectrum.push_back(temp);

				// Reset counters
				counter = 0;
				temp.m_photon_number = 0;
				temp.m_energy = 0;
			}
		}

		// Add remaining channels if any
		if (counter)
		{
			temp.m_energy /= temp.m_photon_number;
			new_beam_spectrum.push_back(temp);
		}

		// Store the new spectrum
		m_current_beam_spectrum = new_beam_spectrum;
		m_unfiltered_beam_spectrum = new_beam_spectrum;
	}

    // Sort the spectrum
    sort();
}


//-------------------------------------------------------------------------------
RATIONAL_NUMBER XRayBeam::applyFiltration(const RATIONAL_NUMBER& anEnergy, 
                                          const RATIONAL_NUMBER& aNumberOfPhoton)
//-------------------------------------------------------------------------------
{
    double mu_times_d = 0.0;

    for (std::vector<std::pair<unsigned int, RATIONAL_NUMBER> >::const_iterator ite_filtration(m_filtration.begin());
        ite_filtration != m_filtration.end();
        ++ite_filtration)
    {
        // Set the attenuation coefficient
        double mu = PhotonCrossSection(ite_filtration->first).getLinearAttenuationCoefficient(anEnergy);

        // Convert mu in cm-1
        mu /= 1.0 / cm; // 1/cm because the unit of mu is cm-1

        double d = ite_filtration->second / cm;

        mu_times_d += mu * d;
    }

    return aNumberOfPhoton * exp(-mu_times_d);
}


//----------------------------------------
void XRayBeam::applyFiltrationOnSpectrum()
//----------------------------------------
{
	// Copy the spectrum if needed
    if (!m_unfiltered_beam_spectrum.size() && m_current_beam_spectrum.size())
    {
        m_unfiltered_beam_spectrum = m_current_beam_spectrum;
    }

    // Clear the filtered spectrum
    m_current_beam_spectrum.clear();

    for (std::vector<SpectrumRecord>::iterator ite_beam_spectrum(m_unfiltered_beam_spectrum.begin());
        ite_beam_spectrum != m_unfiltered_beam_spectrum.end();
        ++ite_beam_spectrum)
    {
    	SpectrumRecord filtered_bin;
    	filtered_bin.m_energy = ite_beam_spectrum->m_energy;
    	filtered_bin.m_photon_number = applyFiltration(ite_beam_spectrum->m_energy, ite_beam_spectrum->m_photon_number);
    			
        // The total energy of the bin is above the cut-off threshold
        if (filtered_bin.m_photon_number * filtered_bin.m_energy >= m_bin_total_energy_lower_threshold)
        {
			// Store the data
			m_current_beam_spectrum.push_back(filtered_bin);
        }
    }

    // Sort the spectrum
    sort();
}
