/*

Copyright (c) 2016-2023, Dr Franck P. Vidal, Bangor University, All rights reserved.
Copyright (c) 2023-present, Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), 
UK Research and Innovation, All rights reserved.


Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

3. Neither the name of Bangor University, UK Research and Innovation nor the 
names of its contributors may be used to endorse or promote products derived 
from this software without specific prior written permission.

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
*   @file       FFT.inl
*
*   @brief      Class to handle a greyscale image.
*
*   @date       17/04/2024
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
*   (c) by Dr Franck P. Vidal, Bangor University, Nov 2016-Sept 2023, version 1.1, BSD 3-Clause License
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, Oct 2023-present, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Define
//******************************************************************************


//******************************************************************************
//  Include
//******************************************************************************
#include <cmath>
#include <complex>

#ifdef HAS_FFTW
#include <fftw3.h>
#endif

#ifndef GVXR_EXCEPTION_H
#include "Exception.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//---------------------------------
template<typename T> FFT<T>::FFT():
//---------------------------------
    m_origin_image_width(0),
    m_origin_image_height(0)
//---------------------------------
{}


//---------------------------------------------------
template<typename T> FFT<T>::FFT(const FFT<T>& aFFT):
//---------------------------------------------------
    m_real(aFFT.m_real),
    m_img(aFFT.m_img),
    m_origin_image_width(aFFT.m_origin_image_width),
    m_origin_image_height(aFFT.m_origin_image_height)
//---------------------------------------------------
{}


//----------------------------------------------------------------
template<typename T> FFT<T>& FFT<T>::operator=(const FFT<T>& aFFT)
//----------------------------------------------------------------
{
    m_real                = aFFT.m_real;
    m_img                 = aFFT.m_img;
    m_origin_image_width  = aFFT.m_origin_image_width;
    m_origin_image_height = aFFT.m_origin_image_height;

    return (*this);
}


//---------------------------------------------------------------------
template<typename T> FFT<T> FFT<T>::computeFFT(const Image<T>& anImage)
//---------------------------------------------------------------------
{
#ifdef HAS_FFTW
	FFT fft;
    fft.m_origin_image_width  = anImage.getWidth();
    fft.m_origin_image_height = anImage.getHeight();

    unsigned int width_power_of_two(powerOfTwo(fft.m_origin_image_width));
    unsigned int height_power_of_two(powerOfTwo(fft.m_origin_image_height));

    if (width_power_of_two != 1)
    {
        width_power_of_two = (std::max(width_power_of_two, height_power_of_two));
    }

    if (height_power_of_two != 1)
    {
        height_power_of_two = (std::max(width_power_of_two, height_power_of_two));
    }

    fft.m_real = Image<T>(width_power_of_two, height_power_of_two, anImage.getDepth());
    fft.m_img  = Image<T>(width_power_of_two, height_power_of_two, anImage.getDepth());

    fftw_complex *in_src, *out_src;
    fftw_plan p_forw;

    in_src  = reinterpret_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex) * width_power_of_two * height_power_of_two));
    out_src = reinterpret_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex) * width_power_of_two * height_power_of_two));

    if (width_power_of_two == 1 ||  height_power_of_two == 1)
    {
        p_forw = fftw_plan_dft_1d(std::max(width_power_of_two, height_power_of_two), in_src, out_src, FFTW_FORWARD, FFTW_ESTIMATE);
    }
    else
    {
        p_forw = fftw_plan_dft_2d(width_power_of_two, height_power_of_two, in_src, out_src, FFTW_FORWARD, FFTW_ESTIMATE);
    }

    //float max_value(anImage.getMaxValue());
    bool y_forward_copy(false);
    int j2(0);

    for (unsigned int k(0); k < anImage.getDepth(); ++k)
    {
        for (int j1 = 0 ; j1 < height_power_of_two ; ++j1)
        {
            if (!(j1 % fft.m_origin_image_height))
            {
                y_forward_copy = !y_forward_copy;

                if (y_forward_copy)
                {
                    j2 = 0;
                }
                else
                {
                    j2 = fft.m_origin_image_height - 1;
                }
            }

            bool x_forward_copy(false);
            int i2(0);
            for (int i1 = 0 ; i1 < width_power_of_two ; ++i1)
            {
                if (!(i1 % fft.m_origin_image_width))
                {
                    x_forward_copy = !x_forward_copy;

                    if (x_forward_copy)
                    {
                        i2 = 0;
                    }
                    else
                    {
                        i2 = fft.m_origin_image_width - 1;
                    }
                }

                in_src[j1 * width_power_of_two + i1][0] = anImage(i2, j2, k);
                in_src[j1 * width_power_of_two + i1][1] = 0.0;

                if (x_forward_copy)
                {
                    ++i2;
                }
                else
                {
                    --i2;
                }
            }

            if (y_forward_copy)
            {
                ++j2;
            }
            else
            {
                --j2;
            }
        }

        // Compute the forward fft
        fftw_execute(p_forw);

        //double max_value(getMaxValue());
        double square_size(width_power_of_two * height_power_of_two);

        for (int j = 0 ; j < height_power_of_two ; ++j)
        {
            for (int i = 0 ; i < width_power_of_two ; ++i)
            {
                fft.m_real(i, j, k) = out_src[j * width_power_of_two + i][0] / square_size;
                fft.m_img( i, j, k) = out_src[j * width_power_of_two + i][1] / square_size;
            }
        }
    }

    fftw_destroy_plan(p_forw);
    fftw_free(in_src);
    fftw_free(out_src);

    return (fft);
#else // HAS_FFTW
throw Exception(__FILE__, __FUNCTION__, __LINE__, "FFTW not supported");
#endif // HAS_FFTW
}


//---------------------------------------------------------
template<typename T> Image<T> FFT<T>::getInverseFFT() const
//---------------------------------------------------------
{
#ifdef HAS_FFTW
	Image<T> temp(m_real.getWidth(), m_real.getHeight(), m_real.getDepth());

    // allocate input arrays
    fftw_complex *in_src, *out_src;
    fftw_plan p_back;

    in_src  = reinterpret_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex) * m_real.getWidth() * m_real.getHeight()));
    out_src = reinterpret_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex) * m_real.getWidth() * m_real.getHeight()));


    if (m_real.getWidth() == 1 ||  m_real.getHeight() == 1)
    {
        p_back = fftw_plan_dft_1d(std::max(m_real.getWidth(), m_real.getHeight()), in_src, out_src, FFTW_BACKWARD, FFTW_ESTIMATE);
    }
    else
    {
        p_back = fftw_plan_dft_2d(m_real.getWidth(), m_real.getHeight(), in_src, out_src, FFTW_BACKWARD, FFTW_ESTIMATE);
    }

    // transform magnitude/phase to real/imaginary
    for (unsigned int k(0); k < m_real.getDepth(); ++k)
    {
        for (unsigned int j(0); j < m_real.getHeight(); ++j)
        {
            for (unsigned int i(0); i < m_real.getWidth(); ++i)
            {
                in_src[j * m_real.getWidth() + i][0] = m_real(i, j, k);
                in_src[j * m_real.getWidth() + i][1] = m_img(i, j, k);
            }
        }

        // perform ift
        fftw_execute(p_back);

        // save real parts to output
        for (unsigned int j(0); j < m_real.getHeight(); ++j)
        {
            for (unsigned int i(0); i < m_real.getWidth(); ++i)
            {
                temp(i, j, k) = out_src[j * m_real.getWidth() + i][0];
            }
        }
    }


    // free memory
    fftw_destroy_plan(p_back);
    fftw_free(in_src); fftw_free(out_src);

    return (temp.getROI(0, 0, 0, m_origin_image_width, m_origin_image_height, m_real.getDepth()));
#else // HAS_FFTW
	throw Exception(__FILE__, __FUNCTION__, __LINE__, "FFTW not supported");
#endif // HAS_FFTW
}


//----------------------------------------------------
template<typename T> FFT<T> FFT<T>::filterRamp() const
//----------------------------------------------------
{
    FFT filtered_fft(*this);

    if (filtered_fft.m_real.getWidth() == 1 || filtered_fft.m_real.getHeight() == 1)
    {
        double half_size(std::max(filtered_fft.m_real.getWidth(), filtered_fft.m_real.getHeight()) / 2.0);

        for (unsigned int i(0); i < filtered_fft.m_real.getWidth() * filtered_fft.m_real.getHeight(); ++i)
        {
            double distance(std::abs(half_size - i));
            distance /= half_size;

            filtered_fft.m_real[i] *= 1.0 - distance;
            filtered_fft.m_img[ i] *= 1.0 - distance;
        }
    }
    else
    {
        double half_diagonal_size(std::sqrt(std::pow(filtered_fft.m_real.getWidth(), 2) + std::pow(filtered_fft.m_real.getHeight(), 2)) / 2.0);
        double half_size(filtered_fft.m_real.getWidth() / 2.0);

        for (unsigned int j(0); j < filtered_fft.m_real.getHeight(); ++j)
        {
            for (unsigned int i(0); i < filtered_fft.m_real.getWidth(); ++i)
            {
                double distance(std::sqrt(std::pow(half_size - i, 2) + std::pow(half_size - j, 2)));
                distance /= half_diagonal_size;

                filtered_fft.m_real(i, j) *= 1.0 - distance;
                filtered_fft.m_img(i, j) *= 1.0 - distance;
            }
        }
    }

    return (filtered_fft);
}


template<typename T> FFT<T> FFT<T>::rhoFilter(const std::string& aFilterType, float aScalingFactor)
{
    FFT<T> filtered_fft(*this);

    unsigned int size(filtered_fft.m_real.getWidth());
    unsigned int half_size(size / 2);

    std::vector<double> p_filter(half_size, 1.0);

    // Apply the ramp filter (Ram-Lal)

    if ((aFilterType != "none" && aFilterType != "None" && aFilterType != "NONE") ||
        aFilterType == "ramp" || aFilterType == "Ramp" || aFilterType == "RAMP")
    {
        for (unsigned int i(0); i < p_filter.size(); ++i)
        {
            double distance(std::abs(double(half_size - i)));
            distance /= half_size;
            distance = aScalingFactor * (1.0 - distance);

            p_filter[i] = distance;
        }
    }

    if (aFilterType == "hamming" || aFilterType == "Hamming" || aFilterType == "HAMMING")
    {
        int m(size);
        --m;

        for (unsigned int i(0); i < half_size; ++i)
        {
            //unsigned int index_i(i + half_size);

            float c1 = 1.0 - (0.54 - 0.46 * std::cos (2.0 * Pi * float(i) / float(m)));
            //float c2 = 0.54 - 0.46 * std::cos (2.0 * Pi * float(index_i) / float(m));

            p_filter[i] *= c1;
            //p_filter[index_i] *= c1;
        }
    }
    else if (aFilterType == "hann" || aFilterType == "Hann" || aFilterType == "HANN" || aFilterType == "hanning" || aFilterType == "Hanning" || aFilterType == "HANNING")
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "rho_filter: Unknown window type");
    }
    else if (aFilterType == "cosine" || aFilterType == "Cosine" || aFilterType == "COSINE")
    {
        //f = 0.5 * (0:length (rho) - 1)' / length (rho);
        //filt = fftshift (sin (2 * pi * f));
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "rho_filter: Unknown window type");
    }
    else if (aFilterType == "shepp-logan" || aFilterType == "Shepp-logan" || aFilterType == "Shepp-Logan" || aFilterType == "SHEPP-LOGAN")
    {
        //f = (0:length (rho) / 2)' / length (rho);
        //filt = sin (pi * f) ./ (pi * f);
        //filt (1) = 1;
        //filt = [filt; filt(end - 1:-1:2)];
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "rho_filter: Unknown window type");
    }
    else if (!(aFilterType == "ramp" || aFilterType == "Ramp" || aFilterType == "RAMP"))
    {
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "rho_filter: Unknown window type");
    }
	
    for (unsigned int k(0); k < filtered_fft.m_real.getDepth(); ++k)
    {
        for (unsigned int j(0); j < filtered_fft.m_real.getHeight(); ++j)
        {
            for (unsigned int i(0); i < p_filter.size(); ++i)
            {
                filtered_fft.m_real(i, j, k) *= p_filter[i];
                filtered_fft.m_img( i, j, k) *= p_filter[i];

                filtered_fft.m_real(size - 1 - i, j, k) *= p_filter[i];
                filtered_fft.m_img( size - 1 - i, j, k) *= p_filter[i];
            }
        }
    }

    return (filtered_fft);
}


template<typename T> FFT<T> FFT<T>::rhoFilter(const char* aFilterType, float aScalingFactor)
{
    return (rhoFilter(std::string(aFilterType), aScalingFactor));
}


//-----------------------------
template<typename T> Image<T> FFT<T>::getMagnitude() const
//-----------------------------
{
    Image<T> temp(m_real.getWidth(), m_real.getHeight());

    double square_size(m_real.getWidth() * m_real.getHeight());

    for(int j  = 0 ; j < m_real.getHeight() ; ++j)
    {
        for(int i = 0 ; i < m_real.getWidth() ; ++i)
        {
            unsigned int index(j * m_real.getWidth() + i);
            double real(m_real[index] / square_size);
            double img(m_img[index] / square_size);

            temp[index] = std::sqrt(real * real + img * img);
        }
    }

    return (swapQuadrants(temp));
}



//-------------------------
template<typename T> Image<T> FFT<T>::getPhase() const
//-------------------------
{
    Image<T> temp(m_real.getWidth(), m_real.getHeight());

    double square_size(m_real.getWidth() * m_real.getHeight());

    for(int j  = 0 ; j < m_real.getHeight() ; ++j)
    {
        for(int i = 0 ; i < m_real.getWidth() ; ++i)
        {
            unsigned int index(j * m_real.getWidth() + i);
            double real(m_real[index] / square_size);
            double img(m_img[index] / square_size);

            // phase
            std::complex<double> complex_value(real, img);
            double phase_value = arg(complex_value) + Pi;

            temp[index] = (phase_value / (2.0 * Pi));
        }
    }

    return (swapQuadrants(temp));
}


template<typename T> unsigned int FFT<T>::powerOfTwo(unsigned int i)
{
    unsigned int j(1);

    while (j < i) j *= 2;

    return (j);
}


template<typename T> Image<T> FFT<T>::swapQuadrants(const Image<T>& anImage)
{
    Image<T> temp_image(anImage);

    // It is correct as the width is a power of two (even number)
    int half_width(temp_image.getWidth() / 2);

    // It is correct as the height is a power of two (even number)
    int half_height(temp_image.getHeight() / 2);

    if (temp_image.getWidth() == 1)
    {
        for(unsigned int i = 0; i < half_width; ++i)
        {
            // It is correct as the width is a power of two (even number)
            unsigned int index_i(i + half_width);

            float temp(temp_image[i]);
            temp_image[i] = temp_image[index_i];
            temp_image[index_i] = temp;
        }
    }
    else if (temp_image.getHeight() == 1)
    {
        for(unsigned int j = 0; j < half_height; ++j)
        {
            // It is correct as the height is a power of two (even number)
            unsigned int index_j(j + half_height);

            float temp(temp_image[j * temp_image.getWidth()]);
            temp_image[j * temp_image.getWidth()] = temp_image[index_j * temp_image.getWidth()];
            temp_image[index_j * temp_image.getWidth()] = temp;
            }
    }
    else
    {
        // swap quadrants diagonally
        for(unsigned int j = 0; j < half_height; ++j)
        {
            // It is correct as the height is a power of two (even number)
            unsigned int index_j(j + half_height);

            for(unsigned int i = 0; i < half_width; ++i)
            {
                // It is correct as the width is a power of two (even number)
                unsigned int index_i(i + half_width);

                float temp(temp_image[j * temp_image.getWidth() + i]);
                temp_image[j * temp_image.getWidth() + i] = temp_image[index_j * temp_image.getWidth() + index_i];
                temp_image[index_j * temp_image.getWidth() + index_i] = temp;

                temp = temp_image[index_j * temp_image.getWidth() + i];
                temp_image[index_j * temp_image.getWidth() + i] = temp_image[j * temp_image.getWidth() + index_i];
                temp_image[j * temp_image.getWidth() + index_i] = temp;
            }
        }
    }

    return (temp_image);
}


} // namespace gVirtualXRay
