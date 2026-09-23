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


#ifndef GVXR_ANATOMICAL_MESH_H
#define GVXR_ANATOMICAL_MESH_H


/**
********************************************************************************
*
*   @file       AnatomicalMesh.h
*
*   @brief      Class to handle a polygon mesh for tissue.
*
*   @version    1.0
*
*   @date       18/07/2016
*
*   @author     Andreas Meuleman
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <string>

#ifndef GVXR_POLYGON_MESH_H
#include "gVirtualXRay/PolygonMesh.h"
#endif

#ifndef GVXR_MATRIX_4X4_H
#include "gVirtualXRay/Matrix4x4.h"
#endif


//******************************************************************************
//  Namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*   @class  AnatomicalMesh
*   @brief  AnatomicalMesh is a class to handle a polygon mesh for tissue.
*/
//==============================================================================
class AnatomicalMesh: public PolygonMesh
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default Constructor
    //--------------------------------------------------------------------------
    AnatomicalMesh();

    
    //--------------------------------------------------------------------------
    /// Destructor
    //--------------------------------------------------------------------------
    virtual ~AnatomicalMesh();


    //--------------------------------------------------------------------------
    /// return true if the mesh has to be displayed with transparency
    //--------------------------------------------------------------------------
    [[nodiscard]] virtual bool isTransparent() const = 0;

    
    //--------------------------------------------------------------------------
    /// return true if the mesh represent a soft tissue
    //--------------------------------------------------------------------------
    [[nodiscard]] virtual bool isSoftTissue() const = 0;
    

    //--------------------------------------------------------------------------
    /// Link the attributes to their id in the shader
    /*
    *   @param shader_id: The GLSL shader used to display the anatomical mesh
    */
    //-------------------------------------------------------------------------
    virtual void defineDisplayShaderInput(unsigned int aShaderID);


    //--------------------------------------------------------------------------
    /// initialize the needful depending on the organ
    //--------------------------------------------------------------------------
    virtual void initialize() = 0;


    void copyFrom(const AnatomicalMesh& aPolygonMesh);


    AnatomicalMesh& operator=(const PolygonMesh& aPolygonMesh);


    AnatomicalMesh& operator=(const AnatomicalMesh& aPolygonMesh);


    //--------------------------------------------------------------------------
    /// Accessor on the label that determines the type of the mesh
    /*
    *   @return the label
    */
    //-------------------------------------------------------------------------
    [[nodiscard]] virtual const std::string& getLabel() const;
    
    
//******************************************************************************
protected:
    /// 1 if the mesh use texturing; default value is 0
    int m_use_texture;

    /// 1 if the mesh use bump mapping; default value is 0
    int m_use_bump_map;
    
    /// A label to determine the type of the mesh
    std::string m_label;
};


} //namespace gVirtualXRay


#endif // GVXR_ANATOMICAL_MESH_H
