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
*   @file       detector.cxx
*
*   @brief      Expose here all the functions of SimpleGVXR related to 
*               the X-ray detector. It includes it geometry and resolution as 
*               well as the scintilator.
*
*   @version    1.0
*
*   @date       05/06/2024
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
#ifndef GVXR_TYPES_H
#include "gVirtualXRay/Types.h"
#endif

#ifndef GVXR_Detector_h
#include "gVirtualXRay/XRayDetector.h"
#endif

#ifndef GVXR_OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef SGVXR_Utils_h
#include "Utils.h"
#endif

#ifndef SGVXR_Internal_h
#include "internal.h"
#endif

#ifndef SGVXR_Detector_h
#include "detector.h"
#endif


//******************************************************************************
//  Namespace
//******************************************************************************


//******************************************************************************
//  Global variables
//******************************************************************************
gVirtualXRay::XRayDetector g_xray_detector; // X-ray detector
extern bool g_renderer_initialised;
extern int g_samples_per_pixels;
extern uint8_t g_mixed_precision_flag;

bool g_detector_position_initialised = false;
bool g_detector_up_vector_initialised = false;
bool g_detector_size_initialised = false;
bool g_detector_pixel_size_initialised = false;


//******************************************************************************
//  Function definitions
//******************************************************************************


//-----------------------------------------------------
void setDetectorNumberOfPixels(int aWidth, int aHeight)
//-----------------------------------------------------
{
    // Get the current number of pixels
	std::vector<int> old_resolution = getDetectorNumberOfPixels();

	// It has changed
	if (old_resolution[0] != aWidth || old_resolution[1] != aHeight)
	{
		g_xray_detector.setNumberOfPixels(gVirtualXRay::Vec2ui(aWidth, aHeight));
	    g_renderer_initialised = false;
	    gVirtualXRay::LOGGER.logNow("Changed resolution") << std::endl;
	}
    g_detector_size_initialised = true;

    // Make sure the detector is initialised if needed
    if (!g_renderer_initialised) {
        // Initialise the X-ray renderer if needed and if possible
        initialiseXRayRenderer(g_mixed_precision_flag);
    }
}


//------------------------------------------
std::vector<int> getDetectorNumberOfPixels()
//------------------------------------------
{
    std::vector<int> number_of_pixels(2);

    number_of_pixels[0] = static_cast<int>(g_xray_detector.getNumberOfPixels()[0]);
    number_of_pixels[1] = static_cast<int>(g_xray_detector.getNumberOfPixels()[1]);

    return (number_of_pixels);
}


//-------------------------------------
void setOversamplingFactor(int aFactor)
//-------------------------------------
{
	g_samples_per_pixels = std::max(1, aFactor);
}


//-------------------------
int getOversamplingFactor()
//-------------------------
{
	return g_samples_per_pixels;
}


//---------------------------------------------------------
void setDetectorPixelSize(double aWidth,
                          double aHeight,
                          const std::string& aUnitOfLength)
//---------------------------------------------------------
{
    double unit_of_length = getUnitOfLength(aUnitOfLength);

    g_xray_detector.setResolutionInUnitsOfLengthPerPixel(
        gVirtualXRay::VEC2(aWidth * unit_of_length,
                           aHeight * unit_of_length));

    g_detector_pixel_size_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer(g_mixed_precision_flag);
}


//----------------------------------------------------------
void setDetectorPixelPitch(double aWidth,
                           double aHeight,
                           const std::string& aUnitOfLength)
//----------------------------------------------------------
{
    setDetectorPixelSize(aWidth, aHeight, aUnitOfLength);
}


//---------------------------------------------------------------------------
std::vector<double> getDetectorPixelSpacing(const std::string& aUnitOfLength)
//---------------------------------------------------------------------------
{
    double unit_of_length = getUnitOfLength(aUnitOfLength);

    std::vector<double> spacing(2);

    spacing[0] =
        g_xray_detector.getPixelSpacingInUnitOfLength()[0] / unit_of_length;

    spacing[1] =
        g_xray_detector.getPixelSpacingInUnitOfLength()[1] / unit_of_length;

    return (spacing);
}


//---------------------------------------------------------------------------------
void setDetectorSize(float aWidth, float aHeight, const std::string& aUnitOfLength)
//---------------------------------------------------------------------------------
{
    std::vector<int> resolution = getDetectorNumberOfPixels();

    if (resolution[0] == 0 || resolution[1] == 0) {
        throw gVirtualXRay::Exception(__FILE__,
            __FUNCTION__,
            __LINE__,
            "You must set the number of pixels of the detector before calling this function.");
    }
    setDetectorPixelSize(aWidth / resolution[0], aHeight / resolution[1], aUnitOfLength);
}


//-------------------------------------------------------------------
std::vector<double> getDetectorSize(const std::string& aUnitOfLength)
//-------------------------------------------------------------------
{
    double unit_of_length = getUnitOfLength(aUnitOfLength);

    std::vector<double> detector_size(2);

    detector_size[0] =
        g_xray_detector.getSizeInUnitOfLength()[0] / unit_of_length;

    detector_size[1] =
        g_xray_detector.getSizeInUnitOfLength()[1] / unit_of_length;

    return (detector_size);
}


