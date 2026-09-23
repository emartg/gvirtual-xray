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


#ifndef GVXR_SINOGRAM_H
#define GVXR_SINOGRAM_H


/**
********************************************************************************
*
*   @file       Sinogram.h
*
*   @brief      Class to reconstruct images from a sinogram.
*
*   @version    1.0
*
*   @date       13/01/2017
*
*   @author     Franck Vidal
*
*
********************************************************************************
*/

//******************************************************************************
//  Include
//******************************************************************************
#include <string>

#ifndef GVXR_IMAGE_H
#include "gVirtualXRay/Image.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*   @class  Sinogram
*   @brief  Sinogram is a class to reconstruct images from a sinogram.
*/
//==============================================================================
template<typename T> class Sinogram: public Image<T>
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    Sinogram();
    
    
    Sinogram(const Image<T> anImage);
    
    
    Sinogram(unsigned int aWidth,
             unsigned int aHeight,
             unsigned int aNumberOfSlices = 1);
    
    Image<T> getProjectionSet() const;

    
    Image<T> art(double aFirstAngle,
                 double anAngleStep,
                 unsigned int aNumberOfIterations = 0,
                 const char* aDirectory = 0,
                 void (*aCallback)(const Image<T>*, const Image<T>*, const Image<T>*) = 0) const;
    
    
    Image<T> mart(double aFirstAngle,
                  double anAngleStep,
                  unsigned int aNumberOfIterations = 0,
                  const char* aDirectory = 0,
                  void (*aCallback)(const Image<T>*, const Image<T>*, const Image<T>*) = 0) const;
    
    
    Image<T> sirt(double aFirstAngle,
                  double anAngleStep,
                  unsigned int aNumberOfIterations = 0,
                  const char* aDirectory = 0,
                  void (*aCallback)(const Image<T>*, const Image<T>*, const Image<T>*) = 0) const;
    
    
    Image<T> simpleBackProjection(double aFirstAngle,
                                  double anAngleStep,
                                  const char* aDirectorye = 0,
                                  void (*aCallback)(const Image<T>*, const Image<T>*, const Image<T>*) = 0) const;
    
    
    Image<T> filteredBackProjection(double aFirstAngle,
                                    double anAngleStep,
                                    const char* aFilterType = "hamming",
                                    float aScalingFactor = 1.0,
                                    const char* aDirectory = 0,
                                    void (*aCallback)(const Image<T>*, const Image<T>*, const Image<T>*) = 0) const;
    
    
    Image<T> art(double aFirstAngle,
                 double anAngleStep,
                 unsigned int aNumberOfIterations,
                 const std::string& aDirectory,
                 void (*aCallback)(const Image<T>*, const Image<T>*, const Image<T>*)) const;
    
    
    Image<T> mart(double aFirstAngle,
                  double anAngleStep,
                  unsigned int aNumberOfIterations,
                  const std::string& aDirectory,
                  void (*aCallback)(const Image<T>*, const Image<T>*, const Image<T>*)) const;
    
    
    Image<T> sirt(double aFirstAngle,
                  double anAngleStep,
                  unsigned int aNumberOfIterations,
                  const std::string& aDirectory,
                  void (*aCallback)(const Image<T>*, const Image<T>*, const Image<T>*)) const;
    
    
    Image<T> simpleBackProjection(double aFirstAngle,
                                  double anAngleStep,
                                  const std::string& aDirectory,
                                  void (*aCallback)(const Image<T>*, const Image<T>*, const Image<T>*)) const;
    
    
    Image<T> filteredBackProjection(double aFirstAngle,
                                    double anAngleStep,
                                    const std::string& aFilterType,
                                    float aScalingFactor,
                                    const std::string& aDirectory,
                                    void (*aCallback)(const Image<T>*, const Image<T>*, const Image<T>*)) const;
};


} // namespace gVirtualXRay


//******************************************************************************
#include "Sinogram.inl"


#endif // GVXR_SINOGRAM_H
