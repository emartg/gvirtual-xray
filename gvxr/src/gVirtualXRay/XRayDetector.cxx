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
*	@file		XRayDetector.cxx
*
*	@brief		Class to handle virtual X-Ray detectors.
*
*	@version	1.0
*
*	@date		30/11/2013
*
*	@author		Dr Franck P. Vidal
*
*	@section	License
*				BSD 3-Clause License.
*
*				For details on use and redistribution please refer
*				to http://opensource.org/licenses/BSD-3-Clause
*
*	@section	Copyright
*				(c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*				http://www.fpvidal.net/, Dec 2014, 2014, version 1.0,
*				BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#ifndef GVXR_XRAY_DETECTOR_H
#include "gVirtualXRay/XRayDetector.h"
#endif

#ifndef GVXR_OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif


//******************************************************************************
//	Name space
//******************************************************************************
using namespace gVirtualXRay;


//----------------------------------
void XRayDetector::displayDetector()
//----------------------------------
{
	// Update the VBOs if needed
	updateVBOs();

	// Store the current transformation matrix
	pushModelViewMatrix();

	// Apply the transformation
	g_current_modelview_matrix *= MATRIX4::buildTranslationMatrix(m_detector_position);
	g_current_modelview_matrix *= m_rotation_matrix;
	g_current_modelview_matrix *= MATRIX4::buildTranslationMatrix(-m_detector_position);
	applyModelViewMatrix();

	// Display the detector using OpenGL's direct mode rendering
	m_detector_geometry->display();

	// Restore the current transformation matrix
	popModelViewMatrix();
}


//------------------------------
void XRayDetector::displayBeam()
//------------------------------
{
	// Update the VBOs if needed
	updateVBOs();

#ifdef HAS_OPENGL
	// Store the enable/disable state
	pushEnableDisableState(GL_DEPTH_TEST);
	pushEnableDisableState(GL_BLEND);

	//glPushAttrib(GL_DEPTH_BUFFER_BIT);
	//glPushAttrib(GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (m_source_shape == PARALLEL)
	{
		m_parallel_beam->display();
	}
	else
	{
		m_cone_beam->display();
	}

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);

	if (m_source_shape == PARALLEL)
	{
		m_parallel_beam_highlight->display();
	}
	else
	{
		m_cone_beam_highlight->display();
	}

	// Restore the enable/disable state
	popEnableDisableState();
	popEnableDisableState();
#endif
}


//--------------------------------------------
void XRayDetector::displayFirstPixelPosition()
//--------------------------------------------
{
	// Update the VBOs if needed
	updateVBOs();

	// Store the current transformation matrix
	pushModelViewMatrix();

	// Apply the transformation
	VEC3 translation_vector = m_detector_position - m_right_vector * m_size_in_unit_of_length.getX() / 2.0 - m_up_vector * m_size_in_unit_of_length.getY() / 2.0;
	g_current_modelview_matrix *= MATRIX4::buildTranslationMatrix(translation_vector);
	g_current_modelview_matrix *= m_rotation_matrix;
	// g_current_modelview_matrix *= MATRIX4::buildTranslationMatrix(-m_detector_position);
	applyModelViewMatrix();

	// Display the detector using OpenGL's direct mode rendering
	m_first_pixel_mesh.display();

	// Restore the current transformation matrix
	popModelViewMatrix();
}


//------------------------------------------
void XRayDetector::displayDetectorPosition()
//------------------------------------------
{
	// Update the VBOs if needed
	updateVBOs();

	// Store the current transformation matrix
	pushModelViewMatrix();

	// Apply the transformation
	g_current_modelview_matrix *= MATRIX4::buildTranslationMatrix(m_detector_position);
	applyModelViewMatrix();

	// Display the detector using OpenGL's direct mode rendering
	m_detector_centre_mesh.display();

	// Restore the current transformation matrix
	popModelViewMatrix();
}


//-------------------------------------------------------------------------------
void XRayDetector::setCubicSource(const VEC3& aXraySourcePosition,
								  unsigned int aNumberOfSamplesAlongOneDirection,
								  RATIONAL_NUMBER aLength)
