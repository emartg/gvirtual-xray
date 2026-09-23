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


#ifndef SGVXR_Detector_h
#define SGVXR_Detector_h


/**
********************************************************************************
*
*   @file       detector.h
*
*   @brief      Expose here all the functions of SimpleGVXR related to 
*               the X-ray detector. It includes it geometry and resolution as 
*               well as the scintilator.
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
#include <string>


//------------------------------------------------------------------------------
/// Set the number of pixels of the X-ray detector.
/**
 *  @param aWidth:  the number of pixels along the X-axis
 *  @param aHeight: the number of pixels along the Y-axis
 */
//------------------------------------------------------------------------------
void setDetectorNumberOfPixels(int aWidth, int aHeight);


//------------------------------------------------------------------------------
/// Accessor on the number of pixels of the X-ray detector.
/**
 *  @return the size of the detector in number of pixels
 */
//------------------------------------------------------------------------------
std::vector<int> getDetectorNumberOfPixels();


//--------------------------------------------------------------------------
/// Set the multiplicative factor to oversample the L-buffers.
/**
 *  @param aFactor:  the multiplicative factor (default value: 1, for no oversampling)
 */
//--------------------------------------------------------------------------
void setOversamplingFactor(int aFactor = 1);


//--------------------------------------------------------------------------
/// Accessor on the multiplicative factor to oversample the L-buffers.
/**
 *  @return the multiplicative factor
 */
//--------------------------------------------------------------------------
int getOversamplingFactor();


//------------------------------------------------------------------------------
/// Set the pixel size. Same as the function setDetectorPixelPitch.
/**
 *  @param aWidth:  the pixel size along the X-axis
 *  @param aHeight: the pixel size along the Y-axis
 *  @param aUnitOfLength:   the unit of length corresponding to the aWidth and
 *                          aHeight parameters. Acceptable values are:
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
void setDetectorPixelSize(double aWidth,
    double aHeight,
    const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Set the pixel pitch. It is the distance between the centre of two pixels.
/// It is often assumed that the edge of pixels have no width.
/// Same as the function setDetectorPixelSize.
/**
 *  @param aWidth:  the pixel size along the X-axis
 *  @param aHeight: the pixel size along the Y-axis
 *  @param aUnitOfLength:   the unit of length corresponding to the aWidth and
 *                          aHeight parameters. Acceptable values are:
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
void setDetectorPixelPitch(double aWidth,
    double aHeight,
    const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Accessor on the pixel spacing of the X-ray detector.
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
 *  @return the size in a given unit of length
 */
