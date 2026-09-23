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


#ifndef GVXR_OPENGL_UTILITIES_H
#define GVXR_OPENGL_UTILITIES_H


/**
********************************************************************************
*
*   @file       OpenGLUtilities.h
*
*   @brief      Some utility functions about OpenGL.
*               Now supports GLSL450 and OpenGL 4.5
*
*   @version    1.0
*
*   @date       25/02/2020
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
*   http://www.fpvidal.net/, Feb 2020, 2020, version 1.0, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifndef GVXR_GVIRTUAL_XRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#ifndef GVXR_MATRIX_4X4_H
#include "gVirtualXRay/Matrix4x4.h"
#endif

#ifndef GVXR_TYPES_H
#include "gVirtualXRay/Types.h"
#endif

#ifndef GVXR_VBO_h
#include "gVirtualXRay/VBO.h"
#endif

#ifndef GVXR_OPENGL2_VBO_H
#include "gVirtualXRay/OpenGL2VBO.h"
#endif

#ifndef GVXR_OPENGL3_VBO_H
#include "gVirtualXRay/OpenGL3VBO.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {

  class OpenGL2VBO;
  class OpenGL3VBO;


//------------------------------------------------------------------------------
/// Initialise GLAD.
//------------------------------------------------------------------------------
void initialiseGLAD(const std::string& anAPI,
        void* aGLXDisplay = nullptr,
        int aGLXScreen = 0,
        void* anEGLDisplay = 0);


//------------------------------------------------------------------------------
/// Initialize GLAD.
//------------------------------------------------------------------------------
void initializeGLAD(const std::string& anAPI);


//------------------------------------------------------------------------------
/// Initialise GLEW.
//------------------------------------------------------------------------------
void initialiseGLEW();


//------------------------------------------------------------------------------
/// Initialize GLEW.
//------------------------------------------------------------------------------
void initializeGLEW();


//------------------------------------------------------------------------------
/// Check OpenGL's error status.
/**
*   @param  aFileName:  name of the source file where OpenGL is checked
*   @param  aFunctionName:  name of the function where OpenGL is checked
*   @param  aLineNumber:    name at which OpenGL is checked
*/
//------------------------------------------------------------------------------
void checkOpenGLErrorStatus(const char* aFileName,
                            const char* aFunctionName,
                            int aLineNumber);


//------------------------------------------------------------------------------
/// Check the current FBO's error status.
/**
*   @param  aFileName:  name of the source file where FBO is checked
*   @param  aFunctionName:  name of the function where FBO is checked
*   @param  aLineNumber:    name at which FBO is checked
*/
//------------------------------------------------------------------------------
void checkFBOErrorStatus(const char* aFileName,
                         const char* aFunctionName,
                         int aLineNumber);


//------------------------------------------------------------------------------
/// Add the current matrix to the projection matrix stack.
//------------------------------------------------------------------------------
void pushProjectionMatrix();


//------------------------------------------------------------------------------
/// Add the current matrix to the model/view matrix stack.
//------------------------------------------------------------------------------
void pushModelViewMatrix();


//------------------------------------------------------------------------------
/// Add the current FBO binding to the FBO stack.
//------------------------------------------------------------------------------
void pushFBO();


//------------------------------------------------------------------------------
/// Add the current 1D texture binding to the texture stack.
//------------------------------------------------------------------------------
void pushTexture1D();


//------------------------------------------------------------------------------
/// Add the current 2D texture binding to the texture stack.
//------------------------------------------------------------------------------
void pushTexture2D();


//------------------------------------------------------------------------------
/// Add the current rectangle texture binding to the texture stack.
//------------------------------------------------------------------------------
void pushTextureRectangle();


//------------------------------------------------------------------------------
/// Add the current 3D texture binding to the texture stack.
//------------------------------------------------------------------------------
void pushTexture3D();


//------------------------------------------------------------------------------
/// Add the enable/disable state to the corresponding stack.
/**
*   @param  aParameter: parameter to add
*/
//------------------------------------------------------------------------------
void pushEnableDisableState(int aParameter);


//------------------------------------------------------------------------------
/// Add the current shader program to the corresponding stack.
//------------------------------------------------------------------------------
void pushShaderProgram();


//------------------------------------------------------------------------------
/// Restore the matrix from the projection matrix stack,
/// and remove the last matrix of the stack.
//------------------------------------------------------------------------------
void popProjectionMatrix();


