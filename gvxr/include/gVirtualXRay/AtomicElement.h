/*

Copyright (c) 2017, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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


#ifndef GVXR_ATOMIC_ELEMENT_H
#define GVXR_ATOMIC_ELEMENT_H


/**
********************************************************************************
*
*   @file       AtomicElement.h
*
*   @brief      Class to manage elements in material.
*
*   @version    1.0
*
*   @date       13/10/2014
*
*   @todo       Merge the energy set and mass attenuation set using
*               std::pair<double, double>
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
#include <vector>
#include <string>


//******************************************************************************
//  defines
//******************************************************************************


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//******************************************************************************
//  Class declaration
//******************************************************************************
class AtomicElement;


//******************************************************************************
//  Function declarations
//******************************************************************************

//------------------------------------------------------------------------------
/// operator <<
/*
*   @param anOutputSream:   the stream in which an element is going
*                           to be printed
*   @param anElement:       the element to send into the stream
*   @return the output stream
*/
//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& anOutputSream,
        const gVirtualXRay::AtomicElement& anElement);


//------------------------------------------------------------------------------
/// operator >>
/*
*   @param anInputSream:    the stream from which an element is going to be read
*   @param anElement:       the element to read from the stream
*   @return the input stream
*/
//------------------------------------------------------------------------------
std::istream& operator>>(std::istream& anInputSream,
        gVirtualXRay::AtomicElement& anElement);


