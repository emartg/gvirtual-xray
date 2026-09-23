/*

Copyright (c) 2014, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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
*	@file		XRayBeam.inl
*
*	@brief		Class to manage X-Ray beams. The beam spectrum is discretised
*				into energy channels.
*
*	@version    1.0
*
*	@date		29/11/2013
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
*   http://www.fpvidal.net/, Dec 2014, 2014, version 1.0, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
#include <cfloat>
#include <algorithm>

#ifndef GVXR_ELEMENT_SET_H
#include "gVirtualXRay/ElementSet.h"
#endif

#ifndef GVXR_FILE_DOES_NOT_EXIST_EXCEPTION_H
#include "gVirtualXRay/FileDoesNotExistException.h"
#endif

#ifndef GVXR_OUT_OF_BOUNDS_EXCEPTION_H
#include "gVirtualXRay/OutOfBoundsException.h"
#endif

#ifndef GVXR_LOGGER_H
#include <gVirtualXRay/Logger.h>
#endif


//******************************************************************************
//	definitions
//******************************************************************************
#define DEFAULT_VOLTAGE -1
#define DEFAULT_TUBE_ANGLE 12
#define DEFAULT_mAs -1


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//-------------------------------------------------
inline XRayBeam::XRayBeam():
//-------------------------------------------------
		m_bin_total_energy_lower_threshold(0.01 * eV),
        m_use_poisson_noise(false),
        m_lambda_cutoff(50),
        m_photon_count_type(PhotonCountType::UNKNOWN),
        m_voltage(DEFAULT_VOLTAGE),
        m_tube_angle_in_degrees(DEFAULT_TUBE_ANGLE),
        m_mAs(DEFAULT_mAs),
        m_first_energy_bin(1 * keV),
        m_size_energy_bin(1 * keV)
//-------------------------------------------------
{
}


//--------------------------
inline XRayBeam::~XRayBeam()
//--------------------------
{
    clear();
}


//---------------------------
inline void XRayBeam::clear()
//---------------------------
{
    // Remove all the energy channels
    clearSpectrumOnly();

    m_voltage = DEFAULT_VOLTAGE;
    m_tube_angle_in_degrees = DEFAULT_TUBE_ANGLE;
    m_mAs = DEFAULT_mAs;
    
    m_filtration.clear();
    m_inherent_filtration.clear();
}


//---------------------------------------
inline void XRayBeam::clearSpectrumOnly()
//---------------------------------------
{
    // Remove all the energy channels
    m_unfiltered_beam_spectrum.clear();
    m_current_beam_spectrum.clear();
    m_beam_spectrum_for_photon_count_detector.clear();
    m_photon_count_bands.clear();

    m_photon_count_type = PhotonCountType::UNKNOWN;
}


//-----------------------------------------------------------------------
inline bool XRayBeam::addChannel(const RATIONAL_NUMBER& aNumberOfPhotons,
                                 const RATIONAL_NUMBER& anIncidentEnergy)
//-----------------------------------------------------------------------
{
    LOGGER.logWarning(std::string(__FUNCTION__) + 
        " is deprecated. It will be removed in a future release. "
        "Choose addChannelPerPixelAtSDD or addChannelPerCm2At1m. "
        "Note that addChannel and addChannelPerPixelAtSDD are equivalent.") << std::endl;

    return addChannelPerPixelAtSDD(aNumberOfPhotons, anIncidentEnergy);
}


//------------------------------------------------------------------------------------
inline bool XRayBeam::addChannelPerPixelAtSDD(const RATIONAL_NUMBER& aNumberOfPhotons,
                                              const RATIONAL_NUMBER& anIncidentEnergy)
//------------------------------------------------------------------------------------
{
    if (m_photon_count_type == PhotonCountType::PER_CM2_AT_1M)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
            "Cannot mix photon numbers per pixel at SDD with per cm2 at 1m");
    }

    // Record the photon count type
    m_photon_count_type = PhotonCountType::PER_PIXEL_AT_SDD;

    // Temporary structure
    SpectrumRecord unfiltered_bin;
    SpectrumRecord filtered_bin;

    // Store the number of photons
    unfiltered_bin.m_photon_number = aNumberOfPhotons;
    filtered_bin.m_photon_number = applyFiltration(anIncidentEnergy, aNumberOfPhotons);

    // Store the energy
    unfiltered_bin.m_energy = anIncidentEnergy;
    filtered_bin.m_energy = anIncidentEnergy;

    // Do not apply the cut-off threshold in case the flux is changed later on
    // Store the data
    m_current_beam_spectrum.push_back(filtered_bin);

    // Store the data
    m_unfiltered_beam_spectrum.push_back(unfiltered_bin);

    // Sort the spectrum
    sort();

    if (unfiltered_bin.m_photon_number * unfiltered_bin.m_energy >= m_bin_total_energy_lower_threshold)
    {
        return true;
    }
    else
    {
        return false;
    }
}


//---------------------------------------------------------------------------------
inline bool XRayBeam::addChannelPerCm2At1m(const RATIONAL_NUMBER& aNumberOfPhotons,
                                           const RATIONAL_NUMBER& anIncidentEnergy)
//---------------------------------------------------------------------------------
{
    if (m_photon_count_type == PhotonCountType::PER_PIXEL_AT_SDD)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
            "Cannot mix photon numbers per pixel at SDD with per cm2 at 1m");
    }

    // Record the photon count type
    m_photon_count_type = PhotonCountType::PER_CM2_AT_1M;

    // Temporary structure
    SpectrumRecord unfiltered_bin;
    SpectrumRecord filtered_bin;

    // Store the number of photons
    unfiltered_bin.m_photon_number = aNumberOfPhotons;
    filtered_bin.m_photon_number = applyFiltration(anIncidentEnergy, aNumberOfPhotons);

    // Store the energy
    unfiltered_bin.m_energy = anIncidentEnergy;
    filtered_bin.m_energy = anIncidentEnergy;

    // Do not apply the cut-off threshold in case the flux is changed later on
    // Store the data
    m_current_beam_spectrum.push_back(filtered_bin);

    // The total energy of the bin is above the cut-off threshold
    // Store the data
    m_unfiltered_beam_spectrum.push_back(unfiltered_bin);

    // Sort the spectrum
    sort();

    if (unfiltered_bin.m_photon_number * unfiltered_bin.m_energy >= m_bin_total_energy_lower_threshold)
    {
        return true;
    }
    else
    {
        return false;
    }
}


//-----------------------------------------------------------------------
inline void XRayBeam::initialise(const RATIONAL_NUMBER& anIncidentEnergy)
//-----------------------------------------------------------------------
{
	// Empty the current beam spectrum
	m_unfiltered_beam_spectrum.clear();
	m_current_beam_spectrum.clear();

    // Clear the filtration
    m_filtration.clear();

	// Add the record
	m_current_beam_spectrum.push_back(SpectrumRecord(1, anIncidentEnergy));
	m_unfiltered_beam_spectrum.push_back(SpectrumRecord(1, anIncidentEnergy));

    m_photon_count_type = PhotonCountType::PER_PIXEL_AT_SDD;

    m_voltage = DEFAULT_VOLTAGE;
    m_tube_angle_in_degrees = DEFAULT_TUBE_ANGLE;
    m_mAs = DEFAULT_mAs;
}


//------------------------------------------------------------------------------
inline void XRayBeam::setBinTotalEnergyCutoff(const RATIONAL_NUMBER& aThreshold)
//------------------------------------------------------------------------------
{
		m_bin_total_energy_lower_threshold = aThreshold;
}


//----------------------------------------------------
inline unsigned int XRayBeam::getEnergyChannelNumber()
//----------------------------------------------------
{
    unsigned int count(0);

    for (std::vector<SpectrumRecord>::const_iterator ite_beam_spectrum(m_current_beam_spectrum.begin());
        ite_beam_spectrum != m_current_beam_spectrum.end();
        ++ite_beam_spectrum)
    {
        RATIONAL_NUMBER bin_total_energy = ite_beam_spectrum->m_energy * ite_beam_spectrum->m_photon_number;

        // Apply the energy threshold
        if (bin_total_energy >= m_bin_total_energy_lower_threshold)
            count++;
    }

    return (count);
}


//-------------------------------------------------------
inline std::vector<double> XRayBeam::getEnergySet() const
//-------------------------------------------------------
{
    std::vector<double> energy_set;

    for (std::vector<SpectrumRecord>::const_iterator ite_beam_spectrum(m_current_beam_spectrum.begin());
        ite_beam_spectrum != m_current_beam_spectrum.end();
        ++ite_beam_spectrum)
    {
        RATIONAL_NUMBER bin_total_energy = ite_beam_spectrum->m_energy * ite_beam_spectrum->m_photon_number;

        // Apply the energy threshold
        if (bin_total_energy >= m_bin_total_energy_lower_threshold)
            energy_set.push_back(ite_beam_spectrum->m_energy);
    }

    return energy_set;
}


//------------------------------------------------------------------------------
inline SpectrumRecord XRayBeam::getEnergyChannel(unsigned int anID,
                                                 const XRayDetector* apDetector,
                                                 bool anApplyFiltrationFlag)
//------------------------------------------------------------------------------
{
    LOGGER.logWarning(std::string(__FUNCTION__) + 
        " is deprecated. It will be removed in a future release. "
        "Choose getEnergyChannelPerPixelAtSDD or setNumberOfPhotonsPerCm2At1m. "
        "Note that getEnergyChannel and getEnergyChannelPerPixelAtSDD are equivalent.") << std::endl;

	return getEnergyChannelPerPixelAtSDD(anID, apDetector, anApplyFiltrationFlag);
}


//------------------------------------------------------------------------------------------------
inline int XRayBeam::getBinID(const std::vector<SpectrumRecord>& aBeamSpectrum, unsigned int anID)
//------------------------------------------------------------------------------------------------
{
    int count = -1;
    unsigned int without_threshold_id = 0;

    while (without_threshold_id < aBeamSpectrum.size()) {

        SpectrumRecord record(aBeamSpectrum[without_threshold_id]);

        if (record.m_energy * record.m_photon_number >= m_bin_total_energy_lower_threshold) {
            ++count;

            if (anID == static_cast<unsigned int>(count)) {
                return without_threshold_id;
            }
        }

        ++without_threshold_id;
    }

    return -1;
}


//-------------------------------------------------------------------------------------------
inline SpectrumRecord XRayBeam::getEnergyChannelPerPixelAtSDD(unsigned int anID,
                                                              const XRayDetector* apDetector,
                                                              bool anApplyFiltrationFlag)
//-------------------------------------------------------------------------------------------
{
    if (anID >= getEnergyChannelNumber())
    {
        throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    if (m_photon_count_type == PhotonCountType::UNKNOWN)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
            "Unknown photon count type, it should be either per pixel at SDD or cm2 at 1m");
    }

    SpectrumRecord record;
    if (anApplyFiltrationFlag) {
        int bin_id = getBinID(m_current_beam_spectrum, anID);

        if (bin_id < 0)
            throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);

        record = m_current_beam_spectrum[bin_id];
    }
    else {
        int bin_id = getBinID(m_unfiltered_beam_spectrum, anID);

        if (bin_id < 0)
            throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);

        record = m_unfiltered_beam_spectrum[bin_id];
    }

    if (m_photon_count_type == PhotonCountType::PER_PIXEL_AT_SDD)
        return record;
    
    if (!apDetector)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
            "The detector must be known to convert the number of photons");
    }

    return SpectrumRecord(apDetector->convertPerCm2At1m2PerPixelAtSDD(record.getPhotonNumber()),
        record.getPhotonEnergy());
}


//----------------------------------------------------------------------------------------
inline SpectrumRecord XRayBeam::getEnergyChannelPerCm2At1m(unsigned int anID,
                                                           const XRayDetector* apDetector,
                                                           bool anApplyFiltrationFlag)
//----------------------------------------------------------------------------------------
{
    if (anID >= getEnergyChannelNumber())
    {
        throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    if (m_photon_count_type == PhotonCountType::UNKNOWN)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
            "Unknown photon count type, it should be either per pixel at SDD or cm2 at 1m");
    }

    SpectrumRecord record;
    if (anApplyFiltrationFlag) {
        int bin_id = getBinID(m_current_beam_spectrum, anID);

        if (bin_id < 0)
            throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);

        record = m_current_beam_spectrum[bin_id];
    }
    else {
        int bin_id = getBinID(m_unfiltered_beam_spectrum, anID);

        if (bin_id < 0)
            throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);

        record = m_unfiltered_beam_spectrum[bin_id];
    }

    if (m_photon_count_type == PhotonCountType::PER_CM2_AT_1M)
        return record;
    
    if (!apDetector)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
            "The detector must be known to convert the number of photons");
    }

    return SpectrumRecord(apDetector->convertPerPixelAtSDD2PerCm2At1m(record.getPhotonNumber()),
        record.getPhotonEnergy());
}


//-----------------------------------------------
inline RATIONAL_NUMBER XRayBeam::getTotalEnergy()
//-----------------------------------------------
{
    // Compute the total incident energy
    RATIONAL_NUMBER total_energy(0);
    for (std::vector<SpectrumRecord>::const_iterator ite_beam_spectrum(m_current_beam_spectrum.begin());
        ite_beam_spectrum != m_current_beam_spectrum.end();
        ++ite_beam_spectrum)
    {
        RATIONAL_NUMBER bin_total_energy = ite_beam_spectrum->m_energy * ite_beam_spectrum->m_photon_number;

        // Apply the energy threshold
        if (bin_total_energy >= m_bin_total_energy_lower_threshold)
            total_energy += bin_total_energy;
    }

    return (total_energy);
}


//--------------------------------------------------------------------------
    /// Set the value of lambda in a Poisson distribution where a Gaussian 
    /// distribution is a good enough approximation of the Poisson distribution 
    /// the number of photons to add the Poisson noise.
    /**
     *  @param aCutoffValue:    the cutoff value of lambda.
     */