//-----------------------------------------------------------
double getDetectorPixelArea(const std::string& aUnitOfLength)
//-----------------------------------------------------------
{
    std::vector<double> detector_size = getDetectorSize(aUnitOfLength);
    double pixel_width = detector_size[0] / g_xray_detector.getNumberOfPixels()[0];
    double pixel_height = detector_size[1] / g_xray_detector.getNumberOfPixels()[1];

    return pixel_width * pixel_height;
}


//--------------------------------------------------------
void setDetectorPosition(double x,
                         double y,
                         double z,
                         const std::string& aUnitOfLength)
//--------------------------------------------------------
{
    double unit_of_length = getUnitOfLength(aUnitOfLength);

    // Update the position
    g_xray_detector.setDetectorPosition(
            gVirtualXRay::VEC3(x * unit_of_length, y * unit_of_length,
                    z * unit_of_length));

    // The position has been initialised
    g_detector_position_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer(g_mixed_precision_flag);
}


//-----------------------------------------------------------------------
std::vector<double> getDetectorPosition(const std::string& aUnitOfLength)
//-----------------------------------------------------------------------
{
    double unit_of_length = getUnitOfLength(aUnitOfLength);

    std::vector<double> position(3);

    position[0] = g_xray_detector.getDetectorPosition()[0] / unit_of_length;
    position[1] = g_xray_detector.getDetectorPosition()[1] / unit_of_length;
    position[2] = g_xray_detector.getDetectorPosition()[2] / unit_of_length;

    return (position);
}


//----------------------------------------------------
void setDetectorUpVector(double x, double y, double z)
//----------------------------------------------------
{
    // Update the up vector
    g_xray_detector.setUpVector(gVirtualXRay::VEC3(x, y, z).normal());

    // The up vector has been initialised
    g_detector_up_vector_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer(g_mixed_precision_flag);
}


//---------------------------------------
std::vector<double> getDetectorUpVector()
//---------------------------------------
{
    std::vector<double> up_vector(3);

    up_vector[0] = g_xray_detector.getUpVector()[0];
    up_vector[1] = g_xray_detector.getUpVector()[1];
    up_vector[2] = g_xray_detector.getUpVector()[2];

    return (up_vector);
}


//-------------------------------------------------------
void setDetectorRightVector(double x, double y, double z)
//-------------------------------------------------------
{
    // Update the right vector
    g_xray_detector.setRightVector(gVirtualXRay::VEC3(x, y, z).normal());

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer(g_mixed_precision_flag);
}


//------------------------------------------
std::vector<double> getDetectorRightVector()
//------------------------------------------
{
    std::vector<double> right_vector(3);

    right_vector[0] = g_xray_detector.getRightVector()[0];
    right_vector[1] = g_xray_detector.getRightVector()[1];
    right_vector[2] = g_xray_detector.getRightVector()[2];

    return (right_vector);
}


//---------------------------------------------------------------
void rotateDetector(double anAngle, double x, double y, double z)
//---------------------------------------------------------------
{
    g_xray_detector.rotateDetector(anAngle, gVirtualXRay::VEC3(x, y, z).normal());
}


//--------------------------------
void autoAlignDetector(bool aFlag)
//--------------------------------
{
    g_xray_detector.autoAlignDetector(aFlag);
}


//--------------------------------------
std::vector<float> getDetectorVertices()
//--------------------------------------
{
    return g_xray_detector.getVertices();
}


//-----------------------------------
void setLSF(const std::string& fname)
//-----------------------------------
{
    g_xray_detector.setLSF(fname);
}


//-----------------------------------------
void setLSF(const std::vector<float>& aLSF)
//-----------------------------------------
{
    g_xray_detector.setLSF(aLSF);
}


//--------------------------------
const std::vector<float>& getLSF()
//--------------------------------
{
    return g_xray_detector.getLSF();
}


//-------------
void clearLSF()
//-------------
{
    g_xray_detector.clearLSF();
}


//---------------------------------------------------------------------------------------------
void setDetectorEnergyResponse(const std::vector<std::vector<float> >& anEnergyResponse,
                               const std::string& aUnitOfEnergy)
//---------------------------------------------------------------------------------------------
{
    // Due to SWIG, we use a vector of vector of floats instead of a vector of pairs of floats

    // Create the  vector of pairs of floats and reserve the right amount of memory
    std::vector<std::pair<float, float> > response;
    response.reserve(anEnergyResponse.size());

    float unit = getUnitOfEnergy(aUnitOfEnergy);

    // Convert the vector of vector of floats into a vector of pairs of floats
    for (std::vector<std::vector<float> >::const_iterator ite(anEnergyResponse.begin());
        ite != anEnergyResponse.end();
        ++ite)
    {
        // Double-check the data integrity
        if (ite->size() != 2)
        {

            throw gVirtualXRay::Exception(__FILE__,
                                        __FUNCTION__,
                                        __LINE__,
                                        "The size of a pair must be two.");
        }

        // Add the pair
        response.push_back(std::pair<float, float>(ite->front() * unit, ite->back() * unit));
    }

    // Set the energy response
    g_xray_detector.setEnergyResponse(response);
}


