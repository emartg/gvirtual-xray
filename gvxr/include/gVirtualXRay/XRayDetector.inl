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

3. Neither the name of Bangor University, UK Research and Innovation nor the 
names of its contributors may be used to endorse or promote products derived 
from this software without specific prior written permission.

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
*   @file       XRayDetector.inl
*
*   @brief      Class to handle virtual X-Ray detectors.
*
*   @version    1.0
*
*   @date       10/04/2024
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
*   (c) by Dr Franck P. Vidal, Bangor University, Nov 2013-Sept 2023, version 1.1, BSD 3-Clause License
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, Oct 2023-present, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>
#include <numeric> // To sum all the elements of the LSF

#ifndef GVXR_FILE_DOES_NOT_EXIST_EXCEPTION_H
#include "gVirtualXRay/FileDoesNotExistException.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//----------------------------------------------
inline XRayDetector::XRayDetector():
//----------------------------------------------
	m_resolution_in_unit_of_length_per_pixel(0),
	m_distance_source_detector(0),
	m_vbo_need_update(true),
	//m_source_mesh(10, 10, 0.5 * cm),
	m_widget_length(5 * mm),
	m_source_mesh(WIDGET_SPHERE_SUBDIVISIONS, m_widget_length / 2.0),
	m_first_pixel_mesh(m_widget_length, GL_UNSIGNED_BYTE),
	m_detector_centre_mesh(WIDGET_SPHERE_SUBDIVISIONS, m_widget_length / 2.0),
    m_source_shape(UNKNOWN_SHAPE),
	m_update_right_vector_flag(true),
	m_gain(1.0f),
	m_bias(0.0f),
	m_std_dev_bias(0.0f)
//----------------------------------------------
{
}


//----------------------------------
inline XRayDetector::~XRayDetector()
//----------------------------------
{
	release();
}


//---------------------------------
inline void XRayDetector::release()
//---------------------------------
{
    m_p_xray_source_position_set.clear();
    m_detector_geometry.reset();
    m_parallel_beam.reset();
    m_cone_beam.reset();
    m_parallel_beam_highlight.reset();
    m_cone_beam_highlight.reset();
    
	m_source_mesh.resetVBOs();
	m_first_pixel_mesh.resetVBOs();
	m_detector_centre_mesh.resetVBOs();
	
	m_gain = 1.0;
	m_bias = 0.0;
	m_std_dev_bias = 0.0;
}


//-----------------------------------------------------------------------
inline void XRayDetector::setWidgetLength(const RATIONAL_NUMBER& aLength)
//-----------------------------------------------------------------------
{
	m_widget_length = aLength;

	m_source_mesh.create(WIDGET_SPHERE_SUBDIVISIONS, m_widget_length / 2.0);
	m_first_pixel_mesh.create(m_widget_length, GL_UNSIGNED_BYTE),
	m_detector_centre_mesh.create(WIDGET_SPHERE_SUBDIVISIONS, m_widget_length / 2.0);
}


//---------------------------------------
inline void XRayDetector::displaySource()
//---------------------------------------
{
    // Display all the sub-sources
    for (std::vector<VEC3>::const_iterator ite(m_p_xray_source_position_set.begin());
            ite != m_p_xray_source_position_set.end();
            ++ite)
    {
        // Store the model/view matrix
        pushModelViewMatrix();

        g_current_modelview_matrix *= MATRIX4::buildTranslationMatrix(ite->getX(), ite->getY(), ite->getZ());
        applyModelViewMatrix();

        // Draw sphere
        m_source_mesh.display();

        // Restore the model/view matrix
        popModelViewMatrix();
    }
}


//------------------------------------------------
inline void XRayDetector::displaySourceWireframe()
//------------------------------------------------
{
    // Display all the sub-sources
    for (std::vector<VEC3>::const_iterator ite(m_p_xray_source_position_set.begin());
            ite != m_p_xray_source_position_set.end();
            ++ite)
    {
        // Store the model/view matrix
        pushModelViewMatrix();

        g_current_modelview_matrix *= MATRIX4::buildTranslationMatrix(ite->getX(), ite->getY(), ite->getZ());
        applyModelViewMatrix();

        // Draw sphere
        m_source_mesh.displayWireFrame();

        // Restore the model/view matrix
        popModelViewMatrix();
    }
}


