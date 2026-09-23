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


#ifndef GVXR_MATRIX_4X4_H
#define GVXR_MATRIX_4X4_H


/**
********************************************************************************
*
*   @file       Matrix4x4.h
*
*   @brief      Template class to handle a 4x4 matrixs.
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
*   http://www.fpvidal.net/, Jan 2017, 2017, version 1.0, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <vector>

#ifndef GVXR_VEC3_H
#include "gVirtualXRay/Vec3.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//******************************************************************************
//  class declaration
//******************************************************************************
template<typename T> class Matrix4x4;


//******************************************************************************
//  function declaration
//******************************************************************************
template <typename T> std::ostream& operator<<(std::ostream& anOutputStream,
                                               const Matrix4x4<T>& aMatrix);


//==============================================================================
/**
*   @class  Matrix4x4
*   @brief  Matrix4x4 is a template class to handle a 4 by 4 matrix.
*/
//==============================================================================
template<typename T> class Matrix4x4
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default Constructor (create an identity matrix).
    /**
     *  @param a1:  the  1st element of the matrix
     *  @param a2:  the  2nd element of the matrix
     *  @param a3:  the  3rd element of the matrix
     *  @param a4:  the  4th element of the matrix
     *  @param a5:  the  5th element of the matrix
     *  @param a6:  the  6th element of the matrix
     *  @param a7:  the  7th element of the matrix
     *  @param a8:  the  8th element of the matrix
     *  @param a9:  the  9th element of the matrix
     *  @param a10: the 10th element of the matrix
     *  @param a11: the 11th element of the matrix
     *  @param a12: the 12th element of the matrix
     *  @param a13: the 13th element of the matrix
     *  @param a14: the 14th element of the matrix
     *  @param a15: the 15th element of the matrix
     *  @param a16: the 16th element of the matrix
     */
    //--------------------------------------------------------------------------
    Matrix4x4(const T& a1 = 1.0f,
              const T& a2 = 0.0f,
              const T& a3 = 0.0f,
              const T& a4 = 0.0f,
              const T& a5 = 0.0f,
              const T& a6 = 1.0f,
              const T& a7 = 0.0f,
              const T& a8 = 0.0f,
              const T& a9 = 0.0f,
              const T& a10 = 0.0f,
              const T& a11 = 1.0f,
              const T& a12 = 0.0f,
              const T& a13 = 0.0f,
              const T& a14 = 0.0f,
              const T& a15 = 0.0f,
              const T& a16 = 1.0f);


    //--------------------------------------------------------------------------
    /// Copy Constructor.
    /**
     *  @param aMatrix: the matrix to copy
     */
    //--------------------------------------------------------------------------
    Matrix4x4(const Matrix4x4& aMatrix);


    //--------------------------------------------------------------------------
    /// Copy Constructor.
    /**
     *  @param apMatrix: the matrix to copy
     */
    //--------------------------------------------------------------------------
    Matrix4x4(const Matrix4x4* apMatrix);


    //--------------------------------------------------------------------------
    /// Copy Constructor.
    /**
     *  @param apMatrix: the matrix to copy
     */
    //--------------------------------------------------------------------------
    Matrix4x4(const T* apMatrix);


    //--------------------------------------------------------------------------
    /// Copy Constructor.
    /**
     *  @param apMatrix: the matrix to copy
     */
    //--------------------------------------------------------------------------
    Matrix4x4(const std::vector<double>& apMatrix);


    //--------------------------------------------------------------------------
    /// Load identity matrix.
    //--------------------------------------------------------------------------
    void loadIdentity();


    //--------------------------------------------------------------------------
    /// Load the matrix.
    /**
     *  @param apMatrix: the matrix to copy
     */
    //--------------------------------------------------------------------------
    void setFromVector(const std::vector<double>& apMatrix);


    //--------------------------------------------------------------------------
    /// Multiply the current matrix by a rotation matrix.
    /**
     *  @param anAngle: the angle of rotation in degrees
     *  @param aVector: the axis of rotation
     */
    //--------------------------------------------------------------------------
    void rotate(const double anAngle, const Vec3<T>& aVector);


    //--------------------------------------------------------------------------
    /// Multiply the current matrix by a yaw, pitch, roll rotation matrix.
    /**
     *  @param yaw: a counterclockwise rotation of yaw degrees about the z-axis
     *  @param pitch: a counterclockwise rotation of yaw degrees about the y-axis
     *  @param roll: a counterclockwise rotation of yaw degrees about the x-axis
     */
    //--------------------------------------------------------------------------
    void yawPitchRollRotate(double yaw, double pitch, double roll);


    //--------------------------------------------------------------------------
    /// Multiply the current matrix by a translation matrix.
    /**
     *  @param aVector: the translation vector
     */
    //--------------------------------------------------------------------------
    void translate(const Vec3<T>& aVector);


    //--------------------------------------------------------------------------
    /// Multiply the current matrix by a translation matrix.
    /**
     *  @param x: the x-component of the translation vector
     *  @param y: the y-component of the translation vector
     *  @param z: the z-component of the translation vector
     */
    //--------------------------------------------------------------------------
    void translate(T x, T y, T z);


    //--------------------------------------------------------------------------
    /// Build an identity matrix.
    /**
     *  @return the identity matrix
     */
    //--------------------------------------------------------------------------
    static Matrix4x4 buildIdentityMatrix();


    //--------------------------------------------------------------------------
    /// Build a tranlation matrix.
    /**
     *  @param aVector: the translation vector
     *  @return the corresponding translation matrix
     */
    //--------------------------------------------------------------------------
    static Matrix4x4 buildTranslationMatrix(const Vec3<T>& aVector);


    //--------------------------------------------------------------------------
    /// Build a rotation matrix.
    /**
     *  @param anAngle: the angle of rotation in degrees
     *  @param x: the x-component of the axis of rotation
     *  @param y: the y-component of the axis of rotation
     *  @param z: the z-component of the axis of rotation
     *  @return the corresponding rotation matrix
     */
    //--------------------------------------------------------------------------
    static Matrix4x4 buildRotationMatrix(T anAngle, T x, T y, T z);


    //--------------------------------------------------------------------------
    /// Build a rotation matrix.
    /**
     *  @param yaw: a counterclockwise rotation of yaw degrees about the z-axis
     *  @param pitch: a counterclockwise rotation of yaw degrees about the y-axis
     *  @param roll: a counterclockwise rotation of yaw degrees about the x-axis
     *  @return the corresponding rotation matrix
     */
    //--------------------------------------------------------------------------
    static Matrix4x4 buildYawPitchRollRotationMatrix(T yaw, T pitch, T roll);


    //--------------------------------------------------------------------------
    /// Build a scale matrix.
    /**
    *  @param aVector: the scale vector
    *  @return the corresponding scale matrix
    */
    //--------------------------------------------------------------------------
    static Matrix4x4 buildScaleMatrix( const Vec3<T>& aVector );


    //--------------------------------------------------------------------------
    /// Build a scale matrix.
    /**
    *
    *  @param x: the x-component of the axis of scale
    *  @param y: the y-component of the axis of scale
    *  @param z: the z-component of the axis of scale
    *  @return the corresponding scale matrix
    */
    //--------------------------------------------------------------------------
    static Matrix4x4 buildScaleMatrix(  T x, T y, T z );


    //--------------------------------------------------------------------------
    /// Build a rotation matrix.
    /**
     *  @param anAngle: the angle of rotation in degrees
     *  @param aVector: the axis of rotation
     *  @return the corresponding translation matrix
     */
    //--------------------------------------------------------------------------
    static Matrix4x4 buildRotationMatrix(T anAngle, const Vec3<T>& aVector);


    //--------------------------------------------------------------------------
    /// Build a rotation matrix.
    /**
     *  @param x: the x-component of the translation vector
     *  @param y: the y-component of the translation vector
     *  @param z: the z-component of the translation vector
     *  @return the corresponding translation matrix
     */
    //--------------------------------------------------------------------------
    static Matrix4x4 buildTranslationMatrix(T x, T y, T z);


    //--------------------------------------------------------------------------
    /// Multiply the current matrix by a scaling matrix.
    /**
     *  @param x: the scaling factor along the X-axis
     *  @param y: the scaling factor along the Y-axis
     *  @param z: the scaling factor along the Z-axis
     */
    //--------------------------------------------------------------------------
    void scale(T x, T y, T z);


    //--------------------------------------------------------------------------
    /// Get the transpose of the current matrix.
    /**
     *  @return the transpose matrix
     */
    //--------------------------------------------------------------------------
    Matrix4x4 getTranspose() const;


    //--------------------------------------------------------------------------
    /// Compute the determinant of the current matrix.
    /**
     *  @return the determinant
     */
    //--------------------------------------------------------------------------
    T determinant() const;


    //--------------------------------------------------------------------------
    /// Get the inverse of the current matrix.
    /**
     *  @return the inverse matrix
     */
    //--------------------------------------------------------------------------
    Matrix4x4 getInverse() const;


    //--------------------------------------------------------------------------
    /// Copy operator.
    /**
     *  @param aMatrix: the matrix to copy
     *  @return the copied matrix
     */
    //--------------------------------------------------------------------------
    Matrix4x4& operator=(const Matrix4x4& aMatrix);


    //--------------------------------------------------------------------------
    /// Accessor on the 4x4 matrix data.
    /**
     *  @return the matrix raw data
     */
    //--------------------------------------------------------------------------
    const T* get() const;


    //--------------------------------------------------------------------------
    /// Accessor on the 4x4 matrix data.
    /**
     *  @return the matrix raw data
     */
    //--------------------------------------------------------------------------
    const T* get4x4() const;


    //--------------------------------------------------------------------------
    /// Accessor on the 3x3 matrix data.
    /**
     *  @return the matrix raw data
     */
    //--------------------------------------------------------------------------
    const T* get3x3();


    //--------------------------------------------------------------------------
    /// Accessor on the 4x4 matrix data.
    /**
     *  @return the matrix raw data
     */
    //--------------------------------------------------------------------------
    std::vector<double> getAsVector() const;


    //--------------------------------------------------------------------------
    /// Operator * to multiply the current matrix by a vector.
    /**
     *  @param aVector: the vector to transform
     *  @return the transformed vector
     */
    //--------------------------------------------------------------------------
    Vec3<T> operator*(const Vec3<T>& aVector) const;


    //--------------------------------------------------------------------------
    /// Operator * to multiply the current matrix by another matrix.
    /**
     *  @param aMatrix: the other matrix
     *  @return the transformed matrix
     */
    //--------------------------------------------------------------------------
    Matrix4x4 operator*(const Matrix4x4& aMatrix) const;


    //--------------------------------------------------------------------------
    /// Operator *= to multiply the current matrix by another matrix,
    /// and modify the current matrix.
    /**
     *  @param aMatrix: the other matrix
     *  @return the transformed matrix
     */
    //--------------------------------------------------------------------------
    Matrix4x4& operator*=(const Matrix4x4& aMatrix);


    //--------------------------------------------------------------------------
    /// Print the matrix in the console.
    //--------------------------------------------------------------------------
    void print() const;


    //--------------------------------------------------------------------------
    /// Print the matrix in the console.
    /**
     *  @param anOutputStream: the stream in which the matrix will be printed
     */
    //--------------------------------------------------------------------------
    void print(std::ostream& anOutputStream) const;


//******************************************************************************
protected:
    /// the matrix data
    T m_p_4x4_data[4 * 4];


    /// the matrix data
    T m_p_3x3_data[3 * 3];


    /// A flag to check if the 3x3 matrix is up-to-date.
    /// It is used for lazy evaluation.
    bool m_small_matrix_up_to_date;
};


} // namespace gVirtualXRay


//******************************************************************************
#include "Matrix4x4.inl"


#endif // GVXR_MATRIX_4X4_H
