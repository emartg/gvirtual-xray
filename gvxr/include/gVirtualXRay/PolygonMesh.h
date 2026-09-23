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


#ifndef GVXR_POLYGON_MESH_H
#define GVXR_POLYGON_MESH_H


/**
********************************************************************************
*
*   @file       PolygonMesh.h
*
*   @brief      Class to handle polygon (triangles) meshes.
*
*   @version    1.0
*
*   @date       13/01/2017
*
*   @author     Dr Franck P. Vidal
*
*   @todo       Make it possible to merge vertices if an array of indices exists
*               Convert triangle mesh into arbitrary polygon mesh
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
#include <string>
#include <vector>
#include <map>
#include <memory>

#ifndef GVXR_VBO_h
#include "gVirtualXRay/VBO.h"
#endif

#ifndef GVXR_TYPES_H
#include "gVirtualXRay/Types.h"
#endif

#ifndef GVXR_UNITS_H
#include "gVirtualXRay/Units.h"
#endif

#ifndef GVXR_MATERIAL_H
#include "gVirtualXRay/Material.h"
#endif

#ifndef GVXR_PHOTON_CROSS_SECTION_H
#include "gVirtualXRay/PhotonCrossSection.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*   @class  PolygonMesh
*   @brief  PolygonMesh is a class to handle polygon (triangles) meshes.
*/
//==============================================================================
class PolygonMesh
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default constructor.
    //--------------------------------------------------------------------------
    PolygonMesh();


    //--------------------------------------------------------------------------
    /// Copy constructor.
    /**
     * @param aPolygonMesh:    the polygon mesh to copy
     */
    //--------------------------------------------------------------------------
    PolygonMesh(const PolygonMesh& aPolygonMesh);


    //--------------------------------------------------------------------------
    /// Destructor.
    //--------------------------------------------------------------------------
    ~PolygonMesh();


    //--------------------------------------------------------------------------
    /// Reset the data.
    //--------------------------------------------------------------------------
    void reset();


    //--------------------------------------------------------------------------
    /// Reset the VBOs only.
    //--------------------------------------------------------------------------
    void resetVBOs();


    //--------------------------------------------------------------------------
    /// Set if the mesh is displayed or not
    /**
     * @param display:  true or false
     */
    //--------------------------------------------------------------------------
    void setDisplayFlag(bool display);


    //--------------------------------------------------------------------------
    /// Accessor on the display flag of the polygon mesh.
    /**
     * @return display flag
     */
    //--------------------------------------------------------------------------
    bool getDisplayFlag() const;


    //--------------------------------------------------------------------------
    /// Display the triangular mesh using OpenGL.
    //--------------------------------------------------------------------------
    void display();


    //--------------------------------------------------------------------------
    /// Display the triangular mesh in wireframe using OpenGL.
    //--------------------------------------------------------------------------
    void displayWireFrame();


    //--------------------------------------------------------------------------
    /// Display the face normal vectors.
    /**
     * @param aNormalSize: the size of the vectors (default value is 0.5 cm)
     */
    //--------------------------------------------------------------------------
    void displayFaceNormal(RATIONAL_NUMBER aNormalSize = 0.5 * cm);


    //--------------------------------------------------------------------------
    /// Display the vertex normal vectors.
    /**
     * @param aNormalSize: the size of the vectors (default value is 0.5 cm)
     */
    //--------------------------------------------------------------------------
    void displayVertexNormal(RATIONAL_NUMBER aNormalSize = 0.5 * cm);


    //--------------------------------------------------------------------------
    /// Set the name of the file that contains the polygon mesh.
    /**
     * @param aFilename:   the file name
     */
    //--------------------------------------------------------------------------
    void setFilename(const char* aFilename);


    //--------------------------------------------------------------------------
    /// Set the name of the file that contains the polygon mesh.
    /**
     * @param aFilename:   the file name
     */
    //--------------------------------------------------------------------------
    void setFilename(const std::string& aFilename);


    //--------------------------------------------------------------------------
    /// Set the name of the file that contains the polygon mesh.
    /**
     * @param aFilename:   the file name
     */
    //--------------------------------------------------------------------------
    void setFileName(const char* aFilename);


    //--------------------------------------------------------------------------
    /// Set the name of the file that contains the polygon mesh.
    /**
     * @param aFilename:   the file name
     */
    //--------------------------------------------------------------------------
    void setFileName(const std::string& aFilename);


    //--------------------------------------------------------------------------
    /// Accessor on the name of the file that contains the polygon mesh.
    /**
     * @return the file name
     */
    //--------------------------------------------------------------------------
    const std::string& getFilename() const;


    //------------------------------------------------------------------------------
    /// Accessor on the unit of length of the mesh.
    /**
     *  @return the unit of length
     */
    //------------------------------------------------------------------------------
    const RATIONAL_NUMBER& getUnitOfLength() const;


    //--------------------------------------------------------------------------
    /// Set the name of the file that contains the polygon mesh.
    /**
     * @param aMoveToCentreFlag:       a flag to move the polygon to the center
     *                                  or not
     * @param anAutoComputeNormalFlag: a flag to compute normals or not
     * @param aPrintDebugInfoFlag:     a flag to display additional debug
     *                                  information in the console or not
     * @param aCreateVBOFlag:          a flag to use a vertex buffer object or
     *                                  not
     * @param aScale:                  the unit in which data is stored in
     *                                  the file (e.g. mm, cm, m)
     * @param aBufferUsageHing:        the buffer usage hint; valid values are:
     *                                  GL_STATIC_DRAW, GL_DYNAMIC_DRAW and
     *                                  GL_STREAM_DRAW
     */
    //--------------------------------------------------------------------------
    void loadSTLFile(bool aMoveToCentreFlag,
        bool anAutoComputeNormalFlag,
        bool aPrintDebugInfoFlag,
        bool aCreateVBOFlag,
        RATIONAL_NUMBER aScale,
        int aBufferUsageHing);


    //--------------------------------------------------------------------------
    /// Write the STL file.
    /**
     *  @param aBinaryFlag: if true, save a binary file; if false save an ASCII file
     */
    //--------------------------------------------------------------------------
    void writeSTLFile(bool aBinaryFlag);


    //--------------------------------------------------------------------------
    /// Move the polygon to the centre.
    //--------------------------------------------------------------------------
    void moveToCentre();


    //--------------------------------------------------------------------------
    /// Move the polygon to the center.
    //--------------------------------------------------------------------------
    void moveToCenter();


    //--------------------------------------------------------------------------
    /// Compute the normal vectors using the cross product method.
    //--------------------------------------------------------------------------
    void computeNormalVectors();


    //--------------------------------------------------------------------------
    /// Use a vertex buffer object if possible.
    /**
     * @param aBufferUsageHing:    the buffer usage hint; valid values are:
     *                             GL_STATIC_DRAW, GL_DYNAMIC_DRAW and
     *                             GL_STREAM_DRAW
     * @param aTypeOfPrimitive:    type of primitive;
     *                             valid values are GL_TRIANGLES
     * @return true if the VBO can be used, false otherwise
     */
    //--------------------------------------------------------------------------
    bool useVBO(int aBufferUsageHing, int aTypeOfPrimitive);


    void setHU(short HU);
    void setHounsfieldUnit(short HU);
    void setHounsfieldValue(short HU);
    void setElement(unsigned short Z);
    void setElement(const std::string& aName);
    void setMixture(const Mixture& aMixture);
    void setMixture(const std::map<int, double>& aMixture);
    void setMixture(const std::map<std::string, double>& aMixture);
    void setMixture(const std::string& aName);
    void setCompound(const std::string& aName);
    void setMassAttenuationCoefficient(double aCoefficient);
    void setLinearAttenuationCoefficient(double aCoefficient);
    void setDensity(double aDensity);
    double getDensity();
    const std::string& getMaterialLabel() const;


    //------------------------------------------------------------------------------
    /// Accessor on the compound description of the polygon mesh.
    /**
     *  @return if the sample is made of a compound, then return the description of the compound (e.g. H2O for water), otherwise return an empty string
     */
     //------------------------------------------------------------------------------
    std::string getCompound() const;


    //------------------------------------------------------------------------------
    /// Accessor on the element Z number set of the mixture of the polygon mesh.
    /**
     *  @return if the sample is made of a mixture, then return the element Z number set of the mixture, otherwise return an empty vector
     */
     //------------------------------------------------------------------------------
    std::vector<int> getMixtureElementSet() const;


    //------------------------------------------------------------------------------
    /// Accessor on the element weight set of the mixture of the polygon mesh.
    /**
     *  @return if the sample is made of a mixture, then return the element weight set of the mixture, otherwise return an empty vector
     */
     //------------------------------------------------------------------------------
    std::vector<double> getMixtureWeightSet() const;


    const PhotonCrossSection& getPhotonCrossSection() const;


    void setPhotonCrossSection(const PhotonCrossSection& aPhotonCrossSection);


    //--------------------------------------------------------------------------
    /// Accessor on the bounding box lower corner.
    /**
     * @return the bounding box lower corner
     */
    //--------------------------------------------------------------------------
    const VEC3& getLocalMinCorner() const;


    //--------------------------------------------------------------------------
    /// Accessor on the bounding box upper corner.
    /**
     * @return the bounding box upper corner
     */
    //--------------------------------------------------------------------------
    const VEC3& getLocalMaxCorner() const;


    //--------------------------------------------------------------------------
    /// Translate the polygon mesh given a translation vector, and
    /// update the VBO if needed.
    /**
     * @param aTranslationVector:  the translation vector
     */
    //--------------------------------------------------------------------------
    void applyTranslation(const VEC3& aTranslationVector);


    //--------------------------------------------------------------------------
    /// Scale the polygon mesh given scaling factors, and
    /// update the VBO if needed.
    /**
     * @param x:    the scaling factor along the X-axis
     * @param y:    the scaling factor along the Y-axis
     * @param z:    the scaling factor along the Z-axis
     */
    //--------------------------------------------------------------------------
    void applyScaling(RATIONAL_NUMBER x, RATIONAL_NUMBER y, RATIONAL_NUMBER z);


    //--------------------------------------------------------------------------
    /// Rotate the polygon mesh given a rotation axis and angle, and
    /// update the VBO if needed.
    /**
     * @param aRotationAxis:  the rotation axis
     * @param aRotationAngleInDegrees:  the rotation angle in degrees
     */
    //--------------------------------------------------------------------------
    void applyRotation(const VEC3& aRotationAxis,
                RATIONAL_NUMBER aRotationAngleInDegrees);


    //--------------------------------------------------------------------------
    /// Transform the polygon mesh given a transformation matrix and
    /// update the VBO if needed.
    /**
     * @param aTransformationMatrix:  the transformation matrix
     */
    //--------------------------------------------------------------------------
    void applyTransform(const MATRIX4& aTransformationMatrix);


    //--------------------------------------------------------------------------
    /// Accessor on the number of faces.
    /**
     * @return the number of faces
     */
    //--------------------------------------------------------------------------
    unsigned int getFaceNumber() const;


    //--------------------------------------------------------------------------
    /// Accessor on the number of triangles.
    /**
     * @return the number of triangles
     */
    //--------------------------------------------------------------------------
    unsigned int getTriangleNumber() const;


    //--------------------------------------------------------------------------
    /// Accessor on the number of face normal vectors.
    /**
     * @return the number of face normal vectors
     */
    //--------------------------------------------------------------------------
    unsigned int getFaceNormalNumber() const;


    //--------------------------------------------------------------------------
    /// Accessor on the number of vertex normal vectors.
    /**
     * @return the number of vertex normal vectors
     */
    //--------------------------------------------------------------------------
    unsigned int getVertexNormalNumber() const;


    //--------------------------------------------------------------------------
    /// Accessor on the number of vertices.
    /**
     * @return the number of vertices
     */
    //--------------------------------------------------------------------------
    unsigned int getVertexNumber() const;


    //--------------------------------------------------------------------------
    /// Accessor on the number of indices.
    /**
     * @return the number of indices
     */
    //--------------------------------------------------------------------------
    unsigned int getIndexNumber() const;


    //--------------------------------------------------------------------------
    /// Merge the vertices of the mesh if the data is managed by the
    /// class instance, and if no index has been build.
    //--------------------------------------------------------------------------
    void mergeVertices(bool aPrintDebugInfoFlag = false);


    //--------------------------------------------------------------------------
    /// Split each face into four faces
    //--------------------------------------------------------------------------
    void splitFaces(bool aPrintDebugInfoFlag = false);


    //--------------------------------------------------------------------------
    /// Accessor on a given vertex.
    /**
     * @param anIndex: the index of the vertex to access
     * @return the vertex
     */
    //--------------------------------------------------------------------------
    VEC3 getVertex(unsigned int anIndex) const;


    //--------------------------------------------------------------------------
    /// Change the value of a given vertex.
    /**
     * @param anIndex: the index of the vertex to access
     * @param aVertex: the new value of the vertex
     */
    //--------------------------------------------------------------------------
    void setVertex(unsigned int anIndex, const VEC3& aVertex);
    void setFaceNormal(unsigned int anIndex, const VEC3& aNormalVector);
    void setVertexNormal(unsigned int anIndex, const VEC3& aNormalVector);


    //--------------------------------------------------------------------------
    /// Accessor on a given index.
    /**
     * @param anIndex: the index of the index to access
     * @return the index
     */
    //--------------------------------------------------------------------------
    int getIndex(unsigned int anIndex) const;


    //--------------------------------------------------------------------------
    /// Set the value of a given index.
    /**
     * @param anIndex: the index of the index to access
     * @param aValue: the new value of the index
     */
    //--------------------------------------------------------------------------
    void setIndex(unsigned int anIndex, unsigned int aValue);


    //--------------------------------------------------------------------------
    /// Accessor on a given face normal.
    /**
     * @param anIndex: the index of the normal to access
     *  @return the normal
     */
    //--------------------------------------------------------------------------
    VEC3 getFaceNormal(unsigned int anIndex) const;


    //--------------------------------------------------------------------------
    /// Accessor on a given vertex normal.
    /**
     *  @param anIndex: the index of the normal to access
     *  @return the normal
     */
    //--------------------------------------------------------------------------
    VEC3 getVertexNormal(unsigned int anIndex) const;


    //--------------------------------------------------------------------------
    /// Set the polygon mesh using external data. The class instance will not
    /// manage the memory.
    /**
     *  @param aTypeOfPrimitive:    type of primitive; valid values are
     *                              GL_TRIANGLES
     *  @param aVertexArray:        array containing the vertex data
     *  @param aNumberOfVertices:   number of vertices
     *  @param aVertexDataType:     data type of the vertex data; valid values
     *                              are GL_FLOAT, GL_DOUBLE and GL_NONE
     *  @param anIndexArray:        array containing the index data
     *  @param aNumberOfIndices:    number of indices
     *  @param anIndexDataType:     data type of the index data; valid values
     *                              are GL_UNSIGNED_INT, GL_UNSIGNED_SHORT,
     *                              GL_UNSIGNED_BYTE and GL_NONE
     *  @param aCreateVBOFlag:      flag set to true to create a VBO,
     *                              set to false to not set a VBO
     *  @param aTypeOfVBO:          VBO usage hint to set; valid values are
     *                              GL_STATIC_DRAW, GL_DYNAMIC_DRAW and
     *                              GL_STREAM_DRAW
     */
    //--------------------------------------------------------------------------
    void setExternalData(int aTypeOfPrimitive,
            const void* aVertexArray,
            unsigned int aNumberOfVertices,
            int aVertexDataType,
            const void* anIndexArray,
            unsigned int aNumberOfIndices,
            int anIndexDataType,
            bool aCreateVBOFlag,
            int aTypeOfVBO);


    //--------------------------------------------------------------------------
    /// Set the polygon mesh using external data. The class instance will not
    /// manage the memory.
    /**
     *  @param aTypeOfPrimitive:    type of primitive; valid values are
     *                              GL_TRIANGLES
     *  @param aVertexArray:        array containing the vertex data
     *  @param aCreateVBOFlag:      flag set to true to create a VBO,
     *                              set to false to not set a VBO
     *  @param aTypeOfVBO:          VBO usage hint to set; valid values are
     *                              GL_STATIC_DRAW, GL_DYNAMIC_DRAW and
     *                              GL_STREAM_DRAW
     */
    //--------------------------------------------------------------------------
    void setExternalData(int aTypeOfPrimitive,
            const std::vector<float>* aVertexArray,
            bool aCreateVBOFlag,
            int aTypeOfVBO);


    //--------------------------------------------------------------------------
    /// Set the polygon mesh using external data. The class instance will not
    /// manage the memory.
    /**
     *  @param aTypeOfPrimitive:    type of primitive; valid values are
     *                              GL_TRIANGLES
     *  @param aVertexArray:        array containing the vertex data
     *  @param aCreateVBOFlag:      flag set to true to create a VBO,
     *                              set to false to not set a VBO
     *  @param aTypeOfVBO:          VBO usage hint to set; valid values are
     *                              GL_STATIC_DRAW, GL_DYNAMIC_DRAW and
     *                              GL_STREAM_DRAW
     */
    //--------------------------------------------------------------------------
    void setExternalData(int aTypeOfPrimitive,
            const std::vector<double>* aVertexArray,
            bool aCreateVBOFlag,
            int aTypeOfVBO);


    //--------------------------------------------------------------------------
    /// Set the polygon mesh using external data. The class instance will not
    /// manage the memory.
    /**
     *  @param aTypeOfPrimitive:    type of primitive; valid values are
     *                              GL_TRIANGLES
     *  @param aVertexArray:        array containing the vertex data
     *  @param anIndexArray:        array containing the index data
     *  @param aCreateVBOFlag:      flag set to true to create a VBO,
     *                              set to false to not set a VBO
     *  @param aTypeOfVBO:          VBO usage hint to set; valid values are
     *                              GL_STATIC_DRAW, GL_DYNAMIC_DRAW and
     *                              GL_STREAM_DRAW
     */
    //--------------------------------------------------------------------------
    void setExternalData(int aTypeOfPrimitive,
            const std::vector<float>* aVertexArray,
            const std::vector<unsigned int>* anIndexArray,
            bool aCreateVBOFlag,
            int aTypeOfVBO);


    //--------------------------------------------------------------------------
    /// Set the polygon mesh using external data. The class instance will not
    /// manage the memory.
    /**
     *  @param aTypeOfPrimitive:    type of primitive;
     *                              valid values are GL_TRIANGLES
     *  @param aVertexArray:        array containing the vertex data
     *  @param anIndexArray:        array containing the index data
     *  @param aCreateVBOFlag:      flag set to true to create a VBO,
     *                              set to false to not set a VBO
     *  @param aTypeOfVBO:          VBO usage hint to set; valid values are
     *                              GL_STATIC_DRAW, GL_DYNAMIC_DRAW and
     *                              GL_STREAM_DRAW
     */
    //--------------------------------------------------------------------------
    void setExternalData(int aTypeOfPrimitive,
            const std::vector<double>* aVertexArray,
            const std::vector<unsigned int>* anIndexArray,
            bool aCreateVBOFlag,
            int aTypeOfVBO);


    //--------------------------------------------------------------------------
    /// Set the polygon mesh using external data. The class instance will not
    /// manage the memory.
    /**
     *  @param aTypeOfPrimitive:    type of primitive;
     *                              valid values are GL_TRIANGLES
     *  @param aVertexArray:        array containing the vertex data
     *  @param anIndexArray:        array containing the index data
     *  @param aCreateVBOFlag:      flag set to true to create a VBO,
     *                              set to false to not set a VBO
     *  @param aTypeOfVBO:          VBO usage hint to set; valid values are
     *                              GL_STATIC_DRAW, GL_DYNAMIC_DRAW and
     *                              GL_STREAM_DRAW
     */
    //--------------------------------------------------------------------------
    void setExternalData(int aTypeOfPrimitive,
            const std::vector<float>* aVertexArray,
            const std::vector<unsigned short>* anIndexArray,
            bool aCreateVBOFlag,
            int aTypeOfVBO);


    //--------------------------------------------------------------------------
    /// Set the polygon mesh using external data. The class instance will not
    /// manage the memory.
    /**
     *  @param aTypeOfPrimitive:    type of primitive;
     *                              valid values are GL_TRIANGLES
     *  @param aVertexArray:        array containing the vertex data
     *  @param anIndexArray:        array containing the index data
     *  @param aCreateVBOFlag:      flag set to true to create a VBO,
     *                              set to false to not set a VBO
     *  @param aTypeOfVBO:          VBO usage hint to set; valid values are
     *                              GL_STATIC_DRAW, GL_DYNAMIC_DRAW and
     *                              GL_STREAM_DRAW
     */
    //--------------------------------------------------------------------------
    void setExternalData(int aTypeOfPrimitive,
            const std::vector<double>* aVertexArray,
            const std::vector<unsigned short>* anIndexArray,
            bool aCreateVBOFlag,
            int aTypeOfVBO);


    //--------------------------------------------------------------------------
    /// Set the polygon mesh using external data. The class instance will not
    /// manage the memory.
    /**
     *  @param aTypeOfPrimitive:    type of primitive;
     *                              valid values are GL_TRIANGLES
     *  @param aVertexArray:        array containing the vertex data
     *  @param anIndexArray:        array containing the index data
     *  @param aCreateVBOFlag:      flag set to true to create a VBO,
     *                              set to false to not set a VBO
     *  @param aTypeOfVBO:          VBO usage hint to set; valid values are
     *                              GL_STATIC_DRAW, GL_DYNAMIC_DRAW and
     *                              GL_STREAM_DRAW
     */
    //--------------------------------------------------------------------------
    void setExternalData(int aTypeOfPrimitive,
            const std::vector<float>* aVertexArray,
            const std::vector<unsigned char>* anIndexArray,
            bool aCreateVBOFlag,
            int aTypeOfVBO);


    //--------------------------------------------------------------------------
    /// Set the polygon mesh using external data. The class instance will not
    /// manage the memory.
    /**
     *  @param aTypeOfPrimitive:    type of primitive;
     *                              valid values are GL_TRIANGLES
     *  @param aVertexArray:        array containing the vertex data
     *  @param anIndexArray:        array containing the index data
     *  @param aCreateVBOFlag:      flag set to true to create a VBO,
     *                              set to false to not set a VBO
     *  @param aTypeOfVBO:          VBO usage hint to set; valid values are
     *                              GL_STATIC_DRAW, GL_DYNAMIC_DRAW and
     *                              GL_STREAM_DRAW
     */
    //--------------------------------------------------------------------------
    void setExternalData(int aTypeOfPrimitive,
            const std::vector<double>* aVertexArray,
            const std::vector<unsigned char>* anIndexArray,
            bool aCreateVBOFlag,
            int aTypeOfVBO);


	//--------------------------------------------------------------------------
	/// Set the polygon mesh using external data. The class instance will
	/// manage the memory.
	/**

	*  @param aVertexArray:        array containing the vertex data
	*  @param aNumberOfVertices:   number of vertices
	*  @param aVertexVBO		   identificator of vbo for vertices
	*  @param aNormalVBO		   identificator of vbo for normals
	*  @param aTextureVBO		   identificator of vbo for texture coords
	*  @param anIndexVBO:          array containing the index data
	*  @param aNumberOfIndices:    number of indices
	*  @param anIndexDataType:     data type of the index data; valid values
	*                              are GL_UNSIGNED_INT, GL_UNSIGNED_SHORT,
	*                              GL_UNSIGNED_BYTE and GL_NONE
	*  @param aIndexArray:		   array containing the index data
	*  @param aNormalArray:        array containing the normal data
	*/
	//--------------------------------------------------------------------------
	void setVBOData(
		const void* aVertexArray,
		unsigned int aNumberOfVertices,
		unsigned int aVertexVBO,
		unsigned int aNormalVBO,
		unsigned int aTextureVBO,
		unsigned int anIndexVBO,
		unsigned int aNumberOfIndices,
		int anIndexDataType,
		const void* aIndexArray = NULL,
		const void* aNormalArray = NULL
		);


    //--------------------------------------------------------------------------
    /// Set the polygon mesh using external data. The class instance will
    /// manage the memory.
    /**
     *  @param aTypeOfPrimitive:    type of primitive;
     *                              valid values are GL_TRIANGLES
     *  @param aVertexArray:        array containing the vertex data
     *  @param aNumberOfVertices:   number of vertices
     *  @param aVertexDataType:     data type of the vertex data; valid values
     *                              are GL_FLOAT, GL_DOUBLE and GL_NONE
     *  @param anIndexArray:        array containing the index data
     *  @param aNumberOfIndices:    number of indices
     *  @param anIndexDataType:     data type of the index data; valid values
     *                              are GL_UNSIGNED_INT, GL_UNSIGNED_SHORT,
     *                              GL_UNSIGNED_BYTE and GL_NONE
     *  @param aCreateVBOFlag:      flag set to true to create a VBO,
     *                              set to false to not set a VBO
     *  @param aTypeOfVBO:          VBO usage hint to set; valid values are
     *                              GL_STATIC_DRAW, GL_DYNAMIC_DRAW and
     *                              GL_STREAM_DRAW
     */
    //--------------------------------------------------------------------------
    void setInternalData(int aTypeOfPrimitive,
            const void* aVertexArray,
            unsigned int aNumberOfVertices,
            int aVertexDataType,
            const void* anIndexArray,
            unsigned int aNumberOfIndices,
            int anIndexDataType,
            bool aCreateVBOFlag,
            int aTypeOfVBO);


    //--------------------------------------------------------------------------
    /// Set the polygon mesh using external data. The class instance will
    /// manage the memory.
    /**
     *  @param aTypeOfPrimitive:    type of primitive;
     *                              valid values are GL_TRIANGLES
     *  @param aVertexArray:        array containing the vertex data
     *  @param aNormalArray:        array containing the normal data
     *  @param aCreateVBOFlag:      flag set to true to create a VBO,
     *                              set to false to not set a VBO
     *  @param aTypeOfVBO:          VBO usage hint to set; valid values are
     *                              GL_STATIC_DRAW, GL_DYNAMIC_DRAW and
     *                              GL_STREAM_DRAW
     */
    //--------------------------------------------------------------------------
    void setInternalData(int aTypeOfPrimitive,
            const std::vector<float>* aVertexArray,
            const std::vector<float>* aNormalArray,
            bool aCreateVBOFlag,
            int aTypeOfVBO);


    //--------------------------------------------------------------------------
    /// Set the polygon mesh using external data. The class instance will
    /// manage the memory.
    /**
     *  @param aTypeOfPrimitive:    type of primitive;
     *                              valid values are GL_TRIANGLES
     *  @param aVertexArray:        array containing the vertex data
     *  @param aCreateVBOFlag:      flag set to true to create a VBO,
     *                              set to false to not set a VBO
     *  @param aTypeOfVBO:          VBO usage hint to set; valid values are
     *                              GL_STATIC_DRAW, GL_DYNAMIC_DRAW and
     *                              GL_STREAM_DRAW
     */
    //--------------------------------------------------------------------------
    void setInternalData(int aTypeOfPrimitive,
            const std::vector<float>* aVertexArray,
            bool aCreateVBOFlag,
            int aTypeOfVBO);


    //--------------------------------------------------------------------------
    /// Set the polygon mesh using external data. The class instance will
    /// manage the memory.
    /**
     *  @param aTypeOfPrimitive:    type of primitive;
     *                              valid values are GL_TRIANGLES
     *  @param aVertexArray:        array containing the vertex data
     *  @param aNormalArray:        array containing the normal data
     *  @param aCreateVBOFlag:      flag set to true to create a VBO,
     *                              set to false to not set a VBO
     *  @param aTypeOfVBO:          VBO usage hint to set; valid values are
     *                              GL_STATIC_DRAW, GL_DYNAMIC_DRAW and
     *                              GL_STREAM_DRAW
     */
    //--------------------------------------------------------------------------
    void setInternalData(int aTypeOfPrimitive,
            const std::vector<double>* aVertexArray,
            const std::vector<double>* aNormalArray,
            bool aCreateVBOFlag,
            int aTypeOfVBO);


    //--------------------------------------------------------------------------
    /// Set the polygon mesh using external data. The class instance will
    /// manage the memory.
    /**
     *  @param aTypeOfPrimitive:    type of primitive;
     *                              valid values are GL_TRIANGLES
     *  @param aVertexArray:        array containing the vertex data
     *  @param aCreateVBOFlag:      flag set to true to create a VBO,
     *                              set to false to not set a VBO
     *  @param aTypeOfVBO:          VBO usage hint to set; valid values are
     *                              GL_STATIC_DRAW, GL_DYNAMIC_DRAW and
     *                              GL_STREAM_DRAW
     */
    //--------------------------------------------------------------------------
    void setInternalData(int aTypeOfPrimitive,
            const std::vector<double>* aVertexArray,
            bool aCreateVBOFlag,
            int aTypeOfVBO);


    //--------------------------------------------------------------------------
    /// Set the polygon mesh using external data. The class instance will
    /// manage the memory.
    /**
     *  @param aTypeOfPrimitive:    type of primitive; valid values are
     *                              GL_TRIANGLES
     *  @param aVertexArray:        array containing the vertex data
     *  @param anIndexArray:        array containing the index data
     *  @param aCreateVBOFlag:      flag set to true to create a VBO,
     *                              set to false to not set a VBO
     *  @param aTypeOfVBO:          VBO usage hint to set; valid values are
     *                              GL_STATIC_DRAW, GL_DYNAMIC_DRAW and
     *                              GL_STREAM_DRAW
     */
    //--------------------------------------------------------------------------
    void setInternalData(int aTypeOfPrimitive,
            const std::vector<float>* aVertexArray,
            const std::vector<unsigned int>* anIndexArray,
            bool aCreateVBOFlag,
            int aTypeOfVBO);


    void setInternalData(int aTypeOfPrimitive,
            const std::vector<float>* aVertexArray,
            const std::vector<float>* aNormalArray,
            const std::vector<unsigned int>* anIndexArray,
            bool aCreateVBOFlag,
            int aTypeOfVBO);


    //--------------------------------------------------------------------------
    /// Set the polygon mesh using external data. The class instance will
    /// manage the memory.
    /**
     *  @param aTypeOfPrimitive:    type of primitive; valid values are
     *                              GL_TRIANGLES
     *  @param aVertexArray:        array containing the vertex data
     *  @param anIndexArray:        array containing the index data
     *  @param aCreateVBOFlag:      flag set to true to create a VBO,
     *                              set to false to not set a VBO
     *  @param aTypeOfVBO:          VBO usage hint to set; valid values are
     *                              GL_STATIC_DRAW, GL_DYNAMIC_DRAW and
     *                              GL_STREAM_DRAW
     */
    //--------------------------------------------------------------------------
    void setInternalData(int aTypeOfPrimitive,
            const std::vector<double>* aVertexArray,
            const std::vector<unsigned int>* anIndexArray,
            bool aCreateVBOFlag,
            int aTypeOfVBO);


    //--------------------------------------------------------------------------
    /// Set the polygon mesh using external data. The class instance will
    /// manage the memory.
    /**
     *  @param aTypeOfPrimitive:    type of primitive; valid values are
     *                              GL_TRIANGLES
     *  @param aVertexArray:        array containing the vertex data
     *  @param aNormalArray:        array containing the normal data
     *  @param anIndexArray:        array containing the index data
     *  @param aCreateVBOFlag:      flag set to true to create a VBO,
     *                              set to false to not set a VBO
     *  @param aTypeOfVBO:          VBO usage hint to set; valid values are
     *                              GL_STATIC_DRAW, GL_DYNAMIC_DRAW and
     *                              GL_STREAM_DRAW
     */
    //--------------------------------------------------------------------------
    void setInternalData(int aTypeOfPrimitive,
            const std::vector<double>* aVertexArray,
            const std::vector<float>* aNormalArray,
            const std::vector<unsigned int>* anIndexArray,
            bool aCreateVBOFlag,
            int aTypeOfVBO);


    //--------------------------------------------------------------------------
    /// Set the polygon mesh using external data. The class instance will
    /// manage the memory.
    /**
     *  @param aTypeOfPrimitive:    type of primitive;
     *                              valid values are GL_TRIANGLES
     *  @param aVertexArray:        array containing the vertex data
     *  @param anIndexArray:        array containing the index data
     *  @param aCreateVBOFlag:      flag set to true to create a VBO,
     *                              set to false to not set a VBO
     *  @param aTypeOfVBO:          VBO usage hint to set; valid values are
     *                              GL_STATIC_DRAW, GL_DYNAMIC_DRAW and
     *                              GL_STREAM_DRAW
     */
    //--------------------------------------------------------------------------
    void setInternalData(int aTypeOfPrimitive,
            const std::vector<float>* aVertexArray,
            const std::vector<unsigned short>* anIndexArray,
            bool aCreateVBOFlag,
            int aTypeOfVBO);


    //--------------------------------------------------------------------------
    /// Set the polygon mesh using external data. The class instance will
    /// manage the memory.
    /**
     *  @param aTypeOfPrimitive:    type of primitive;
     *                              valid values are GL_TRIANGLES
     *  @param aVertexArray:        array containing the vertex data
     *  @param aNormalArray:        array containing the normal data
     *  @param anIndexArray:        array containing the index data
     *  @param aCreateVBOFlag:      flag set to true to create a VBO,
     *                              set to false to not set a VBO
     *  @param aTypeOfVBO:          VBO usage hint to set; valid values are
     *                              GL_STATIC_DRAW, GL_DYNAMIC_DRAW and
     *                              GL_STREAM_DRAW
     */
    //--------------------------------------------------------------------------
    void setInternalData(int aTypeOfPrimitive,
            const std::vector<float>* aVertexArray,
            const std::vector<float>* aNormalArray,
            const std::vector<unsigned short>* anIndexArray,
            bool aCreateVBOFlag,
            int aTypeOfVBO);

    //--------------------------------------------------------------------------
    /// Set the polygon mesh using external data. The class instance will
    /// manage the memory.
    /**
     *  @param aTypeOfPrimitive:    type of primitive; valid values are
     *                              GL_TRIANGLES
     *  @param aVertexArray:        array containing the vertex data
     *  @param anIndexArray:        array containing the index data
     *  @param aCreateVBOFlag:      flag set to true to create a VBO,
     *                              set to false to not set a VBO
     *  @param aTypeOfVBO:          VBO usage hint to set; valid values are
     *                              GL_STATIC_DRAW, GL_DYNAMIC_DRAW and
     *                              GL_STREAM_DRAW
     */
    //--------------------------------------------------------------------------
    void setInternalData(int aTypeOfPrimitive,
            const std::vector<double>* aVertexArray,
            const std::vector<unsigned short>* anIndexArray,
            bool aCreateVBOFlag,
            int aTypeOfVBO);


    //--------------------------------------------------------------------------
    /// Set the polygon mesh using external data. The class instance will
    /// manage the memory.
    /**
     *  @param aTypeOfPrimitive:    type of primitive; valid values are
     *                              GL_TRIANGLES
     *  @param aVertexArray:        array containing the vertex data
     *  @param aNormalArray:        array containing the normal data
     *  @param anIndexArray:        array containing the index data
     *  @param aCreateVBOFlag:      flag set to true to create a VBO,
     *                              set to false to not set a VBO
     *  @param aTypeOfVBO:          VBO usage hint to set; valid values are
     *                              GL_STATIC_DRAW, GL_DYNAMIC_DRAW and
     *                              GL_STREAM_DRAW
     */
    //--------------------------------------------------------------------------
    void setInternalData(int aTypeOfPrimitive,
            const std::vector<double>* aVertexArray,
            const std::vector<float>* aNormalArray,
            const std::vector<unsigned short>* anIndexArray,
            bool aCreateVBOFlag,
            int aTypeOfVBO);


    //--------------------------------------------------------------------------
    /// Set the polygon mesh using external data. The class instance will
    /// manage the memory.
    /**
     *  @param aTypeOfPrimitive:    type of primitive; valid values are
     *                              GL_TRIANGLES
     *  @param aVertexArray:        array containing the vertex data
     *  @param anIndexArray:        array containing the index data
     *  @param aCreateVBOFlag:      flag set to true to create a VBO,
     *                              set to false to not set a VBO
     *  @param aTypeOfVBO:          VBO usage hint to set; valid values are
     *                              GL_STATIC_DRAW, GL_DYNAMIC_DRAW and
     *                              GL_STREAM_DRAW
     */
    //--------------------------------------------------------------------------
    void setInternalData(int aTypeOfPrimitive,
            const std::vector<float>* aVertexArray,
            const std::vector<unsigned char>* anIndexArray,
            bool aCreateVBOFlag,
            int aTypeOfVBO);


    //--------------------------------------------------------------------------
    /// Set the polygon mesh using external data. The class instance will
    /// manage the memory.
    /**
     *  @param aTypeOfPrimitive:    type of primitive; valid values are
     *                              GL_TRIANGLES
     *  @param aVertexArray:        array containing the vertex data
     *  @param aNormalArray:        array containing the normal data
     *  @param anIndexArray:        array containing the index data
     *  @param aCreateVBOFlag:      flag set to true to create a VBO,
     *                              set to false to not set a VBO
     *  @param aTypeOfVBO:          VBO usage hint to set; valid values are
     *                              GL_STATIC_DRAW, GL_DYNAMIC_DRAW and
     *                              GL_STREAM_DRAW
     */
    //--------------------------------------------------------------------------
    void setInternalData(int aTypeOfPrimitive,
            const std::vector<float>* aVertexArray,
            const std::vector<float>* aNormalArray,
            const std::vector<unsigned char>* anIndexArray,
            bool aCreateVBOFlag,
            int aTypeOfVBO);


    //--------------------------------------------------------------------------
    /// Set the polygon mesh using external data. The class instance will
    /// manage the memory.
    /**
     *  @param aTypeOfPrimitive:    type of primitive; valid values are
     *                              GL_TRIANGLES
     *  @param aVertexArray:        array containing the vertex data
     *  @param anIndexArray:        array containing the index data
     *  @param aCreateVBOFlag:      flag set to true to create a VBO,
     *                              set to false to not set a VBO
     *  @param aTypeOfVBO:          VBO usage hint to set; valid values are
     *                              GL_STATIC_DRAW, GL_DYNAMIC_DRAW and
     *                              GL_STREAM_DRAW
     */
    //--------------------------------------------------------------------------
    void setInternalData(int aTypeOfPrimitive,
            const std::vector<double>* aVertexArray,
            const std::vector<unsigned char>* anIndexArray,
            bool aCreateVBOFlag,
            int aTypeOfVBO);


    //--------------------------------------------------------------------------
    /// Set the polygon mesh using external data. The class instance will
    /// manage the memory.
    /**
     *  @param aTypeOfPrimitive:    type of primitive; valid values are
     *                              GL_TRIANGLES
     *  @param aVertexArray:        array containing the vertex data
     *  @param aNormalArray:        array containing the normal data
     *  @param anIndexArray:        array containing the index data
     *  @param aCreateVBOFlag:      flag set to true to create a VBO,
     *                              set to false to not set a VBO
     *  @param aTypeOfVBO:          VBO usage hint to set; valid values are
     *                              GL_STATIC_DRAW, GL_DYNAMIC_DRAW and
     *                              GL_STREAM_DRAW
     */
    //--------------------------------------------------------------------------
    void setInternalData(int aTypeOfPrimitive,
            const std::vector<double>* aVertexArray,
            const std::vector<float>* aNormalArray,
            const std::vector<unsigned char>* anIndexArray,
            bool aCreateVBOFlag,
            int aTypeOfVBO);


    //--------------------------------------------------------------------------
    /// Set the polygon mesh using external data. The class instance will
    /// manage the memory.
    /**
     *  @param aTypeOfPrimitive:    type of primitive; valid values are
     *                              GL_TRIANGLES
     *  @param aVertexArray:        array containing the vertex data
     *  @param aNormalArray:        array containing the normal data
     *  @param anIndexArray:        array containing the index data
     *  @param aCreateVBOFlag:      flag set to true to create a VBO,
     *                              set to false to not set a VBO
     *  @param aTypeOfVBO:          VBO usage hint to set; valid values are
     *                              GL_STATIC_DRAW, GL_DYNAMIC_DRAW and
     *                              GL_STREAM_DRAW
     */
    //--------------------------------------------------------------------------
    void setInternalData(int aTypeOfPrimitive,
                         const std::vector<double>* aVertexArray,
                         const std::vector<double>* aNormalArray,
                         const std::vector<unsigned char>* anIndexArray,
                         bool aCreateVBOFlag,
                         int aTypeOfVBO);


    //--------------------------------------------------------------------------
    /// Set the polygon mesh using external data. The class instance will
    /// manage the memory.
    /**
     *  @param aTypeOfPrimitive:    type of primitive; valid values are
     *                              GL_TRIANGLES
     *  @param aVertexArray:        array containing the vertex data
     *  @param aNormalArray:        array containing the normal data
     *  @param anIndexArray:        array containing the index data
     *  @param aCreateVBOFlag:      flag set to true to create a VBO,
     *                              set to false to not set a VBO
     *  @param aTypeOfVBO:          VBO usage hint to set; valid values are
     *                              GL_STATIC_DRAW, GL_DYNAMIC_DRAW and
     *                              GL_STREAM_DRAW
     */
    //--------------------------------------------------------------------------
    void setInternalData(int aTypeOfPrimitive,
                         const std::vector<double>* aVertexArray,
                         const std::vector<double>* aNormalArray,
                         const std::vector<unsigned short>* anIndexArray,
                         bool aCreateVBOFlag,
                         int aTypeOfVBO);


    //--------------------------------------------------------------------------
    /// Set the polygon mesh using external data. The class instance will
    /// manage the memory.
    /**
     *  @param aTypeOfPrimitive:    type of primitive; valid values are
     *                              GL_TRIANGLES
     *  @param aVertexArray:        array containing the vertex data
     *  @param aNormalArray:        array containing the normal data
     *  @param anIndexArray:        array containing the index data
     *  @param aCreateVBOFlag:      flag set to true to create a VBO,
     *                              set to false to not set a VBO
     *  @param aTypeOfVBO:          VBO usage hint to set; valid values are
     *                              GL_STATIC_DRAW, GL_DYNAMIC_DRAW and
     *                              GL_STREAM_DRAW
     */
    //--------------------------------------------------------------------------
    void setInternalData(int aTypeOfPrimitive,
                         const std::vector<double>* aVertexArray,
                         const std::vector<double>* aNormalArray,
                         const std::vector<unsigned int>* anIndexArray,
                         bool aCreateVBOFlag,
                         int aTypeOfVBO);


    //--------------------------------------------------------------------------
    /// Apply a scaling factor to the vertex data. Note that the bounding box
    /// will be automatically updated.
    /**
     *  @param aScale:  scaling factor
     */
    //--------------------------------------------------------------------------
    void applyScale(const RATIONAL_NUMBER& aScale);


    //--------------------------------------------------------------------------
    /// Apply a scaling factor to the vertex data. Note that the bounding box
    /// will be automatically updated.
    /**
     *  @param aScaleX:  scaling factor alng the x-axis
     *  @param aScaleY:  scaling factor alng the y-axis
     *  @param aScaleZ:  scaling factor alng the z-axis
     */
    //--------------------------------------------------------------------------
    void applyScale(const RATIONAL_NUMBER& aScaleX,
                    const RATIONAL_NUMBER& aScaleY,
                    const RATIONAL_NUMBER& aScaleZ);


    //--------------------------------------------------------------------------
    /// Inverse the normal vectors.
    //--------------------------------------------------------------------------
    void invertNormalVectors();


    //--------------------------------------------------------------------------
    /// Update the VBO.
    /**
     *  @param aTypeOfVBO:          VBO usage hint to set; valid values are
     *                              GL_STATIC_DRAW, GL_DYNAMIC_DRAW and
     *                              GL_STREAM_DRAW
     *  @param aTypeOfPrimitive:    type of primitive;
     *                              valid values are GL_TRIANGLES
     */
    //--------------------------------------------------------------------------
    virtual void updateVBO(int aTypeOfVBO, int aTypeOfPrimitive);


    //--------------------------------------------------------------------------
    /// Update the VBOs with VBO already created.
    /**
     *  @param aVertexVBO		   identificator of vbo for vertices
     *  @param aNormalVBO		   identificator of vbo for normals
     *  @param aTextureVBO		   identificator of vbo for texture coords
     *  @param anIndexVBO:          array containing the index data
     */
    //--------------------------------------------------------------------------
    virtual void updateVBOs(unsigned int aVertexVBO,
        unsigned int aNormalVBO,
        unsigned int aTextureVBO,
        unsigned int anIndexVBO);


    //--------------------------------------------------------------------------
    /// Accessor on the material of the polygon mesh.
    /**
     *  @return the material
     */
    //--------------------------------------------------------------------------
    const Material& getMaterial() const;


    //--------------------------------------------------------------------------
    /// Accessor on the material of the polygon mesh.
    /**
     *  @return the material
     */
    //--------------------------------------------------------------------------
    Material& getMaterial();


    //--------------------------------------------------------------------------
    /// Copy.
    /**
     *  @param aPolygonMesh:    the polygon mesh to copy
     */
    //--------------------------------------------------------------------------
    void copyFrom(const PolygonMesh& aPolygonMesh);


    //--------------------------------------------------------------------------
    /// Copy operator.
    /**
     *  @param aPolygonMesh:    the polygon mesh to copy
     *  @return the new PolygonMesh
     */
    //--------------------------------------------------------------------------
    PolygonMesh& operator=(const PolygonMesh& aPolygonMesh);


    //--------------------------------------------------------------------------
    /// operator+.
    /**
     *  @param aPolygonMesh:    the polygon mesh to add
     *  @return the new PolygonMesh
     */
    //--------------------------------------------------------------------------
    virtual PolygonMesh operator+(const PolygonMesh& aPolygonMesh) const;


    //--------------------------------------------------------------------------
    /// operator-.
    /**
     *  @param aPolygonMesh:    the polygon mesh to remove
     *  @return the new PolygonMesh
     */
    //--------------------------------------------------------------------------
    virtual PolygonMesh operator-(const PolygonMesh& aPolygonMesh) const;


    //--------------------------------------------------------------------------
    /// operator+=.
    /**
     *  @param aPolygonMesh:    the polygon mesh to add
     *  @return the new PolygonMesh
     */
    //--------------------------------------------------------------------------
    virtual PolygonMesh& operator+=(const PolygonMesh& aPolygonMesh);


    //--------------------------------------------------------------------------
    /// operator-=.
    /**
     *  @param aPolygonMesh:    the polygon mesh to remove
     *  @return the new PolygonMesh
     */
    //--------------------------------------------------------------------------
    virtual PolygonMesh& operator-=(const PolygonMesh& aPolygonMesh);


    //--------------------------------------------------------------------------
    /// operator+.
    /**
     *  @param aTranslationVector:    the translation vector
     *  @return the new PolygonMesh
     */
    //--------------------------------------------------------------------------
    virtual PolygonMesh operator+(const VEC3& aTranslationVector) const;


    //--------------------------------------------------------------------------
    /// operator-.
    /**
     *  @param aTranslationVector:    the translation vector
     *  @return the new PolygonMesh
     */
    //--------------------------------------------------------------------------
    virtual PolygonMesh operator-(const VEC3& aTranslationVector) const;


    //--------------------------------------------------------------------------
    /// operator+=.
    /**
     *  @param aTranslationVector:    the translation vector
     *  @return the new PolygonMesh
     */
    //--------------------------------------------------------------------------
    virtual PolygonMesh& operator+=(const VEC3& aTranslationVector);


    //--------------------------------------------------------------------------
    /// operator-=.
    /**
     *  @param aTranslationVector:    the translation vector
     *  @return the new PolygonMesh
     */
    //--------------------------------------------------------------------------
    virtual PolygonMesh& operator-=(const VEC3& aTranslationVector);


    std::map<RATIONAL_NUMBER, VEC3> rayIntersect(
    		const VEC3& aRayOrigin,
			const VEC3& aRayDirection,
			const MATRIX4& aTransformationMatrix) const;


    void getTriangleVertices(VEC3& v0, VEC3& v1, VEC3& v2, unsigned int anIndex) const;


    //--------------------------------------------------------------------------
    /// Check if the normal vectors have been inverted.
    /**
     *  @return true if the normal vectors have been inverted; false otherwise.
     */
    //--------------------------------------------------------------------------
    bool hasNormalVectorsInverted() const;


