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


#ifndef SGVXR_Utils_h
#define SGVXR_Utils_h


/**
********************************************************************************
*
*   @file       Utils.h
*
*   @brief      Utility functions that do not belong anywhere in particular.
*
*   @version    1.0
*
*   @date       08/08/2024
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
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, Aug 2024-present, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <string>
#include <vector>


//------------------------------------------------------------------------------
/// Convert an angle given in radians into degrees.
/**
 *  @param anAngle:   the angle in radians
 *  @return the corresponding angle in degrees
 */
//------------------------------------------------------------------------------
float radian2degree(float anAngle);


//------------------------------------------------------------------------------
/// Accessor on the numerical value corresponding to a unit of energy.
/**
 *  @param aUnitOfEnergy:   the unit of energy.
 *                          Acceptable values are:
 *                          "electronvolt", "eV",
 *                          "kiloelectronvolt", "keV",
 *                          "megaelectronvolt", "MeV"
 *  @return the corresponding numerical value
 */
//------------------------------------------------------------------------------
double getUnitOfEnergy(const std::string& aUnitOfEnergy);


//------------------------------------------------------------------------------
/// Accessor on the numerical value corresponding to a unit of length.
/**
 *  @param aUnitOfLength:   the unit of length. Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 *  @return the corresponding numerical value
 */
