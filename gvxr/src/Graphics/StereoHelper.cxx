/*

Copyright (c) 2014-2023, Dr Franck P. Vidal, Bangor University, All rights reserved.
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
*   @file       StereoHelper.cxx
*
*   @brief      Class to handle stereo vision using OpenGL's quad buffer.
*               The code is relying on // http://www.orthostereo.com/geometryopengl.html
*
*   @version	1.0
*
*   @date       17/04/2024
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
*   (c) by Dr Franck P. Vidal, Bangor University, Fev 2014-Sept 2023, version 1.1, BSD 3-Clause License
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

#ifndef GVXR_STEREO_HELPER_H
#include "gVirtualXRay/StereoHelper.h"
#endif

#ifndef GVXR_OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif


//******************************************************************************
//	Name space
//******************************************************************************
using namespace gVirtualXRay;


//---------------------------------------
StereoHelper::StereoHelper():
//---------------------------------------
		m_intraocular_distance(4.5),
		m_near(0.01),
		m_far(5000.0),
		m_fovy(45),
		m_screen_aspect_ratio(1),
		m_screen_projection_plane(10000),
		m_matrices_are_up_to_date(false),
		m_current_eye(MONO),
		m_stereo_currently_in_use(false)
//---------------------------------------
{
	m_left_eye_frustum[0] = 0;
	m_left_eye_frustum[1] = 0;
	m_left_eye_frustum[2] = 0;
	m_left_eye_frustum[3] = 0;

	m_right_eye_frustum[0] = 0;
	m_right_eye_frustum[1] = 0;
	m_right_eye_frustum[2] = 0;
	m_right_eye_frustum[3] = 0;
}


//-------------------------
void StereoHelper::update()
//-------------------------
{
	const double degree_to_radian(Pi / 180.0);

	// Sets top of frustum based on fovy and near clipping plane
	double top = m_near * tan(degree_to_radian * m_fovy / 2.0);
	double right = m_screen_aspect_ratio * top;                             //sets right of frustum based on aspect ratio
	double frustum_shift = (m_intraocular_distance / 2.0) * m_near / m_screen_projection_plane;

	m_left_eye_frustum[0] = -right + frustum_shift;
	m_left_eye_frustum[1] =  right + frustum_shift;
	m_left_eye_frustum[2] = -top;
	m_left_eye_frustum[3] =  top;

	double left_camera_translation(m_intraocular_distance / 2.0);

	m_left_eye_projection_matrix = buildFrustumProjectionMatrix(m_left_eye_frustum[0],
			m_left_eye_frustum[1],
			m_left_eye_frustum[2],
			m_left_eye_frustum[3],
			m_near,
			m_far);

	m_left_eye_projection_matrix *= MATRIX4::buildTranslationMatrix(left_camera_translation, 0, 0);

	m_right_eye_frustum[0] = -right - frustum_shift;
	m_right_eye_frustum[1] =  right - frustum_shift;
	m_right_eye_frustum[2] = -top;
	m_right_eye_frustum[3] =  top;

	double right_camera_translation(-left_camera_translation);

	m_right_eye_projection_matrix = buildFrustumProjectionMatrix(m_right_eye_frustum[0],
			m_right_eye_frustum[1],
			m_right_eye_frustum[2],
			m_right_eye_frustum[3],
			m_near,
			m_far);

	m_right_eye_projection_matrix *= MATRIX4::buildTranslationMatrix(right_camera_translation, 0, 0);

	m_matrices_are_up_to_date = true;
}


//--------------------------
void StereoHelper::swapEye()
//--------------------------
{
	// Change the eye
	switch (m_current_eye)
	{
	case LEFT_EYE:
		m_current_eye = RIGHT_EYE;
		break;

	case RIGHT_EYE:
		m_current_eye = LEFT_EYE;
		break;

	default:
		if (m_stereo_currently_in_use)
		{
			m_current_eye = RIGHT_EYE;
		}
		else
		{
			m_current_eye = MONO;
		}
		break;
	}

	// Change the back buffer and the projection matrix
	switch (m_current_eye)
	{
	case LEFT_EYE:
		g_current_projection_matrix = getLeftEyeProjectionMatrix();
		if (m_stereo_currently_in_use)
		{
			glDrawBuffer(GL_BACK_LEFT);
		}
		break;

	case RIGHT_EYE:
		g_current_projection_matrix = getRightEyeProjectionMatrix();
		if (m_stereo_currently_in_use)
		{
			glDrawBuffer(GL_BACK_RIGHT);
		}
		break;

	case MONO:
	default:
		glDrawBuffer(GL_BACK);
		break;
	}
}


//-------------------------
bool StereoHelper::enable()
//-------------------------
{
	// Stereo is disable
	if (!m_stereo_currently_in_use)
	{
		// Check the stereo
		GLboolean can_use_stereo;
		glGetBooleanv(GL_STEREO, &can_use_stereo);

		// It is enable on the system
		if (can_use_stereo)
		{
			// Enable the use of stereo
			m_stereo_currently_in_use = true;
		}
		// It is not enable on the system
		else
		{
			disable();
		}
	}

	return (m_stereo_currently_in_use);
}


//--------------------------
void StereoHelper::disable()
//--------------------------
{
	// Stereo is enable
	if (m_stereo_currently_in_use)
	{
		m_current_eye = MONO;
		m_stereo_currently_in_use = false;
	}
}
