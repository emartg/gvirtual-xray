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
*	@file		SpectrumRecord.inl
*
*	@brief		Class to manage a record of the X-Ray beam,
*				i.e. an energy bin (number of photons, energy of the photons in
*				the bin).
*
*	@version	1.0
*
*	@date		30/11/2013
*
*	@author		Dr Franck P. Vidal
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
//	Include
//******************************************************************************
#ifndef GVXR_UNITS_H
#include "gVirtualXRay/Units.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//-----------------------------------------------------------------------------
inline SpectrumRecord::SpectrumRecord(const RATIONAL_NUMBER& aPhotonNumber,
									  const RATIONAL_NUMBER& anIncidentEnergy):
//-----------------------------------------------------------------------------
		m_photon_number(aPhotonNumber),
		m_energy(anIncidentEnergy)
//-----------------------------------------------------------------------------
{}


//------------------------------------------------------------
inline RATIONAL_NUMBER SpectrumRecord::getPhotonNumber() const
//------------------------------------------------------------
{
	return (m_photon_number);
}


//------------------------------------------------------------
inline RATIONAL_NUMBER SpectrumRecord::getPhotonEnergy() const
//------------------------------------------------------------
{
	return (m_energy);
}


//---------------------------------------------------
inline std::istream& operator >>(std::istream &is,
								 SpectrumRecord &obj)
//---------------------------------------------------
{
	is >> obj.m_energy;
	is >> obj.m_photon_number;

	return (is);
}


//---------------------------------------------------------
inline std::ostream& operator <<(std::ostream &os,
								 const SpectrumRecord &obj)
//---------------------------------------------------------
{
	os << obj.getPhotonNumber() << " photons of " << obj.getPhotonEnergy() / keV << " keV";
	return (os);
}


//------------------------------------------------------------------------
inline bool SpectrumRecord::operator<(const SpectrumRecord& aRecord) const
//------------------------------------------------------------------------
{
    return (m_energy < aRecord.m_energy);
}


} // namespace gVirtualXRay
