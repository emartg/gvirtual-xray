/*

Copyright (c) 2018-2023, Dr Franck P. Vidal, Bangor University, All rights reserved.
Copyright (c) 2023-present, Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), 
UK Research and Innovation, All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

3. Neither the name of the Bangor University, UK Research and Innovation nor the names of its contributors
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


#ifndef SGVXR_SimpleGVXR_h
#define SGVXR_SimpleGVXR_h


/**
********************************************************************************
*
*   @file       SimpleGVXR.h
*
*   @brief      SimpleGVXR is an API to the bulk functionalities and classes of
*               gVirtualXRay. SimpleGVXR is used as the basis for
*               the language bindings generated using SWIG.
*
*   @version    1.0
*
*   @date       02/05/2024
*
*   @author     Prof Franck P. Vidal
*
*   @todo       Write the equation for computeZNCC.
* 
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Dr Franck P. Vidal, Bangor University, Apr 2019-Sept 2023, version 1.1, BSD 3-Clause License
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, Oct 2023-present, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <vector>
#include <utility>
#include <string>

#ifndef SGVXR_Acquisition_h
#include "acquisition.h"
#endif

#ifndef SGVXR_detector_h
#include "detector.h"
#endif

#ifndef SGVXR_IMPLICIT_SURFACE_H
#include "ImplicitSurface.h"
#endif

#ifndef SGVXR_sample_h
#include "sample.h"
#endif

#ifndef SGVXR_source_h
#include "source.h"
#endif

#ifndef SGVXR_Utils_h
#include "Utils.h"
#endif

#ifndef SGVXR_visualisation_h
#include "visualisation.h"
#endif


//------------------------------------------------------------------------------
/// Accessor on the major version of the core gVirtualXRay library (gvxr).
/// This number is changed when incompatible API changes have been made.
/**
 *  @return the major version number
 */
//------------------------------------------------------------------------------
int getMajorVersionOfCoreGVXR();


//------------------------------------------------------------------------------
/// Accessor on the minor version of the core gVirtualXRay library (gvxr).
/// This number is changed when new functionalities have been added in
/// a backwards-compatible manner.
/**
 *  @return the minor version number
 */
//------------------------------------------------------------------------------
int getMinorVersionOfCoreGVXR();


//------------------------------------------------------------------------------
/// Accessor on the minor version of the core gVirtualXRay library (gvxr).
/// This number is changed when bug fixes have been added in
/// a backwards-compatible manner.
/**
 *  @return the patch version number
 */
//------------------------------------------------------------------------------
int getPatchVersionOfCoreGVXR();


//------------------------------------------------------------------------------
/// Accessor on the full string version of the core gVirtualXRay library (gvxr).
/**
 *  @return the full version number
 */
//------------------------------------------------------------------------------
std::string getVersionOfCoreGVXR();


//------------------------------------------------------------------------------
/// Accessor on the major version of SimpleGVXR. This number is changed
/// when incompatible API changes have been made.
/**
 *  @return the major version number
 */
//------------------------------------------------------------------------------
int getMajorVersionOfSimpleGVXR();


//------------------------------------------------------------------------------
/// Accessor on the minor version of SimpleGVXR. This number is changed when
/// new functionalities have been added in a backwards-compatible manner.
/**
 *  @return the minor version number
 */
//------------------------------------------------------------------------------
int getMinorVersionOfSimpleGVXR();


//------------------------------------------------------------------------------
/// Accessor on the minor version of SimpleGVXR. This number is changed when
/// bug fixes have been added in a backwards-compatible manner.
/**
 *  @return the patch version number
 */
//------------------------------------------------------------------------------
int getPatchVersionOfSimpleGVXR();


//------------------------------------------------------------------------------
/// Accessor on the full string version of SimpleGVXR.
/**
 *  @return the full version number
 */
//------------------------------------------------------------------------------
std::string getVersionOfSimpleGVXR();


//------------------------------------------------------------------------------
/// Accessor on the OpenGL renderer.
/**
 *  @return a string displaying the OpenGL renderer
 */
//------------------------------------------------------------------------------
std::string getOpenGlRenderer();


//------------------------------------------------------------------------------
/// Accessor on the OpenGL version.
/**
 *  @return a string displaying the OpenGL version
 */
//------------------------------------------------------------------------------
std::string getOpenGlVersion();


//------------------------------------------------------------------------------
/// Accessor on the OpenGL vendor.
/**
 *  @return a string displaying the OpenGL vendor
 */
//------------------------------------------------------------------------------
std::string getOpenGlVendor();


//------------------------------------------------------------------------------
/// Accessor on the shading language version or release number.
/**
 *  @return a string displaying the shading language version or release number
 */
//------------------------------------------------------------------------------
std::string getShadingLanguageVersion();


//------------------------------------------------------------------------------
/// Accessor on the Poisson noise implementation.
/**
 *  @return a string displaying the Poisson noise implementation, e.g.
 *          C++ standard library or gVXR's own implementation on CPU, with or without OpenMP (a GPU version is planned in future).
 */
//------------------------------------------------------------------------------
std::string getNoiseGenerator();


//------------------------------------------------------------------------------
/// Print out all the build environment variables.
/**
 *  @return a string all the build environment variables.
 */
 //------------------------------------------------------------------------------
std::string getBuildEnvironment();


//------------------------------------------------------------------------------
/// Turn on/off the use of a log file.
/**
 *  @param aFileName: the name of the file in which the output will be saved. 
 *                    If it is equal to 0, NULL, or nullptr, the standard output,
 *                    will be used (default value: "gvxr.log")
 */
//------------------------------------------------------------------------------
void useLogFile(const char* aFileName = "gvxr.log");


//------------------------------------------------------------------------------
/// Turn on/off the use of a log file.
/**
 *  @param aFileName: the name of the file in which the output will be saved.
 *                    If it is equal to 0, NULL, or nullptr, the standard output,
 *                    will be used (default value: "gvxr.log")
 */
//------------------------------------------------------------------------------
void useLogFile(const std::string& aFileName);


//------------------------------------------------------------------------------
/// Set the seed of the random number generator.
/**
 *  @param aRandomGeneratorSeed:    the seed
 */