//------------------------------------------------------------------------------
/// Restore the matrix from the model/view matrix stack,
/// and remove the last matrix of the stack.
//------------------------------------------------------------------------------
void popModelViewMatrix();


//------------------------------------------------------------------------------
/// Restore the FBO binding from the FBO stack,
/// and remove the last FBO of the stack.
//------------------------------------------------------------------------------
void popFBO();


//------------------------------------------------------------------------------
/// Restore the texture binding from the texture stack,
/// and remove the last texture of the stack.
//------------------------------------------------------------------------------
void popTexture();


//------------------------------------------------------------------------------
/// Restore the enable/disable state to the corresponding stack,
/// and remove the last state of the stack.
//------------------------------------------------------------------------------
void popEnableDisableState();


//------------------------------------------------------------------------------
/// Restore the shader program to the corresponding stack,
/// and remove the last state of the stack.
//------------------------------------------------------------------------------
void popShaderProgram();


//--------------------------------------------------------------------------
/// Load the current OpenGL matrix.
/**
*   @return the current OpenGL matrix.
*/
//--------------------------------------------------------------------------
gVirtualXRay::Matrix4x4<float> getCurrentOpenGLMatrix();


//------------------------------------------------------------------------------
/// Replace the projection matrix by the identity matrix.
//------------------------------------------------------------------------------
void loadIdentityProjectionMatrix();


//------------------------------------------------------------------------------
/// Replace the model/view matrix by the identity matrix.
//------------------------------------------------------------------------------
void loadIdentityModelViewMatrix();


//------------------------------------------------------------------------------
/// Replace the projection matrix by a perspective projection matrix.
/**
*   @param  left:   Specify the coordinates for the left vertical clipping plane
*   @param  right:  Specify the coordinates for the right vertical
*                   clipping plane
*   @param  bottom: Specify the coordinates for the bottom horizontal
*                   clipping plane
*   @param  top:    Specify the coordinates for the top horizontal
*                   clipping plane
*   @param  near:   Specify the distances to the near clipping plane.
*                   The value must be positive
*   @param  far:    Specify the distances to the far clipping plane.
*                   The value must be positive
*/
//------------------------------------------------------------------------------
void loadFrustumProjectionMatrix(double left,
        double right,
        double bottom,
        double top,
        double near,
        double far);


//------------------------------------------------------------------------------
/// Replace the projection matrix by a perspective projection matrix.
/**
*   @param  fovy:   specifies the field of view angle, in degrees, in
*                   the y-direction.
*   @param  aspect: Specifies the aspect ratio that determines
*                   the field of view in the x direction.
*                   The aspect ratio is the ratio of x (width) to y (height).
*   @param  zNear:  Specifies the distance from the viewer to the near
*                   clipping plane (always positive)
*   @param  zFar:   Specifies the distance from the viewer to the far
*                   clipping plane (always positive).
*/
//------------------------------------------------------------------------------
void loadPerspectiveProjectionMatrix(double fovy,
                                     double aspect,
                                     double zNear,
                                     double zFar);


//------------------------------------------------------------------------------
/// Replace the projection matrix by a orthographic projection matrix.
/**
*   @param  left:   Specify the coordinates for the left vertical clipping plane
*   @param  right:  Specify the coordinates for the right vertical
*                   clipping plane
*   @param  bottom: Specify the coordinates for the bottom horizontal
*                   clipping plane
*   @param  top:    Specify the coordinates for the top horizontal
*                   clipping plane
*   @param  near:   Specify the distances to the near clipping plane.
*                   The value must be positive
*   @param  far:    Specify the distances to the far clipping plane.
*                   The value must be positive
*/
//------------------------------------------------------------------------------
void loadOrthoProjectionMatrix(double left,
        double right,
        double bottom,
        double top,
        double near,
        double far);


//------------------------------------------------------------------------------
/// Create a orthographic projection matrix.
/**
*   @param  left:   Specify the coordinates for the left vertical clipping plane
*   @param  right:  Specify the coordinates for the right vertical
*                   clipping plane
*   @param  bottom: Specify the coordinates for the bottom horizontal
*                   clipping plane
*   @param  top:    Specify the coordinates for the top horizontal
*                   clipping plane
*   @param  near:   Specify the distances to the near clipping plane.
*                   The value must be positive
*   @param  far:    Specify the distances to the far clipping plane.
*                   The value must be positive
*   @return the projection matrix
*/
//------------------------------------------------------------------------------
MATRIX4 buildOrthoProjectionMatrix(double left,
        double right,
        double bottom,
        double top,
        double near,
        double far);