//-------------------------------------------------------------------------
inline void XRayBeam::setLambdaCutoff(const RATIONAL_NUMBER& aLambdaCutoff)
//-------------------------------------------------------------------------
{
    m_lambda_cutoff = aLambdaCutoff;
}


//-------------------------------------------------------------
inline const RATIONAL_NUMBER& XRayBeam::getLambdaCutoff() const
//-------------------------------------------------------------
{
    return m_lambda_cutoff;
}


//----------------------------------------
inline void XRayBeam::enablePoissonNoise()
//----------------------------------------
{
    m_use_poisson_noise = true;
}


//-----------------------------------------
inline void XRayBeam::disablePoissonNoise()
//-----------------------------------------
{
    m_use_poisson_noise = false;
}


//------------------------------------------------------------------------
inline void XRayBeam::setNumberOfPhotons(RATIONAL_NUMBER aNumberOfPhotons,
                                         bool anApplyFiltrationFlag)
//------------------------------------------------------------------------
{
    LOGGER.logWarning(std::string(__FUNCTION__) + 
        " is deprecated. It will be removed in a future release. "
        "Choose setNumberOfPhotonsPerPixelAtSDD or setNumberOfPhotonsPerCm2At1m. "
        "Note that setNumberOfPhotons and setNumberOfPhotonsPerPixelAtSDD are equivalent.") << std::endl;
    setNumberOfPhotonsPerPixelAtSDD(aNumberOfPhotons, anApplyFiltrationFlag);
}


