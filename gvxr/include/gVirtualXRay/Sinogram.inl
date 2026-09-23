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
*   @file       Sinogram.h
*
*   @brief      Class to reconstruct images from a sinogram.
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
*   (c) by Dr Franck P. Vidal, Bangor University, Sept 2016-Sept 2023, version 1.1, BSD 3-Clause License
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
#ifndef GVXR_FFT_h
#include "gVirtualXRay/FFT.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


template<typename T> Sinogram<T>::Sinogram():
        Image<T>()
{}


template<typename T> Sinogram<T>::Sinogram(const Image<T> anImage):
        Image<T>(anImage)
{}


template<typename T> Sinogram<T>::Sinogram(unsigned int aWidth,
            unsigned int aHeight,
            unsigned int aNumberOfSlices):
        Image<T>(aWidth, aHeight, aNumberOfSlices)
{}


template<typename T> Image<T> Sinogram<T>::getProjectionSet() const
{
    Image<T> projection_set(this->m_width, this->m_number_of_slices, this->m_height);

#ifdef NDEBUG
#ifdef WIN32
        #pragma omp parallel for
#else
        #pragma omp parallel for collapse(2)
#endif
#endif
    for (int angle_id = 0; angle_id < this->m_height; ++angle_id)
    {
        for (int row_projection_id = 0; row_projection_id < this->m_number_of_slices; ++row_projection_id)
        {
            const T* p_input(this->getRawData() + angle_id * this->m_width + row_projection_id * this->m_width * this->m_height);
            T* p_output(projection_set.getRawData() + angle_id * this->m_width * this->m_number_of_slices + this->m_width * row_projection_id);

            std::copy(p_input, p_input + this->m_width, p_output);
        }
    }
    return (projection_set);
}


//------------------------------------------------------------------------------
template<typename T> Image<T> Sinogram<T>::art(double aFirstAngle,
                                               double anAngleStep,
                                               unsigned int aNumberOfIterations,
                                               const char* aDirectory,
                                               void (*aCallback)(const Image<T>*, const Image<T>*, const Image<T>*)) const
//------------------------------------------------------------------------------
{
    unsigned int image_width(2 * floor (this->m_width / (2.0 * std::sqrt(2.0))));

    // Start with a homogenous image
    Image<T> new_estimated_image(image_width, image_width, this->m_number_of_slices, this->getSum() / (image_width * image_width));
    Image<T> old_estimated_image(image_width, image_width, this->m_number_of_slices, this->getSum() / (image_width * image_width));

    Image<T> error_back_projection;
    
    double old_ncc(0.0);

    unsigned int iteration(1);
    bool stop(false);
    while (!stop)
    {
        old_estimated_image = new_estimated_image;
    
        // Process every angle
        double angle(aFirstAngle);
        for (unsigned int angle_id(0); angle_id < this->m_height; ++angle_id, angle += anAngleStep)
        {
            // Compute the projections
            Image<T> estimated_projection(new_estimated_image.radonTransform(angle, 0.0, angle).setCanvasSize(this->m_width, 1, this->m_number_of_slices));
            //double sum_estimated(estimated_projection.getSum());

            // Real image
            Image<T> real_projection(this->getROI(0, angle_id, 0, this->m_width, 1, this->m_number_of_slices));

            // Compute the error
            Sinogram<T> error_in_projections = (real_projection - estimated_projection);
        
            // Correct for errors
            error_back_projection = error_in_projections.simpleBackProjection(angle, angle);
        
            new_estimated_image += error_back_projection.setCanvasSize(image_width, image_width, this->m_number_of_slices);

            T* p_temp(new_estimated_image.getRawData());
            for (unsigned int i(0); i < new_estimated_image.getWidth() * new_estimated_image.getHeight() * new_estimated_image.getDepth(); ++i, ++p_temp)
            {
                *p_temp = std::max(0.0f, *p_temp);
            }

            if (aDirectory)
            {
                std::stringstream filename;
                filename << aDirectory << "/iteration_" << iteration << "_" << angle_id << ".mha";
                (new_estimated_image.setCanvasSize(image_width, image_width, this->m_number_of_slices).normalised() * 255).savePGM(filename.str());
            }
            
            if (aCallback)
            {
                (*aCallback)(&new_estimated_image, &estimated_projection, &error_back_projection);
            }
        }

        // Compute the projection
        Image<T> estimated_projections(new_estimated_image.radonTransform(aFirstAngle, anAngleStep, this->m_height).setCanvasSize(this->m_width, this->m_height, this->m_number_of_slices));

        // Stopping criteria
        double ncc_projections(100.0 * this->computeNCC(estimated_projections));
        double ncc_reconstructions(100.0 * new_estimated_image.computeNCC(old_estimated_image));

        if ((ncc_projections  > 99.50 || ncc_reconstructions > 99.50) /*&& std::abs(old_ncc - ncc_reconstructions) < EPSILON*/)
        {
            stop = true;
        }
        else if (aNumberOfIterations == iteration)
        {
            stop = true;
        }

        old_ncc = ncc_reconstructions;

        iteration++;

        
        if (aCallback)
        {
            (*aCallback)(&new_estimated_image, &estimated_projections, &error_back_projection);
        }
    }
    
    return (new_estimated_image.setCanvasSize(image_width, image_width, this->m_number_of_slices));
}


