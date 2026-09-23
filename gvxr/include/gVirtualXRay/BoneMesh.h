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


#ifndef GVXR_BONE_MESH_H
#define GVXR_BONE_MESH_H


/**
********************************************************************************
*
*   @file       BoneMesh.h
*
*   @brief      Class to handle a polygon mesh for Bone with thin
*               bumps stored using a 3D texture.
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
#ifndef GVXR_INTERNAL_ORGAN_MESH_H
#include "gVirtualXRay/InternalOrganMesh.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*   @class  BoneMesh
*   @brief  BoneMesh is a class to handle a polygon mesh for Bone,
*           with thin bumps stored using a 3D texture.
*/
//==============================================================================
class BoneMesh: public InternalOrganMesh
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default Constructor
    /*
     *  @param texture_3D_size: size of the side of the buffer of
     *                          the 3D texture (default: 32)
     */
    //--------------------------------------------------------------------------
    BoneMesh(unsigned int texture_3D_size = 32);

    //--------------------------------------------------------------------------
    /// Destructor
    //--------------------------------------------------------------------------
    virtual ~BoneMesh();

    //--------------------------------------------------------------------------
    /// return true if the mesh represent a soft tissue
    //--------------------------------------------------------------------------
    virtual bool isSoftTissue() const;

    //--------------------------------------------------------------------------
    ///    Update the VBO.
    /**
     *   @param aTypeOfVBO:         VBO usage hint to set; valid values are
     *                              GL_STATIC_DRAW, GL_DYNAMIC_DRAW and
     *                              GL_STREAM_DRAW
     *   @param aTypeOfPrimitive:   type of primitive; valid values are
     *                              GL_TRIANGLES
     */
    //--------------------------------------------------------------------------
    virtual void updateVBO(int aTypeOfVBO, int aTypeOfPrimitive);

    //--------------------------------------------------------------------------
    /// Link the attributes to their id in the shader
    /*
    *   @param aShaderID: The GLSL shader used to display the anatomical mesh
    */
    //-------------------------------------------------------------------------
    virtual void defineDisplayShaderInput(int shader_id);


    //--------------------------------------------------------------------------
    /// Merge the vertices for optimization and a smoother display and
    /// create the bump map
    //--------------------------------------------------------------------------
    virtual void initialize();


//******************************************************************************
protected:
    //--------------------------------------------------------------------------
    /// generates the textures
    //--------------------------------------------------------------------------
    virtual void generateTextures();

    //-------------------------------------------------------------------------
    /// Link the attributes to their id in the shader bump map generation
    /*
     *  @param shader_id: The GLSL shader
     */
    //-------------------------------------------------------------------------
    virtual void defineBumpMapShaderInput(int shader_id);

    //--------------------------------------------------------------------------
    /// Link the attributes to their id in the shader
    /// for colour texture generation
    /*
    *   @param shader_id: The GLSL shader
    */
    //-------------------------------------------------------------------------
    virtual void defineColourTextureShaderInput(int shader_id);
};

} // namespace gVirtualXRay

#endif // GVXR_BONE_MESH_H
