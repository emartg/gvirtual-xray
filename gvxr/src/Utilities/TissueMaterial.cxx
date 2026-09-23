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
*   @file       TissueMaterial.cxx
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
*   @section    License
*               BSD 3-Clause License.
*
*               For details on use and redistribution please refer
*               to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Dr Franck P. Vidal, Bangor University, Dec 2013-Sept 2023, version 1.1, BSD 3-Clause License
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, Oct 2023-present, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#ifndef GVXR_TISSUE_MATERIAL_H
#include "gVirtualXRay/TissueMaterial.h"
#endif

#ifndef GVXR_UNITS_H
#include "gVirtualXRay/Units.h"
#endif

#ifndef GVXR_ATOMIC_ELEMENT_H
#include "gVirtualXRay/AtomicElement.h"
#endif

#ifndef GVXR_ELEMENT_SET_H
#include "gVirtualXRay/ElementSet.h"
#endif

#ifndef GVXR_CONSTANT_VALUES_H
#include "gVirtualXRay/ConstantValues.h"
#endif

#ifndef GVXR_EXCEPTION_H
#include "gVirtualXRay/Exception.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
using namespace gVirtualXRay;


//-------------------------------------------------------
double TissueMaterial::getDensity(short aHounsfieldValue)
//-------------------------------------------------------
{
    double density(0);

    if (aHounsfieldValue <= -1000)
    {
        density = 1.21e-3;
    }
    else if (-1000 <= aHounsfieldValue && aHounsfieldValue < -98)
    {
        // interpolate mass densities of air (1.21e-3 g/cm3) and adipose tissue 3 (0.93 g/cm3)
        density = 1.21e-3 + (aHounsfieldValue - -1000) * (0.93 - 1.21e-3) / (-98 - -1000);
    }
    else if (-98 <= aHounsfieldValue && aHounsfieldValue <= 14)
    {
        // Use of Eq. 21 in W Schneider et al 2000
        density = 1.018 + 0.893e-3 * aHounsfieldValue;
    }
    else if (14 < aHounsfieldValue && aHounsfieldValue < 23)
    {
        density = 1.03;
    }
    else if (23 <= aHounsfieldValue && aHounsfieldValue <= 100)
    {
        // Use of Eq. 23 in W Schneider et al 2000
        density = 1.003 + 1.169e-3 * aHounsfieldValue;
    }
    else if (100 <= aHounsfieldValue)
    {
        // Use of Eq. 19 in W Schneider et al 2000
        density = 1.017 + 0.592e-3 * aHounsfieldValue;
    }

    return (density * (g / cm3));
}


//----------------------------------------------------------------------
void TissueMaterial::setMixture(const std::map<int, double>& aWeightSet)
//----------------------------------------------------------------------
{
    if (aWeightSet.size() != 19) 
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Wrong number of atomic elements in the mixture.");

    if (aWeightSet.find(Z_H) == aWeightSet.end())
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "H is not in the mixture.");

    if (aWeightSet.find(Z_C) == aWeightSet.end())
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "C is not in the mixture.");

    if (aWeightSet.find(Z_N) == aWeightSet.end())
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "N is not in the mixture.");

    if (aWeightSet.find(Z_O) == aWeightSet.end())
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "O is not in the mixture.");

    if (aWeightSet.find(Z_Na) == aWeightSet.end())
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Na is not in the mixture.");

    if (aWeightSet.find(Z_Mg) == aWeightSet.end())
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Mg is not in the mixture.");

    if (aWeightSet.find(Z_P) == aWeightSet.end())
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "P is not in the mixture.");

    if (aWeightSet.find(Z_S) == aWeightSet.end())
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "S is not in the mixture.");

    if (aWeightSet.find(Z_Cl) == aWeightSet.end())
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cl is not in the mixture.");

    if (aWeightSet.find(Z_Ar) == aWeightSet.end())
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Ar is not in the mixture.");

    if (aWeightSet.find(Z_K) == aWeightSet.end())
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "K is not in the mixture.");

    if (aWeightSet.find(Z_Ca) == aWeightSet.end())
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Ca is not in the mixture.");

    if (aWeightSet.find(Z_Ti) == aWeightSet.end())
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Ti is not in the mixture.");

    if (aWeightSet.find(Z_Cu) == aWeightSet.end())
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cu is not in the mixture.");

    if (aWeightSet.find(Z_Zn) == aWeightSet.end())
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Zn is not in the mixture.");

    if (aWeightSet.find(Z_Ag) == aWeightSet.end())
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Ag is not in the mixture.");

    if (aWeightSet.find(Z_Sn) == aWeightSet.end())
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Sn is not in the mixture.");

    if (aWeightSet.find(Z_Fe) == aWeightSet.end())
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Fe is not in the mixture.");

    if (aWeightSet.find(Z_I) == aWeightSet.end())
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "I is not in the mixture.");

    m_mixture.setMixture(aWeightSet);
}
