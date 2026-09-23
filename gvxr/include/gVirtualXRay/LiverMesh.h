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


#ifndef GVXR_LIVER_MESH_H
#define GVXR_LIVER_MESH_H


/**
********************************************************************************
*
*   @file       LiverMesh.h
*
*   @brief      Class to handle a polygon mesh for a liver. It uses 3D texture
*               for the bump map and 2D texture for the colour
*
*   @version    1.0
*
*   @date       13/01/2017
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
*   @class  LiverMesh
*   @brief  LiverMesh is a class to handle a polygon mesh for a liver.
*           It uses 3D texture for the bump map and 2D texture for the colour
*/
//==============================================================================
class LiverMesh: public SoftTissueMesh
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default Constructor
    /*
     *  @param texture_2D_size: size of the side of the buffer of the 2D texture
     *                          (default: 512)
     *  @param texture_3D_size: size of the side of the buffer of the 3D texture
     *                          (default: 32)
     *  @param optimize_texture_coordinates:    To know if the texture
     *                                          coordinates need to be optimized
     *                                          (default: true)
     */
    //--------------------------------------------------------------------------
    LiverMesh(bool optimize_texture_coordinates = true,
              unsigned int texture_2D_size = 512,
              unsigned int texture_3D_size = 32);

    
    //--------------------------------------------------------------------------
    /// Destructor
    //--------------------------------------------------------------------------
    virtual ~LiverMesh();
};

    
} // namespace gVirtualXRay


#endif // GVXR_LIVER_MESH_H
