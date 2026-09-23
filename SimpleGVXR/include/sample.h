/*

Copyright (c) 2024-present, Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), 
UK Research and Innovation, All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

3. Neither the name of UK Research and Innovation nor the names of its contributors
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


#ifndef SGVXR_Sample_h
#define SGVXR_Sample_h


/**
********************************************************************************
*
*   @file       sample.h
*
*   @brief      Expose here all the functions of SimpleGVXR related to 
*               the sample. It includes its geometry and material composition.
*
*   @version    1.0
*
*   @date       10/07/2024
*
*   @author     Prof Franck P. Vidal
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, May 2024-present, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************


//------------------------------------------------------------------------------
/// Load a polygon mesh from a file, set its label in the scenegraph
/// (i.e. identifier) and add it to the X-ray renderer.
/**
 *  @param aLabel:          the label in the scenegraph
 *  @param aFileName:       the file where the polygon mesh data is stored
 *  @param aUnitOfLength:   the unit of length corresponding to the data stored
 *                          in the file. Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 *  @param addToRendererAsInnerSurface: a flag to add the mesh to the X-ray renderer (as an inner surface) (default value: true)
 *  @param aParent: the parent of the node in the scenegraph (default: "root")
 */
//------------------------------------------------------------------------------
void loadMeshFile(const std::string& aLabel,
        const std::string& aFileName,
        const std::string& aUnitOfLength,
        bool addToRendererAsInnerSurface = true,
        const std::string& aParent = "root");


//------------------------------------------------------------------------------
/// Accessor on the filename of the mesh.
/**
 *  @param aLabel:          the label in the scenegraph
 *  @return the filename
 */
