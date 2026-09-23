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


#ifndef GVXR_VBO_h
#define GVXR_VBO_h


/**
********************************************************************************
*
*   @file       VBO.h
*
*   @brief      Class to handle vertex buffer objects (VBOs).
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


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*   @class  VBO
*   @brief  VBO is a class to handle a vertex buffer object (VBO).
*/
//==============================================================================
class VBO
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default constructor.
    //--------------------------------------------------------------------------
    VBO();
    
    
    //--------------------------------------------------------------------------
    /// Destructor.
    //--------------------------------------------------------------------------
    virtual ~VBO();
    
    
    //--------------------------------------------------------------------------
    /// Release the data on the GPU.
    //--------------------------------------------------------------------------
    virtual void destroy();


    //--------------------------------------------------------------------------
    /// Reset the data.
    //--------------------------------------------------------------------------
    void reset();
    
    
    //--------------------------------------------------------------------------
    /// Set the vertex data.
    /**
     *  @param aPrimitiveType:  the primitive type; valid values are:
     *                          GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP,
     *                          GL_LINES, GL_LINE_STRIP_ADJACENCY,
     *                          GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP,
     *                          GL_TRIANGLE_FAN, GL_TRIANGLES,
     *                          GL_TRIANGLE_STRIP_ADJACENCY,
     *                          GL_TRIANGLES_ADJACENCY and GL_PATCHES
     *  @param aNumberOfVertices:   number of vertices (default: 0)
     *  @param aNumberOfCompontentsPerVertex:   the number of component per
     *                                          vertex (default: 0)
     *  @param aVertexDataType: the data type of vertices; valid values are
     *                          GL_FLOAT and GL_DOUBLE (default: 0)
     *  @param apVertices:  address of the first component of the first vertex
     *                      (default: 0)
     *  @param aNumberOfNormals:    number of normal vectors (default: 0)
     *  @param aNumberOfCompontentsPerNormal:   the number of component per
     *                                          normal vector (default: 0)
     *  @param aNormalDataType:     the data type of normal vectors; valid
     *                              values are GL_FLOAT and GL_DOUBLE
     *                              (default: 0)
     *  @param apNormals:   address of the first component of the firth normal
     *                      vector (default: 0)
     *  @param aNumberOfTextureCoordinates:     number of texture coordinates
     *                                          (default: 0)
     *  @param aNumberOfCompontentsPerCoord:    the number of component per
     *                                          texture coordinate (default: 0)
     *  @param aCoordDataType:  the data type of texture coordinates; valid
     *                          values are GL_FLOAT and GL_DOUBLE (default: 0)
     *  @param apTextureCoordinates:    address of the first component of
     *                                  the first texture coordinate
     *                                  (default: 0)
     *  @param aNumberOfAdditionalData: number of additional data (default: 0)
     *  @param aNumberOfCompontentsPerAdditionalData:   the number of component
     *                                                  per additional data
     *                                                  (default: 0)
     *  @param anAdditionalDataType:    the data type of additional data;
     *                                  valid values are GL_FLOAT and GL_DOUBLE
     *                                  (default: 0)
     *  @param apAdditionalData:    address of the first component of
     *                              the first additional data (default: 0)
     */
    //--------------------------------------------------------------------------
    virtual void setVertexData(int aPrimitiveType,
            const unsigned int aNumberOfVertices = 0,
            unsigned int aNumberOfCompontentsPerVertex = 0,
            int aVertexDataType = 0,
            const void* apVertices = 0,
            const unsigned int aNumberOfNormals = 0,
            unsigned int aNumberOfCompontentsPerNormal = 0,
            int aNormalDataType = 0,
            const void* apNormals = 0,
            const unsigned int aNumberOfTextureCoordinates = 0,
            unsigned int aNumberOfCompontentsPerCoord = 0,
            int aCoordDataType = 0,
            const void* apTextureCoordinates = 0,
            const unsigned int aNumberOfAdditionalData = 0,
            unsigned int aNumberOfCompontentsPerAdditionalData = 0,
            int anAdditionalDataType = 0,
            const void* apAdditionalData = 0) = 0;


    //--------------------------------------------------------------------------
    /// Set the index.
    /**
     *  @param aNumberOfIndexes:    number of indices
     *  @param anIndexDataType:     the data type of indices;
     *                              valid values are GL_UNSGINED_BYTE,
     *                              GL_UNSGINED_SHORT and GL_UNSGINED_INT
     *  @param apIndexSet:          the index set
     */
    //--------------------------------------------------------------------------
    virtual void setIndexData(const unsigned int aNumberOfIndexes,
                              int anIndexDataType,
                              const void* apIndexSet) = 0;


    //--------------------------------------------------------------------------
    /// Display the triangular mesh using OpenGL.
    //--------------------------------------------------------------------------
    virtual void display() const = 0;


    //--------------------------------------------------------------------------
    /// Check if the vertex buffer object is ready.
    /**
     *  @return true if the VBO is ready to use, false if it is not
     */
    //--------------------------------------------------------------------------
    bool isReady() const;
    
    
    //--------------------------------------------------------------------------
    /// Set the buffer usage hint. Accepted values are:
    ///     GL_STATIC_DRAW is for vertex buffers that are rendered many times,
    ///             and whose contents are specified once and never change.
    ///     GL_DYNAMIC_DRAW is for vertex buffers that are rendered many times,
    ///             and whose contents change during the rendering loop.
    ///     GL_STREAM_DRAW is for vertex buffers that are rendered
    ///             a small number of times and then discarded.
    /**
     *  @param aFlag: the buffer usage hint to set
     */
    //--------------------------------------------------------------------------
    void setBufferUsageHint(int aFlag);


    //--------------------------------------------------------------------------
    /// Accessor on the buffer usage hint.
    /**
     *  @return the buffer usage hint
     */
    //--------------------------------------------------------------------------
    int getBufferUsageHint() const;


    //--------------------------------------------------------------------------
    /// Accessor on the primitive type.
    /**
     *  @return the primitive type
     */
    //--------------------------------------------------------------------------
    int getPrimitiveType() const;


    //--------------------------------------------------------------------------
    /// Copy operator.
    /**
     *  @param aVbo:    VBO to copy
     *  @return the new VBO
     */
    //--------------------------------------------------------------------------
    VBO& operator=(const VBO& aVbo);

    
    //--------------------------------------------------------------------------
    /// Accessor on the major version of OpenGL that was used to create the VBO.
    /**
     *  @return the major version of OpenGL
     */
    //--------------------------------------------------------------------------
    unsigned int getOpenglMajorVersion() const;


