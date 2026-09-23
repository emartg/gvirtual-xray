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

3. Neither the name of the UK Research and Innovation nor the names of its contributors
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


/**
********************************************************************************
*
*   @file       IsoSurface.cxx
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
#ifndef SGVXR_Internal_h
#include "internal.h"
#endif

#ifndef SGVXR_IsoSurface_h
#include "IsoSurface.h"
#endif


//******************************************************************************
//  Namespace
//******************************************************************************


//******************************************************************************
//  Global variables
//******************************************************************************


//******************************************************************************
//  Function definitions
//******************************************************************************


//-------------------------------------------------------------------------------
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
                    const std::string& aParent,
                    unsigned char aVerboseLevel)
//-------------------------------------------------------------------------------
{
    _makeIsoSurface<float>(aMeshLabel,
        apVolume,
        anIsoValue,
        aCentrePositionX,
        aCentrePositionY,
        aCentrePositionZ,
        aVoxelWidth,
        aVoxelHeight,
        aVoxelDepth,
        aUnitOfLength,
        aParent,
        aVerboseLevel);
}


//-------------------------------------------------------------------------------
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
                    const std::string& aParent,
                    unsigned char aVerboseLevel)
//-------------------------------------------------------------------------------
{
    _makeIsoSurface<float>(aMeshLabel,
        apVolume,
        aLowerIsoValue,
        anUpperIsoValue,
        aCentrePositionX,
        aCentrePositionY,
        aCentrePositionZ,
        aVoxelWidth,
        aVoxelHeight,
        aVoxelDepth,
        aUnitOfLength,
        aParent,
        aVerboseLevel);
}


//---------------------------------------------------
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
                    const std::string& aParent,
                    unsigned char aVerboseLevel)
//---------------------------------------------------
{
    _makeIsoSurface<float>(aMeshLabel,
        apVolume,
        aNumberOfVoxelsZ,
        aNumberOfVoxelsY,
        aNumberOfVoxelsX,
        anIsoValue,
        aCentrePositionX,
        aCentrePositionY,
        aCentrePositionZ,
        aVoxelWidth,
        aVoxelHeight,
        aVoxelDepth,
        aUnitOfLength,
        aParent,
        aVerboseLevel);
}


//---------------------------------------------------
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
                    const std::string& aParent,
                    unsigned char aVerboseLevel)
//---------------------------------------------------
{
    _makeIsoSurface<float>(aMeshLabel,
        apVolume,
        aNumberOfVoxelsZ,
        aNumberOfVoxelsY,
        aNumberOfVoxelsX,
        aLowerIsoValue,
        anUpperIsoValue,
        aCentrePositionX,
        aCentrePositionY,
        aCentrePositionZ,
        aVoxelWidth,
        aVoxelHeight,
        aVoxelDepth,
        aUnitOfLength,
        aParent,
        aVerboseLevel);
}


//-----------------------------------------------------------------------------
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
                    const std::string& aParent,
                    unsigned char aVerboseLevel)
//-----------------------------------------------------------------------------
{
    _makeIsoSurface<int>(aMeshLabel,
        apVolume,
        anIsoValue,
        aCentrePositionX,
        aCentrePositionY,
        aCentrePositionZ,
        aVoxelWidth,
        aVoxelHeight,
        aVoxelDepth,
        aUnitOfLength,
        aParent,
        aVerboseLevel);
}


//-----------------------------------------------------------------------------
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
                    const std::string& aParent,
                    unsigned char aVerboseLevel)
//-----------------------------------------------------------------------------
{
    _makeIsoSurface<int>(aMeshLabel,
        apVolume,
        aLowerIsoValue,
        anUpperIsoValue,
        aCentrePositionX,
        aCentrePositionY,
        aCentrePositionZ,
        aVoxelWidth,
        aVoxelHeight,
        aVoxelDepth,
        aUnitOfLength,
        aParent,
        aVerboseLevel);
}


//---------------------------------------------------
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
                    const std::string& aParent,
                    unsigned char aVerboseLevel)
//---------------------------------------------------
{
    _makeIsoSurface<int>(aMeshLabel,
        apVolume,
        aNumberOfVoxelsZ,
        aNumberOfVoxelsY,
        aNumberOfVoxelsX,
        anIsoValue,
        aCentrePositionX,
        aCentrePositionY,
        aCentrePositionZ,
        aVoxelWidth,
        aVoxelHeight,
        aVoxelDepth,
        aUnitOfLength,
        aParent,
        aVerboseLevel);
}


//---------------------------------------------------
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
                    const std::string& aParent,
                    unsigned char aVerboseLevel)
//---------------------------------------------------
{
    _makeIsoSurface<int>(aMeshLabel,
        apVolume,
        aNumberOfVoxelsZ,
        aNumberOfVoxelsY,
        aNumberOfVoxelsX,
        aLowerIsoValue,
        anUpperIsoValue,
        aCentrePositionX,
        aCentrePositionY,
        aCentrePositionZ,
        aVoxelWidth,
        aVoxelHeight,
        aVoxelDepth,
        aUnitOfLength,
        aParent,
        aVerboseLevel);
}