//-------------------------------------------------------------------------------------
inline void XRayBeam::setNumberOfPhotonsPerPixelAtSDD(RATIONAL_NUMBER aNumberOfPhotons,
                                                      bool anApplyFiltrationFlag)
//-------------------------------------------------------------------------------------
{
    // Change its value
    m_photon_count_type = PhotonCountType::PER_PIXEL_AT_SDD;

    // Compute the current number of photons
    double number_of_photons = 0.0;

    if (anApplyFiltrationFlag)
    {
        for (std::vector<SpectrumRecord>::const_iterator ite_beam_spectrum(m_current_beam_spectrum.begin());
           ite_beam_spectrum != m_current_beam_spectrum.end();
           ++ite_beam_spectrum)
        {
            number_of_photons += ite_beam_spectrum->m_photon_number;
        }
    }
    else
    {
        for (std::vector<SpectrumRecord>::const_iterator ite_beam_spectrum(m_unfiltered_beam_spectrum.begin());
           ite_beam_spectrum != m_unfiltered_beam_spectrum.end();
           ++ite_beam_spectrum)
        {
            number_of_photons += ite_beam_spectrum->m_photon_number;
        }
    }

    // Compute a scaling factor
    double old_to_new = aNumberOfPhotons / number_of_photons;

    // Apply this scaling factor on all the bins
    for (std::vector<SpectrumRecord>::iterator ite_beam_spectrum(m_unfiltered_beam_spectrum.begin());
        ite_beam_spectrum != m_unfiltered_beam_spectrum.end();
        ++ite_beam_spectrum)
    {
        ite_beam_spectrum->m_photon_number *= old_to_new;
    }

    for (std::vector<SpectrumRecord>::iterator ite_beam_spectrum(m_current_beam_spectrum.begin());
        ite_beam_spectrum != m_current_beam_spectrum.end();
        ++ite_beam_spectrum)
    {
        ite_beam_spectrum->m_photon_number *= old_to_new;
    }
}


