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
*	@file		Vec2.inl
*
*	@brief		Template class to handle 2D vectors.
*
*	@version    1.0
*
*	@date		11/11/2013
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


//-----------------------------------------------------------
template<typename T> Vec2<T>::Vec2(const T& aX, const T& aY):
//-----------------------------------------------------------
	m_x(aX),
	m_y(aY)
//-----------------------------------------------------------
{}


//------------------------------------------------------
template<typename T> void Vec2<T>::setX(const T& aValue)
//------------------------------------------------------
{
	m_x = aValue;
}


//------------------------------------------------------
template<typename T> void Vec2<T>::setY(const T& aValue)
//------------------------------------------------------
{
	m_y = aValue;
}


//-------------------------------------------
 template<typename T> T Vec2<T>::getX() const
//-------------------------------------------
{
	return (m_x);
}


//-------------------------------------------
 template<typename T> T Vec2<T>::getY() const
//-------------------------------------------
{
	return (m_y);
}


//--------------------------------------------------
template<typename T> double Vec2<T>::length() const
//--------------------------------------------------
{
	return (sqrt(m_x * m_x + m_y * m_y));
}


//---------------------------------------------------
template<typename T> Vec2<T> Vec2<T>::normal() const
//---------------------------------------------------
{
	return (Vec2(m_x, m_y) / length());
}


//---------------------------------------------
template<typename T> void Vec2<T>::normalize()
//---------------------------------------------
{
	double vector_length(length());
	m_x /= vector_length;
	m_y /= vector_length;
}


//---------------------------------------------
template<typename T> void Vec2<T>::normalise()
//---------------------------------------------
{
	return (normalize());
}


//------------------------------------------------------------------------
template<typename T> double Vec2<T>::dotProduct(const Vec2& aVector) const
//------------------------------------------------------------------------
{
	return (m_x * aVector.m_x + m_y * aVector.m_y);
}


//----------------------------------------------------------------------
template<typename T> double Vec2<T>::distance(const Vec2& aVector) const
//----------------------------------------------------------------------
{
	return (sqrt(
			pow(m_x - aVector.m_x, 2.0) +
			pow(m_y - aVector.m_y, 2.0))
	);
}



//-----------------------------------------------------------------------------
template<typename T> const Vec2<T> Vec2<T>::operator+(const Vec2& aValue) const
//-----------------------------------------------------------------------------
{
	return (Vec2(m_x + aValue.m_x, m_y + aValue.m_y));
}


//--------------------------------------------------------------------
 template<typename T> Vec2<T>& Vec2<T>::operator+=(const Vec2& aValue)
//--------------------------------------------------------------------
{
	m_x += aValue.m_x;
	m_y += aValue.m_y;

	return (*this);
}


//-----------------------------------------------------------------------------
template<typename T> const Vec2<T> Vec2<T>::operator-(const Vec2& aValue) const
//-----------------------------------------------------------------------------
{
	return (Vec2(m_x - aValue.m_x, m_y - aValue.m_y));
}


//-------------------------------------------------------------------
template<typename T> Vec2<T>& Vec2<T>::operator-=(const Vec2& aValue)
//-------------------------------------------------------------------
{
	m_x -= aValue.m_x;
	m_y -= aValue.m_y;

	return (*this);
}


//-------------------------------------------------------------------------------
template<typename T> const Vec2<T> Vec2<T>::operator*(const double& aValue) const
//-------------------------------------------------------------------------------
{
	return (Vec2(m_x * aValue, m_y * aValue));
}


//---------------------------------------------------------------------
template<typename T> Vec2<T>& Vec2<T>::operator*=(const double& aValue)
//---------------------------------------------------------------------
{
	m_x *= aValue;
	m_y *= aValue;

	return (*this);
}


//------------------------------------------------------------------------------
template<typename T> const Vec2<T> Vec2<T>::operator*(const float& aValue) const
//------------------------------------------------------------------------------
{
	return (Vec2(m_x * aValue, m_y * aValue));
}


//--------------------------------------------------------------------
template<typename T> Vec2<T>& Vec2<T>::operator*=(const float& aValue)
//--------------------------------------------------------------------
{
	m_x *= aValue;
	m_y *= aValue;

	return (*this);
}


//-------------------------------------------------------------------------------
template<typename T> const Vec2<T> Vec2<T>::operator/(const double& aValue) const
//-------------------------------------------------------------------------------
{
	return (Vec2(m_x / aValue, m_y / aValue));
}


//---------------------------------------------------------------------
template<typename T> Vec2<T>& Vec2<T>::operator/=(const double& aValue)
//---------------------------------------------------------------------
{
	m_x /= aValue;
	m_y /= aValue;

	return (*this);
}


//------------------------------------------------------------------------------
template<typename T> const Vec2<T> Vec2<T>::operator/(const float& aValue) const
//------------------------------------------------------------------------------
{
	return (Vec2(m_x / aValue, m_y / aValue));
}


//--------------------------------------------------------------------
template<typename T> Vec2<T>& Vec2<T>::operator/=(const float& aValue)
//--------------------------------------------------------------------
{
	m_x /= aValue;
	m_y /= aValue;

	return (*this);
}


//-----------------------------------------------------------
template<typename T> const Vec2<T> Vec2<T>::operator-() const
//-----------------------------------------------------------
{
	return (Vec2(-m_x, -m_y));
}


//---------------------------------------------------------
template<typename T> T& Vec2<T>::operator()(unsigned int i)
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
    else
        {
        throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
        }
}


//---------------------------------------------------------------------
template<typename T> const T& Vec2<T>::operator()(unsigned int i) const
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
    else
        {
        throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
        }
}


//---------------------------------------------------------
template<typename T> T& Vec2<T>::operator[](unsigned int i)
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
    else
        {
        throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
        }
}


//---------------------------------------------------------------------
template<typename T> const T& Vec2<T>::operator[](unsigned int i) const
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
    else
        {
        throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
        }
}


//--------------------------------------------------------------------------
template <typename T> std::ostream& operator<<(std::ostream& anOutputStream,
											   const Vec2<T>& aVector)
//--------------------------------------------------------------------------
{
	anOutputStream << aVector.getX() << ", " << aVector.getY();
	return (anOutputStream);
}


} // namespace gVirtualXRay
