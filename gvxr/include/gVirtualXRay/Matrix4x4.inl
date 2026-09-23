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
*   @file       Matrix4x4.inl
*
*   @brief      Template class to handle 4x4 matrices.
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
*   (c) by Dr Franck P. Vidal, Bangor University, Nov 2013-Sept 2023, version 1.1, BSD 3-Clause License
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, Oct 2023-present, version 1.1, BSD 3-Clause License

*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
#ifndef GVXR_CONSTANT_VALUES_H
#include "gVirtualXRay/ConstantValues.h"
#endif

#include <iostream>
#include <cmath>
#include <cstring>

#ifndef GVXR_EXCEPTION_H
#include "gVirtualXRay/Exception.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//----------------------------------------------------------------------------------------------
template<typename T> Matrix4x4<T>::Matrix4x4(const T& a1, const T& a2, const T& a3, const T& a4,
		const T& a5, const T& a6, const T& a7, const T& a8,
		const T& a9, const T& a10, const T& a11, const T& a12,
		const T& a13, const T& a14, const T& a15, const T& a16):
//----------------------------------------------------------------------------------------------
		m_small_matrix_up_to_date(false)
//----------------------------------------------------------------------------------------------
{
	m_p_4x4_data[0]  = a1;
	m_p_4x4_data[1]  = a2;
	m_p_4x4_data[2]  = a3;
	m_p_4x4_data[3]  = a4;
	m_p_4x4_data[4]  = a5;
	m_p_4x4_data[5]  = a6;
	m_p_4x4_data[6]  = a7;
	m_p_4x4_data[7]  = a8;
	m_p_4x4_data[8]  = a9;
	m_p_4x4_data[9]  = a10;
	m_p_4x4_data[10] = a11;
	m_p_4x4_data[11] = a12;
	m_p_4x4_data[12] = a13;
	m_p_4x4_data[13] = a14;
	m_p_4x4_data[14] = a15;
	m_p_4x4_data[15] = a16;

	for (unsigned int i = 0; i < 9; ++i)
		m_p_3x3_data[i] = 0;
}


//---------------------------------------------------------------------
template<typename T> Matrix4x4<T>::Matrix4x4(const Matrix4x4& aMatrix):
//---------------------------------------------------------------------
		m_small_matrix_up_to_date(aMatrix.m_small_matrix_up_to_date)
//---------------------------------------------------------------------
{
	memcpy(m_p_4x4_data, aMatrix.m_p_4x4_data, sizeof(m_p_4x4_data));
	memcpy(m_p_3x3_data, aMatrix.m_p_3x3_data, sizeof(m_p_3x3_data));
}


//--------------------------------------------------------------
template<typename T> Matrix4x4<T>::Matrix4x4(const T* apMatrix):
//--------------------------------------------------------------
		m_small_matrix_up_to_date(false)
//--------------------------------------------------------------
{
	memcpy(m_p_4x4_data, apMatrix, sizeof(m_p_4x4_data));

	for (unsigned int i = 0; i < 9; ++i)
		m_p_3x3_data[i] = 0;
}


//--------------------------------------------------------------------------------
template<typename T> Matrix4x4<T>::Matrix4x4(const std::vector<double>& apMatrix):
//--------------------------------------------------------------------------------
		m_small_matrix_up_to_date(false)
//--------------------------------------------------------------------------------
{
	if (apMatrix.size() != 16)
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "The input matrix is not 4x4.");
	}

	for (unsigned int i = 0; i < 16; ++i)
		m_p_4x4_data[i] = apMatrix[i];

	for (unsigned int i = 0; i < 9; ++i)
		m_p_3x3_data[i] = 0;
}


//----------------------------------------------------
template<typename T> void Matrix4x4<T>::loadIdentity()
//----------------------------------------------------
{
	m_p_4x4_data[0]  = 1.0;
	m_p_4x4_data[1]  = 0.0;
	m_p_4x4_data[2]  = 0.0;
	m_p_4x4_data[3]  = 0.0;

	m_p_4x4_data[4]  = 0.0;
	m_p_4x4_data[5]  = 1.0;
	m_p_4x4_data[6]  = 0.0;
	m_p_4x4_data[7]  = 0.0;

	m_p_4x4_data[8]  = 0.0;
	m_p_4x4_data[9]  = 0.0;
	m_p_4x4_data[10] = 1.0;
	m_p_4x4_data[11] = 0.0;

	m_p_4x4_data[12] = 0.0;
	m_p_4x4_data[13] = 0.0;
	m_p_4x4_data[14] = 0.0;
	m_p_4x4_data[15] = 1.0;

	m_small_matrix_up_to_date = false;
}