//------------------------------------------------------------------------------------------------
inline void XRayDetector::setResolutionInUnitsOfLengthPerPixel(const RATIONAL_NUMBER& aResolution)
//------------------------------------------------------------------------------------------------
{
    // The resolution has changed
    m_resolution_in_unit_of_length_per_pixel = VEC2(aResolution, aResolution);

    // Update the size of the detector
    updateSizeInUnitOfLength();

	// Update the distance between the source and the detector, etc.
	updateInternalValues();

	// The VBOs need to be updated
	m_vbo_need_update = true;
}


//-------------------------------------------------------------------------------------
inline void XRayDetector::setResolutionInUnitsOfLengthPerPixel(const VEC2& aResolution)
//-------------------------------------------------------------------------------------
{
    // The resolution has changed
    m_resolution_in_unit_of_length_per_pixel = aResolution;

    // Update the size of the detector
    updateSizeInUnitOfLength();

	// Update the distance between the source and the detector, etc.
	updateInternalValues();

	// The VBOs need to be updated
	m_vbo_need_update = true;
}


//----------------------------------------------------------------------
inline void XRayDetector::setNumberOfPixels(const Vec2ui& aSizeInPixels)
//----------------------------------------------------------------------
{
	// The number of pixels has changed
	m_size_in_number_of_pixels = aSizeInPixels;

	// Update the size of the detector
	updateSizeInUnitOfLength();

	// Update the distance between the source and the detector, etc.
	updateInternalValues();

	// The VBOs need to be updated
	m_vbo_need_update = true;
}


//--------------------------------------------------------------------------
inline void XRayDetector::setDetectorPosition(const VEC3& aDetectorPosition)
//--------------------------------------------------------------------------
{
	// The position of the source has changed
	m_detector_position = aDetectorPosition;

	// Update the distance between the source and the detector, etc.
	updateInternalValues();

	// The VBOs need to be updated
	m_vbo_need_update = true;
}


//---------------------------------------------------------------------------
inline void XRayDetector::setXrayPointSource(const VEC3& aXraySourcePosition)
//---------------------------------------------------------------------------
{
	// Store the centre position
	m_xray_source_centre = aXraySourcePosition;

	// Clear the sample set
	m_p_xray_source_position_set.clear();

	// Add the centre
	m_p_xray_source_position_set.push_back(m_xray_source_centre);

	// Save the source type
	m_source_shape = POINT;

	// Update the distance between the source and the detector, etc.
	updateInternalValues();

	// The VBOs need to be updated
	m_vbo_need_update = true;
}


//--------------------------------------------
inline void XRayDetector::setXrayPointSource()
//--------------------------------------------
{
	// Save the source type
	m_source_shape = POINT;

	// Update the distance between the source and the detector, etc.
	updateInternalValues();

	// The VBOs need to be updated
	m_vbo_need_update = true;
}


//----------------------------------------
inline void XRayDetector::setCubicSource()
//----------------------------------------
{
	// Save the source type
	m_source_shape = CUBE;

	// Update the distance between the source and the detector, etc.
	updateInternalValues();

	// The VBOs need to be updated
	m_vbo_need_update = true;
}


//-----------------------------------------
inline void XRayDetector::setSquareSource()
//-----------------------------------------
{
	// Save the source type
	m_source_shape = SQUARE;

	// Update the distance between the source and the detector, etc.
	updateInternalValues();

	// The VBOs need to be updated
	m_vbo_need_update = true;
}


//--------------------------------------------
inline void XRayDetector::setRectangleSource()
//--------------------------------------------
{
	// Save the source type
	m_source_shape = RECTANGLE;

	// Update the distance between the source and the detector, etc.
	updateInternalValues();

	// The VBOs need to be updated
	m_vbo_need_update = true;
}


//---------------------------------------
inline void XRayDetector::setLineSource()
//---------------------------------------
{
	// Save the source type
	m_source_shape = LINE;

	// Update the distance between the source and the detector, etc.
	updateInternalValues();

	// The VBOs need to be updated
	m_vbo_need_update = true;
}


//-----------------------------------------------------------
inline void XRayDetector::setUpVector(const VEC3& anUpVector)
//-----------------------------------------------------------
{
	m_up_vector = anUpVector.normal();

	// Update the distance between the source and the detector, etc.
	updateInternalValues();

	// The VBOs need to be updated
	m_vbo_need_update = true;
}


