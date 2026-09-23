/*

Copyright (c) 2023, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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


#ifndef GVXR_COORDINATE_SYSTEM_MESH_H
#define GVXR_COORDINATE_SYSTEM_MESH_H


/**
********************************************************************************
*
*   @file       CoordinateSystemMesh.h
*
*   @brief      Class to handle coordinate system meshes.
*
*   @version    1.0
*
*   @date       20/11/2023
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
*   http://www.fpvidal.net/, Nov 2023, 2023, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#ifndef GVXR_LINE_MESH_H
#include "gVirtualXRay/LineMesh.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*   @class  CoordinateSystemMesh
*   @brief  CoordinateSystemMesh is a class to handle the display of coordinate 
*           system axes in the visualisation.
*/
//==============================================================================
class CoordinateSystemMesh
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default constructor.
    //--------------------------------------------------------------------------
    CoordinateSystemMesh(RATIONAL_NUMBER aLength = 1.0 * cm);


    //--------------------------------------------------------------------------
    /// Destructor.
    //--------------------------------------------------------------------------
    ~CoordinateSystemMesh();


    //--------------------------------------------------------------------------
    /// Release the data.
    //--------------------------------------------------------------------------
    void release();


    //--------------------------------------------------------------------------
    /// Display the coordinate system axes using OpenGL.
    //--------------------------------------------------------------------------
    void display(GLuint aShaderID);


    //--------------------------------------------------------------------------
    /// Set the length of axes (in units of length)
    /**
    *   @param aLength: the length
    */
    //--------------------------------------------------------------------------
    void setLength(const RATIONAL_NUMBER& aLength);


    //--------------------------------------------------------------------------
    /// Accessor on the length of axes (in units of length)
    /**
    *   @return the length
    */
    //--------------------------------------------------------------------------
    const RATIONAL_NUMBER& getLength() const;


//******************************************************************************
private:
    RATIONAL_NUMBER m_length;


    //--------------------------------------------------------------------------
    /// Update the VBOs.
    //--------------------------------------------------------------------------
    void updateVBOs();


    /// The triangle mesh corresponding to the X-axis
    LineMesh<float> m_x_axis_geometry;


    /// The triangle mesh corresponding to the Y-axis
    LineMesh<float> m_y_axis_geometry;


    /// The triangle mesh corresponding to the Z-axis
    LineMesh<float> m_z_axis_geometry;
};


} // namespace gVirtualXRay


//******************************************************************************
#include "CoordinateSystemMesh.inl"


#endif // GVXR_COORDINATE_SYSTEM_MESH_H