//-------------------------------------------------------------------------------
template<typename T> Image<T> Sinogram<T>::mart(double aFirstAngle,
                                                double anAngleStep,
                                                unsigned int aNumberOfIterations,
                                                const char* aDirectory,
                                                void (*aCallback)(const Image<T>*, const Image<T>*, const Image<T>*)) const
//-------------------------------------------------------------------------------
{
    unsigned int image_width(2 * floor (this->m_width / (2.0 * std::sqrt(2.0))));

    // Start with a homogenous image
    Image<T> new_estimated_image(image_width, image_width, this->m_number_of_slices, this->getSum() / (image_width * image_width));
    Image<T> old_estimated_image(image_width, image_width, this->m_number_of_slices, this->getSum() / (image_width * image_width));

    Image<T> error_back_projection;
    
    double old_ncc(0.0);

    unsigned int iteration(1);
    bool stop(false);
    while (!stop)
    {
        old_estimated_image = new_estimated_image;
    
        // Process every angle
        double angle(aFirstAngle);
        for (unsigned int angle_id(0); angle_id < this->m_height; ++angle_id, angle += anAngleStep)
        {
            // Compute the projections
            Image<T> estimated_projection(new_estimated_image.radonTransform(angle, 0.0, angle).setCanvasSize(this->m_width, 1, this->m_number_of_slices));
            //double sum_estimated(estimated_projection.getSum());

            // Real image
            Image<T> real_projection(this->getROI(0, angle_id, 0, this->m_width, 1, this->m_number_of_slices));

            // Compute the error
            Sinogram<T> error_in_projections(real_projection - estimated_projection);
        
            // Correct for errors
            error_back_projection = Image<T>(error_in_projections.simpleBackProjection(angle, angle));
        
            new_estimated_image *= error_back_projection.setCanvasSize(image_width, image_width, this->m_number_of_slices);

            T* p_temp(new_estimated_image.getRawData());
            for (unsigned int i(0); i < new_estimated_image.getWidth() * new_estimated_image.getHeight() * new_estimated_image.getDepth(); ++i, ++p_temp)
            {
                *p_temp = std::max(0.0f, *p_temp);
            }

            if (aDirectory)
            {
                std::stringstream filename;
                filename << aDirectory << "/iteration_" << iteration << "_" << angle_id << ".mha";
                (new_estimated_image.setCanvasSize(image_width, image_width, this->m_number_of_slices).normalised() * 255).savePGM(filename.str());
            }
            
            if (aCallback)
            {
                (*aCallback)(&new_estimated_image, &estimated_projection, &error_back_projection);
            }
        }

        // Compute the projection
        Image<T> estimated_projections(new_estimated_image.radonTransform(aFirstAngle, anAngleStep, this->m_height).setCanvasSize(this->m_width, this->m_height, this->m_number_of_slices));

        // Stopping criteria
        double ncc_projections(100.0 * this->computeNCC(estimated_projections));
        double ncc_reconstructions(100.0 * new_estimated_image.computeNCC(old_estimated_image));

        if ((ncc_projections  > 99.50 || ncc_reconstructions > 99.50) /*&& std::abs(old_ncc - ncc_reconstructions) < EPSILON*/)
        {
            stop = true;
        }
        else if (aNumberOfIterations == iteration)
        {
            stop = true;
        }

        old_ncc = ncc_reconstructions;

        iteration++;

        
        if (aCallback)
        {
            (*aCallback)(&new_estimated_image, &estimated_projections, &error_back_projection);
        }
    }
    
    return (new_estimated_image.setCanvasSize(image_width, image_width, this->m_number_of_slices));
}