//----------------------------------------------------------------------------------
inline void XRayBeam::setNumberOfPhotonsPerCm2At1m(RATIONAL_NUMBER aNumberOfPhotons,
                                                   bool anApplyFiltrationFlag)
//----------------------------------------------------------------------------------
{
    // Change its value
    m_photon_count_type = PhotonCountType::PER_CM2_AT_1M;

    // Compute the current number of photons
    double number_of_photons = 0.0;

    if (anApplyFiltrationFlag)
    {
        for (std::vector<SpectrumRecord>::const_iterator ite_beam_spectrum(m_current_beam_spectrum.begin());
            ite_beam_spectrum != m_current_beam_spectrum.end();
            ++ite_beam_spectrum)
        {
            number_of_photons += ite_beam_spectrum->m_photon_number;
        }
    }
    else
    {
        for (std::vector<SpectrumRecord>::const_iterator ite_beam_spectrum(m_unfiltered_beam_spectrum.begin());
            ite_beam_spectrum != m_unfiltered_beam_spectrum.end();
            ++ite_beam_spectrum)
        {
            number_of_photons += ite_beam_spectrum->m_photon_number;
        }
    }

    // Compute a scaling factor
    double old_to_new = aNumberOfPhotons / number_of_photons;

    // Apply this scaling factor on all the bins
    for (std::vector<SpectrumRecord>::iterator ite_beam_spectrum(m_unfiltered_beam_spectrum.begin());
        ite_beam_spectrum != m_unfiltered_beam_spectrum.end();
        ++ite_beam_spectrum)
    {
        ite_beam_spectrum->m_photon_number *= old_to_new;
    }

    for (std::vector<SpectrumRecord>::iterator ite_beam_spectrum(m_current_beam_spectrum.begin());
        ite_beam_spectrum != m_current_beam_spectrum.end();
        ++ite_beam_spectrum)
    {
        ite_beam_spectrum->m_photon_number *= old_to_new;
    }
}


