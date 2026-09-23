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


#ifndef GVXR_DEPTH_MAP_H
#define GVXR_DEPTH_MAP_H


/**
*******************************************************************************
*
*   @file       DepthMap.h
*
*   @brief      Class to render to a buffer in order to create, for exemple,
*               a shadow map.
*
*   @version    1.0
*
*   @date       19/07/2016
*
*   @author     Andreas Meuleman
*
*******************************************************************************
*/

//******************************************************************************
//  Include
//******************************************************************************
#include <vector>

#ifndef GVXR_MATRIX_4X4_H
#include "gVirtualXRay/Matrix4x4.h"
#endif

#ifndef GVXR_POLYGON_MESH_H
#include "gVirtualXRay/PolygonMesh.h"
#endif

#ifndef GVXR_SHADER_H
#include "gVirtualXRay/Shader.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {

//==============================================================================
/**
*   @class  DepthMap
*   @brief  DepthMap is a class to render to a buffer in order to create,
*           for exemple, a shadow map.
*/
//==============================================================================
class DepthMap {

public:
    //--------------------------------------------------------------------------
    /// default constructor
    /*
    *   @param texture_size: size of the side of the buffer (default: 8192)
    */
    //--------------------------------------------------------------------------
    DepthMap(int map_size = 8192);


    //--------------------------------------------------------------------------
    /// destructor
    //--------------------------------------------------------------------------
    ~DepthMap();


    void release();

    
    //--------------------------------------------------------------------------
    ///Initialize the buffers
    //--------------------------------------------------------------------------
    void initialize();


    //--------------------------------------------------------------------------
    /// render to the frame buffer, call initialize() if it has never
    /// been called before/ Use this overload in case there is one
    /// model matrixes for all the meshes
    /**
     *  @param mesh_set: the Polygon mesh set to be rendered
     *  @param proj_matrix: the projection matrix
     */
    //--------------------------------------------------------------------------
    void render(std::vector<PolygonMesh*>& mesh_set,
            Matrix4x4<float> const& proj_matrix);


    //--------------------------------------------------------------------------
    ///render to the frame buffer, call initialize() if it has never
    /// been called before. Use this overload in case there are
    /// different model matrixes
    /**
     *  @param mesh_set: the Polygon mesh set to be rendered
     *  @param proj_matrix: the projection matrix
     *  @param model_matrix_set: the model matrix set
     */
    //--------------------------------------------------------------------------
    void render(std::vector<PolygonMesh*>& mesh_set,
            Matrix4x4<float> const& proj_matrix,
            std::vector< Matrix4x4<float> > const& model_matrix_set);
    

    //--------------------------------------------------------------------------
    ///Get the texture
    //--------------------------------------------------------------------------
    unsigned int getMapTexture() const;


//******************************************************************************
private:
    /// No copy constructor
    DepthMap(DepthMap const&);


    /// to know if initialize() has been called
    bool m_is_initialized;


    Shader m_depth_shader;


    /// To create a buffer for the scene map
    unsigned int    m_map_frame_buffer,
        m_map_texture;


    int m_map_size;
};


} // namespace gVirtualXRay


#endif // GVXR_DEPTH_MAP_H
