/*

Copyright (c) 2025-present, Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), 
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


#ifndef SGVXR_IsoSurface_h
#define SGVXR_IsoSurface_h


/**
********************************************************************************
*
*   @file       IsoSurface.h
*
*   @brief      Expose here all the functions of SimpleGVXR related to 
*               iso-surfacing.
*
*   @version    1.0
*
*   @date       21/07/2025
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
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, July 2025-present, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <string>
#include <vector>


//--------------------------------------------------------------------------
/// Apply the marching cubes algorithm on a specific volume to extract 
/// the polygon mesh and add the node in the scenegraph
/**
*  @param aMeshLabel:  the label identifying the polygon mesh in the scenegraph
*  @param apVolume: the volume
*  @param anIsoValue:    the iso value
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
*  @param aParent: the parent of the node in the scenegraph (default: "root")
*  @param aVerboseLevel:   the level of statements in the standard output. For no output, use 0 (default value: 0)
*/
//--------------------------------------------------------------------------
void makeIsoSurface(const std::string& aMeshLabel,
    const std::vector<std::vector<std::vector<float>>>& apVolume,
    float anIsoValue,
    float aCentrePositionX,
    float aCentrePositionY,
    float aCentrePositionZ,
    float aVoxelWidth,
    float aVoxelHeight,
    float aVoxelDepth,
    const std::string& aUnitOfLength,
    const std::string& aParent = "root",
    unsigned char aVerboseLevel = 0
);


//--------------------------------------------------------------------------
/// Apply the marching cubes algorithm on a specific volume to extract 
/// the polygon mesh and add the node in the scenegraph
/**
*  @param aMeshLabel:  the label identifying the polygon mesh in the scenegraph
*  @param apVolume: the volume
*  @param aLowerIsoValue:    the lower limit of the iso value
*  @param anUpperIsoValue:    the upper limit of the the iso value
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
*  @param aParent: the parent of the node in the scenegraph (default: "root")
*  @param aVerboseLevel:   the level of statements in the standard output. For no output, use 0 (default value: 0)
*/
//--------------------------------------------------------------------------
void makeIsoSurface(const std::string& aMeshLabel,
    const std::vector<std::vector<std::vector<float>>>& apVolume,
    float aLowerIsoValue,
    float anUpperIsoValue,
    float aCentrePositionX,
    float aCentrePositionY,
    float aCentrePositionZ,
    float aVoxelWidth,
    float aVoxelHeight,
    float aVoxelDepth,
    const std::string& aUnitOfLength,
    const std::string& aParent = "root",
    unsigned char aVerboseLevel = 0
);


//--------------------------------------------------------------------------
/// Apply the marching cubes algorithm on a specific volume to extract 
/// the polygon mesh and add the node in the scenegraph
/**
*  @param aMeshLabel:  the label identifying the polygon mesh in the scenegraph
*  @param apVolume: the volume
*  @param aNumberOfVoxelsZ:  the number of voxels along the Z-axis
*  @param aNumberOfVoxelsY:  the number of voxels along the Y-axis
*  @param aNumberOfVoxelsX:  the number of voxels along the X-axis
*  @param anIsoValue:    the iso value
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
*  @param aParent: the parent of the node in the scenegraph (default: "root")
*  @param aVerboseLevel:   the level of statements in the standard output. For no output, use 0 (default value: 0)
*/
//--------------------------------------------------------------------------
void makeIsoSurface(const std::string& aMeshLabel,
    const float* apVolume,
    int aNumberOfVoxelsZ,
    int aNumberOfVoxelsY,
    int aNumberOfVoxelsX,
    float anIsoValue,
    float aCentrePositionX,
    float aCentrePositionY,
    float aCentrePositionZ,
    float aVoxelWidth,
    float aVoxelHeight,
    float aVoxelDepth,
    const std::string& aUnitOfLength,
    const std::string& aParent = "root",
    unsigned char aVerboseLevel = 0
);


//--------------------------------------------------------------------------
/// Apply the marching cubes algorithm on a specific volume to extract 
/// the polygon mesh and add the node in the scenegraph
/**
*  @param aMeshLabel:  the label identifying the polygon mesh in the scenegraph
*  @param apVolume: the volume
*  @param aNumberOfVoxelsZ:  the number of voxels along the Z-axis
*  @param aNumberOfVoxelsY:  the number of voxels along the Y-axis
*  @param aNumberOfVoxelsX:  the number of voxels along the X-axis
*  @param aLowerIsoValue:    the lower limit of the iso value
*  @param anUpperIsoValue:    the upper limit of the the iso value
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
*  @param aParent: the parent of the node in the scenegraph (default: "root")
*  @param aVerboseLevel:   the level of statements in the standard output. For no output, use 0 (default value: 0)
*/
//--------------------------------------------------------------------------
void makeIsoSurface(const std::string& aMeshLabel,
    const float* apVolume,
    int aNumberOfVoxelsZ,
    int aNumberOfVoxelsY,
    int aNumberOfVoxelsX,
    float aLowerIsoValue,
    float anUpperIsoValue,
    float aCentrePositionX,
    float aCentrePositionY,
    float aCentrePositionZ,
    float aVoxelWidth,
    float aVoxelHeight,
    float aVoxelDepth,
    const std::string& aUnitOfLength,
    const std::string& aParent = "root",
    unsigned char aVerboseLevel = 0
);


