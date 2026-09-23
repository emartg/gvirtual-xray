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
*	@file		StereoHelper.inl
*
*	@brief		Class to handle stereo vision using OpenGL's quad buffer.
*				The code is relying on // http://www.orthostereo.com/geometryopengl.html
*
*	@version	1.0
*
*	@date		14/02/2014
*
*	@author		Dr Franck P. Vidal
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net), 
*   http://www.fpvidal.net/, Dec 2014, 2014, version 1.0, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//----------------------------------------------------------------
inline void StereoHelper::setIntraocularDistance(double aDistance)
//----------------------------------------------------------------
{
	m_intraocular_distance = aDistance;
	m_matrices_are_up_to_date = false;
}


//--------------------------------------------------------
inline double StereoHelper::getIntraocularDistance() const
//--------------------------------------------------------
{
	return (m_intraocular_distance);
}


//-----------------------------------------------------------
inline void StereoHelper::setScreenAspectRatio(double aRatio)
//-----------------------------------------------------------
{
	m_screen_aspect_ratio = aRatio;
	m_matrices_are_up_to_date = false;
}


//-----------------------------------------------------
inline void StereoHelper::setFieldOfViewY(double aFOVY)
//-----------------------------------------------------
{
	m_fovy = aFOVY;
	m_matrices_are_up_to_date = false;
}


//-------------------------------------------------
inline double StereoHelper::getFieldOfViewY() const
//-------------------------------------------------
{
	return (m_fovy);
}


//----------------------------------------------
inline void StereoHelper::setNear(double aPlane)
//----------------------------------------------
{
	m_near = aPlane;
	m_matrices_are_up_to_date = false;
}


//-----------------------------------------
inline double StereoHelper::getNear() const
//-----------------------------------------
{
	return (m_near);
}


//---------------------------------------------
inline void StereoHelper::setFar(double aPlane)
//---------------------------------------------
{
	m_far = aPlane;
	m_matrices_are_up_to_date = false;
}


//----------------------------------------
inline double StereoHelper::getFar() const
//----------------------------------------
{
	return (m_far);
}


//---------------------------------------------------------------
inline void StereoHelper::setScreenProjectionPlane(double aPlane)
//---------------------------------------------------------------
{
	m_screen_projection_plane = aPlane;
	m_matrices_are_up_to_date = false;
}


//--------------------------------------------------------------
inline const MATRIX4& StereoHelper::getLeftEyeProjectionMatrix()
//--------------------------------------------------------------
{
	if (!m_matrices_are_up_to_date)
	{
		update();
	}

	return (m_left_eye_projection_matrix);
}


//---------------------------------------------------------------
inline const MATRIX4& StereoHelper::getRightEyeProjectionMatrix()
//---------------------------------------------------------------
{
	if (!m_matrices_are_up_to_date)
	{
		update();
	}

	return (m_right_eye_projection_matrix);
}


//----------------------------------------
inline bool StereoHelper::isActive() const
//----------------------------------------
{
	return (m_stereo_currently_in_use);
}


} // namespace gVirtualXRay