//-------------------------------------------------------------------------------
template<typename T> Image<T> Sinogram<T>::sirt(double aFirstAngle,
                                                double anAngleStep,
                                                unsigned int aNumberOfIterations,
                                                const char* aDirectory,
                                                void (*aCallback)(const Image<T>*, const Image<T>*, const Image<T>*)) const
//-------------------------------------------------------------------------------
{
    unsigned int image_width(2 * floor (this->m_width / (2.0 * std::sqrt(2.0))));

    // Start with a homogenous image
    Image<T> new_estimated_image(image_width, image_width, this->m_number_of_slices, this->getSum() / (image_width * image_width));
    Image<T> old_estimated_image(image_width, image_width, this->m_number_of_slices, this->getSum() / (image_width * image_width));

    //std::cout << this->getSum() << "\t" << new_estimated_image.getSum() << std::endl;
    double old_ncc(0.0);

    unsigned int iteration(1);
    bool stop(false);
    while (!stop)
    {
        old_estimated_image = new_estimated_image;

        Image<T> estimated_projection(new_estimated_image.radonTransform(aFirstAngle, anAngleStep, this->m_height));

        // Compute the error
        Sinogram<T> error_in_projections(*this - estimated_projection);

        this->saveASCII("real_projection.mat");
        estimated_projection.saveASCII("estimated_projection.mat");
        error_in_projections.saveASCII("error_in_projections.mat");

        Image<T> error_back_projection(error_in_projections.simpleBackProjection(aFirstAngle, anAngleStep));
        error_back_projection.saveASCII("error_back_projection.mat");

        // Correct for errors
        new_estimated_image += error_back_projection.setCanvasSize(image_width, image_width, this->m_number_of_slices);
        new_estimated_image.saveASCII("new_estimated_image.mat");
        old_estimated_image.saveASCII("old_estimated_image.mat");

        T* p_temp(new_estimated_image.getRawData());
        for (unsigned int i(0); i < new_estimated_image.getWidth() * new_estimated_image.getHeight() * new_estimated_image.getDepth(); ++i, ++p_temp)
        {
            *p_temp = std::max(0.0f, *p_temp);
        }

        if (aDirectory)
        {
            std::stringstream filename;
            filename << aDirectory << "/iteration_" << iteration << ".mha";
            (new_estimated_image.setCanvasSize(image_width, image_width, this->m_number_of_slices).normalised() * 255).savePGM(filename.str());
        }

        
        if (aCallback)
        {
            (*aCallback)(&new_estimated_image, &estimated_projection, &error_back_projection);
        }

        // Compute the projection
        Image<T> estimated_projections(new_estimated_image.radonTransform(aFirstAngle, anAngleStep, this->m_height).setCanvasSize(this->m_width, this->m_height, this->m_number_of_slices));
        estimated_projections.saveASCII("simulated_projection.mat");

        // Stopping criteria
        double ncc_projections(100.0 * this->computeNCC(estimated_projections));
        double ncc_reconstructions(100.0 * new_estimated_image.computeNCC(old_estimated_image));

        if ((ncc_projections  > 99.50 || ncc_reconstructions > 99.50) /*&& std::abs(old_ncc - ncc_reconstructions) < EPSILON*/)
        {
            stop = true;
        }
        else if (aNumberOfIterations == iteration)
        {
            stop = true;
        }

        old_ncc = ncc_reconstructions;

        iteration++;

    }

    return (new_estimated_image.setCanvasSize(image_width, image_width, this->m_number_of_slices));
}


