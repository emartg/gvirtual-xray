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


#ifndef GVXR_DIAPHRAGM_MESH_H
#define GVXR_DIAPHRAGM_MESH_H


/**
********************************************************************************
*
*   @file       DiaphragmMesh.h
*
*   @brief      Class to handle a polygon mesh for a diaphragm.
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
#ifndef SOFT_TISSUE_MESH_H
#include "gVirtualXRay/SoftTissueMesh.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*   @class  DiaphragmMesh
*   @brief  DiaphragmMesh is a class to handle a polygon mesh for a diaphragm.
*/
//==============================================================================
class DiaphragmMesh: public SoftTissueMesh
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default Constructor
    /*
     *  @param texture_size:                    size of the side of the buffer
     *                                          (default: 2048)
     *  @param optimize_texture_coordinates:    To know if the texture
     *                                          coordinates need to be optimized
     *                                          (default: true)
     */
    //--------------------------------------------------------------------------
    DiaphragmMesh(bool optimize_texture_coordinates = true,
            unsigned int texture_size = 2048);


    //--------------------------------------------------------------------------
    /// Destructor
    //--------------------------------------------------------------------------
    virtual ~DiaphragmMesh();


//******************************************************************************
protected:
    //--------------------------------------------------------------------------
    /// Link the attributes to their id in the shader bump map generation
    /*
     *  @param shader_id: The GLSL shader
     */
    //-------------------------------------------------------------------------
    virtual void defineBumpMapShaderInput(int shader_id);

    //--------------------------------------------------------------------------
    /// Link the attributes to their ID in the shader
    /// for colour texture generation
    /*
    *  @param shader_id: The GLSL shader
    */
    //-------------------------------------------------------------------------
    virtual void defineColourTextureShaderInput(int shader_id);
};

} // namespace gVirtualXRay

#endif // GVXR_DIAPHRAGM_MESH_H