//-------------------------------------------------------------------------------
{
	// Store the centre position
	m_xray_source_centre = aXraySourcePosition;

	// Clear the sample set
	m_p_xray_source_position_set.clear();

	// Use a cubic source
	if (aNumberOfSamplesAlongOneDirection > 1)
	{
		// Save the source type
		m_source_shape = CUBE;

		// Sample the source along the 3 main axis
		VEC3 source_lower_corner(m_xray_source_centre.getX() - aLength / 2.0,
				m_xray_source_centre.getY() - aLength / 2.0,
				m_xray_source_centre.getZ() - aLength / 2.0);

		RATIONAL_NUMBER offset(aLength / RATIONAL_NUMBER(aNumberOfSamplesAlongOneDirection - 1));

		// Compute the sample position along the 3 axis
		for (unsigned int k(0); k < aNumberOfSamplesAlongOneDirection; ++k)
		{
			for (unsigned int j(0); j < aNumberOfSamplesAlongOneDirection; ++j)
			{
				for (unsigned int i(0); i < aNumberOfSamplesAlongOneDirection; ++i)
				{
					// Compute the sample position
					VEC3 sample_position(source_lower_corner.getX () + offset * i,
							source_lower_corner.getY () + offset * j,
							source_lower_corner.getZ () + offset * k);

					// Add the sample position
					m_p_xray_source_position_set.push_back(sample_position);
				}
			}
		}
	}
	// This is a point source
	else
	{
		// Save the source type
		m_source_shape = POINT;

		// Add the centre
		m_p_xray_source_position_set.push_back(m_xray_source_centre);
	}

	// Update the distance between the source and the detector
	updateInternalValues();

	// The VBOs need to be updated
	m_vbo_need_update = true;
}


//--------------------------------------------------------------------------------
void XRayDetector::setSquareSource(const VEC3& aXraySourcePosition,
								   unsigned int aNumberOfSamplesAlongOneDirection,
								   RATIONAL_NUMBER aLength)
//--------------------------------------------------------------------------------
{
	// Store the centre position
	m_xray_source_centre = aXraySourcePosition;

	// Clear the sample set
	m_p_xray_source_position_set.clear();

	// Use a cubic source
	if (aNumberOfSamplesAlongOneDirection > 1)
	{
		// Save the source type
		m_source_shape = SQUARE;

		// Compute the normal vector
		VEC3 look_at_vector(m_detector_position - m_xray_source_centre);
		look_at_vector.normalize();

		// Compute the position of each vertex
		VEC3 right_vector(look_at_vector ^ m_up_vector);

		// Sample the source along the 3 main axis
		VEC3 source_lower_corner(m_xray_source_centre);
		source_lower_corner -= right_vector * (aLength / 2.0);
		source_lower_corner -= m_up_vector * (aLength / 2.0);

		RATIONAL_NUMBER offset(aLength / RATIONAL_NUMBER(aNumberOfSamplesAlongOneDirection - 1));

		// Compute the sample position along the 2 axis
		for (unsigned int j(0); j < aNumberOfSamplesAlongOneDirection; ++j)
		{
			for (unsigned int i(0); i < aNumberOfSamplesAlongOneDirection; ++i)
			{
				// Compute the sample position
				VEC3 sample_position(source_lower_corner + m_up_vector * (offset * j) + right_vector * (offset * i));

				// Add the sample position
				m_p_xray_source_position_set.push_back(sample_position);
			}
		}
	}
	// This is a point source
	else
	{
		// Save the source type
		m_source_shape = POINT;

		// Add the centre
		m_p_xray_source_position_set.push_back(m_xray_source_centre);
	}

	// Update the distance between the source and the detector
	updateInternalValues();

	// The VBOs need to be updated
	m_vbo_need_update = true;
}


//-----------------------------------------------------------------------------
void XRayDetector::setRectangleSource(const VEC3& aXraySourcePosition,
								      unsigned int aNumberOfSamplesAlongLength,
								      unsigned int aNumberOfSamplesAlongHeight,
								      RATIONAL_NUMBER aLongitudinalLength,
								      RATIONAL_NUMBER aHeight)