//------------------------------------------------------------------------------
double getUnitOfLength(const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Accessor on the numerical value corresponding to a unit of electric potential.
/**
 *  @param aUnitOfElectriPotential:   the unit of electric potential.
 *                                    Acceptable values are:
 *                                    "V", "volt",
 *                                    "kV", "kilovolt",
 *                                    "MV", "megavolt"
 *  @return the corresponding numerical value
 */
//------------------------------------------------------------------------------
double getUnitOfElectriPotential(const std::string& aUnitOfElectriPotential);


//------------------------------------------------------------------------------
/// Enable artefact filtering on GPU (it is fast but can be inaccurate).
//------------------------------------------------------------------------------
void enableArtefactFilteringOnGPU(void);


//------------------------------------------------------------------------------
/// Enable artefact filtering on GPU (it is fast but can be inaccurate).
//------------------------------------------------------------------------------
void enableArtifactFilteringOnGPU(void);


//------------------------------------------------------------------------------
/// Enable artefact filtering on CPU (can be extremely slow as it makes use
/// of ray-tracing).
//------------------------------------------------------------------------------
void enableArtefactFilteringOnCPU(void);


//------------------------------------------------------------------------------
/// Enable artefact filtering on CPU (can be extremely slow as it makes use
/// of ray-tracing).
//------------------------------------------------------------------------------
void enableArtifactFilteringOnCPU(void);


//------------------------------------------------------------------------------
/// Disable artefact filtering altogether.
//------------------------------------------------------------------------------
void disableArtefactFiltering(void);


//------------------------------------------------------------------------------
/// Disable artefact filtering altogether.
//------------------------------------------------------------------------------
void disableArtifactFiltering(void);


//------------------------------------------------------------------------------
/// Compute the overall mixed precision flag from individual flags
/*
*  @param aUncleanedLBufferFlag:	true for FP32 for uncleaned L-buffer texture, false for FP16
*  @param aCleanedLBufferFlag:	true for FP32 for cleaned L-buffer texture, false for FP16
*  @param aCumulatedLBufferFlag:	true for FP32 for cumulative L-buffer texture, false for FP16
*  @param anEnergyFluenceFlag:	true for FP32 for energy fluence texture, false for FP16
*  @param aSumMuXdXFlag:	true for FP32 for sum muX dX texture, false for FP16
*  @param aZBufferFlag:	true for FP32 for Z-buffer texture, false for FP16
*  @return on the corresponding mixed precision flag
*/
//------------------------------------------------------------------------------
uint8_t computeMixedPrecisionFlag(bool aUncleanedLBufferFlag,
	bool aCleanedLBufferFlag,
	bool aCumulatedLBufferFlag,
	bool anEnergyFluenceFlag,
	bool aSumMuXdXFlag,
	bool aZBufferFlag);


//------------------------------------------------------------------------------
/// Accessor on the current mixed precision flag
/*
*  @return on the current mixed precision flag
*/
//------------------------------------------------------------------------------
uint8_t getMixedPrecisionFlag();


//------------------------------------------------------------------------------
/// Accessor on the current mixed precision flag for uncleaned L-buffer texture
/*
*  @return true for FP32, or false for FP16
*/
//------------------------------------------------------------------------------
bool getPrecisionUncleanedLBuffer();


//------------------------------------------------------------------------------
/// Accessor on the current mixed precision flag for cleaned L-buffer texture
/*
*  @return true for FP32, or false for FP16
*/
//------------------------------------------------------------------------------
bool getPrecisionCleanedLBuffer();


//------------------------------------------------------------------------------
/// Accessor on the current mixed precision flag for cumulated L-buffer texture
/*
*  @return true for FP32, or false for FP16
*/
//------------------------------------------------------------------------------
bool getPrecisionCumulatedLBuffer();


//------------------------------------------------------------------------------
/// Accessor on the current mixed precision flag for energy fluence texture
/*
*  @return true for FP32, or false for FP16
*/
//------------------------------------------------------------------------------
bool getPrecisionEnergyFluence();


//------------------------------------------------------------------------------
/// Accessor on the current mixed precision flag for sum muX dX texture
/*
*  @return true for FP32, or false for FP16
*/
//------------------------------------------------------------------------------
bool getPrecisionSumMuXdX();


//------------------------------------------------------------------------------
/// Accessor on the current mixed precision flag for uncleaned Z-buffer texture
/*
*  @return true for FP32, or false for UINT16
*/
//------------------------------------------------------------------------------
bool getPrecisionZBuffer();


//------------------------------------------------------------------------------
/// Accessor on the number of EGL devices
/*
*  @return on the number of EGL devices
*/
//------------------------------------------------------------------------------
unsigned int getNumberOfEGLDevices();


//------------------------------------------------------------------------------
/// Accessor on the list of supported EGL clients
/*
*  @return on the list of supported EGL clients
*/
//------------------------------------------------------------------------------
std::vector<std::string> findEGLClients();


//------------------------------------------------------------------------------
/// Create an OpenGL context (the window won't be shown).
/**
 *  @param aWindowID:   the numerical ID of the context to create
 *                      (default value: -1, means that the ID will be
 *                      automatically generated)
 *  @param aRendererMajorVersion: Select the major version of the renderer.
 *                    (default value: 4)
 *  @param aRendererMinorVersion: Select the minor version of the renderer.
 *                    (default value: 3)
*  @param aMixedPrecisionFlag: Set of mixed-precision flag to select the number
 *                              of bits per floating-point numbers in GPU memory
 *                              for various textures. See computeMixedPrecisionFlag
 *                              for help about how to set the flag.
 *                              (default value: 255, i.e. FP32 for all textures)
 */
//------------------------------------------------------------------------------
void createOpenGLContext(int aWindowID = -1,
		int aRendererMajorVersion = 4,
		int aRendererMinorVersion = 3,
	    unsigned int aMixedPrecisionFlag = 255);


//------------------------------------------------------------------------------
/// Create an OpenGL context and display it in a window
/**
 *  @param aWindowID:   the numerical ID of the context to create
 *                      (default value: -1, means that the ID will be
 *                      automatically generated)
 *  @param aVisibilityFlag: flag controling if the window should be visible (1)
 *                          or hidden (0). (default value: 0)
 *  @param aRenderer: Select the renderer to use, e.g. OpenGL or Vulkan.
 *                    (default value: OPENGL)
 *  @param aRendererMajorVersion: Select the major version of the renderer.
 *                    (default value: 4)
 *  @param aRendererMinorVersion: Select the minor version of the renderer.
 *                    (default value: 3)
 *  @param aMixedPrecisionFlag: Set of mixed-precision flag to select the number
 *                              of bits per floating-point numbers in GPU memory
 *                              for various textures. See computeMixedPrecisionFlag
 *                              for help about how to set the flag.
 *                              (default value: 255, i.e. FP32 for all textures)
 */
//------------------------------------------------------------------------------
#ifndef __APPLE__
void createWindow(int aWindowID = -1,
		int aVisibilityFlag = 0,
		const std::string& aRenderer = "OPENGL",
		int aRendererMajorVersion = 4,
		int aRendererMinorVersion = 3,
	    unsigned int aMixedPrecisionFlag = 255);
#else // __APPLE__
void createWindow(int aWindowID = -1,
		int aVisibilityFlag = 0,
		const std::string& aRenderer = "OPENGL",
		int aRendererMajorVersion = 2,
		int aRendererMinorVersion = 1,
	    unsigned int aMixedPrecisionFlag = 255);
#endif // __APPLE__


//------------------------------------------------------------------------------
/// Create an OpenGL context
/**
 *  @param aVisibilityFlag: flag controlling if the window should be visible (1)
 *                          or hidden (0). (default value: 0)
 *  @param aRenderer: Select the renderer to use, e.g. OpenGL or Vulkan.
 *                    (default value: OPENGL)
 *  @param aWindowID:   the numerical ID of the context to create
 *                      (default value: -1, means that the ID will be
 *                      automatically generated)
 *  @param aRendererMajorVersion: Select the major version of the renderer.
 *                    (default value: 4)
 *  @param aRendererMinorVersion: Select the minor version of the renderer.
 *                    (default value: 3)
 *  @param aMixedPrecisionFlag: Set of mixed-precision flag to select the number
 *                              of bits per floating-point numbers in GPU memory
 *                              for various textures. See computeMixedPrecisionFlag
 *                              for help about how to set the flag.
 *                              (default value: 255, i.e. FP32 for all textures)
 */
//------------------------------------------------------------------------------
void createNewContext(const std::string& aRenderer = "OPENGL",
    int aWindowID = -1,
		int aVisibilityFlag = 0,
		int aRendererMajorVersion = 4,
		int aRendererMinorVersion = 3,
	    unsigned int aMixedPrecisionFlag = 255);


//------------------------------------------------------------------------------
/// Set the size of the given window.
/**
 *  @param aWidth:      the number of pixels along the X-axis
 *  @param aHeight:     the number of pixels along the Y-axis
 *  @param aWindowID:   the numerical ID of the corresponding context
 *                      (default value: -1, means that the active context
 *                      will be used)
 */
//------------------------------------------------------------------------------
void setWindowSize(int aWidth, int aHeight, int aWindowID = -1);


//------------------------------------------------------------------------------
/// Accessor on the size of a given window.
/**
 *  @return the size of the window
 */
//------------------------------------------------------------------------------
std::vector<int> getWindowSize(int aWindowID = -1);


//------------------------------------------------------------------------------
/// Close and destroy a given window or OpenGL context.
//------------------------------------------------------------------------------
void destroyWindow(int aWindowID = -1);


//------------------------------------------------------------------------------
/// Close and destroy all the windows and OpenGL contexts that have been created.
/// Same as void terminate(void);
//------------------------------------------------------------------------------
void destroyAllWindows(void);


//------------------------------------------------------------------------------
/// Close and destroy all the windows and OpenGL contexts that have been created.
/// It also releases all the graphics memory.
//------------------------------------------------------------------------------
void destroy(void);

#ifndef _WIN32
//------------------------------------------------------------------------------
/// Close and destroy all the windows and OpenGL contexts that have been created.
/// It also releases all the graphics memory. Note: it'll be deprecated in 
/// a future version of gVXR. Use void destroy(void) instead.
//------------------------------------------------------------------------------
void terminate(void);
#endif


//------------------------------------------------------------------------------
/// Access the largest possible 2D texture size on the current environment.
/*
* @return the largest possible 2D texture size
*/
//------------------------------------------------------------------------------
std::vector<unsigned int> getMax2DTextureSize();


//------------------------------------------------------------------------------
/// Access the largest possible 3D texture size on the current environment.
/*
* @return the largest possible 3D texture size
*/
//------------------------------------------------------------------------------
std::vector<unsigned int> getMax3DTextureSize();


#endif