//---------------------------------------------------------------------------------
inline RATIONAL_NUMBER XRayBeam::getNumberOfPhotons(const XRayDetector* apDetector,
                                                    bool anApplyFiltrationFlag)
//---------------------------------------------------------------------------------
{
    LOGGER.logWarning(std::string(__FUNCTION__) + 
        " is deprecated. It will be removed in a future release. "
        "Choose getNumberOfPhotonsPerPixelAtSDD or getNumberOfPhotonsPerCm2At1m. "
        "Note that getNumberOfPhotons and getNumberOfPhotonsPerPixelAtSDD are equivalent.") << std::endl;
    return getNumberOfPhotonsPerPixelAtSDD(apDetector, anApplyFiltrationFlag);
}


//----------------------------------------------------------------------------------------------
inline RATIONAL_NUMBER XRayBeam::getNumberOfPhotonsPerPixelAtSDD(const XRayDetector* apDetector,
                                                                 bool anApplyFiltrationFlag)
//----------------------------------------------------------------------------------------------
{
    switch (m_photon_count_type)
    {
        case PhotonCountType::PER_PIXEL_AT_SDD:
            {
                // Compute the current number of photons
                double number_of_photons = 0.0;

                if (anApplyFiltrationFlag)
                {
                    for (std::vector<SpectrumRecord>::const_iterator ite_beam_spectrum(m_current_beam_spectrum.begin());
                        ite_beam_spectrum != m_current_beam_spectrum.end();
                        ++ite_beam_spectrum)
                    {
                        if (ite_beam_spectrum->m_photon_number * ite_beam_spectrum->m_photon_number >= m_bin_total_energy_lower_threshold)
                            number_of_photons += ite_beam_spectrum->m_photon_number;
                    }
                }
                else
                {
                    for (std::vector<SpectrumRecord>::const_iterator ite_beam_spectrum(m_unfiltered_beam_spectrum.begin());
                        ite_beam_spectrum != m_unfiltered_beam_spectrum.end();
                        ++ite_beam_spectrum)
                    {
                        if (ite_beam_spectrum->m_photon_number * ite_beam_spectrum->m_photon_number >= m_bin_total_energy_lower_threshold)
                            number_of_photons += ite_beam_spectrum->m_photon_number;
                    }
                }

                return number_of_photons;
            }
        
        case PhotonCountType::PER_CM2_AT_1M:
            {
                // Convert the number of photons per cm2 at 1m into
                // the number of photons per pixel at source-detector distance
                if (!apDetector)
                {
                    throw Exception(__FILE__, __FUNCTION__, __LINE__,
                        "The detector must be known to convert the number of photons");
                }

                // Compute the current number of photons
                double number_of_photons = 0.0;

                if (anApplyFiltrationFlag)
                {
                    for (std::vector<SpectrumRecord>::const_iterator ite_beam_spectrum(m_current_beam_spectrum.begin());
                        ite_beam_spectrum != m_current_beam_spectrum.end();
                        ++ite_beam_spectrum)
                    {
                        if (ite_beam_spectrum->m_photon_number * ite_beam_spectrum->m_photon_number >= m_bin_total_energy_lower_threshold)
                            number_of_photons += apDetector->convertPerCm2At1m2PerPixelAtSDD(ite_beam_spectrum->m_photon_number);
                    }
                }
                else
                {
                    for (std::vector<SpectrumRecord>::const_iterator ite_beam_spectrum(m_unfiltered_beam_spectrum.begin());
                        ite_beam_spectrum != m_unfiltered_beam_spectrum.end();
                        ++ite_beam_spectrum)
                    {
                        if (ite_beam_spectrum->m_photon_number * ite_beam_spectrum->m_photon_number >= m_bin_total_energy_lower_threshold)
                            number_of_photons += apDetector->convertPerCm2At1m2PerPixelAtSDD(ite_beam_spectrum->m_photon_number);
                    }
                }

                return number_of_photons;
            }

        case PhotonCountType::UNKNOWN:
            return 0;

        default:
            return 0;
    }
}