//------------------------------------------------------------------------------
/// Create a perspective projection matrix.
/**
*   @param  left:   Specify the coordinates for the left vertical clipping plane
*   @param  right:  Specify the coordinates for the right vertical
*                   clipping plane
*   @param  bottom: Specify the coordinates for the bottom horizontal
*                   clipping plane
*   @param  top:    Specify the coordinates for the top horizontal
*                   clipping plane
*   @param  near:   Specify the distances to the near clipping plane.
*                   The value must be positive
*   @param  far:    Specify the distances to the far clipping plane.
*                   The value must be positive
*   @return the projection matrix
*/
//------------------------------------------------------------------------------
MATRIX4 buildFrustumProjectionMatrix(double left,
        double right,
        double bottom,
        double top,
        double near,
        double far);


//------------------------------------------------------------------------------
/// Create a perspective projection matrix.
/**
*   @param  fovy:   specifies the field of view angle, in degrees, in
*                   the y-direction.
*   @param  aspect: Specifies the aspect ratio that determines
*                   the field of view in the x direction.
*                   The aspect ratio is the ratio of x (width) to y (height).
*   @param  zNear:  Specifies the distance from the viewer to the near
*                   clipping plane (always positive)
*   @param  zFar:   Specifies the distance from the viewer to the far
*                   clipping plane (always positive).
*   @return the projection matrix
*/
//------------------------------------------------------------------------------
MATRIX4 buildPerspectiveProjectionMatrix(double fovy,
        double aspect,
        double zNear,
        double zFar);


//------------------------------------------------------------------------------
/// Replace the modelling-viewing matrix by a viewing transformation matrix.
/**
*   @param  eye:    Specifies the position of the eye point
*   @param  centre: Specifies the position of the reference point
*   @param  up:     Specifies the direction of the up vector
*/
//------------------------------------------------------------------------------
void loadLookAtModelViewMatrix(const VEC3& eye,
        const VEC3& centre,
        const VEC3& up);


//------------------------------------------------------------------------------
/// Replace the modelling-viewing matrix by a viewing transformation matrix.
/**
*   @param  eyeX:       Specifies the position of the eye point along the X-axis
*   @param  eyeY:       Specifies the position of the eye point along the Y-axis
*   @param  eyeZ:       Specifies the position of the eye point along the Z-axis
*   @param  centreX:    Specifies the position of the reference point along
*                       the X-axis
*   @param  centreY:    Specifies the position of the reference point along
*                       the Y-axis
*   @param  centreZ:    Specifies the position of the reference point along
*                       the Z-axis
*   @param  upX         Specifies the direction of the up vector along
*                       the X-axis
*   @param  upY         Specifies the direction of the up vector along
*                       the Y-axis
*   @param  upZ         Specifies the direction of the up vector along
*                       the Z-axis
*/
//------------------------------------------------------------------------------
void loadLookAtModelViewMatrix(double eyeX,
        double eyeY,
        double eyeZ,
        double centreX,
        double centreY,
        double centreZ,
        double upX,
        double upY,
        double upZ);


//------------------------------------------------------------------------------
/// Create a modelling-viewing matrix.
/**
*   @param  eye:    Specifies the position of the eye point
*   @param  centre: Specifies the position of the reference point
*   @param  up:     Specifies the direction of the up vector
*   @return the modelling-viewing matrix
*/
//------------------------------------------------------------------------------
MATRIX4 buildLookAtModelViewMatrix(const VEC3& eye,
          const VEC3& centre,
          const VEC3& up);


