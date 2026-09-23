/*

Copyright (c) 2024-present, Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), 
UK Research and Innovation, All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

3. Neither the name of UK Research and Innovation nor the names of its contributors
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


#ifndef SGVXR_Source_h
#define SGVXR_Source_h


/**
********************************************************************************
*
*   @file       source.h
*
*   @brief      Expose here all the functions of SimpleGVXR related to 
*               the X-ray source. It includes the beam spectrum.
*
*   @version    1.0
*
*   @date       07/05/2024
*
*   @author     Prof Franck P. Vidal
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, May 2024-present, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <vector>


//------------------------------------------------------------------------------
/// Set the position of the X-ray source.
/**
 *  @param x:   the position along the X-axis
 *  @param y:   the position along the Y-axis
 *  @param z:   the position along the Z-axis
 *  @param aUnitOfLength:   the unit of length corresponding to the x, y and z
 *                          parameters. Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 */
//------------------------------------------------------------------------------
void setSourcePosition(double x,
    double y,
    double z,
    const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Accessor on the shape of the X-ray source.
/*
 *  @return the shape of the X-ray source (e.g. POINT, PARALLEL, SQUARE, CUBE, LINE,  UNKNOWN)
 */
//------------------------------------------------------------------------------
std::string getSourceShape();


//------------------------------------------------------------------------------
/// Accessor on the position of the X-ray source.
/**
 *  @param aUnitOfLength:   the unit of length corresponding to
 *                          the returned value. Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 *  @return the source position in a given unit of length
 */
//------------------------------------------------------------------------------
std::vector<double> getSourcePosition(const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Use a point source, i.e. a cone-beam geometry.
//------------------------------------------------------------------------------
void usePointSource();


//------------------------------------------------------------------------------
/// Use a cone-beam geometry, i.e. a point source.
//------------------------------------------------------------------------------
void useConeBeam();


//------------------------------------------------------------------------------
/// Use a parallel source (e.g. to mimic a source that is extremely far from
/// the scanned object and detector).
//------------------------------------------------------------------------------
void useParallelBeam();


//------------------------------------------------------------------------------
/// Use a parallel source (e.g. to mimic a source that is extremely far from
/// the scanned object and detector).
//------------------------------------------------------------------------------
void useParallelSource();



//------------------------------------------------------------------------------
/// Check if the beam shape is parallel (e.g. to mimic a source that is 
/// extremely far from the scanned object and detector).
/**
 *  @return true if the beam shape is parallel, false otherwise
 */
//------------------------------------------------------------------------------
bool isUsingParallelBeam();


//------------------------------------------------------------------------------
/// Check if the beam shape is parallel (e.g. to mimic a source that is 
/// extremely far from the scanned object and detector).
/**
 *  @return true if the beam shape is parallel, false otherwise
 */
//------------------------------------------------------------------------------
bool isUsingParallelSource();


//------------------------------------------------------------------------------
/// Check if the beam shape is cone-beam and the source an infinitively 
/// small point.
/**
 *  @return true if an infinitively small point source is used, false otherwise
 */
//------------------------------------------------------------------------------
bool isUsingPointSource();


//------------------------------------------------------------------------------
/// Check if the beam shape is cone-beam. Note that an infinitively small point
/// source or a finite size focal spot may have been used.
/**
 *  @return true if a cone-beam shape is used, false otherwise
 */
//------------------------------------------------------------------------------
bool isUsingConeBeam();


//------------------------------------------------------------------------------
/// Check if the beam shape is cone-beam and the source a finite size 
/// focal spot.
/**
 *  @return true if a finite size focal spot is used, false otherwise
 */
//------------------------------------------------------------------------------
bool isUsingFocalSpot();


//------------------------------------------------------------------------------
/// Set the size of the focal spot of the X-ray generator. A cube shape is used.
/// The volume of the cube is sampled by i^3 point sources.
/**
 *  @param x:   the position along the X-axis
 *  @param y:   the position along the Y-axis
 *  @param z:   the position along the Z-axis
 *  @param l:   the lenght of the cube along the 3 axes
 *  @param aUnitOfLength:   the unit of length. Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 *  @param i:   the number of point souces used along each axes.
 */
//------------------------------------------------------------------------------
void setFocalSpot(float x, float y, float z, float l, const std::string& aUnitOfLength,
  int i);


//------------------------------------------------------------------------------
/// Set the size of the focal spot of the X-ray generator.
/// A square shape is used. The area of the square is sampled by
/// i*i point sources.
/**
 *  @param x:   the position along the X-axis
 *  @param y:   the position along the Y-axis
 *  @param z:   the position along the Z-axis
 *  @param l:   the length of the square
 *  @param aUnitOfLength:   the unit of length. Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 *  @param i:   the number of point souces used along each axes.
 */
//------------------------------------------------------------------------------
void setFocalSpotAsSquare(float x, float y, float z,
    float l,
    const std::string& aUnitOfLength,
    int i);


//------------------------------------------------------------------------------
/// Set the size of the focal spot of the X-ray generator.
/// A rectangle shape is used. The area of the rectangle is sampled by
/// i*j point sources.
/**
 *  @param x:   the position along the X-axis
 *  @param y:   the position along the Y-axis
 *  @param z:   the position along the Z-axis
 *  @param l:   the length of the rectangle along the axis defined by
 *              the cross product between the source-detector centres and
 *              the detector up-vector
 *  @param k:   the height of the source along the detector up-vector direction
 *  @param aUnitOfLength:   the unit of length. Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 *  @param i:   the number of point sources along the axis defined by the cross
 *              product between the source-detector centres and
 *              the detector up-vector.
 *  @param j:   the number of point sources along the detector up-vector direction.
 */
//------------------------------------------------------------------------------
void setFocalSpotAsRectangle(float x, float y, float z,
    float l, float k,
    const std::string& aUnitOfLength,
    int i, int j);


//--------------------------------------------------------------------------
/// Rotate the focal spot around its centre.
/**
*  @param aRotationAngle: the rotation angle in degrees
*  @param u: the rotaton axis along the X-axis
*  @param v: the rotaton axis along the Y-axis
*  @param w: the rotaton axis along the Z-axis
*/
//--------------------------------------------------------------------------
void rotateFocalSpot(const double& aRotationAngle, double u, double v, double w);


//--------------------------------------------------------------------------
/// Rotate the focal spot around an arbitrary point.
/**
*  @param aRotationAngle: the rotation angle in degrees
*  @param x: the rotaton centre along the X-axis
*  @param y: the rotaton centre along the Y-axis
*  @param z: the rotaton centre along the Z-axis
*  @param u: the rotaton axis along the X-axis
*  @param v: the rotaton axis along the Y-axis
*  @param w: the rotaton axis along the Z-axis
*/
//--------------------------------------------------------------------------
void rotateSource(const double& aRotationAngle,
    double x, double y, double z,
    double u, double v, double w);


//------------------------------------------------------------------------------
/// Empty the beam spectrum.
/**
*  @param aClearTubeParameterFlag: if the flag is true, then the X-ray tubes parameters are reset too
*/
//------------------------------------------------------------------------------
void resetBeamSpectrum(bool aClearTubeParameterFlag = true);


//------------------------------------------------------------------------------
/// Set a threshold to discard energy bins with a tiny amount of energy.
/**
 *  @param aThreshold:  the threshold
 *  @param aUnitOfEnergy:       the unit of energy corresponding to aThreshold.
 *                              Acceptable values are:
 *                              "electronvolt", "eV",
 *                              "kiloelectronvolt", "keV",
 *                              "megaelectronvolt", "MeV"
*/
//------------------------------------------------------------------------------
void setBinTotalEnergyCutoff(double aThreshold,
    const std::string& aUnitOfEnergy);


//------------------------------------------------------------------------------
/// Use a monochromatic beam spectrum (i.e. one single energy).
/**
 *  @param anEnergy:            the incident photon energy
 *  @param aUnitOfEnergy:       the unit of energy corresponding to anEnergy.
 *                              Acceptable values are:
 *                              "electronvolt", "eV",
 *                              "kiloelectronvolt", "keV",
 *                              "megaelectronvolt", "MeV"
 *  @param aNumberOfPhotons:    the number of incident photons
 */
//------------------------------------------------------------------------------
void setMonoChromatic(double anEnergy,
                      const std::string& aUnitOfEnergy,
                      double aNumberOfPhotons);


//------------------------------------------------------------------------------
/// Use a monochromatic beam spectrum (i.e. one single energy).
/// The number of photons is given per pixel at the source-detector distance.
/**
 *  @param anEnergy:            the incident photon energy
 *  @param aUnitOfEnergy:       the unit of energy corresponding to anEnergy.
 *                              Acceptable values are:
 *                              "electronvolt", "eV",
 *                              "kiloelectronvolt", "keV",
 *                              "megaelectronvolt", "MeV"
 *  @param aNumberOfPhotons:    the number of incident photons
 */
//------------------------------------------------------------------------------
void setMonoChromaticPerPixelAtSDD(double anEnergy,
                                   const std::string& aUnitOfEnergy,
                                   double aNumberOfPhotons);


//------------------------------------------------------------------------------
/// Use a monochromatic beam spectrum (i.e. one single energy).
/// The number of photons is given per cm2 at 1 metre.
/**
 *  @param anEnergy:            the incident photon energy
 *  @param aUnitOfEnergy:       the unit of energy corresponding to anEnergy.
 *                              Acceptable values are:
 *                              "electronvolt", "eV",
 *                              "kiloelectronvolt", "keV",
 *                              "megaelectronvolt", "MeV"
 *  @param aNumberOfPhotons:    the number of incident photons
 */
//------------------------------------------------------------------------------
void setMonoChromaticPerCm2At1m(double anEnergy,
                                const std::string& aUnitOfEnergy,
                                double aNumberOfPhotons);


//------------------------------------------------------------------------------
/// Add an energy bin to the beam spectrum (useful to generate polychromatism).
/**
 *  @param anEnergy:            the incident photon energy
 *  @param aUnitOfEnergy:       the unit of energy corresponding to anEnergy.
 *                              Acceptable values are:
 *                              "electronvolt", "eV",
 *                              "kiloelectronvolt", "keV",
 *                              "megaelectronvolt", "MeV"
 *  @param aNumberOfPhotons:    the number of incident photons
 */
//------------------------------------------------------------------------------
void addEnergyBinToSpectrum(double anEnergy,
                            const std::string& aUnitOfEnergy,
                            double aNumberOfPhotons);


//------------------------------------------------------------------------------
/// Add an energy bin to the beam spectrum (useful to generate polychromatism).
/// The number of photons is given per pixel at the source-detector distance.
/**
 *  @param anEnergy:            the incident photon energy
 *  @param aUnitOfEnergy:       the unit of energy corresponding to anEnergy.
 *                              Acceptable values are:
 *                              "electronvolt", "eV",
 *                              "kiloelectronvolt", "keV",
 *                              "megaelectronvolt", "MeV"
 *  @param aNumberOfPhotons:    the number of incident photons
 */
//------------------------------------------------------------------------------
void addEnergyBinToSpectrumPerPixelAtSDD(double anEnergy,
                                         const std::string& aUnitOfEnergy,
                                         double aNumberOfPhotons);


//------------------------------------------------------------------------------
/// Add an energy bin to the beam spectrum (useful to generate polychromatism).
/// The number of photons is given per cm2 at 1 metre.
/**
 *  @param anEnergy:            the incident photon energy
 *  @param aUnitOfEnergy:       the unit of energy corresponding to anEnergy.
 *                              Acceptable values are:
 *                              "electronvolt", "eV",
 *                              "kiloelectronvolt", "keV",
 *                              "megaelectronvolt", "MeV"
 *  @param aNumberOfPhotons:    the number of incident photons
 */
//------------------------------------------------------------------------------
void addEnergyBinToSpectrumPerCm2At1m(double anEnergy,
                                      const std::string& aUnitOfEnergy,
                                      double aNumberOfPhotons);


//------------------------------------------------------------------------------
/// Load the incident beam energy spectrum from a TSV file.
/**
 *  @param aFileName:           name of the file to load
 *  @param aUnitOfEnergy:       the unit of energy corresponding to anEnergy.
 *                              Acceptable values are:
 *                              "electronvolt", "eV",
 *                              "kiloelectronvolt", "keV",
 *                              "megaelectronvolt", "MeV"
 *  @param aNormalisationFlag:  true to normalise the total energy to 1,
 *                              false otherwise
 */
//------------------------------------------------------------------------------
void loadSpectrumFromTSV(const std::string& aFileName,
                         const std::string& aUnitOfEnergy,
                         bool aNormalisationFlag);


//------------------------------------------------------------------------------
/// Load the incident beam energy spectrum from a TSV file. 
/// The number of photons is given per pixel at the source-detector distance.
/**
 *  @param aFileName:           name of the file to load
 *  @param aUnitOfEnergy:       the unit of energy corresponding to anEnergy.
 *                              Acceptable values are:
 *                              "electronvolt", "eV",
 *                              "kiloelectronvolt", "keV",
 *                              "megaelectronvolt", "MeV"
 *  @param aNormalisationFlag:  true to normalise the total energy to 1,
 *                              false otherwise
 */
//------------------------------------------------------------------------------
void loadSpectrumFromTSVPerPixelAtSDD(const std::string& aFileName,
                                      const std::string& aUnitOfEnergy,
                                      bool aNormalisationFlag);


//------------------------------------------------------------------------------
/// Load the incident beam energy spectrum from a TSV file.
/// The number of photons is given per cm2 at 1 metre.
/**
 *  @param aFileName:           name of the file to load
 *  @param aUnitOfEnergy:       the unit of energy corresponding to anEnergy.
 *                              Acceptable values are:
 *                              "electronvolt", "eV",
 *                              "kiloelectronvolt", "keV",
 *                              "megaelectronvolt", "MeV"
 *  @param aNormalisationFlag:  true to normalise the total energy to 1,
 *                              false otherwise
 */
//------------------------------------------------------------------------------
void loadSpectrumFromTSVPerCm2At1m(const std::string& aFileName,
                                   const std::string& aUnitOfEnergy,
                                   bool aNormalisationFlag);


//------------------------------------------------------------------------------
/// Accessor on the energy bins of the beam spectrum.
/**
 *  @param aUnitOfEnergy:   the unit of energy corresponding to
 *                          the returned vector. Acceptable values are:
 *                          "electronvolt", "eV",
 *                          "kiloelectronvolt", "keV",
 *                          "megaelectronvolt", "MeV"
 *  @return the list of energy bins in aUnitOfEnergy
 */
//------------------------------------------------------------------------------
std::vector<double> getEnergyBins(const std::string& aUnitOfEnergy);


//------------------------------------------------------------------------------
/// Accessor on the number of photons per energy bin of the beam spectrum.
/**
 *  @param anApplyFiltrationFlag: true if the number of photons apply to the filtered spectrum;
 *                                false if the number of photons apply to the unfiltered spectrum
 *                                (default value: true)
 *  @return the list of number of photons
 */
//------------------------------------------------------------------------------
std::vector<double> getPhotonCountEnergyBins(bool anApplyFiltrationFlag = true);


//------------------------------------------------------------------------------
/// Accessor on the number of photons per cm2 at 1 m for per energy bin of the beam spectrum.
/**
 *  @param anApplyFiltrationFlag: true if the number of photons apply to the filtered spectrum;
 *                                false if the number of photons apply to the unfiltered spectrum
 *                                (default value: true)
 *  @return the list of number of photons
 */
//------------------------------------------------------------------------------
std::vector<double> getPhotonCountsPerCm2At1m(bool anApplyFiltrationFlag = true);


//------------------------------------------------------------------------------
/// Accessor on the number of photons per pixel at SDD for per energy bin of the beam spectrum.
/**
 *  @param anApplyFiltrationFlag: true if the number of photons apply to the filtered spectrum;
 *                                false if the number of photons apply to the unfiltered spectrum
 *                                (default value: true)
 *  @return the list of number of photons
 */
//------------------------------------------------------------------------------
std::vector<double> getPhotonCountsPerPixelAtSDD(bool anApplyFiltrationFlag = true);


//--------------------------------------------------------------------------
/// Accessor on a total energy of the beam when the detector response is applied.
/**
 *  @param anApplyFiltrationFlag: true if the number of photons apply to the filtered spectrum;
 *                                false if the number of photons apply to the unfiltered spectrum
 *                                (default value: true)
 *  @return the total energy
 */
//--------------------------------------------------------------------------
double getTotalEnergyWithDetectorResponse(bool anApplyFiltrationFlag = true);


//--------------------------------------------------------------------------
/// Accessor on a total energy of the beam when the detector response and the gain are applied.
/**
 *  @param anApplyFiltrationFlag: true if the number of photons apply to the filtered spectrum;
 *                                false if the number of photons apply to the unfiltered spectrum
 *                                (default value: true)
 *  @return the total energy
 */
//--------------------------------------------------------------------------
unsigned short getTotalEnergyWithDetectorResponseWithGain(bool anApplyFiltrationFlag = true);


//--------------------------------------------------------------------------
/// Accessor on a total energy of the beam when the detector response is not applied.
/**
 *  @param anApplyFiltrationFlag: true if the number of photons apply to the filtered spectrum;
 *                                false if the number of photons apply to the unfiltered spectrum
 *                                (default value: true)
 *  @return the total energy
 */
//--------------------------------------------------------------------------
double getTotalEnergyWithoutDetectorResponse(bool anApplyFiltrationFlag = true);


//--------------------------------------------------------------------------
/// Accessor on a total energy of the beam when the gain is applied but not the detector response.
/**
 *  @param anApplyFiltrationFlag: true if the number of photons apply to the filtered spectrum;
 *                                false if the number of photons apply to the unfiltered spectrum
 *                                (default value: true)
 *  @return the total energy
 */
//--------------------------------------------------------------------------
unsigned short getTotalEnergyWithoutDetectorResponseWithGain(bool anApplyFiltrationFlag = true);


//--------------------------------------------------------------------------
/// Set the voltage of the X-ray tube.
/**
 *  @param aVoltage:   the voltage
 *  @param aUnitOfElectriPotential:   the unit of electric potential to
 *                          the voltage of the tube. Acceptable values are:
 *                          "megavolt", "MV",
 *                          "kilovolt", "kV",
 *                          "volt", "V"
 */
//--------------------------------------------------------------------------
void setVoltage(const float& aVoltage,
    const std::string& aUnitOfElectriPotential);


//--------------------------------------------------------------------------
/// Accessor on the voltage of the X-ray tube.
/**
 *  @param aUnitOfElectriPotential:   the unit of electric potential to
 *                          the voltage of the tube. Acceptable values are:
 *                          "megavolt", "MV",
 *                          "kilovolt", "kV",
 *                          "volt", "V"
 *  @return the voltage of the X-ray tube
 */
//--------------------------------------------------------------------------
float getVoltage(const std::string& aUnitOfElectriPotential);





//--------------------------------------------------------------------------
/// If the beam is filtered with an inherent filatration, 
/// remove all the filtration.
//--------------------------------------------------------------------------
void clearInherentFiltration();


//--------------------------------------------------------------------------
/// Add an inherent filter.
/**
 *  @param aMaterial:  The symbol of the material of the filter
 *  @param aThickness:  The thickness of the filter
 *  @param aUnitOfLength:   the unit of length. Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 */
 //--------------------------------------------------------------------------
void addInherentFilter(const std::string& aMaterial, const double& aThickness, const std::string& aUnitOfLength);


//--------------------------------------------------------------------------
/// Add an inherent filter.
/**
 *  @param aMaterial:  The Z number of the material of the filter
 *  @param aThickness:  The thickness of the filter
 *  @param aUnitOfLength:   the unit of length. Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 */
 //--------------------------------------------------------------------------
void addInherentFilter(unsigned int aMaterial, const double& aThickness, const std::string& aUnitOfLength);


//--------------------------------------------------------------------------
/// Accessor on all the materials (Z numbers) of all the inherent filters used.
/**
*  @return the Z numbers corresponding to the inherent filters used. If none is used, an empty array is returned.
*/
//--------------------------------------------------------------------------
std::vector<int> getInherentFiltrationMaterial();


//--------------------------------------------------------------------------
/// Accessor on all the thickness of all the inherent filters used.
/**
*  @param aUnitOfLength:   the unit of length corresponding to the thicknesses.
*                          Acceptable values are:
*                          "um", "micrometre",  "micrometer",
*                          "mm", "millimetre",  "millimeter",
*                          "cm", "centimetre",  "centimeter",
*                          "dm",  "decimetre",  "decimeter",
*                          "m",   "metre",      "meter",
*                          "dam", "decametre",  "decameter",
*                          "hm",  "hectometre", "hectometer",
*                          "km",  "kilometre",  "kilometer"
*  @return the thickness of all the inherent filters used. If none is used, an empty array is returned.
*/
//--------------------------------------------------------------------------
std::vector<float> getInherentFiltrationThickness(const std::string& aUnitOfLength);


//--------------------------------------------------------------------------
/// Set the exposure in milliampere second (mA⋅s).
/**
 *  @param an_mAs:  the exposure
 */
//--------------------------------------------------------------------------
void setmAs(const float& an_mAs);


//--------------------------------------------------------------------------
/// Accessor on the exposure in milliampere second (mA⋅s).
/**
 *  @return the exposure
 */
//--------------------------------------------------------------------------
float getmAs();


//--------------------------------------------------------------------------
/// Set the angle of the X-ray tube (i.e. its anode's target) in degrees.
/**
 *  @param anAngle:  the angle
 */
//--------------------------------------------------------------------------
void setTubeAngle(const float& anAngle);


//--------------------------------------------------------------------------
/// Accessor on the angle of the X-ray tube (i.e. its anode's target) in degrees.
/**
 *  @return the angle
 */
//--------------------------------------------------------------------------
float getTubeAngle();


//--------------------------------------------------------------------------
/// Set the energy of the first energy bin in the unfiltered spectrum. 
/// This function only has an effect for X-ray tube spectra generated 
/// with `setVoltage`.
/**
 *  @param aFirstEnergyBin: the energy of the first energy bin in the unfiltered spectrum
 *  @param aUnitOfEnergy:   the unit of energy corresponding to aFirstEnergyBin.
 *                          Acceptable values are:
 *                          "electronvolt", "eV",
 *                          "kiloelectronvolt", "keV",
 *                          "megaelectronvolt", "MeV"
 */
//--------------------------------------------------------------------------
void setFirstEnergyBin(const float& aFirstEnergyBin,
    const std::string& aUnitOfEnergy);


//--------------------------------------------------------------------------
/// Set the size of an energy bin in the unfiltered spectrum. 
/// This function only has an effect for X-ray tube spectra generated 
/// with `setVoltage`.
/**
 *  @param anEnergyBinSize: the size of an energy bin in the unfiltered spectrum
 *  @param aUnitOfEnergy:   the unit of energy corresponding to aFirstEnergyBin.
 *                          Acceptable values are:
 *                          "electronvolt", "eV",
 *                          "kiloelectronvolt", "keV",
 *                          "megaelectronvolt", "MeV"
 */
//--------------------------------------------------------------------------
void setEnergyBinSize(const float& anEnergyBinSize, 
                      const std::string& aUnitOfEnergy);


//--------------------------------------------------------------------------
/// Accessor on the size of an energy bin in the unfiltered spectrum.
/// This function is only relevant for X-ray tube spectra generated
/// with `setVoltage`.
/**
 *  @param aUnitOfEnergy:   the unit of energy corresponding to aFirstEnergyBin.
 *                          Acceptable values are:
 *                          "electronvolt", "eV",
 *                          "kiloelectronvolt", "keV",
 *                          "megaelectronvolt", "MeV"
 */
//--------------------------------------------------------------------------
float getEnergyBinSize(const std::string& aUnitOfEnergy);


//--------------------------------------------------------------------------
/// If the beam is filtered, remove all the filtration.
//--------------------------------------------------------------------------
void clearFiltration();


//--------------------------------------------------------------------------
/// Set the beam filtration.
/**
 *  @param aSetOfFilters:  the set of filter. In the C++ notation, it is a vector of pairs of int and float. Each int value corresponds to the Z number of the material of the filter; and each float value corresponds to the thickness of the filter. Several filters can be used.
 *  @param aUnitOfLength:   the unit of length. Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 */
//--------------------------------------------------------------------------
void setFiltration(const std::vector<std::pair<unsigned int, float> >& aSetOfFilters,
    const std::string& aUnitOfLength);


//--------------------------------------------------------------------------
/// Add a filter.
/**
 *  @param aMaterial:  The symbol of the material of the filter
 *  @param aThickness:  The thickness of the filter
 *  @param aUnitOfLength:   the unit of length. Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 */
//--------------------------------------------------------------------------
void addFilter(const std::string& aMaterial, const double& aThickness, const std::string& aUnitOfLength);


//--------------------------------------------------------------------------
/// Add a filter.
/**
 *  @param aMaterial:  The Z number of the material of the filter
 *  @param aThickness:  The thickness of the filter
 *  @param aUnitOfLength:   the unit of length. Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 */
//--------------------------------------------------------------------------
void addFilter(unsigned int aMaterial, const double& aThickness, const std::string& aUnitOfLength);


//--------------------------------------------------------------------------
/// Accessor on all the materials (Z numbers) of all the beam filters used.
/**
*  @return the Z numbers corresponding to the beam filters used. If none is used, an empty array is returned.
*/
//--------------------------------------------------------------------------
std::vector<int> getFiltrationMaterial();


//--------------------------------------------------------------------------
/// Accessor on all the thickness of all the beam filters used.
/**
*  @param aUnitOfLength:   the unit of length corresponding to the thicknesses.
*                          Acceptable values are:
*                          "um", "micrometre",  "micrometer",
*                          "mm", "millimetre",  "millimeter",
*                          "cm", "centimetre",  "centimeter",
*                          "dm",  "decimetre",  "decimeter",
*                          "m",   "metre",      "meter",
*                          "dam", "decametre",  "decameter",
*                          "hm",  "hectometre", "hectometer",
*                          "km",  "kilometre",  "kilometer"
*  @return the thickness of all the beam filters used. If none is used, an empty array is returned.
*/
//--------------------------------------------------------------------------
std::vector<float> getFiltrationThickness(const std::string& aUnitOfLength);


//--------------------------------------------------------------------------
/// Enable Poisson noise for the energy fluence.
//--------------------------------------------------------------------------
void enablePoissonNoise();


//--------------------------------------------------------------------------
/// Disable Poisson noise for the energy fluence.
//--------------------------------------------------------------------------
void disablePoissonNoise();


//--------------------------------------------------------------------------
/// Set the total number of photons per pixel.
/**
*  @param aNumberOfPhotons: the number of photons
*  @param anApplyFiltrationFlag: true if the number of photons apply to the filtered spectrum;
*                                false if the number of photons apply to the unfiltered spectrum
*                                (default value: false)
*/
//--------------------------------------------------------------------------
void setNumberOfPhotons(double aNumberOfPhotons,
    bool anApplyFiltrationFlag = false);


//--------------------------------------------------------------------------
/// Set the total number of photons per pixel at the source-detector distance.
/**
*  @param aNumberOfPhotons: the number of photons
*  @param anApplyFiltrationFlag: true if the number of photons apply to the filtered spectrum;
*                                false if the number of photons apply to the unfiltered spectrum
*                                (default value: false)
*/
//--------------------------------------------------------------------------
void setNumberOfPhotonsPerPixelAtSDD(double aNumberOfPhotons,
    bool anApplyFiltrationFlag = false);


//--------------------------------------------------------------------------
/// Set the total number of photons per square centimetre at 1 metre.
/**
*  @param aNumberOfPhotons: the number of photons
*  @param anApplyFiltrationFlag: true if the number of photons apply to the filtered spectrum;
*                                false if the number of photons apply to the unfiltered spectrum
*                                (default value: false)
*/
//--------------------------------------------------------------------------
void setNumberOfPhotonsPerCm2At1m(double aNumberOfPhotons,
    bool anApplyFiltrationFlag = false);


//--------------------------------------------------------------------------
/// Accessor on the total number of photons per pixel.
/**
*  @param anApplyFiltrationFlag: true if the number of photons apply to the filtered spectrum;
*                                false if the number of photons apply to the unfiltered spectrum
*                                (default value: true)
*  @return the number of photons
*/
//--------------------------------------------------------------------------
double getNumberOfPhotons(bool anApplyFiltrationFlag = true);


//--------------------------------------------------------------------------
/// Accessor on the total number of photons per pixel.
/**
*  @param anApplyFiltrationFlag: true if the number of photons apply to the filtered spectrum;
*                                false if the number of photons apply to the unfiltered spectrum
*                                (default value: true)
*  @return the number of photons
*/
//--------------------------------------------------------------------------
double getNumberOfPhotonsPerPixelAtSDD(bool anApplyFiltrationFlag = true);


//--------------------------------------------------------------------------
/// Accessor on the total number of photons per square centimetre at 1 metre.
/**
*  @param anApplyFiltrationFlag: true if the number of photons apply to the filtered spectrum;
*                                false if the number of photons apply to the unfiltered spectrum
*                                (default value: true)
*  @return the number of photons
*/
//--------------------------------------------------------------------------
double getNumberOfPhotonsPerCm2At1m(bool anApplyFiltrationFlag = true);


//--------------------------------------------------------------------------
/// Enable or disable Poisson noise for the energy fluence.
/**
*  @param aFlag: if true, then enable Poisson noise. If false, then disable it
*/
//--------------------------------------------------------------------------
void usePoissonNoise(bool aFlag);


//--------------------------------------------------------------------------
/// Check if Poisson noise for the energy fluence is enable or disable.
/**
*  @return true if Poisson noise is enable, or false if Poisson noise is disable
*/
//--------------------------------------------------------------------------
bool usePoissonNoise();


//------------------------------------------------------------------------------
/// Get the largest possible number of energy bin in the spectrum.
/**
 *  @return the largest possible number of energy bin in the spectrum
 */
//------------------------------------------------------------------------------
int getLargestPossibleNumberOfEnergyBins();


#endif
