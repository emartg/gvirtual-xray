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


#ifndef GVXR_UTILITIES_H
#define GVXR_UTILITIES_H


/**
********************************************************************************
*
*   @file       Utilities.h
*
*   @brief      Some utility functions that do not fit anywhere else.
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
#include <string>


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//------------------------------------------------------------------------------
/// Uncompress data using the Zlib.
/**
*   @param  src:    compressed data
*   @param  srcLen: length of the uncompressed data
*   @param  apDst:  uncompressed data
*/
//------------------------------------------------------------------------------
int inflate(const void *src, int srcLen, char** apDst);


//------------------------------------------------------------------------------
/// Compress data using the Zlib.
/**
*   @param  src:    uncompressed data
*   @param  srcLen: length of the uncompressed data
*   @param  apDst:  compressed data
*/
//------------------------------------------------------------------------------
int deflate(const void *src, int srcLen, char** apDst);


//------------------------------------------------------------------------------
/// Check if the extension of a file name is MAT.
/**
*   @param  aFileName:  the file name to check
*   @return true if the extension of aFileName is MAT; false if it is not
*/
//------------------------------------------------------------------------------
bool isMAT(const char* aFileName);


//------------------------------------------------------------------------------
/// Check if the extension of a file name is DAT.
/**
*   @param  aFileName:  the file name to check
*   @return true if the extension of aFileName is DAT; false if it is not
*/
//------------------------------------------------------------------------------
bool isDAT(const char* aFileName);


//------------------------------------------------------------------------------
/// Check if the extension of a file name is TXT.
/**
*   @param  aFileName:  the file name to check
*   @return true if the extension of aFileName is TXT; false if it is not
*/
//------------------------------------------------------------------------------
bool isTXT(const char* aFileName);


//------------------------------------------------------------------------------
/// Check if the extension of a file name is PGM.
/**
*   @param  aFileName:  the file name to check
*   @return true if the extension of aFileName is PGM; false if it is not
*/
//------------------------------------------------------------------------------
bool isPGM(const char* aFileName);


//------------------------------------------------------------------------------
/// Check if the extension of a file name is RAW.
/**
*   @param  aFileName:  the file name to check
*   @return true if the extension of aFileName is RAW; false if it is not
*/
//------------------------------------------------------------------------------
bool isRAW(const char* aFileName);


//------------------------------------------------------------------------------
/// Check if the extension of a file name is MHA.
/**
*   @param  aFileName:  the file name to check
*   @return true if the extension of aFileName is MHA; false if it is not
*/
//------------------------------------------------------------------------------
bool isMHA(const char* aFileName);


//------------------------------------------------------------------------------
/// Check if the extension of a file name is MHD.
/**
*   @param  aFileName:  the file name to check
*   @return true if the extension of aFileName is MHD; false if it is not
*/
//------------------------------------------------------------------------------
bool isMHD(const char* aFileName);


//------------------------------------------------------------------------------
/// Check if the extension of a file name is DCM.
/**
*   @param  aFileName:  the file name to check
*   @return true if the extension of aFileName is DCM; false if it is not
*/
//------------------------------------------------------------------------------
bool isDCM(const char* aFileName);


//------------------------------------------------------------------------------
/// Check if the extension of a file name is JPEG.
/**
*   @param  aFileName:  the file name to check
*   @return true if the extension of aFileName is JPEG; false if it is not
*/
//------------------------------------------------------------------------------
bool isJPEG(const char* aFileName);


//------------------------------------------------------------------------------
/// Check if the extension of a file name is TIFF.
/**
*   @param  aFileName:  the file name to check
*   @return true if the extension of aFileName is TIFF; false if it is not
*/
//------------------------------------------------------------------------------
bool isTIFF(const char* aFileName);


//------------------------------------------------------------------------------
/// Check if the extension of a file name is MAT.
/**
*   @param  aFileName:  the file name to check
*   @return true if the extension of aFileName is MAT; false if it is not
*/
//------------------------------------------------------------------------------
bool isMAT(const std::string& aFileName);


//------------------------------------------------------------------------------
/// Check if the extension of a file name is DAT.
/**
*   @param  aFileName:  the file name to check
*   @return true if the extension of aFileName is DAT; false if it is not
*/
//------------------------------------------------------------------------------
bool isDAT(const std::string& aFileName);


//------------------------------------------------------------------------------
/// Check if the extension of a file name is TXT.
/**
*   @param  aFileName:  the file name to check
*   @return true if the extension of aFileName is TXT; false if it is not
*/
//------------------------------------------------------------------------------
bool isTXT(const std::string& aFileName);


//------------------------------------------------------------------------------
/// Check if the extension of a file name is PGM.
/**
*   @param  aFileName:  the file name to check
*   @return true if the extension of aFileName is PGM; false if it is not
*/
//------------------------------------------------------------------------------
bool isPGM(const std::string& aFileName);


//------------------------------------------------------------------------------
/// Check if the extension of a file name is RAW.
/**
*   @param  aFileName:  the file name to check
*   @return true if the extension of aFileName is RAW; false if it is not
*/
//------------------------------------------------------------------------------
bool isRAW(const std::string& aFileName);


//------------------------------------------------------------------------------
/// Check if the extension of a file name is MHA.
/**
*   @param  aFileName:  the file name to check
*   @return true if the extension of aFileName is MHA; false if it is not
*/
//------------------------------------------------------------------------------
bool isMHA(const std::string& aFileName);


//------------------------------------------------------------------------------
/// Check if the extension of a file name is MHD.
/**
*   @param  aFileName:  the file name to check
*   @return true if the extension of aFileName is MHD; false if it is not
*/
//------------------------------------------------------------------------------
bool isMHD(const std::string& aFileName);


//------------------------------------------------------------------------------
/// Check if the extension of a file name is DCM.
/**
*   @param  aFileName:  the file name to check
*   @return true if the extension of aFileName is DCM; false if it is not
*/
//------------------------------------------------------------------------------
bool isDCM(const std::string& aFileName);


//------------------------------------------------------------------------------
/// Check if the extension of a file name is JPEG.
/**
*   @param  aFileName:  the file name to check
*   @return true if the extension of aFileName is JPEG; false if it is not
*/
//------------------------------------------------------------------------------
bool isJPEG(const std::string& aFileName);


//------------------------------------------------------------------------------
/// Check if the extension of a file name is TIFF.
/**
*   @param  aFileName:  the file name to check
*   @return true if the extension of aFileName is TIFF; false if it is not
*/
//------------------------------------------------------------------------------
bool isTIFF(const std::string& aFileName);


bool checkExtension(const char* aFileName, const char* anExtension);


bool checkExtension(const std::string& aFileName,
                    const std::string& anExtension);


bool isBigEndian();


bool isLittleEndian();


bool isLetter(char aValue);


bool isUpperCase(char aValue);


bool isLowerCase(char aValue);


bool isNumber(char aValue);


template<typename T> void swapBytes(T& aValue);


template<typename T> T degreeToRadian(const T& aAngleInDegree);


template<typename T> T radianToDegree(const T& aAngleInRadian);


template <typename T> int sgn(const T& aValue);


double interpolate(const double& a_low,
    const double& a_high,
    const double& a0,
    const double& b_low,
    const double& b_high);


float interpolate(const float& a_low,
    const float& a_high,
    const float& a0,
    const float& b_low,
    const float& b_high);


} // namespace gVirtualXRay


//******************************************************************************
#include "Utilities.inl"


#endif // GVXR_UTILITIES_H