//------------------------------------------------------------------------------
std::string getMeshFilename(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Accessor on the unit of length of the mesh.
/**
 *  @param aLabel:          the label in the scenegraph
 *  @return the unit of length
 */
//------------------------------------------------------------------------------
std::string getMeshUnitOfLength(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Load a scenegraph from a file and add its polygon meshes
/// to the X-ray renderer.
/**
 *  @param aFileName:       the file where the polygon mesh data is stored
 *  @param aUnitOfLength:   the unit of length corresponding to the data stored
 *                          in the file. Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 */
//------------------------------------------------------------------------------
void loadSceneGraph(const std::string& aFileName,
    const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Accessor on the number of primitives of a given node.
/**
 *  @param aLabel:  the label in the scenegraph
 *  @return the number of primitives
 */
//------------------------------------------------------------------------------
unsigned int getNumberOfPrimitives(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Accessor on the number of face normal vectors of a given node.
/**
 *  @param aLabel:  the label in the scenegraph
 *  @return the number of face normal vectors
 */
//------------------------------------------------------------------------------
unsigned int getNumberOfFaceNormalVectors(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Accessor on the number of vertex normal vectors of a given node.
/**
 *  @param aLabel:  the label in the scenegraph
 *  @return the number of vertex normal vectors
 */
//------------------------------------------------------------------------------
unsigned int getNumberOfVertexNormalVectors(const std::string& aLabel);


//--------------------------------------------------------------------------
/// Accessor on the number of verticesof a given node.
/**
 * @param aLabel:  the label in the scenegraph
 * @return the number of vertices
 */
//--------------------------------------------------------------------------
unsigned int getVertexNumber(const std::string& aLabel);


//--------------------------------------------------------------------------
/// Accessor on the number of indicesof a given node.
/**
 * @param aLabel:  the label in the scenegraph
 * @return the number of indices
 */
//--------------------------------------------------------------------------
unsigned int getIndexNumber(const std::string& aLabel);


//--------------------------------------------------------------------------
/// Accessor on the vertices of a given node.
/**
 * @param aLabel:  the label in the scenegraph
 * @return the vertex
 */
//--------------------------------------------------------------------------
std::vector<float> getVertexSet(const std::string& aLabel);


//--------------------------------------------------------------------------
/// Accessor on the face normals of a given node.
/**
 * @param aLabel:  the label in the scenegraph
 * @return the face normals
 */
 //--------------------------------------------------------------------------
std::vector<float> getFaceNormalVectorSet(const std::string& aLabel);


//--------------------------------------------------------------------------
/// Accessor on the vertex normals of a given node.
/**
 * @param aLabel:  the label in the scenegraph
 * @return the vertex normals
 */
 //--------------------------------------------------------------------------
std::vector<float> getVertexNormalVectorSet(const std::string& aLabel);


//--------------------------------------------------------------------------
/// Accessor on the set of indices of a given node.
/**
 * @param aLabel:  the label in the scenegraph
 * @return the set of indices
 */
 //--------------------------------------------------------------------------
std::vector<int> getIndexSet(const std::string& aLabel);


//--------------------------------------------------------------------------
/// Check if a given node is an innersurface ("an" because there can be many).
/**
 * @param aLabel:  the label in the scenegraph
 * @return true if the node is an inner surface, otherwise false
 */
//--------------------------------------------------------------------------
bool isInnerSurface(const std::string& aLabel);


//--------------------------------------------------------------------------
/// Check if a given node is the outer surface ("the" because there can only be one).
/**
 * @param aLabel:  the label in the scenegraph
 * @return true if the node is the outer surface, otherwise false
 */
 //--------------------------------------------------------------------------
bool isOuterSurface(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Create an empty polygon mesh and set its label in the scenegraph
/// (i.e. identifier). Note that it is not added to the X-ray renderer.
/**
 *  @param aLabel:  the label in the scenegraph
 *  @param aParent: the parent of the node in the scenegraph (default: "root")
 */
//------------------------------------------------------------------------------
void emptyMesh(const std::string& aLabel,
        const std::string& aParent = "root");


//------------------------------------------------------------------------------
/// Remove and delete a polygon mesh from the scenegraph
/// (i.e. identifier). Note that all its children, if any, will be removed too.
/**
 *  @param aLabel:  the label in the scenegraph
 */
//------------------------------------------------------------------------------
void removeMeshAndChildren(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Create a cube, centred on (0, 0, 0) and set its label in the scenegraph (i.e. identifier).
/// Note that it is not added to the X-ray renderer.
/**
 *  @param aLabel:  the label in the scenegraph
 *  @param aLength: the size of an edge of the cube (default value: 1)
 *  @param aUnitOfLength:   the unit of length corresponding to aLength.
 *                          Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 *                          (default value: "cm")
 *  @param aParent: the parent of the node in the scenegraph (default: "root")
 */
//------------------------------------------------------------------------------
void makeCube(const std::string& aLabel,
        double aLength = 1.0,
        const std::string& aUnitOfLength = "cm",
        const std::string& aParent = "root");


//------------------------------------------------------------------------------
/// Create a cuboid, centred on (0, 0, 0) and set its label in the scenegraph (i.e. identifier).
/// Note that it is not added to the X-ray renderer.
/**
 *  @param aLabel:  the label in the scenegraph
 *   @param aWidth:   the width of the cuboid, i.e. its length along the X-axis (default value: 1)
 *   @param aHeight:  the height of the cuboid, i.e. its length along the Y-axis (default value: 1)
 *   @param aDepth:   the depth of the cuboid, i.e. its length along the Z-axis (default value: 1)
 *  @param aUnitOfLength:   the unit of length corresponding to aLength.
 *                          Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 *                          (default value: "cm")
 *  @param aParent: the parent of the node in the scenegraph (default: "root")
 */
//------------------------------------------------------------------------------
void makeCuboid(const std::string& aLabel,
        double aWidth = 1.0,
        double aHeight = 1.0,
        double aDepth = 1.0,
        const std::string& aUnitOfLength = "cm",
        const std::string& aParent = "root");


//------------------------------------------------------------------------------
/// Create a cylinder and set its label in the scenegraph (i.e. identifier).
/// Note that it is not added to the X-ray renderer.
/**
 *  @param aLabel:  the label in the scenegraph
 *  @param aNumberOfSectors:    the number of sectors defining the mesh (default value: 10)
 *  @param aHeight: the height of the cylinder (default value: 1.0)
 *  @param aRadius: the radius of the cylinder (default value: 0.5)
 *  @param aUnitOfLength:   the unit of length corresponding to aHeight and
 *                          aRadius. Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 *                          (default value: "cm")
 *  @param aParent: the parent of the node in the scenegraph (default: "root")
 */
//------------------------------------------------------------------------------
void makeCylinder(const std::string& aLabel,
                  unsigned int aNumberOfSectors = 10,
                  double aHeight = 1.0,
                  double aRadius = 0.5,
                  const std::string& aUnitOfLength = "cm",
                  const std::string& aParent = "root");


//------------------------------------------------------------------------------
/// Create a sphere and set its label in the scenegraph (i.e. identifier).
/// Note that it is not added to the X-ray renderer.
/**
 *  @param aLabel:  the label in the scenegraph
 *  @param aNumberOfRings:    the number of rings defining the mesh (default value: 10)
 *  @param aNumberOfSectors:    the number of sectors defining the mesh (default value: 10)
 *  @param aRadius: the radius of the sphere (default value: 0.5)
 *  @param aUnitOfLength:   the unit of length corresponding to aRadius. 
 *                          Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 *                          (default value: "cm")
 *  @param aParent: the parent of the node in the scenegraph (default: "root")
 */
//------------------------------------------------------------------------------
void makeSphere(const std::string& aLabel,
                unsigned int aNumberOfRings = 10,
                unsigned int aNumberOfSectors = 10,
                double aRadius = 0.5,
                const std::string& aUnitOfLength = "cm",
                const std::string& aParent = "root");


//------------------------------------------------------------------------------
/// Create a step wedge and set its label in the scenegraph (i.e. identifier).
/// Note that it is not added to the X-ray renderer.
/**
 *  @param aLabel:  the label in the scenegraph
 *  @param aNumberOfSteps:   the number of steps in the stepwedge (default value: 10)
 *  @param aTotalLength:  the length of the step wedge, i.e. its length along the X-axis (default value: 10)
 *  @param aWidth:   the width of the step wedge, i.e. its length along the Y-axis (default value: 1)
 *  @param aFirstStepHeight:  the height of the first step on the step wedge, i.e. its length along the Z-axis (default value: 1)
 *  @param anOtherStepHeight:  the height of the rest of the steps on the step wedge, i.e. its length along the Z-axis (default value: 0.1)
 *  @param aUnitOfLength:   the unit of length corresponding to aRadius. 
 *                          Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 *                          (default value: "cm")
 *  @param aParent: the parent of the node in the scenegraph (default: "root")
 */
//------------------------------------------------------------------------------
void makeStepWedge(const std::string& aLabel,
                   unsigned int aNumberOfSteps = 2,
                   float aTotalLength = 10.0,
                   float aWidth = 1.0,
                   float aFirstStepHeight = 1.0,
                   float anOtherStepHeight = 0.1,
                   const std::string& aUnitOfLength = "cm",
                   const std::string& aParent = "root");


//------------------------------------------------------------------------------
/// Create an iso-surface from a 3D image and set its label in the scenegraph (i.e. identifier).
/// Note that it is not added to the X-ray renderer.
/**
*  @param aLabel:  the label in the scenegraph
*  @param anIsoValue: the iso-value
*  @param aVoxelDataSet:  the voxel dataset
*  @param aWidth:  the number of voxel along the X-axis
*  @param aHeight:  the number of voxel along the Y-axis
*  @param aDepth:  the number of voxel along the Z-axis
*  @param aSpacingX: the space between the centre of two successive voxels along the X-axis
*  @param aSpacingY: the space between the centre of two successive voxels along the Y-axis
*  @param aSpacingZ: the space between the centre of two successive voxels along the Z-axis
*  @param aUnitOfLength:   the unit of length corresponding to aSpacingX, aSpacingY and aSpacingZ.
*                          Acceptable values are:
*                          "um", "micrometre",  "micrometer",
*                          "mm", "millimetre",  "millimeter",
*                          "cm", "centimetre",  "centimeter",
*                          "dm",  "decimetre",  "decimeter",
*                          "m",   "metre",      "meter",
*                          "dam", "decametre",  "decameter",
*                          "hm",  "hectometre", "hectometer",
*                          "km",  "kilometre",  "kilometer"
*                          (default value: "cm")
*  @param aParent: the parent of the node in the scenegraph (default: "root")
*/
//------------------------------------------------------------------------------
void makeIsoSurface(const std::string& aLabel,
    short anIsoValue,
    const std::vector<short>& aVoxelDataSet,
    unsigned int aWidth,
    unsigned int aHeight,
    unsigned int aDepth,
    double aSpacingX,
    double aSpacingY,
    double aSpacingZ,
    const std::string& aUnitOfLength = "cm",
    const std::string& aParent = "root");


//------------------------------------------------------------------------------
/// Create a foam structure, i.e. porosities into a bounding volume.
/// Deprecated function: The parameter aBoundingVolumeName is not used. 
/// The function will be removed in future versions.
/**
 *  @param aPorosityName:  the label of the prorosities in the scenegraph
 *  @param aBoundingVolumeName:  the label of the bounding volume in the scenegraph
 *  @param aCuboidWidth:    the width of the cuboid, i.e. its length along the X-axis
 *  @param aCuboidHeight:   the height of the cuboid, i.e. its length along the Y-axis
 *  @param aCuboidDepth:    the depth of the cuboid, i.e. its length along the Z-axis
 *  @param aPorosityRadius: Tuple of the mean radius and statistical deviation for foam holes created.
 *  @param aPercentageOfVolume  : Percentage of the enclosing volume occupied by foam pores
 *  @param aPercentageDeviation : The standard deviation relative to the radius in percentage (Default is 10%)
 *  @param aVoxelSpacing        : Spacing (or resolution) of the voxels in cube.
 *  @param aUnitOfLength:   the unit of length corresponding to aRadius and aVoxelSpacing.
 *                          Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 *  @param aVerboseLevel:   the level of statements in the standard output. For no output, use 0 (default value: 0)
 */
 //------------------------------------------------------------------------------
void makeFoamInCuboid(const std::string& aPorosityName,
    const std::string& aBoundingVolumeName,
    double aCuboidWidth,
    double aCuboidHeight,
    double aCuboidDepth,
    float aPorosityRadius,
    float aPercentageOfVolume,
    float aPercentageDeviation,
    float aVoxelSpacing,
    const std::string& aUnitOfLength,
    unsigned char aVerboseLevel = 0);


//------------------------------------------------------------------------------
/// Create a triangular mesh structure from a set of vertices.
/**
 *  @param aLabel:  the label of the node to create in the scenegraph
 *  @param aVertexSet:  the set of vertices
 *  @param aUnitOfLength:   the unit of length corresponding to aVertexSet.
 *                          Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 *  @param aParent: the parent of the node in the scenegraph (default: "root")
 */
 //------------------------------------------------------------------------------
void makeTriangularMesh(const std::string& aLabel,
		const std::vector<float>& aVertexSet,
	    const std::string& aUnitOfLength = "cm",
		const std::string& aParent = "root");


//------------------------------------------------------------------------------
/// Create a triangular mesh structure from a set of vertices and a set of indices.
/**
 *  @param aLabel:  the label of the node to create in the scenegraph
 *  @param aVertexSet:  the set of vertices
 *  @param aTriangleIndexSet:   the set of indices
 *  @param aUnitOfLength:   the unit of length corresponding to aVertexSet.
 *                          Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 *  @param aParent: the parent of the node in the scenegraph (default: "root")
 */
 //------------------------------------------------------------------------------
void makeTriangularMesh(const std::string& aLabel,
		const std::vector<float>& aVertexSet,
		const std::vector<int>& aTriangleIndexSet,
	    const std::string& aUnitOfLength = "cm",
		const std::string& aParent = "root");


//------------------------------------------------------------------------------
/// Create a cylinder in which spheres are embedded. The cylinder is centred on
/// (0, 0, 0) and the spheres are randomly positioned in the cylinder.
/// Note that it is not added to the X-ray renderer.
/**
 *  @param aCylinderLabel:  the cylinder label in the scenegraph
 *  @param aCylinderNumberOfSectors:    the number of sectors defining the mesh
 *  @param aCylinderRadius:  the radius of the cylinder
 *  @param aCylinderHeight:  the height of the cylinder
 *
 *  @param aSphereLabel:            the sphere label in the scenegraph
 *  @param aSphereNumberOfRings:    the number of rings defining the mesh
 *  @param aSphereNumberOfSectors:  the number of sectors defining the mesh
 *  @param aSphereRadius:  the radius of the cylinder
 *
 *  @param aPercentVolume the percentage of the cylinder volume occupied by spheres
 *
 *  @param aUnitOfLength:   the unit of length corresponding to aCylinderRadius, aCylinderHeight, and aSphereRadius.
 *                          Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 *                          (default value: "cm")
 *
 *  @param aParent: the parent of the node in the scenegraph (default: "root")
 *  
 *  @return the number of spheres
 */
//------------------------------------------------------------------------------
unsigned int makeSpheresInCylinder(
        const std::string& aCylinderLabel,
        unsigned int aCylinderNumberOfSectors,
        double aCylinderRadius,
        double aCylinderHeight,
        const std::string& aSphereLabel,
        unsigned int aSphereNumberOfRings,
        unsigned int aSphereNumberOfSectors,
        double aSphereRadius,
        double aPercentVolume,
        const std::string& aUnitOfLength = "cm",
        const std::string& aParent = "root");


//------------------------------------------------------------------------------
/// Add a polygon mesh (aSource) to another one (aDestination).
/// It corresponds to the operation as follows: aDestination += aSource.
/// Both aSource and aDestination have to already exist in the scenegraph.
/**
 *  @param aDestination:    the polygon mesh that is going to be modified
 *  @param aSource:         the polygon mesh that is going to be added
 *                          to aDestination
 */
//------------------------------------------------------------------------------
void addMesh(const std::string& aDestination, const std::string& aSource);


//------------------------------------------------------------------------------
/// Subtract a polygon mesh (aSource) from another one (aDestination).
/// It corresponds to the operation as follows: aDestination -= aSource.
/// Both aSource and aDestination have to already exist in the scenegraph.
/**
 *  @param aDestination:    the polygon mesh that is going to be modified
 *  @param aSource:         the polygon mesh that is going to be subtracted
 *                          from aDestination
 */
//------------------------------------------------------------------------------
void subtractMesh(const std::string& aDestination, const std::string& aSource);


//------------------------------------------------------------------------------
/// Get the number of children of a node in the scenegraph.
/**
 *  @param aLabel:  the label in the scenegraph
 *  @return the number of chidren
 */
//------------------------------------------------------------------------------
unsigned int getNumberOfChildren(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Accessor on the i-th child's label in the list of children of a given node
/// in the scenegraph.
/**
 *  @param aLabel:  the label in the scenegraph
 *  @param i:   the ID of the child
 *  @return the label corresponding to the i-th child
 */
//------------------------------------------------------------------------------
std::string getChildLabel(const std::string& aLabel, unsigned int i);


//------------------------------------------------------------------------------
/// Accessor on the local transformation matrix of a given node in
/// the scenegraph. If the node does not exist, the identity matrix is returned.
/**
 *  @param aLabel:  the label in the scenegraph
 *  @return the corresponding local transformation matrix
 */
//------------------------------------------------------------------------------
std::vector<double> getLocalTransformationMatrix(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Set the local transformation matrix of a given node in the scenegraph.
/**
 *  @param aLabel:  the label in the scenegraph
 *  @param aMatrix: the local transformation matrix
 */
//------------------------------------------------------------------------------
void setLocalTransformationMatrix(const std::string& aLabel,
    const std::vector<double>& aMatrix);


//------------------------------------------------------------------------------
/// Apply the current local transformation to all the vertices of a given node in the scenegraph.
/// Note the the local transformation is then reset to the identity matrix.
/**
 *  @param aLabel:  the label in the scenegraph
 */
//------------------------------------------------------------------------------
void applyCurrentLocalTransformation(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Access the bounding box of a given node and all its children (if any).
/// The bounding box is given in the world coordinate system.
/**
 *  @param aLabel:  the label in the scenegraph
 *  @param aUnitOfLength:   the unit of length corresponding to the bounding box.
 *                          Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 *                          (default value: "cm")
 *  @return the bounding box as: min_x, min_y, min_z, max_x, max_y, max_z
 */
//------------------------------------------------------------------------------
std::vector<double> getNodeAndChildrenBoundingBox(const std::string& aLabel,
    const std::string& aUnitOfLength = "cm");


//------------------------------------------------------------------------------
/// Access the bounding box of a given node (without its children).
/// The bounding box is given in the world coordinate system.
/**
 *  @param aLabel:  the label in the scenegraph
 *  @param aUnitOfLength:   the unit of length corresponding to the bounding box.
 *                          Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 *                          (default value: "cm")
 *  @return the bounding box as: min_x, min_y, min_z, max_x, max_y, max_z
 */
//------------------------------------------------------------------------------
std::vector<double> getNodeOnlyBoundingBox(const std::string& aLabel,
    const std::string& aUnitOfLength = "cm");


//------------------------------------------------------------------------------
/// Access the centre of the bounding box of a given node (including its children).
/// The centre is given in the world coordinate system.
/**
 *  @param aLabel:  the label in the scenegraph
 *  @param aUnitOfLength:   the unit of length corresponding to the bounding box.
 *                          Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 *                          (default value: "cm")
 *  @return the bounding box as: min_x, min_y, min_z, max_x, max_y, max_z
 */
//------------------------------------------------------------------------------
std::vector<double> getNodeAndChildrenBoundingBoxCentre(const std::string& aLabel,
    const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Access the centre of the bounding box of a given node (including its children).
/// The centre is given in the world coordinate system.
/**
 *  @param aLabel:  the label in the scenegraph
 *  @param aUnitOfLength:   the unit of length corresponding to the bounding box.
 *                          Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 *                          (default value: "cm")
 *  @return the bounding box as: min_x, min_y, min_z, max_x, max_y, max_z
 */
//------------------------------------------------------------------------------
std::vector<double> getNodeAndChildrenBoundingBoxCenter(const std::string& aLabel,
    const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Access the centre of the bounding box of a given node (without its children).
/// The centre is given in the world coordinate system.
/**
 *  @param aLabel:  the label in the scenegraph
 *  @param aUnitOfLength:   the unit of length corresponding to the bounding box.
 *                          Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 *                          (default value: "cm")
 *  @return the bounding box as: min_x, min_y, min_z, max_x, max_y, max_z
 */
//------------------------------------------------------------------------------
std::vector<double> getNodeOnlyBoundingBoxCentre(const std::string& aLabel,
    const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Access the centre of the bounding box of a given node (without its children).
/// The centre is given in the world coordinate system.
/**
 *  @param aLabel:  the label in the scenegraph
 *  @param aUnitOfLength:   the unit of length corresponding to the bounding box.
 *                          Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 *                          (default value: "cm")
 *  @return the bounding box as: min_x, min_y, min_z, max_x, max_y, max_z
 */
//------------------------------------------------------------------------------
std::vector<double> getNodeOnlyBoundingBoxCenter(const std::string& aLabel,
    const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Invert the normal vectors of a given polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh
 */
//------------------------------------------------------------------------------
void invertNormalVectors(const std::string& aLabel);


//--------------------------------------------------------------------------
/// Check if the normal vectors have been inverted.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @return true if the normal vectors have been inverted; false otherwise.
 */
//--------------------------------------------------------------------------
bool hasNormalVectorsInverted(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Set the color of a given polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @param R:   the red channel
 *  @param G:   the green channel
 *  @param B:   the blue channel
 *  @param A:   the alpha channel
 */
//------------------------------------------------------------------------------
void setColor(const std::string& aLabel, float R, float G, float B, float A);


//------------------------------------------------------------------------------
/// Set the colour of a given polygon mesh. It will be applied to the ambiant, 
/// diffuse and specular colours.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @param R:   the red channel
 *  @param G:   the green channel
 *  @param B:   the blue channel
 *  @param A:   the alpha channel
 */
//------------------------------------------------------------------------------
void setColour(const std::string& aLabel, float R, float G, float B, float A);


//------------------------------------------------------------------------------
/// Set the ambiant colour of a given polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @param R:   the red channel
 *  @param G:   the green channel
 *  @param B:   the blue channel
 *  @param A:   the alpha channel
 */
//------------------------------------------------------------------------------
void setAmbiantColour(const std::string& aLabel, float R, float G, float B, float A);


//------------------------------------------------------------------------------
/// Accessor on the ambient colour of the material of a given polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @return the ambient colour
 */
//------------------------------------------------------------------------------
std::vector<float> getAmbientColour(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Set the diffuse colour of a given polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @param R:   the red channel
 *  @param G:   the green channel
 *  @param B:   the blue channel
 *  @param A:   the alpha channel
 */
//------------------------------------------------------------------------------
void setDiffuseColour(const std::string& aLabel, float R, float G, float B, float A);


//------------------------------------------------------------------------------
/// Accessor on the diffuse colour of the material of a given polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @return the diffuse colour
 */
//------------------------------------------------------------------------------
std::vector<float> getDiffuseColour(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Set the specular colour of a given polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @param R:   the red channel
 *  @param G:   the green channel
 *  @param B:   the blue channel
 *  @param A:   the alpha channel
 */
//------------------------------------------------------------------------------
void setSpecularColour(const std::string& aLabel, float R, float G, float B, float A);


//------------------------------------------------------------------------------
/// Accessor on the specular colour of the material of a given polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @return the specular colour
 */
//------------------------------------------------------------------------------
std::vector<float> getSpecularColour(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Set the shininess of the material of a given polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @param aShininessValue: the shininess coefficient
 */
//------------------------------------------------------------------------------
void setShininess(const std::string& aLabel, float aShininessValue);


//------------------------------------------------------------------------------
/// Accessor on the shininess of the material of a given polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @return the shininess coefficient
 */
//------------------------------------------------------------------------------
float getShininess(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Add a polygon mesh, given its label, to the X-ray renderer as
/// an inner surface.
/**
 *  @param aLabel:  the label of the polygon mesh in the scenegraph
 */
//------------------------------------------------------------------------------
void addPolygonMeshAsInnerSurface(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Add a polygon mesh, given its label, to the X-ray renderer as
/// an outer surface.
/**
 *  @param aLabel:  the label of the polygon mesh in the scenegraph
 */
//------------------------------------------------------------------------------
void addPolygonMeshAsOuterSurface(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Empty the scenegraph. Note that it also empties the X-ray renderer
/// from all its meshes.
//------------------------------------------------------------------------------
void removePolygonMeshesFromSceneGraph();


//------------------------------------------------------------------------------
/// Empty the X-ray renderer from all its meshes.
/// Note that the meshes are kept in the scenegraph.
//------------------------------------------------------------------------------
void removePolygonMeshesFromXRayRenderer();


//------------------------------------------------------------------------------
/// Compute the current source-object distance (SOD) for a given object in
/// a cone-beam geometry.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @param aUnitOfLength:   the unit of length corresponding to the SOD
 *                          in the file. Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 *  @return the current source-object distance
 */
//------------------------------------------------------------------------------
float getSOD(const std::string& aLabel,
    const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Compute the optimal source-object distance (SOD) for a given object in
/// a cone-beam geometry.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @param aUnitOfLength:   the unit of length corresponding to the SOD
 *                          in the file. Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 *  @param aMultiplicativeFactor:   a multiplicative factor applied on 
 *                                  the optimal SOD to move the sample slightly 
 *                                  off the detector (default value: 1.05)
 *  @return the optimal source-object distance
 */
//------------------------------------------------------------------------------
float getBestSOD(const std::string& aLabel,
    const std::string& aUnitOfLength,
    float aMultiplicativeFactor = 1.05);


//------------------------------------------------------------------------------
/// Change the object position given a fixed source and detector system.
/**
 *  @param aLabel:  the label of the polygon mesh
 *  @param aSourceObjectDistance: the source-object distance (SOD)
 *  @param aUnitOfLength:   the unit of length corresponding to the SOD
 *                          in the file. Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 */
//------------------------------------------------------------------------------
void applySOD(const std::string& aLabel,
    float aSourceObjectDistance,
    const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Move the scenegraph to the centre.
//------------------------------------------------------------------------------
void moveToCenter();


//------------------------------------------------------------------------------
/// Move the scenegraph to the centre.
//------------------------------------------------------------------------------
void moveToCentre();


//------------------------------------------------------------------------------
/// Move a polygon mesh to the centre.
/**
 *  @param aLabel:  the label of the polygon mesh in the scenegraph
 */
 //------------------------------------------------------------------------------
void moveToCenter(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Move a polygon mesh to the centre.
/**
 *  @param aLabel:  the label of the polygon mesh in the scenegraph
 */
 //------------------------------------------------------------------------------
void moveToCentre(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Scale a polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh to transform
 *  @param x:   the scaling factor along the X-axis
 *  @param y:   the scaling factor along the Y-axis
 *  @param z:   the scaling factor along the Z-axis
 */
 //------------------------------------------------------------------------------
void scaleNode(const std::string& aLabel,
    double x,
    double y,
    double z);


//------------------------------------------------------------------------------
/// Rotate a polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh to transform
 *  @param anAngle:   the rotation angle in degrees
 *  @param x:   the component of the rotation vector along the X-axis
 *  @param y:   the component of the rotation vector along the Y-axis
 *  @param z:   the component of the rotation vector along the Z-axis
 */
 //------------------------------------------------------------------------------
void rotateNode(const std::string& aLabel,
    double anAngle,
    double x,
    double y,
    double z);


//------------------------------------------------------------------------------
/// Rotate a polygon mesh using the yaw, pitch, roll method
/**
 *  @param aLabel:  the label of the polygon mesh to transform
 *  @param yaw: a counterclockwise rotation of yaw degrees about the z-axis
 *  @param pitch: a counterclockwise rotation of yaw degrees about the y-axis
 *  @param roll: a counterclockwise rotation of yaw degrees about the x-axis
 */
 //------------------------------------------------------------------------------
void rotateNode(const std::string& aLabel,
    double yaw, double pitch, double roll);


//------------------------------------------------------------------------------
/// Translate a polygon mesh.
/**
 *  @param aLabel:  the label of the polygon mesh to transform
 *  @param x:   the component of the translation vector along the X-axis
 *  @param y:   the component of the translation vector along the Y-axis
 *  @param z:   the component of the translation vector along the Z-axis
 *  @param aUnitOfLength:   the unit of length corresponding to the x, y and z
 *                          parameters. Acceptable values are:
 *                          "um", "micrometre",  "micrometer",
 *                          "mm", "millimetre",  "millimeter",
 *                          "cm", "centimetre",  "centimeter",
 *                          "dm",  "decimetre",  "decimeter",
 *                          "m",   "metre",      "meter",
 *                          "dam", "decametre",  "decameter",
 *                          "hm",  "hectometre", "hectometer",
 *                          "km",  "kilometre",  "kilometer"
 */
 //------------------------------------------------------------------------------
void translateNode(const std::string& aLabel,
    double x,
    double y,
    double z,
    const std::string& aUnitOfLength);


//------------------------------------------------------------------------------
/// Use a shear matrix  to transform the node.
/**
 *  @param aLabel:  the label of the polygon mesh to transform
 *  @param yx:   the yx component of the shear matrix
 *  @param zx:   the zx component of the shear matrix
 *  @param xy:   the xy component of the shear matrix
 *  @param zy:   the zy component of the shear matrix
 *  @param xz:   the xz component of the shear matrix
 *  @param yz:   the yz component of the shear matrix
 */
 //------------------------------------------------------------------------------
void shearNode(const std::string& aLabel,
    double yx,
    double zx,
    double xy,
    double zy,
    double xz,
    double yz);










#endif