//----------------------------------------------------------------------------------------
template<typename T>void  Matrix4x4<T>::setFromVector(const std::vector<double>& apMatrix)
//----------------------------------------------------------------------------------------
{
	if (apMatrix.size() != 16)
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "The input matrix is not 4x4.");
	}

	m_small_matrix_up_to_date = false;

	for (unsigned int i = 0; i < 16; ++i)
		m_p_4x4_data[i] = apMatrix[i];
}


//------------------------------------------------------------------------------------------
template<typename T> void Matrix4x4<T>::rotate(const double anAngle, const Vec3<T>& aVector)
//------------------------------------------------------------------------------------------
{
	*this *= buildRotationMatrix(anAngle, aVector);

	m_small_matrix_up_to_date = false;
}


//----------------------------------------------------------------------
template<typename T> void Matrix4x4<T>::yawPitchRollRotate(double yaw,
	                                                         double pitch,
																													 double roll)
//----------------------------------------------------------------------
{
	*this *= buildYawPitchRollRotationMatrix(yaw, pitch, roll);

	m_small_matrix_up_to_date = false;
}


//-----------------------------------------------------------------------
template<typename T> void Matrix4x4<T>::translate(const Vec3<T>& aVector)
//-----------------------------------------------------------------------
{
	*this *= buildTranslationMatrix(aVector);

	m_small_matrix_up_to_date = false;
}


//--------------------------------------------------------------
template<typename T> void Matrix4x4<T>::translate(T x, T y, T z)
//--------------------------------------------------------------
{
	*this *= buildTranslationMatrix(x, y, z);

	m_small_matrix_up_to_date = false;
}


//-------------------------------------------------------------------
template<typename T> Matrix4x4<T> Matrix4x4<T>::buildIdentityMatrix()
//-------------------------------------------------------------------
{
    return (Matrix4x4<T>(
            1.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0));
}


//--------------------------------------------------------------------------------------------
template<typename T> Matrix4x4<T> Matrix4x4<T>::buildTranslationMatrix(const Vec3<T>& aVector)
//--------------------------------------------------------------------------------------------
{
	return (buildTranslationMatrix(aVector.getX(), aVector.getY(), aVector.getZ()));
}


//-----------------------------------------------------------------------------------
template<typename T> Matrix4x4<T> Matrix4x4<T>::buildTranslationMatrix(T x, T y, T z)
//-----------------------------------------------------------------------------------
{
	return (Matrix4x4<T>(
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			  x,   y,   z, 1.0));
}

//--------------------------------------------------------------------------------------------
template<typename T> Matrix4x4<T> Matrix4x4<T>::buildScaleMatrix( const Vec3<T>& aVector )
//--------------------------------------------------------------------------------------------
{
  return ( buildScaleMatrix( aVector.getX( ), aVector.getY( ), aVector.getZ( ) ) );
}


//-----------------------------------------------------------------------------------
template<typename T> Matrix4x4<T> Matrix4x4<T>::buildScaleMatrix( T x, T y, T z )
//-----------------------------------------------------------------------------------
{
	// Make sure not all the scaling factors are null
	Vec3<T> temp(Vec3<T>(x, y, z));
	if (temp.length() < 1e-9)
	{
		std::stringstream error_message;
		error_message << "Invalid scaling factors, all of them are null: " << temp;
		throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
	}

  return ( Matrix4x4<T>(
    x, 0.0, 0.0, 0.0,
    0.0, y, 0.0, 0.0,
    0.0, 0.0, z, 0.0,
    0.0, 0.0, 0.0, 1.0 ) );
}



//-----------------------------------------------------------------------------------------
template<typename T> Matrix4x4<T> Matrix4x4<T>::buildRotationMatrix(T anAngle,
																	const Vec3<T>& aVector)
