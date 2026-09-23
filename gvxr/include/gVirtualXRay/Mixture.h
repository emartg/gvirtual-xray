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


#ifndef GVXR_MIXTURE_H
#define GVXR_MIXTURE_H


/**
********************************************************************************
*
*   @file       Mixture.h
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
*   http://www.fpvidal.net/, Sept 2018, 2018, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <map>
#include <string>


//******************************************************************************
//  define
//******************************************************************************


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//******************************************************************************
//  Class declaration
//******************************************************************************
class ElementSet;


//==============================================================================
/**
*   @class  Mixture
*   @brief  Mixture is a class to manage a mixture (e.g. Ti90Al6V4).
*/
//==============================================================================
class Mixture
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default Constructor
    /*
     *  @param aWeightSet: ???
     */
    //--------------------------------------------------------------------------
    Mixture();


    Mixture(const std::map<int, double>& aWeightSet);

    
    Mixture(const std::map<std::string, double>& aWeightSet);


    Mixture(const std::map<int, double>& aWeightSet, double aMolarMass);

    
    Mixture(const std::map<std::string, double>& aWeightSet, double aMolarMass);


    //--------------------------------------------------------------------------
    /// Copy constructor
    /*
    *  @param aMixture:    the mixture to copy
    */
    //--------------------------------------------------------------------------
    Mixture(const Mixture& aMixture);


    void clear();

    void setMixture(const std::map<int, double>& aWeightSet);
    void setMixture(const std::map<std::string, double>& aWeightSet);

    void setMixture(const std::map<int, double>& aWeightSet, double aMolarMass);
    void setMixture(const std::map<std::string, double>& aWeightSet, double aMolarMass);

    void addElement(const std::string& aName, double aPercentage);


    //--------------------------------------------------------------------------
    /// Normalise the weight set.
    //--------------------------------------------------------------------------
    void normaliseWeightSet();


    //--------------------------------------------------------------------------
    /// Update the value of the molar mass of the material.
    /*
    *   @return the molar mass of the material
    */
    //--------------------------------------------------------------------------
    double computeMolarMass();


    //--------------------------------------------------------------------------
    /// Accessor on the molar mass of the material.
    /*
    *   @return the molar mass of the element
    */
    //--------------------------------------------------------------------------
    double getMolarMass() const;


    void setDensity(double aDensity);


    //--------------------------------------------------------------------------
    /// Get the density of the material.
    /*
    *   @return the material density
    */
    //--------------------------------------------------------------------------
    double getDensity() const;


    //--------------------------------------------------------------------------
    /// Get the mass attenuation coefficient at a given energy
    /// (values are interpolated).
    /*
    *   @param aHounsfieldValue:    the material (HU) value
    *   @param anEnergy:    the energy
    *   @return the corresponding linear attenuation coefficient
    */
    //--------------------------------------------------------------------------
    double getMassAttenuationTotal(double anEnergy) const;


    //--------------------------------------------------------------------------
    /// Get the linear attenuation coefficient at a given energy (in mm-1)
    /// (values are interpolated).
    /*
     *  @param anEnergy:             the energy
     *  @return the corresponding linear attenuation coefficient
     */
    //--------------------------------------------------------------------------
    double getLinearAttenuationTotal(double anEnergy) const;


    //--------------------------------------------------------------------------
    /// Get the linear attenuation coefficient at a given energy (in mm-1)
    /// (values are interpolated).
    /*
    *  @param anEnergy:             the energy
    *  @return the corresponding linear attenuation coefficient
    */
    //--------------------------------------------------------------------------
    double getMu(double anEnergy) const;


    //--------------------------------------------------------------------------
    /// Get the effective atomic number
    /*
    *  @return the effective atomic number of the mixture
    */
    //--------------------------------------------------------------------------
    double getEffectiveAtomicNumber() const;


    //--------------------------------------------------------------------------
    /// Get the effective atomic number
    /*
    *  @return the effective atomic number of the mixture
    */
    //--------------------------------------------------------------------------
    double getZ() const;


    //--------------------------------------------------------------------------
    /// Get the Compton scattering cross section sample
    /// (values are interpolated).
    /*
    *   @param x:   the sample
    *   @return the Compton scattering cross section value
    */
    //--------------------------------------------------------------------------
    double getS(double x) const;


    const std::string& getLabel() const;


    //--------------------------------------------------------------------------
    /// Copy operator.
    /*
    *  @param aMaterial:    the material to copy
    *  @return the newly created material
    */
    //--------------------------------------------------------------------------
    Mixture& operator=(const Mixture& aMixture);


    bool operator==(const Mixture& aMixture) const;


    bool operator!=(const Mixture& aMixture) const;


    const std::map<int, double>& getMassWeightSet() const;


    const std::map<int, double>& getElectronWeightSet() const;


//******************************************************************************
private:
    void setLabel();


    // The weight of each element in the material
    std::map<int, double> m_p_mass_weight_set; // First: Atomic number; Second: Weight (w_i) in percentage

    // The weight of each element in the material
    std::map<int, double> m_p_electron_weight_set; // First: Atomic number; Second: Weight (f_i) in percentage


    // The molar mass of the material
    double m_molar_mass;

    // The density (a negative value means that it is computed automatically from the HU number
    // A positive value means that is is set by the user
    double m_density;


    std::string m_label;
};


} // namespace gVirtualXRay


//******************************************************************************
#include "Mixture.inl"


#endif // GVXR_MIXTURE_H
