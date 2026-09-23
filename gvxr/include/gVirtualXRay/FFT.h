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


#ifndef GVXR_FFT_h
#define GVXR_FFT_h


/**
********************************************************************************
*
*   @file       FFT.h
*
*   @brief      Class to compute the FFT of a greyscale image.
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
*   @class  FFT
*   @brief  FFT is a class to compute the FFT of a greyscale image.
*/
//==============================================================================
template<typename T> class FFT
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default constructor.
    //--------------------------------------------------------------------------
    FFT();


    //--------------------------------------------------------------------------
    /// Copy constructor.
    /**
     *  @param aFFT:    the FFT to copy
     */
    //--------------------------------------------------------------------------
    FFT(const FFT<T>& aFFT);


    //--------------------------------------------------------------------------
    /// Constructor to build a FFT.
    /**
     *  @param aWidth:  the width of the FFT
     *  @param aHeight: the height of the FFT
     */
    //--------------------------------------------------------------------------
    FFT(unsigned int aWidth,
        unsigned int aHeight);


    //--------------------------------------------------------------------------
    /// Assignment operator (also called copy operator).
    /**
     *  @param aFFT:    the FFT to copy
     *  @return the updated version of the current FFT
     */
    //--------------------------------------------------------------------------
    FFT<T>& operator=(const FFT<T>& aFFT);

    
    static FFT<T> computeFFT(const Image<T>& anImage);


    Image<T> getInverseFFT() const;


    FFT<T> filterRamp() const;

    
    FFT<T> rhoFilter(const std::string& aFilterType, float aScalingFactor);
    
    
    FFT<T> rhoFilter(const char* aFilterType, float aScalingFactor);

    
    Image<T> getMagnitude() const;
    
    
    //Image& getMagnitude();
    
    
    Image<T> getPhase() const;
   
    
    // Image& getPhase();


//******************************************************************************
private:
    static unsigned int powerOfTwo(unsigned int i);


    static Image<T> swapQuadrants(const Image<T>& anImage);

    Image<T> m_real;
    Image<T> m_img;

    unsigned int m_origin_image_width;
    unsigned int m_origin_image_height;
};


} // namespace gVirtualXRay


//******************************************************************************
#include "FFT.inl"


#endif // GVXR_FFT_h
