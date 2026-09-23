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


#ifndef SGVXR_IMPLICIT_SURFACE_H
#define SGVXR_IMPLICIT_SURFACE_H


/**
********************************************************************************
*
*   @file       ImplicitSurface.h
*
*   @brief      Expose here all the functions of SimpleGVXR related to 
*               implicit modelling, e.g. metaballs, soft objects, 
*               blobby molecules. Both control points and line segments can be used.
*
*   @version    1.0
*
*   @date       10/06/2024
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
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, June 2024-present, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <string>
#include <vector>


//------------------------------------------------------------------------------
/// Get the total number of implicit surfaces that have been added to 
/// the scenegraph.
/**
*  @return the number of implicit surfaces
*/
//------------------------------------------------------------------------------
unsigned int getNumberOfImplicitSurfaces();


//------------------------------------------------------------------------------
/// Get the labels of all the implicit surfaces that have been added to 
/// the scenegraph.
/**
*  @return the set of implicit surface labels
*/
//------------------------------------------------------------------------------
std::vector<std::string> getImplicitSurfaceLabelSet();


//------------------------------------------------------------------------------
/// Check whether a given label is used to identify an implicit surface.
/**
*  @param aLabel:  the label identifying a potential implicit surface
*  @return true if the label identify an implicit surface, otherwise retrun false 
*/
//------------------------------------------------------------------------------
bool implicitSurfaceExists(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Create a new implicit surface.
/**
*  @param aLabel:  the label identifying the new implicit surface
*/
//------------------------------------------------------------------------------
void createImplicitSurface(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Destroy a given implicit surface.
/**
*  @param aLabel:  the label identifying the implicit surface
*/
//------------------------------------------------------------------------------
void destroyImplicitSurface(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Destroy all the existing implicit surfaces.
//------------------------------------------------------------------------------
void destroyAllImplicitSurfaces();


//------------------------------------------------------------------------------
/// Add a control point to an existing implicit surface. See the excellent webpage by 
/// Paul Bourke: https://paulbourke.net/geometry/implicitsurf/index.html
/**
*  @param aLabel:   the label identifying the implicit surface
*  @param aPositionX:   the position of the point along the X-axis
*  @param aPositionY:   the position of the point along the Y-axis
*  @param aPositionZ:   the position of the point along the Z-axis
*  @param aUnitOfLength:    the unit of length corresponding to aPositionX, aPositionY and aPositionZ.
*                           Acceptable values are:
*                           "um", "micrometre",  "micrometer",
*                           "mm", "millimetre",  "millimeter",
*                           "cm", "centimetre",  "centimeter",
*                           "dm",  "decimetre",  "decimeter",
*                           "m",   "metre",      "meter",
*                           "dam", "decametre",  "decameter",
*                           "hm",  "hectometre", "hectometer",
*                           "km",  "kilometre",  "kilometer"
*  @param aDentityFunction: the name of the density function. Accepted values are:\n
*                           "sphere", which uses the density function \f$D(r) = \frac{1}{r^2}\f$.\n
*                           "blobby molecule", which uses the density density function \f$a \mathrm{e}^{-b r^2}\f$\n
*                           \f$b\f$ is related to the standard deviation of the curve, \f$a\f$ to the height. The function should be recognised as a Gaussian curve centered at the origin, actually a half Gaussian as \f$r \geq 0\f$.\n
*                           "metaball", which uses the density function 
*                           \f$\left\{ 
*                           \begin{array}{ c l }
*                              a \left(1 - \frac{3 r^2}{b^2}\right) & \quad \textrm{if } 0 \leq r \leq b/3 \\
*                              \frac{3 a}{2} \left(1 - \frac{r}{b}\right)^2 & \quad \textrm{if } b/3 \leq r \leq b \\
*                               0                 & \quad \textrm{otherwise}
*                           \end{array}
*                           \right.\f$\n
*                           \f$a\f$ is a scaling factor, \f$b\f$ is the maximum distance a control primitive contributes to the field.\n
*                           "soft object", which uses the density function 
*                           \f$\left\{ 
*                           \begin{array}{ c l }
*                              a \left(1 - \frac{4r^6}{9b^6} + \frac{17r^4}{9b^4} - \frac{22r^2}{9b^2}\right) & \quad \textrm{if } 0 \leq r \leq b \\
*                               0                 & \quad \textrm{otherwise}
*                           \end{array}
*                           \right.\f$\n
*                           \f$a\f$ scales the function, each control primitive has no influence after a distance \f$b\f$. This function has a slight advantage over that for Meta balls as it only uses squares of the distance and so it isn't necessary to compute square roots.
*  @param aParameterA:  \f$a\f$ in the equations above
*  @param aParameterB:  \f$b\f$ in the equations above (default value: 0.0)
*/
//------------------------------------------------------------------------------
void addControlPoint(const std::string& aLabel,
    float aPositionX,
    float aPositionY,
    float aPositionZ,
    const std::string& aUnitOfLength,
    const std::string& aDentityFunction,
    float aParameterA,
    float aParameterB = 0.0
);


//------------------------------------------------------------------------------
/// Add a control point to an existing implicit surface. See the excellent webpage by 
/// Paul Bourke: https://paulbourke.net/geometry/implicitsurf/index.html
/**
*  @param aLabel:   the label identifying the implicit surface
*  @param aPosition1X:   the position of the 1st point defining the line segment along the X-axis
*  @param aPosition1Y:   the position of the 1st point defining the line segment along the Y-axis
*  @param aPosition1Z:   the position of the 1st point defining the line segment along the Z-axis
*  @param aPosition2X:   the position of the 2nd point defining the line segment along the X-axis
*  @param aPosition2Y:   the position of the 2nd point defining the line segment along the Y-axis
*  @param aPosition2Z:   the position of the 2nd point defining the line segment along the Z-axis
*  @param aUnitOfLength:    the unit of length corresponding to aPositionX, aPositionY and aPositionZ.
*                           Acceptable values are:
*                           "um", "micrometre",  "micrometer",
*                           "mm", "millimetre",  "millimeter",
*                           "cm", "centimetre",  "centimeter",
*                           "dm",  "decimetre",  "decimeter",
*                           "m",   "metre",      "meter",
*                           "dam", "decametre",  "decameter",
*                           "hm",  "hectometre", "hectometer",
*                           "km",  "kilometre",  "kilometer"
*  @param aDentityFunction: the name of the density function. Accepted values are:\n
*                           "sphere", which uses the density function \f$D(r) = \frac{1}{r^2}\f$.\n
*                           "blobby molecule", which uses the density density function \f$a \mathrm{e}^{-b r^2}\f$\n
*                           \f$b\f$ is related to the standard deviation of the curve, \f$a\f$ to the height. The function should be recognised as a Gaussian curve centered at the origin, actually a half Gaussian as \f$r \geq 0\f$.\n
*                           "metaball", which uses the density function 
*                           \f$\left\{ 
*                           \begin{array}{ c l }
*                              a \left(1 - \frac{3 r^2}{b^2}\right) & \quad \textrm{if } 0 \leq r \leq b/3 \\
*                              \frac{3 a}{2} \left(1 - \frac{r}{b}\right)^2 & \quad \textrm{if } b/3 \leq r \leq b \\
*                               0                 & \quad \textrm{otherwise}
*                           \end{array}
*                           \right.\f$\n
*                           \f$a\f$ is a scaling factor, \f$b\f$ is the maximum distance a control primitive contributes to the field.\n
*                           "soft object", which uses the density function 
*                           \f$\left\{ 
*                           \begin{array}{ c l }
*                              a \left(1 - \frac{4r^6}{9b^6} + \frac{17r^4}{9b^4} - \frac{22r^2}{9b^2}\right) & \quad \textrm{if } 0 \leq r \leq b \\
*                               0                 & \quad \textrm{otherwise}
*                           \end{array}
*                           \right.\f$\n
*                           \f$a\f$ scales the function, each control primitive has no influence after a distance \f$b\f$. This function has a slight advantage over that for Meta balls as it only uses squares of the distance and so it isn't necessary to compute square roots.
*  @param aParameterA:  \f$a\f$ in the equations above
*  @param aParameterB:  \f$b\f$ in the equations above (default value: 0.0)
*/
//------------------------------------------------------------------------------
void addControlLineSegment(const std::string& aLabel,
    float aPosition1X,
    float aPosition1Y,
    float aPosition1Z,
    float aPosition2X,
    float aPosition2Y,
    float aPosition2Z,
    const std::string& aUnitOfLength,
    const std::string& aDentityFunction,
    float aParameterA,
    float aParameterB = 0.0
);


//------------------------------------------------------------------------------
/// Accessor on the total number of control points in a given implicit surface.
/**
*  @param aLabel:  the label identifying the implicit surface
*  @return the total number of control points in the implicit surface
*/
//------------------------------------------------------------------------------
unsigned int getNumberOfControlPoints(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Accessor on the total number of control line segments in a given implicit surface.
/**
*  @param aLabel:  the label identifying the implicit surface
*  @return the total number of line segments in the implicit surface
*/
//------------------------------------------------------------------------------
unsigned int getNumberOfControlLineSegments(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Change the density function and the parameters \f$a\f$ and \f$b\f$ of a given control point of a given implicit surface.
/**
*  @param aLabel:  the label identifying the implicit surface
*  @param aPointID:  the numerial ID of the control point
*  @param aDentityFunction: the name of the density function. Accepted values are:\n
*                           "sphere", which uses the density function \f$D(r) = \frac{1}{r^2}\f$.\n
*                           "blobby molecule", which uses the density density function \f$a \mathrm{e}^{-b r^2}\f$\n
*                           \f$b\f$ is related to the standard deviation of the curve, \f$a\f$ to the height. The function should be recognised as a Gaussian curve centered at the origin, actually a half Gaussian as \f$r \geq 0\f$.\n
*                           "metaball", which uses the density function 
*                           \f$\left\{ 
*                           \begin{array}{ c l }
*                              a \left(1 - \frac{3 r^2}{b^2}\right) & \quad \textrm{if } 0 \leq r \leq b/3 \\
*                              \frac{3 a}{2} \left(1 - \frac{r}{b}\right)^2 & \quad \textrm{if } b/3 \leq r \leq b \\
*                               0                 & \quad \textrm{otherwise}
*                           \end{array}
*                           \right.\f$\n
*                           \f$a\f$ is a scaling factor, \f$b\f$ is the maximum distance a control primitive contributes to the field.\n
*                           "soft object", which uses the density function 
*                           \f$\left\{ 
*                           \begin{array}{ c l }
*                              a \left(1 - \frac{4r^6}{9b^6} + \frac{17r^4}{9b^4} - \frac{22r^2}{9b^2}\right) & \quad \textrm{if } 0 \leq r \leq b \\
*                               0                 & \quad \textrm{otherwise}
*                           \end{array}
*                           \right.\f$\n
*                           \f$a\f$ scales the function, each control primitive has no influence after a distance \f$b\f$. This function has a slight advantage over that for Meta balls as it only uses squares of the distance and so it isn't necessary to compute square roots.
*  @param aParameterA:  \f$a\f$ in the equations above
*  @param aParameterB:  \f$b\f$ in the equations above
*/
//------------------------------------------------------------------------------
void setControlPointParameters(const std::string& aLabel,
    unsigned int aPointID,
    const std::string& aDentityFunction,
    float aParameterA,
    float aParameterB = 0.0
);
                              

//------------------------------------------------------------------------------
/// Change the density function and the parameters \f$a\f$ and \f$b\f$ of a given control line segment of a given implicit surface.
/**
*  @param aLabel:  the label identifying the implicit surface
*  @param aLineSegmentID:  the numerial ID of the control line segment
*  @param aDentityFunction: the name of the density function. Accepted values are:\n
*                           "sphere", which uses the density function \f$D(r) = \frac{1}{r^2}\f$.\n
*                           "blobby molecule", which uses the density density function \f$a \mathrm{e}^{-b r^2}\f$\n
*                           \f$b\f$ is related to the standard deviation of the curve, \f$a\f$ to the height. The function should be recognised as a Gaussian curve centered at the origin, actually a half Gaussian as \f$r \geq 0\f$.\n
*                           "metaball", which uses the density function 
*                           \f$\left\{ 
*                           \begin{array}{ c l }
*                              a \left(1 - \frac{3 r^2}{b^2}\right) & \quad \textrm{if } 0 \leq r \leq b/3 \\
*                              \frac{3 a}{2} \left(1 - \frac{r}{b}\right)^2 & \quad \textrm{if } b/3 \leq r \leq b \\
*                               0                 & \quad \textrm{otherwise}
*                           \end{array}
*                           \right.\f$\n
*                           \f$a\f$ is a scaling factor, \f$b\f$ is the maximum distance a control primitive contributes to the field.\n
*                           "soft object", which uses the density function 
*                           \f$\left\{ 
*                           \begin{array}{ c l }
*                              a \left(1 - \frac{4r^6}{9b^6} + \frac{17r^4}{9b^4} - \frac{22r^2}{9b^2}\right) & \quad \textrm{if } 0 \leq r \leq b \\
*                               0                 & \quad \textrm{otherwise}
*                           \end{array}
*                           \right.\f$\n
*                           \f$a\f$ scales the function, each control primitive has no influence after a distance \f$b\f$. This function has a slight advantage over that for Meta balls as it only uses squares of the distance and so it isn't necessary to compute square roots.
*  @param aParameterA:  \f$a\f$ in the equations above
*  @param aParameterB:  \f$b\f$ in the equations above
*/
//------------------------------------------------------------------------------
void setControlLineSegmentParameters(const std::string& aLabel,
    unsigned int aLineSegmentID,
    const std::string& aDentityFunction,
    float aParameterA,
    float aParameterB = 0.0
);


//------------------------------------------------------------------------------
/// Compute the density function of a given implicit surface. It create a volume (i.e. voxels)
/**
*  @param aLabel:  the label identifying the implicit surface
*  @param aNumberOfVoxelX:  the number of voxels along the X-axis
*  @param aNumberOfVoxelY:  the number of voxels along the Y-axis
*  @param aNumberOfVoxelZ:  the number of voxels along the Z-axis
*  @param aCentrePositionX:  the position of the centre of the volume along the X-axis
*  @param aCentrePositionY:  the position of the centre of the volume along the Y-axis
*  @param aCentrePositionZ:  the position of the centre of the volume along the Z-axis
*  @param aVoxelWidth:  the size of a voxel along the X-axis
*  @param aVoxelHeight:  the size of a voxel along the Y-axis
*  @param aVoxelDepth:  the size of a voxel along the Z-axis
*  @param aUnitOfLength:    the unit of length corresponding to aCentrePositionX, aCentrePositionY, aCentrePositionZ, aVoxelWidth, aVoxelHeight and aVoxelDepth.
*                           Acceptable values are:
*                           "um", "micrometre",  "micrometer",
*                           "mm", "millimetre",  "millimeter",
*                           "cm", "centimetre",  "centimeter",
*                           "dm",  "decimetre",  "decimeter",
*                           "m",   "metre",      "meter",
*                           "dam", "decametre",  "decameter",
*                           "hm",  "hectometre", "hectometer",
*                           "km",  "kilometre",  "kilometer"
*  @param aVerboseLevel:   the level of statements in the standard output. For no output, use 0 (default value: 0)
*/
//------------------------------------------------------------------------------
void voxeliseImplicitSurface(const std::string& aLabel,
    unsigned int aNumberOfVoxelX,
    unsigned int aNumberOfVoxelY,
    unsigned int aNumberOfVoxelZ,
    float aCentrePositionX,
    float aCentrePositionY,
    float aCentrePositionZ,
    float aVoxelWidth,
    float aVoxelHeight,
    float aVoxelDepth,
    const std::string& aUnitOfLength,
    unsigned char aVerboseLevel = 0);


//------------------------------------------------------------------------------
/// Compute the density function of a given implicit surface. It create a volume (i.e. voxels)
/**
*  @param aLabel:  the label identifying the implicit surface
*  @param aNumberOfVoxelX:  the number of voxels along the X-axis
*  @param aNumberOfVoxelY:  the number of voxels along the Y-axis
*  @param aNumberOfVoxelZ:  the number of voxels along the Z-axis
*  @param aCentrePositionX:  the position of the centre of the volume along the X-axis
*  @param aCentrePositionY:  the position of the centre of the volume along the Y-axis
*  @param aCentrePositionZ:  the position of the centre of the volume along the Z-axis
*  @param aVoxelWidth:  the size of a voxel along the X-axis
*  @param aVoxelHeight:  the size of a voxel along the Y-axis
*  @param aVoxelDepth:  the size of a voxel along the Z-axis
*  @param aUnitOfLength:    the unit of length corresponding to aCentrePositionX, aCentrePositionY, aCentrePositionZ, aVoxelWidth, aVoxelHeight and aVoxelDepth.
*                           Acceptable values are:
*                           "um", "micrometre",  "micrometer",
*                           "mm", "millimetre",  "millimeter",
*                           "cm", "centimetre",  "centimeter",
*                           "dm",  "decimetre",  "decimeter",
*                           "m",   "metre",      "meter",
*                           "dam", "decametre",  "decameter",
*                           "hm",  "hectometre", "hectometer",
*                           "km",  "kilometre",  "kilometer"
*  @param aVerboseLevel:   the level of statements in the standard output. For no output, use 0 (default value: 0)
*/
//------------------------------------------------------------------------------
void voxelizeImplicitSurface(const std::string& aLabel,
    unsigned int aNumberOfVoxelX,
    unsigned int aNumberOfVoxelY,
    unsigned int aNumberOfVoxelZ,
    float aCentrePositionX,
    float aCentrePositionY,
    float aCentrePositionZ,
    float aVoxelWidth,
    float aVoxelHeight,
    float aVoxelDepth,
    const std::string& aUnitOfLength,
    unsigned char aVerboseLevel = 0);


//------------------------------------------------------------------------------
/// Accessor on the last voxelisation of a given iso-surface that has been computed.
/**
*  @param aLabel:  the label identifying the polygon mesh in the scenegraph
*  @return the last voxelisation
*/
//------------------------------------------------------------------------------
std::vector<std::vector<std::vector<float>>> getImplicitSurfaceLastVoxelisation(const std::string& aLabel);


//------------------------------------------------------------------------------
/// Accessor on the last voxelisation of a given iso-surface that has been computed.
/**
*  @param aLabel:  the label identifying the polygon mesh in the scenegraph
*  @return the last voxelisation
*/
//------------------------------------------------------------------------------
std::vector<std::vector<std::vector<float>>> getImplicitSurfaceLastVoxelization(const std::string& aLabel);


//--------------------------------------------------------------------------
/// Apply the marching cubes algorithm to extract the polygon mesh and 
/// add the node in the scenegraph
/**
*  @param aImplicitSurfaceLabel:  the label identifying the implicit surface
*  @param aMeshLabel:  the label identifying the polygon mesh in the scenegraph
*  @param anIsoValue:    the iso value
*  @param aParent: the parent of the node in the scenegraph (default: "root")
*  @param aVerboseLevel:   the level of statements in the standard output. For no output, use 0 (default value: 0)
*/
//--------------------------------------------------------------------------
void createMeshFromImplicitSurface(const std::string& aImplicitSurfaceLabel,
    const std::string& aMeshLabel,
    float anIsoValue,
    const std::string& aParent = "root",
    unsigned char aVerboseLevel = 0);


//--------------------------------------------------------------------------
/// Apply the marching cubes algorithm to extract the polygon mesh and 
/// add the node in the scenegraph
/**
*  @param aImplicitSurfaceLabel:  the label identifying the implicit surface
*  @param aMeshLabel:  the label identifying the polygon mesh in the scenegraph
*  @param aLowerIsoValue:    the lower limit of the iso value
*  @param anUpperIsoValue:    the upper limit of the the iso value
*  @param aParent: the parent of the node in the scenegraph (default: "root")
*  @param aVerboseLevel:   the level of statements in the standard output. For no output, use 0 (default value: 0)
*/
//--------------------------------------------------------------------------
void createMeshFromImplicitSurface(const std::string& aImplicitSurfaceLabel,
    const std::string& aMeshLabel,
    float aLowerIsoValue,
    float anUpperIsoValue,
    const std::string& aParent = "root",
    unsigned char aVerboseLevel = 0);


#endif