//----------------------------------------------------------------
inline void XRayDetector::setRightVector(const VEC3& aRightVector)
//----------------------------------------------------------------
{
	m_update_right_vector_flag = false;
	m_right_vector = aRightVector.normal();

	// Update the distance between the source and the detector, etc.
	updateInternalValues();

	// The VBOs need to be updated
	m_vbo_need_update = true;
}


//-----------------------------------------------------
inline void XRayDetector::autoAlignDetector(bool aFlag)
//-----------------------------------------------------
{
	m_update_right_vector_flag = aFlag;

	if (m_update_right_vector_flag)
	{
		// Update the distance between the source and the detector, etc.
		updateInternalValues();

		// The VBOs need to be updated
		m_vbo_need_update = true;
	}
}


//----------------------------------------------------------------------
inline void XRayDetector::rotateDetector(const RATIONAL_NUMBER& anAngle,
                                         const VEC3& aRotationAxis)
//----------------------------------------------------------------------
{
	// Rotate around the centre of the detector
	MATRIX4 transformation_matrix;
	transformation_matrix *= MATRIX4::buildRotationMatrix(anAngle, aRotationAxis.normal());

	// Apply the rotation
	m_up_vector = transformation_matrix * m_up_vector;
	m_right_vector = transformation_matrix * m_right_vector;

	// Update the distance between the source and the detector, etc.
	updateInternalValues();

	// The VBOs need to be updated
	m_vbo_need_update = true;
}


//-----------------------------------------------------------------
inline void XRayDetector::setRotationMatrix(const MATRIX4& aMatrix)
//-----------------------------------------------------------------
{
	m_rotation_matrix = aMatrix;

	// Update the distance between the source and the detector, etc.
	updateInternalValues();

	// The VBOs need to be updated
	m_vbo_need_update = true;
}


//-----------------------------------------
inline void XRayDetector::setParallelBeam()
//-----------------------------------------
{
	// Save the source type
	m_source_shape = PARALLEL;

	// Clear the sample set
	m_p_xray_source_position_set.clear();

	// Add the centre
	m_p_xray_source_position_set.push_back(m_xray_source_centre);

	// Update the distance between the source and the detector, etc.
	updateInternalValues();

	// The VBOs need to be updated
	m_vbo_need_update = true;
}


//-------------------------------------
inline void XRayDetector::setConeBeam()
//-------------------------------------
{
	// Save the source type
	m_source_shape = POINT;

	// Clear the sample set
	m_p_xray_source_position_set.clear();

	// Add the centre
	m_p_xray_source_position_set.push_back(m_xray_source_centre);

	// Update the distance between the source and the detector, etc.
	updateInternalValues();

	// The VBOs need to be updated
	m_vbo_need_update = true;
}


//----------------------------------------
inline void XRayDetector::setPointSource()
//----------------------------------------
{
	setConeBeam();
}


//----------------------------------------------------------
inline const Vec2ui& XRayDetector::getNumberOfPixels() const
//----------------------------------------------------------
{
	return (m_size_in_number_of_pixels);
}


//------------------------------------------------------------
inline const VEC2& XRayDetector::getSizeInUnitOfLength() const
//------------------------------------------------------------
{
	return (m_size_in_unit_of_length);
}


//--------------------------------------------------------------------
inline const VEC2& XRayDetector::getPixelSpacingInUnitOfLength() const
//--------------------------------------------------------------------
{
	return (m_resolution_in_unit_of_length_per_pixel);
}


//----------------------------------------------------------
inline const VEC3& XRayDetector::getDetectorPosition() const
//----------------------------------------------------------
{
	return (m_detector_position);
}


//----------------------------------------------------------
inline const VEC3& XRayDetector::getXraySourceCentre() const
//----------------------------------------------------------
{
	return (m_xray_source_centre);
}


//----------------------------------------------------------------
inline unsigned int XRayDetector::getNumberOfSourceSamples() const
//----------------------------------------------------------------
{
	return (m_p_xray_source_position_set.size());
}


//------------------------------------------------------------------------------------
inline const VEC3& XRayDetector::getSourceSamplePosition(unsigned int aSampleID) const
//------------------------------------------------------------------------------------
{
	return (m_p_xray_source_position_set[aSampleID]);
}


//--------------------------------------------------
inline const VEC3& XRayDetector::getUpVector() const
//--------------------------------------------------
{
	return (m_up_vector);
}


//-----------------------------------------------------
inline const VEC3& XRayDetector::getRightVector() const
//-----------------------------------------------------
{
	return (m_right_vector);
}


