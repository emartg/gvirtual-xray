/*

Copyright (c) 2017, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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


#ifndef GVXR_VEC2_H
#define GVXR_VEC2_H


/**
********************************************************************************
*
*   @file       Vec2.h
*
*   @brief      Template class to handle 2D vectors.
*
*   @version    1.0
*
*   @date       13/01/2017
*
*   @author     Dr Franck P. Vidal
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*   http://www.fpvidal.net/, Jan 2017, 2017, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//******************************************************************************
//  class declaration
//******************************************************************************
template<typename T> class Vec2;


//******************************************************************************
//  function declaration
//******************************************************************************
template <typename T> std::ostream& operator<<(std::ostream& anOutputStream,
                                               const Vec2<T>& aVector);


//==============================================================================
/**
*   @class  Vec2
*   @brief  Vec2 is a template class to handle a 2D vector.
*/
//==============================================================================
template<typename T> class Vec2
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //---------------------------------------------------------------------------
    /// Default Constructor
    /**
     *  @param aX: the position along the x-axis
     *  @param aY: the position along the y-axis
     */
    //---------------------------------------------------------------------------
    Vec2(const T& aX = 0.0f, const T& aY = 0.0f);


    //---------------------------------------------------------------------------
    /// Set the position along the x-axis
    /**
     *  @param aValue:  the position along the x-axis
     */
    //---------------------------------------------------------------------------
    void setX(const T& aValue);


    //---------------------------------------------------------------------------
    /// Set the position along the y-axis
    /**
     *  @param aValue:  the position along the y-axis
     */
    //---------------------------------------------------------------------------
    void setY(const T& aValue);


    //---------------------------------------------------------------------------
    /// Accessor on the position along the x-axis
    /**
     *  @return the position along the x-axis
     */
    //---------------------------------------------------------------------------
    T getX() const;


    //---------------------------------------------------------------------------
    /// Accessor on the position along the y-axis
    /**
     *  @return the position along the y-axis
     */
    //---------------------------------------------------------------------------
    T getY() const;


    //---------------------------------------------------------------------------
    /// Get the length of the vector.
    /**
     *  @return the length of the vector
     */
    //---------------------------------------------------------------------------
    double length() const;


    //---------------------------------------------------------------------------
    /// Get the unit vector corresponding to the normed current vector.
    /**
     *  @return the unit vector
     */
    //---------------------------------------------------------------------------
    Vec2 normal() const;


    //---------------------------------------------------------------------------
    /// Normalize the current vector so that its length is 1.
    //---------------------------------------------------------------------------
    void normalize();


    //---------------------------------------------------------------------------
    /// Normalise the current vector so that its length is 1.
    //---------------------------------------------------------------------------
    void normalise();


    //---------------------------------------------------------------------------
    /// Get the dot product between the current vector and a given vector.
    /**
     *  @param aVector: the other vector
     *  @return the dot product
     */
    //---------------------------------------------------------------------------
    double dotProduct(const Vec2& aVector) const;


    //---------------------------------------------------------------------------
    /// Get the distance between the 2D position represented
    /// by the current vector and another position.
    /**
     *  @param aVector: the other vector
     *  @return the distance
     */
    //---------------------------------------------------------------------------
    double distance(const Vec2& aVector) const;


    //---------------------------------------------------------------------------
    /// Operator +
    /**
     *  @param aVector: the other vector
     *  @return the modified vector
     */
    //---------------------------------------------------------------------------
    const Vec2 operator+(const Vec2& aVector) const;


    //---------------------------------------------------------------------------
    /// Operator +=
    /**
     *  @param aVector: the other vector
     *  @return the modified vector
     */
    //---------------------------------------------------------------------------
    Vec2& operator+=(const Vec2& aVector);
    

    //---------------------------------------------------------------------------
    /// Operator -
    /**
     *  @param aVector: the other vector
     *  @return the modified vector
     */
    //---------------------------------------------------------------------------
    const Vec2 operator-(const Vec2& aVector) const;


    //---------------------------------------------------------------------------
    /// Operator -=
    /**
     *  @param aVector: the other vector
     *  @return the modified vector
     */
    //---------------------------------------------------------------------------
    Vec2& operator-=(const Vec2& aVector);


    //---------------------------------------------------------------------------
    /// Operator * to multiply each component of the vector by a given value.
    /**
     *  @param aValue:  the multiplicative value
     *  @return the modified vector
     */
    //---------------------------------------------------------------------------
    const Vec2 operator*(const double& aValue) const;


    //---------------------------------------------------------------------------
    /// Operator *= to multiply each component of the vector by a given value,
    /// and modify the current vector.
    /**
     *  @param aValue:  the multiplicative value
     *  @return the modified vector
     */
    //---------------------------------------------------------------------------
    Vec2& operator*=(const double& aValue);


    //---------------------------------------------------------------------------
    /// Operator * to multiply each component of the vector by a given value.
    /**
     *  @param aValue:  the multiplicative value
     *  @return the modified vector
     */
    //---------------------------------------------------------------------------
    const Vec2 operator*(const float& aValue) const;


    //---------------------------------------------------------------------------
    /// Operator *= to multiply each component of the vector by a given value,
    /// and modify the current vector.
    /**
     *  @param aValue:  the multiplicative value
     *  @return the modified vector
     */
    //---------------------------------------------------------------------------
    Vec2& operator*=(const float& aValue);


    //---------------------------------------------------------------------------
    /// Operator / to divide each component of the vector by a given value.
    /**
     *  @param aValue:  the value
     *  @return the modified vector
     */
    //---------------------------------------------------------------------------
    const Vec2 operator/(const double& aValue) const;


    //---------------------------------------------------------------------------
    /// Operator /= to divide each component of the vector by a given value,
    /// and modify the current vector.
    /**
     *  @param aValue:  the value
     *  @return the modified vector
     */
    //---------------------------------------------------------------------------
    Vec2& operator/=(const double& aValue);


    //---------------------------------------------------------------------------
    /// Operator / to divide each component of the vector by a given value.
    /**
     *  @param aValue:  the value
     *  @return the modified vector
     */
    //---------------------------------------------------------------------------
    const Vec2 operator/(const float& aValue) const;


    //---------------------------------------------------------------------------
    /// Operator /= to divide each component of the vector by a given value,
    /// and modify the current vector.
    /**
     *  @param aValue:  the value
     *  @return the modified vector
     */
    //---------------------------------------------------------------------------
    Vec2& operator/=(const float& aValue);


    //---------------------------------------------------------------------------
    /// Operator - to get the opposite vector.
    /**
     *  @return the opposite vector
     */
    //---------------------------------------------------------------------------
    const Vec2 operator-() const;


    T& operator()(unsigned int i);

    
    const T& operator()(unsigned int i) const;

    
    T& operator[](unsigned int i);
    
    
    const T& operator[](unsigned int i) const;


//******************************************************************************
protected:
    //**************************************************************************
    T m_x; ///< the position along the x-axis
    T m_y; ///< the position along the y-axi
};


} // namespace gVirtualXRay


//******************************************************************************
#include "Vec2.inl"


#endif // GVXR_VEC2_H