//-----------------------------------------------------------------------------
{
	// Store the centre position
	m_xray_source_centre = aXraySourcePosition;

	// Clear the sample set
	m_p_xray_source_position_set.clear();

	// Use a rectangular source
	if (aNumberOfSamplesAlongLength * aNumberOfSamplesAlongHeight > 1)
	{
		// Save the source type
		m_source_shape = RECTANGLE;

		// Compute the normal vector
		VEC3 look_at_vector(m_detector_position - m_xray_source_centre);
		look_at_vector.normalize();

		// Compute the position of each vertex
		VEC3 right_vector(look_at_vector ^ m_up_vector);

		// Sample the source along the 3 main axis
		VEC3 source_lower_corner(m_xray_source_centre);
		source_lower_corner -= right_vector * (aLongitudinalLength / 2.0);
		source_lower_corner -= m_up_vector * (aHeight / 2.0);

		RATIONAL_NUMBER longitudinal_offset(aLongitudinalLength / RATIONAL_NUMBER(aNumberOfSamplesAlongLength - 1));
		RATIONAL_NUMBER height_offset(aHeight / RATIONAL_NUMBER(aNumberOfSamplesAlongHeight - 1));

		// Compute the sample position along the 2 axis
		for (unsigned int j(0); j < aNumberOfSamplesAlongHeight; ++j)
		{
			for (unsigned int i(0); i < aNumberOfSamplesAlongLength; ++i)
			{
				// Compute the sample position
				VEC3 sample_position(source_lower_corner + m_up_vector * (height_offset * j) + right_vector * (longitudinal_offset * i));

				// Add the sample position
				m_p_xray_source_position_set.push_back(sample_position);
			}
		}
	}
	// This is a point source
	else
	{
		// Save the source type
		m_source_shape = POINT;

		// Add the centre
		m_p_xray_source_position_set.push_back(m_xray_source_centre);
	}

	// Update the distance between the source and the detector
	updateInternalValues();

	// The VBOs need to be updated
	m_vbo_need_update = true;
}

//------------------------------------------------------------------------------
void XRayDetector::setLineSource(const VEC3& aXraySourcePosition,
								 const VEC3& aDirection,
								 unsigned int aNumberOfSamplesAlongOneDirection,
								 RATIONAL_NUMBER aLength)
//------------------------------------------------------------------------------
{
	// Normalise the direction
	VEC3 unit_direction_vector(aDirection.normal());

	// Store the centre position
	m_xray_source_centre = aXraySourcePosition;

	// Clear the sample set
	m_p_xray_source_position_set.clear();

	// Use a cubic source
	if (aNumberOfSamplesAlongOneDirection > 1)
	{
		// Save the source type
		m_source_shape = LINE;

		// Sample the source along the 3 main axis
		VEC3 source_lower_end(m_xray_source_centre - unit_direction_vector * (aLength / 2.0));

		RATIONAL_NUMBER offset(aLength / RATIONAL_NUMBER(aNumberOfSamplesAlongOneDirection - 1));

		// Compute the sample position along the direction vector
		for (unsigned int i(0); i < aNumberOfSamplesAlongOneDirection; ++i)
		{
			// Compute the sample position
			VEC3 sample_position(source_lower_end + (offset * i) * unit_direction_vector);

			// Add the sample position
			m_p_xray_source_position_set.push_back(sample_position);
		}
	}
	// This is a point source
	else
	{
		// Save the source type
		m_source_shape = POINT;

		// Add the centre
		m_p_xray_source_position_set.push_back(m_xray_source_centre);
	}

	// Update the distance between the source and the detector
	updateInternalValues();

	// The VBOs need to be updated
	m_vbo_need_update = true;
}


//-----------------------------
void XRayDetector::updateVBOs()
//-----------------------------
{
	// The VBOs need to be updated
	if (m_vbo_need_update)
	{
		// Update the VBOs
		updateDetectorVBO();
		updateParallelBeamVBO();
		updateConeBeamVBO();

		// The VBOs are up-to-date
		m_vbo_need_update = false;
	}
}