//-----------------------------------------------------------------------------------------
{
	return (buildRotationMatrix(anAngle, aVector.getX(), aVector.getY(), aVector.getZ()));
}


//--------------------------------------------------------------------------------
template<typename T> Matrix4x4<T> Matrix4x4<T>::buildRotationMatrix(T anAngle,
																	T x, T y, T z)
//--------------------------------------------------------------------------------
{
	Vec3<T> temp_rotation_axis(Vec3<T>(x, y, z));

	// Make sure the length of the vector is not null
	if (temp_rotation_axis.length() < 1e-9)
	{
		std::stringstream error_message;
		error_message << "Invalid rotation axis, (" << temp_rotation_axis << ")'s length is zero.";
		throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
	}

	Vec3<T> rotation_axis(temp_rotation_axis.normal());
	double angle(Pi * anAngle / 180.0);
	double c(cos(angle));
	double s(sin(angle));
	double one_minus_cos(1.0 - c);
	double xy(rotation_axis.getX() * rotation_axis.getY());
	double xz(rotation_axis.getX() * rotation_axis.getZ());
	double yz(rotation_axis.getY() * rotation_axis.getZ());

	Matrix4x4<T> rotation_matrix(
		rotation_axis.getX() * rotation_axis.getX() * (1.0 - c) + c,
		xy * one_minus_cos + rotation_axis.getZ() * s,
		xz * one_minus_cos - rotation_axis.getY() * s,
		0.0,

		xy * one_minus_cos - rotation_axis.getZ() * s,
		rotation_axis.getY() * rotation_axis.getY() * one_minus_cos + c,
		yz * one_minus_cos + rotation_axis.getX() * s,
		0.0,

		xz * one_minus_cos + rotation_axis.getY() * s,
		yz * one_minus_cos - rotation_axis.getX() * s,
		rotation_axis.getZ() * rotation_axis.getZ() * one_minus_cos + c,
		0.0,

		0.0,
		0.0,
		0.0,
		1.0);

	return (rotation_matrix);
}


//-----------------------------------------------------------------------------------------------------
template<typename T> Matrix4x4<T> Matrix4x4<T>::buildYawPitchRollRotationMatrix(T yaw, T pitch, T roll)
//-----------------------------------------------------------------------------------------------------
{
	T alpha = Pi * yaw / 180.0;
	T cos_alpha = cos(alpha);
	T sin_alpha = sin(alpha);

	T beta = Pi * pitch / 180.0;
	T cos_beta = cos(beta);
	T sin_beta = sin(beta);

	T gamma = Pi * roll / 180.0;
	T cos_gamma = cos(gamma);
	T sin_gamma = sin(gamma);


	Matrix4x4<T> rotation_matrix(
		cos_alpha * cos_beta,
		sin_alpha * cos_beta,
		-sin_beta,
		0.0,

		cos_alpha * sin_beta * sin_gamma - sin_alpha * cos_gamma,
		sin_alpha * sin_beta * sin_gamma + cos_alpha * cos_gamma,
		cos_beta * sin_gamma,
		0.0,

		cos_alpha * sin_beta * cos_gamma + sin_alpha * sin_gamma,
		sin_alpha * sin_beta * cos_gamma - cos_alpha * sin_gamma,
		cos_beta * cos_gamma,
		0.0,

		0.0,
		0.0,
		0.0,
		1.0);

	return (rotation_matrix);
}



//----------------------------------------------------------
template<typename T> void Matrix4x4<T>::scale(T x, T y, T z)
//----------------------------------------------------------
{
	// Make sure not all the scaling factors are null
	Vec3<T> temp(Vec3<T>(x, y, z));
	if (temp.length() < 1e-9)
	{
		std::stringstream error_message;
		error_message << "Invalid scaling factors, all of them are null: " << temp;
		throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
	}

	Matrix4x4<T> scaling_matrix(
			  x, 0.0, 0.0, 0.0,
			0.0,   y, 0.0, 0.0,
			0.0, 0.0,   z, 0.0,
			0.0, 0.0, 0.0, 1.0);

	*this *= scaling_matrix;

	m_small_matrix_up_to_date = false;
}