//-----------------------------------------------------------
inline const MATRIX4& XRayDetector::getRotationMatrix() const
//-----------------------------------------------------------
{
	return (m_rotation_matrix);
}


//-------------------------------------------------------------------
inline XRayDetector::SourceShape XRayDetector::getSourceShape() const
//-------------------------------------------------------------------
{
	return (m_source_shape);
}


//------------------------------------------------------------------------
inline const std::vector<VEC3>& XRayDetector::getSourcePositionSet() const
//------------------------------------------------------------------------
{
	return (m_p_xray_source_position_set);
}


//--------------------------------------------------------------------------------------
inline void XRayDetector::setSourcePositions(const std::vector<VEC3>& aSourcePostionSet)
//--------------------------------------------------------------------------------------
{
	// Set the position of sources
	m_p_xray_source_position_set = aSourcePostionSet;

	// Update the distance between the source and the detector, etc.
	updateInternalValues();

	// The VBOs need to be updated
	m_vbo_need_update = true;
}


//------------------------------------------------------------------------------
inline void XRayDetector::rotateFocalSpot(const RATIONAL_NUMBER& aRotationAngle,
                                          const VEC3& aRotationAxis)
//------------------------------------------------------------------------------
{
	RATIONAL_NUMBER l = aRotationAxis.length();
	RATIONAL_NUMBER x = aRotationAxis[0] / l;
	RATIONAL_NUMBER y = aRotationAxis[1] / l;
	RATIONAL_NUMBER z = aRotationAxis[2] / l;

	MATRIX4 rotation_matrix = MATRIX4::buildRotationMatrix(aRotationAngle, x, y, z);

	// Set the position of sources
	if (m_p_xray_source_position_set.size() > 1)
	{
		for (std::vector<VEC3>::iterator ite = m_p_xray_source_position_set.begin();
			ite != m_p_xray_source_position_set.end();
			++ite)
		{
			*ite -= m_xray_source_centre;
			*ite = rotation_matrix * *ite;
			*ite += m_xray_source_centre;
		}
	}

	// The VBOs need to be updated
	m_vbo_need_update = true;
}


//---------------------------------------------------------------------------
inline void XRayDetector::rotateSource(const RATIONAL_NUMBER& aRotationAngle,
                                       const VEC3& aRotationCentre,
                                       const VEC3& aRotationAxis)
//---------------------------------------------------------------------------
{
	RATIONAL_NUMBER l = aRotationAxis.length();
	RATIONAL_NUMBER x = aRotationAxis[0] / l;
	RATIONAL_NUMBER y = aRotationAxis[1] / l;
	RATIONAL_NUMBER z = aRotationAxis[2] / l;

	MATRIX4 rotation_matrix = MATRIX4::buildRotationMatrix(aRotationAngle, x, y, z);

	// Set the position of sources
	if (m_p_xray_source_position_set.size() > 0)
	{
		m_xray_source_centre = VEC3();

		for (std::vector<VEC3>::iterator ite = m_p_xray_source_position_set.begin();
			ite != m_p_xray_source_position_set.end();
			++ite)
		{
			*ite -= aRotationCentre;
			*ite = rotation_matrix * *ite;
			*ite += aRotationCentre;

			m_xray_source_centre += *ite;
		}

		m_xray_source_centre[0] /= m_p_xray_source_position_set.size();
		m_xray_source_centre[1] /= m_p_xray_source_position_set.size();
		m_xray_source_centre[2] /= m_p_xray_source_position_set.size();
	}

	// The VBOs need to be updated
	m_vbo_need_update = true;
}


//-----------------------------------------------
inline bool XRayDetector::useParallelBeam() const
//-----------------------------------------------
{
	return (m_source_shape == PARALLEL);
}


//---------------------------------------------------------
inline const RATIONAL_NUMBER& XRayDetector::getFOVY() const
//---------------------------------------------------------
{
    return (m_fovy);
}


//---------------------------------------------
inline void XRayDetector::clearEnergyResponse()
//---------------------------------------------
{
	m_scintillator.release();
}


//--------------------------------------------------------------------------
inline void XRayDetector::setScintillator(const Scintillator& aScintillator)
//--------------------------------------------------------------------------
{
	m_scintillator = aScintillator;
}


//---------------------------------------------------------------------
inline void XRayDetector::setScintillator(const std::string& aMaterial,
	                                      const float& aThickness)
