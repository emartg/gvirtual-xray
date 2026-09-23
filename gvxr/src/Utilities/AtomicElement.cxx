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
*   @file       AtomicElement.cxx
*
*   @brief      Class to manage elements in material.
*
*   @version    1.0
*
*   @date       07/04/2022
*
*   @author     Dr Franck P. Vidal
*
*   @section    License
*               BSD 3-Clause License.
*
*               For details on use and redistribution please refer
*               to http://opensource.org/licenses/BSD-3-Clause
*
*   @section    Copyright
*               (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*               http://www.fpvidal.net/, Apr 2022, 2022, version 1.1,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
#ifndef GVXR_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <vector>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstdlib>

#ifndef GVXR_ATOMIC_ELEMENT_H
#include "gVirtualXRay/AtomicElement.h"
#endif

#ifndef GVXR_CONSTANT_VALUES_H
#include "gVirtualXRay/ConstantValues.h"
#endif

#ifndef GVXR_EXCEPTION_H
#include "gVirtualXRay/Exception.h"
#endif

#ifndef GVXR_UTILITIES_H
#include "gVirtualXRay/Utilities.h"
#endif

#ifndef GVXR_UNITS_H
#include "gVirtualXRay/Units.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
using namespace gVirtualXRay;


void AtomicElement::printSummary() const
{
	std::cout << "	gVirtualXRay: " << m_name << std::endl;
	std::cout << "	Constituents (Atomic Number : Fraction by Weight)" << std::endl;
	std::cout << std::endl;
	std::cout << "	    Z=" << m_atomic_number << " : 1.000000" << std::endl;
	std::cout << "" << std::endl;
	std::cout << "\tPHOTON ENERGY\tTOTAL ATTENUATION" << std::endl;
	std::cout << "\t(MeV)\t(cm2/g)" << std::endl;
	for (unsigned int i = 0; i < getEnergyChannelNumber(); ++i)
	{
		std::cout.precision(3);
		std::cout << std::scientific <<
				"\t" <<
				m_p_energy_set[i] / MeV << "\t" <<
				// m_p_coherent_scattering_set[i] / (cm2/g) << "\t" <<
				// m_p_incoherent_scattering_set[i] / (cm2/g) << "\t" <<
				// m_p_photo_electric_absorption_set[i] / (cm2/g) << "\t" <<
				// m_p_pair_production_in_nuclear_field_set[i] / (cm2/g) << "\t" <<
				// m_p_pair_production_in_electron_field_set[i] / (cm2/g) << "\t" <<
				m_p_mass_attenuation_coefficient_set[i] / (cm2/g) << "\t" <<
				// m_p_total_attenuation_without_coherent_scattering[i] / (cm2/g) << "\t" <<
				std::endl;
	}
}


//-----------------------------------------------------------------
std::ostream& gVirtualXRay::operator<<(std::ostream& anOutputSream,
                                       const AtomicElement& anElement)
//-----------------------------------------------------------------
{
    anOutputSream << "Element: "                 << anElement.getName() << "(" << anElement.getSymbol() << ")" << std::endl;
    anOutputSream << "\tatomic number (Z): "     << anElement.getAtomicNumber()           << std::endl;
    anOutputSream << "\tmolar mass (in g/mol): " << anElement.getMolarMass() / (g / mole) << std::endl;
    anOutputSream << "\tdensity (in g/cm3): "    << anElement.getDensity() / (g/cm3)      << std::endl;
    anOutputSream << "\tstate: "                 << anElement.getState()                  << std::endl;

    return (anOutputSream);
}


//----------------------------------------------------------------
std::istream& gVirtualXRay::operator>>(std::istream& anInputSream,
                                       AtomicElement& anElement)
//----------------------------------------------------------------
{
    std::string temp_string;
    unsigned short temp_short;
    double temp_float;

    anInputSream >> temp_string;
    anElement.setName(temp_string);

    anInputSream >> temp_string;
    anElement.setSymbol(temp_string);

    anInputSream >> temp_short;
    anElement.setAtomicNumber(temp_short);

    anInputSream >> temp_float;
    temp_float *= g / mole;
    anElement.setMolarMass(temp_float);

    anInputSream >> temp_float;
    temp_float *= g / cm3;
    anElement.setDensity(temp_float);

    anInputSream >> temp_string;
    anElement.setState(temp_string);

    return (anInputSream);
}