//******************************************************************************
protected:
    //--------------------------------------------------------------------------
    /// Update the bounding box.
    //--------------------------------------------------------------------------
    void computeBoundingBox();


    //--------------------------------------------------------------------------
    /// Normalise the normal vectors.
    //--------------------------------------------------------------------------
    void normaliseNormals();


    //--------------------------------------------------------------------------
    /// Release the memory held by the vertex data if needed.
    //--------------------------------------------------------------------------
    void destroyVertexData();


    //--------------------------------------------------------------------------
    /// Release the memory held by the index data if needed.
    //--------------------------------------------------------------------------
    void destroyIndexData();


    //--------------------------------------------------------------------------
    /// Remove the index.
    //--------------------------------------------------------------------------
    void removeIndex(bool aPrintDebugInfoFlag = false);


    //--------------------------------------------------------------------------
    /// Copy a vertex set.
    //--------------------------------------------------------------------------
    void copyVertexSet(const void* apVertexSet);


    //--------------------------------------------------------------------------
    /// Copy an index set.
    //--------------------------------------------------------------------------
    void copyIndexSet(const void* apIndexSet);


    //--------------------------------------------------------------------------
    /// Set the name of the file that contains the polygon mesh.
    /**
     * @param aMoveToCentreFlag:       a flag to move the polygon to the center
     *                                  or not
     * @param anAutoComputeNormalFlag: a flag to compute normals or not
     * @param aPrintDebugInfoFlag:     a flag to display additional debug
     *                                  information in the console or not
     * @param aCreateVBOFlag:          a flag to use a vertex buffer object or
     *                                  not
     * @param aScale:                  the unit in which data is stored in
     *                                  the file (e.g. mm, cm, m)
     * @param aBufferUsageHing:        the buffer usage hint; valid values are:
     *                                  GL_STATIC_DRAW, GL_DYNAMIC_DRAW and
     *                                  GL_STREAM_DRAW
     */
    //--------------------------------------------------------------------------
    void loadASCIISTLFile(bool aMoveToCentreFlag,
        bool anAutoComputeNormalFlag,
        bool aPrintDebugInfoFlag,
        bool aCreateVBOFlag,
        RATIONAL_NUMBER aScale,
        int aBufferUsageHing);


    //--------------------------------------------------------------------------
    /// Set the name of the file that contains the polygon mesh.
    /**
     * @param aMoveToCentreFlag:       a flag to move the polygon to the center
     *                                  or not
     * @param anAutoComputeNormalFlag: a flag to compute normals or not
     * @param aPrintDebugInfoFlag:     a flag to display additional debug
     *                                  information in the console or not
     * @param aCreateVBOFlag:          a flag to use a vertex buffer object or
     *                                  not
     * @param aScale:                  the unit in which data is stored in
     *                                  the file (e.g. mm, cm, m)
     * @param aBufferUsageHing:        the buffer usage hint; valid values are:
     *                                  GL_STATIC_DRAW, GL_DYNAMIC_DRAW and
     *                                  GL_STREAM_DRAW
     */
    //--------------------------------------------------------------------------
    void loadBinarySTLFile(bool aMoveToCentreFlag,
        bool anAutoComputeNormalFlag,
        bool aPrintDebugInfoFlag,
        bool aCreateVBOFlag,
        RATIONAL_NUMBER aScale,
        int aBufferUsageHing);


    //--------------------------------------------------------------------------
    /// Set the name of the file that contains the polygon mesh.
    /**
     * @param apInputData:             the input data to load
     * @param aMoveToCentreFlag:       a flag to move the polygon to the centre
     *                                  or not
     * @param anAutoComputeNormalFlag: a flag to compute normals or nor
     * @param aPrintDebugInfoFlag:     a flag to display additional debug
     *                                 information in the console or not
     * @param aCreateVBOFlag:          a flag to use a vertex buffer object or
     *                                 not
     * @param aScale:                  the unit in which data is stored in
     *                                 the file (e.g. mm, cm, m)
     * @param aBufferUsageHing:        the buffer usage hint; valid values are:
     *                                 GL_STATIC_DRAW, GL_DYNAMIC_DRAW and
     *                                 GL_STREAM_DRAW
     */
    //--------------------------------------------------------------------------
    void loadSTLDataFromBinaryStream(const char* apInputData,
            bool aMoveToCentreFlag,
            bool anAutoComputeNormalFlag,
            bool aPrintDebugInfoFlag,
            bool aCreateVBOFlag,
            RATIONAL_NUMBER aScale,
            int aBufferUsageHing);


    /// Name of the file that contains the polygon mesh
    std::string m_filename;


    /// Unit of length of the file that contains the polygon mesh (if negative, then unknown)
    RATIONAL_NUMBER m_file_scale;


    /// Vertex buffer object