//==============================================================================
/**
*   @class  AtomicElement
*   @brief  AtomicElement is a class to manage elements in material.
*/
//==============================================================================
class AtomicElement
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default Constructor
    /*
     *  @param aName:       the name of the element
     *  @param aSymbol:     the symbol of the element
     *  @param aZNumber:    the atomic number (Z) of the element
     *  @param aMolarMass:  the molar mass of the element (in g/mol))
     *  @param aDensity:    the density of the element (in g/cm3)
     *  @param aState:      the state of the element
     *  @param aVoxelSize:  the size of a voxel
     */
    //--------------------------------------------------------------------------
    AtomicElement(const std::string& aName = "",
            const std::string& aSymbol = "",
            unsigned short aZNumber = 0,
            double aMolarMass = 0.0,
            double aDensity = 0.0,
            const std::string& aState = "");


    //--------------------------------------------------------------------------
    /// Copy Constructor
    /*
    *   @param anElement:   the element to copy
    */
    //--------------------------------------------------------------------------
    AtomicElement(const AtomicElement& anElement);


    //--------------------------------------------------------------------------
    /// Destructor
    //--------------------------------------------------------------------------
    ~AtomicElement();


    //--------------------------------------------------------------------------
    /// Set the name of the element.
    /*
    *   @param aName:   the name of the element
    */
    //--------------------------------------------------------------------------
    void setName(const std::string& aName);


    //--------------------------------------------------------------------------
    /// Set the symbol of the element.
    /*
    *   @param aSymbol: the symbol of the element
    */
    //--------------------------------------------------------------------------
    void setSymbol(const std::string& aSymbol);


    //--------------------------------------------------------------------------
    /// Set the atomic number (Z) of the element.
    /*
    *   @param aZNumber:    the atomic number (Z) of the element
    */
    //--------------------------------------------------------------------------
    void setAtomicNumber(unsigned short aZNumber);


    //--------------------------------------------------------------------------
    /// Set the molar mass of the element.
    /*
    *   @param aMolarMass:  the molar mass of the element
    */
    //--------------------------------------------------------------------------
    void setMolarMass(double aMolarMass);


    //--------------------------------------------------------------------------
    /// Set the density of the element.
    /*
    *   @param aDensity:    the density of the element
    */
    //--------------------------------------------------------------------------
    void setDensity(double aDensity);


    //--------------------------------------------------------------------------
    /// Set the state of the element.
    /*
    *   @param aState:  the state of the element
    */
    //--------------------------------------------------------------------------
    void setState(const std::string& aState);


    //--------------------------------------------------------------------------
    /// Get the mass attenuation coefficient at a given energy
    /// (values are interpolated).
    /*
    *   @param anEnergy:    the energy
    *   @return the corresponding linear attenuation coefficient
    */
    //--------------------------------------------------------------------------
    double getMassAttenuationTotal(double anEnergy);


    //--------------------------------------------------------------------------
    /// Get the linear attenuation coefficient at a given energy (in mm-1)
    /// (values are interpolated).
    /*
    *   @param anEnergy:    the energy
    *   @return the corresponding linear attenuation coefficient
    */
    //--------------------------------------------------------------------------
    double getMU(double anEnergy);


    //--------------------------------------------------------------------------
    /// Accessor on the name of the element.
    /*
    *   @return the name of the element
    */
    //--------------------------------------------------------------------------
    [[nodiscard]] const std::string& getName() const;


    //--------------------------------------------------------------------------
    /// Accessor on the symbol of the element.
    /*
    *   @return the symbol of the element
    */
    //--------------------------------------------------------------------------
    [[nodiscard]] const std::string& getSymbol() const;


    //--------------------------------------------------------------------------
    /// Accessor on the atomic number (Z) of the element.
    /*
    *   @return the atomic number (Z) of the element
    */
    //--------------------------------------------------------------------------
    [[nodiscard]] unsigned short getAtomicNumber() const;


    //--------------------------------------------------------------------------
    /// Accessor on the atomic number (Z) of the element.
    /*
    *   @return the atomic number (Z) of the element
    */
    //--------------------------------------------------------------------------
    [[nodiscard]] unsigned short getZ() const;


    //--------------------------------------------------------------------------
    /// Accessor on the atomic number (Z) of the element.
    /*
    *   @return the atomic number (Z) of the element
    */
    //--------------------------------------------------------------------------
    [[nodiscard]] unsigned short Z() const;


    //--------------------------------------------------------------------------
    /// Accessor on the molar mass of the element.
    /*
    *   @return the molar mass of the element
    */
    //--------------------------------------------------------------------------
    [[nodiscard]] double getMolarMass() const;


    //--------------------------------------------------------------------------
    /// Accessor on the density of the element.
    /*
    *   @return the density of the element
    */
    //--------------------------------------------------------------------------
    [[nodiscard]] double getDensity() const;


    //--------------------------------------------------------------------------
    /// Accessor on the state of the element.
    /*
    *   @return the state of the element
    */
    //--------------------------------------------------------------------------
    [[nodiscard]] const std::string& getState() const;


    //--------------------------------------------------------------------------
    /// Accessor on the number of energy channels in the attenuation
    /// coefficient list.
    /*
    *   @return the number of energy channels
    */
    //--------------------------------------------------------------------------
    [[nodiscard]] unsigned int getEnergyChannelNumber() const;


    //--------------------------------------------------------------------------
    /// Accessor on the smallest energy step in the linear attenuation
    /// coefficient list.
    /*
    *   @return the smallest energy step
    */
    //--------------------------------------------------------------------------
    [[nodiscard]] double getSmallestEnergyStep() const;


    //--------------------------------------------------------------------------
    /// Copy operator
    /*
    *   @param anElement:   the element to copy
    */
    //--------------------------------------------------------------------------
    AtomicElement& operator=(const AtomicElement& anElement);


    [[nodiscard]] double getEnergyChannel(unsigned int i) const;
    [[nodiscard]] double getMassAttenuationCoefficientChannel(unsigned int i) const;
    void printSummary() const;


//*****************************************************************************
protected:

    std::string    m_name;                  ///< The name of the element
    std::string    m_symbol;                ///< The symbol of the element
    unsigned short m_atomic_number;         ///< The atomic number of the element
    double         m_molar_mass;            ///< The molar mass of the element
    double         m_density;               ///< The density of the element
    std::string    m_state;                 ///< The state of the element

    /// The energy set
    std::vector<double> m_p_energy_set;

    /// The mass attenuation coefficient set corresponding to the energy set
    std::vector<double> m_p_mass_attenuation_coefficient_set;

    // std::vector<double> m_p_coherent_scattering_set;
    // std::vector<double> m_p_incoherent_scattering_set;
    // std::vector<double> m_p_photo_electric_absorption_set;
    // std::vector<double> m_p_pair_production_in_nuclear_field_set;
    // std::vector<double> m_p_pair_production_in_electron_field_set;
    // std::vector<double> m_p_total_attenuation_without_coherent_scattering;
};


} // namespace gVirtualXRay


//******************************************************************************
#include "AtomicElement.inl"


#endif // GVXR_ATOMIC_ELEMENT_H