//------------------------------------
void XRayDetector::updateDetectorVBO()
//------------------------------------
{
	// Normal
	std::vector<GLfloat> p_normal;
	p_normal.push_back(-m_normal.getX());
	p_normal.push_back(-m_normal.getY());
	p_normal.push_back(-m_normal.getZ());


	// Texture coordinates
	std::vector<GLfloat> p_texture_coordinates;

	p_texture_coordinates.push_back(0);
	p_texture_coordinates.push_back(0);
	p_texture_coordinates.push_back(0);

	p_texture_coordinates.push_back(1);
	p_texture_coordinates.push_back(0);
	p_texture_coordinates.push_back(0);

	p_texture_coordinates.push_back(1);
	p_texture_coordinates.push_back(1);
	p_texture_coordinates.push_back(0);

	p_texture_coordinates.push_back(0);
	p_texture_coordinates.push_back(0);
	p_texture_coordinates.push_back(0);

	p_texture_coordinates.push_back(1);
	p_texture_coordinates.push_back(1);
	p_texture_coordinates.push_back(0);

	p_texture_coordinates.push_back(0);
	p_texture_coordinates.push_back(1);
	p_texture_coordinates.push_back(0);

    // Create the VBO if needed
    if (!m_detector_geometry.get())
    {
        m_detector_geometry.reset(createVBO());
    }

    m_detector_geometry->setBufferUsageHint(GL_STATIC_DRAW);
	m_detector_geometry->setVertexData(GL_TRIANGLES,
			m_p_detector_geometry_vertices.size()            / 3, 3, GL_FLOAT, &m_p_detector_geometry_vertices[0],
			p_normal.size()              / 3, 3, GL_FLOAT, &p_normal[0],
			p_texture_coordinates.size() / 3, 3, GL_FLOAT, &p_texture_coordinates[0]);
}


//----------------------------------------
void XRayDetector::updateParallelBeamVBO()
//----------------------------------------
{
	VEC3 v0(m_detector_position + m_right_vector * m_size_in_unit_of_length.getX() / 2.0 + m_up_vector * m_size_in_unit_of_length.getY() / 2.0);
	VEC3 v1(m_detector_position - m_right_vector * m_size_in_unit_of_length.getX() / 2.0 + m_up_vector * m_size_in_unit_of_length.getY() / 2.0);
	VEC3 v2(m_detector_position - m_right_vector * m_size_in_unit_of_length.getX() / 2.0 - m_up_vector * m_size_in_unit_of_length.getY() / 2.0);
	VEC3 v3(m_detector_position + m_right_vector * m_size_in_unit_of_length.getX() / 2.0 - m_up_vector * m_size_in_unit_of_length.getY() / 2.0);

	VEC3 v4(m_xray_source_centre);
	VEC3 v5(m_xray_source_centre);
	VEC3 v6(m_xray_source_centre);
	VEC3 v7(m_xray_source_centre);

	v4 +=  m_right_vector * m_size_in_unit_of_length.getX() / 2.0 + m_up_vector * m_size_in_unit_of_length.getY() / 2.0;
	v5 += -m_right_vector * m_size_in_unit_of_length.getX() / 2.0 + m_up_vector * m_size_in_unit_of_length.getY() / 2.0;
	v6 += -m_right_vector * m_size_in_unit_of_length.getX() / 2.0 - m_up_vector * m_size_in_unit_of_length.getY() / 2.0;
	v7 +=  m_right_vector * m_size_in_unit_of_length.getX() / 2.0 - m_up_vector * m_size_in_unit_of_length.getY() / 2.0;

	// Manual rotation  (see above in displayDetector() for rotation using OpenGL)
	v0 -= m_detector_position;
	v1 -= m_detector_position;
	v2 -= m_detector_position;
	v3 -= m_detector_position;

	v0 = m_rotation_matrix * v0;
	v1 = m_rotation_matrix * v1;
	v2 = m_rotation_matrix * v2;
	v3 = m_rotation_matrix * v3;

	v0 += m_detector_position;
	v1 += m_detector_position;
	v2 += m_detector_position;
	v3 += m_detector_position;

	// Vertices
	std::vector<GLfloat> p_vertices;

	p_vertices.push_back(v0.getX());
	p_vertices.push_back(v0.getY());
	p_vertices.push_back(v0.getZ());

	p_vertices.push_back(v1.getX());
	p_vertices.push_back(v1.getY());
	p_vertices.push_back(v1.getZ());

	p_vertices.push_back(v2.getX());
	p_vertices.push_back(v2.getY());
	p_vertices.push_back(v2.getZ());

	p_vertices.push_back(v3.getX());
	p_vertices.push_back(v3.getY());
	p_vertices.push_back(v3.getZ());

	p_vertices.push_back(v4.getX());
	p_vertices.push_back(v4.getY());
	p_vertices.push_back(v4.getZ());

	p_vertices.push_back(v5.getX());
	p_vertices.push_back(v5.getY());
	p_vertices.push_back(v5.getZ());

	p_vertices.push_back(v6.getX());
	p_vertices.push_back(v6.getY());
	p_vertices.push_back(v6.getZ());

	p_vertices.push_back(v7.getX());
	p_vertices.push_back(v7.getY());
	p_vertices.push_back(v7.getZ());


	// The quads' indices
	std::vector<GLubyte> p_indices;

	p_indices.push_back(0);
	p_indices.push_back(4);
	p_indices.push_back(5);
	p_indices.push_back(0);
	p_indices.push_back(5);
	p_indices.push_back(1);

	p_indices.push_back(1);
	p_indices.push_back(5);
	p_indices.push_back(6);
	p_indices.push_back(1);
	p_indices.push_back(6);
	p_indices.push_back(2);

	p_indices.push_back(2);
	p_indices.push_back(6);
	p_indices.push_back(7);
	p_indices.push_back(2);
	p_indices.push_back(7);
	p_indices.push_back(3);

	p_indices.push_back(3);
	p_indices.push_back(7);
	p_indices.push_back(4);
	p_indices.push_back(3);
	p_indices.push_back(4);
	p_indices.push_back(0);

	p_indices.push_back(4);
	p_indices.push_back(5);
	p_indices.push_back(6);
	p_indices.push_back(4);
	p_indices.push_back(6);
	p_indices.push_back(7);

    // Create the VBO if needed
    if (!m_parallel_beam.get())
    {
        m_parallel_beam.reset(createVBO());
    }

	m_parallel_beam->setBufferUsageHint(GL_STATIC_DRAW);
	m_parallel_beam->setVertexData(GL_TRIANGLES,
			p_vertices.size() / 3, 3, GL_FLOAT, &p_vertices[0],
			0, 0, GL_NONE, 0,
			0, 0, GL_NONE, 0);

	m_parallel_beam->setIndexData(p_indices.size(), GL_UNSIGNED_BYTE, &p_indices[0]);

	// The lines' indices
	p_indices.clear();
	p_indices.push_back(0);
	p_indices.push_back(4);

	p_indices.push_back(1);
	p_indices.push_back(5);

	p_indices.push_back(2);
	p_indices.push_back(6);

	p_indices.push_back(3);
	p_indices.push_back(7);

	p_indices.push_back(4);
	p_indices.push_back(5);

	p_indices.push_back(5);
	p_indices.push_back(6);

	p_indices.push_back(6);
	p_indices.push_back(7);

	p_indices.push_back(7);
	p_indices.push_back(4);

    // Create the VBO if needed
    if (!m_parallel_beam_highlight.get())
    {
        m_parallel_beam_highlight.reset(createVBO());
    }

	m_parallel_beam_highlight->setBufferUsageHint(GL_STATIC_DRAW);
	m_parallel_beam_highlight->setVertexData(GL_LINES,
			p_vertices.size() / 3, 3, GL_FLOAT, &p_vertices[0],
			0, 0, GL_NONE, 0,
			0, 0, GL_NONE, 0);

	m_parallel_beam_highlight->setIndexData(p_indices.size(), GL_UNSIGNED_BYTE, &p_indices[0]);
}


