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
*   @file       AtomicElement.inl
*
*   @brief      Class to manage elements in material.
*
*   @version    1.0
*
*   @date       13/10/2014
*
*   @author     Dr Franck P. Vidal
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
//  Include
//******************************************************************************
#include <xraylib.h>

#ifndef GVXR_OUT_OF_BOUNDS_EXCEPTION_H
#include "gVirtualXRay/OutOfBoundsException.h"
#endif

#ifndef GVXR_UNITS_H
#include "gVirtualXRay/Units.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//-------------------------------------------------------------
inline AtomicElement::AtomicElement(const std::string& aName,
                                    const std::string& aSymbol,
                                    unsigned short aZNumber,
                                    double aMolarMass,
                                    double aDensity,
                                    const std::string& aState):
//-------------------------------------------------------------
        m_name(aName),
        m_symbol(aSymbol),
        m_atomic_number(aZNumber),
        m_molar_mass(aMolarMass),
        m_density(aDensity),
        m_state(aState)
//-------------------------------------------------------------
{
}


//------------------------------------------------------------------
inline AtomicElement::AtomicElement(const AtomicElement& anElement):
//------------------------------------------------------------------
        m_name(anElement.m_name),
        m_symbol(anElement.m_symbol),
        m_atomic_number(anElement.m_atomic_number),
        m_molar_mass(anElement.m_molar_mass),
        m_density(anElement.m_density),
        m_state(anElement.m_state),
        m_p_energy_set(anElement.m_p_energy_set),
        m_p_mass_attenuation_coefficient_set(anElement.m_p_mass_attenuation_coefficient_set)//,
        // m_p_coherent_scattering_set(anElement.m_p_coherent_scattering_set),
        // m_p_incoherent_scattering_set(anElement.m_p_incoherent_scattering_set),
        // m_p_photo_electric_absorption_set(anElement.m_p_photo_electric_absorption_set),
        // m_p_pair_production_in_nuclear_field_set(anElement.m_p_pair_production_in_nuclear_field_set),
        // m_p_pair_production_in_electron_field_set(anElement.m_p_pair_production_in_electron_field_set),
        // m_p_total_attenuation_without_coherent_scattering(anElement.m_p_total_attenuation_without_coherent_scattering)
//------------------------------------------------------------------
{
}


//------------------------------------
inline AtomicElement::~AtomicElement()
//------------------------------------
{
}


//----------------------------------------------------------
inline void AtomicElement::setName(const std::string& aName)
//----------------------------------------------------------
{
	m_name = aName;
}


//--------------------------------------------------------------
inline void AtomicElement::setSymbol(const std::string& aSymbol)
//--------------------------------------------------------------
{
	m_symbol = aSymbol;
}


//-----------------------------------------------------------------
inline void AtomicElement::setAtomicNumber(unsigned short aZNumber)
//-----------------------------------------------------------------
{
    m_atomic_number = aZNumber;
}


//--------------------------------------------------------
inline void AtomicElement::setMolarMass(double aMolarMass)
//--------------------------------------------------------
{
	m_molar_mass = aMolarMass;
}


//----------------------------------------------------
inline void AtomicElement::setDensity(double aDensity)
//----------------------------------------------------
{
	m_density = aDensity;
}


//------------------------------------------------------------
inline void AtomicElement::setState(const std::string& aState)
//------------------------------------------------------------
{
	m_state = aState;
}


//-------------------------------------------------
inline double AtomicElement::getMU(double anEnergy)
//-------------------------------------------------
{
    return (getMassAttenuationTotal(anEnergy) * m_density);
}


//-------------------------------------------------------------------
inline double AtomicElement::getMassAttenuationTotal(double anEnergy)
//-------------------------------------------------------------------
{
    return CS_Total(m_atomic_number, anEnergy / keV, 0) * (cm2 / g);
}


//------------------------------------------------------
inline const std::string& AtomicElement::getName() const
//------------------------------------------------------
{
    return (m_name);
}


//--------------------------------------------------------
inline const std::string& AtomicElement::getSymbol() const
//--------------------------------------------------------
{
    return (m_symbol);
}


//----------------------------------------------------------
inline unsigned short AtomicElement::getAtomicNumber() const
//----------------------------------------------------------
{
    return (m_atomic_number);
}


//-----------------------------------------------
inline unsigned short AtomicElement::getZ() const
//-----------------------------------------------
{
    return (m_atomic_number);
}


//--------------------------------------------
inline unsigned short AtomicElement::Z() const
//--------------------------------------------
{
    return (m_atomic_number);
}


//-----------------------------------------------
inline double AtomicElement::getMolarMass() const
//-----------------------------------------------
{
    return (m_molar_mass);
}


//---------------------------------------------
inline double AtomicElement::getDensity() const
//---------------------------------------------
{
    return (m_density);
}


//-------------------------------------------------------
inline const std::string& AtomicElement::getState() const
//-------------------------------------------------------
{
	return (m_state);
}


//---------------------------------------------------------------
inline unsigned int AtomicElement::getEnergyChannelNumber() const
//---------------------------------------------------------------
{
	return (m_p_energy_set.size());
}


//----------------------------------------------------------------------------
inline AtomicElement& AtomicElement::operator=(const AtomicElement& anElement)
//----------------------------------------------------------------------------
{
  	m_name          = anElement.m_name;
  	m_symbol        = anElement.m_symbol;
  	m_atomic_number = anElement.m_atomic_number;
  	m_molar_mass    = anElement.m_molar_mass;
  	m_density       = anElement.m_density;
  	m_state         = anElement.m_state;

    m_p_energy_set                                    = anElement.m_p_energy_set;
    m_p_mass_attenuation_coefficient_set              = anElement.m_p_mass_attenuation_coefficient_set;
    // m_p_coherent_scattering_set                       = anElement.m_p_coherent_scattering_set;
    // m_p_incoherent_scattering_set                     = anElement.m_p_incoherent_scattering_set;
    // m_p_photo_electric_absorption_set                 = anElement.m_p_photo_electric_absorption_set;
    // m_p_pair_production_in_nuclear_field_set          = anElement.m_p_pair_production_in_nuclear_field_set;
    // m_p_pair_production_in_electron_field_set         = anElement.m_p_pair_production_in_electron_field_set;
    // m_p_total_attenuation_without_coherent_scattering = anElement.m_p_total_attenuation_without_coherent_scattering;

    return (*this);
}


    inline double AtomicElement::getEnergyChannel(unsigned int i) const
    {
        if (i >= m_p_energy_set.size())
        {
            throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
        }

        return m_p_energy_set[i];
    }


    inline double AtomicElement::getMassAttenuationCoefficientChannel(unsigned int i) const
    {
        if (i >= m_p_mass_attenuation_coefficient_set.size())
        {
            throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
        }


        return m_p_mass_attenuation_coefficient_set[i];
    }


} // namespace gVirtualXRay