//******************************************************************************
protected:
    /// ID of VBOs
    unsigned int m_id_set[5];


    /// Number of normal vectors
    unsigned int m_number_of_normals;


    /// Number of components per normal
    unsigned int m_number_of_compontents_per_normal;


    /// Number of additional data
    unsigned int m_number_of_additional_data;


    /// Number of components per additional data
    unsigned int m_number_of_compontents_per_additional_data;


    /// Number of texture coordinates
    unsigned int m_number_of_texture_coordinates;


    /// Number of components per texture coordinate
    unsigned int m_number_of_compontents_per_texture_coord;


    /// Number of vertices
    unsigned int m_number_of_vertices;


    /// Number of components per vertex
    unsigned int m_number_of_compontents_per_vertex;


    /// Number of indices
    unsigned int m_number_of_indices;


    /// Size in bytes of the array of normal vectors
    unsigned int m_normal_size;


    /// Size in bytes of the array of additional data
    unsigned int m_additional_data_size;


    /// Size in bytes of the array of texture coordinates
    unsigned int m_texture_coordinate_size;


    /// Size in bytes of the array of vertices
    unsigned int m_vertex_size;


    /// Size in bytes of the array of indices
    unsigned int m_index_size;


    /// Data type of normal vectors; valid values are GL_FLOAT and GL_DOUBLE
    int m_normal_data_type;

    /// Data type of additional data; valid values are GL_FLOAT and GL_DOUBLE
    int m_additional_data_type;


    /// Data type of texture coordinates; valid values are
    /// GL_FLOAT and GL_DOUBLE
    int m_texture_coordinate_data_type;


    /// Data type of vertices; valid values are GL_FLOAT and GL_DOUBLE
    int m_vertex_data_type;


    /// Data type of indices; valid values are GL_UNSIGNED_BYTE,
    /// GL_UNSIGNED_SHORT and GL_UNSIGNED_INT
    int m_index_data_type;


    /// Type of primitive to draw; valid values are  GL_POINTS, GL_LINE_STRIP,
    /// GL_LINE_LOOP, GL_LINES, GL_LINE_STRIP_ADJACENCY, GL_LINES_ADJACENCY,
    /// GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES,
    /// GL_TRIANGLE_STRIP_ADJACENCY, GL_TRIANGLES_ADJACENCY and GL_PATCHES
    int m_primitive_type;


    /// The buffer usage hint; valid values are GL_DYNAMIC_DRAW,
    /// GL_STATIC_DRAW and GL_STREAM_DRAW
    int m_storage_type;


    /// The major number of the OpenGL version used to create the VBO.
    /// 2 or 3 are supported. We do not distinguish at this level between 3 and 4.
    unsigned int m_opengl_major_version;
};


} // namespace gVirtualXRay


//******************************************************************************
#include "VBO.inl"


#endif // GVXR_VBO_h