//------------------------------------------------------------------
template<typename T> Matrix4x4<T> Matrix4x4<T>::getTranspose() const
//------------------------------------------------------------------
{
	return Matrix4x4<T>(
		m_p_4x4_data[0], m_p_4x4_data[4],  m_p_4x4_data[8], m_p_4x4_data[12],
		m_p_4x4_data[1], m_p_4x4_data[5],  m_p_4x4_data[9], m_p_4x4_data[13],
		m_p_4x4_data[2], m_p_4x4_data[6], m_p_4x4_data[10], m_p_4x4_data[14],
		m_p_4x4_data[3], m_p_4x4_data[7], m_p_4x4_data[11], m_p_4x4_data[15]);
}


 //-----------------------------------------------------
template<typename T> T Matrix4x4<T>::determinant() const
//------------------------------------------------------
{
	return
		m_p_4x4_data[3] * m_p_4x4_data[6] * m_p_4x4_data[9]  * m_p_4x4_data[12] - m_p_4x4_data[2] * m_p_4x4_data[7] * m_p_4x4_data[9]  * m_p_4x4_data[12] - m_p_4x4_data[3] * m_p_4x4_data[5] * m_p_4x4_data[10] * m_p_4x4_data[12]+m_p_4x4_data[1] * m_p_4x4_data[7] * m_p_4x4_data[10] * m_p_4x4_data[12]+
      m_p_4x4_data[2] * m_p_4x4_data[5] * m_p_4x4_data[11] * m_p_4x4_data[12] - m_p_4x4_data[1] * m_p_4x4_data[6] * m_p_4x4_data[11] * m_p_4x4_data[12] - m_p_4x4_data[3] * m_p_4x4_data[6] * m_p_4x4_data[8]  * m_p_4x4_data[13]+m_p_4x4_data[2] * m_p_4x4_data[7] * m_p_4x4_data[8]  * m_p_4x4_data[13]+
      m_p_4x4_data[3] * m_p_4x4_data[4] * m_p_4x4_data[10] * m_p_4x4_data[13] - m_p_4x4_data[0] * m_p_4x4_data[7] * m_p_4x4_data[10] * m_p_4x4_data[13] - m_p_4x4_data[2] * m_p_4x4_data[4] * m_p_4x4_data[11] * m_p_4x4_data[13]+m_p_4x4_data[0] * m_p_4x4_data[6] * m_p_4x4_data[11] * m_p_4x4_data[13]+
      m_p_4x4_data[3] * m_p_4x4_data[5] * m_p_4x4_data[8]  * m_p_4x4_data[14] - m_p_4x4_data[1] * m_p_4x4_data[7] * m_p_4x4_data[8]  * m_p_4x4_data[14] - m_p_4x4_data[3] * m_p_4x4_data[4] * m_p_4x4_data[9]  * m_p_4x4_data[14]+m_p_4x4_data[0] * m_p_4x4_data[7] * m_p_4x4_data[9]  * m_p_4x4_data[14]+
      m_p_4x4_data[1] * m_p_4x4_data[4] * m_p_4x4_data[11] * m_p_4x4_data[14] - m_p_4x4_data[0] * m_p_4x4_data[5] * m_p_4x4_data[11] * m_p_4x4_data[14] - m_p_4x4_data[2] * m_p_4x4_data[5] * m_p_4x4_data[8]  * m_p_4x4_data[15]+m_p_4x4_data[1] * m_p_4x4_data[6] * m_p_4x4_data[8]  * m_p_4x4_data[15]+
      m_p_4x4_data[2] * m_p_4x4_data[4] * m_p_4x4_data[9]  * m_p_4x4_data[15] - m_p_4x4_data[0] * m_p_4x4_data[6] * m_p_4x4_data[9]  * m_p_4x4_data[15] - m_p_4x4_data[1] * m_p_4x4_data[4] * m_p_4x4_data[10] * m_p_4x4_data[15]+m_p_4x4_data[0] * m_p_4x4_data[5] * m_p_4x4_data[10] * m_p_4x4_data[15];
}


