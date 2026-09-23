/*

Copyright (c) 2022, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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


#ifndef GVXR_SCINTILLATOR_H
#define GVXR_SCINTILLATOR_H


/**
********************************************************************************
*
*   @file       Scintillator.h
*
*   @brief      Class to generate the energy response due to
*               the scintillator of the X-Ray detector.
*
*   @version    1.0
*
*   @date       18/10/2022
*
*   @author     Dr Franck P. Vidal
*
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*   http://www.fpvidal.net/, Oct 2022, 2022, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <string> // To store the material properties
#include <vector> // To store the energy response
#include <utility> // To create STL pairs

#ifndef GVXR_TYPES_H
#include "gVirtualXRay/Types.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*   @class  Scintillator
*   @brief  Scintillator is a class to generate the energy response due to
*           the scintillator of the X-Ray detector.
*/
//==============================================================================
class Scintillator
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default constructor.
    //--------------------------------------------------------------------------
    Scintillator();


    //--------------------------------------------------------------------------
    /// Destructor.
    //--------------------------------------------------------------------------
    ~Scintillator();


    //--------------------------------------------------------------------------
    /// Release the data.
    //--------------------------------------------------------------------------
    void release();


    void setMaterial(const std::string& aMaterial);


    //--------------------------------------------------------------------------
    /// Accessor on the list of scintillator materials currently supported.
    /**
    *  @return the list of supported scintillator materials
    */
    //--------------------------------------------------------------------------
    static std::vector<std::string> getSupportedScintillatorMaterials();


    void setThickness(const RATIONAL_NUMBER& aThickness);


    const std::string& getMaterial() const;


    RATIONAL_NUMBER getThickness();


    std::vector<std::pair<RATIONAL_NUMBER, RATIONAL_NUMBER> > getEnergyResponse() const;


    static std::vector<std::pair<RATIONAL_NUMBER, RATIONAL_NUMBER> > getEnergyResponse(const std::string& aMaterial,
        const RATIONAL_NUMBER& aThickness);


    static RATIONAL_NUMBER getDensity(const std::string& aMaterial);


    static std::string getCompound(const std::string& aMaterial);


    //--------------------------------------------------------------------------
    /// Load the energy response of the detector from a TSV file.
    /**
     *  @param aFileName: name of the file. Each line of the file is formatted as follows:
     *                    input_energy  output_energy
     *  @param aUnitOfEnergy: the unit of energy corresponding to the data in the file
     */
     //--------------------------------------------------------------------------
    void loadEnergyResponse(const std::string& aFileName,
        const RATIONAL_NUMBER& aUnitOfEnergy);


    void setEnergyResponse(const std::vector<std::pair<float, float> >& anEnergyResponse);


    void setEnergyResponse(const std::vector<std::pair<double, double> >& anEnergyResponse);


    //--------------------------------------------------------------------------
    /// Apply the energy response if any.
    /**
    *  @param anEnergy: the input energy
    *  @return the output energy
     */
     //--------------------------------------------------------------------------
    RATIONAL_NUMBER applyEnergyResponse(const RATIONAL_NUMBER& anEnergy) const;


//******************************************************************************
protected:
    //--------------------------------------------------------------------------
    /// Find the index of the closest input energy in the energy response.
    /**
    *  @return the index of the closest input energy
    */
    //--------------------------------------------------------------------------
    int findNearestEnergyIdx(const RATIONAL_NUMBER& anEnergy) const;


    std::string m_material;
    RATIONAL_NUMBER m_density;

    RATIONAL_NUMBER m_thickness;

    /// Energy response. The keys of the map are the input energies
    std::vector<std::pair<RATIONAL_NUMBER, RATIONAL_NUMBER> > m_p_energy_response;


    // PhotonCrossSection m_photon_cross_section;
};


} // namespace gVirtualXRay


//******************************************************************************
#include "Scintillator.inl"


#endif // GVXR_SCINTILLATOR_H
