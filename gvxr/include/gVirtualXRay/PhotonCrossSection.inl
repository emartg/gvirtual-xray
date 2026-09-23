/*

 Copyright (c) 2018, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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
 *   @file       PhotonCrossSection.inl
 *
 *   @brief      Class to manage photon cross sections of elements, compounds and mixtures.
 *
 *   @version    1.0
 *
 *   @date       27/09/2018
 *
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
#include <string>

#ifndef GVXR_NOT_IMPLEMENTED_EXCEPTION_H
#include "gVirtualXRay/NotImplementedException.h"
#endif


//******************************************************************************
//  defines
//******************************************************************************


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


inline std::ostream& operator<<(std::ostream& anOuputStream,
                                const PhotonCrossSection& anInstance)
{
    anOuputStream << "m_use_HU: " << anInstance.m_use_HU << std::endl;
    anOuputStream << "m_use_element: " << anInstance.m_use_element << std::endl;
    anOuputStream << "m_use_mixture: " << anInstance.m_use_mixture << std::endl;
    anOuputStream << "m_use_element: " << anInstance.m_use_element << std::endl;
    anOuputStream << "m_use_mass_attenuation_coefficient: " << anInstance.m_use_mass_attenuation_coefficient << std::endl;
    anOuputStream << "m_use_linear_attenuation_coefficient: " << anInstance.m_use_linear_attenuation_coefficient << std::endl;

    anOuputStream << "m_HU: " << anInstance.m_HU << std::endl;
    anOuputStream << "m_Z_number: " << anInstance.m_Z_number << std::endl;
    anOuputStream << "m_mass_attenuation_coefficient: " << anInstance.m_mass_attenuation_coefficient << std::endl;
    anOuputStream << "m_linear_attenuation_coefficient: " << anInstance.m_linear_attenuation_coefficient << std::endl;
    anOuputStream << "m_mixture: " << anInstance.m_mixture << std::endl;
    anOuputStream << "m_density: " << anInstance.getDensity() << std::endl;
    anOuputStream << "m_label: " << anInstance.m_label << std::endl;

    return anOuputStream;
}


//--------------------------------------------------
inline PhotonCrossSection::PhotonCrossSection():
//--------------------------------------------------
        m_use_HU(false),
        m_use_element(false),
        m_use_mixture(false),
        m_use_mass_attenuation_coefficient(false),
        m_use_linear_attenuation_coefficient(false),
        m_HU(0),
        m_Z_number(0),
        m_mass_attenuation_coefficient(0.0),
        m_linear_attenuation_coefficient(0.0),
        m_density(-1.0)
//--------------------------------------------------
{}


//-------------------------------------------------------------------------------------
inline PhotonCrossSection::PhotonCrossSection(unsigned short Z):
//-------------------------------------------------------------------------------------
        m_use_HU(false),
        m_use_element(true),
        m_use_mixture(false),
        m_use_mass_attenuation_coefficient(false),
        m_use_linear_attenuation_coefficient(false),
        m_HU(0),
        m_Z_number(Z),
        m_mass_attenuation_coefficient(0.0),
        m_linear_attenuation_coefficient(0.0),
        m_density(-1.0),
        m_label(std::string("Element: ") + ElementSet::getInstance().getElement(Z).getName())
//-------------------------------------------------------------------------------------
{
}


//-------------------------------------------------------------------------------------
inline PhotonCrossSection::PhotonCrossSection(const std::map<int, double>& aWeightSet):
//-------------------------------------------------------------------------------------
        m_use_HU(false),
        m_use_element(false),
        m_use_mixture(true),
        m_use_mass_attenuation_coefficient(false),
        m_use_linear_attenuation_coefficient(false),
        m_HU(0),
        m_Z_number(0),
        m_mass_attenuation_coefficient(0.0),
        m_linear_attenuation_coefficient(0.0),
        m_mixture(aWeightSet),
        m_density(-1.0),
        m_label(std::string("Mixture: ") + m_mixture.getLabel())
//-------------------------------------------------------------------------------------
{
}


//---------------------------------------------------------------------------------------------
inline PhotonCrossSection::PhotonCrossSection(const std::map<std::string, double>& aWeightSet):
//---------------------------------------------------------------------------------------------
        m_use_HU(false),
        m_use_element(false),
        m_use_mixture(true),
        m_use_mass_attenuation_coefficient(false),
        m_use_linear_attenuation_coefficient(false),
        m_HU(0),
        m_Z_number(0),
        m_mass_attenuation_coefficient(0.0),
        m_linear_attenuation_coefficient(0.0),
        m_mixture(aWeightSet),
        m_density(-1.0),
        m_label(std::string("Mixture: ") + m_mixture.getLabel())
//---------------------------------------------------------------------------------------------
{
}


//---------------------------------------------------------------------
inline PhotonCrossSection::PhotonCrossSection(const Mixture& aMixture):
//---------------------------------------------------------------------
        m_use_HU(false),
        m_use_element(false),
        m_use_mixture(true),
        m_use_mass_attenuation_coefficient(false),
        m_use_linear_attenuation_coefficient(false),
        m_HU(0),
        m_Z_number(0),
        m_mass_attenuation_coefficient(0.0),
        m_linear_attenuation_coefficient(0.0),
        m_mixture(aMixture),
        m_density(-1.0),
        m_label(std::string("Mixture: ") + m_mixture.getLabel())
//---------------------------------------------------------------------
{}


//-------------------------------------
inline void PhotonCrossSection::reset()
//-------------------------------------
{
    m_use_HU      = false;
    m_use_element = false;
    m_use_mixture = false;
    m_use_mass_attenuation_coefficient   = false;
    m_use_linear_attenuation_coefficient = false;

    m_density = -1.0;
    m_label.clear();
}


//---------------------------------------------
inline void PhotonCrossSection::useHU(short HU)
//---------------------------------------------
{
    reset();
    m_use_HU      = true;
    m_HU = HU;

    m_label = "HU: ";
    m_label += std::to_string(HU);
}


//---------------------------------------------------------
inline void PhotonCrossSection::useHounsfieldUnit(short HU)
//---------------------------------------------------------
{
    useHU(HU);
}


//----------------------------------------------------------
inline void PhotonCrossSection::useHounsfieldValue(short HU)
//----------------------------------------------------------
{
    useHU(HU);
}



//-----------------------------------------------------------------
inline void PhotonCrossSection::useMixture(const Mixture& aMixture)
//-----------------------------------------------------------------
{
    reset();
    m_use_mixture = true;
    m_mixture = aMixture;

    m_label = "Mixture: ";
    m_label += m_mixture.getLabel();
}


//-------------------------------------------------------------------------------
inline void PhotonCrossSection::useMixture(const std::map<int, double>& aMixture)
//-------------------------------------------------------------------------------
{
    reset();
    m_use_mixture = true;
    m_mixture.setMixture(aMixture);

    m_label = "Mixture: ";
    m_label += m_mixture.getLabel();
}


//---------------------------------------------------------------------------------------
inline void PhotonCrossSection::useMixture(const std::map<std::string, double>& aMixture)
//---------------------------------------------------------------------------------------
{
    reset();
    m_use_mixture = true;
    m_mixture.setMixture(aMixture);

    m_label = "Mixture: ";
    m_label += m_mixture.getLabel();
}


//--------------------------------------------------------------------------------
inline void PhotonCrossSection::useMassAttenuationCoefficient(double aCoefficient)
//--------------------------------------------------------------------------------
{
    reset();
    m_use_mass_attenuation_coefficient   = true;
    m_mass_attenuation_coefficient = aCoefficient;

    m_label = "mu/rho: ";
    m_label += std::to_string(aCoefficient);
}


//----------------------------------------------------------------------------------
inline void PhotonCrossSection::useLinearAttenuationCoefficient(double aCoefficient)
//----------------------------------------------------------------------------------
{
    reset();
    double inv_cm = 1.0 / cm;

    m_use_linear_attenuation_coefficient = true;
    m_linear_attenuation_coefficient = aCoefficient;


    m_label = "mu: ";
    m_label += std::to_string(aCoefficient / inv_cm);
    m_label += " cm-1";
}


//-----------------------------------------------------------
inline double PhotonCrossSection::getEffectiveZNumber() const
//-----------------------------------------------------------
{
    if (useElement())
    {
        return m_Z_number;
    }
    else if (useHU()) {
        throw NotImplementedException(__FILE__, __FUNCTION__, __LINE__);
    }
    else
    {
        return m_mixture.getEffectiveAtomicNumber();
    }
}


//----------------------------------------------------------------
inline double PhotonCrossSection::getEffectiveAtomicNumber() const
//----------------------------------------------------------------
{
    return getEffectiveZNumber();
}


//-------------------------------------------------------------
inline double PhotonCrossSection::getEffectiveMolarMass() const
//-------------------------------------------------------------
{
    if (useElement())
    {
        return ElementSet::getInstance().getElement(m_Z_number).getMolarMass();;
    }
    else if (useHU()) {
        throw NotImplementedException(__FILE__, __FUNCTION__, __LINE__);
    }
    else
    {
        return m_mixture.getMolarMass();
    }
}


//-------------------------------------------
inline bool PhotonCrossSection::useHU() const
//-------------------------------------------
{
    return m_use_HU;
}


//-------------------------------------------------------
inline bool PhotonCrossSection::useHounsfieldUnit() const
//-------------------------------------------------------
{
    return m_use_HU;
}


//--------------------------------------------------------
inline bool PhotonCrossSection::useHounsfieldValue() const
//--------------------------------------------------------
{
    return m_use_HU;
}


//------------------------------------------------
inline bool PhotonCrossSection::useElement() const
//------------------------------------------------
{
    return m_use_element;
}


//------------------------------------------------
inline bool PhotonCrossSection::useMixture() const
//------------------------------------------------
{
    return m_use_mixture;
}


//-------------------------------------------------------------------
inline bool PhotonCrossSection::useMassAttenuationCoefficient() const
//-------------------------------------------------------------------
{
    return m_use_mass_attenuation_coefficient;
}


//---------------------------------------------------------------------
inline bool PhotonCrossSection::useLinearAttenuationCoefficient() const
//---------------------------------------------------------------------
{
    return m_use_linear_attenuation_coefficient;
}


//---------------------------------------------------------
inline void PhotonCrossSection::setDensity(double aDensity)
//---------------------------------------------------------
{
    m_density = aDensity;
}


//------------------------------------------------------------
inline double PhotonCrossSection::getMu(double anEnergy) const
//------------------------------------------------------------
{
    return (getLinearAttenuationCoefficient(anEnergy));
}


//------------------------------------------------------------
inline const std::string& PhotonCrossSection::getLabel() const
//------------------------------------------------------------
{
    return (m_label);
}


//-----------------------------------------------------------------------------------
inline bool PhotonCrossSection::operator!=(const PhotonCrossSection& aTestData) const
//-----------------------------------------------------------------------------------
{
    return (!((*this) == aTestData));
}


//--------------------------------------------------------
inline std::string PhotonCrossSection::getCompound() const
//--------------------------------------------------------
{
    std::string composition;
    std::string prefix("Compound: ");
    size_t prefix_length(prefix.length());

    std::cout << m_label.length() << "\t" << prefix_length << std::endl;
    if (m_label.length() < prefix_length)
    {
        std::cout << m_label.substr(0, prefix_length) << " == " << prefix << " ? " << int(m_label.substr(0, prefix_length) == prefix)  << std::endl;
        if (m_label.substr(0, prefix_length) == prefix)
        {
            composition = m_label.substr(prefix_length, m_label.length() - prefix_length);
            std::cout << composition << std::endl;
        }
    }

    throw "NOT WORKING YET";
    return composition;
}


//----------------------------------------------------------------------
inline std::vector<int> PhotonCrossSection::getMixtureElementSet() const
//----------------------------------------------------------------------
{
    std::vector<int> return_value;

    if (m_use_mixture)
    {
        for (const auto& ite : m_mixture.getMassWeightSet())
        {
            return_value.push_back(ite.first);
        }
    }

    return return_value;
}


//------------------------------------------------------------------------
inline std::vector<double> PhotonCrossSection::getMixtureWeightSet() const
//------------------------------------------------------------------------
{
    std::vector<double> return_value;

    if (m_use_mixture)
    {
        for (const auto& ite : m_mixture.getMassWeightSet())
        {
            return_value.push_back(ite.second);
        }
    }

    return return_value;
}


} // namespace gVirtualXRay
