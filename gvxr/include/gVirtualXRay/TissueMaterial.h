/*

Copyright (c) 2013-2023, Dr Franck P. Vidal, Bangor University, All rights reserved.
Copyright (c) 2023-present, Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), 
UK Research and Innovation, All rights reserved.

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


#ifndef GVXR_TISSUE_MATERIAL_H
#define GVXR_TISSUE_MATERIAL_H


/**
********************************************************************************
*
*   @file       TissueMaterial.h
*
*   @brief      Class to manage a material database. It implements the method
*               as follows:
*               Schneider W, Bortfeld T, Schlegel W.:
*               ``Correlation between CT numbers and tissue parameters needed
*               for Monte Carlo simulations of clinical dose distributions'',
*               Phys Med Biol. 2000 Feb;45(2):459-78.
*
*   @version    1.0
*
*   @date       10/04/2024
*
*   @author     Prof Franck P. Vidal
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Dr Franck P. Vidal, Bangor University, Dec 2013-Sept 2023, version 1.1, BSD 3-Clause License
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, Oct 2023-present, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifndef GVXR_MIXTURE_H
#include "gVirtualXRay/Mixture.h"
#endif

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
*   @class  TissueMaterial
*   @brief  TissueMaterial is a class to manage a material.
*/
//==============================================================================
class TissueMaterial
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default Constructor
    /*
     *  @param aMinHUValue: the minimum Hounsfield unit of the material
     *  @param aMaxHUValue: the maximum Hounsfield unit of the material
     *  @param H:   the amount of H in the material
     *  @param C:   the amount of C in the material
     *  @param N:   the amount of N in the material
     *  @param O:   the amount of O in the material
     *  @param Na:  the amount of Na in the material
     *  @param Mg:  the amount of Mg in the material
     *  @param P:   the amount of P in the material
     *  @param S:   the amount of S in the material
     *  @param Cl:  the amount of Cl in the material
     *  @param Ar:  the amount of Ar in the material
     *  @param K:   the amount of K in the material
     *  @param Ca:  the amount of Ca in the material
     *  @param Ti:  the amount of Ti in the material
     *  @param Cu:  the amount of Cu in the material
     *  @param Zn:  the amount of Zn in the material
     *  @param Ag:  the amount of Ag in the material
     *  @param Sn:  the amount of Sn in the material
     *  @param Fe:  the amount of Fe in the material
     *  @param I:   the amount of I in the material
     */
    //--------------------------------------------------------------------------
    TissueMaterial(short aMinHUValue = 0,
                   short aMaxHUValue = 0,
                   double H = 0,
                   double C = 0,
                   double N = 0,
                   double O = 0,
                   double Na = 0,
                   double Mg = 0,
                   double P = 0,
                   double S = 0,
                   double Cl = 0,
                   double Ar = 0,
                   double K = 0,
                   double Ca = 0,
                   double Ti = 0,
                   double Cu = 0,
                   double Zn = 0,
                   double Ag = 0,
                   double Sn = 0,
                   double Fe = 0,
                   double I = 0);


    //--------------------------------------------------------------------------
    /// Copy constructor
    /*
    *  @param aMaterial:    the material to copy
    */
    //--------------------------------------------------------------------------
    TissueMaterial(const TissueMaterial& aMaterial);


    //--------------------------------------------------------------------------
    /// Remove all the atomic elements from the material
    //--------------------------------------------------------------------------
    void clear();
    

    //--------------------------------------------------------------------------
    /// Accessor on the mass weight (w_i) of each atomic element
    /*
    *  @return the mass weight (w_i) of each atomic element
    */
    //--------------------------------------------------------------------------
    const std::map<int, double>& getMassWeightSet() const;


    //--------------------------------------------------------------------------
    /// Accessor on the electron weight (f_i) of each atomic element
    /*
    *  @return the electron weight (f_i)  of each atomic element
    */
    //--------------------------------------------------------------------------
    const std::map<int, double>& getElectronWeightSet() const;


    //--------------------------------------------------------------------------
    /// Accessor on the minimum HU.
    /*
    *   @return the minimum HU
    */
    //--------------------------------------------------------------------------
    short getMinHUValue() const;


    //--------------------------------------------------------------------------
    /// Accessor on the maximum HU.
    /*
    *   @return the maximum HU
    */
    //--------------------------------------------------------------------------
    short getMaxHUValue() const;


    //--------------------------------------------------------------------------
    /// Accessor on the molar mass of the material.
    /*
    *   @return the molar mass of the element
    */
    //--------------------------------------------------------------------------
    double getMolarMass() const;


    //--------------------------------------------------------------------------
    /// Set the density for the mixture regardless of the HU value.
    /*
    *   @param aDensity:    the density of the material
    */
    //--------------------------------------------------------------------------
    void setDensity(double aDensity);

    
    //--------------------------------------------------------------------------
    /// Get the density for a given material (in HU).
    /*
    *   @param aHounsfieldValue:    the material (HU) value
    *   @return the material (HU) density
    */
    //--------------------------------------------------------------------------
    static double getDensity(short aHounsfieldValue);

    
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
    /// Get the linear attenuation coefficient for a HU value at a given energy (in mm-1)
    /// (values are interpolated).
    /*
     *  @param aHounsfieldValue:     the material (HU) value
     *  @param anEnergy:             the energy
     *  @return the corresponding linear attenuation coefficient
     */
    //--------------------------------------------------------------------------
    double getLinearAttenuationTotal(short aHounsfieldValue, double anEnergy) const;


    //--------------------------------------------------------------------------
    /// Get the linear attenuation coefficient for a HU value at a given energy (in mm-1)
    /// (values are interpolated).
    /*
    *  @param aHounsfieldValue:     the material (HU) value
    *  @param anEnergy:             the energy
    *  @return the corresponding linear attenuation coefficient
    */
    //--------------------------------------------------------------------------
    double getMu(short aHounsfieldValue, double anEnergy) const;


    //--------------------------------------------------------------------------
    /// Get the linear attenuation coefficient of the mixture (regardless of the
    ///  HU value) at a given energy (values are interpolated). Note that 
    /// the density must have been set.
    /*
     *   @param anEnergy:    the energy
     *   @return the corresponding linear attenuation coefficient
     */
    //--------------------------------------------------------------------------
    double getLinearAttenuationTotal(double anEnergy) const;


    //--------------------------------------------------------------------------
    /// Get the linear attenuation coefficient of the mixture (regardless of the
    ///  HU value) at a given energy (values are interpolated). Note that 
    /// the density must have been set.
    /*
    *  @param anEnergy:             the energy
    *  @return the corresponding linear attenuation coefficient
    */
    //--------------------------------------------------------------------------
    double getMu(double anEnergy) const;


    //--------------------------------------------------------------------------
    /// Get the Compton scattering cross section sample
    /// (values are interpolated).
    /*
    *   @param x:   the sample
    *   @return the Compton scattering cross section value
    */
    //--------------------------------------------------------------------------
    double getS(double x) const;


    //--------------------------------------------------------------------------
    /// Copy operator.
    /*
    *  @param aMaterial:    the material to copy
    *  @return the newly created material
    */
    //--------------------------------------------------------------------------
    TissueMaterial& operator=(const TissueMaterial& aMaterial);


    //--------------------------------------------------------------------------
    /// Operator ==
    /*
    *  @param aMaterial:    the material to check against
    *  @return true if the materials are the same, else return false
    */
    //--------------------------------------------------------------------------
    bool operator==(const TissueMaterial& aMaterial) const;


    //--------------------------------------------------------------------------
    /// Operator !=
    /*
    *  @param aMaterial:    the material to check against
    *  @return true if the materials are different, else return false
    */
    //--------------------------------------------------------------------------
    bool operator!=(const TissueMaterial& aMaterial) const;


    //--------------------------------------------------------------------------
    /// operator <<
    /*
     *  @param anOutputSream:   the output stream
     *  @param aTissueMaterial: the material to output
     *  @return the output stream
     */
    //--------------------------------------------------------------------------
    friend std::ostream & operator << (std::ostream& anOutputSream,
                                       const TissueMaterial& aTissueMaterial);

    
    //--------------------------------------------------------------------------
    /// operator <<
    /*
     *  @param anInputSream:    the input stream
     *  @param aTissueMaterial: the material to read
     *  @return the input stream
     */
    //--------------------------------------------------------------------------
    friend std::istream & operator >> (std::istream& anInputSream,
                                       TissueMaterial& aTissueMaterial);

//******************************************************************************
protected:
    void setMixture(const std::map<int, double>& aWeightSet);


//******************************************************************************
private:
    // The minimum HU value of the material
    short m_min_HU_value;
    
    
    // The maximum HU value of the material
    short m_max_HU_value;
    

    // The mixture
    Mixture m_mixture;
};


} // namespace gVirtualXRay


//******************************************************************************
#include "TissueMaterial.inl"


#endif // GVXR_TISSUE_MATERIAL_H