//------------------------------------------------------------------------------
void setSeed(unsigned int aRandomGeneratorSeed);






//------------------------------------------------------------------------------
/// Create an OpenGL context automatically.
/**
 *  @param aFlag:   true to create the OpenGL context automatically,
 *                  false otherwise (default value: true)
 */
//------------------------------------------------------------------------------
void autoCreateOpenGLContext(bool aFlag = true);







//------------------------------------------------------------------------------
/// Return a 4x4 identity matrix.
/**
 *  @return a 4x4 identity matrix
 */
//------------------------------------------------------------------------------
std::vector<double> getIdentityMatrix4();





//------------------------------------------------------------------------------
/// Display the scenegraph node using OpenGL.
/**
 *  @param aLabel:          the label of the polygon mesh
 */
//------------------------------------------------------------------------------
void displayNode(const std::string& aLabel);






//------------------------------------------------------------------------------
/// Replace the 3-D transformation matrix of the overall 3-D scene by
/// an identity matrix.
//------------------------------------------------------------------------------
void resetSceneTransformation();


//------------------------------------------------------------------------------
/// Scale the 3-D scene
/// (all the polygon meshes will be affected but not modified).
/**
 *  @param x:   the scaling factor along the X-axis
 *  @param y:   the scaling factor along the Y-axis
 *  @param z:   the scaling factor along the Z-axis
 */
//------------------------------------------------------------------------------
void scaleScene(double x, double y, double z);


//------------------------------------------------------------------------------
/// Scale the 3-D scene
/// (all the polygon meshes will be affected but not modified).
/**
 *  @param x:   the scaling factor along the X-axis
 *  @param y:   the scaling factor along the Y-axis
 *  @param z:   the scaling factor along the Z-axis
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
void scaleScene(double x, double y, double z, const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Rotate the 3-D scene
/// (all the polygon meshes will be affected but not modified).
/**
 *  @param anAngle:   the rotation angle in degrees
 *  @param x:   the component of the rotation vector along the X-axis
 *  @param y:   the component of the rotation vector along the Y-axis
 *  @param z:   the component of the rotation vector along the Z-axis
 */
//------------------------------------------------------------------------------
void rotateScene(double anAngle, double x, double y, double z);


//------------------------------------------------------------------------------
/// Rotate the 3-D scene using the yaw, pitch, roll method
/// (all the polygon meshes will be affected but not modified).
/**
 *  @param yaw: a counterclockwise rotation of yaw degrees about the z-axis
 *  @param pitch: a counterclockwise rotation of yaw degrees about the y-axis
 *  @param roll: a counterclockwise rotation of yaw degrees about the x-axis
 */
//------------------------------------------------------------------------------
void rotateScene(double yaw, double pitch, double roll);


//------------------------------------------------------------------------------
/// Translate the 3-D scene
/// (all the polygon meshes will be affected but not modified).
/**
 *  @param x:   the component of the translation vector along the X-axis
 *  @param y:   the component of the translation vector along the Y-axis
 *  @param z:   the component of the translation vector along the Z-axis
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
void translateScene(double x,
                    double y,
                    double z,
                    const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Accessor on the 3-D scene transformation matrix.
/**
 *  @return the 3-D scene transformation matrix as a 4x4 array
 */
//------------------------------------------------------------------------------
std::vector<std::vector<float> > getSceneTransformationMatrix();


//------------------------------------------------------------------------------
/// Accessor on the transformation matrix of the scengraph's root node.
/**
 *  @return the scengraph's root node transformation matrix as a 4x4 array
 */
//------------------------------------------------------------------------------
std::vector<std::vector<float> > getRootTransformationMatrix();


//------------------------------------------------------------------------------
/// Accessor on the transformation matrix of a given node.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @return the scengraph's root node transformation matrix as a 4x4 array
 */
//------------------------------------------------------------------------------
std::vector<std::vector<float> > getNodeLocalTransformationMatrix(
    const std::string& aLabel);


//------------------------------------------------------------------------------
/// Accessor on the transformation matrix of a given node.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @return the scengraph's root node transformation matrix as a 4x4 array
 */
//------------------------------------------------------------------------------
std::vector<std::vector<float> > getNodeWorldTransformationMatrix(
    const std::string& aLabel);


//------------------------------------------------------------------------------
/// Set the 3-D scene transformation matrix.
/**
 *  @param aMatrix: the transformation matrix as a 4x4 array
 */
//------------------------------------------------------------------------------
void setSceneTransformationMatrix(
    const std::vector<std::vector<float> >& aMatrix);


//------------------------------------------------------------------------------
/// Set the transformation matrix of the scengraph's root node.
/**
 *  @param aMatrix: the transformation matrix as a 4x4 array
 */
//------------------------------------------------------------------------------
void setRootTransformationMatrix(
    const std::vector<std::vector<float> >& aMatrix);


//------------------------------------------------------------------------------
/// Set the transformation matrix of a given node.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @param aMatrix: the transformation matrix as a 4x4 array
 */
//------------------------------------------------------------------------------
void setNodeTransformationMatrix(
    const std::string& aLabel,
    const std::vector<std::vector<float> >& aMatrix);


//------------------------------------------------------------------------------
/// Set the Hounsfield value corresponding to the material properties of
/// a polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @param HU:      the Hounsfield value
 */
//------------------------------------------------------------------------------
void setHU(const std::string& aLabel, short HU);


//------------------------------------------------------------------------------
/// Set the Hounsfield value corresponding to the material properties of
/// a polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @param HU:      the Hounsfield value
 */
//------------------------------------------------------------------------------
void setHounsfieldUnit(const std::string& aLabel, short HU);


//------------------------------------------------------------------------------
/// Set the Hounsfield value corresponding to the material properties of
/// a polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @param HU:      the Hounsfield value
 */
//------------------------------------------------------------------------------
void setHounsfieldValue(const std::string& aLabel, short HU);


//------------------------------------------------------------------------------
/// Set the chemical element (or element) corresponding
/// to the material properties of a polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @param Z:       the atomic number (or Z number) corresponding to the element
 */
