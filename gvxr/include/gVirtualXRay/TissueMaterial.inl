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


/**
********************************************************************************
*
*   @file       TissueMaterial.inl
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
//	Include
//******************************************************************************
#ifndef GVXR_CONSTANT_VALUES_H
#include "gVirtualXRay/ConstantValues.h"
#endif

#ifndef GVXR_NOT_IMPLEMENTED_EXCEPTION_H
#include "gVirtualXRay/NotImplementedException.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//------------------------------------------------------
inline TissueMaterial::TissueMaterial(short aMinHUValue,
                                      short aMaxHUValue,
                                      double H,
                                      double C,
                                      double N,
                                      double O,
                                      double Na,
                                      double Mg,
                                      double P,
                                      double S,
                                      double Cl,
                                      double Ar,
                                      double K,
                                      double Ca,
                                      double Ti,
                                      double Cu,
                                      double Zn,
                                      double Ag,
                                      double Sn,
                                      double Fe,
                                      double I):
//------------------------------------------------------
        m_min_HU_value(aMinHUValue),
        m_max_HU_value(aMaxHUValue)
//------------------------------------------------------
{
    if (m_max_HU_value < m_min_HU_value)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "The min HU value is greater than the max HU value. This is not possible.");
    }

    std::map<int, double> weight_set;
    weight_set[Z_H]  =  H;
    weight_set[Z_C]  =  C;
    weight_set[Z_N]  =  N;
    weight_set[Z_O]  =  O;
    weight_set[Z_Na]  = Na;
    weight_set[Z_Mg]  = Mg;
    weight_set[Z_P]  =  P;
    weight_set[Z_S]  =  S;
    weight_set[Z_Cl]  = Cl;
    weight_set[Z_Ar]  = Ar;
    weight_set[Z_K] =  K;
    weight_set[Z_Ca] = Ca;
    weight_set[Z_Ti] = Ti;
    weight_set[Z_Cu] = Cu;
    weight_set[Z_Zn] = Zn;
    weight_set[Z_Ag] = Ag;
    weight_set[Z_Sn] = Sn;
    weight_set[Z_Fe] = Fe;
    weight_set[Z_I] = I;
    
    m_mixture.setMixture(weight_set);
}


//---------------------------------------------------------------------
inline TissueMaterial::TissueMaterial(const TissueMaterial& aMaterial):
//---------------------------------------------------------------------
        m_min_HU_value(aMaterial.m_min_HU_value),
        m_max_HU_value(aMaterial.m_max_HU_value),
        m_mixture(aMaterial.m_mixture)
//---------------------------------------------------------------------
{
}


//---------------------------------
inline void TissueMaterial::clear()
//---------------------------------
{
    m_min_HU_value = 0;
    m_max_HU_value = 0;

	m_mixture.clear();
}


//--------------------------------------------------------------------------
inline const std::map<int, double>& TissueMaterial::getMassWeightSet() const
//--------------------------------------------------------------------------
{
	return m_mixture.getMassWeightSet();
}


//------------------------------------------------------------------------------
inline const std::map<int, double>& TissueMaterial::getElectronWeightSet() const
//------------------------------------------------------------------------------
{
	return m_mixture.getElectronWeightSet();
}


//------------------------------------------------
inline short TissueMaterial::getMinHUValue() const
//------------------------------------------------
{
	return (m_min_HU_value);
}
	
	
//------------------------------------------------
inline short TissueMaterial::getMaxHUValue() const
//------------------------------------------------
{
	return (m_max_HU_value);
}
	

//-----------------------------------------------
inline double TissueMaterial::getMolarMass() const
//-----------------------------------------------
{
	return (m_mixture.getMolarMass());
}


//-----------------------------------------------------
inline void TissueMaterial::setDensity(double aDensity)
//-----------------------------------------------------
{
    m_mixture.setDensity(aDensity);
}


//--------------------------------------------------------------------------
inline double TissueMaterial::getMassAttenuationTotal(double anEnergy) const
//--------------------------------------------------------------------------
{
    return (m_mixture.getMassAttenuationTotal(anEnergy));
}


//-----------------------------------------------------------------------------
inline double TissueMaterial::getLinearAttenuationTotal(short aHounsfieldValue,
                                                        double anEnergy) const
//-----------------------------------------------------------------------------
{
    return (m_mixture.getMassAttenuationTotal(anEnergy) *
            (getDensity(aHounsfieldValue)));
}


//---------------------------------------------------------
inline double TissueMaterial::getMu(short aHounsfieldValue,
                                    double anEnergy) const
//---------------------------------------------------------
{
    return (getLinearAttenuationTotal(aHounsfieldValue, anEnergy));
}



//----------------------------------------------------------------------------
inline double TissueMaterial::getLinearAttenuationTotal(double anEnergy) const
//----------------------------------------------------------------------------
{
    double density(m_mixture.getDensity());

    if (density < 0.0)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "The density of the mixture has not been initialised. It is therefore impossible to compute the linear attenuation coefficient.");
    }

    return (m_mixture.getMassAttenuationTotal(anEnergy) * density);
}


//---------------------------------------------------------
inline double TissueMaterial::getMu(double anEnergy) const
//---------------------------------------------------------
{
    return (getLinearAttenuationTotal(anEnergy));
}


//------------------------------------------------
inline double TissueMaterial::getS(double x) const
//------------------------------------------------
{
    throw NotImplementedException(__FILE__, __FUNCTION__, __LINE__);
}


//-------------------------------------------------------------------------------
inline TissueMaterial& TissueMaterial::operator=(const TissueMaterial& aMaterial)
//-------------------------------------------------------------------------------
{
    // Prevent self-assignment
    if (this != &aMaterial)
    {
        m_min_HU_value = aMaterial.m_min_HU_value;
        m_max_HU_value = aMaterial.m_max_HU_value;
        m_mixture      = aMaterial.m_mixture;
    }

	return (*this);
}


//---------------------------------------------------------------------------
inline bool TissueMaterial::operator==(const TissueMaterial& aMaterial) const
//---------------------------------------------------------------------------
{
    if (m_min_HU_value != aMaterial.m_min_HU_value) return false;
    if (m_max_HU_value != aMaterial.m_max_HU_value) return false;
    if (m_mixture      != aMaterial.m_mixture)      return false;

    return (true);
}


//---------------------------------------------------------------------------
inline bool TissueMaterial::operator!=(const TissueMaterial& aMaterial) const
//---------------------------------------------------------------------------
{
    return (!(operator==(aMaterial)));
}


//-----------------------------------------------------------------------
inline std::ostream & operator << (std::ostream& anOutputSream,
                                   const TissueMaterial& aTissueMaterial)
//-----------------------------------------------------------------------
{
	anOutputSream << aTissueMaterial.m_min_HU_value     << "\t";
	anOutputSream << aTissueMaterial.m_max_HU_value     << "\t";
	
	for (std::map<int, double>::const_iterator ite = aTissueMaterial.getMassWeightSet().begin();
			ite != aTissueMaterial.getMassWeightSet().end();
			++ite)
	{
		anOutputSream << ite->second << "\t";
	}
	
	return (anOutputSream);
}


//-----------------------------------------------------------------
inline std::istream & operator >> (std::istream& anInputSream,
                                   TissueMaterial& aTissueMaterial)
//-----------------------------------------------------------------
{
    std::map<int, double> weight_set;

	anInputSream >> aTissueMaterial.m_min_HU_value;
	anInputSream >> aTissueMaterial.m_max_HU_value;
	anInputSream >> weight_set[Z_H];
	anInputSream >> weight_set[Z_C];
	anInputSream >> weight_set[Z_N];
	anInputSream >> weight_set[Z_O];
	anInputSream >> weight_set[Z_Na];
	anInputSream >> weight_set[Z_Mg];
	anInputSream >> weight_set[Z_P];
	anInputSream >> weight_set[Z_S];
	anInputSream >> weight_set[Z_Cl];
	anInputSream >> weight_set[Z_Ar];
	anInputSream >> weight_set[Z_K];
	anInputSream >> weight_set[Z_Ca];
	anInputSream >> weight_set[Z_Ti];
	anInputSream >> weight_set[Z_Cu];
	anInputSream >> weight_set[Z_Zn];
	anInputSream >> weight_set[Z_Ag];
    anInputSream >> weight_set[Z_Sn];
    anInputSream >> weight_set[Z_Fe];
    anInputSream >> weight_set[Z_I];
	
	aTissueMaterial.setMixture(weight_set);
	
	return (anInputSream);
}


} // namespace gVirtualXRay/
