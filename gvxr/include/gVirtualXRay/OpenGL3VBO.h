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


#ifndef GVXR_OPENGL3_VBO_H
#define GVXR_OPENGL3_VBO_H


/**
********************************************************************************
*
*   @file       OpenGL3VBO.h
*
*   @brief      Class to handle vertex buffer objects (VBOs) in
*               OpenGL 3.x or 4.x.
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
#ifndef GVXR_VBO_H
#include "gVirtualXRay/VBO.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*   @class  OpenGL3VBO
*   @brief  OpenGL3VBO is a class to handle a vertex buffer object (VBO) in
*           OpenGL 3.x or 4.x.
*/
//==============================================================================
class OpenGL3VBO: public VBO
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default constructor.
    //--------------------------------------------------------------------------
    OpenGL3VBO();
    
    
    //--------------------------------------------------------------------------
    /// Destructor.
    //--------------------------------------------------------------------------
    virtual ~OpenGL3VBO();


    //--------------------------------------------------------------------------
    /// Release the data on the GPU.
    //--------------------------------------------------------------------------
    virtual void destroy();


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
            const void* apAdditionalData = 0);


	//--------------------------------------------------------------------------
	/// Set the vertex buffer objects.
	/**
	*  @param aNumberOfVertices:   number of vertices
	*  @param aVertexVBO		   identificator of vbo for vertices
	*  @param aNormalVBO		   identificator of vbo for normals
	*  @param aTextureVBO		   identificator of vbo for texture coords
	*/
	//--------------------------------------------------------------------------
	virtual void setVertexVBO(
		unsigned int aNumberOfVertices,
		unsigned int aVertexVBO,
		unsigned int aNormalVBO,
		unsigned int aTextureVBO
		);

	//--------------------------------------------------------------------------
	/// Set the index.
	/**
	*  @param aNumberOfIndexes:    number of indices
	*  @param anIndexDataType:     the data type of indices;
	*                              valid values are GL_UNSGINED_BYTE,
	*                              GL_UNSGINED_SHORT and GL_UNSGINED_INT
	*  @param anIndexVBO:          array containing the index data
	*/
	//--------------------------------------------------------------------------
	virtual void setIndexVBO(
		unsigned int aNumberOfIndexes,
		int anIndexDataType,
		unsigned int anIndexVBO);




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
            const void* apIndexSet);


    //--------------------------------------------------------------------------
    /// Display the triangular mesh using OpenGL.
    //--------------------------------------------------------------------------
    virtual void display() const;


    //--------------------------------------------------------------------------
    /// Copy operator.
    /**
     *  @param aVbo:    VBO to copy
     *  @return the new VBO
     */
    //--------------------------------------------------------------------------
    VBO& operator=(const OpenGL3VBO& aVbo);


//******************************************************************************
protected:
    /// ID of VAO
    unsigned int m_vao_id;
};


} // namespace gVirtualXRay


//******************************************************************************
#include "OpenGL3VBO.inl"


#endif // GVXR_OPENGL3_VBO_H