//------------------------------------------------------------------------------
void setElement(const std::string& aLabel, unsigned short Z);


//------------------------------------------------------------------------------
/// Set the chemical element (or element) corresponding
/// to the material properties of a polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @param aName:   the symbol or name corresponding to the element
 */
//------------------------------------------------------------------------------
void setElement(const std::string& aLabel, const std::string& aName);


//------------------------------------------------------------------------------
/// Set the mixture corresponding to the material properties of a polygon mesh.
/// Don't forget to set the density of the material.
/**
 *  @param aLabel:      the label of the polygon mesh
 *  @param aMixture:    the details about the mixture. It is given as a sequence
 *                      of element symbol & relative weight, e.g.
 *                      Ti90Al6V4.
 */
//------------------------------------------------------------------------------
void setMixture(const std::string& aLabel, const std::string& aMixture);


//------------------------------------------------------------------------------
/// Set the mixture corresponding to the material properties of a polygon mesh.
/// Don't forget to set the density of the material.
/**
 *  @param aLabel:      the label of the polygon mesh
 *  @param aZNumberSet:      the array of Z numbers
 *  @param aWeightSet:      the corresponding weights
 */
//------------------------------------------------------------------------------
void setMixture(const std::string& aLabel,
                const std::vector<int>& aZNumberSet,
                const std::vector<double>& aWeightSet);


//------------------------------------------------------------------------------
/// Set the mixture corresponding to the material properties of a polygon mesh.
/// Don't forget to set the density of the material.
/**
 *  @param aLabel:      the label of the polygon mesh
 *  @param aSymbolSet:      the array of element symbols
 *  @param aWeightSet:      the corresponding weights
 */
//------------------------------------------------------------------------------
void setMixture(const std::string& aLabel,
                const std::vector<std::string>& aSymbolSet,
                const std::vector<double>& aWeightSet);


//------------------------------------------------------------------------------
/// Set the compound corresponding to the material properties of a polygon mesh.
/// Don't forget to set the density of the material.
/**
 *  @param aLabel:      the label of the polygon mesh
 *  @param aCompound:   the details about the compound. It is given as
 *                      a sequence of element symbol & number of atoms, e.g.
 *                      H2O for water and SiC for silicon carbide.
 */
//------------------------------------------------------------------------------
void setCompound(const std::string& aLabel, const std::string& aCompound);


//------------------------------------------------------------------------------
/// Set the mass attenuation coefficient corresponding to
/// the material properties of a polygon mesh.
/// Don't forget to set the density of the material.
/**
 *  @param aLabel:          the label of the polygon mesh
 *  @param aCoefficient:    the mass attenuation coefficient
 *  @param aUnit:           the unit corresponding to aCoefficient.
 *                          Acceptable values are:
 *                          "cm2/g" and "cm2.g-1"
 */
//------------------------------------------------------------------------------
void setMassAttenuationCoefficient(const std::string& aLabel,
                                   double aCoefficient,
                                   const std::string& aUnit);


//------------------------------------------------------------------------------
/// Set the linear attenuation coefficient corresponding
/// to the material properties of a polygon mesh.
/**
 *  @param aLabel:          the label of the polygon mesh
 *  @param aCoefficient:    the liner attenuation coefficient
 *  @param aUnit:           the unit corresponding to aCoefficient.
 *                          Acceptable values are:
 *                          "cm-1"
 */
//------------------------------------------------------------------------------
void setLinearAttenuationCoefficient(const std::string& aLabel,
                                     double aCoefficient,
                                     const std::string& aUnit);


//------------------------------------------------------------------------------
/// Set the linear attenuation coefficient corresponding
/// to the material properties of a polygon mesh.
/**
 *  @param aLabel:          the label of the polygon mesh
 *  @param aCoefficient:    the liner attenuation coefficient
 *  @param aUnit:           the unit corresponding to aCoefficient.
 *                          Acceptable values are:
 *                          "cm-1"
 */
//------------------------------------------------------------------------------
void setMu(const std::string& aLabel,
           double aCoefficient,
           const std::string& aUnit);


//------------------------------------------------------------------------------
/// Set the density corresponding to the material properties of a polygon mesh.
/**
 *  @param aLabel:      the label of the polygon mesh
 *  @param aDensity:    the density
 *  @param aUnit:       the unit corresponding to aDensity.
 *                      Acceptable values are:
 *                      "g/cm3", "g.cm-3", "mg/cm3", "mg.cm-3",
 *                      "kg/m3", and "kg.m-3".
 */
//------------------------------------------------------------------------------
void setDensity(const std::string& aLabel,
                double aDensity,
                const std::string& aUnit);




//------------------------------------------------------------------------------
/// Accessor on the density corresponding
/// to the material properties of a polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @return the density of the polygon mesh's material in g/cm3
 */
