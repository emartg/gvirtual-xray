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


#ifndef GVXR_ATTENUATION_COEFFICIENT_H
#define GVXR_ATTENUATION_COEFFICIENT_H


/**
********************************************************************************
*
*   @file       AttenuationCoefficient.h
*
*   @brief      Class to manage an attenuation coefficient
*
*   @version    1.0
*
*   @date       30/11/2013
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
class AttenuationCoefficient;


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
std::istream& operator>>(std::istream& is, AttenuationCoefficient& obj);


//------------------------------------------------------------------------------
/// operator<<
/**
*   @param os: the output stream
*   @param obj: the record to store in the stream
*   @return the output stream
*/
//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const AttenuationCoefficient& obj);


//==============================================================================
/**
*   @class  AttenuationCoefficient
*   @brief  AttenuationCoefficient is a class to handle
*           an attenuation coefficient.
*/
//==============================================================================
class AttenuationCoefficient
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default constructor.
    /**
    *   @param anEnergy:                    the energy (default value: 0)
    *   @param anAttenuationCoefficient:    the attenuation coefficient
    *                                       (default value: 0)
    */
    //--------------------------------------------------------------------------
    AttenuationCoefficient(const RATIONAL_NUMBER& anEnergy = 0,
            const RATIONAL_NUMBER& anAttenuationCoefficient = 0);


    //--------------------------------------------------------------------------
    /// Accessor on the energy.
    /**
    *   @return the energy
    */
    //--------------------------------------------------------------------------
    RATIONAL_NUMBER getEnergy() const;


    //--------------------------------------------------------------------------
    /// Accessor on the attenuation coefficient.
    /**
    *   @return the attenuation coefficient
    */
    //--------------------------------------------------------------------------
    RATIONAL_NUMBER getAttenuationCoefficient() const;


//******************************************************************************
protected:
    /// Energy
    RATIONAL_NUMBER m_energy;


    /// Attenuation coefficient corresponding to m_energy
    RATIONAL_NUMBER m_mu;


    //**************************************************************************
    //  Friend operators
    //**************************************************************************
    friend std::istream& operator>>(std::istream &is,
            AttenuationCoefficient &obj);
    
    friend std::ostream& operator<<(std::ostream &os,
            const AttenuationCoefficient &obj);
};


} // namespace gVirtualXRay


//******************************************************************************
#include "AttenuationCoefficient.inl"


#endif // GVXR_ATTENUATION_COEFFICIENT_H