//-------------------------------------------------------------------------------------------
template<typename T> Image<T> Sinogram<T>::simpleBackProjection(double aFirstAngle,
                                                                double anAngleStep,
                                                                const char* aDirectory,
                                                                void (*aCallback)(const Image<T>*, const Image<T>*, const Image<T>*)) const
//-------------------------------------------------------------------------------------------
{
    Image<T> sinogram(this->resize(this->m_width * 2, this->m_height, this->m_number_of_slices));
    sinogram.saveASCII("resized.mat");
    unsigned int image_width(2 * floor (sinogram.getWidth() / (2.0 * std::sqrt(2.0))));

    Image<T> reconstruction(sinogram.getWidth(), sinogram.getWidth(), sinogram.getDepth());

    double angle(aFirstAngle);
    for (unsigned int angle_id(0);
            angle_id < this->m_height;
            ++angle_id, angle += anAngleStep)
    {
        Image<T> back_projection(sinogram.getWidth(), sinogram.getWidth(), sinogram.getDepth());

#ifdef NDEBUG
#ifdef WIN32
        #pragma omp parallel for
#else
        #pragma omp parallel for collapse(3)
#endif
#endif
        for (int k = 0; k < reconstruction.getDepth(); ++k)
        {
            for (int j = 0; j < reconstruction.getHeight(); ++j)
            {
                for (int i = 0; i < reconstruction.getWidth(); ++i)
                {
                    back_projection(i, j, k) += sinogram.getPixel(i, angle_id, k);
                }
            }
        }

        reconstruction += back_projection.rotate(-angle) / reconstruction.getHeight();

        if (aDirectory)
        {
            std::stringstream filename;
            filename << aDirectory << "/angle_" << angle_id << ".mha";
            (reconstruction.setCanvasSize(image_width, image_width, reconstruction.getDepth()).resize(image_width / 2, image_width / 2, reconstruction.getDepth()).normalised() * 255).savePGM(filename.str());
        }

        
        if (aCallback)
        {
            (*aCallback)(&reconstruction, &back_projection, this);
        }
    }

    //reconstruction = reconstruction.rotate(-angle);

    reconstruction = reconstruction.setCanvasSize(image_width, image_width, this->m_number_of_slices);
    reconstruction *= Pi / (/*2.0 * */ this->getHeight());

    return (reconstruction.resize(image_width / 2, image_width / 2, this->getDepth()));
}


//---------------------------------------------------------------------------------------------
template<typename T> Image<T> Sinogram<T>::filteredBackProjection(double aFirstAngle,
                                                                  double anAngleStep,
                                                                  const char* aFilterType,
                                                                  float aScalingFactor,
                                                                  const char* aDirectory,
                                                                  void (*aCallback)(const Image<T>*, const Image<T>*, const Image<T>*)) const
