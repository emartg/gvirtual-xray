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


#ifndef GVXR_VEC3_H
#define GVXR_VEC3_H


/**
********************************************************************************
*
*   @file       Vec3.h
*
*   @brief      Template class to handle 3D vectors.
*
*   @todo       Add an exception in Length().
*               Add a header for constants, use EPSILON instead of 0.0000001
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
#include <iostream>


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//******************************************************************************
//  class declaration
//******************************************************************************
template<typename T> class Vec3;


//******************************************************************************
//  Function declaration
//******************************************************************************
template<typename T> std::ostream& operator<<(std::ostream& anOutputStream,
        const Vec3<T>& aVector);


//==============================================================================
/**
 *  @class  Vec3
 *  @brief  Vec3 is a template class to handle a 3D vector.
 */
//==============================================================================
template<typename T> class Vec3
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default Constructor
    /**
     *  @param aX: the position along the x-axis (default value is 0)
     *  @param aY: the position along the y-axis (default value is 0)
     *  @param aZ: the position along the z-axis (default value is 0)
     */
    //--------------------------------------------------------------------------
    Vec3(const T& aX = 0, const T& aY = 0, const T& aZ = 0);


    //--------------------------------------------------------------------------
    /// Set the position along the x-axis
    /**
     *  @param aValue: the position along the x-axis
     */
    //--------------------------------------------------------------------------
    void setX(const T& aValue);


    //--------------------------------------------------------------------------
    /// Set the position along the y-axis
    /**
     *  @param aValue: the position along the y-axis
     */
    //--------------------------------------------------------------------------
    void setY(const T& aValue);


    //--------------------------------------------------------------------------
    /// Set the position along the z-axis
    /**
     *  @param aValue: the position along the z-axis
     */
    //--------------------------------------------------------------------------
    void setZ(const T& aValue);


    //--------------------------------------------------------------------------
    /// Accessor on the position along the x-axis
    /**
     *  @return the position along the x-axis
     */
    //--------------------------------------------------------------------------
    T getX() const;


    //--------------------------------------------------------------------------
    /// Accessor on the position along the y-axis
    /**
     *  @return the position along the y-axis
     */
    //--------------------------------------------------------------------------
    T getY() const;


    //--------------------------------------------------------------------------
    /// Accessor on the position along the z-axis
    /**
     *  @return the position along the z-axis
     */
    //--------------------------------------------------------------------------
    T getZ() const;


    //--------------------------------------------------------------------------
    /// Get the length of the vector.
    /**
     *  @return the length of the vector
     */
    //--------------------------------------------------------------------------
    double length() const;


    //--------------------------------------------------------------------------
    /// Get the unit vector corresponding to the normed current vector.
    /**
     *  @return the unit vector
     */
    //--------------------------------------------------------------------------
    Vec3 normal() const;


    //--------------------------------------------------------------------------
    /// Normalize the current vector so that its length is 1.
    //--------------------------------------------------------------------------
    void normalize();


    //--------------------------------------------------------------------------
    /// Normalise the current vector so that its length is 1.
    //--------------------------------------------------------------------------
    void normalise();


    //--------------------------------------------------------------------------
    /// Get the dot product between the current vector and a given vector.
    /**
     *  @param aVector: the other vector
     *  @return the dot product
     */
    //--------------------------------------------------------------------------
    double dotProduct(const Vec3& aVector) const;


    //--------------------------------------------------------------------------
    /// Cross product between the current vector and
    /// a given vector.
    /**
     *  @param aVector: the other vector
     *  @return the cross product
     */
    //--------------------------------------------------------------------------
    Vec3 crossProduct(const Vec3& aVector) const;


    //--------------------------------------------------------------------------
    /// Get the distance between the 3D position represented
    /// by the current vector and another position.
    /**
     *  @param aVector: the other vector
     *  @return the distance
     */
    //--------------------------------------------------------------------------
    double distance(const Vec3& aVector) const;


    //--------------------------------------------------------------------------
    /// Operator * to compute the dot product between the current vector and
    /// a given vector.
    /**
     *  @param aVector: the other vector
     *  @return the dot product
     */
    //--------------------------------------------------------------------------
    //double operator*(const Vec3& aVector) const;


    //--------------------------------------------------------------------------
    /// Element-wise product
    /**
     *  @param aVector: the other vector
     *  @return the new vector resulting of the element-wise product
     */
    //--------------------------------------------------------------------------
    Vec3<T> elementWiseProduct(const Vec3& aVector) const;


    //--------------------------------------------------------------------------
    /// Operator +
    /**
     *  @param aVector: the other vector
     *  @return the dot product
     */
    //--------------------------------------------------------------------------
    Vec3 operator+(const Vec3& aVector) const;


    //--------------------------------------------------------------------------
    /// Operator +=
    /**
     *  @param aVector: the other vector
     *  @return the modified vector
     */
    //--------------------------------------------------------------------------
    Vec3& operator+=(const Vec3& aVector);


    //--------------------------------------------------------------------------
    /// Operator -
    /**
     *  @param aVector: the other vector
     *  @return the modified vector
     */
    //--------------------------------------------------------------------------
    Vec3 operator-(const Vec3& aVector) const;


    //--------------------------------------------------------------------------
    /// Operator -=
    /**
     *  @param aVector: the other vector
     *  @return the modified vector
     */
    //--------------------------------------------------------------------------
    Vec3& operator-=(const Vec3& aVector);


    //--------------------------------------------------------------------------
    /// Operator * to multiply each component of the vector by a given value.
    /**
     *  @param aValue: the multiplicative value
     *  @return the modified vector
     */
    //--------------------------------------------------------------------------
    Vec3 operator*(const double& aValue) const;


    //--------------------------------------------------------------------------
    /// Operator *= to multiply each component of the vector by a given value,
    /// and modify the current vector.
    /**
     *  @param aValue: the multiplicative value
     *  @return the modified vector
     */
    //--------------------------------------------------------------------------
    Vec3& operator*=(const double& aValue);


    //--------------------------------------------------------------------------
    /// Operator * to multiply each component of the vector by a given value.
    /**
     *  @param aValue: the multiplicative value
     *  @return the modified vector
     */
    //--------------------------------------------------------------------------
    Vec3 operator*(const float& aValue) const;


    //--------------------------------------------------------------------------
    /// Operator *= to multiply each component of the vector by a given value,
    /// and modify the current vector.
    /**
     *  @param aValue: the multiplicative value
     *  @return the modified vector
     */
    //--------------------------------------------------------------------------
    Vec3& operator*=(const float& aValue);


    //--------------------------------------------------------------------------
    /// Operator / to divide each component of the vector by a given value.
    /**
     *  @param aValue: the value
     *  @return the modified vector
     */
    //--------------------------------------------------------------------------
    Vec3 operator/(const double& aValue) const;


    //--------------------------------------------------------------------------
    /// Operator -=
    /**
     *  @param aValue: the other vector
     *  @return the modified vector
     */
    //--------------------------------------------------------------------------
    Vec3& operator/=(const double& aValue);


    //--------------------------------------------------------------------------
    /// Operator / to divide each component of the vector by a given value.
    /**
     *  @param aValue: the value
     *  @return the modified vector
     */
    //--------------------------------------------------------------------------
    Vec3 operator/(const float& aValue) const;


    //--------------------------------------------------------------------------
    /// Operator -=
    /**
     *  @param aValue: the other vector
     *  @return the modified vector
     */
    //--------------------------------------------------------------------------
    Vec3& operator/=(const float& aValue);


    //--------------------------------------------------------------------------
    /// Operator ^ to compute the cross product between the current vector and
    /// a given vector.
    /**
     *  @param aVector: the other vector
     *  @return the cross product
     */
    //--------------------------------------------------------------------------
    Vec3 operator^(const Vec3& aVector) const;


    //--------------------------------------------------------------------------
    /// Operator - to get the opposite vector.
    /**
     *  @return the opposite vector
     */
    //--------------------------------------------------------------------------
    Vec3 operator-() const;


    //--------------------------------------------------------------------------
    /// Operator ==
    /**
     *  @param aVector: the vector to compare the current vector with
     *  @return true if the two vectors are the same,
     *          false if the two vectors are different
     */
    //--------------------------------------------------------------------------
    bool operator==(const Vec3& aVector) const;


    //--------------------------------------------------------------------------
    /// Operator !=
    /**
     *  @param aVector: the vector to compare the current vector with
     *  @return true if the two vectors are different, false if the two vectors
     *          are the same
     */
    //--------------------------------------------------------------------------
    bool operator!=(const Vec3& aVector) const;


    T& operator()(unsigned int i);
    const T& operator()(unsigned int i) const;

    T& operator[](unsigned int i);
    const T& operator[](unsigned int i) const;
    
    
//******************************************************************************
protected:
    //**************************************************************************
    T m_x; ///< the position along the x-axis
    T m_y; ///< the position along the y-axi
    T m_z; ///< the position along the z-axi
};


//------------------------------------------------------------------------------
/// Operator * to multiply each component of the vector by a given value.
/**
 *  @param aValue:  the multiplicative value
 *  @param aVector: the vector to multiply
 *  @return the modified vector
 */
//------------------------------------------------------------------------------
template<typename T> Vec3<T> operator*(const double& aValue,
        const Vec3<T>& aVector);


} // namespace gVirtualXRay


//******************************************************************************
#include "Vec3.inl"


#endif // GVXR_VEC3_H