//---------------------------------------------------------------------
{
	m_scintillator.setMaterial(aMaterial);
	m_scintillator.setThickness(aThickness);
}


//---------------------------------------------------------------
inline const std::string& XRayDetector::getScintillatorMaterial()
//---------------------------------------------------------------
{
	return m_scintillator.getMaterial();
}


//----------------------------------------------------
inline double XRayDetector::getScintillatorThickness()
//----------------------------------------------------
{
	return m_scintillator.getThickness();
}


//------------------------------------------------------------
inline void XRayDetector::setLSF(const std::string& aFileName)
//------------------------------------------------------------
{
	// Empty the current LSF
	m_LSF.clear();

	// Open the file
	std::ifstream input_stream(aFileName);

	// The file is not open
	if (!input_stream.is_open())
	{
		throw FileDoesNotExistException(__FILE__, __FUNCTION__, __LINE__, aFileName);
	}

	// Load the file
	double coefficient;
	while (input_stream >> coefficient)
	{
		// The record is valid
		if (!input_stream.eof())
		{
			m_LSF.push_back(coefficient);
		}
	}

	// Normalise the array (divide by sum)
    double sum = std::accumulate(m_LSF.begin(), m_LSF.end(), 0.0);
	for (std::vector<RATIONAL_NUMBER>::iterator ite(m_LSF.begin());
        ite != m_LSF.end();
        ++ite)
    {
        *ite /= sum;
    }
}


//------------------------------------------------------------------------
inline void XRayDetector::setLSF(const std::vector<RATIONAL_NUMBER>& aLSF)
//------------------------------------------------------------------------
{
    // Copy the LSF
    m_LSF = aLSF;

    // Normalise the array (divide by sum)
    double sum = std::accumulate(m_LSF.begin(), m_LSF.end(), 0.0);
	for (std::vector<RATIONAL_NUMBER>::iterator ite(m_LSF.begin());
        ite != m_LSF.end();
        ++ite)
    {
        *ite /= sum;
    }
}


//---------------------------------------------------------------------
inline const std::vector<RATIONAL_NUMBER>& XRayDetector::getLSF() const
//---------------------------------------------------------------------
{
    return m_LSF;
}


//----------------------------------
inline void XRayDetector::clearLSF()
//----------------------------------
{
    m_LSF.clear();
}


//------------------------------------------------------------------
inline void XRayDetector::setPSF(const Image<RATIONAL_NUMBER>& aPSF)
//------------------------------------------------------------------
{
	m_PSF = aPSF;
}


//---------------------------------------------------------------
inline const Image<RATIONAL_NUMBER>& XRayDetector::getPSF() const
//---------------------------------------------------------------
{
	return m_PSF;
}


//-------------------------------------------------------------------------------------------------------------
inline RATIONAL_NUMBER XRayDetector::convertPerCm2At1m2PerPixelAtSDD(const RATIONAL_NUMBER& aPhotonCount) const
//-------------------------------------------------------------------------------------------------------------
{
	RATIONAL_NUMBER area_pixel_in_cm2 = (getPixelSpacingInUnitOfLength()[0] / cm) * 
		(getPixelSpacingInUnitOfLength()[1] / cm);
	RATIONAL_NUMBER SDD_in_m = (getDetectorPosition() / m - getXraySourceCentre() / m).length();

	if (SDD_in_m * m / um < 0.1)
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "The source-detector distance is null. You must initialise the source and detector before using this function.");
	}

	if (area_pixel_in_cm2 * (cm * cm) / (um * um) < 0.1)
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "The pixel pitch is null. You must initialise the detector before using this function.");
	}

	RATIONAL_NUMBER number_of_photons_per_pixel_at_1m = aPhotonCount * area_pixel_in_cm2;
	
	RATIONAL_NUMBER number_of_photons_per_pixel_at_sdd = number_of_photons_per_pixel_at_1m / (SDD_in_m * SDD_in_m);
	
	//if (std::isnan(number_of_photons_per_pixel_at_sdd)) number_of_photons_per_pixel_at_sdd = 0;
	
	return number_of_photons_per_pixel_at_sdd; 
}