//------------------------------------------------------------------------------
double getDensity(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Accessor on the mass attenuation coefficient at a given energy
/// corresponding to the material properties of a polygon mesh.
/**
 *  @param aLabel:          the label of the polygon mesh
 *  @param anEnergy:        the photon energy of interest
 *  @param aUnitOfEnergy:   the unit of energy corresponding to anEnergy.
 *                          Acceptable values are:
 *                          "electronvolt", "eV",
 *                          "kiloelectronvolt", "keV",
 *                          "megaelectronvolt", "MeV"
 *  @return the mass attenuation coefficient of the polygon mesh's material
 *          in cm2/g
 */
//------------------------------------------------------------------------------
double getMassAttenuationCoefficient(const std::string& aLabel,
                                     double anEnergy,
                                     const std::string& aUnitOfEnergy);


//------------------------------------------------------------------------------
/// Accessor on the linear attenuation coefficient at a given energy
/// corresponding to the material properties of a polygon mesh.
/**
 *  @param aLabel:          the label of the polygon mesh
 *  @param anEnergy:        the photon energy of interest
 *  @param aUnitOfEnergy:   the unit of energy corresponding to anEnergy.
 *                          Acceptable values are:
 *                          "electronvolt", "eV",
 *                          "kiloelectronvolt", "keV",
 *                          "megaelectronvolt", "MeV"
 *  @return the linear attenuation coefficient of the polygon mesh's material
 *          in cm-1
 */
//------------------------------------------------------------------------------
double getLinearAttenuationCoefficient(const std::string& aLabel,
                                       double anEnergy,
                                       const std::string& aUnitOfEnergy);


//------------------------------------------------------------------------------
/// Accessor on the material label of a polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @return the label of the material
 */
//------------------------------------------------------------------------------
std::string getMaterialLabel(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Accessor on the compound description of a polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @return if the sample is made of a compound, then return the description of the compound (e.g. H2O for water), otherwise return an empty string
 */
//------------------------------------------------------------------------------
std::string getCompound(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Accessor on the element Z number set of the mixture of a polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @return if the sample is made of a mixture, then return the element Z number set of the mixture, otherwise return an empty vector
 */
//------------------------------------------------------------------------------
std::vector<int> getMixtureElementSet(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Accessor on the element weight set of the mixture of a polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @return if the sample is made of a mixture, then return the element weight set of the mixture, otherwise return an empty vector
 */
//------------------------------------------------------------------------------
std::vector<double> getMixtureWeightSet(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Accessor on the effective atomic number of a polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @return the effective atomic number
 */
//------------------------------------------------------------------------------
float getEffectiveZNumber(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Accessor on the effective atomic number of a polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @return the effective atomic number
 */
//------------------------------------------------------------------------------
float getEffectiveAtomicNumber(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Accessor on the effective molar mass of a polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @return the effective molar mass
 */
//------------------------------------------------------------------------------
float getEffectiveMolarMass(const std::string& aLabel);


//------------------------------------------------------------------------------
/// 3-D visualisation of the 3-D scene (source, detector, and scanned objects).
/// Note that there is no interactive loop running.
/**
 *  @param aSceneRotationFlag:  true if the 3-D scene has to be rotated with
 *                              the arc-ball method, false otherwise
 *                              (default value: true)
 *  @param aWindowID:   the numerical ID of the corresponding context
 *                      (default value: -1, means that the active context
 *                      will be used)
 */
//------------------------------------------------------------------------------
void displayScene(bool aSceneRotationFlag = true, int aWindowID = -1);


//------------------------------------------------------------------------------
/// 3-D visualisation of the 3-D scene (source, detector, and scanned objects).
/// Note that there is interactive loop running.
/// Keys are:
/// - Q/Escape: to quit the event loop (does not close the window)
/// - B: display/hide the X-ray beam
/// - W: display the polygon meshes in solid or wireframe
/// - N: display the X-ray image in negative or positive
/// - L: switch lighting on/off
/// - D: display/hide the X-ray detector
/// - V: display/hide normal vectors
/**
 *  @param aWindowID:   the numerical ID of the corresponding context
 *                      (default value: -1, means that the active context
 * will be used)
 */
//------------------------------------------------------------------------------
void renderLoop(int aWindowID = -1);


//------------------------------------------------------------------------------
/// Set the title of an interactive window.
/**
 *  @param aTitle: the title shown by the window manager
 *  @param aWindowID: the numerical ID of the corresponding window
 *                    (default value: -1, means that the active window is used)
 */
//------------------------------------------------------------------------------
void setWindowTitle(const std::string& aTitle, int aWindowID = -1);


//------------------------------------------------------------------------------
/// Call this function when the user starts an arc-ball rotation
/// (e.g. left-mouse button click in the 3-D visualisation window).
/**
 *  @param x:   the cursor x-axis position in the viewport coordinate system
 *  @param y:   the cursor y-axis position in the viewport coordinate system
 */
//------------------------------------------------------------------------------
void startArcBallRotation(double x, double y);


//------------------------------------------------------------------------------
/// Call this function when the user stops an arc-ball rotation
/// (e.g. release the left-mouse button click in the 3-D visualisation window).
//------------------------------------------------------------------------------
void stopArcBallRotation();


//------------------------------------------------------------------------------
/// Check if arc-ball rotation is currently in use.
/**
 *  @return true if arc-ball rotation is currently in use, false otherwise
 */
//------------------------------------------------------------------------------
bool usingArcBallRotation();


//------------------------------------------------------------------------------
/// Call this function when the user moves the mouse
/**
 *  @param x:   the cursor x-axis position in the viewport coordinate system
 *  @param y:   the cursor y-axis position in the viewport coordinate system
 *  @param aViewportWidth:  the size of the viewport along the x-axis
 *  @param aViewportHeight: the size of the viewport along the y-axis
 */
//------------------------------------------------------------------------------
void cursorPositionCallback(double x,
                            double y,
                            int aViewportWidth,
                            int aViewportHeight);


//------------------------------------------------------------------------------
/// Call this function when the user scrolls
/**
 *  @param xoffset: the scroll offset along the x-axis (not used)
 *  @param yoffset: the scroll offset along the y-axis (used for zooming in
 *                  and out)
 */
//------------------------------------------------------------------------------
void scrollCallback(double xoffset, double yoffset);


//------------------------------------------------------------------------------
/// Set the zoom value used in the 3D visualisation.
/**
 *  @param aZoomValue: the zoom value
 */
//------------------------------------------------------------------------------
void setZoom(float aZoomValue);


//------------------------------------------------------------------------------
/// Accessor on the zoom value used in the 3D visualisation.
/**
 *  @return the zoom value
 */
//------------------------------------------------------------------------------
float getZoom();


//------------------------------------------------------------------------------
/// Set the transformation matrix used in the 3D visualisation.
/**
 *  @param aRotationMatrix: the transformation matrix
 */
//------------------------------------------------------------------------------
void setSceneRotationMatrix(const std::vector<double>& aRotationMatrix);


//------------------------------------------------------------------------------
/// Accessor on the transformation matrix used in the 3D visualisation.
/**
 *  @return the transformation matrix
 */
//------------------------------------------------------------------------------
std::vector<double> getSceneRotationMatrix();


//------------------------------------------------------------------------------
/// Make an OpenGL context visible and display a window.
/**
 *  @param aWindowID:   the numerical ID of the corresponding context
 *                      (default value: -1, means that the active context
 *                      will be used)
 */
//------------------------------------------------------------------------------
void showWindow(int aWindowID = -1);


//------------------------------------------------------------------------------
/// Hide a window.
/**
 *  @param aWindowID:   the numerical ID of the corresponding context
 *                      (default value: -1, means that the active context
 *                      will be used)
 */
//------------------------------------------------------------------------------
void hideWindow(int aWindowID = -1);


//------------------------------------------------------------------------------
/// Move a window.
/**
 *  @param aPositionX:  the window position along the X axis of the screen
 *  @param aPositionY:  the window position along the Y axis of the screen
 *  @param aWindowID:   the numerical ID of the corresponding context
 *                      (default value: -1, means that the active context
 *                      will be used)
 */
//------------------------------------------------------------------------------
void setWindowPosition(int aPositionX, int aPositionY, int aWindowID = -1);


//------------------------------------------------------------------------------
/// Set window background colour.
/**
 *  @param R: the red colour [0, 1]
 *  @param G: the green colour [0, 1]
 *  @param B: the blue colour [0, 1]
 *  @param aWindowID:   the numerical ID of the corresponding context
 *                      (default value: -1, means that the active context
 *                      will be used)
 */
//------------------------------------------------------------------------------
void setWindowBackGroundColour(float R, float G, float B, int aWindowID = -1);


//------------------------------------------------------------------------------
/// Set window background colour.
/**
 *  @param R: the red colour [0, 1]
 *  @param G: the green colour [0, 1]
 *  @param B: the blue colour [0, 1]
 *  @param aWindowID:   the numerical ID of the corresponding context
 *                      (default value: -1, means that the active context
 *                      will be used)
 */
//------------------------------------------------------------------------------
void setWindowBackGroundColor(float R, float G, float B, int aWindowID = -1);


//------------------------------------------------------------------------------
/// Take screenshot.
/**
 *  @param aWindowID:   the numerical ID of the corresponding context
 *                      (default value: -1, means that the active context
 *                      will be used)
 *  @return the 2D image in RGB
 */
//------------------------------------------------------------------------------
const std::vector<std::vector<std::vector< float> > >& takeScreenshot(int aWindowID = -1);


//------------------------------------------------------------------------------
/// Get the latest screenshot.
/**
 *  @return the 2D image in RGB
 */
//------------------------------------------------------------------------------
const std::vector<std::vector<std::vector< float> > >& getLatestScreenshot();


//------------------------------------------------------------------------------
/// Get a flat field image that can be used in the flat-field correction (FFC).
/// Note that if Poisson noise is used, the function will return a different
/// image each time the function is called.
/**
 *  @param anIntegrateEnergyFlag:   if true the energy fluence is returned,
 *                                  otherwise the number of photons is returned
 *                                  (default value: true)
 *  @return the 2D image in greyscale values
 */
//------------------------------------------------------------------------------
std::vector<std::vector< float> > getWhiteImage(bool anIntegrateEnergyFlag = true);


//------------------------------------------------------------------------------
/// Get a flat field image that can be used in the flat-field correction (FFC).
/// Note that if Poisson noise is used, the function will return a different
/// image each time the function is called.
/**
 *  @param anIntegrateEnergyFlag:   if true the energy fluence is returned,
 *                                  otherwise the number of photons is returned
 *                                  (default value: true)
 *  @return the 2D image in greyscale values
 */
//------------------------------------------------------------------------------
std::vector<std::vector< unsigned short > > getWhiteImageWithGain(bool anIntegrateEnergyFlag = true);


//------------------------------------------------------------------------------
/// Get a set of flat field images that can be used in the flat-field correction (FFC).
/// When a spectral detector is used, a 2D image is generated for each energy band.
/// Note that if Poisson noise is used, the function will return a different
/// set each time the function is called.
/**
 *  @return the set of 2D images in greyscale values
 */
//------------------------------------------------------------------------------
std::vector<std::vector<std::vector< float> > > getWhiteSpectralImage();


//------------------------------------------------------------------------------
/// Get a set of flat field images that can be used in the flat-field correction (FFC).
/// When a spectral detector is used, a 2D image is generated for each energy band.
/// Note that if Poisson noise is used, the function will return a different
/// set each time the function is called.
/**
 *  @return the set of 2D images in greyscale values
 */
//------------------------------------------------------------------------------
std::vector<std::vector<std::vector< unsigned short> > > getWhiteSpectralImageWithGain();


//------------------------------------------------------------------------------
/// Save the polygon mesh of a given node.
/**
 *  @param aLabel:          the label of the polygon mesh
 *  @param aFileName:   the name of the output file
 *  (default: means that the filename will be
 *  automatically generated and the file saved in
 *  the current path)
 *  @param aBinaryFlag: if true, save a binary file; if false save an ASCII file
 *                      (default value: true)
 */
//------------------------------------------------------------------------------
void saveSTLfile(const std::string& aLabel,
		const std::string& aFileName = std::string(),
		bool aBinaryFlag = true);


//------------------------------------------------------------------------------
/// Accessor on the max radius of the L-buffer artefact correction filter
/**
 *  @return the radius in number of pixels
 */
//------------------------------------------------------------------------------
unsigned int getMaxRadiusLbufferArtefactFilter();


//------------------------------------------------------------------------------
/// Set the max radius of the L-buffer artefact correction filter
/**
 *  @param aRadius: the radius in number of pixels
 */
//------------------------------------------------------------------------------
void setMaxRadiusLbufferArtefactFilter(unsigned int aRadius);


//------------------------------------------------------------------------------
/// Check whether the GPU implementation of the L-buffer artefact correction filter
/// is enabled
/**
 *  @return true if the filter is active, otherwise return false
 */
//------------------------------------------------------------------------------
bool isArtefactFilteringOnGPUEnabled();


//------------------------------------------------------------------------------
/// Check whether the CPU implementation of the L-buffer artefact correction filter
/// is enabled
/**
 *  @return true if the filter is active, otherwise return false
 */
//------------------------------------------------------------------------------
bool isArtefactFilteringOnCPUEnabled();







//------------------------------------------------------------------------------
/// Accessor on the width of a given image.
/**
 *  @param anImage: the image
 *  @return the width of the image (in number of pixels)
 */
//------------------------------------------------------------------------------
int getImageWidth(const std::vector<std::vector<float> >& anImage);


//------------------------------------------------------------------------------
/// Accessor on the height of a given image.
/**
 *  @param anImage: the image
 *  @return the height of the image (in number of pixels)
 */
//------------------------------------------------------------------------------
int getImageHeght(const std::vector<std::vector<float> >& anImage);


//------------------------------------------------------------------------------
/// Accessor on the j-th row of pixels of a given image.
/**
 *  @param anImage: the image
 *  @param j:       the row number
 *  @return the j-th row of pixels
 */
//------------------------------------------------------------------------------
std::vector<float> getImageRow(const std::vector<std::vector<float> >& anImage,
                               int j);


//------------------------------------------------------------------------------
/// Accessor on the chemical element's name depending on its Z number.
/**
 *  @param Z:   the atomic number of the element
 *  @return the name of the corresponding element
 */
//------------------------------------------------------------------------------
std::string getElementName(unsigned short Z);


//------------------------------------------------------------------------------
/// Accessor on the chemical element's name depending on its symbol.
/**
 *  @param aSymbol:   the symbol of the element
 *  @return the name of the corresponding element
 */
//------------------------------------------------------------------------------
std::string getElementName(const std::string& aSymbol);


//------------------------------------------------------------------------------
/// Accessor on the chemical element' symbol depending on its Z number.
/**
 *  @param Z:   the atomic number of the element
 *  @return the name of the corresponding element
 */
//------------------------------------------------------------------------------
std::string getElementSymbol(unsigned short Z);


//------------------------------------------------------------------------------
/// Accessor on the chemical element's Z number.
/**
 *  @param anElement:   the name or symbol of the element
 *  @return the Z number of the corresponding element
 */
//------------------------------------------------------------------------------
unsigned short getElementAtomicNumber(const std::string& anElement);


//------------------------------------------------------------------------------
/// Accessor on the chemical element's molar mass.
/**
 *  @param anElement:   the name or symbol of the element
 *  @return the molar mass of the corresponding element
 */
//------------------------------------------------------------------------------
float getElementMolarMass(const std::string& anElement);


//------------------------------------------------------------------------------
/// Accessor on the chemical element's density.
/**
 *  @param anElement:   the name or symbol of the element
 *  @return the density of the corresponding element (in g / cm3)
 */
//------------------------------------------------------------------------------
double getDensityFromElement(const std::string& anElement);


//------------------------------------------------------------------------------
/// Accessor on the chemical element's density.
/**
 *  @param Z:   the Z number of the element
 *  @return the density of the corresponding element (in g / cm3)
 */
//------------------------------------------------------------------------------
double getDensityFromElement(unsigned short Z);


//------------------------------------------------------------------------------
/// Accessor on the HU's density.
/**
 *  @param HU:   the HU
 *  @return the density of the corresponding HU (in g / cm3)
 */
//------------------------------------------------------------------------------
double getDensityFromHU(double HU);


//------------------------------------------------------------------------------
/// Accessor on the chemical element's mass attenuation.
/**
 *  @param anElement:   the name or symbol of the element
 *  @param anEnergy:        the photon energy of interest
 *  @param aUnitOfEnergy:   the unit of energy corresponding to anEnergy.
 *                          Acceptable values are:
 *                          "electronvolt", "eV",
 *                          "kiloelectronvolt", "keV",
 *                          "megaelectronvolt", "MeV"
 *  @return the mass attenuation of the corresponding element (in cm2/g)
 */
//------------------------------------------------------------------------------
double getMassAttenuationFromElement(const std::string& anElement,
		double anEnergy,
		const std::string& aUnitOfEnergy);


//------------------------------------------------------------------------------
/// Accessor on the chemical element's mass attenuation.
/**
 *  @param Z:   the Z number of the element
 *  @param anEnergy:        the photon energy of interest
 *  @param aUnitOfEnergy:   the unit of energy corresponding to anEnergy.
 *                          Acceptable values are:
 *                          "electronvolt", "eV",
 *                          "kiloelectronvolt", "keV",
 *                          "megaelectronvolt", "MeV"
 *  @return the mass attenuation of the corresponding element (in cm2/g)
 */
//------------------------------------------------------------------------------
double getMassAttenuationFromElement(unsigned short Z,
		double anEnergy,
		const std::string& aUnitOfEnergy);


/// Accessor on the HU's mass attenuation.
/**
 *  @param HU:   the HU
 *  @param anEnergy:        the photon energy of interest
 *  @param aUnitOfEnergy:   the unit of energy corresponding to anEnergy.
 *                          Acceptable values are:
 *                          "electronvolt", "eV",
 *                          "kiloelectronvolt", "keV",
 *                          "megaelectronvolt", "MeV"
 *  @return the density of the corresponding HU (in cm2/g)
 */
//------------------------------------------------------------------------------
double getMassAttenuationFromHU(double HU,
		double anEnergy,
		const std::string& aUnitOfEnergy);


//------------------------------------------------------------------------------
/// Accessor on the mixture's mass attenuation.
/**
 *  @param aMixture:    the details about the mixture. It is given as a sequence
 *                      of element symbol & relative weight, e.g.
 *                      Ti90Al6V4.
 *  @param anEnergy:        the photon energy of interest
 *  @param aUnitOfEnergy:   the unit of energy corresponding to anEnergy.
 *                          Acceptable values are:
 *                          "electronvolt", "eV",
 *                          "kiloelectronvolt", "keV",
 *                          "megaelectronvolt", "MeV"
 *  @return the mass attenuation of the corresponding mixture (in cm2/g)
 */
//------------------------------------------------------------------------------
double getMassAttenuationFromMixture(const std::string& aMixture,
		double anEnergy,
		const std::string& aUnitOfEnergy);


//------------------------------------------------------------------------------
/// Accessor on the compound's mass attenuation.
/**
 *  @param aCompound:   the details about the compound. It is given as
 *                      a sequence of element symbol & number of atoms, e.g.
 *                      H2O for water and SiC for silicon carbide.
 *  @param anEnergy:        the photon energy of interest
 *  @param aUnitOfEnergy:   the unit of energy corresponding to anEnergy.
 *                          Acceptable values are:
 *                          "electronvolt", "eV",
 *                          "kiloelectronvolt", "keV",
 *                          "megaelectronvolt", "MeV"
 *  @return the mass attenuation of the corresponding compound (in cm2/g)
 */
//------------------------------------------------------------------------------
double getMassAttenuationFromCompound(const std::string& aCompound,
		double anEnergy,
		const std::string& aUnitOfEnergy);


//------------------------------------------------------------------------------
/// Accessor on the HU's linear attenuation coefficient.
/**
 *  @param HU:   the HU
 *  @param anEnergy:        the photon energy of interest
 *  @param aUnitOfEnergy:   the unit of energy corresponding to anEnergy.
 *                          Acceptable values are:
 *                          "electronvolt", "eV",
 *                          "kiloelectronvolt", "keV",
 *                          "megaelectronvolt", "MeV"
 *  @return the linear attenuation coefficient (in cm-1)
 */
//------------------------------------------------------------------------------
double getMuFromHU(double HU,
		double anEnergy,
		const std::string& aUnitOfEnergy);


//------------------------------------------------------------------------------
/// Convert a 3D volume in HU values into linear attenuation coefficients.
/**
 *  @param anImage:   the 3D volume in HU values
 *  @param anEnergy:        the photon energy of interest
 *  @param aUnitOfEnergy:   the unit of energy corresponding to anEnergy.
 *                          Acceptable values are:
 *                          "electronvolt", "eV",
 *                          "kiloelectronvolt", "keV",
 *                          "megaelectronvolt", "MeV"
 *  @return the 3D volume in linear attenuation coefficients (cm^-1)
 */
//------------------------------------------------------------------------------
std::vector<std::vector<std::vector< float> > > convertHU2mu(const std::vector<std::vector<std::vector< float> > >& anImage,
    double anEnergy,
    const std::string& aUnitOfEnergy);

/**
void setShiftFilter(double aValue);
void setScaleFilter(double aValue);
double getShiftFilter();
double getScaleFilter();
*/

std::vector<float> rayIntersect(const std::string& aLabel,
    double aRayOriginX, double aRayOriginY, double aRayOriginZ,
    double aRayDirectionX, double aRayDirectionY, double aRayDirectionZ);




//------------------------------------------------------------------------------
/// Compute the arithmetic mean of a 1D array.
/**
 *  @param anImage:   the 1D array
 *  @return the arithmetic mean
 */
//------------------------------------------------------------------------------
double getMean(const std::vector<float>& anImage);


//------------------------------------------------------------------------------
/// Compute the standard deviation of a 1D array.
/**
 *  @param anImage:   the 1D array
 *  @return the standard deviation
 */
//------------------------------------------------------------------------------
double getStddev(const std::vector<float>& anImage);



//------------------------------------------------------------------------------
/// Compute the zero-normalized cross-correlation (ZNCC) (also known as 
/// zero-mean unit-variance cross-correlation) between two 1D arrays `aReferenceSet` 
/// \f$(\theta)\f$ and `aTestImage` \f$\left(\hat{\theta}\right)\f$.
/// The ZNCC has a range of [-1, 1]. 
/// When the ZNCC is equal to -1, then \f$\theta\f$ and \f$\hat{\theta}\f$ are anticorrelated; 
/// when the ZNCC is equal to +1, then \f$\theta\f$ and \f$\hat{\theta}\f$ are fully correlated; and 
/// when the ZNCC is equal to 0, then \f$\theta\f$ and \f$\hat{\theta}\f$ are not correlated at all.
/**
 *  @param aReferenceSet:   the 1st 1D array
 *  @param aTestSet:   the 2nd 1D array
 *  @return the ZNCC between aReferenceSet and aTestSet
 */
//------------------------------------------------------------------------------
double computeZNCC(const std::vector<float>& aReferenceSet,
    const std::vector<float>& aTestSet);


//------------------------------------------------------------------------------
/// Compute the root mean squared error (RMSE) (also known as 
/// root mean squared distance, RMSD) between two 1D arrays `aReferenceSet` 
/// \f$(\theta)\f$ and `aTestImage` \f$\left(\hat{\theta}\right)\f$. When RMSE
/// is close to zero, then \f$\theta\f$ and \f$\hat{\theta}\f$ 
/// are similar.
/**
 \f{eqnarray*}{
    MSE(\theta, \hat{\theta}) &=& \frac{\sum_{i = 0}^{I-1}\left(\theta(i) - \hat{\theta}(i)\right)^2}{I}\\
    RMSE(\theta, \hat\theta)  &=& \sqrt{MSE\left(\theta, \hat\theta\right)}
 \f}
*/
/**
 *  @param aReferenceSet:   the 1st 1D array
 *  @param aTestSet:    the 2nd 1D array
 *  @param normalise:   if the flag is true, the data will be normalised between 0 and 1. If it is false, the data is not normalised. (default value: false)
 *  @return the RMSE between aReferenceSet and aTestSet
 */
//------------------------------------------------------------------------------
double computeRMSE(const std::vector<float>& aReferenceSet,
    const std::vector<float>& aTestSet,
    bool normalise = false);


//------------------------------------------------------------------------------
/// Load the reference data \f$(\theta)\f$ as a 1D array. It can be used with
/// `computeZNCC` and `computeRMSE`.
/**
 *  @param aReferenceSet:   the 1st 1D array
 */
//------------------------------------------------------------------------------
void loadReference(const std::vector<float>& aReferenceSet);


//------------------------------------------------------------------------------
/// Load the test data \f$\left(\hat{\theta}\right)\f$ as a 1D array. It can be 
/// used with `computeZNCC` and `computeRMSE`.
/**
 *  @param aTestSet:   the 2nd 1D array
 */
//------------------------------------------------------------------------------
void loadTest(const std::vector<float>& aTestSet);


//------------------------------------------------------------------------------
/// Linearise the test data \f$\left(\hat{\theta}\right)\f$ as follows:
/**
 \f{eqnarray*}{
    \hat{\theta}(i) = -\log(aScalingFactor \times \max(aThreshold, \hat{\theta}(i)))
 \f}
*/
/**
 *  @param aThreshold:   the 2nd 1D array
 *  @param aScalingFactor:   the 2nd 1D array
 */
//------------------------------------------------------------------------------
void lineariseTest(float aThreshold, float aScalingFactor);


//------------------------------------------------------------------------------
/// Compute the zero-normalized cross-correlation (ZNCC) (also known as 
/// zero-mean unit-variance cross-correlation) between two 1D arrays (\f$(\theta)\f$ and 
/// \f$\left(\hat{\theta}\right)\f$) that were previously loaded using `loadReference` and `loadTest`. 
/// The ZNCC has a range of [-1, 1]. 
/// When the ZNCC is equal to -1, then \f$\theta\f$ and \f$\hat{\theta}\f$ are anticorrelated; 
/// when the ZNCC is equal to +1, then \f$\theta\f$ and \f$\hat{\theta}\f$ are fully correlated; and 
/// when the ZNCC is equal to 0, then \f$\theta\f$ and \f$\hat{\theta}\f$ are not correlated at all.
/**
 *  @return the ZNCC between \f$(\theta)\f$ and \f$\left(\hat{\theta}\right)\f$
 */
//------------------------------------------------------------------------------
double computeZNCC();


//------------------------------------------------------------------------------
/// Compute the root mean squared error (RMSE) (also known as 
/// root mean squared distance, RMSD) between two 1D arrays (\f$(\theta)\f$ and 
/// \f$\left(\hat{\theta}\right)\f$) that were previously loaded using `loadReference` and `loadTest`. 
/// When RMSE is close to zero, then \f$\theta\f$ and \f$\hat{\theta}\f$ 
/// are similar.
/**
 \f{eqnarray*}{
    MSE(\theta, \hat{\theta}) &=& \frac{\sum_{i = 0}^{I-1}\left(\theta(i) - \hat{\theta}(i)\right)^2}{I}\\
    RMSE(\theta, \hat\theta)  &=& \sqrt{MSE\left(\theta, \hat\theta\right)}
 \f}
*/
/**
 *  @param normalise:   if the flag is true, the data will be normalised between 0 and 1. If it is false, the data is not normalised. (default value: false)
 *  @return the RMSE between \f$(\theta)\f$ and \f$\left(\hat{\theta}\right)\f$
 */
//------------------------------------------------------------------------------
double computeRMSE(bool normalise = false);


//------------------------------------------------------------------------------
/// Load a 2D image from a file.
/**
 *  @param aFileName:   the file name of the 2D image
 *  @return the corresponding 2D image
 */
//------------------------------------------------------------------------------
std::vector<std::vector<float> > loadImage2D(const std::string& aFileName);


//------------------------------------------------------------------------------
/// Load a 3D volume from a file.
/**
 *  @param aFileName:   the file name of the 3D volume
 *  @return the corresponding 3D volume
 */
//------------------------------------------------------------------------------
std::vector<std::vector<std::vector<float> > > loadImage3D(const std::string& aFileName);


//------------------------------------------------------------------------------
/// Find the smallest voxel value in a 3D volume.
/**
 *  @param a3DVolume:   the 3D volume
 *  @return the smallest voxel value
 */
//------------------------------------------------------------------------------
float getMinValue(const std::vector<std::vector<std::vector<float> > >& a3DVolume);


//------------------------------------------------------------------------------
/// Find the largest voxel value in a 3D volume.
/**
 *  @param a3DVolume:   the 3D volume
 *  @return the largest voxel value
 */
//------------------------------------------------------------------------------
float getMaxValue(const std::vector<std::vector<std::vector<float> > >& a3DVolume);


//------------------------------------------------------------------------------
/// Compute the zero-normalized cross-correlation (ZNCC) (also known as 
/// zero-mean unit-variance cross-correlation) between two 3D volumes.
/// The ZNCC has a range of [-1, 1]. When the ZNCC is equal to -1, then
/// the two volume are anticorrelated; when the ZNCC is equal to +1, then
/// the two volume are fully correlated; and when the ZNCC is equal to 0, then
/// the two volume are not correlated at all.
/**
 *  @param aReferenceSet:   the 1st 3D volume
 *  @param aTestSet:   the 2nd 3D volume
 *  @return the ZNCC between aReferenceSet and aTestSet
 */
//------------------------------------------------------------------------------
double computeZNCC(const std::vector<std::vector<std::vector<float> > >& aReferenceSet,
    const std::vector<std::vector<std::vector<float> > >& aTestSet);


//------------------------------------------------------------------------------
/// Compute the root mean squared error (RMSE) (also known as 
/// root mean squared distance or root mean square deviation, RMSD) between two 
/// 3D volumes `aReferenceSet` \f$(\theta)\f$ and `aTestSet` 
/// \f$\left(\hat{\theta}\right)\f$. When RMSE is close to zero, then 
/// \f$\theta\f$ and \f$\hat{\theta}\f$ are similar.
/**
 \f{eqnarray*}{
    MSE(\theta, \hat{\theta}) &=& \frac{\sum_{i = 0}^{I-1}\sum_{j = 0}^{J-1}\sum_{k = 0}^{K-1}\left(\theta(i,j,k) - \hat{\theta}(i,j,k)\right)^2}{I \times J \times K}\\
    RMSE(\theta, \hat\theta)  &=& \sqrt{MSE\left(\theta, \hat\theta\right)}
 \f}
*/
/**
 *  @param aReferenceSet:   the 1st 3D volume
 *  @param aTestSet:   the 2nd 3D volume
 *  @return the RMSE between aReferenceSet and aTestSet
 */
//------------------------------------------------------------------------------
double computeRMSE(const std::vector<std::vector<std::vector<float> > >& aReferenceSet,
    const std::vector<std::vector<std::vector<float> > >& aTestSet);


#endif