//------------------------------------------------------------------------------
std::vector<double> getDetectorPixelSpacing(const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Set the detector size by changing the pixel pitch, i.e. the number of pixels
/// will not be changed.
/**
 *  @param aWidth:  the size of the detector pannel along its horizontal axis
 *  @param aHeight:  the size of the detector pannel along its vertical axis
 *  @param aUnitOfLength:   the unit of length corresponding to
 *                          the width and height of the detector.
 *                          Acceptable values are:
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
void setDetectorSize(float aWidth, float aHeight, const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Accessor on the size of the X-ray detector.
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
 *  @return the size in a given unit of length
 */
//------------------------------------------------------------------------------
std::vector<double> getDetectorSize(const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Accessor on the area of a pixel of the X-ray detector.
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
 *  @return the size in a given unit of length
 */
//------------------------------------------------------------------------------
double getDetectorPixelArea(const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Set the position of the X-ray detector.
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
void setDetectorPosition(double x,
    double y,
    double z,
    const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Accessor on the position of the X-ray detector.
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
std::vector<double> getDetectorPosition(const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Set the up-vector defining the orientation of the X-ray detector.
/**
 *  @param x:   the component of the vector along the X-axis
 *  @param y:   the component of the vector along the Y-axis
 *  @param z:   the component of the vector along the Z-axis
 */
//------------------------------------------------------------------------------
void setDetectorUpVector(double x, double y, double z);


//------------------------------------------------------------------------------
/// Accessor on the up-vector defining the orientation of the X-ray detector.
/**
 *  @return the up-vector
 */
//------------------------------------------------------------------------------
std::vector<double> getDetectorUpVector();


//------------------------------------------------------------------------------
/// Set the right vector defining the orientation of the X-ray detector.
/**
 *  @param x:   the component of the vector along the X-axis
 *  @param y:   the component of the vector along the Y-axis
 *  @param z:   the component of the vector along the Z-axis
 */
//------------------------------------------------------------------------------
void setDetectorRightVector(double x, double y, double z);


//------------------------------------------------------------------------------
/// Accessor on the right-vector defining the orientation of the X-ray detector.
/**
 *  @return the right-vector
 */
//------------------------------------------------------------------------------
std::vector<double> getDetectorRightVector();


//------------------------------------------------------------------------------
/// Rotate the detector around a given axis.
/**
 *  @param anAngle:   the angle of rotation in degrees
 *  @param x:   the component of the rotation vector along the X-axis
 *  @param y:   the component of the rotation vector along the Y-axis
 *  @param z:   the component of the rotation vector along the Z-axis
 */
//------------------------------------------------------------------------------
void rotateDetector(double anAngle, double x, double y, double z);


//------------------------------------------------------------------------------
/// Set a flag to enable/disable the automatic alignment of the detector so that
/// the ray from the middle of the source to the centre of the detector is
/// perpendicular to both the unit vectors defining the horizontal and vertical 
/// axes of the detector.
/**
 *  @param aFlag:   the boolean flag (default value: true)
 */
//------------------------------------------------------------------------------
void autoAlignDetector(bool aFlag = true);


//------------------------------------------------------------------------------
/// Accessor on the four corners of the X-ray detector.
/**
 *  @return the corresponding vertices
 */
//------------------------------------------------------------------------------
std::vector<float> getDetectorVertices();


//--------------------------------------------------------------------------
/// Set the line spread function from a file.
/**
*  @param aFileName: the input file name
*/
//--------------------------------------------------------------------------
void setLSF(const std::string& aFileName);


//--------------------------------------------------------------------------
/// Set the line spread function from an array.
/**
*  @param aLSF: the LSF coefficients
*/
//--------------------------------------------------------------------------
void setLSF(const std::vector<float>& aLSF);


//--------------------------------------------------------------------------
/// Accessor on the line spread function.
/**
*  @return the LSF coefficients
*/
//--------------------------------------------------------------------------
const std::vector<float>& getLSF();


//--------------------------------------------------------------------------
/// Clear the LSF to use a Dirac as impulse response.
//--------------------------------------------------------------------------
void clearLSF();










//------------------------------------------------------------------------------
/// Load the energy response of the detector from an array in the memory.
/**
 *  @param anEnergyResponse: the energy response as an array. Each row of the array is formatted as follows:
 *                    input_energy  output_energy
 *  @param aUnitOfEnergy:   the unit of energy corresponding to the data in the array
 *                          Acceptable values are:
 *                          "electronvolt", "eV",
 *                          "kiloelectronvolt", "keV",
 *                          "megaelectronvolt", "MeV"
 */
//------------------------------------------------------------------------------
void setDetectorEnergyResponse(const std::vector<std::vector<float> >& anEnergyResponse,
    const std::string& aUnitOfEnergy);


//------------------------------------------------------------------------------
/// Load the energy response of the detector from a TSV file.
/**
 *  @param aFileName: name of the file. Each line of the file is formatted as follows:
 *                    input_energy  output_energy
 *  @param aUnitOfEnergy:   the unit of energy corresponding to the data in the file.
 *                          Acceptable values are:
 *                          "electronvolt", "eV",
 *                          "kiloelectronvolt", "keV",
 *                          "megaelectronvolt", "MeV"
*/
//------------------------------------------------------------------------------
void loadDetectorEnergyResponse(const std::string& aFileName,
    const std::string& aUnitOfEnergy);


//------------------------------------------------------------------------------
/// Accessor on the energy response of the detector.
/**
 *  @param aUnitOfEnergy:   the unit of energy corresponding to the data in the array.
 *                          Acceptable values are:
 *                          "electronvolt", "eV",
 *                          "kiloelectronvolt", "keV",
 *                          "megaelectronvolt", "MeV"
 *  @return the energy response as an array. Each row of the array is formatted as follows:
 *                    input_energy  output_energy
 */
//------------------------------------------------------------------------------
std::vector<std::vector<float> > getEnergyResponse(const std::string& aUnitOfEnergy);


//------------------------------------------------------------------------------
/// Clear the energy response of the detector.
//------------------------------------------------------------------------------
void clearDetectorEnergyResponse();


//------------------------------------------------------------------------------
/// Set the scintillator material of the detector. It will generate the energy response of the detector.
/**
 *  @param aMaterial:   the material of the scintillator.
 *                      Acceptable values are:
 *                      "CsI", "Cesium iodine",
 *                      "CdWO4", "CWO", "Cadmium tungstate",
 *                      "Y2O3", "Yttrium oxyde",
 *                      "Gd2O3", "Gadolinum oxyde",
 *                      "(Y,Gd)2O3", "YGO", "Yttrium gadolinum oxyde",
 *                      "Gd2O2S", "Gadox", "Gadolinum silicate", "GOS",
 *                      "Gd2O2S DRZ-Plus", "Gadox DRZ-Plus",
 *                      "NaI", "Sodium iodine",
 *                      "La2HfO7", "Lanthanum hafnate",
 *                      "Gd3Ga5O12", "Gadolinium gallium garnet",
 *                      "Y3Al5O12", "Yttrium aluminium garnate", "YAG"
 *  @param aThickness:  the thickness of the scintillator.
 *  @param aUnitOfLength:   the unit corresponding to the thickness of the scintillator.
 *                          Acceptable values are:
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
void setScintillator(const std::string& aMaterial,
    const float& aThickness,
    const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Accessor on the material of the scintillator of the detector.
/**
 *  @return the material of the scintillator
 */
//------------------------------------------------------------------------------
const std::string& getScintillatorMaterial();


//------------------------------------------------------------------------------
/// Accessor on the list of scintillator materials currently supported.
/**
 *  @return the list of supported scintillator materials
 */
//------------------------------------------------------------------------------
std::vector<std::string> getSupportedScintillatorMaterials();


//------------------------------------------------------------------------------
/// Accessor on the energy response of a  detector scintillator of a given material and thickness.
/**
 *  @param aMaterial:   the material of the scintillator.
 *                      Acceptable values are:
 *                      "CsI", "Cesium iodine",
 *                      "CdWO4", "CWO", "Cadmium tungstate",
 *                      "Y2O3", "Yttrium oxyde",
 *                      "Gd2O3", "Gadolinum oxyde",
 *                      "(Y,Gd)2O3", "YGO", "Yttrium gadolinum oxyde",
 *                      "Gd2O2S", "Gadox", "Gadolinum silicate", "GOS",
 *                      "Gd2O2S DRZ-Plus", "Gadox DRZ-Plus",
 *                      "NaI", "Sodium iodine",
 *                      "La2HfO7", "Lanthanum hafnate",
 *                      "Gd3Ga5O12", "Gadolinium gallium garnet",
 *                      "Y3Al5O12", "Yttrium aluminium garnate", "YAG"
 *  @param aThickness:  the thickness of the scintillator.
 *  @param aUnitOfLength:   the unit corresponding to the thickness of the scintillator.
 *                          Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 *  @param aUnitOfEnergy:   the unit of energy corresponding to the data in the array.
 *                          Acceptable values are:
 *                          "electronvolt", "eV",
 *                          "kiloelectronvolt", "keV",
 *                          "megaelectronvolt", "MeV"
 *  @return the corresponding energy response
 */
//------------------------------------------------------------------------------
std::vector<std::vector<float> > getEnergyResponse(const std::string& aMaterial,
    const float& aThickness,
    const std::string& aUnitOfLength,
    const std::string& aUnitOfEnergy);


//------------------------------------------------------------------------------
/// Accessor on the thickness of the scintillator of the detector.
/**
 *  @param aUnitOfLength:   the unit corresponding to the thickness of the scintillator.
 *                          Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 *  @return the thickness of the scintillator
 */
//------------------------------------------------------------------------------
double getScintillatorThickness(const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Define the energy bands of a spectral detector. Photon counting will be used.
/**
 *  @param aMinEnergy:  the lower energy threshod
 *  @param aMaxEnergy:  the upper energy threshod
 *  @param aUnitOfEnergy:   the unit of energy corresponding to aMinEnergy and aMaxEnergy.
 *                          Acceptable values are:
 *                          "electronvolt", "eV",
 *                          "kiloelectronvolt", "keV",
 *                          "megaelectronvolt", "MeV"
 *  @param aNumberOfBands:  the number of energy bands
 */
 //------------------------------------------------------------------------------
void setPhotonCountingBands(double aMinEnergy,
    double aMaxEnergy,
    const std::string& aUnitOfEnergy,
    unsigned int aNumberOfBands);


//------------------------------------------------------------------------------
/// Accessor on the number of energy bands of the spectral detector.
/**
 *  @return the number of energy bands of the spectral detector
 */
 //------------------------------------------------------------------------------
unsigned int getNumberOfPhotonCountingBands();


//------------------------------------------------------------------------------
/// Merge all the energy bands. In this case a typical detector will be used and it'll integrate the energy.
//------------------------------------------------------------------------------
void mergePhotonCountingBands();


//------------------------------------------------------------------------------
/// Accessor on the energy bands when a spectral detector is used.
/**
 *  @param aUnitOfEnergy:   the unit of energy corresponding to energies in the bands.
 *                          Acceptable values are:
 *                          "electronvolt", "eV",
 *                          "kiloelectronvolt", "keV",
 *                          "megaelectronvolt", "MeV"
 *  @return the set of energy bands
 */
 //------------------------------------------------------------------------------
std::vector<double> getPhotonCountingBands(const std::string& aUnitOfEnergy);


//------------------------------------------------------------------------------
/// Accessor on the detector gain.
/**
 *  @return the gain
 */
//------------------------------------------------------------------------------
float getDetectorGain();


//------------------------------------------------------------------------------
/// Set the detector gain.
/**
 *  @param aGain:   the gain
 */
//------------------------------------------------------------------------------
void setDetectorGain(float aGain);


//------------------------------------------------------------------------------
/// Accessor on the detector bias.
/**
 *  @return the bias
 */
//------------------------------------------------------------------------------
float getDetectorBias();


//------------------------------------------------------------------------------
/// Set the detector bias.
/**
 *  @param aBias:   the bias
 */
//------------------------------------------------------------------------------
void setDetectorBias(float aBias);


//------------------------------------------------------------------------------
/// Accessor on the standard deviation of the detector bias.
/**
 *  @return the standard deviation of the detector bias
 */
//------------------------------------------------------------------------------
float getDetectorStdDevBias();


//------------------------------------------------------------------------------
/// Set the standard deviation of the detector bias.
/**
 *  @param aStdDevBias: the standard deviation of the detector bias
 */
//------------------------------------------------------------------------------
void setDetectorStdDevBias(float aStdDevBias);


//------------------------------------------------------------------------------
/// Get the largest possible number of pixels on this hardware implementation.
/**
 *  @return the largest possible number of pixels on this hardware implementation
 */
//------------------------------------------------------------------------------
std::vector<int> getLargestPossibleDetectorNumberOfPixels();


#endif