//----------------------------------------------------------------
template<typename T> Matrix4x4<T> Matrix4x4<T>::getInverse() const
//----------------------------------------------------------------
{
	T m0(0), m1(0), m2(0), m3(0), m4(0), m5(0), m6(0), m7(0), m8(0), m9(0), m10(0), m11(0), m12(0), m13(0), m14(0), m15(0);
	T matrix_determinant(determinant());

	if (fabs(matrix_determinant) > 0.000001)
	{
		m0  = (m_p_4x4_data[6]*m_p_4x4_data[11]*m_p_4x4_data[13] - m_p_4x4_data[7]*m_p_4x4_data[10]*m_p_4x4_data[13] + m_p_4x4_data[7]*m_p_4x4_data[9]*m_p_4x4_data[14] - m_p_4x4_data[5]*m_p_4x4_data[11]*m_p_4x4_data[14] - m_p_4x4_data[6]*m_p_4x4_data[9]*m_p_4x4_data[15] + m_p_4x4_data[5]*m_p_4x4_data[10]*m_p_4x4_data[15]) / matrix_determinant;
		m1  = (m_p_4x4_data[3]*m_p_4x4_data[10]*m_p_4x4_data[13] - m_p_4x4_data[2]*m_p_4x4_data[11]*m_p_4x4_data[13] - m_p_4x4_data[3]*m_p_4x4_data[9]*m_p_4x4_data[14] + m_p_4x4_data[1]*m_p_4x4_data[11]*m_p_4x4_data[14] + m_p_4x4_data[2]*m_p_4x4_data[9]*m_p_4x4_data[15] - m_p_4x4_data[1]*m_p_4x4_data[10]*m_p_4x4_data[15]) / matrix_determinant;
		m2  = (m_p_4x4_data[2]*m_p_4x4_data[7]*m_p_4x4_data[13] - m_p_4x4_data[3]*m_p_4x4_data[6]*m_p_4x4_data[13] + m_p_4x4_data[3]*m_p_4x4_data[5]*m_p_4x4_data[14] - m_p_4x4_data[1]*m_p_4x4_data[7]*m_p_4x4_data[14] - m_p_4x4_data[2]*m_p_4x4_data[5]*m_p_4x4_data[15] + m_p_4x4_data[1]*m_p_4x4_data[6]*m_p_4x4_data[15]) / matrix_determinant;
		m3  = (m_p_4x4_data[3]*m_p_4x4_data[6]*m_p_4x4_data[9] - m_p_4x4_data[2]*m_p_4x4_data[7]*m_p_4x4_data[9] - m_p_4x4_data[3]*m_p_4x4_data[5]*m_p_4x4_data[10] + m_p_4x4_data[1]*m_p_4x4_data[7]*m_p_4x4_data[10] + m_p_4x4_data[2]*m_p_4x4_data[5]*m_p_4x4_data[11] - m_p_4x4_data[1]*m_p_4x4_data[6]*m_p_4x4_data[11]) / matrix_determinant;
		m4  = (m_p_4x4_data[7]*m_p_4x4_data[10]*m_p_4x4_data[12] - m_p_4x4_data[6]*m_p_4x4_data[11]*m_p_4x4_data[12] - m_p_4x4_data[7]*m_p_4x4_data[8]*m_p_4x4_data[14] + m_p_4x4_data[4]*m_p_4x4_data[11]*m_p_4x4_data[14] + m_p_4x4_data[6]*m_p_4x4_data[8]*m_p_4x4_data[15] - m_p_4x4_data[4]*m_p_4x4_data[10]*m_p_4x4_data[15]) / matrix_determinant;
		m5  = (m_p_4x4_data[2]*m_p_4x4_data[11]*m_p_4x4_data[12] - m_p_4x4_data[3]*m_p_4x4_data[10]*m_p_4x4_data[12] + m_p_4x4_data[3]*m_p_4x4_data[8]*m_p_4x4_data[14] - m_p_4x4_data[0]*m_p_4x4_data[11]*m_p_4x4_data[14] - m_p_4x4_data[2]*m_p_4x4_data[8]*m_p_4x4_data[15] + m_p_4x4_data[0]*m_p_4x4_data[10]*m_p_4x4_data[15]) / matrix_determinant;
		m6  = (m_p_4x4_data[3]*m_p_4x4_data[6]*m_p_4x4_data[12] - m_p_4x4_data[2]*m_p_4x4_data[7]*m_p_4x4_data[12] - m_p_4x4_data[3]*m_p_4x4_data[4]*m_p_4x4_data[14] + m_p_4x4_data[0]*m_p_4x4_data[7]*m_p_4x4_data[14] + m_p_4x4_data[2]*m_p_4x4_data[4]*m_p_4x4_data[15] - m_p_4x4_data[0]*m_p_4x4_data[6]*m_p_4x4_data[15]) / matrix_determinant;
		m7  = (m_p_4x4_data[2]*m_p_4x4_data[7]*m_p_4x4_data[8] - m_p_4x4_data[3]*m_p_4x4_data[6]*m_p_4x4_data[8] + m_p_4x4_data[3]*m_p_4x4_data[4]*m_p_4x4_data[10] - m_p_4x4_data[0]*m_p_4x4_data[7]*m_p_4x4_data[10] - m_p_4x4_data[2]*m_p_4x4_data[4]*m_p_4x4_data[11] + m_p_4x4_data[0]*m_p_4x4_data[6]*m_p_4x4_data[11]) / matrix_determinant;
		m8  = (m_p_4x4_data[5]*m_p_4x4_data[11]*m_p_4x4_data[12] - m_p_4x4_data[7]*m_p_4x4_data[9]*m_p_4x4_data[12] + m_p_4x4_data[7]*m_p_4x4_data[8]*m_p_4x4_data[13] - m_p_4x4_data[4]*m_p_4x4_data[11]*m_p_4x4_data[13] - m_p_4x4_data[5]*m_p_4x4_data[8]*m_p_4x4_data[15] + m_p_4x4_data[4]*m_p_4x4_data[9]*m_p_4x4_data[15]) / matrix_determinant;
		m9  = (m_p_4x4_data[3]*m_p_4x4_data[9]*m_p_4x4_data[12] - m_p_4x4_data[1]*m_p_4x4_data[11]*m_p_4x4_data[12] - m_p_4x4_data[3]*m_p_4x4_data[8]*m_p_4x4_data[13] + m_p_4x4_data[0]*m_p_4x4_data[11]*m_p_4x4_data[13] + m_p_4x4_data[1]*m_p_4x4_data[8]*m_p_4x4_data[15] - m_p_4x4_data[0]*m_p_4x4_data[9]*m_p_4x4_data[15]) / matrix_determinant;
		m10 = (m_p_4x4_data[1]*m_p_4x4_data[7]*m_p_4x4_data[12] - m_p_4x4_data[3]*m_p_4x4_data[5]*m_p_4x4_data[12] + m_p_4x4_data[3]*m_p_4x4_data[4]*m_p_4x4_data[13] - m_p_4x4_data[0]*m_p_4x4_data[7]*m_p_4x4_data[13] - m_p_4x4_data[1]*m_p_4x4_data[4]*m_p_4x4_data[15] + m_p_4x4_data[0]*m_p_4x4_data[5]*m_p_4x4_data[15]) / matrix_determinant;
		m11 = (m_p_4x4_data[3]*m_p_4x4_data[5]*m_p_4x4_data[8] - m_p_4x4_data[1]*m_p_4x4_data[7]*m_p_4x4_data[8] - m_p_4x4_data[3]*m_p_4x4_data[4]*m_p_4x4_data[9] + m_p_4x4_data[0]*m_p_4x4_data[7]*m_p_4x4_data[9] + m_p_4x4_data[1]*m_p_4x4_data[4]*m_p_4x4_data[11] - m_p_4x4_data[0]*m_p_4x4_data[5]*m_p_4x4_data[11]) / matrix_determinant;
		m12 = (m_p_4x4_data[6]*m_p_4x4_data[9]*m_p_4x4_data[12] - m_p_4x4_data[5]*m_p_4x4_data[10]*m_p_4x4_data[12] - m_p_4x4_data[6]*m_p_4x4_data[8]*m_p_4x4_data[13] + m_p_4x4_data[4]*m_p_4x4_data[10]*m_p_4x4_data[13] + m_p_4x4_data[5]*m_p_4x4_data[8]*m_p_4x4_data[14] - m_p_4x4_data[4]*m_p_4x4_data[9]*m_p_4x4_data[14]) / matrix_determinant;
		m13 = (m_p_4x4_data[1]*m_p_4x4_data[10]*m_p_4x4_data[12] - m_p_4x4_data[2]*m_p_4x4_data[9]*m_p_4x4_data[12] + m_p_4x4_data[2]*m_p_4x4_data[8]*m_p_4x4_data[13] - m_p_4x4_data[0]*m_p_4x4_data[10]*m_p_4x4_data[13] - m_p_4x4_data[1]*m_p_4x4_data[8]*m_p_4x4_data[14] + m_p_4x4_data[0]*m_p_4x4_data[9]*m_p_4x4_data[14]) / matrix_determinant;
		m14 = (m_p_4x4_data[2]*m_p_4x4_data[5]*m_p_4x4_data[12] - m_p_4x4_data[1]*m_p_4x4_data[6]*m_p_4x4_data[12] - m_p_4x4_data[2]*m_p_4x4_data[4]*m_p_4x4_data[13] + m_p_4x4_data[0]*m_p_4x4_data[6]*m_p_4x4_data[13] + m_p_4x4_data[1]*m_p_4x4_data[4]*m_p_4x4_data[14] - m_p_4x4_data[0]*m_p_4x4_data[5]*m_p_4x4_data[14]) / matrix_determinant;
		m15 = (m_p_4x4_data[1]*m_p_4x4_data[6]*m_p_4x4_data[8] - m_p_4x4_data[2]*m_p_4x4_data[5]*m_p_4x4_data[8] + m_p_4x4_data[2]*m_p_4x4_data[4]*m_p_4x4_data[9] - m_p_4x4_data[0]*m_p_4x4_data[6]*m_p_4x4_data[9] - m_p_4x4_data[1]*m_p_4x4_data[4]*m_p_4x4_data[10] + m_p_4x4_data[0]*m_p_4x4_data[5]*m_p_4x4_data[10]) / matrix_determinant;
	}

	return (Matrix4x4<T>(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15));
}