//-------------------------------------------------------------------------------------------
inline RATIONAL_NUMBER XRayBeam::getNumberOfPhotonsPerCm2At1m(const XRayDetector* apDetector,
                                                              bool anApplyFiltrationFlag)
//-------------------------------------------------------------------------------------------
{
    switch (m_photon_count_type)
    {
        case PhotonCountType::PER_PIXEL_AT_SDD:
            {
                // Convert the number of photons per pixel at source-detector distance into
                // the number of photons per cm2 at 1m into
                if (!apDetector)
                {
                    throw Exception(__FILE__, __FUNCTION__, __LINE__,
                        "The detector must be known to convert the number of photons");
                }

                // Compute the current number of photons
                double number_of_photons = 0.0;

                if (anApplyFiltrationFlag)
                {
                    for (std::vector<SpectrumRecord>::const_iterator ite_beam_spectrum(m_current_beam_spectrum.begin());
                        ite_beam_spectrum != m_current_beam_spectrum.end();
                        ++ite_beam_spectrum)
                    {
                        if (ite_beam_spectrum->m_photon_number * ite_beam_spectrum->m_photon_number >= m_bin_total_energy_lower_threshold)
                            number_of_photons += apDetector->convertPerPixelAtSDD2PerCm2At1m(ite_beam_spectrum->m_photon_number);
                    }
                }
                else
                {
                    for (std::vector<SpectrumRecord>::const_iterator ite_beam_spectrum(m_unfiltered_beam_spectrum.begin());
                        ite_beam_spectrum != m_unfiltered_beam_spectrum.end();
                        ++ite_beam_spectrum)
                    {
                        if (ite_beam_spectrum->m_photon_number * ite_beam_spectrum->m_photon_number >= m_bin_total_energy_lower_threshold)
                            number_of_photons += apDetector->convertPerPixelAtSDD2PerCm2At1m(ite_beam_spectrum->m_photon_number);
                    }
                }

                return number_of_photons;
            }

        case PhotonCountType::PER_CM2_AT_1M:
            {
                // Compute the current number of photons
                double number_of_photons = 0.0;

                if (anApplyFiltrationFlag)
                {
                    for (std::vector<SpectrumRecord>::const_iterator ite_beam_spectrum(m_current_beam_spectrum.begin());
                        ite_beam_spectrum != m_current_beam_spectrum.end();
                        ++ite_beam_spectrum)
                    {
                        if (ite_beam_spectrum->m_photon_number * ite_beam_spectrum->m_photon_number >= m_bin_total_energy_lower_threshold)
                            number_of_photons += ite_beam_spectrum->m_photon_number;
                    }
                }
                else
                {
                    for (std::vector<SpectrumRecord>::const_iterator ite_beam_spectrum(m_unfiltered_beam_spectrum.begin());
                        ite_beam_spectrum != m_unfiltered_beam_spectrum.end();
                        ++ite_beam_spectrum)
                    {
                        if (ite_beam_spectrum->m_photon_number * ite_beam_spectrum->m_photon_number >= m_bin_total_energy_lower_threshold)
                            number_of_photons += ite_beam_spectrum->m_photon_number;
                    }
                }

                return number_of_photons;
            }

        case PhotonCountType::UNKNOWN:
            return 0;

        default:
            return 0;
    }
}


