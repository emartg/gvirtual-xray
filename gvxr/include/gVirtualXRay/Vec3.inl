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
 *******************************************************************************
 *
 *  @file       Vec3.inl
 *
 *  @brief      Template class to handle 3D vectors.
 *
 *  @todo       Add an exception in Length().
 *              Add a header for constants, use EPSILON instead of 0.0000001
 *
 *  @version    1.0
 *
 *  @date       11/11/2013
 *
 *  @author     Dr Franck P. Vidal
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
 *******************************************************************************
 */


//******************************************************************************
//	Include
//******************************************************************************
#include <cmath>

#ifndef GVXR_OUT_OF_BOUNDS_EXCEPTION_H
#include "gVirtualXRay/OutOfBoundsException.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//------------------------------------------------------------------------
template<typename T> Vec3<T>::Vec3(const T& aX, const T& aY, const T& aZ):
//------------------------------------------------------------------------
        m_x(aX), m_y(aY), m_z(aZ)
//------------------------------------------------------------------------
{
}


//------------------------------------------------------
template<typename T> void Vec3<T>::setX(const T& aValue)
//------------------------------------------------------
{
    m_x = aValue;
}


//------------------------------------------------------
template<typename T> void Vec3<T>::setY(const T& aValue)
//------------------------------------------------------
{
    m_y = aValue;
}


//------------------------------------------------------
template<typename T> void Vec3<T>::setZ(const T& aValue)
//------------------------------------------------------
{
    m_z = aValue;
}


//------------------------------------------
template<typename T> T Vec3<T>::getX() const
//------------------------------------------
{
    return (m_x);
}


//------------------------------------------
template<typename T> T Vec3<T>::getY() const
//------------------------------------------
{
    return (m_y);
}


//------------------------------------------
template<typename T> T Vec3<T>::getZ() const
//------------------------------------------
{
    return (m_z);
}


//-------------------------------------------------
template<typename T> double Vec3<T>::length() const
//-------------------------------------------------
{
    return (sqrt(m_x * m_x + m_y * m_y + m_z * m_z));
}


//--------------------------------------------------
template<typename T> Vec3<T> Vec3<T>::normal() const
//--------------------------------------------------
{
    return (Vec3(m_x, m_y, m_z) / length());
}


//--------------------------------------------
template<typename T> void Vec3<T>::normalize()
//--------------------------------------------
{
    double vector_length(length());
    m_x /= vector_length;
    m_y /= vector_length;
    m_z /= vector_length;
}


//--------------------------------------------
template<typename T> void Vec3<T>::normalise()
//--------------------------------------------
{
    return (normalize());
}


//------------------------------------------------------------------------
template<typename T> double Vec3<T>::dotProduct(const Vec3& aVector) const
//------------------------------------------------------------------------
{
    return (m_x * aVector.m_x + m_y * aVector.m_y + m_z * aVector.m_z);
}


//------------------------------------------------------------------------------
template<typename T> Vec3<T> Vec3<T>::crossProduct(const Vec3<T>& aVector) const
//------------------------------------------------------------------------------
{
    return (Vec3(  m_y * aVector.m_z - m_z * aVector.m_y,
                 -(m_x * aVector.m_z - m_z * aVector.m_x),
                   m_x * aVector.m_y - m_y * aVector.m_x));
}


/*
//-----------------------------------------------------------------------
template<typename T> double Vec3<T>::operator*(const Vec3& aVector) const
//-----------------------------------------------------------------------
{
    return (m_x * aVector.m_x + m_y * aVector.m_y + m_z * aVector.m_z);
}*/


//---------------------------------------------------------------------------------
template<typename T> Vec3<T> Vec3<T>::elementWiseProduct(const Vec3& aVector) const
//---------------------------------------------------------------------------------
{
    return (Vec3(m_x * aVector.m_x, m_y * aVector.m_y, m_z * aVector.m_z));
}


//----------------------------------------------------------------------
template<typename T> double Vec3<T>::distance(const Vec3& aVector) const
//----------------------------------------------------------------------
{
    return (sqrt(
            pow(m_x - aVector.m_x, 2.0) +
            pow(m_y - aVector.m_y, 2.0) +
            pow(m_z - aVector.m_z, 2.0)
        )
    );
}


//-----------------------------------------------------------------------
template<typename T> Vec3<T> Vec3<T>::operator+(const Vec3& aValue) const
//-----------------------------------------------------------------------
{
    return (Vec3(m_x + aValue.m_x, m_y + aValue.m_y, m_z + aValue.m_z));
}


//--------------------------------------------------------------------
template<typename T> Vec3<T>& Vec3<T>::operator+=(const Vec3& aValue)
//--------------------------------------------------------------------
{
    m_x += aValue.m_x;
    m_y += aValue.m_y;
    m_z += aValue.m_z;

    return (*this);
}


//-----------------------------------------------------------------------
template<typename T> Vec3<T> Vec3<T>::operator-(const Vec3& aValue) const
//-----------------------------------------------------------------------
{
    return (Vec3(m_x - aValue.m_x, m_y - aValue.m_y, m_z - aValue.m_z));
}


//-------------------------------------------------------------------
template<typename T> Vec3<T>& Vec3<T>::operator-=(const Vec3& aValue)
//-------------------------------------------------------------------
{
    m_x -= aValue.m_x;
    m_y -= aValue.m_y;
    m_z -= aValue.m_z;

    return (*this);
}