//----------------------------------------------------------------------------------
template<typename T> Matrix4x4<T>& Matrix4x4<T>::operator=(const Matrix4x4& aMatrix)
//----------------------------------------------------------------------------------
{
    // Prevent self assignment
    if (this != &aMatrix)
	{
		memcpy(m_p_4x4_data, aMatrix.m_p_4x4_data, sizeof(m_p_4x4_data));
		memcpy(m_p_3x3_data, aMatrix.m_p_3x3_data, sizeof(m_p_3x3_data));

		m_small_matrix_up_to_date = aMatrix.m_small_matrix_up_to_date;
	}

	return (*this);
}


//--------------------------------------------------------------------------------
template<typename T> Vec3<T> Matrix4x4<T>::operator*(const Vec3<T>& aVector) const
//--------------------------------------------------------------------------------
{
	T x, y, z, w;
	x = m_p_4x4_data[0] * aVector.getX() + m_p_4x4_data[4] * aVector.getY() + m_p_4x4_data[8]  * aVector.getZ() + m_p_4x4_data[12];
	y = m_p_4x4_data[1] * aVector.getX() + m_p_4x4_data[5] * aVector.getY() + m_p_4x4_data[9]  * aVector.getZ() + m_p_4x4_data[13];
	z = m_p_4x4_data[2] * aVector.getX() + m_p_4x4_data[6] * aVector.getY() + m_p_4x4_data[10] * aVector.getZ() + m_p_4x4_data[14];
	w = m_p_4x4_data[3] * aVector.getX() + m_p_4x4_data[7] * aVector.getY() + m_p_4x4_data[11] * aVector.getZ() + m_p_4x4_data[15];
	return (Vec3<T>(x / w, y / w, z / w));
}


