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


#ifndef GVXR_SPECTRUM_RECORD_H
#define GVXR_SPECTRUM_RECORD_H


/**
********************************************************************************
*
*   @file       SpectrumRecord.h
*
*   @brief      Class to manage a record of the X-Ray beam,
*               i.e. an energy bin (number of photons, energy of the photons in
*               the bin).
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
*   http://www.fpvidal.net/, Jan 2017, 2017, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifndef GVXR_TYPES_H
#include "gVirtualXRay/Types.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//******************************************************************************
//  Class declaration
//******************************************************************************
class SpectrumRecord;


//******************************************************************************
//  Function declaration
//******************************************************************************
//------------------------------------------------------------------------------
/// operator>>
/**
*   @param is: the input stream
*   @param obj: the record to load from the stream
*   @return the input stream
*/
//------------------------------------------------------------------------------
std::istream& operator>>(std::istream& is, SpectrumRecord& obj);


//------------------------------------------------------------------------------
/// operator<<
/**
*   @param os: the output stream
*   @param obj: the record to store in the stream
*   @return the output stream
*/
//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const SpectrumRecord& obj);


//==============================================================================
/**
*   @class  SpectrumRecord
*   @brief  SpectrumRecord is a class to handle a record of the X-Ray beam,
*           i.e. an energy bin (number of photons, energy of the photons in
*           the bin).
*/
//==============================================================================
class SpectrumRecord
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default constructor.
    /**
     *  @param aPhotonNumber: number of photons in the bin (default value: 0)
     *  @param anIncidentEnergy: energy of the photons (default value: 0)
     */
    //--------------------------------------------------------------------------
    SpectrumRecord(const RATIONAL_NUMBER& aPhotonNumber = 0,
                   const RATIONAL_NUMBER& anIncidentEnergy = 0);


    //--------------------------------------------------------------------------
    /// Accessor on the number of photons in the bin.
    /**
     *  @return the number of photons
     */
    //--------------------------------------------------------------------------
    RATIONAL_NUMBER getPhotonNumber() const;


    //--------------------------------------------------------------------------
    /// Accessor on the energy of photons.
    /**
     *  @return the energy of photons
     */
    //--------------------------------------------------------------------------
    RATIONAL_NUMBER getPhotonEnergy() const;


    //--------------------------------------------------------------------------
    /// operator<.
    /**
     *  @return true is this->m_energy is lower than aRecord.m_energy
     */
    //--------------------------------------------------------------------------
    bool operator<(const SpectrumRecord& aRecord) const;


//******************************************************************************
protected:
    /// The number of photons
    RATIONAL_NUMBER m_photon_number;


    /// The energy of the photons
    RATIONAL_NUMBER m_energy;


    //**************************************************************************
    //  Friend class
    //**************************************************************************
    friend class XRayBeam;


    //**************************************************************************
    //  Friend operators
    //**************************************************************************
    friend std::istream& operator>>(std::istream& is, SpectrumRecord& obj);
    friend std::ostream& operator<<(std::ostream& os, const SpectrumRecord& obj);
};


} // namespace gVirtualXRay


//******************************************************************************
#include "SpectrumRecord.inl"


#endif // GVXR_SPECTRUM_RECORD_H