//--------------------------------------------------------------------------
/// Apply the marching cubes algorithm on a specific volume to extract 
/// the polygon mesh and add the node in the scenegraph
/**
*  @param aMeshLabel:  the label identifying the polygon mesh in the scenegraph
*  @param apVolume: the volume
*  @param anIsoValue:    the iso value
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
*  @param aParent: the parent of the node in the scenegraph (default: "root")
*  @param aVerboseLevel:   the level of statements in the standard output. For no output, use 0 (default value: 0)
*/
//--------------------------------------------------------------------------
void makeIsoSurface(const std::string& aMeshLabel,
    const std::vector<std::vector<std::vector<int>>>& apVolume,
    int anIsoValue,
    float aCentrePositionX,
    float aCentrePositionY,
    float aCentrePositionZ,
    float aVoxelWidth,
    float aVoxelHeight,
    float aVoxelDepth,
    const std::string& aUnitOfLength,
    const std::string& aParent = "root",
    unsigned char aVerboseLevel = 0
);


//--------------------------------------------------------------------------
/// Apply the marching cubes algorithm on a specific volume to extract 
/// the polygon mesh and add the node in the scenegraph
/**
*  @param aMeshLabel:  the label identifying the polygon mesh in the scenegraph
*  @param apVolume: the volume
*  @param aLowerIsoValue:    the lower limit of the iso value
*  @param anUpperIsoValue:    the upper limit of the the iso value
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
*  @param aParent: the parent of the node in the scenegraph (default: "root")
*  @param aVerboseLevel:   the level of statements in the standard output. For no output, use 0 (default value: 0)
*/
//--------------------------------------------------------------------------
void makeIsoSurface(const std::string& aMeshLabel,
    const std::vector<std::vector<std::vector<int>>>& apVolume,
    int aLowerIsoValue,
    int anUpperIsoValue,
    float aCentrePositionX,
    float aCentrePositionY,
    float aCentrePositionZ,
    float aVoxelWidth,
    float aVoxelHeight,
    float aVoxelDepth,
    const std::string& aUnitOfLength,
    const std::string& aParent = "root",
    unsigned char aVerboseLevel = 0
);


//--------------------------------------------------------------------------
/// Apply the marching cubes algorithm on a specific volume to extract 
/// the polygon mesh and add the node in the scenegraph
/**
*  @param aMeshLabel:  the label identifying the polygon mesh in the scenegraph
*  @param apVolume: the volume
*  @param aNumberOfVoxelsZ:  the number of voxels along the Z-axis
*  @param aNumberOfVoxelsY:  the number of voxels along the Y-axis
*  @param aNumberOfVoxelsX:  the number of voxels along the X-axis
*  @param anIsoValue:    the iso value
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
*  @param aParent: the parent of the node in the scenegraph (default: "root")
*  @param aVerboseLevel:   the level of statements in the standard output. For no output, use 0 (default value: 0)
*/
//--------------------------------------------------------------------------
void makeIsoSurface(const std::string& aMeshLabel,
    const int* apVolume,
    int aNumberOfVoxelsZ,
    int aNumberOfVoxelsY,
    int aNumberOfVoxelsX,
    int anIsoValue,
    float aCentrePositionX,
    float aCentrePositionY,
    float aCentrePositionZ,
    float aVoxelWidth,
    float aVoxelHeight,
    float aVoxelDepth,
    const std::string& aUnitOfLength,
    const std::string& aParent = "root",
    unsigned char aVerboseLevel = 0
);


//--------------------------------------------------------------------------
/// Apply the marching cubes algorithm on a specific volume to extract 
/// the polygon mesh and add the node in the scenegraph
/**
*  @param aMeshLabel:  the label identifying the polygon mesh in the scenegraph
*  @param apVolume: the volume
*  @param aNumberOfVoxelsZ:  the number of voxels along the Z-axis
*  @param aNumberOfVoxelsY:  the number of voxels along the Y-axis
*  @param aNumberOfVoxelsX:  the number of voxels along the X-axis
*  @param aLowerIsoValue:    the lower limit of the iso value
*  @param anUpperIsoValue:    the upper limit of the the iso value
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
*  @param aParent: the parent of the node in the scenegraph (default: "root")
*  @param aVerboseLevel:   the level of statements in the standard output. For no output, use 0 (default value: 0)
*/
//--------------------------------------------------------------------------
void makeIsoSurface(const std::string& aMeshLabel,
    const int* apVolume,
    int aNumberOfVoxelsZ,
    int aNumberOfVoxelsY,
    int aNumberOfVoxelsX,
    int aLowerIsoValue,
    int anUpperIsoValue,
    float aCentrePositionX,
    float aCentrePositionY,
    float aCentrePositionZ,
    float aVoxelWidth,
    float aVoxelHeight,
    float aVoxelDepth,
    const std::string& aUnitOfLength,
    const std::string& aParent = "root",
    unsigned char aVerboseLevel = 0
);


#endif