//------------------------------------------------------------------------------------------
template<typename T> Matrix4x4<T> Matrix4x4<T>::operator*(const Matrix4x4<T>& aMatrix) const
//------------------------------------------------------------------------------------------
{
	Matrix4x4<T> result;

	T* p_matrix1(const_cast<T*>(aMatrix.m_p_4x4_data));
	T* p_result(result.m_p_4x4_data);

	// Process each column
	for (int i(0); i < 4; i++, p_matrix1 += 4)
	{
		T* p_matrix0(const_cast<T*>(m_p_4x4_data));

		// Process each line
		for (int j(0); j < 4; j++, p_matrix0++, p_result++)
		{
			T* p_temp(p_matrix1);

			*p_result  = p_matrix0[0]  * *p_temp++;
			*p_result += p_matrix0[4]  * *p_temp++;
			*p_result += p_matrix0[8]  * *p_temp++;
			*p_result += p_matrix0[12] * *p_temp++;
		}
	}

	return (result);
}


//--------------------------------------------------------------------------------------
template<typename T> Matrix4x4<T>& Matrix4x4<T>::operator*=(const Matrix4x4<T>& aMatrix)
//--------------------------------------------------------------------------------------
{
	*this = *this * aMatrix;

	m_small_matrix_up_to_date = false;

	return (*this);
}


