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


#ifndef GVXR_MATERIAL_SET_H
#define GVXR_MATERIAL_SET_H


/**
********************************************************************************
*
*   @file       MaterialSet.h
*
*   @brief      Class to manage a table of materials.
*
*   @version    1.0
*
*   @date       13/01/2017
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
*   http://www.fpvidal.net/, Jan 2017, 2017, version 1.0, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <iostream>
#include <map>
#include <string>

#ifndef GVXR_TISSUE_MATERIAL_H
#include "TissueMaterial.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*   @class  MaterialSet
*   @brief  MaterialSet is a class to manage a table of materials.
*/
//==============================================================================
class MaterialSet
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    static MaterialSet& getInstance();


    //--------------------------------------------------------------------------
    /// Destructor.
    //--------------------------------------------------------------------------
    ~MaterialSet();


    //--------------------------------------------------------------------------
    /// Clear all the memory.
    //--------------------------------------------------------------------------
    void clearMemory();


    //--------------------------------------------------------------------------
    /// Load a file containing materials.
    /*
    *   @param aFileName:   the name of the file to load
    */
    //--------------------------------------------------------------------------
    void loadFromFile(const std::string& aFileName);


    //--------------------------------------------------------------------------
    /// Load the table of materials.
    //--------------------------------------------------------------------------
    void loadMaterialSetFromMemory();


    //---------------------------------------------------------------------------
    /// Add a material to the list.
    /*
    *   @param aMaterial    the material to add
    */
    //---------------------------------------------------------------------------
    void addMaterial(const TissueMaterial& aMaterial);


    //--------------------------------------------------------------------------
    /// Get the material corresponding to a given HU value.
    /*
    *   @param aHUValue:    the HU value of the material
    */
    //--------------------------------------------------------------------------
    const TissueMaterial& getMaterial(short aHUValue) const;


    //--------------------------------------------------------------------------
    /// Accessor on the lower energy cutoff for attenuation coefficient.
    /*
    *   @return the lower energy cutoff for attenuation coefficient
    */
    //--------------------------------------------------------------------------
    float getMuMinEnergy() const;


    //--------------------------------------------------------------------------
    /// Accessor on the higher energy cutoff for attenuation coefficient.
    /*
    *   @return the higher energy cutoff for attenuation coefficient
    */
    //--------------------------------------------------------------------------
    float getMuMaxEnergy() const;


    //--------------------------------------------------------------------------
    /// Accessor on the minimum Hounsfield unit that the material set can handle.
    /*
    *   @return the minimum Hounsfield unit that the material set can handle
    */
    //--------------------------------------------------------------------------
    short getMinHU() const;


    //--------------------------------------------------------------------------
    /// Accessor on the maximum Hounsfield unit that the material set can handle.
    /*
    *   @return the maximum Hounsfield unit that the material set can handle
    */
    //--------------------------------------------------------------------------
    short getMaxHU() const;
    

    //--------------------------------------------------------------------------
    /// Accessor on the number of materials in the set.
    /*
    *   @return the number of materials
    */
    //--------------------------------------------------------------------------
    unsigned int getMaterialNumber() const;


    //--------------------------------------------------------------------------
    /// operator <<
    /*
     *  @param anOutputSream:   the output stream
     *  @param anElement:       the element to output
     *  @return the output stream
     */
    //--------------------------------------------------------------------------
    friend std::ostream & operator << (std::ostream& anOutputSream,
                                       const MaterialSet& anMaterialSet);


//*****************************************************************************
private:
    //---------------------------------------------------------------------------
    /// Default Constructor (prevent instanciation).
    //---------------------------------------------------------------------------
    MaterialSet();


    //--------------------------------------------------------------------------
    /// Copy constructor (prevent copy-construction).
    //--------------------------------------------------------------------------
    MaterialSet(const MaterialSet&);


    //--------------------------------------------------------------------------
    /// Constructor (prevent instanciation).
    /*
     *  @param aFileName:   the name of the file to load
     */
    //--------------------------------------------------------------------------
    MaterialSet(const std::string& aFileName);


    //--------------------------------------------------------------------------
    /// Copy operator (prevent assignment)
    //--------------------------------------------------------------------------
    MaterialSet& operator=(const MaterialSet&);


    //--------------------------------------------------------------------------
    /// An indexed table containing elements.
    //--------------------------------------------------------------------------
    std::map<std::pair<short, short>, TissueMaterial> m_material_set;

    
    //--------------------------------------------------------------------------
    /// The lower energy cutoff for attenuation coefficient.
    //--------------------------------------------------------------------------
    float m_mu_min_energy;


    //--------------------------------------------------------------------------
    /// The higher energy cutoff for attenuation coefficient.
    //--------------------------------------------------------------------------
    float m_mu_max_energy;


    //--------------------------------------------------------------------------
    /// The lower cutoff for Compton scattering cross section samples.
    //--------------------------------------------------------------------------
    float m_min_S;


    //--------------------------------------------------------------------------
    /// The higher cutoff for Compton scattering cross section samples.
    //--------------------------------------------------------------------------
    float m_max_S;


    //--------------------------------------------------------------------------
    /// The minimum Hounsfield unit that the material set can handle.
    //--------------------------------------------------------------------------
    short m_min_HU;


    //--------------------------------------------------------------------------
    /// The maximum Hounsfield unit that the material set can handle.
    //--------------------------------------------------------------------------
    short m_max_HU;
};


} // namespace gVirtualXRay


//******************************************************************************
#include "MaterialSet.inl"


#endif // GVXR_MATERIAL_SET_H