//-----------------------------------------------
inline void XRayBeam::usePoissonNoise(bool aFlag)
//-----------------------------------------------
{
    m_use_poisson_noise = aFlag;
}


//-------------------------------------------
inline bool XRayBeam::usePoissonNoise() const
//-------------------------------------------
{
    return (m_use_poisson_noise);
}


//--------------------------
inline void XRayBeam::sort()
//--------------------------
{
    std::sort(m_current_beam_spectrum.begin(), m_current_beam_spectrum.end());
    std::sort(m_unfiltered_beam_spectrum.begin(), m_unfiltered_beam_spectrum.end());
}


//------------------------------------------------------
inline size_t XRayBeam::getNumberOfPhotonCountingBands()
//------------------------------------------------------
{
    return m_beam_spectrum_for_photon_count_detector.size();
}


//------------------------------------------------------------------
inline void XRayBeam::activatePhotonCountingBand(unsigned int aBand)
//------------------------------------------------------------------
{
    // The functionality is in use
    if (getNumberOfPhotonCountingBands())
    {
        // aBand is valid
        if (aBand > getNumberOfPhotonCountingBands())
        {
            throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
        }

        // Replace the current spectrum with the corresponding band
        m_current_beam_spectrum = m_beam_spectrum_for_photon_count_detector[aBand];

        if (m_photon_count_type == PhotonCountType::UNKNOWN)
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__,
                "Unknown photon count type, it should be either per pixel at SDD or cm2 at 1m");
        }
    }
}


//---------------------------------------------------------------------------
inline const std::vector<RATIONAL_NUMBER>& XRayBeam::getPhotonCountingBands()
//---------------------------------------------------------------------------
{
    return m_photon_count_bands;
}


//---------------------------------------------------------------
inline void XRayBeam::setVoltage(const RATIONAL_NUMBER& aVoltage)
//---------------------------------------------------------------
{
    m_voltage = aVoltage;

    generateTubeSpectrum();
}


//-------------------------------------------------
inline RATIONAL_NUMBER XRayBeam::getVoltage() const
//-------------------------------------------------
{
    return m_voltage;
}


//------------------------------------------------------------------------
inline void XRayBeam::addInherentFilter(unsigned int aMaterial,
                                        const RATIONAL_NUMBER& aThickness)
//------------------------------------------------------------------------
{
    m_inherent_filtration.push_back(std::pair<unsigned int, RATIONAL_NUMBER>(aMaterial, aThickness));

    generateTubeSpectrum();
}


//---------------------------------------------
inline void XRayBeam::clearInherentFiltration()
//---------------------------------------------
{
    m_inherent_filtration.clear();

    generateTubeSpectrum();
}