#if __cplusplus < 199711L
    std::auto_ptr<VBO>          m_p_vbo;
    std::auto_ptr<PolygonMesh>  m_p_face_normal_vbo;
    std::auto_ptr<PolygonMesh>  m_p_vertex_normal_vbo;
#else
    std::unique_ptr<VBO>            m_p_vbo;
    std::unique_ptr<PolygonMesh>    m_p_face_normal_vbo;
    std::unique_ptr<PolygonMesh>    m_p_vertex_normal_vbo;
#endif


    /// Material properties, with respect to X-ray
    PhotonCrossSection m_photon_cross_section;


    /// Lower corner of the bounding box
    VEC3 m_local_min_corner;


    /// Upper corner of the bounding box
    VEC3 m_local_max_corner;


    /// Array containing the normal vectors (one per triangle)
    //std::vector<RATIONAL_NUMBER> m_p_face_normal_set;


    /// Array containing the normal vectors (one per vertex)
    std::vector<RATIONAL_NUMBER> m_p_vertex_normal_set;


    /// Array containing the vertices
    void* m_p_vertex_set;


    /// Array containing the index
    void* m_p_index_set;


    /// Number of indices
    unsigned int m_number_of_indices;


    /// Number of vertices
    unsigned int m_number_of_vertices;


    /// Data type of the index data; valid values are GL_UNSIGNED_INT,
    /// GL_UNSIGNED_SHORT, GL_UNSIGNED_BYTE and GL_NONE
    int m_index_data_type;


    /// Data type of the vertex data; valid values are GL_FLOAT,
    /// GL_DOUBLE and GL_NONE
    int m_vertex_data_type;


    /// Polygon type; valid values are GL_TRIANGLES
    int m_polygon_type;


    /// A flag set to true if the memory is managed outside of
    /// this class instance; set to false if the memory is managed
    /// by this class instance
    bool m_external_data_flag;


    /// A flag set to true if you want to display in the Xray
    ///image
    bool m_display_flag;


    /// Material of the object
    Material m_material;


    /// A flag set to true if the normal vectors are inverted
    bool m_has_inverted_normal_vectors;
};


} // namespace gVirtualXRay


//******************************************************************************
#include "PolygonMesh.inl"


#endif // GVXR_POLYGON_MESH_H