//-------------------------------------------------------------------------------------------------------------
inline RATIONAL_NUMBER XRayDetector::convertPerPixelAtSDD2PerCm2At1m(const RATIONAL_NUMBER& aPhotonCount) const
//-------------------------------------------------------------------------------------------------------------
{
	RATIONAL_NUMBER area_pixel_in_cm2 = (getPixelSpacingInUnitOfLength()[0] / cm) * 
		(getPixelSpacingInUnitOfLength()[1] / cm);
	RATIONAL_NUMBER SDD_in_m = (getDetectorPosition() / m - getXraySourceCentre() / m).length();

	if (SDD_in_m * m / um < 0.1)
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "The source-detector distance is null. You must initialise the source and detector before using this function.");
	}

	if (area_pixel_in_cm2 * (cm * cm) / (um * um) < 0.1)
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "The pixel pitch is null. You must initialise the detector before using this function.");
	}
	
	RATIONAL_NUMBER number_of_photons_per_cm2_at_SDD = aPhotonCount / area_pixel_in_cm2;
	return number_of_photons_per_cm2_at_SDD * (SDD_in_m * SDD_in_m);
}


//--------------------------------------------
inline RATIONAL_NUMBER XRayDetector::getGain()
//--------------------------------------------
{
	return m_gain;
}


//------------------------------------------------------
inline void XRayDetector::setGain(RATIONAL_NUMBER aGain)
//------------------------------------------------------
{
	if (aGain < 1.0f) {
		std::stringstream error_message;
		error_message << "The gain is " << aGain << ", which is less than 1.0. This value is unexpected.";
		throw Exception(__FILE__, __FUNCTION__, __LINE__,
			error_message.str());
	}

	m_gain = aGain;
}


//--------------------------------------------
inline RATIONAL_NUMBER XRayDetector::getBias()
//--------------------------------------------
{
	return m_bias;
}


//------------------------------------------------------
inline void XRayDetector::setBias(RATIONAL_NUMBER aBias)
//------------------------------------------------------
{
	if (std::abs(aBias) > EPSILON && aBias < 0.0) {
		std::stringstream error_message;
		error_message << "The bias is " << aBias << ", which is less than 0.0. This value is unexpected.";
		throw Exception(__FILE__, __FUNCTION__, __LINE__,
			error_message.str());
	}

	m_bias = aBias;
}


//--------------------------------------------------
inline RATIONAL_NUMBER XRayDetector::getStdDevBias()
//--------------------------------------------------
{
	return m_std_dev_bias;
}


//------------------------------------------------------------------
inline void XRayDetector::setStdDevBias(RATIONAL_NUMBER aStdDevBias)
//------------------------------------------------------------------
{
	m_std_dev_bias = aStdDevBias;
}


//--------------------------------------------------------------------------------
inline void XRayDetector::loadEnergyResponse(const std::string& aFileName,
  		        							 const RATIONAL_NUMBER& aUnitOfEnergy)
//--------------------------------------------------------------------------------
{
	m_scintillator.loadEnergyResponse(aFileName, aUnitOfEnergy);
}


//--------------------------------------------------------------------------------------------------------
inline void XRayDetector::setEnergyResponse(const std::vector<std::pair<float, float> >& anEnergyResponse)
//--------------------------------------------------------------------------------------------------------
{
	m_scintillator.setEnergyResponse(anEnergyResponse);
}


//----------------------------------------------------------------------------------------------------------
inline void XRayDetector::setEnergyResponse(const std::vector<std::pair<double, double> >& anEnergyResponse)
//---------------------------------------------------------------------------------------------------------
{
	m_scintillator.setEnergyResponse(anEnergyResponse);
}


//------------------------------------------------------------------------------------------------------
inline std::vector<std::pair<RATIONAL_NUMBER, RATIONAL_NUMBER> > XRayDetector::getEnergyResponse() const
//------------------------------------------------------------------------------------------------------
{
	return m_scintillator.getEnergyResponse();
}


//---------------------------------------------------------------------------------------------
inline RATIONAL_NUMBER XRayDetector::applyEnergyResponse(const RATIONAL_NUMBER& anEnergy) const
//---------------------------------------------------------------------------------------------
{
	return m_scintillator.applyEnergyResponse(anEnergy);
}


//--------------------------------------------------
inline void XRayDetector::updateSizeInUnitOfLength()
//--------------------------------------------------
{
	m_size_in_unit_of_length.setX(m_resolution_in_unit_of_length_per_pixel.getX() * m_size_in_number_of_pixels.getX());
	m_size_in_unit_of_length.setY(m_resolution_in_unit_of_length_per_pixel.getY() * m_size_in_number_of_pixels.getY());
}