//---------------------------------------------------------------------------------------------
{
    typedef  FFT<T>   FAST_FOURIER_TRANSFORM;

    // Compute the FFTs
    std::vector<FAST_FOURIER_TRANSFORM> p_fft_set;
    for (unsigned int j(0); j < this->m_height; ++j)
    {
        p_fft_set.push_back(this->getROI(0, j, 0, this->m_width, 1, this->m_number_of_slices).getFFT());
    }

    // Compute the rho filter
    for (typename std::vector<FAST_FOURIER_TRANSFORM>::iterator ite(p_fft_set.begin());
            ite != p_fft_set.end();
            ++ite)
    {
        *ite = ite->rhoFilter(aFilterType, aScalingFactor);
    }

    // Compute the iFFTs
    std::vector<Image<T> > p_ifft_set(this->m_height);
    for (unsigned int j(0); j < this->m_height; ++j)
    {
        p_ifft_set[j] = p_fft_set[j].getInverseFFT();
    }

    // Store the results
    Sinogram<T> filtered_sinogram(this->m_width, this->m_height, this->m_number_of_slices);

    for (unsigned int k(0); k < this->m_number_of_slices; ++k)
    {
        for (unsigned int j(0); j < this->m_height; ++j)
        {
            for (unsigned int i(0); i < this->m_width; ++i)
            {
                filtered_sinogram(i, j, k) = p_ifft_set[j](i, 0, k);
            }
        }
    }

    //std::cout << getAverage() << "  " << filtered_sinogram.getAverage() << std::endl;
    //filtered_sinogram *= filtered_sinogram.getAverage() / getAverage();
    //filtered_sinogram.saveASCII("filtered_sinogram.mat");
    return (filtered_sinogram.simpleBackProjection(aFirstAngle,
                                                   anAngleStep,
                                                   aDirectory,
                                                   aCallback));
}


//---------------------------------------------------------------------------------
template<typename T> Image<T> Sinogram<T>::art(double aFirstAngle,
                                               double anAngleStep, 
                                               unsigned int aNumberOfIterations, 
                                               const std::string& aDirectory,
                                               void (*aCallback)(const Image<T>*, const Image<T>*, const Image<T>*)) const
//---------------------------------------------------------------------------------
{
    return (art(aFirstAngle,
                anAngleStep,
                aNumberOfIterations,
                aDirectory.data(),
                aCallback));
}


//----------------------------------------------------------------------------------
template<typename T> Image<T> Sinogram<T>::mart(double aFirstAngle,
                                                double anAngleStep,
                                                unsigned int aNumberOfIterations,
                                                const std::string& aDirectory,
                                                void (*aCallback)(const Image<T>*, const Image<T>*, const Image<T>*)) const
//----------------------------------------------------------------------------------
{
    return (mart(aFirstAngle,
                 anAngleStep,
                 aNumberOfIterations,
                 aDirectory.data(),
                 aCallback));
}


//----------------------------------------------------------------------------------
template<typename T> Image<T> Sinogram<T>::sirt(double aFirstAngle,
                                                double anAngleStep,
                                                unsigned int aNumberOfIterations,
                                                const std::string& aDirectory,
                                                void (*aCallback)(const Image<T>*, const Image<T>*, const Image<T>*)) const
//----------------------------------------------------------------------------------
{
    return (sirt(aFirstAngle,
                 anAngleStep,
                 aNumberOfIterations,
                 aDirectory.data(),
                 aCallback));
}


//--------------------------------------------------------------------------------------------------
template<typename T> Image<T> Sinogram<T>::simpleBackProjection(double aFirstAngle,
                                                                double anAngleStep,
                                                                const std::string& aDirectory,
                                                                void (*aCallback)(const Image<T>*, const Image<T>*, const Image<T>*)) const
//--------------------------------------------------------------------------------------------------
{
    return (simpleBackProjection(aFirstAngle,
                                 anAngleStep,
                                 aDirectory.data(),
                                 aCallback));
}


//----------------------------------------------------------------------------------------------------
template<typename T> Image<T> Sinogram<T>::filteredBackProjection(double aFirstAngle,
                                                                  double anAngleStep,
                                                                  const std::string& aFilterType,
                                                                  float aScalingFactor,
                                                                  const std::string& aDirectory,
                                                                  void (*aCallback)(const Image<T>*, const Image<T>*, const Image<T>*)) const
//----------------------------------------------------------------------------------------------------
{
    return (filteredBackProjection(aFirstAngle,
                                   anAngleStep,
                                   aFilterType.data(),
                                   aScalingFactor,
                                   aDirectory.data(),
                                   aCallback));
}


} // namespace gVirtualXRay