//-----------------------------------------------------
template<typename T> const T* Matrix4x4<T>::get() const
//-----------------------------------------------------
{
	return (&m_p_4x4_data[0]);
}


//--------------------------------------------------------
template<typename T> const T* Matrix4x4<T>::get4x4() const
//--------------------------------------------------------
{
	return (&m_p_4x4_data[0]);
}


//--------------------------------------------------
template<typename T> const T* Matrix4x4<T>::get3x3()
//--------------------------------------------------
{
	// The 4x4 matrix has changed, the 3x3 matrix needs to be updated
	if (!m_small_matrix_up_to_date)
	{
		m_p_3x3_data[0] = m_p_4x4_data[ 0];
		m_p_3x3_data[1] = m_p_4x4_data[ 1];
		m_p_3x3_data[2] = m_p_4x4_data[ 2];

		m_p_3x3_data[3] = m_p_4x4_data[ 4];
		m_p_3x3_data[4] = m_p_4x4_data[ 5];
		m_p_3x3_data[5] = m_p_4x4_data[ 6];

		m_p_3x3_data[6] = m_p_4x4_data[ 8];
		m_p_3x3_data[7] = m_p_4x4_data[ 9];
		m_p_3x3_data[8] = m_p_4x4_data[10];

		m_small_matrix_up_to_date = true;
	}

	return (&m_p_3x3_data[0]);
}


//------------------------------------------------------------------------
template<typename T> std::vector<double> Matrix4x4<T>::getAsVector() const
//------------------------------------------------------------------------
{
	std::vector<double> temp;

	for (unsigned int i = 0; i < 16; ++i)
	{
		temp.push_back(m_p_4x4_data[i]);
	}

	return (temp);
}


//---------------------------------------------------
template<typename T> void Matrix4x4<T>::print() const
//---------------------------------------------------
{
	print(std::cout);
}


//-------------------------------------------------------------------------------
template<typename T> void Matrix4x4<T>::print(std::ostream& anOutputStream) const
//-------------------------------------------------------------------------------
{
	anOutputStream << m_p_4x4_data[0] << "  " << m_p_4x4_data[4] << "  " << m_p_4x4_data[ 8] << "  " << m_p_4x4_data[12] << std::endl;
	anOutputStream << m_p_4x4_data[1] << "  " << m_p_4x4_data[5] << "  " << m_p_4x4_data[ 9] << "  " << m_p_4x4_data[13] << std::endl;
	anOutputStream << m_p_4x4_data[2] << "  " << m_p_4x4_data[6] << "  " << m_p_4x4_data[10] << "  " << m_p_4x4_data[14] << std::endl;
	anOutputStream << m_p_4x4_data[3] << "  " << m_p_4x4_data[7] << "  " << m_p_4x4_data[11] << "  " << m_p_4x4_data[15] << std::endl;
}


//--------------------------------------------------------------------------
template <typename T> std::ostream& operator<<(std::ostream& anOutputStream,
											   const Matrix4x4<T>& aMatrix)
//--------------------------------------------------------------------------
{
	aMatrix.print(anOutputStream);
	return (anOutputStream);
}


} // namespace gVirtualXRay
