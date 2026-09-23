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
*   @file       Mixture.inl
*
*   @brief      Class to manage a mixture (e.g. Ti90Al6V4).
*
*   @version    1.0
*
*   @date       27/09/2018
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
*   http://www.fpvidal.net/, Sept 2018, 2018, version 1.0, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
#ifndef GVXR_CONSTANT_VALUES_H
#include "gVirtualXRay/ConstantValues.h"
#endif

#ifndef GVXR_ELEMENT_SET_H
#include "gVirtualXRay/ElementSet.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//------------------------
inline Mixture::Mixture():
//------------------------
        m_molar_mass(0.0),
        m_density(-1.0)
//------------------------
{}


//---------------------------------------------------------------
inline Mixture::Mixture(const std::map<int, double>& aWeightSet):
//---------------------------------------------------------------
        m_p_mass_weight_set(aWeightSet),
        m_molar_mass(0.0),
        m_density(-1.0)
//---------------------------------------------------------------
{
	normaliseWeightSet();

	m_molar_mass = computeMolarMass();

    setLabel();
}


//-----------------------------------------------------------------------
inline Mixture::Mixture(const std::map<std::string, double>& aWeightSet):
//-----------------------------------------------------------------------
        m_molar_mass(0.0),
        m_density(-1.0)
//-----------------------------------------------------------------------
{
    setMixture(aWeightSet);
}


//--------------------------------------------------------------
inline Mixture::Mixture(const std::map<int, double>& aWeightSet,
                        double aMolarMass):
//--------------------------------------------------------------
        m_p_mass_weight_set(aWeightSet),
        m_density(-1.0)
//--------------------------------------------------------------
{
	normaliseWeightSet();

    m_molar_mass = aMolarMass;

    setLabel();
}


//----------------------------------------------------------------------
inline Mixture::Mixture(const std::map<std::string, double>& aWeightSet,
                        double aMolarMass):
//----------------------------------------------------------------------
        m_molar_mass(0.0),
        m_density(-1.0)
//----------------------------------------------------------------------
{
    setMixture(aWeightSet, aMolarMass);
}


//-----------------------------------------------
inline Mixture::Mixture(const Mixture& aMixture):
//-----------------------------------------------
        m_p_mass_weight_set(aMixture.m_p_mass_weight_set),
        m_p_electron_weight_set(aMixture.m_p_electron_weight_set),
        m_molar_mass(aMixture.m_molar_mass),
        m_density(aMixture.m_density),
        m_label(aMixture.m_label)
//-----------------------------------------------
{
}


//--------------------------
inline void Mixture::clear()
//--------------------------
{
    m_label.clear();
    m_p_mass_weight_set.clear();
    m_p_electron_weight_set.clear();

    m_molar_mass = 0.0;
    m_density = -1.0;
}


//----------------------------------------------------------------------
inline void Mixture::setMixture(const std::map<int, double>& aWeightSet)
//----------------------------------------------------------------------
{
    m_p_mass_weight_set = aWeightSet;
    normaliseWeightSet();
    m_molar_mass = computeMolarMass();

    setLabel();
}


//------------------------------------------------------------------------------
inline void Mixture::setMixture(const std::map<std::string, double>& aWeightSet)
//------------------------------------------------------------------------------
{
    std::map<int, double> mixture;

    int balance_Z = 0;
    double sum_weight(0.0);

    for (std::map<std::string, double>::const_iterator ite = aWeightSet.begin();
        ite != aWeightSet.end();
        ++ite)
    {
        // Retreive the Z number from the element name or symbol
        int Z = ElementSet::getInstance().getElement(ite->first).getZ();
        
        // The element's weight is unknown
        if (ite->second < -EPSILON)
        {
            balance_Z = Z;
        }
        // The element's weight is known
        else
        {
            mixture[Z] = ite->second;
            sum_weight += ite->second;
        }
    }

    // The weight of one of the element is the balance of all the other weights
    if (balance_Z)
    {
        // Assume the weights are given for the range [0, 100]
        if (sum_weight > 1.0)
        {
            mixture[balance_Z] = 100.0 - sum_weight;
        }
        // Assume the weights are given for the range [0, 1]
        else
        {
            mixture[balance_Z] = 1.0 - sum_weight;
        }
    }

    m_p_mass_weight_set = mixture;
    normaliseWeightSet();
    m_molar_mass = computeMolarMass();

    setLabel();
}


//----------------------------------------------------------------------
inline void Mixture::setMixture(const std::map<int, double>& aWeightSet,
                                double aMolarMass)
//----------------------------------------------------------------------
{
    setMixture(aWeightSet);
    m_molar_mass = aMolarMass;
}


//------------------------------------------------------------------------------
inline void Mixture::setMixture(const std::map<std::string, double>& aWeightSet,
                                double aMolarMass)
//------------------------------------------------------------------------------
{
    setMixture(aWeightSet);
    m_molar_mass = aMolarMass;
}


//-----------------------------------------
inline double Mixture::getMolarMass() const
//-----------------------------------------
{
	return (m_molar_mass);
}


//----------------------------------------------
inline void Mixture::setDensity(double aDensity)
//----------------------------------------------
{
    m_density = aDensity;
}


//---------------------------------------
inline double Mixture::getDensity() const
//---------------------------------------
{
    return m_density;
}


//---------------------------------------------------------------------
inline double Mixture::getLinearAttenuationTotal(double anEnergy) const
//---------------------------------------------------------------------
{
    return (getMassAttenuationTotal(anEnergy) *
            (getDensity()));
}


//-------------------------------------------------
inline double Mixture::getMu(double anEnergy) const
//-------------------------------------------------
{
    return (getLinearAttenuationTotal(anEnergy));
}


//---------------------------------
inline double Mixture::getZ() const
//---------------------------------
{
    return getEffectiveAtomicNumber();
}


//-------------------------------------------------
inline const std::string& Mixture::getLabel() const
//-------------------------------------------------
{
    return (m_label);
}


//---------------------------------------------------------
inline Mixture& Mixture::operator=(const Mixture& aMixture)
//---------------------------------------------------------
{
    m_molar_mass   = aMixture.m_molar_mass;
    m_density      = aMixture.m_density;
    m_p_mass_weight_set = aMixture.m_p_mass_weight_set;
    m_p_electron_weight_set = aMixture.m_p_electron_weight_set;
    m_label        = aMixture.m_label;

	return (*this);
}


//------------------------------------------------------------
inline bool Mixture::operator!=(const Mixture& aMixture) const
//------------------------------------------------------------
{
    return (!(operator==(aMixture)));
}


//-------------------------------------------------------------------
inline const std::map<int, double>& Mixture::getMassWeightSet() const
//-------------------------------------------------------------------
{
    return m_p_mass_weight_set;
}


    //-----------------------------------------------------------------------
    inline const std::map<int, double>& Mixture::getElectronWeightSet() const
    //-----------------------------------------------------------------------
    {
    return m_p_electron_weight_set;
    }


} // namespace gVirtualXRay/
