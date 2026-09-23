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
*   @file       MaterialSet.inl
*
*   @brief      Class to manage a table of materials.
*
*   @version    1.0
*
*   @date       13/10/2014
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
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//--------------------------------
inline MaterialSet::MaterialSet():
//--------------------------------
	m_mu_min_energy(0),
	m_mu_max_energy(0),
	m_min_S(0),
	m_max_S(0),
	m_min_HU(0),
	m_max_HU(0)
//--------------------------------
{
    loadMaterialSetFromMemory();
}


//--------------------------------
inline MaterialSet::~MaterialSet()
//--------------------------------
{
	clearMemory();
}


//------------------------------------
inline void MaterialSet::clearMemory()
//------------------------------------
{
	m_material_set.clear();
}


//----------------------------------------------
inline float MaterialSet::getMuMinEnergy() const
//----------------------------------------------
{
	return (m_mu_min_energy);
}


//----------------------------------------------
inline float MaterialSet::getMuMaxEnergy() const
//----------------------------------------------
{
	return (m_mu_max_energy);
}


inline short MaterialSet::getMinHU() const
{
	return (m_min_HU);
}


inline short MaterialSet::getMaxHU() const
{
	return (m_max_HU);
}


//--------------------------------------------------------
inline unsigned int MaterialSet::getMaterialNumber() const
//--------------------------------------------------------
{
	return (m_material_set.size());
}


//-----------------------------------------------------------------
inline std::ostream & operator << (std::ostream& anOutputSream,
                                   const MaterialSet& aMaterialSet)
//-----------------------------------------------------------------
{
	for (std::map<std::pair<short, short>, TissueMaterial>::const_iterator ite(aMaterialSet.m_material_set.begin());
		ite != aMaterialSet.m_material_set.end();
		++ite)
	{
		anOutputSream << ite->second << std::endl;
	}
	
	return (anOutputSream);
}


} // namespace gVirtualXRay
