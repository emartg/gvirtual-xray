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


#ifndef GVXR_UNITS_H
#define GVXR_UNITS_H


/**
********************************************************************************
*
*   @file       Units.h
*
*   @brief      Units, such as meters, etc.
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


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//******************************************************************************
//  Constant declarations
//******************************************************************************
const double kilometer  = 1000.0    / 0.001; ///< kilometre
const double hectometer =  100.0    / 0.001; ///< hectometre
const double decameter  =   10.0    / 0.001; ///< decametre
const double meter      =    1.0    / 0.001; ///< metre
const double decimeter  =    0.1    / 0.001; ///< decimetre
const double centimeter =    0.01   / 0.001; ///< centimetre
const double millimeter =    0.001  / 0.001; ///< millimetre
const double micrometer =    1.0e-6 / 0.001; ///< micrometre

const double kilometre  = kilometer;  ///< kilometre
const double hectometre = hectometer; ///< hectometre
const double decametre  = decameter;  ///< decametre
const double metre      = meter;      ///< metre
const double decimetre  = decimeter;  ///< decimetre
const double centimetre = centimeter; ///< centimetre
const double millimetre = millimeter; ///< millimetre
const double micrometre = micrometer; ///< micrometre

const double km  = kilometer;  ///< kilometre
const double hm  = hectometer; ///< hectometre
const double dam = decameter;  ///< decametre
const double m   = meter;      ///< meter
const double dm  = decimeter;  ///< decimeter
const double cm  = centimeter; ///< centimeter
const double mm  = millimeter; ///< millimeter
const double um  = micrometer; ///< micrometre

const double km2  = km * km;   ///< square kilometre
const double hm2  = hm * hm;   ///< square hectometre
const double dam2 = dam * dam; ///< square decametre
const double m2   = m * m;     ///< square meter
const double dm2  = dm * dm;   ///< square decimeter
const double cm2  = cm * cm;   ///< square centimeter
const double mm2  = mm * mm;   ///< square millimeter
const double um2  = um * um;   ///< square micrometre

const double km3  = km * km * km;    ///< cubic kilometre
const double hm3  = hm * hm * hm;    ///< cubic hectometre
const double dam3 = dam * dam * dam; ///< cubic decametre
const double m3   = m * m * m;       ///< cubic meter
const double dm3  = dm * dm * dm;    ///< cubic decimeter
const double cm3  = cm * cm * cm;    ///< cubic centimeter
const double mm3  = mm * mm * mm;    ///< cubic millimeter
const double um3  = um * um * um;    ///< cubic micrometre

const double megaelectronvolt = 1.0; ///< megaelectron volt
const double kiloelectronvolt = megaelectronvolt * 1.0e-3;///< kiloelectron volt
const double electronvolt     = megaelectronvolt * 1.0e-6;///< electronvolt

const double MeV              = megaelectronvolt;  ///< megaelectron volt
const double keV              = kiloelectronvolt;  ///< kiloelectron volt
const double eV               = electronvolt;      ///< electronvolt

const double kilogram  = 1000.0;    ///< kilogram
const double hectogram =  100.0;    ///< hectogram
const double decagram  =   10.0;    ///< decagram
const double gram      =    1.0;    ///< gram
const double decigram  =    0.1;    ///< decigram
const double centigram =    0.01;   ///< centigram
const double milligram =    0.001;  ///< milligram
const double microgram =    1e-6; ///< microgram

const double kg  = kilogram;  ///< kilogram
const double hg  = hectogram; ///< hectogram
const double dag = decagram;  ///< decagram
const double g   = gram;      ///< gram
const double dg  = decigram;  ///< decigram
const double cg  = centigram; ///< centigram
const double mg  = milligram; ///< milligram
const double ug  = microgram; ///< microgram

const double mole = 1.0; ///< mole

const double megavolt = 1.0; ///< mega volt
const double kilovolt = megavolt * 1.0e-3;///< kilo volt
const double volt     = megavolt * 1.0e-6;///< volt

const double MV              = megavolt;  ///< mega volt
const double kV              = kilovolt;  ///< kilo volt
const double V               = volt;      ///< volt


} // namespace gVirtualXRay


#endif // GVXR_UNITS_H
