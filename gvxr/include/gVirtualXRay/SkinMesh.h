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


#ifndef GVXR_SKIN_MESH_H
#define GVXR_SKIN_MESH_H


/**
********************************************************************************
*
*   @file       SkinMesh.h
*
*   @brief      Class to handle a polygon mesh for Skin.
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
#ifndef GVXR_ANATOMICAL_MESH_H
#include "gVirtualXRay/AnatomicalMesh.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*   @class  SkinMesh
*   @brief  SkinMesh is a class to handle a polygon mesh for Skin.
*/
//==============================================================================
class SkinMesh: public AnatomicalMesh
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default Constructor
    //--------------------------------------------------------------------------
    SkinMesh();

    
    //--------------------------------------------------------------------------
    /// Destructor
    //--------------------------------------------------------------------------
    virtual ~SkinMesh();

    
    //--------------------------------------------------------------------------
    /// return true if the mesh has to be displayed with transparency
    //--------------------------------------------------------------------------
    virtual bool isTransparent() const;

    
    //--------------------------------------------------------------------------
    /// return true if the mesh represent a soft tissue
    //--------------------------------------------------------------------------
    virtual bool isSoftTissue() const;

    
    //--------------------------------------------------------------------------
    /// Merge the vertices for optimization and a smoother display
    //--------------------------------------------------------------------------
    virtual void initialize();
};

    
} // namespace gVirtualXRay


#endif // GVXR_SKIN_MESH_H