//------------------------------------------------------------------------------
/// Create a modelling-viewing matrix.
/**
*   @param  eyeX:       Specifies the position of the eye point along the X-axis
*   @param  eyeY:       Specifies the position of the eye point along the Y-axis
*   @param  eyeZ:       Specifies the position of the eye point along the Z-axis
*   @param  centreX:    Specifies the position of the reference point along
*                       the X-axis
*   @param  centreY:    Specifies the position of the reference point along
*                       the Y-axis
*   @param  centreZ:    Specifies the position of the reference point along
*                       the Z-axis
*   @param  upX         Specifies the direction of the up vector along
*                       the X-axis
*   @param  upY         Specifies the direction of the up vector along
*                       the Y-axis
*   @param  upZ         Specifies the direction of the up vector along
*                       the Z-axis
*   @return the modelling-viewing matrix
*/
//------------------------------------------------------------------------------
MATRIX4 buildLookAtModelViewMatrix(double eyeX,
        double eyeY,
        double eyeZ,
        double centreX,
        double centreY,
        double centreZ,
        double upX,
        double upY,
        double upZ);


//------------------------------------------------------------------------------
/// Replace the projection matrix by OpenGL's projection matrix.
//------------------------------------------------------------------------------
void loadProjectionMatrixFromOpenGL();


//------------------------------------------------------------------------------
/// Replace the modelling-viewing matrix by OpenGL's modelling-viewing matrix.
//------------------------------------------------------------------------------
void loadModelViewMatrixFromOpenGL();


//------------------------------------------------------------------------------
/// Load the modelling-viewing matrix in the current shader program, or
/// replace OpenGL's modelling-viewing matrix.
//------------------------------------------------------------------------------
void applyModelViewMatrix();


//------------------------------------------------------------------------------
/// Check if OpenGL 4.5 is supported by the current OpenGL context.
/*
* @return   true if the current OpenGL context supports OpenGL 4.5,
*           false otherwise
*/
//------------------------------------------------------------------------------
bool useOpenGL45();


//------------------------------------------------------------------------------
/// Check if OpenGL 3.2 is supported by the current OpenGL context.
/*
* @return   true if the current OpenGL context supports OpenGL 3.2,
*           false otherwise
*/
//------------------------------------------------------------------------------
bool useOpenGL32();


//------------------------------------------------------------------------------
/// Check if OpenGL Compute Shaders are supported by the current OpenGL context.
/*
* @return   true if the current OpenGL context supports Compute Shaders,
*           false otherwise
*/
//------------------------------------------------------------------------------
bool useOpenGLCompute();


//------------------------------------------------------------------------------
/// Create an OpenGL VBO suitable for the current OpenGL context.
/*
* @return the new OpenGL VBO
*/
//------------------------------------------------------------------------------
VBO* createVBO();


//------------------------------------------------------------------------------
/// Create an OpenGL 2.x VBO.
/*
* @return the new OpenGL VBO
*/
//------------------------------------------------------------------------------
OpenGL2VBO* createOpenGL2VBO();


//------------------------------------------------------------------------------
/// Create an OpenGL 3.x or 4.x VBO.
/*
* @return the new OpenGL VBO
*/
//------------------------------------------------------------------------------
OpenGL3VBO* createOpenGL3VBO();


std::string getShaderTypeID(const std::type_info& aTypeID);
std::string getShaderImageType(const std::type_info& aTypeID);
std::string getShaderPixelType(const std::type_info& aTypeID);
std::string getShaderRGBAPixelType(const std::type_info& aTypeID);


//------------------------------------------------------------------------------
/// Access the largest possible 2D texture size on the current environment.
/*
* @return the largest possible 2D texture size
*/
//------------------------------------------------------------------------------
Vec2ui getMax2DTextureSize();


//------------------------------------------------------------------------------
/// Access the largest possible 3D texture size on the current environment.
/*
* @return the largest possible 3D texture size
*/
//------------------------------------------------------------------------------
Vec3ui getMax3DTextureSize();


/// The stack of model/view matrices
extern std::vector<MATRIX4> g_p_modelview_matrix_stack;


/// The stack of projection matrix
extern std::vector<MATRIX4> g_p_projection_matrix_stack;


/// The model/view matrices
extern MATRIX4 g_current_modelview_matrix;


/// The projection matrix
extern MATRIX4 g_current_projection_matrix;


/// The stack of FBO binding
extern std::vector<int> g_frame_buffer_object_stack;


/// The stack of texture binding
extern std::vector<std::pair<int, int> > g_texture_stack;


/// The stack of enable/disable states
extern std::vector<std::pair<int, bool> > g_enable_disable_state_stack;


/// The stack of shader programs
extern std::vector<int> g_shader_program_stack;


} // namespace gVirtualXRay


//******************************************************************************
#include "OpenGLUtilities.inl"


#endif // GVXR_OPENGL_UTILITIES_H