//-------------------------------------------------------------------------
template<typename T> Vec3<T> Vec3<T>::operator*(const double& aValue) const
//-------------------------------------------------------------------------
{
    return (Vec3(m_x * aValue, m_y * aValue, m_z * aValue));
}


//---------------------------------------------------------------------
template<typename T> Vec3<T>& Vec3<T>::operator*=(const double& aValue)
//---------------------------------------------------------------------
{
    m_x *= aValue;
    m_y *= aValue;
    m_z *= aValue;

    return (*this);
}


//------------------------------------------------------------------------
template<typename T> Vec3<T> Vec3<T>::operator*(const float& aValue) const
//------------------------------------------------------------------------
{
    return (Vec3(m_x * aValue, m_y * aValue, m_z * aValue));
}


//--------------------------------------------------------------------
template<typename T> Vec3<T>& Vec3<T>::operator*=(const float& aValue)
//--------------------------------------------------------------------
{
    m_x *= aValue;
    m_y *= aValue;
    m_z *= aValue;

    return (*this);
}


//-------------------------------------------------------------------------
template<typename T> Vec3<T> Vec3<T>::operator/(const double& aValue) const
//-------------------------------------------------------------------------
{
    return (Vec3(m_x / aValue, m_y / aValue, m_z / aValue));
}


//---------------------------------------------------------------------
template<typename T> Vec3<T>& Vec3<T>::operator/=(const double& aValue)
//---------------------------------------------------------------------
{
    m_x /= aValue;
    m_y /= aValue;
    m_z /= aValue;

    return (*this);
}


//------------------------------------------------------------------------
template<typename T> Vec3<T> Vec3<T>::operator/(const float& aValue) const
//------------------------------------------------------------------------
{
    return (Vec3(m_x / aValue, m_y / aValue, m_z / aValue));
}


//--------------------------------------------------------------------
template<typename T> Vec3<T>& Vec3<T>::operator/=(const float& aValue)
//--------------------------------------------------------------------
{
    m_x /= aValue;
    m_y /= aValue;
    m_z /= aValue;

    return (*this);
}


//-----------------------------------------------------------------------
template<typename T> Vec3<T> Vec3<T>::operator^(const Vec3& aValue) const
//-----------------------------------------------------------------------
{
	return (crossProduct(aValue));
}


//-----------------------------------------------------------
template<typename T> Vec3<T> Vec3<T>::operator-() const
//-----------------------------------------------------------
{
    return (Vec3(-m_x, -m_y, -m_z));
}


//-------------------------------------------------------------------------
template<typename T> bool Vec3<T>::operator==(const Vec3<T>& aVector) const
//-------------------------------------------------------------------------
{
    bool return_value(
            std::abs(m_x - aVector.m_x) < 0.0000001 &&
            std::abs(m_y - aVector.m_y) < 0.0000001 &&
            std::abs(m_z - aVector.m_z) < 0.0000001);

    return (return_value);
}


//-------------------------------------------------------------------------
template<typename T> bool Vec3<T>::operator!=(const Vec3<T>& aVector) const
//-------------------------------------------------------------------------
{
    bool return_value(
            std::abs(m_x - aVector.m_x) >= 0.0000001 ||
            std::abs(m_y - aVector.m_y) >= 0.0000001 ||
            std::abs(m_z - aVector.m_z) >= 0.0000001);

    return (return_value);
}


//---------------------------------------------------------
template<typename T> T& Vec3<T>::operator()(unsigned int i)
//---------------------------------------------------------
{
    if (i == 0)
        {
        return (m_x);
        }
    else if (i == 1)
        {
        return (m_y);
        }
    else if (i == 2)
        {
        return (m_z);
        }
    else
        {
        throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
        }
}


//---------------------------------------------------------------------
template<typename T> const T& Vec3<T>::operator()(unsigned int i) const
//---------------------------------------------------------------------
{
    if (i == 0)
        {
        return (m_x);
        }
    else if (i == 1)
        {
        return (m_y);
        }
    else if (i == 2)
        {
        return (m_z);
        }
    else
        {
        throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
        }
}


//---------------------------------------------------------
template<typename T> T& Vec3<T>::operator[](unsigned int i)
//---------------------------------------------------------
{
    if (i == 0)
        {
        return (m_x);
        }
    else if (i == 1)
        {
        return (m_y);
        }
    else if (i == 2)
        {
        return (m_z);
        }
    else
        {
        throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
        }
}


//---------------------------------------------------------------------
template<typename T> const T& Vec3<T>::operator[](unsigned int i) const
//---------------------------------------------------------------------
{
    if (i == 0)
        {
        return (m_x);
        }
    else if (i == 1)
        {
        return (m_y);
        }
    else if (i == 2)
        {
        return (m_z);
        }
    else
        {
        throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
        }
}


//-------------------------------------------------------------
template<typename T> Vec3<T> operator*(const double& aValue,
                                       const Vec3<T>& aVector)
//-------------------------------------------------------------
{
    return Vec3<T>(aVector.getX() * aValue, aVector.getY() * aValue, aVector.getZ() * aValue);
}


//-------------------------------------------------------------------------
template<typename T> std::ostream& operator<<(std::ostream& anOutputStream,
                                              const Vec3<T>& aVector)
//-------------------------------------------------------------------------
{
    anOutputStream << aVector.getX() << ", " << aVector.getY() << ", " << aVector.getZ();
    return (anOutputStream);
}


} // namespace gVirtualXRay