//------------------------------------
void XRayDetector::updateConeBeamVBO()
//------------------------------------
{
	VEC3 v0(m_detector_position + m_right_vector * m_size_in_unit_of_length.getX() / 2.0 + m_up_vector * m_size_in_unit_of_length.getY() / 2.0);
	VEC3 v1(m_detector_position - m_right_vector * m_size_in_unit_of_length.getX() / 2.0 + m_up_vector * m_size_in_unit_of_length.getY() / 2.0);
	VEC3 v2(m_detector_position - m_right_vector * m_size_in_unit_of_length.getX() / 2.0 - m_up_vector * m_size_in_unit_of_length.getY() / 2.0);
	VEC3 v3(m_detector_position + m_right_vector * m_size_in_unit_of_length.getX() / 2.0 - m_up_vector * m_size_in_unit_of_length.getY() / 2.0);

	VEC3 v4(m_xray_source_centre);
	VEC3 v5(m_xray_source_centre);
	VEC3 v6(m_xray_source_centre);
	VEC3 v7(m_xray_source_centre);

	// Manual rotation  (see above in displayDetector() for rotation using OpenGL)
	v0 -= m_detector_position;
	v1 -= m_detector_position;
	v2 -= m_detector_position;
	v3 -= m_detector_position;

	v0 = m_rotation_matrix * v0;
	v1 = m_rotation_matrix * v1;
	v2 = m_rotation_matrix * v2;
	v3 = m_rotation_matrix * v3;

	v0 += m_detector_position;
	v1 += m_detector_position;
	v2 += m_detector_position;
	v3 += m_detector_position;

	// Vertices
	std::vector<GLfloat> p_vertices;

	p_vertices.push_back(v0.getX());
	p_vertices.push_back(v0.getY());
	p_vertices.push_back(v0.getZ());

	p_vertices.push_back(v1.getX());
	p_vertices.push_back(v1.getY());
	p_vertices.push_back(v1.getZ());

	p_vertices.push_back(v2.getX());
	p_vertices.push_back(v2.getY());
	p_vertices.push_back(v2.getZ());

	p_vertices.push_back(v3.getX());
	p_vertices.push_back(v3.getY());
	p_vertices.push_back(v3.getZ());

	p_vertices.push_back(v4.getX());
	p_vertices.push_back(v4.getY());
	p_vertices.push_back(v4.getZ());

	p_vertices.push_back(v5.getX());
	p_vertices.push_back(v5.getY());
	p_vertices.push_back(v5.getZ());

	p_vertices.push_back(v6.getX());
	p_vertices.push_back(v6.getY());
	p_vertices.push_back(v6.getZ());

	p_vertices.push_back(v7.getX());
	p_vertices.push_back(v7.getY());
	p_vertices.push_back(v7.getZ());


	// The triangles' indices
	std::vector<GLubyte> p_indices;

	p_indices.push_back(0);
	p_indices.push_back(4);
	p_indices.push_back(1);

	p_indices.push_back(1);
	p_indices.push_back(5);
	p_indices.push_back(2);

	p_indices.push_back(2);
	p_indices.push_back(6);
	p_indices.push_back(3);

	p_indices.push_back(3);
	p_indices.push_back(7);
	p_indices.push_back(0);

    // Create the VBO if needed
    if (!m_cone_beam.get())
    {
        m_cone_beam.reset(createVBO());
    }

	m_cone_beam->setBufferUsageHint(GL_STATIC_DRAW);
	m_cone_beam->setVertexData(GL_TRIANGLES,
			p_vertices.size() / 3, 3, GL_FLOAT, &p_vertices[0],
			0, 0, GL_NONE, 0,
			0, 0, GL_NONE, 0);

	m_cone_beam->setIndexData(p_indices.size(), GL_UNSIGNED_BYTE, &p_indices[0]);

	// The lines, there is no need for an index as no vertex is used twice
	p_vertices.clear();

	p_vertices.push_back(v0.getX());
	p_vertices.push_back(v0.getY());
	p_vertices.push_back(v0.getZ());

	p_vertices.push_back(v4.getX());
	p_vertices.push_back(v4.getY());
	p_vertices.push_back(v4.getZ());

	p_vertices.push_back(v1.getX());
	p_vertices.push_back(v1.getY());
	p_vertices.push_back(v1.getZ());

	p_vertices.push_back(v5.getX());
	p_vertices.push_back(v5.getY());
	p_vertices.push_back(v5.getZ());

	p_vertices.push_back(v2.getX());
	p_vertices.push_back(v2.getY());
	p_vertices.push_back(v2.getZ());

	p_vertices.push_back(v6.getX());
	p_vertices.push_back(v6.getY());
	p_vertices.push_back(v6.getZ());

	p_vertices.push_back(v3.getX());
	p_vertices.push_back(v3.getY());
	p_vertices.push_back(v3.getZ());

	p_vertices.push_back(v7.getX());
	p_vertices.push_back(v7.getY());
	p_vertices.push_back(v7.getZ());

    // Create the VBO if needed
    if (!m_cone_beam_highlight.get())
    {
        m_cone_beam_highlight.reset(createVBO());
    }

	m_cone_beam_highlight->setBufferUsageHint(GL_STATIC_DRAW);
	m_cone_beam_highlight->setVertexData(GL_LINES,
			p_vertices.size() / 3, 3, GL_FLOAT, &p_vertices[0],
			0, 0, GL_NONE, 0,
			0, 0, GL_NONE, 0);
}