//-----------------------------------------------------------------
inline const VEC2& XRayDetector::getPixelSizeInUnitOfLength() const
//-----------------------------------------------------------------
{
	return (m_resolution_in_unit_of_length_per_pixel);
}


//----------------------------------------------
inline void XRayDetector::updateInternalValues()
//----------------------------------------------
{
	// The distance between the source and the detector has changed
	m_distance_source_detector = (m_detector_position - m_xray_source_centre).length();

	// Compute the normal vector
	m_look_at_vector = m_detector_position - m_xray_source_centre;
	m_look_at_vector.normalize();

	if (m_update_right_vector_flag || m_right_vector.length() < 0.5)
		m_right_vector = m_look_at_vector ^ m_up_vector;
	
	m_normal = m_up_vector ^ m_right_vector;

    // Compute the position of each vertex
	VEC3 v0(m_detector_position - m_right_vector * m_size_in_unit_of_length.getX() / 2.0 - m_up_vector * m_size_in_unit_of_length.getY() / 2.0);
	VEC3 v1(m_detector_position + m_right_vector * m_size_in_unit_of_length.getX() / 2.0 - m_up_vector * m_size_in_unit_of_length.getY() / 2.0);
	VEC3 v2(m_detector_position + m_right_vector * m_size_in_unit_of_length.getX() / 2.0 + m_up_vector * m_size_in_unit_of_length.getY() / 2.0);
	VEC3 v3(m_detector_position - m_right_vector * m_size_in_unit_of_length.getX() / 2.0 + m_up_vector * m_size_in_unit_of_length.getY() / 2.0);

	// Vertices
    m_p_detector_geometry_vertices.clear();

	m_p_detector_geometry_vertices.push_back(v0.getX());
	m_p_detector_geometry_vertices.push_back(v0.getY());
	m_p_detector_geometry_vertices.push_back(v0.getZ());

	m_p_detector_geometry_vertices.push_back(v1.getX());
	m_p_detector_geometry_vertices.push_back(v1.getY());
	m_p_detector_geometry_vertices.push_back(v1.getZ());

	m_p_detector_geometry_vertices.push_back(v2.getX());
	m_p_detector_geometry_vertices.push_back(v2.getY());
	m_p_detector_geometry_vertices.push_back(v2.getZ());

	m_p_detector_geometry_vertices.push_back(v0.getX());
	m_p_detector_geometry_vertices.push_back(v0.getY());
	m_p_detector_geometry_vertices.push_back(v0.getZ());

	m_p_detector_geometry_vertices.push_back(v2.getX());
	m_p_detector_geometry_vertices.push_back(v2.getY());
	m_p_detector_geometry_vertices.push_back(v2.getZ());

	m_p_detector_geometry_vertices.push_back(v3.getX());
	m_p_detector_geometry_vertices.push_back(v3.getY());
	m_p_detector_geometry_vertices.push_back(v3.getZ());

	// Update the view along the Y-axis
	updateFOVY();

	m_detector_centre_mesh.create(2, m_size_in_unit_of_length[0] * 0.05 / 2);
    m_first_pixel_mesh.create(m_size_in_unit_of_length[0] * 0.05, GL_UNSIGNED_BYTE);

}


//------------------------------------------------------------------
inline void XRayDetector::getPlaneEquation(RATIONAL_NUMBER& A,
												 RATIONAL_NUMBER& B,
												 RATIONAL_NUMBER& C,
												 RATIONAL_NUMBER& D)
//------------------------------------------------------------------
{
	// Transform the normal
	VEC3 normal(m_rotation_matrix * m_normal);

	// Compute the plane equation
	A = normal.getX();
	B = normal.getY();
	C = normal.getZ();
	D = -(A * m_detector_position.getX() + B * m_detector_position.getY() + C * m_detector_position.getZ());
}


//----------------------------------------------------------------
inline const std::vector<float>& XRayDetector::getVertices() const
//----------------------------------------------------------------
{
    return m_p_detector_geometry_vertices;
}


//------------------------------------
inline void XRayDetector::updateFOVY()
//------------------------------------
{
    m_fovy = 2.0 * (180.0  / Pi) * atan2(RATIONAL_NUMBER(m_size_in_unit_of_length.getY() / 2.0), m_distance_source_detector);
}


} // namespace gVirtualXRay