//----------------------------------------------------------------------------------------------------------
inline const std::vector<std::pair<unsigned int, RATIONAL_NUMBER> >& XRayBeam::getInherentFiltration() const
//----------------------------------------------------------------------------------------------------------
{
    return m_inherent_filtration;
}


//---------------------------------------------------------
inline void XRayBeam::setmAs(const RATIONAL_NUMBER& an_mAs)
//---------------------------------------------------------
{
    m_mAs = an_mAs;

    generateTubeSpectrum();
}


//---------------------------------------------
inline RATIONAL_NUMBER XRayBeam::getmAs() const
//---------------------------------------------
{
    return m_mAs;
}


//----------------------------------------------------------------
inline void XRayBeam::setTubeAngle(const RATIONAL_NUMBER& anAngle)
//----------------------------------------------------------------
{
    m_tube_angle_in_degrees = anAngle;

    generateTubeSpectrum();
}


//---------------------------------------------------
inline RATIONAL_NUMBER XRayBeam::getTubeAngle() const
//---------------------------------------------------
{
    return m_tube_angle_in_degrees;
}


//-----------------------------------------------------------------------------
inline void XRayBeam::setFirstEnergyBin(const RATIONAL_NUMBER& aFirstEnergyBin)
//-----------------------------------------------------------------------------
{
    
    m_first_energy_bin = std::max(RATIONAL_NUMBER(1.0 * keV), aFirstEnergyBin);

    generateTubeSpectrum();
}


//------------------------------------------------------------------
inline void XRayBeam::setEnergyBinSize(const float& anEnergyBinSize)
//------------------------------------------------------------------
{
    if (anEnergyBinSize < 0.0)
    {
        throw Exception(__FILE__, __FILE__, __LINE__,
            "The size of an energy bin cannot be negative");
    }

    m_size_energy_bin = anEnergyBinSize;

    generateTubeSpectrum();
}


//---------------------------------------------
inline float XRayBeam::getEnergyBinSize() const
//---------------------------------------------
{
    return m_size_energy_bin;
}


//-------------------------------------
inline void XRayBeam::clearFiltration()
//-------------------------------------
{
    // Restore the original spectrum if it exists
    if (m_unfiltered_beam_spectrum.size() && m_filtration.size())
    {
        m_current_beam_spectrum = m_unfiltered_beam_spectrum;
    }

    // Remove the filtration
    m_filtration.clear();
}


//--------------------------------------------------------------------------------------------------------------
inline void XRayBeam::setFiltration(const std::vector<std::pair<unsigned int, RATIONAL_NUMBER> >& aSetOfFilters)
//--------------------------------------------------------------------------------------------------------------
{
    m_filtration = aSetOfFilters;

    if (!m_unfiltered_beam_spectrum.size())
    {
        m_unfiltered_beam_spectrum = m_current_beam_spectrum;
    }

    applyFiltrationOnSpectrum();
}


//-----------------------------------------------------------
inline void XRayBeam::addFilter(const std::string& aMaterial,
                                const double& aThickness)
//-----------------------------------------------------------
{
    m_filtration.push_back(
        std::pair<unsigned int, RATIONAL_NUMBER>(
            ElementSet::getInstance().getElement(aMaterial).getZ(),
            RATIONAL_NUMBER(aThickness)
        )
    );

    if (!m_unfiltered_beam_spectrum.size() && m_current_beam_spectrum.size())
    {
        m_unfiltered_beam_spectrum = m_current_beam_spectrum;
    }

    applyFiltrationOnSpectrum();
}


//-------------------------------------------------------
inline void XRayBeam::addFilter(unsigned int aMaterial,
                                const double& aThickness)
//-------------------------------------------------------
{
    m_filtration.push_back(
        std::pair<int, RATIONAL_NUMBER>(
            aMaterial, 
            RATIONAL_NUMBER(aThickness)
        )
    );
        
    if (!m_unfiltered_beam_spectrum.size())
    {
        m_unfiltered_beam_spectrum = m_current_beam_spectrum;
    }

    applyFiltrationOnSpectrum();
}


//--------------------------------------------------------------------------------------------------
inline const std::vector<std::pair<unsigned int, RATIONAL_NUMBER> >& XRayBeam::getFiltration() const
//--------------------------------------------------------------------------------------------------
{
    return m_filtration;
}


} // namespace gVirtualXRay