//---------------------------------------------------------------
void loadDetectorEnergyResponse(const std::string& aFileName,
                                const std::string& aUnitOfEnergy)
//---------------------------------------------------------------
{
    g_xray_detector.loadEnergyResponse(aFileName, getUnitOfEnergy(aUnitOfEnergy));
}


//----------------------------------------------------------------------------------
std::vector<std::vector<float> > getEnergyResponse(const std::string& aUnitOfEnergy)
//----------------------------------------------------------------------------------
{
    std::vector<std::vector<float> > temp;

    const std::vector<std::pair<gVirtualXRay::RATIONAL_NUMBER, gVirtualXRay::RATIONAL_NUMBER> >& p_energy_response = g_xray_detector.getEnergyResponse();
    
    float unit = getUnitOfEnergy(aUnitOfEnergy);

    std::vector<float> record(2);
    for (std::vector<std::pair<gVirtualXRay::RATIONAL_NUMBER, gVirtualXRay::RATIONAL_NUMBER> >::const_iterator ite = p_energy_response.begin();
        ite != p_energy_response.end();
        ++ite)
    {
        record[0] = ite->first / unit;
        record[1] = ite->second / unit;
        temp.push_back(record);
    }

    return temp;
}


//--------------------------------
void clearDetectorEnergyResponse()
//--------------------------------
{
    g_xray_detector.clearEnergyResponse();
}


//----------------------------------------------------
void setScintillator(const std::string& aMaterial,
                     const float& aThickness,
                     const std::string& aUnitOfLength)
//----------------------------------------------------
{
    g_xray_detector.clearEnergyResponse();
    g_xray_detector.setScintillator(aMaterial, aThickness * getUnitOfLength(aUnitOfLength));
}


//------------------------------------------
const std::string& getScintillatorMaterial()
//------------------------------------------
{
    return g_xray_detector.getScintillatorMaterial();
}


//----------------------------------------------------------
std::vector<std::string> getSupportedScintillatorMaterials()
//----------------------------------------------------------
{
    return gVirtualXRay::Scintillator::getSupportedScintillatorMaterials();
}


//----------------------------------------------------------------------------------
std::vector<std::vector<float> > getEnergyResponse(const std::string& aMaterial,
                                                   const float& aThickness,
                                                   const std::string& aUnitOfLength,
                                                   const std::string& aUnitOfEnergy)
//----------------------------------------------------------------------------------
{
    std::vector<std::pair<gVirtualXRay::RATIONAL_NUMBER, gVirtualXRay::RATIONAL_NUMBER> > p_input = gVirtualXRay::Scintillator::getEnergyResponse(aMaterial, aThickness * getUnitOfLength(aUnitOfLength));
    std::vector<std::vector<float> > p_output;

    float unit_of_energy = getUnitOfEnergy(aUnitOfEnergy);

    for (std::vector<std::pair<gVirtualXRay::RATIONAL_NUMBER, gVirtualXRay::RATIONAL_NUMBER> >::const_iterator ite = p_input.begin();
        ite != p_input.end();
        ++ite)    
    {

        p_output.push_back(std::vector<float>(2));
        p_output.back()[0] = ite->first / unit_of_energy;
        p_output.back()[1] = ite->second / unit_of_energy;
    }

    return p_output;
}


//---------------------------------------------------------------
double getScintillatorThickness(const std::string& aUnitOfLength)
//---------------------------------------------------------------
{
    return g_xray_detector.getScintillatorThickness() / getUnitOfLength(aUnitOfLength);
}


//---------------------
float getDetectorGain()
//---------------------
{
    return g_xray_detector.getGain();
}

//-------------------------------
void setDetectorGain(float aGain)
//-------------------------------
{
    g_xray_detector.setGain(aGain);
}


//---------------------
float getDetectorBias()
//---------------------
{
    return g_xray_detector.getBias();
}


//-------------------------------
void setDetectorBias(float aBias)
//-------------------------------
{
    g_xray_detector.setBias(aBias);
}


//---------------------------
float getDetectorStdDevBias()
//---------------------------
{
    return g_xray_detector.getStdDevBias();
}


//-------------------------------------------
void setDetectorStdDevBias(float aStdDevBias)
//-------------------------------------------
{
    g_xray_detector.setStdDevBias(aStdDevBias);
}


//---------------------------------------------------------
std::vector<int> getLargestPossibleDetectorNumberOfPixels()
//---------------------------------------------------------
{
    GLint texture_size = 0;

#ifdef HAS_OPENGL
    glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &texture_size);
    gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
#endif

    std::vector<int> return_value = {texture_size, texture_size};

    return return_value;
}