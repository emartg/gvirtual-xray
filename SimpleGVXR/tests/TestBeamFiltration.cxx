/*

Copyright (c) 2025-present, Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), 
UK Research and Innovation, All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

3. Neither the name of the UK Research and Innovation nor the names of its contributors
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
*   @file       TestBeamFiltration.cxx
*
*   @brief      Unit testing: Test the beam filtration.
*
*   @date       17/07/2025
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
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, Jul 2025-present, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>

#include "gtest/gtest.h"

#ifndef SGVXR_Utils_h
#include "Utils.h"
#endif

#ifndef SGVXR_Source_h
#include "source.h"
#endif

#ifndef SGVXR_Detector_h
#include "detector.h"
#endif


//******************************************************************************
//  Global variables
//******************************************************************************
// Spectra generated using: https://spekpy.smile.ki.se/

// # SpekpyWeb (Spekpy ver. 2.0.8)
// # -----------------------
// # Physics: casim
// # Attenuation data: pene
// # Energy bin (keV): 0.5
// # Target material: W
// # Anode angle (°): 12
// # kV: 100
// # x (cm): 0
// # y (cm): 0
// # z (cm): 100
// # mAs: 1
// # Bremsstrahlung: true
// # Char. x-ray: true

// # Filtration: None

// # Mid-bin energy (keV), Fluence (cm⁻² keV⁻¹)
std::vector<double> g_p_spectrum_without_filtration = {
    1.25, 5617538.738383886,
    1.75, 4394876.488413364,
    2.25, 2603460.6019269447,
    2.75, 2648450.574365658,
    3.25, 3022351.064034248,
    3.75, 3687988.6939349775,
    4.25, 4532660.910851721,
    4.75, 5519325.090763533,
    5.25, 6619662.177884574,
    5.75, 7825557.452223433,
    6.25, 9119749.967877218,
    6.75, 10471605.666473925,
    7.25, 21503155.767127205,
    7.75, 13412383.52151717,
    8.25, 238913167.28825733,
    8.75, 18701893.684575733,
    9.25, 17216724.671588477,
    9.75, 227548062.79716873,
    10.25, 13701236.488340784,
    10.75, 8524248.178415198,
    11.25, 22763381.045767482,
    11.75, 9450691.995848445,
    12.25, 6683109.480913801,
    12.75, 6441625.021643212,
    13.25, 6837801.449290736,
    13.75, 7225274.983807843,
    14.25, 7605520.16257631,
    14.75, 7976081.991748688,
    15.25, 8333442.284604678,
    15.75, 8675908.48198391,
    16.25, 8998820.240041222,
    16.75, 9300678.636949113,
    17.25, 9580707.974086048,
    17.75, 9840515.273664942,
    18.25, 10079937.80709178,
    18.75, 10295810.14614845,
    19.25, 10488640.752822997,
    19.75, 10658332.884828534,
    20.25, 10807196.540725064,
    20.75, 10935240.87373582,
    21.25, 11042321.636711499,
    21.75, 11128817.728128683,
    22.25, 11197109.221638715,
    22.75, 11247570.619477324,
    23.25, 11281026.422270138,
    23.75, 11297882.248000247,
    24.25, 11299929.332622902,
    24.75, 11287554.541509043,
    25.25, 11263245.277782055,
    25.75, 11227000.791730272,
    26.25, 11179804.929273527,
    26.75, 11121821.178203396,
    27.25, 11054729.41364812,
    27.75, 10978679.908443907,
    28.25, 10895204.026057845,
    28.75, 10804362.070757093,
    29.25, 10707703.595776264,
    29.75, 10605055.075191226,
    30.25, 10498109.978190651,
    30.75, 10386544.51371257,
    31.25, 10271414.540606579,
    31.75, 10152561.827568073,
    32.25, 10031100.699306235,
    32.75, 9906965.021102019,
    33.25, 9781124.566381037,
    33.75, 9653393.733746106,
    34.25, 9524613.055006111,
    34.75, 9394649.523489514,
    35.25, 9264296.420803579,
    35.75, 9133233.965263134,
    36.25, 9002118.532152023,
    36.75, 8870674.292683652,
    37.25, 8739553.723474095,
    37.75, 8608615.734677888,
    38.25, 8478385.925849386,
    38.75, 8348666.701543788,
    39.25, 8219850.558903829,
    39.75, 8091834.312433231,
    40.25, 7964941.035254543,
    40.75, 7838951.519416666,
    41.25, 7714168.83915712,
    41.75, 7590435.661507128,
    42.25, 7468053.393708532,
    42.75, 7346837.18741925,
    43.25, 7227118.490528906,
    43.75, 7108609.32195212,
    44.25, 6991653.754699318,
    44.75, 6875952.130092264,
    45.25, 6761911.3116594525,
    45.75, 6649023.003960567,
    46.25, 6537887.613172468,
    46.75, 6427907.281770883,
    47.25, 6319734.921239655,
    47.75, 6212608.208747907,
    48.25, 6107282.778487821,
    48.75, 6003026.921640365,
    49.25, 5900726.213632898,
    49.75, 5799423.995456878,
    50.25, 5700033.271103867,
    50.75, 5601564.245328782,
    51.25, 5504954.27573912,
    51.75, 5409274.797976082,
    52.25, 5315489.236639857,
    52.75, 5222578.273761395,
    53.25, 5131508.759745356,
    53.75, 5041311.038629734,
    54.25, 4952909.686636893,
    54.75, 4865345.85491871,
    55.25, 4779546.476355103,
    55.75, 4694554.923494802,
    56.25, 4611238.044225994,
    56.75, 4528629.978741891,
    57.25, 4447632.604467299,
    57.75, 13914926.537115175,
    58.25, 4288637.076307915,
    58.75, 4210673.305720656,
    59.25, 21007468.910405666,
    59.75, 4058520.4840996093,
    60.25, 3984314.746600983,
    60.75, 3910913.247542123,
    61.25, 3838849.6052464107,
    61.75, 3767479.0166118043,
    62.25, 3697364.970411651,
    62.75, 3627954.520016934,
    63.25, 3559742.9006315027,
    63.75, 3492193.223352505,
    64.25, 3425829.5112777227,
    64.75, 3360118.1602016985,
    65.25, 3295524.0548383184,
    65.75, 3231532.6380528235,
    66.25, 3168550.0916701937,
    66.75, 5087218.612573652,
    67.25, 6884534.513868103,
    67.75, 3081646.060591804,
    68.25, 2924225.628510106,
    68.75, 2864973.5650404734,
    69.25, 4185713.243387177,
    69.75, 2680344.1388104036,
    70.25, 2419027.1699586553,
    70.75, 2373651.0414160597,
    71.25, 2328654.318729768,
    71.75, 2284026.3619603463,
    72.25, 2239750.7249931777,
    72.75, 2195740.140086757,
    73.25, 2152068.2276213537,
    73.75, 2108560.4460644936,
    74.25, 2065362.6507909205,
    74.75, 2022323.7970990215,
    75.25, 1979610.7736548518,
    75.75, 1937036.5688403642,
    76.25, 1894741.2126042936,
    76.75, 1852556.7638324087,
    77.25, 1810648.1646920692,
    77.75, 1768865.0882238839,
    78.25, 1727355.6609431698,
    78.75, 1685949.5154314823,
    79.25, 1644783.3374632916,
    79.75, 1603732.0852593242,
    80.25, 1562948.5137270805,
    80.75, 1522271.9833460015,
    81.25, 1481845.2398602045,
    81.75, 1441497.2539940097,
    82.25, 1401374.0294947324,
    82.75, 1361309.0010804865,
    83.25, 1321440.9129111841,
    83.75, 1281647.1554945237,
    84.25, 1242052.1806673075,
    84.75, 1202495.2660205706,
    85.25, 1163095.0881405524,
    85.75, 1123730.9614419313,
    86.25, 1084516.1401204402,
    86.75, 1045311.5161655487,
    87.25, 1006223.4026698027,
    87.75, 967138.7713523175,
    88.25, 928156.2781049311,
    88.75, 889162.4563935347,
    89.25, 850245.494303098,
    89.75, 811299.9853519868,
    90.25, 772406.2023071158,
    90.75, 733498.9280971013,
    91.25, 694639.6768610172,
    91.75, 655751.1243489089,
    92.25, 616889.0971148123,
    92.75, 578000.6358001827,
    93.25, 539145.5711484876,
    93.75, 500300.65092261706,
    94.25, 461526.0444862358,
    94.75, 422741.84975010087,
    95.25, 384015.05799275794,
    95.75, 345216.3344960941,
    96.25, 306463.16111184465,
    96.75, 267588.6900108323,
    97.25, 228875.73083958743,
    97.75, 189852.933838614,
    98.25, 151416.7789229415,
    98.75, 111850.05881190307,
    99.25, 72263.69481597422,
    99.75, 25707.12299285603
};

// SpekpyWeb (Spekpy ver. 2.0.8)
// -----------------------
// Physics: casim
// Attenuation data: pene
// Energy bin (keV): 0.5
// Target material: W
// Anode angle (°): 12
// kV: 100
// x (cm): 0
// y (cm): 0
// z (cm): 100
// mAs: 1
// Bremsstrahlung: true
// Char. x-ray: true

// Filtration: 5 mm Cu + 10 mm Al

// Mid-bin energy (keV), Fluence (cm⁻² keV⁻¹)
std::vector<double> g_p_spectrum_with_filtration = {
		// We remove all the bins with a total energy of less than 1 eV, i.e. 0.001 keV
//    1.25, 0,
//    1.75, 0,
//    2.25, 0,
//    2.75, 0,
//    3.25, 0,
//    3.75, 0,
//    4.25, 0,
//    4.75, 0,
//    5.25, 0,
//    5.75, 0,
//    6.25, 1.113156138194607e-307,
//    6.75, 5.6417669387768415e-247,
//    7.25, 9.507214844739397e-201,
//    7.75, 1.5838408785613312e-165,
//    8.25, 2.2446514032178035e-136,
//    8.75, 3.3460912617706587e-115,
//    9.25, 0,
//    9.75, 0,
//    10.25, 0,
//    10.75, 0,
//    11.25, 0,
//    11.75, 2.0128061357152316e-288,
//    12.25, 9.710324403267082e-258,
//    12.75, 4.0883153314499413e-231,
//    13.25, 6.169609731750465e-208,
//    13.75, 1.2610457211205942e-187,
//    14.25, 7.968815625220716e-170,
//    14.75, 3.743679429916761e-154,
//    15.25, 2.87632713653031e-140,
//    15.75, 6.428468434194536e-128,
//    16.25, 6.011536443227059e-117,
//    16.75, 3.6377368528451194e-107,
//    17.25, 2.0529864039014075e-98,
//    17.75, 1.5773848054483413e-90,
//    18.25, 2.103641658605791e-83,
//    18.75, 5.678110004533733e-77,
//    19.25, 3.9272001719184e-71,
//    19.75, 7.764638550828406e-66,
//    20.25, 5.1940167010681495e-61,
//    20.75, 1.3236299024325166e-56,
//    21.25, 1.3952275978489119e-52,
//    21.75, 6.6878264917341595e-49,
//    22.25, 1.613182534484814e-45,
//    22.75, 2.0970294876256558e-42,
//    23.25, 1.5359131563004516e-39,
//    23.75, 6.707428138958067e-37,
//    24.25, 1.8362546600960052e-34,
//    24.75, 3.2939122355107797e-32,
//    25.25, 4.076645238963315e-30,
//    25.75, 3.5963876919096035e-28,
//    26.25, 2.3039714185381823e-26,
//    26.75, 1.102441466787577e-24,
//    27.25, 4.0411232831944025e-23,
//    27.75, 1.1541884598793506e-21,
//    28.25, 2.6327798625213843e-20,
//    28.75, 4.895444838194086e-19,
//    29.25, 7.544001577753192e-18,
//    29.75, 9.778718631196074e-17,
//    30.25, 1.0809886001158845e-15,
//    30.75, 1.0315004479769259e-14,
//    31.25, 8.590000061198596e-14,
//    31.75, 6.306203981005227e-13,
//    32.25, 4.1194092587547125e-12,
//    32.75, 2.4142986510628937e-11,
//    33.25, 1.2794992126225622e-10,
//    33.75, 6.175086633317526e-10,
//    34.25, 2.7316885442293763e-9,
//    34.75, 1.1141968127238516e-8,
//    35.25, 4.196909122877793e-8,
//    35.75, 1.4678914949185858e-7,
//    36.25, 4.808725834402897e-7,
//    36.75, 0.0000014816448896289883,
//    37.25, 0.00000431070149550008,
//    37.75, 0.000011884678816193706,
    38.25, 0.00003115486842646843,
    38.75, 0.00007789048058939552,
    39.25, 0.00018625896972732136,
    39.75, 0.00042713426924958906,
    40.25, 0.0009380439517085586,
    40.75, 0.0019781173120366102,
    41.25, 0.004030127292468205,
    41.75, 0.007947974030722426,
    42.25, 0.015200830999215336,
    42.75, 0.028240581332036024,
    43.25, 0.05104808668672364,
    43.75, 0.0899099513838802,
    44.25, 0.1545177096194725,
    44.75, 0.2594378259263498,
    45.25, 0.4245575709341105,
    45.75, 0.6781101625853685,
    46.25, 1.0623139844444132,
    46.75, 1.6337190029600375,
    47.25, 2.4689618417055694,
    47.75, 3.6693172376649787,
    48.25, 5.367767924625198,
    48.75, 7.734267252521471,
    49.25, 10.985944048255906,
    49.75, 15.391216803484008,
    50.25, 21.220543193267833,
    50.75, 28.812944040366524,
    51.25, 38.67508966050521,
    51.75, 51.33824721288206,
    52.25, 67.43843807913497,
    52.75, 87.69014123741249,
    53.25, 112.93860451896091,
    53.75, 144.10631191136554,
    54.25, 182.2736789483581,
    54.75, 228.58386309662586,
    55.25, 283.694918736024,
    55.75, 348.56402177421626,
    56.25, 425.21984946015215,
    56.75, 515.0886780637364,
    57.25, 619.8621150485682,
    57.75, 2361.2644130967738,
    58.25, 880.7164926123173,
    58.75, 1040.3537185618252,
    59.25, 6209.75314349318,
    59.75, 1427.5531339365118,
    60.25, 1654.4625970507077,
    60.75, 1902.769381508106,
    61.25, 2178.377556570569,
    61.75, 2482.557200630149,
    62.25, 2817.2085667262127,
    62.75, 3183.429158976381,
    63.25, 3583.032113272673,
    63.75, 4016.8158746768645,
    64.25, 4486.545681665525,
    64.75, 4992.6417771579145,
    65.25, 5533.046200504691,
    65.75, 6110.1863227516715,
    66.25, 6725.75404992721,
    66.75, 12085.66044104462,
    67.25, 18251.308005739626,
    67.75, 9090.637952815398,
    68.25, 9572.351133639248,
    68.75, 10379.297463996929,
    69.25, 16739.319619153,
    69.75, 11803.143649846355,
    70.25, 11672.824614114978,
    70.75, 12492.992350903707,
    71.25, 13340.417264249785,
    71.75, 14213.742075266438,
    72.25, 15111.402342694622,
    72.75, 16031.136990094315,
    73.25, 16971.608441833552,
    73.75, 17929.39867067471,
    74.25, 18903.436766621115,
    74.75, 19889.97014809287,
    75.25, 20888.044036806492,
    75.75, 21893.057311147164,
    76.25, 22903.668954471854,
    76.75, 23914.885486789608,
    77.25, 24925.62152485919,
    77.75, 25930.6328877706,
    78.25, 26928.68706396381,
    78.75, 27913.745276482277,
    79.25, 28884.41234374112,
    79.75, 29834.90063288304,
    80.25, 30709.748974789567,
    80.75, 31500.403663515113,
    81.25, 32260.298766170716,
    81.75, 32982.39911153412,
    82.25, 33666.50447767804,
    82.75, 34305.19851781504,
    83.25, 34898.33157395285,
    83.75, 35439.199441205805,
    84.25, 35927.635871631304,
    84.75, 36355.502376231074,
    85.25, 36722.64965485391,
    85.75, 37021.697418394804,
    86.25, 37252.655319683785,
    86.75, 37407.16690126835,
    87.25, 37485.24283999905,
    87.75, 37479.05095149082,
    88.25, 37388.638876721154,
    88.75, 37205.82223714221,
    89.25, 36930.503765578025,
    89.75, 36554.46946636114,
    90.25, 36033.17812433077,
    90.75, 35364.141260871984,
    91.25, 34592.87720671647,
    91.75, 33712.56127832194,
    92.25, 32722.98264015018,
    92.75, 31618.39724065553,
    93.25, 30399.121722030806,
    93.75, 29061.106042748663,
    94.25, 27605.19076173194,
    94.75, 26024.073882712582,
    95.25, 24319.25945533483,
    95.75, 22480.006243628097,
    96.25, 20511.22464684175,
    96.75, 18399.188999519974,
    97.25, 16160.75801040543,
    97.75, 13760.294925280896,
    98.25, 11260.407647531,
    98.75, 8531.216912989714,
    99.25, 5650.907679723584,
    99.75, 2060.1886099319945
};


//******************************************************************************
//  Function declarations
//******************************************************************************
double getTotalEnergy(const std::vector<double>& apSpectrum);
double getNumberOfPhotons(const std::vector<double>& apSpectrum);
double nmae(const std::vector<double>& apReferenceSet,
            const std::vector<double>& apTestSet);
void runTest(const std::vector<double>& apReferenceSet);


//******************************************************************************
//  Function definitions
//******************************************************************************

//-------------------------------------------------------------------------
inline double getTotalEnergyTestData(const std::vector<double>& apSpectrum)
//-------------------------------------------------------------------------
{
    double total_energy = 0.0;

    for (auto ite = apSpectrum.begin(); ite != apSpectrum.end(); ite += 2)
    {
        double energy = *ite;
        double photon_count = *(ite + 1);
        total_energy += energy * photon_count;
    }

    return total_energy;
}


//-----------------------------------------------------------------------------
inline double getNumberOfPhotonsTestData(const std::vector<double>& apSpectrum)
//-----------------------------------------------------------------------------
{
    double total_photon_count = 0.0;

    for (auto ite = apSpectrum.begin(); ite != apSpectrum.end(); ite += 2)
    {
        total_photon_count += *(ite + 1);
    }

    return total_photon_count;
}


//----------------------------------
inline void loadUnfilteredSpectrum()
//----------------------------------
{
    for (auto ite = g_p_spectrum_without_filtration.begin();
        ite != g_p_spectrum_without_filtration.end();
        ite += 2)
    {
        addEnergyBinToSpectrumPerCm2At1m(*ite, "keV", *(ite+1));
    }
}


//--------------------------------------------
TEST(TestBeamFiltration, TestBeamNoFiltration)
//--------------------------------------------
{
	// Create a dunny simulation environment
	createWindow(-1, 0, "OPENMP");

    usePointSource();

    setSourcePosition(0, 0, 50, "cm");

    setDetectorPosition(0, 0, -50, "cm");
    setDetectorUpVector(0, 1, 0);
    setDetectorNumberOfPixels(10, 10);
    setDetectorPixelSize(1, 1, "cm");

    setBinTotalEnergyCutoff(1, "eV");

    // Reset the spectrum and filtration if any
    resetBeamSpectrum();

    // Load the unfiltered spectrum
    loadUnfilteredSpectrum();

    // Perform the comparison
    runTest(g_p_spectrum_without_filtration);
}


//-------------------------------------------
TEST(TestBeamFiltration, TestBeamFiltration2)
//-------------------------------------------
{
    // Reset the spectrum and filtration if any
    resetBeamSpectrum();

    // Load the unfiltered spectrum
    loadUnfilteredSpectrum();

    // Add a couple of filters
    addFilter("Cu", 5, "mm");
    addFilter("Al", 10, "mm");

    // Perform the comparison
    runTest(g_p_spectrum_with_filtration);
}


//--------------------------------------------------------
TEST(TestBeamFiltration, TestBeamFiltration2InvertedCalls)
//--------------------------------------------------------
{
    // Reset the spectrum and filtration if any
    resetBeamSpectrum();

    // Add a couple of filters
    addFilter("Al", 10, "mm");
    addFilter("Cu", 5, "mm");

    // Load the unfiltered spectrum
    loadUnfilteredSpectrum();

    // Perform the comparison
    runTest(g_p_spectrum_with_filtration);
}


//-------------------------------------------
TEST(TestBeamFiltration, TestBeamFiltration3)
//-------------------------------------------
{
    // Reset the spectrum and filtration if any
    resetBeamSpectrum();

    // Load the unfiltered spectrum
    loadUnfilteredSpectrum();

    // Add a couple of filters
    addFilter("Cu", 0.5, "cm");
    addFilter("Al", 1.0, "cm");

    // Perform the comparison
    runTest(g_p_spectrum_with_filtration);
}


//--------------------------------------------------------
TEST(TestBeamFiltration, TestBeamFiltration3InvertedCalls)
//--------------------------------------------------------
{
    // Reset the spectrum and filtration if any
    resetBeamSpectrum();

    // Add a couple of filters
    addFilter("Al", 1.0, "cm");
    addFilter("Cu", 0.5, "cm");

    // Load the unfiltered spectrum
    loadUnfilteredSpectrum();

    // Perform the comparison
    runTest(g_p_spectrum_with_filtration);
}


//-------------------------------------------
TEST(TestBeamFiltration, TestBeamFiltration4)
//-------------------------------------------
{
    // Reset the spectrum and filtration if any
    resetBeamSpectrum();

    // Load the unfiltered spectrum
    loadUnfilteredSpectrum();

    // Add a couple of filters
    addFilter(29, 5, "mm");
    addFilter(13, 10, "mm");

    // Perform the comparison
    runTest(g_p_spectrum_with_filtration);
}


//--------------------------------------------------------
TEST(TestBeamFiltration, TestBeamFiltration4InvertedCalls)
//--------------------------------------------------------
{
    // Reset the spectrum and filtration if any
    resetBeamSpectrum();

    // Add a couple of filters
    addFilter(13, 10, "mm");
    addFilter(29, 5, "mm");

    // Load the unfiltered spectrum
    loadUnfilteredSpectrum();

    // Perform the comparison
    runTest(g_p_spectrum_with_filtration);
}


//-------------------------------------------
TEST(TestBeamFiltration, TestBeamFiltration5)
//-------------------------------------------
{
    // Reset the spectrum and filtration if any
    resetBeamSpectrum();

    // Load the unfiltered spectrum
    loadUnfilteredSpectrum();

    // Add a couple of filters
    addFilter(29, 0.5, "cm");
    addFilter(13, 1.0, "cm");

    // Perform the comparison
    runTest(g_p_spectrum_with_filtration);
}


//--------------------------------------------------------
TEST(TestBeamFiltration, TestBeamFiltration5InvertedCalls)
//--------------------------------------------------------
{
    // Reset the spectrum and filtration if any
    resetBeamSpectrum();

    // Add a couple of filters
    addFilter(13, 1.0, "cm");
    addFilter(29, 0.5, "cm");

    // Load the unfiltered spectrum
    loadUnfilteredSpectrum();

    // Perform the comparison
    runTest(g_p_spectrum_with_filtration);
}


//-------------------------------------------
TEST(TestBeamFiltration, TestBeamFiltration6)
//-------------------------------------------
{
    // Reset the spectrum and filtration if any
    resetBeamSpectrum();

    // Load the unfiltered spectrum
    loadUnfilteredSpectrum();

    // Add a couple of filters
    std::vector<std::pair<unsigned int, float> > p_filtration;
    p_filtration.emplace_back(29, 5.0f);
    p_filtration.emplace_back(13, 10.0f);
    setFiltration(p_filtration, "mm");

    // Perform the comparison
    runTest(g_p_spectrum_with_filtration);
}


//--------------------------------------------------------
TEST(TestBeamFiltration, TestBeamFiltration6InvertedCalls)
//--------------------------------------------------------
{
    // Reset the spectrum and filtration if any
    resetBeamSpectrum();

    // Add a couple of filters
    std::vector<std::pair<unsigned int, float> > p_filtration;
    p_filtration.emplace_back(13, 10.0f);
    p_filtration.emplace_back(29, 5.0f);
    setFiltration(p_filtration, "mm");

    // Load the unfiltered spectrum
    loadUnfilteredSpectrum();

    // Perform the comparison
    runTest(g_p_spectrum_with_filtration);
}


//-------------------------------------------
TEST(TestBeamFiltration, TestBeamFiltration7)
//-------------------------------------------
{
    // Reset the spectrum and filtration if any
    resetBeamSpectrum();

    // Load the unfiltered spectrum
    loadUnfilteredSpectrum();

    // Add a couple of filters
    std::vector<std::pair<unsigned int, float> > p_filtration;
    p_filtration.emplace_back(29, 0.5f);
    p_filtration.emplace_back(13, 1.0f);
    setFiltration(p_filtration, "cm");

    // Perform the comparison
    runTest(g_p_spectrum_with_filtration);
}


//--------------------------------------------------------
TEST(TestBeamFiltration, TestBeamFiltration7InvertedCalls)
//--------------------------------------------------------
{
    // Reset the spectrum and filtration if any
    resetBeamSpectrum();

    // Add a couple of filters
    std::vector<std::pair<unsigned int, float> > p_filtration;
    p_filtration.emplace_back(13, 1.0f);
    p_filtration.emplace_back(29, 0.5f);
    setFiltration(p_filtration, "cm");

    // Load the unfiltered spectrum
    loadUnfilteredSpectrum();

    // Perform the comparison
    runTest(g_p_spectrum_with_filtration);
}


//-------------------------------------------
TEST(TestBeamFiltration, TestBeamFiltration8)
//-------------------------------------------
{
    // Reset the spectrum and filtration if any
    resetBeamSpectrum();

    // Load the unfiltered spectrum
    loadUnfilteredSpectrum();

    // Add a couple of filters
    addFilter(29,  5, "cm");
    addFilter(13, 10, "cm");

    // Replace them
    std::vector<std::pair<unsigned int, float> > p_filtration;
    p_filtration.emplace_back(29, 0.5f);
    p_filtration.emplace_back(13, 1.0f);
    setFiltration(p_filtration, "cm");

    // Perform the comparison
    runTest(g_p_spectrum_with_filtration);
}


//--------------------------------------------------------
TEST(TestBeamFiltration, TestBeamFiltration8InvertedCalls)
//--------------------------------------------------------
{
    // Reset the spectrum and filtration if any
    resetBeamSpectrum();

    // Add a couple of filters
    addFilter(29,  5, "cm");
    addFilter(13, 10, "cm");

    // Replace them
    std::vector<std::pair<unsigned int, float> > p_filtration;
    p_filtration.emplace_back(13, 1.0f);
    p_filtration.emplace_back(29, 0.5f);
    setFiltration(p_filtration, "cm");

    // Load the unfiltered spectrum
    loadUnfilteredSpectrum();

    // Perform the comparison
    runTest(g_p_spectrum_with_filtration);
}


//-------------------------------
TEST(TestBeamFiltration, TestMAS)
//-------------------------------
{
    // Reset the spectrum and filtration if any
    resetBeamSpectrum();

    setVoltage(100, "kV");
    setmAs(1.0);
    setEnergyBinSize(0.5, "keV");

    std::vector<double> p_energy_set = getEnergyBins("keV");
    std::vector<double> p_photon_count_set = getPhotonCountsPerCm2At1m(true);
    double sum_1mAs = accumulate(p_photon_count_set.begin(), p_photon_count_set.end(), 0.0);

    setmAs(2.0);
    p_photon_count_set = getPhotonCountsPerCm2At1m(true);
    double sum_2mAs = accumulate(p_photon_count_set.begin(), p_photon_count_set.end(), 0.0);

    ASSERT_DOUBLE_EQ(2.0 * sum_1mAs, sum_2mAs);
}


//----------------------------------------------------
double nmae(const std::vector<double>& apReferenceSet,
            const std::vector<double>& apTestSet)
//----------------------------------------------------
{
    double ref_max = *std::max_element(apReferenceSet.begin(), apReferenceSet.end());
    double test_max = *std::max_element(apTestSet.begin(), apTestSet.end());
    double max_value = std::max(ref_max, test_max);

    double ref_min = *std::min_element(apReferenceSet.begin(), apReferenceSet.end());
    double test_min = *std::min_element(apTestSet.begin(), apTestSet.end());
    double min_value = std::min(ref_min, test_min);

    size_t number_of_bins = apReferenceSet.size();

    double MAE = 0.0;

    for (size_t i = 0; i < number_of_bins; ++i)
    {
        MAE += abs(apReferenceSet[i] - apTestSet[i]);
    }

    MAE /= number_of_bins;

    return MAE / (max_value - min_value);
}

void runTest(const std::vector<double>& apReferenceSet)
//-----------------------------------------------------
{
    std::vector<double> detector_position = getDetectorPosition("cm");

    size_t number_of_bins = apReferenceSet.size() / 2;
    double reference_total_energy = getTotalEnergyTestData(apReferenceSet);
    double reference_total_photon_count = getNumberOfPhotonsTestData(apReferenceSet);

    std::vector<double> p_energy_set = getEnergyBins("keV");
    std::vector<double> p_photon_count_set = getPhotonCountsPerCm2At1m(true);

    // Make sure the spectrum has the same size
    ASSERT_EQ(number_of_bins, p_energy_set.size());
    ASSERT_EQ(number_of_bins, p_photon_count_set.size());

    // The value is within the range [0, 100]
    double test_total_energy = getTotalEnergyWithDetectorResponse(true) / getUnitOfEnergy("keV");
    double error_in_percent = 100.0 * abs(reference_total_energy - test_total_energy) / reference_total_energy;
    EXPECT_LE(error_in_percent, 4.0);

    // The value is within the range [0, 100]
    error_in_percent = 100.0 * abs(reference_total_photon_count - getNumberOfPhotonsPerCm2At1m(true)) / reference_total_photon_count;
    EXPECT_LE(error_in_percent, 4.0);

    std::vector<double> p_reference_energy_set;
    std::vector<double> p_reference_photon_count_set;

    for (auto ite = apReferenceSet.begin(); ite != apReferenceSet.end(); ite += 2)
    {
        p_reference_energy_set.push_back(*ite);
        p_reference_photon_count_set.push_back(*(ite+1));
    }

    // The value is within the range [0, 100]
    error_in_percent = 100.0 * nmae(p_reference_energy_set, p_energy_set);
    EXPECT_LE(error_in_percent, 0.05);

    // The value is within the range [0, 100]
    error_in_percent = 100.0 * nmae(p_reference_photon_count_set, p_photon_count_set);
    EXPECT_LE(error_in_percent, 4.0);


    double number_of_photons_per_second_per_pixel_at_sdd = getNumberOfPhotonsPerPixelAtSDD(true);
    std::vector<double> p_photons_per_pixel_set = getPhotonCountsPerPixelAtSDD(true);
    double sum_photons_per_pixel = std::accumulate(p_photons_per_pixel_set.begin(), p_photons_per_pixel_set.end(), 0.0);
    error_in_percent = 100.0 * abs(sum_photons_per_pixel - number_of_photons_per_second_per_pixel_at_sdd) / sum_photons_per_pixel;
    EXPECT_LE(error_in_percent, 0.1);

    double number_of_photons_per_second_per_cm2_at_1m = getNumberOfPhotonsPerCm2At1m(true);
    std::vector<double> p_photons_per_cm2_set = getPhotonCountsPerCm2At1m(true);
    double sum_photons_per_cm2 = std::accumulate(p_photons_per_cm2_set.begin(), p_photons_per_cm2_set.end(), 0.0);
    error_in_percent = 100.0 * abs(sum_photons_per_cm2 - number_of_photons_per_second_per_cm2_at_1m) / sum_photons_per_cm2;
    EXPECT_LE(error_in_percent, 0.1);

    setNumberOfPhotonsPerPixelAtSDD(10000, true);
    number_of_photons_per_second_per_pixel_at_sdd = getNumberOfPhotonsPerPixelAtSDD(true);
    p_photons_per_pixel_set = getPhotonCountsPerPixelAtSDD(true);
    sum_photons_per_pixel = std::accumulate(p_photons_per_pixel_set.begin(), p_photons_per_pixel_set.end(), 0.0);
    error_in_percent = 100.0 * abs(sum_photons_per_pixel - number_of_photons_per_second_per_pixel_at_sdd) / sum_photons_per_pixel;
    EXPECT_LE(error_in_percent, 0.1);

    number_of_photons_per_second_per_cm2_at_1m = getNumberOfPhotonsPerCm2At1m(true);
    p_photons_per_cm2_set = getPhotonCountsPerCm2At1m(true);
    sum_photons_per_cm2 = std::accumulate(p_photons_per_cm2_set.begin(), p_photons_per_cm2_set.end(), 0.0);
    error_in_percent = 100.0 * abs(sum_photons_per_cm2 - number_of_photons_per_second_per_cm2_at_1m) / sum_photons_per_cm2;
    EXPECT_LE(error_in_percent, 0.1);

    setDetectorPosition(detector_position[0], detector_position[1], -150, "cm");

    number_of_photons_per_second_per_pixel_at_sdd = getNumberOfPhotonsPerPixelAtSDD(true);
    p_photons_per_pixel_set = getPhotonCountsPerPixelAtSDD(true);
    sum_photons_per_pixel = std::accumulate(p_photons_per_pixel_set.begin(), p_photons_per_pixel_set.end(), 0.0);
    error_in_percent = 100.0 * abs(sum_photons_per_pixel - number_of_photons_per_second_per_pixel_at_sdd) / sum_photons_per_pixel;
    EXPECT_LE(error_in_percent, 0.1);

    number_of_photons_per_second_per_cm2_at_1m = getNumberOfPhotonsPerCm2At1m(true);
    p_photons_per_cm2_set = getPhotonCountsPerCm2At1m(true);
    sum_photons_per_cm2 = std::accumulate(p_photons_per_cm2_set.begin(), p_photons_per_cm2_set.end(), 0.0);
    error_in_percent = 100.0 * abs(sum_photons_per_cm2 - number_of_photons_per_second_per_cm2_at_1m) / sum_photons_per_cm2;
    EXPECT_LE(error_in_percent, 0.1);

    setDetectorPosition(detector_position[0], detector_position[1], detector_position[2], "cm");
}


//-----------------------------------------------------
