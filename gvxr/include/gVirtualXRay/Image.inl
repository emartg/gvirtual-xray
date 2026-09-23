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
*   @file       Image.inl
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
#define LINE_SIZE 4096


//******************************************************************************
//  Include
//******************************************************************************
#include <sstream> // Header file for stringstream
#include <fstream> // Header file for filestream
#include <iomanip>
#include <algorithm> // Header file for min/max/fill
#include <cmath> // Header file for abs
#include <numeric> // Header file for accumulate
#include <vector>
#include <typeinfo>

#include <zlib.h>

#ifdef HAS_ITK
#include <itkImage.h>
#include <itkImageSeriesReader.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkNumericSeriesFileNames.h>
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkImportImageFilter.h"
#include "itkDiscreteGaussianImageFilter.h"
#include "itkAntiAliasBinaryImageFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include <itkBinaryMorphologicalOpeningImageFilter.h>
#include <itkBinaryMorphologicalClosingImageFilter.h>
#include <itkRecursiveGaussianImageFilter.h>
#include <itkResampleImageFilter.h>
#include <itkIdentityTransform.h>
#include <itkBSplineInterpolateImageFunction.h>
#include "itkMesh.h"
#include <itkRescaleIntensityImageFilter.h>
//#include "itkBinaryThresholdImageFilter.h"
#include "itkBinaryMask3DMeshSource.h"
//#include "itkMeshFileWriter.h"
#endif

#ifdef HAS_GDCM
#include <gdcmGlobal.h>

#include "gdcmImageReader.h"
#include "gdcmSequenceOfFragments.h"
#include "gdcmSystem.h"
#include "gdcmImageWriter.h"
#include "gdcmPixmapWriter.h"
#include <gdcmRescaler.h>

#include <gdcmImage.h>
#include <gdcmBitmap.h>
#include <gdcmImageWriter.h>
#include <gdcmPixelFormat.h>
#include <gdcmPhotometricInterpretation.h>
#include <gdcmImageChangeTransferSyntax.h>
#endif

#ifdef HAS_OPENMP
#include <omp.h>
#endif

#ifdef HAS_TIFF
#include <tiffio.h>
#endif

#ifndef GVXR_CONSTANT_VALUES_H
#include "gVirtualXRay/ConstantValues.h"
#endif

#ifndef GVXR_UTILITIES_H
#include "gVirtualXRay/Utilities.h"
#endif

#ifndef GVXR_EXCEPTION_H
#include "gVirtualXRay/Exception.h"
#endif

#ifndef GVXR_OUT_OF_BOUNDS_EXCEPTION_H
#include "gVirtualXRay/OutOfBoundsException.h"
#endif

#ifndef GVXR_OUT_OF_MEMORY_EXCEPTION_H
#include "gVirtualXRay/OutOfMemoryException.h"
#endif

#ifndef GVXR_FILE_DOES_NOT_EXIST_EXCEPTION_H
#include "FileDoesNotExistException.h"
#endif

#ifndef GVXR_POLYGON_MESH_H
#include "gVirtualXRay/PolygonMesh.h"
#endif

#ifndef GVXR_LOGGER_H
#include "gVirtualXRay/Logger.h"
#endif

#ifndef GVXR_SHADER_H
#include "gVirtualXRay/Shader.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//---------------------------------------------------------------------------
template<typename T> Image<T> gauss2D(unsigned int aSize, double aSigmaValue)
//---------------------------------------------------------------------------
{
	// Error check
	if (aSize <= 0) {
		throw Exception(__FILE__, __FUNCTION__, __LINE__,
			"Invalid kernel size. It must be greater than 0.");
	}

	Image<T> kernel(aSize, aSize, 1, 0.0);

	double sum = 0.0;

	double two_sigma_square = 2.0 * aSigmaValue * aSigmaValue;
	int x0 = static_cast<int>(aSize) / 2;
	int y0 = static_cast<int>(aSize) / 2;

	for (size_t i = 0; i < aSize; i++)
	{
		for (size_t j = 0; j < aSize; j++)
		{
			kernel(i, j, 0) = exp(-(pow(static_cast<int>(i) - x0, 2) + pow(static_cast<int>(j) - y0, 2)) /
				two_sigma_square);
			sum += kernel(i, j, 0);
		}
	}

	for (size_t i = 0; i < aSize; i++)
	{
		for (size_t j = 0; j < aSize; j++)
		{
			kernel(i, j, 0) /= sum;
		}
	}

	return kernel;
}


//-------------------------------------
template<typename T> Image<T>::Image():
	//-------------------------------------
	m_width(0),
	m_height(0),
	m_number_of_slices(0),
	m_spacing(1, 1, 1),
	m_p_image(nullptr),
	m_texture_id(0)
//-------------------------------------
{}


//------------------------------------------------------------
template<typename T> Image<T>::Image(const Image<T>& anImage):
//------------------------------------------------------------
	m_width(anImage.m_width),
	m_height(anImage.m_height),
	m_number_of_slices(anImage.m_number_of_slices),
	m_spacing(anImage.m_spacing),
	m_p_image(new T[m_width * m_height * m_number_of_slices]),
	m_texture_id(0)
//------------------------------------------------------------
{
	// Out of memory
	if (m_width && m_height && m_number_of_slices && !m_p_image)
	{
		throw OutOfMemoryException(__FILE__, __FUNCTION__, __LINE__);
	}

	// Copy the data
	std::copy(anImage.m_p_image, anImage.m_p_image + m_width * m_height * m_number_of_slices, m_p_image);
}


//----------------------------------------------------------
template<typename T> Image<T>::Image(const char* aFileName):
//----------------------------------------------------------
	m_width(0),
	m_height(0),
	m_number_of_slices(0),
	m_spacing(1, 1, 1),
	m_p_image(nullptr),
	m_texture_id(0)
//----------------------------------------------------------
{
	load(aFileName);
}


//-----------------------------------------------------------------
template<typename T> Image<T>::Image(const std::string& aFileName):
	//-----------------------------------------------------------------
	m_width(0),
	m_height(0),
	m_number_of_slices(0),
	m_spacing(1, 1, 1),
	m_p_image(nullptr),
	m_texture_id(0)
//-----------------------------------------------------------------
{
	load(aFileName);
}


//----------------------------------------------------------------
template<typename T> Image<T>::Image(const T* apData,
                                     unsigned int aWidth,
                                     unsigned int aHeight,
                                     unsigned int aNumberOfSlices,
                                     const VEC3& aVoxelSize):
//----------------------------------------------------------------
	m_width(aWidth),
	m_height(aHeight),
	m_number_of_slices(aNumberOfSlices),
	m_spacing(aVoxelSize),
	m_p_image(new T[m_width * m_height * m_number_of_slices]),
	m_texture_id(0)
//----------------------------------------------------------------
{
	// Out of memeory
	if (m_width && m_height && !m_p_image)
	{
		throw OutOfMemoryException(__FILE__, __FUNCTION__, __LINE__);
	}

	// Copy the data
	std::copy(apData, apData + m_width * m_height * m_number_of_slices, m_p_image);
}


//----------------------------------------------
template<typename T> Image<T>::Image(unsigned int aWidth,
                                     unsigned int aHeight,
                                     unsigned int aNumberOfSlices,
                                     T aDefaultValue,
                                     const VEC3& aVoxelSize):
	//----------------------------------------------
	m_width(aWidth),
	m_height(aHeight),
	m_number_of_slices(aNumberOfSlices),
	m_spacing(aVoxelSize),
	m_p_image(new T[m_width * m_height * m_number_of_slices]),
	m_texture_id(0)
//----------------------------------------------
{
	// Out of memeory
	if (m_width && m_height && m_number_of_slices && !m_p_image)
	{
		throw OutOfMemoryException(__FILE__, __FUNCTION__, __LINE__);
	}

	// Initialise the data
	std::fill_n(m_p_image, m_width * m_height * m_number_of_slices, aDefaultValue);
}


//-------------
template<typename T> Image<T>::~Image()
//-------------
{
	// Release memory
	destroy();
}


//--------------------------------------------------------------------
template<typename T> void Image<T>::makeBlank(const T& aDefaultColour)
//--------------------------------------------------------------------
{
	// Initialise the data
	std::fill_n(m_p_image, m_width * m_height * m_number_of_slices, aDefaultColour);
}

//-----------------------------------------------------------------------
template<typename T> Image<T> Image<T>::getROI(unsigned int i,
                                               unsigned int j,
                                               unsigned int k,
                                               unsigned int aWidth,
                                               unsigned int aHeight,
                                               unsigned int aDepth) const
//-----------------------------------------------------------------------
{
	// Create a black image
	Image<T> roi(aWidth, aHeight, aDepth);
	roi.m_spacing = m_spacing;

	// Process every slice of the ROI
#ifdef NDEBUG
#ifdef WIN32
#pragma omp parallel for
#else
#pragma omp parallel for collapse(3)
#endif
#endif
	for (int z = 0; z < int(aDepth); ++z)
	{
		// Process every row of the ROI
		for (int y = 0; y < int(aHeight); ++y)
		{
			// Process every column of the ROI
			for (int x = 0; x < int(aWidth); ++x)
			{
				unsigned int index_i(x + i);
				unsigned int index_j(y + j);
				unsigned int index_k(z + k);

				// The pixel index is not valid
				if ((index_i >= m_width) ||
				    (index_j >= m_height) ||
				    (index_k >= m_number_of_slices))
				{
					throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
				}

				// Get the pixel intensity from the current instance
				T intensity(getPixel(index_i, index_j, index_k));

				// Set the pixel of the ROI
				roi.setPixel(x, y, z, intensity);
			}
		}
	}

	return (roi);
}


//----------------------------------------------------------
template<typename T> void Image<T>::setPixel(unsigned int i,
                                             unsigned int j,
                                             unsigned int k,
                                             T aValue)
//----------------------------------------------------------
{
	// The pixel index is not valid
	if ((i >= m_width) ||
	    (j >= m_height) ||
	    (k >= m_number_of_slices))
	{
		throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}

	m_p_image[k * m_width * m_height + j * m_width + i] = aValue;
}


//--------------------------------------------------------
template<typename T> T& Image<T>::getPixel(unsigned int i,
                                           unsigned int j,
                                           unsigned int k)
//--------------------------------------------------------
{
	// The pixel index is not valid
	if ((i >= m_width) ||
	    (j >= m_height) ||
	    (k >= m_number_of_slices))
	{
		throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}

	return (m_p_image[k * m_width * m_height + j * m_width + i]);
}


//--------------------------------------------------------------------
template<typename T> const T& Image<T>::getPixel(unsigned int i,
                                                 unsigned int j,
                                                 unsigned int k) const
//--------------------------------------------------------------------
{
	// The pixel index is not valid
	if ((i >= m_width) ||
	    (j >= m_height) ||
	    (k >= m_number_of_slices))
	{
		throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}

	return (m_p_image[k * m_width * m_height + j * m_width + i]);
}


//-------------------------------------------------------------
template<typename T> T& Image<T>::getPixelValue(unsigned int i,
                                                unsigned int j,
                                                unsigned int k)
//-------------------------------------------------------------
{
	return (getPixel(i, j, k));
}


//-------------------------------------------------------------------------
template<typename T> const T& Image<T>::getPixelValue(unsigned int i,
                                                      unsigned int j,
                                                      unsigned int k) const
//-------------------------------------------------------------------------
{
	return (getPixel(i, j, k));
}


//------------------------------------------------------------------------
template<typename T> VEC3 Image<T>::getPixelPosition(unsigned int i,
                                                     unsigned int j,
                                                     unsigned int k) const
//------------------------------------------------------------------------
{
	return (VEC3(m_spacing[0] * i, m_spacing[1] * j, m_spacing[2] * k));
}


//------------------------------------------------------------
template<typename T> T& Image<T>::operator[](unsigned anIndex)
//------------------------------------------------------------
{
	if (anIndex >= m_width * m_height * m_number_of_slices)
	{
		throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}

	return (m_p_image[anIndex]);
}


//------------------------------------------------------------------------
template<typename T> const T& Image<T>::operator[](unsigned anIndex) const
//------------------------------------------------------------------------
{
	if (anIndex >= m_width * m_height * m_number_of_slices)
	{
		throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}

	return (m_p_image[anIndex]);
}


//----------------------------------------------------------
template<typename T> T& Image<T>::operator()(unsigned int i,
                                             unsigned int j,
                                             unsigned int k)
//----------------------------------------------------------
{
	return (getPixel(i, j, k));
}


//----------------------------------------------------------------------
template<typename T> const T& Image<T>::operator()(unsigned int i,
                                                   unsigned int j,
                                                   unsigned int k) const
//----------------------------------------------------------------------
{
	return (getPixel(i, j, k));
}


//-------------------------------------------
template<typename T> Image<T>& Image<T>::operator=(const Image<T>& anImage)
//-------------------------------------------
{
	// The images different
	if (this != &anImage)
	{
		// The total number of pixels/voxels has changed
		if (m_width * m_height * m_number_of_slices != anImage.m_width * anImage.m_height * anImage.m_number_of_slices)
		{
			// Release memory
			destroy();

			// Allocate the memory
			m_p_image = new T[anImage.m_width * anImage.m_height * anImage.m_number_of_slices];

			// Out of memeory
			if (m_width && m_height && m_number_of_slices && !m_p_image)
			{
				throw OutOfMemoryException(__FILE__, __FUNCTION__, __LINE__);
			}
		}

		// Copy the image properites
		m_width   = anImage.m_width;
		m_height  = anImage.m_height;
		m_number_of_slices = anImage.m_number_of_slices;
		m_spacing = anImage.m_spacing;

		// Copy the data
		std::copy(anImage.m_p_image, anImage.m_p_image + m_width * m_height * m_number_of_slices, m_p_image);
	}

	// Return the instance
	return (*this);
}


template<typename T> Image<T>& Image<T>::operator=(const T* apImage)
{
	// Release the texture if needed
	destroyOpenGLTexture();

	// Copy the data
	std::copy(apImage, apImage + m_width * m_height * m_number_of_slices, m_p_image);

	// Return the instance
	return (*this);
}


//-------------------------------------------
template<typename T> void Image<T>::destroy()
//-------------------------------------------
{
	// Memory has been dynamically allocated
	if (m_p_image)
	{
		// Release the memory
		delete [] m_p_image;

		// Make sure the pointer is reset to NULL
		m_p_image = 0;
	}

	// There is no pixel in the image
	m_width  = 0;
	m_height = 0;
	m_number_of_slices = 0;

	destroyOpenGLTexture();
}


//--------------------------------------------
template<typename T> T* Image<T>::getRawData()
//--------------------------------------------
{
	return (m_p_image);
}


//--------------------------------------------------------
template<typename T> const T* Image<T>::getRawData() const
//--------------------------------------------------------
{
	return (m_p_image);
}


//----------------------------------
template<typename T> unsigned int Image<T>::getWidth() const
//----------------------------------
{
	return (m_width);
}


//-----------------------------------
template<typename T> unsigned int Image<T>::getHeight() const
//-----------------------------------
{
	return (m_height);
}


//----------------------------------------------------------
template<typename T> unsigned int Image<T>::getDepth() const
//----------------------------------------------------------
{
	return (m_number_of_slices);
}


//--------------------------------------------------------------------
template<typename T> void Image<T>::setSpacing(const VEC3& aPixelSize)
//--------------------------------------------------------------------
{
	m_spacing = aPixelSize;
}


//------------------------------------------------------------------------
template<typename T> void Image<T>::setSpacing(const double& aPixelWidth,
                                               const double& aPixelHeight,
                                               const double& aPixelDepth)
//------------------------------------------------------------------------
{
	m_spacing[0] = aPixelWidth;
	m_spacing[1] = aPixelHeight;
	m_spacing[2] = aPixelDepth;
}


//-----------------------------------------------------------
template<typename T> const VEC3& Image<T>::getSpacing() const
//-----------------------------------------------------------
{
	return (m_spacing);
}


//--------------------------------------------------
template<typename T> T Image<T>::getMinValue() const
//--------------------------------------------------
{
	// The image is empty
	if (!m_p_image)
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Empty image");
	}

	return (*std::min_element(&m_p_image[0], &m_p_image[m_width * m_height * m_number_of_slices]));
}


//--------------------------------------------------
template<typename T> T Image<T>::getMaxValue() const
//--------------------------------------------------
{
	// The image is empty
	if (!m_p_image)
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Empty image");
	}

	return (*std::max_element(&m_p_image[0], &m_p_image[m_width * m_height * m_number_of_slices]));
}


//-----------------------------------------------------------------------
template<typename T> T Image<T>::getMinValue(unsigned int aSliceID) const
//-----------------------------------------------------------------------
{
	// The image is empty
	if (!m_p_image)
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Empty image");
	}

	return (*std::min_element(&m_p_image[aSliceID * m_width * m_height], &m_p_image[(aSliceID + 1) * m_width * m_height]));
}


//-----------------------------------------------------------------------
template<typename T> T Image<T>::getMaxValue(unsigned int aSliceID) const
//-----------------------------------------------------------------------
{
	// The image is empty
	if (!m_p_image)
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__, "Empty image");
	}

	return (*std::max_element(&m_p_image[aSliceID * m_width * m_height], &m_p_image[(aSliceID + 1) * m_width * m_height]));
}


//--------------------------------------------------------------------------------
template<typename T> Image<T> Image<T>::shiftScaleFilter(double aShiftValue,
                                                         double aScaleValue,
                                                         bool aRunOnGPUFlag) const
//--------------------------------------------------------------------------------
{
	// Copy the instance into a temporary variable
	Image<T> temp(*this);
	temp.m_spacing = m_spacing;

	bool use_gpu = false;

	if (aRunOnGPUFlag)
	{
		if (useOpenGLCompute())
		{
			const_cast<Image<T>&>(*this).transferHostToDevice();
			const_cast<Image<T>&>(temp).transferHostToDevice();

			if (getTextureId() && temp.getTextureId())
				use_gpu = true;
		}
		else
		{
			LOGGER.logWarning("GPU computation requested but not supported in the function as follows:") << std::endl <<
					"\tFile: " << __FILE__ << std::endl <<
					"\tFunction: " << __FUNCTION__ << std::endl <<
					"\tLine: " << __LINE__ << std::endl;
		}
	}

	if (use_gpu)
	{
		// Compute shader
		std::string g_compute_shader;
		g_compute_shader += "#version 430\n";
		g_compute_shader += "\n";
		g_compute_shader += "layout(local_size_x=1, local_size_y=1, local_size_z=1) in;\n";
		g_compute_shader += "layout(" + getShaderTypeID(typeid(T)) + ", binding=0) uniform " + getShaderImageType(typeid(T)) + " input_output_image;\n";
		g_compute_shader += "\n";
		g_compute_shader += "layout(location=0) uniform float shift;\n";
		g_compute_shader += "layout(location=1) uniform float scale;\n";
		g_compute_shader += "\n";
		g_compute_shader += "void main(void)\n";
		g_compute_shader += "{\n";
		g_compute_shader += "    ivec3 pixel_coordinates = ivec3(gl_GlobalInvocationID.xyz);\n";
		g_compute_shader += "    float intensity = imageLoad(input_output_image, pixel_coordinates).r;\n";
		g_compute_shader += "    " + getShaderPixelType(typeid(T)) + " pixel_value = (shift + intensity) * scale;\n";
		g_compute_shader += "\n";
		g_compute_shader += "    imageStore(input_output_image, pixel_coordinates, " + getShaderRGBAPixelType(typeid(T)) + "(pixel_value, 0, 0, 0));\n";
		g_compute_shader += "}\n";

		LOGGER.logNow(g_compute_shader) << std::endl;

		try
		{
			// Instantiate a compute shader
			Shader dummy_shader;

			// Give a text ID to the compute  shader, it can be useful when debuging the shader
			dummy_shader.setLabel("g_compute_shader");

			// Load the source code of the shader onto the GPU
			dummy_shader.loadSource(static_cast<const GLchar*>(g_compute_shader.c_str()));

			// Check that the compute shader is valid
			if (dummy_shader.isValid())
			{
#ifdef HAS_OPENGL
				// Bind the texture
				glBindImageTexture(0, temp.getTextureId(), 0, GL_FALSE, 0, GL_READ_WRITE, getShaderOpenGLType(typeid(T)));
				checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

				// Enable the compute shader
				dummy_shader.enable();
				checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

				// Load uniform variables to the GPU
				GLint program_handle(0);
				glGetIntegerv(GL_CURRENT_PROGRAM, &program_handle);
				checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

				GLint uniform_handle = glGetUniformLocation(program_handle, "shift");
				glUniform1f(uniform_handle, aShiftValue);
				checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

				uniform_handle = glGetUniformLocation(program_handle, "scale");
				glUniform1f(uniform_handle, aScaleValue);
				checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);


				// Dispatch the compute shader
				glDispatchCompute(getWidth(), getHeight(), getDepth());
				checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

				// Make sure all the calculations are performed
				glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
				checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
#endif

				const_cast<Image<T>&>(temp).transferDeviceToHost();
			}
		}
		catch (const Exception& e)
		{
			LOGGER.logWarning(e.what()) << std::endl;

			use_gpu = false;
		}
		catch (...)
		{
			LOGGER.logWarning("GPU computation requested and supported but failed to execute in the function as follows:") << std::endl <<
					"\tFile: " << __FILE__ << std::endl <<
					"\tFunction: " << __FUNCTION__ << std::endl <<
					"\tLine: " << __LINE__ << std::endl;

			use_gpu = false;
		}
	}

	if (use_gpu == false)
	{
		// Process every pixel of the image
#ifdef NDEBUG
#pragma omp parallel for
#endif
#ifdef WIN32
		for (int i = 0; i < m_width * m_height * m_number_of_slices; ++i)
#else
		for (size_t i = 0; i < m_width * m_height * m_number_of_slices; ++i)
#endif
		{
			// Apply the shilft/scale filter
			temp[i] = (temp[i] + aShiftValue) * aScaleValue;
		}
	}

	// Return the result
	return (temp);
}


template<typename T> Image<unsigned char> Image<T>::threshold(const T& aLowerThreshold,
                                                              const T& aUpperThreshold,
                                                              unsigned char anInValue,
                                                              unsigned char anOutValue,
                                                              bool aRunOnGPUFlag) const
{
	Image<unsigned char> segmentation(m_width, m_height, m_number_of_slices, anOutValue);
	segmentation.setSpacing(m_spacing);


	bool use_gpu = false;

	if (aRunOnGPUFlag)
	{
		if (useOpenGLCompute())
		{
			const_cast<Image<T>&>(*this).transferHostToDevice();
			const_cast<Image<unsigned char>&>(segmentation).transferHostToDevice();

			if (getTextureId() && segmentation.getTextureId())
				use_gpu = true;
		}
		else
		{
			LOGGER.logWarning("GPU computation requested but not supported in the function as follows:") << std::endl <<
					"\tFile: " << __FILE__ << std::endl <<
					"\tFunction: " << __FUNCTION__ << std::endl <<
					"\tLine: " << __LINE__ << std::endl;
		}
	}

	if (use_gpu)
	{
		// Compute shader
		std::string g_compute_shader;
		g_compute_shader += "#version 430\n";
		g_compute_shader += "\n";
		g_compute_shader += "layout(local_size_x=1, local_size_y=1, local_size_z=1) in;\n";
		g_compute_shader += "layout(" + getShaderTypeID(typeid(T)) + ", binding=0) uniform " + getShaderImageType(typeid(T)) + " input_image;\n";
		g_compute_shader += "layout(" + getShaderTypeID(typeid(unsigned char)) + ", binding=1) uniform " + getShaderImageType(typeid(unsigned char)) + " output_image;\n";
		g_compute_shader += "\n";
		g_compute_shader += "layout(location=0) uniform float low_threshold;\n";
		g_compute_shader += "layout(location=1) uniform float high_threshold;\n";
		g_compute_shader += "layout(location=2) uniform uint in_value;\n";
		g_compute_shader += "layout(location=3) uniform uint out_value;\n";
		g_compute_shader += "\n";
		g_compute_shader += "void main(void)\n";
		g_compute_shader += "{\n";
		g_compute_shader += "    ivec3 pixel_coordinates = ivec3(gl_GlobalInvocationID.xyz);\n";
		g_compute_shader += "    float intensity = imageLoad(input_image, pixel_coordinates).r;\n";
		g_compute_shader += "    int comparison_in = int(low_threshold <= intensity && intensity <= high_threshold);\n";
		g_compute_shader += "    int comparison_out = int(!(low_threshold <= intensity && intensity <= high_threshold));\n";
		g_compute_shader += "    " + getShaderPixelType(typeid(unsigned char)) + " pixel_value = comparison_in * in_value + comparison_out * out_value;\n";
		g_compute_shader += "\n";
		g_compute_shader += "    imageStore(output_image, pixel_coordinates, " + getShaderRGBAPixelType(typeid(unsigned char)) + "(pixel_value, 0, 0, 0));\n";
		g_compute_shader += "}\n";

		try
		{
			// Instantiate a compute shader
			Shader dummy_shader;

			// Give a text ID to the compute  shader, it can be useful when debuging the shader
			dummy_shader.setLabel("g_compute_shader");

			// Load the source code of the shader onto the GPU
			dummy_shader.loadSource(static_cast<const GLchar*>(g_compute_shader.c_str()));

			// Check that the compute shader is valid
			if (dummy_shader.isValid())
			{
#ifdef HAS_OPENGL
				// Bind the texture
				glBindImageTexture(0, getTextureId(), 0, GL_FALSE, 0, GL_READ_ONLY, getShaderOpenGLType(typeid(T)));
				glBindImageTexture(1, segmentation.getTextureId(), 0, GL_FALSE, 0, GL_WRITE_ONLY, getShaderOpenGLType(typeid(unsigned char)));
				checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

				// Enable the compute shader
				dummy_shader.enable();
				checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

				// Load uniform variables to the GPU
				GLint program_handle(0);
				glGetIntegerv(GL_CURRENT_PROGRAM, &program_handle);
				checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

				GLint uniform_handle = glGetUniformLocation(program_handle, "low_threshold");
				glUniform1f(uniform_handle, aLowerThreshold);
				checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

				uniform_handle = glGetUniformLocation(program_handle, "high_threshold");
				glUniform1f(uniform_handle, aUpperThreshold);
				checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

				uniform_handle = glGetUniformLocation(program_handle, "in_value");
				glUniform1ui(uniform_handle, anInValue);
				checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

				uniform_handle = glGetUniformLocation(program_handle, "out_value");
				glUniform1ui(uniform_handle, anOutValue);
				checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);


				// Dispatch the compute shader
				glDispatchCompute(getWidth(), getHeight(), getDepth());
				checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

				// Make sure all the calculations are performed
				glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
				checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
#endif

				const_cast<Image<unsigned char>&>(segmentation).transferDeviceToHost();
			}
		}
		catch (const Exception& e)
		{
			LOGGER.logWarning(e.what()) << std::endl;

			use_gpu = false;
		}
		catch (...)
		{
			LOGGER.logWarning("GPU computation requested and supported but failed to execute in the function as follows:") << std::endl <<
					"\tFile: " << __FILE__ << std::endl <<
					"\tFunction: " << __FUNCTION__ << std::endl <<
					"\tLine: " << __LINE__ << std::endl;

			use_gpu = false;
		}
	}

	if (use_gpu == false)
	{
#ifdef NDEBUG
#ifdef WIN32
#pragma omp parallel for
#else
#pragma omp parallel for collapse(3)
#endif
#endif
		for (int k = 0; k < int(m_number_of_slices); ++k)
		{
			for (int j = 0; j < int(m_height); ++j)
			{
				for (int i = 0; i < int(m_width); ++i)
				{
					unsigned int index(k * m_width * m_height + j * m_width + i);

					float voxel_value(m_p_image[index]);
					if (aLowerThreshold <= voxel_value && voxel_value <= aUpperThreshold)
					{
						segmentation.setPixel(i, j, k, anInValue);
					}
				}
			}
		}
	}
	return (segmentation);
}


//--------------------------------------------------------------------------------------------
template<typename T> Image<T> Image<T>::convolution1D(const std::vector<float>& aKernel) const
//--------------------------------------------------------------------------------------------
{
	// Copy the instance into a temporary variable
	Image<T> temp(m_width, m_height, m_number_of_slices, 0);
	temp.m_spacing = m_spacing;

	int kernel_size = aKernel.size();
	int half_kernel_size = kernel_size / 2;

	int image_area = m_width * m_height;

	const T* p_input_image_data = getRawData();
	const float* p_kernel = &aKernel[0];

	T* p_output_image_data = temp.getRawData();

	// Process every pixel of the image
#ifdef HAS_OPENMP_GPU
	int number_of_voxels = image_area * m_number_of_slices;

#pragma omp target teams distribute parallel for collapse(3) \
    map(to:p_input_image_data[0:number_of_voxels], p_kernel[0:aKernel.size()]) \
    map(from:p_output_image_data[0:number_of_voxels])
#else
#ifdef WIN32
#pragma omp parallel for
#else
#pragma omp parallel for collapse(3)
#endif
#endif
    for (int k = 0; k < int(m_number_of_slices); ++k)
		for (int j = 0; j < int(m_height); ++j)
			for (int i = 0; i < int(m_width); ++i)
				for (int u = -half_kernel_size; u <= half_kernel_size; ++u)
				{
					int index_i(std::max(0, i + u));
					index_i = std::min(index_i, int(m_width - 1));

					int index_u = u + half_kernel_size;

					if (index_u < kernel_size)
					{
						p_output_image_data[k * image_area + j * m_width + i] += p_input_image_data[k * image_area + j * m_width + index_i] *
								p_kernel[index_u];
					}
				}

	// Return the result
	return (temp);
}


//--------------------------------------------------------------------------------------
template<typename T> Image<T> Image<T>::convolution2D(const Image<float>& aKernel) const
//--------------------------------------------------------------------------------------
{
	// Copy the instance into a temporary variable
	Image<T> temp(m_width, m_height, m_number_of_slices, 0);
	temp.m_spacing = m_spacing;

	int kernel_size_u = aKernel.getWidth();
	int kernel_size_v = aKernel.getHeight();

	int half_kernel_size_u = kernel_size_u / 2;
	int half_kernel_size_v = kernel_size_v / 2;

	int image_area = m_width * m_height;

	const T* p_input_image_data = getRawData();
	const float* p_kernel = &aKernel[0];

	T* p_output_image_data = temp.getRawData();

	// Process every pixel of the image
#ifdef HAS_OPENMP_GPU
	int number_of_voxels = image_area * m_number_of_slices;
#pragma omp target teams distribute parallel for collapse(3) \
		map(to:p_input_image_data[0:number_of_voxels], p_kernel[0:kernel_size_u * kernel_size_v]) \
		map(from:p_output_image_data[0:number_of_voxels])
#else
#if WIN32
#pragma omp parallel for
#else
#pragma omp parallel for collapse(3)
#endif
#endif
    for (int k = 0; k < int(m_number_of_slices); ++k)
		for (int j = 0; j < int(m_height); ++j)
			for (int i = 0; i < int(m_width); ++i)
				for (int v = -half_kernel_size_v; v <= half_kernel_size_v; ++v)
				{
					int index_j(std::max(0, j + v));
					index_j = std::min(index_j, int(m_height - 1));

					int index_v = v + half_kernel_size_v;

					for (int u = -half_kernel_size_u; u <= half_kernel_size_u; ++u)
					{
						int index_i(std::max(0, i + u));
						index_i = std::min(index_i, int(m_width - 1));

						int index_u = u + half_kernel_size_u;

						if (index_u < kernel_size_u && index_v < kernel_size_v)
						{
							p_output_image_data[k * image_area + j * m_width + i] += p_input_image_data[k * image_area + index_j * m_width + index_i] *
									p_kernel[index_v * kernel_size_u + index_u];
						}
					}
				}

	// Return the result
	return (temp);
}


//---------------------------------------------------------------------------------------
template<typename T> Image<T> Image<T>::convolution2D(const Image<double>& aKernel) const
//---------------------------------------------------------------------------------------
{
	// Copy the instance into a temporary variable
	Image<T> temp(m_width, m_height, m_number_of_slices, 0);
	temp.m_spacing = m_spacing;

	int kernel_size_u = aKernel.getWidth();
	int kernel_size_v = aKernel.getHeight();

	int half_kernel_size_u = kernel_size_u / 2;
	int half_kernel_size_v = kernel_size_v / 2;

	int image_area = m_width * m_height;

	const T* p_input_image_data = getRawData();
	const double* p_kernel = &aKernel[0];

	T* p_output_image_data = temp.getRawData();

	// Process every pixel of the image
#ifdef HAS_OPENMP_GPU
	int number_of_voxels = image_area * m_number_of_slices;

#pragma omp target teams distribute parallel for collapse(3) \
		map(to:p_input_image_data[0:number_of_voxels], p_kernel[0:kernel_size_u * kernel_size_v]) \
		map(from:p_output_image_data[0:number_of_voxels])
#else
#pragma omp parallel for collapse(3)
#endif
	for (int k = 0; k < int(m_number_of_slices); ++k)
		for (int j = 0; j < int(m_height); ++j)
			for (int i = 0; i < int(m_width); ++i)
				for (int v = -half_kernel_size_v; v <= half_kernel_size_v; ++v)
				{
					int index_j(std::max(0, j + v));
					index_j = std::min(index_j, int(m_height - 1));

					int index_v = v + half_kernel_size_v;

					for (int u = -half_kernel_size_u; u <= half_kernel_size_u; ++u)
					{
						int index_i(std::max(0, i + u));
						index_i = std::min(index_i, int(m_width - 1));

						int index_u = u + half_kernel_size_u;

						if (index_u < kernel_size_u && index_v < kernel_size_v)
						{
							p_output_image_data[k * image_area + j * m_width + i] += p_input_image_data[k * image_area + index_j * m_width + index_i] *
									p_kernel[index_v * kernel_size_u + index_u];
						}
					}
				}

	// Return the result
	return (temp);
}


template<typename T> Image<unsigned char> Image<T>::opening(unsigned int aRadius) const
{
#ifndef HAS_ITK
	throw Exception(__FILE__, __FUNCTION__, __LINE__,
	                "gVirtualXRay has not been compiled with ITK support. As a consequence Image<T>::antiAliasBinaryImageFilter is not available.");
#else

	typedef itk::Image< unsigned char, 3 > InputImageType;
	typedef itk::ImportImageFilter< unsigned char, 3 >   ImportFilterType;

	ImportFilterType::Pointer importFilter = ImportFilterType::New();

	ImportFilterType::SizeType  size;
	size[0]  = m_width;  // size along X
	size[1]  = m_height;  // size along Y
	size[2]  = m_number_of_slices;  // size along Z

	ImportFilterType::IndexType start;
	start.Fill( 0 );

	ImportFilterType::RegionType region;
	region.SetIndex( start );
	region.SetSize(  size  );

	importFilter->SetRegion( region );

	double origin[ 3 ];
	origin[0] = 0.0;    // X coordinate
	origin[1] = 0.0;    // Y coordinate
	origin[2] = 0.0;    // Z coordinate

	importFilter->SetOrigin( origin );

	double spacing[ 3 ];
	spacing[0] = m_spacing[0];    // along X direction
	spacing[1] = m_spacing[1];    // along Y direction
	spacing[2] = m_spacing[2];    // along Z direction

	importFilter->SetSpacing( spacing );

	const bool importImageFilterWillOwnTheBuffer = false;
	importFilter->SetImportPointer( m_p_image, m_width * m_height * m_number_of_slices,
	                                importImageFilterWillOwnTheBuffer );

	typedef itk::BinaryBallStructuringElement<InputImageType::PixelType, InputImageType::ImageDimension>
			StructuringElementType;
	StructuringElementType structuringElement;
	structuringElement.SetRadius(aRadius);
	structuringElement.CreateStructuringElement();

	typedef itk::BinaryMorphologicalOpeningImageFilter <InputImageType, InputImageType, StructuringElementType>
			BinaryMorphologicalOpeningImageFilterType;
	BinaryMorphologicalOpeningImageFilterType::Pointer openingFilter
			= BinaryMorphologicalOpeningImageFilterType::New();
	openingFilter->SetInput(importFilter->GetOutput());
	openingFilter->SetKernel(structuringElement);

	openingFilter->SetBackgroundValue (getMinValue());

	openingFilter->SetForegroundValue (getMaxValue());

	openingFilter->Update();

	return (Image<unsigned char>(openingFilter->GetOutput()->GetBufferPointer(), m_width, m_height, m_number_of_slices, m_spacing));
#endif
}


template<typename T> Image<unsigned char> Image<T>::closing(unsigned int aRadius) const
{
#ifndef HAS_ITK
	throw Exception(__FILE__, __FUNCTION__, __LINE__,
	                "gVirtualXRay has not been compiled with ITK support. As a consequence Image<T>::antiAliasBinaryImageFilter is not available.");
#else

	typedef itk::Image< unsigned char, 3 > InputImageType;
	typedef itk::ImportImageFilter< unsigned char, 3 >   ImportFilterType;

	ImportFilterType::Pointer importFilter = ImportFilterType::New();

	ImportFilterType::SizeType  size;
	size[0]  = m_width;  // size along X
	size[1]  = m_height;  // size along Y
	size[2]  = m_number_of_slices;  // size along Z

	ImportFilterType::IndexType start;
	start.Fill( 0 );

	ImportFilterType::RegionType region;
	region.SetIndex( start );
	region.SetSize(  size  );

	importFilter->SetRegion( region );

	double origin[ 3 ];
	origin[0] = 0.0;    // X coordinate
	origin[1] = 0.0;    // Y coordinate
	origin[2] = 0.0;    // Z coordinate

	importFilter->SetOrigin( origin );

	double spacing[ 3 ];
	spacing[0] = m_spacing[0];    // along X direction
	spacing[1] = m_spacing[1];    // along Y direction
	spacing[2] = m_spacing[2];    // along Z direction

	importFilter->SetSpacing( spacing );

	const bool importImageFilterWillOwnTheBuffer = false;
	importFilter->SetImportPointer( m_p_image, m_width * m_height * m_number_of_slices,
	                                importImageFilterWillOwnTheBuffer );

	typedef itk::BinaryBallStructuringElement<InputImageType::PixelType, InputImageType::ImageDimension>
			StructuringElementType;
	StructuringElementType structuringElement;
	structuringElement.SetRadius(aRadius);
	structuringElement.CreateStructuringElement();

	typedef itk::BinaryMorphologicalClosingImageFilter <InputImageType, InputImageType, StructuringElementType>
			BinaryMorphologicalClosingImageFilterType;
	BinaryMorphologicalClosingImageFilterType::Pointer closingFilter
			= BinaryMorphologicalClosingImageFilterType::New();
	closingFilter->SetInput(importFilter->GetOutput());
	closingFilter->SetKernel(structuringElement);

	// closingFilter->SetBackgroundValue (getMinValue());

	closingFilter->SetForegroundValue (getMaxValue());

	closingFilter->Update();

	return (Image<unsigned char>(closingFilter->GetOutput()->GetBufferPointer(), m_width, m_height, m_number_of_slices, m_spacing));
#endif
}




template<typename T> Image<float> Image<T>::getIsoTropic() const
{
#ifndef HAS_ITK
	throw Exception(__FILE__, __FUNCTION__, __LINE__,
	                "gVirtualXRay has not been compiled with ITK support. As a consequence Image<T>::antiAliasBinaryImageFilter is not available.");
#else

	typedef itk::Image< unsigned char, 3 > InputImageType;
	typedef itk::Image< float, 3 > OutputImageType;
	typedef itk::ImportImageFilter< unsigned char, 3 >   ImportFilterType;

	ImportFilterType::Pointer importFilter = ImportFilterType::New();

	ImportFilterType::SizeType  current_size;
	current_size[0]  = m_width;  // size along X
	current_size[1]  = m_height;  // size along Y
	current_size[2]  = m_number_of_slices;  // size along Z

	ImportFilterType::IndexType start;
	start.Fill( 0 );

	ImportFilterType::RegionType region;
	region.SetIndex( start );
	region.SetSize(  current_size  );

	importFilter->SetRegion( region );

	double origin[ 3 ];
	origin[0] = 0.0;    // X coordinate
	origin[1] = 0.0;    // Y coordinate
	origin[2] = 0.0;    // Z coordinate

	importFilter->SetOrigin( origin );

	double current_spacing[ 3 ];
	current_spacing[0] = m_spacing[0];    // along X direction
	current_spacing[1] = m_spacing[1];    // along Y direction
	current_spacing[2] = m_spacing[2];    // along Z direction

	importFilter->SetSpacing( current_spacing );

	const bool importImageFilterWillOwnTheBuffer = false;
	importFilter->SetImportPointer( m_p_image, m_width * m_height * m_number_of_slices,
	                                importImageFilterWillOwnTheBuffer );


	// Software Guide : BeginLatex
	//
	// We instantiate the smoothing filter that will be used on the preprocessing
	// for subsampling the in-plane resolution of the dataset.
	//
	// Software Guide : std::endlatex
	// Software Guide : BeginCodeSnippet
	typedef itk::RecursiveGaussianImageFilter<
		InputImageType,
		OutputImageType > InputGaussianFilterType;
	typedef itk::RecursiveGaussianImageFilter<
		OutputImageType,
		OutputImageType > OutputGaussianFilterType;
	// Software Guide : EndCodeSnippet
	// Software Guide : BeginLatex
	//
	// We create two instances of the smoothing filter: one will smooth along the
	// $X$ direction while the other will smooth along the $Y$ direction. They are
	// connected in a cascade in the pipeline, while taking their input from the
	// intensity windowing filter. Note that you may want to skip the intensity
	// windowing scale and simply take the input directly from the reader.
	//
	// Software Guide : std::endlatex
	// Software Guide : BeginCodeSnippet
	InputGaussianFilterType::Pointer smootherX = InputGaussianFilterType::New();
	OutputGaussianFilterType::Pointer smootherY = OutputGaussianFilterType::New();
	smootherX->SetInput( importFilter->GetOutput() );
	smootherY->SetInput( smootherX->GetOutput() );
	// Software Guide : EndCodeSnippet
	// Software Guide : BeginLatex
	//
	// We must now provide the settings for the resampling itself. This is done by
	// searching for a value of isotropic resolution that will provide a trade-off
	// between the evil of subsampling and the evil of supersampling. We advance
	// here the conjecture that the geometrical mean between the in-plane and the
	// inter-slice resolutions should be a convenient isotropic resolution to use.
	// This conjecture is supported on nothing other than intuition and common
	// sense. You can rightfully argue that this choice deserves a more technical
	// consideration, but then, if you are so concerned about the technical integrity
	// of the image sampling process, you should not be using this code, and should
	// discuss these issues with the radiologist who
	// acquired this ugly anisotropic dataset.
	//
	// We take the image from the input and then request its array of pixel spacing
	// values.
	//
	// Software Guide : std::endlatex
	// Software Guide : BeginCodeSnippet
	//InputImageType::ConstPointer inputImage = reader->GetOutput();
	//const InputImageType::SpacingType& inputSpacing = inputImage->GetSpacing();
	// Software Guide : EndCodeSnippet
	// Software Guide : BeginLatex
	//
	// and apply our ad-hoc conjecture that the correct anisotropic resolution
	// to use is the geometrical mean of the in-plane and inter-slice resolutions.
	// Then set this spacing as the Sigma value to be used for the Gaussian
	// smoothing at the preprocessing stage.
	//
	// Software Guide : std::endlatex
	// Software Guide : BeginCodeSnippet
	const double isoSpacing = std::sqrt( current_spacing[2] * current_spacing[0] );
	smootherX->SetSigma( isoSpacing );
	smootherY->SetSigma( isoSpacing );
	// Software Guide : EndCodeSnippet
	// Software Guide : BeginLatex
	//
	// We instruct the smoothing filters to act along the $X$ and $Y$ direction
	// respectively.
	//
	// Software Guide : std::endlatex
	// Software Guide : BeginCodeSnippet
	smootherX->SetDirection( 0 );
	smootherY->SetDirection( 1 );
	// Software Guide : EndCodeSnippet
	// Software Guide : BeginLatex
	//
	// Now that we have taken care of the smoothing in-plane, we proceed to
	// instantiate the resampling filter that will reconstruct an isotropic image.
	// We start by declaring the pixel type to be used as the output of this filter,
	// then instantiate the image type and the type for the resampling filter.
	// Finally we construct an instantiation of the filter.
	//
	// Software Guide : std::endlatex
	// Software Guide : BeginCodeSnippet
	typedef itk::ResampleImageFilter<
		OutputImageType, OutputImageType >  ResampleFilterType;
	ResampleFilterType::Pointer resampler = ResampleFilterType::New();
	// Software Guide : EndCodeSnippet
	// Software Guide : BeginLatex
	//
	// The resampling filter requires that we provide a Transform, which in this
	// particular case can simply be an identity transform.
	//
	// Software Guide : std::endlatex
	// Software Guide : BeginCodeSnippet
	typedef itk::IdentityTransform< double, 3 >  TransformType;
	TransformType::Pointer transform = TransformType::New();
	transform->SetIdentity();
	resampler->SetTransform( transform );
	// Software Guide : EndCodeSnippet
	// Software Guide : BeginLatex
	//
	// The filter also requires an interpolator to be passed to it. In this case we
	// chose to use a linear interpolator.
	//
	// Software Guide : std::endlatex
	// Software Guide : BeginCodeSnippet
	typedef itk::LinearInterpolateImageFunction<
		//typedef itk::BSplineInterpolateImageFunction<
		OutputImageType, double >  InterpolatorType;
	InterpolatorType::Pointer interpolator = InterpolatorType::New();
	resampler->SetInterpolator( interpolator );
	// Software Guide : EndCodeSnippet
	resampler->SetDefaultPixelValue( 0 ); // highlight regions without source
	// Software Guide : BeginLatex
	//
	// The pixel spacing of the resampled dataset is loaded in a \code{SpacingType}
	// and passed to the resampling filter.
	//
	// Software Guide : std::endlatex
	// Software Guide : BeginCodeSnippet
	OutputImageType::SpacingType spacing;
	spacing[0] = isoSpacing;
	spacing[1] = isoSpacing;
	spacing[2] = isoSpacing;
	resampler->SetOutputSpacing( spacing );
	// Software Guide : EndCodeSnippet
	// Software Guide : BeginLatex
	//
	// The origin and orientation of the output image is maintained, since we
	// decided to resample the image in the same physical extent of the input
	// anisotropic image.
	//
	// Software Guide : std::endlatex
	// Software Guide : BeginCodeSnippet
	resampler->SetOutputOrigin( origin );
	resampler->SetOutputDirection( importFilter->GetOutput()->GetDirection() );
	// Software Guide : EndCodeSnippet
	// Software Guide : BeginLatex
	//
	// The number of pixels to use along each dimension in the grid of the
	// resampled image is computed using the ratio between the pixel spacings of the
	// input image and those of the output image. Note that the computation of the
	// number of pixels along the $Z$ direction is slightly different with the
	// purpose of making sure that we don't attempt to compute pixels that are
	// outside of the original anisotropic dataset.
	//
	// Software Guide : std::endlatex
	// Software Guide : BeginCodeSnippet
	InputImageType::SizeType   inputSize =
			importFilter->GetOutput()->GetLargestPossibleRegion().GetSize();
	typedef InputImageType::SizeType::SizeValueType SizeValueType;
	const double dx = current_size[0] * current_spacing[0] / isoSpacing;
	const double dy = current_size[1] * current_spacing[1] / isoSpacing;
	const double dz = (current_size[2] - 1 ) * current_spacing[2] / isoSpacing;
	// Software Guide : EndCodeSnippet
	// Software Guide : BeginLatex
	//
	// Finally the values are stored in a \code{SizeType} and passed to the
	// resampling filter. Note that this process requires a casting since the
	// computations are performed in \code{double}, while the elements of the
	// \code{SizeType} are integers.
	//
	// Software Guide : std::endlatex
	// Software Guide : BeginCodeSnippet
	InputImageType::SizeType   size;
	size[0] = static_cast<SizeValueType>( dx );
	size[1] = static_cast<SizeValueType>( dy );
	size[2] = static_cast<SizeValueType>( dz );
	resampler->SetSize( size );
	// Software Guide : EndCodeSnippet
	// Software Guide : BeginLatex
	//
	// Our last action is to take the input for the resampling image filter from
	// the output of the cascade of smoothing filters, and then to trigger the
	// execution of the pipeline by invoking the \code{Update()} method on the
	// resampling filter.
	//
	// Software Guide : std::endlatex
	// Software Guide : BeginCodeSnippet
	resampler->SetInput( smootherY->GetOutput() );
	resampler->Update();

	return (Image<float>(resampler->GetOutput()->GetBufferPointer(), size[0], size[1], size[2], VEC3(isoSpacing, isoSpacing, isoSpacing)));
#endif
}


template<typename T> Image<float> Image<T>::antiAliasBinaryImageFilter(double aMaximumRMSChange, unsigned int aNumberOfIterations, double aVariance) const
{
#ifndef HAS_ITK
	throw Exception(__FILE__, __FUNCTION__, __LINE__,
	                "gVirtualXRay has not been compiled with ITK support. As a consequence Image<T>::antiAliasBinaryImageFilter is not available.");
#else
	if (aMaximumRMSChange >= 1.0)
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__,
		                "aMaximumRMSChange should be less than 1.0. A value of 0.07 is a good starting estimate.");
	}

	typedef itk::Image< unsigned char, 3 > InputImageType;
	typedef itk::Image< float,         3 > OutputImageType;

	typedef itk::ImportImageFilter< unsigned char, 3 >   ImportFilterType;

	ImportFilterType::Pointer importFilter = ImportFilterType::New();

	ImportFilterType::SizeType  size;
	size[0]  = m_width;  // size along X
	size[1]  = m_height;  // size along Y
	size[2]  = m_number_of_slices;  // size along Z

	ImportFilterType::IndexType start;
	start.Fill( 0 );

	ImportFilterType::RegionType region;
	region.SetIndex( start );
	region.SetSize(  size  );

	importFilter->SetRegion( region );

	double origin[ 3 ];
	origin[0] = 0.0;    // X coordinate
	origin[1] = 0.0;    // Y coordinate
	origin[2] = 0.0;    // Z coordinate

	importFilter->SetOrigin( origin );

	double spacing[ 3 ];
	spacing[0] = m_spacing[0];    // along X direction
	spacing[1] = m_spacing[1];    // along Y direction
	spacing[2] = m_spacing[2];    // along Z direction

	importFilter->SetSpacing( spacing );

	const bool importImageFilterWillOwnTheBuffer = false;
	importFilter->SetImportPointer( m_p_image, m_width * m_height * m_number_of_slices,
	                                importImageFilterWillOwnTheBuffer );

	typedef itk::DiscreteGaussianImageFilter< InputImageType, OutputImageType > GaussianBlurImageFilter;
	GaussianBlurImageFilter::Pointer p_gaussian_blur_filter(GaussianBlurImageFilter::New());
	p_gaussian_blur_filter->SetInput(importFilter->GetOutput());
	p_gaussian_blur_filter->SetVariance(aVariance);
	p_gaussian_blur_filter->Update();

	typedef itk::AntiAliasBinaryImageFilter< OutputImageType, OutputImageType > AntiAliasBinaryImageFilter;
	AntiAliasBinaryImageFilter::Pointer p_filter(AntiAliasBinaryImageFilter::New());
	p_filter->SetInput(p_gaussian_blur_filter->GetOutput());
	p_filter->SetNumberOfIterations(aNumberOfIterations);
	p_filter->SetMaximumRMSError(aMaximumRMSChange);
	p_filter->Update();

	return (Image<float>(p_filter->GetOutput()->GetBufferPointer(), m_width, m_height, m_number_of_slices, m_spacing));
#endif
}


//---------------------------------------------------------------------------------
template<typename T> Image<T> Image<T>::constantPadFilter(const T& aPadValue) const
//---------------------------------------------------------------------------------
{
	Image<T> temp_image(m_width + 2, m_height + 2, m_number_of_slices + 2, aPadValue, m_spacing);

#ifdef NDEBUG
#ifdef WIN32
#pragma omp parallel for
#else
#pragma omp parallel for collapse(3)
#endif
#endif
	for (int k = 0; k < int(m_number_of_slices); ++k)
	{
		for (int j = 0; j < int(m_height); ++j)
		{
			for (int i = 0; i < int(m_width); ++i)
			{
				temp_image.setPixel(i + 1, j + 1, k + 1, getPixel(i, j, k));
			}
		}
	}

	return temp_image;
}


//------------------------------------------------------------------------------------------
template<typename T> Image<unsigned char> Image<T>::threshold(const T& aThreshold,
                                                              unsigned char aValueIn,
                                                              unsigned char aValueOut) const
//------------------------------------------------------------------------------------------
{
	Image<unsigned char> temp_image(m_width, m_height, m_number_of_slices, aValueOut, m_spacing);

#ifdef NDEBUG
#ifdef WIN32
#pragma omp parallel for
#else
#pragma omp parallel for collapse(3)
#endif
#endif
	for (int k = 0; k < int(m_number_of_slices); ++k)
	{
		for (int j = 0; j < int(m_height); ++j)
		{
			for (int i = 0; i < int(m_width); ++i)
			{
				if (aThreshold <= getPixel(i, j, k))
				{
					temp_image.setPixel(i, j, k, aValueIn);
				}
			}
		}
	}

	return temp_image;
}


//------------------------------------------------------------------
template<typename T> void Image<T>::loadASCII(const char* aFileName)
//------------------------------------------------------------------
{
	// Open the file
	std::ifstream input_file (aFileName);

	// The file is not open
	if (!input_file.is_open())
	{
		std::string error_message("The file (");
		error_message += aFileName;
		error_message += ") does not exist";

		throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
	}

	// Load the data into a vector
	std::vector<T> p_data;
	std::string line;
	unsigned int number_of_rows(0);
	unsigned int number_of_columns(0);

	// Read evely line
	while (std::getline(input_file, line))
	{
		if (line.size())
		{
			if (line[0] != '#')
			{
				number_of_columns = 0;
				T intensity;
				std::stringstream line_parser;
				line_parser << line;
				while (line_parser >> intensity)
				{
					p_data.push_back(intensity);
					++number_of_columns;
				}
				++number_of_rows;
			}
		}
	}

	// Wrong number of pixels
	if (number_of_rows * number_of_columns != p_data.size())
	{
		std::string error_message("The file (");
		error_message += aFileName;
		error_message += ") is invalid";

		throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
	}

	// Release the memory
	destroy();

	// Allocate memory for file content
	m_width = number_of_columns;
	m_height = number_of_rows;
	m_number_of_slices = 1;
	m_p_image = new T[m_width * m_height];

	// Copy the data
	std::copy(p_data.begin(), p_data.end(), m_p_image);
}


//-------------------------------------------------------------------------
template<typename T> void Image<T>::loadASCII(const std::string& aFileName)
//-------------------------------------------------------------------------
{
	loadASCII(aFileName.data());
}


//-------------------------------------------------------------------
template<typename T> void Image<T>::loadASCII(const char* aFileName,
                                              unsigned int aWidth,
                                              unsigned int aHeight,
                                              unsigned int aDepth,
                                              const VEC3& aVoxelSize)
//-------------------------------------------------------------------
{
	// Open the file
	std::ifstream input_file(aFileName);

	// Release the memory
	destroy();

	// The file is not open
	if (!input_file.is_open())
	{
		std::string error_message("The file (");
		error_message += aFileName;
		error_message += ") does not exist";

		throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
	}

	// Allocate memory for file content
	m_width = aWidth;
	m_height = aHeight;
	m_number_of_slices = aDepth;
	unsigned int volume_size(m_width * m_height * m_number_of_slices);
	m_p_image = new T[volume_size];
	m_spacing = aVoxelSize;

	// Read evely line
	unsigned int voxels(0);
	T intensity;

	while (input_file >> intensity)
	{
		// Wrong number of pixels
		if (voxels >= volume_size)
		{
			/*std::string error_message("The file (");
            error_message += aFileName;
            error_message += ") is invalid (too many voxels)";

            throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);*/
		}
		else
		{
			m_p_image[voxels] = intensity;

		}
		++voxels;
	}

	// Wrong number of pixels
	/*if (voxels < volume_size)
    {
        std::string error_message("The file (");
        error_message += aFileName;
        error_message += ") is invalid (not enough voxels)";

        throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
    }*/
}


//-------------------------------------------------------------------------
template<typename T> void Image<T>::loadASCII(const std::string& aFileName,
                                              unsigned int aWidth,
                                              unsigned int aHeight,
                                              unsigned int aDepth,
                                              const VEC3& aVoxelSize)
//-------------------------------------------------------------------------
{
	loadASCII(aFileName.data(), aWidth,
	          aHeight,
	          aDepth,
	          aVoxelSize);
}


//--------------------------------------------------------
template<typename T> void Image<T>::transferHostToDevice()
//--------------------------------------------------------
{
#ifdef HAS_OPENGL
	// Generate the texture name
	if (!m_texture_id)
	{
		glGenTextures(1, &m_texture_id);
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

	// Set the texture properties
	glBindTexture(GL_TEXTURE_3D, m_texture_id);

	if (typeid(T) == typeid(unsigned int) ||
	    typeid(T) == typeid(int) ||
	    typeid(T) == typeid(unsigned short) ||
	    typeid(T) == typeid(short) ||
	    typeid(T) == typeid(unsigned char) ||
	    typeid(T) == typeid(char))
	{
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	else if (typeid(T) == typeid(float))
	{
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	else
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__,
		                "The data type of this image is not supported on GPU.");
	}

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	// Make sure the texture can be created using a texture proxy
	GLint texture_width(0);

	if (typeid(T) == typeid(unsigned int))
	{
		glTexImage3D(GL_PROXY_TEXTURE_3D, 0, GL_R32UI,
			m_width, m_height, m_number_of_slices, 0,
			GL_RED_INTEGER, GL_UNSIGNED_INT, nullptr);
	}
	else if (typeid(T) == typeid(int))
	{
		glTexImage3D(GL_PROXY_TEXTURE_3D, 0, GL_R32I,
			m_width, m_height, m_number_of_slices, 0,
			GL_RED_INTEGER, GL_INT, nullptr);
	}
	else if (typeid(T) == typeid(unsigned short))
	{
		glTexImage3D(GL_PROXY_TEXTURE_3D, 0, GL_R16UI,
			m_width, m_height, m_number_of_slices, 0,
			GL_RED_INTEGER, GL_UNSIGNED_SHORT, nullptr);
	}
	else if (typeid(T) == typeid(short))
	{
		glTexImage3D(GL_PROXY_TEXTURE_3D, 0, GL_R16I,
			m_width, m_height, m_number_of_slices, 0,
			GL_RED_INTEGER, GL_SHORT, nullptr);
	}
	else if (typeid(T) == typeid(unsigned char))
	{
		glTexImage3D(GL_PROXY_TEXTURE_3D, 0, GL_R8UI,
			m_width, m_height, m_number_of_slices, 0,
			GL_RED_INTEGER, GL_UNSIGNED_BYTE, nullptr);
	}
	else if (typeid(T) == typeid(char))
	{
		glTexImage3D(GL_PROXY_TEXTURE_3D, 0, GL_R8I,
			m_width, m_height, m_number_of_slices, 0,
			GL_RED_INTEGER, GL_BYTE, nullptr);
	}
	else if (typeid(T) == typeid(float))
	{
		glTexImage3D(GL_PROXY_TEXTURE_3D, 0, GL_R32F,
			m_width, m_height, m_number_of_slices, 0,
			GL_RED, GL_FLOAT, nullptr);
	}
	else
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__,
		                "The data type of this image is not supported on GPU.");
	}

	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	glGetTexLevelParameteriv(GL_PROXY_TEXTURE_3D, 0, GL_TEXTURE_WIDTH, &texture_width);

	if ((unsigned int)(texture_width) != m_width)
	{
		std::stringstream error_message;
		error_message << "Cannot create a " << "(" << m_width << " x " << m_height << " x " << m_number_of_slices << ") floating point render texture.";
		throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
	}

	if (typeid(T) == typeid(unsigned int))
	{
		glTexImage3D(GL_TEXTURE_3D, 0, GL_R32UI, m_width, m_height, m_number_of_slices, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, m_p_image);
	}
	else if (typeid(T) == typeid(int))
	{
		glTexImage3D(GL_TEXTURE_3D, 0, GL_R32I, m_width, m_height, m_number_of_slices, 0, GL_RED_INTEGER, GL_INT, m_p_image);
	}
	else if (typeid(T) == typeid(unsigned short))
	{
		glTexImage3D(GL_TEXTURE_3D, 0, GL_R16UI, m_width, m_height, m_number_of_slices, 0, GL_RED_INTEGER, GL_UNSIGNED_SHORT, m_p_image);
	}
	else if (typeid(T) == typeid(short))
	{
		glTexImage3D(GL_TEXTURE_3D, 0, GL_R16I, m_width, m_height, m_number_of_slices, 0, GL_RED_INTEGER, GL_SHORT, m_p_image);
	}
	else if (typeid(T) == typeid(unsigned char))
	{
		glTexImage3D(GL_TEXTURE_3D, 0, GL_R8UI, m_width, m_height, m_number_of_slices, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, m_p_image);
	}
	else if (typeid(T) == typeid(char))
	{
		glTexImage3D(GL_TEXTURE_3D, 0, GL_R8I, m_width, m_height, m_number_of_slices, 0, GL_RED_INTEGER, GL_BYTE, m_p_image);
	}
	else if (typeid(T) == typeid(float))
	{
		glTexImage3D(GL_TEXTURE_3D, 0, GL_R32F, m_width, m_height, m_number_of_slices, 0, GL_RED, GL_FLOAT, m_p_image);
	}
	else
	{
		throw Exception(__FILE__, __FUNCTION__, __LINE__,
		                "The data type of this image is not supported on GPU.");
	}
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
#endif
}


//--------------------------------------------------------
template<typename T> void Image<T>::transferDeviceToHost()
//--------------------------------------------------------
{
#ifdef HAS_OPENGL
	// The texture exists
	if (m_texture_id)
	{
		glBindTexture(GL_TEXTURE_3D, m_texture_id);

		if (typeid(T) == typeid(unsigned int))
		{
			glGetTexImage(GL_TEXTURE_3D, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, m_p_image);
		}
		else if (typeid(T) == typeid(int))
		{
			glGetTexImage(GL_TEXTURE_3D, 0, GL_RED_INTEGER, GL_INT, m_p_image);
		}
		else if (typeid(T) == typeid(unsigned short))
		{
			glGetTexImage(GL_TEXTURE_3D, 0, GL_RED_INTEGER, GL_UNSIGNED_SHORT, m_p_image);
		}
		else if (typeid(T) == typeid(short))
		{
			glGetTexImage(GL_TEXTURE_3D, 0, GL_RED_INTEGER, GL_SHORT, m_p_image);
		}
		else if (typeid(T) == typeid(unsigned char))
		{
			glGetTexImage(GL_TEXTURE_3D, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, m_p_image);
		}
		else if (typeid(T) == typeid(char))
		{
			glGetTexImage(GL_TEXTURE_3D, 0, GL_RED_INTEGER, GL_BYTE, m_p_image);
		}
		else if (typeid(T) == typeid(float))
		{
			glGetTexImage(GL_TEXTURE_3D, 0, GL_RED, GL_FLOAT, m_p_image);
		}
		else
		{
			throw Exception(__FILE__, __FUNCTION__, __LINE__,
			                "The data type of this image is not supported on GPU.");
		}
		checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}
#endif
}


////------------------------------------------------------------------------------------------
//template<typename T> Image<unsigned char> Image<T>::threshold(const T& aLowerThreshold,
//                                                              const T& aUpperThreshold,
//                                                              unsigned char aValueIn,
//                                                              unsigned char aValueOut) const
////------------------------------------------------------------------------------------------
//{
//    Image<unsigned char> temp_image(m_width, m_height, m_number_of_slices, aValueOut, m_spacing);
//
//#ifdef NDEBUG
//#ifdef WIN32
//        #pragma omp parallel for
//#else
//        #pragma omp parallel for collapse(3)
//#endif
//#endif
//    for (int k = 0; k < m_number_of_slices; ++k)
//    {
//        for (int j = 0; j < m_height; ++j)
//        {
//            for (int i = 0; i < m_width; ++i)
//            {
//                T pixel_value = getPixel(i, j, k);
//                if (aLowerThreshold <= pixel_value && pixel_value < aUpperThreshold)
//                {
//                    temp_image.setPixel(i, j, k, aValueIn);
//                }
//            }
//        }
//    }
//
//    return temp_image;
//}


//--------------------------------------------------------------------------
template<typename T> Image<T> Image<T>::normalised(bool aRunOnGPUFlag) const
//--------------------------------------------------------------------------
{
    return (shiftScaleFilter(-getMinValue(), 1.0 / (getMaxValue() - getMinValue()), aRunOnGPUFlag));
}


//--------------------------------------------------------------------------
template<typename T> Image<T> Image<T>::normalized(bool aRunOnGPUFlag) const
//--------------------------------------------------------------------------
{
    return (normalised(aRunOnGPUFlag));
}


//------------------------------------------------------------
template<typename T> Image<T> Image<T>::flipVertically() const
//------------------------------------------------------------
{
    Image<T> temp(*this);
    temp.m_spacing = m_spacing;

#ifdef NDEBUG
#ifdef WIN32
        #pragma omp parallel for
#else
        #pragma omp parallel for collapse(3)
#endif
#endif
    for (int k = 0; k < int(m_number_of_slices); ++k)
        {
        for (int j = 0; j < int(m_height / 2); ++j)
            {
            for (int i = 0; i < int(m_width); ++i)
                {
                std::swap(
                    temp[k * m_height * m_width +                 j  * m_width + i],
                    temp[k * m_height * m_width + (m_height - 1 - j) * m_width + i]
                    );
                }
            }
        }

    return (temp);
}


//--------------------------------------------------------------
template<typename T> Image<T> Image<T>::flipHorizontally() const
//--------------------------------------------------------------
{
    Image<T> temp(*this);
    temp.m_spacing = m_spacing;

#ifdef NDEBUG
#ifdef WIN32
        #pragma omp parallel for
#else
        #pragma omp parallel for collapse(3)
#endif
#endif
    for (unsigned int k = 0; k < m_number_of_slices; ++k)
        {
        for (unsigned int j = 0; j < m_height; ++j)
            {
            for (unsigned int i = 0; i < (m_width / 2); ++i)
                {
                std::swap(
                    temp[k * m_height * m_width + j * m_width +                i ],
                    temp[k * m_height * m_width + j * m_width + (m_width - 1 - i)]
                    );
                }
            }
        }

    return (temp);
}


//-------------------------------------------------
template<typename T> Image<T> Image<T>::abs() const
//-------------------------------------------------
{
    Image<T> temp(*this);
    temp.m_spacing = m_spacing;

#ifdef NDEBUG
        #pragma omp parallel for
#endif
#ifdef WIN32
    for (int i = 0; i < m_width * m_height * m_number_of_slices; ++i)
#else
    for (size_t i = 0; i < m_width * m_height * m_number_of_slices; ++i)
#endif
        {
        temp[i] = std::abs(m_p_image[i]);
        }

    return (temp);
}


//----------------------
template<typename T> Image<T> Image<T>::log() const
//----------------------
{
    Image<T> temp(*this);
    temp.m_spacing = m_spacing;

    double offset(0.0);

    if (std::abs(getMinValue()) < EPSILON)
        {
        offset = 0.000000157;
        }

#ifdef NDEBUG
        #pragma omp parallel for
#endif
#ifdef WIN32
    for (int i = 0; i < m_width * m_height * m_number_of_slices; ++i)
#else
    for (size_t i = 0; i < m_width * m_height * m_number_of_slices; ++i)
#endif
    {
        double pixel(m_p_image[i]);

        if (std::abs(pixel) < EPSILON)
            {
            pixel = offset;
            }

        temp[i] = std::log(pixel);
        }

    return (temp);
}


//-------------------------------------------------------------
template<typename T> void Image<T>::load(const char* aFileName)
//-------------------------------------------------------------
{
#ifdef HAS_ITK
    loadUsingITK(aFileName);
#else
    if (isMAT(aFileName) || isDAT(aFileName) || isTXT(aFileName))
    {
        loadASCII(aFileName);
    }
    else if (isPGM(aFileName))
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
                "Loading PGM files is not implemented, "
                "please contact Dr FP Vidal.");
    }
    else if (isRAW(aFileName))
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
                "Loading RAW files is not implemented, "
                "please contact Dr FP Vidal.");
    }
    else if (isMHD(aFileName))
    {
        loadMHD(aFileName);
    }
    else if (isMHA(aFileName))
    {
        loadMHA(aFileName);
    }
    else if (isDCM(aFileName))
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
                "Loading DICOM files is not implemented, "
                "please contact Dr FP Vidal.");
    }
    else if (isTIFF(aFileName))
    {
        loadTIFF(aFileName);
    }
    else if (isJPEG(aFileName))
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
                "Loading JPEG files is not implemented, "
                "please contact Dr FP Vidal.");
    }
    else
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
                "Unknown file format, to get it added to the library, "
                "please contact Dr FP Vidal.");
    }
#endif
}


//--------------------------------------------------------------------
template<typename T> void Image<T>::load(const std::string& aFileName)
//--------------------------------------------------------------------
{
    load(aFileName.data());
}


//---------------------------------------------------------------------
template<typename T> void Image<T>::loadUsingITK(const char* aFileName)
//---------------------------------------------------------------------
{
#ifdef HAS_ITK

    // Set some types
    typedef T   PixelType;
    const unsigned int Dimension = 3;

    typedef itk::Image< PixelType, Dimension >  ImageType;
    typedef itk::ImageFileReader < ImageType > ReaderType;

    // Set the loader
    typename ReaderType::Pointer p_reader(ReaderType::New());
    p_reader->SetFileName(aFileName);
    p_reader->Update();

    // Get the image size
    typename ImageType::RegionType region(p_reader->GetOutput()->GetLargestPossibleRegion());
    typename ImageType::SizeType size(region.GetSize());

    m_width            = size[0];
    m_height           = size[1];
    m_number_of_slices = size[2];

    const typename ImageType::SpacingType spacing(p_reader->GetOutput()->GetSpacing());
    m_spacing[0] = spacing[0];
    m_spacing[1] = spacing[1];
    m_spacing[2] = spacing[2];

    // Release the memory if needed
    if (m_p_image)
    {
    	delete [] m_p_image;
    	m_p_image = 0;
	}

    // Allocate the memory
    int number_of_pixels(m_width * m_height * m_number_of_slices);
    m_p_image = new T[number_of_pixels];

    // Copy the pixel data
    std::copy(p_reader->GetOutput()->GetBufferPointer(),
            p_reader->GetOutput()->GetBufferPointer() + number_of_pixels,
            m_p_image);

#else
    throw Exception(__FILE__, __FUNCTION__, __LINE__,
            "ITK is not supported.");
#endif

}


//----------------------------------------------------------------------------
template<typename T> void Image<T>::loadUsingITK(const std::string& aFileName)
//----------------------------------------------------------------------------
{
    loadUsingITK(aFileName.data());
}


//------------------------------------------------------------------
template<typename T> void Image<T>::loadSeries(const char* aPattern,
                                               int aFirstSliceIndex,
                                               int aLastSliceIndex,
                                               int anIncrementIndex)
//------------------------------------------------------------------
{
#ifdef HAS_ITK

    // Set some types
    typedef T   PixelType;
    const unsigned int Dimension = 3;

    typedef itk::Image< PixelType, Dimension >  ImageType;
    typedef itk::ImageSeriesReader< ImageType > ReaderType;
    typedef itk::NumericSeriesFileNames    NameGeneratorType;

    // Create the file names
    NameGeneratorType::Pointer p_name_generator(NameGeneratorType::New());

    p_name_generator->SetSeriesFormat(aPattern);
    p_name_generator->SetStartIndex(aFirstSliceIndex);
    p_name_generator->SetEndIndex(aLastSliceIndex);
    p_name_generator->SetIncrementIndex(anIncrementIndex);
    std::vector<std::string> p_name_set(p_name_generator->GetFileNames());

    // Set the loader
    typename ReaderType::Pointer p_reader(ReaderType::New());
    p_reader->SetFileNames(p_name_set);
    p_reader->Update();

    // Get the image size
    typename ImageType::RegionType region(p_reader->GetOutput()->GetLargestPossibleRegion());
    typename ImageType::SizeType size(region.GetSize());

    m_width            = size[0];
    m_height           = size[1];
    m_number_of_slices = size[2];

    const typename ImageType::SpacingType spacing(p_reader->GetOutput()->GetSpacing());
    m_spacing[0] = spacing[0];
    m_spacing[1] = spacing[1];
    m_spacing[2] = spacing[2];

    // Release the memory if needed
    if (m_p_image)
    {
    	delete [] m_p_image;
    	m_p_image = 0;
	}

    // Allocate the memory
    int number_of_pixels(m_width * m_height * m_number_of_slices);
    m_p_image = new T[number_of_pixels];

    // Copy the pixel data
    std::copy(p_reader->GetOutput()->GetBufferPointer(),
            p_reader->GetOutput()->GetBufferPointer() + number_of_pixels,
            m_p_image);

#else
    throw Exception(__FILE__, __FUNCTION__, __LINE__,
            "ITK is not supported.");
#endif
}


//-------------------------------------------------------------------------
template<typename T> void Image<T>::loadSeries(const std::string& aPattern,
                                               int aFirstSliceIndex,
                                               int aLastSliceIndex,
                                               int anIncrementIndex)
//-------------------------------------------------------------------------
{
    loadSeries(aPattern.data(), aFirstSliceIndex, aLastSliceIndex, anIncrementIndex);
}


//-------------------------------------------------------------------------
template<typename T> void Image<T>::loadDicomSeries(const char* aDirectory)
//-------------------------------------------------------------------------
{
#ifdef HAS_ITK

    // Set some types
    typedef T   PixelType;
    const unsigned int Dimension = 3;

    typedef itk::Image< PixelType, Dimension >  ImageType;
    typedef itk::ImageSeriesReader< ImageType > ReaderType;
    typedef itk::GDCMSeriesFileNames InputNamesGeneratorType;

    // Create the file names
    InputNamesGeneratorType::Pointer p_name_generator(InputNamesGeneratorType::New());
    p_name_generator->SetInputDirectory(aDirectory);
    const typename ReaderType::FileNamesContainer& p_name_set(p_name_generator->GetInputFileNames());

    // Set the loader
    typename ReaderType::Pointer p_reader(ReaderType::New());
    p_reader->SetFileNames(p_name_set);
    p_reader->Update();

    // Get the image size
    typename ImageType::RegionType region(p_reader->GetOutput()->GetLargestPossibleRegion());
    typename ImageType::SizeType size(region.GetSize());

    m_width            = size[0];
    m_height           = size[1];
    m_number_of_slices = size[2];

    const typename ImageType::SpacingType spacing(p_reader->GetOutput()->GetSpacing());
    m_spacing[0] = spacing[0];
    m_spacing[1] = spacing[1];
    m_spacing[2] = spacing[2];

    // Release the memory if needed
    if (m_p_image)
    {
    	delete [] m_p_image;
    	m_p_image = 0;
	}

    // Allocate the memory
    int number_of_pixels(m_width * m_height * m_number_of_slices);
    m_p_image = new T[number_of_pixels];

    // Copy the pixel data
    std::copy(p_reader->GetOutput()->GetBufferPointer(),
            p_reader->GetOutput()->GetBufferPointer() + number_of_pixels,
            m_p_image);

#else
    throw Exception(__FILE__, __FUNCTION__, __LINE__,
            "ITK is not supported.");
#endif
}


//--------------------------------------------------------------------------------
template<typename T> void Image<T>::loadDicomSeries(const std::string& aDirectory)
//--------------------------------------------------------------------------------
{
    loadDicomSeries(aDirectory.data());
}


//-----------------------------------------------------------------
template<typename T> void Image<T>::loadTIFF(const char* aFileName)
//-----------------------------------------------------------------
{
#ifdef HAS_TIFF
    // Open the TIFF file
    TIFF* p_file = TIFFOpen(aFileName, "r");

    // The TIFF file is open
    if (p_file)
    {
        uint32_t cols = 0;
        uint32_t rows = 0;
        uint32_t image_depth = 0;
        uint16_t number_of_bits_per_sample = 0;

        // Set the sample format
        if (typeid(T) == typeid(char) || typeid(T) == typeid(short) || typeid(T) == typeid(int))
        {
            if (TIFFSetField(p_file, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_INT) != 1)
            {
                TIFFClose(p_file);
                std::string error_message("Could not set the sample format (INT) in the TIFF file (");
                error_message += aFileName;
                error_message += ")";
                throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
            }
        }
        else if (typeid(T) == typeid(unsigned char) || typeid(T) == typeid(unsigned short) || typeid(T) == typeid(unsigned int))
        {
            if (TIFFSetField(p_file, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT) != 1)
            {
                TIFFClose(p_file);
                std::string error_message("Could not set the sample format (UINT) in the TIFF file (");
                error_message += aFileName;
                error_message += ")";
                throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
            }
        }
        else if (typeid(T) == typeid(float) || typeid(T) == typeid(double))
        {
            if (TIFFSetField(p_file, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_IEEEFP) != 1)
            {
                TIFFClose(p_file);
                std::string error_message("Could not set the sample format (IEEEFP) in the TIFF file (");
                error_message += aFileName;
                error_message += ")";
                throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
            }
        }
        else
        {
            TIFFClose(p_file);
            throw Exception(__FILE__, __FUNCTION__, __LINE__,
                std::string("Unsupported data type in TIFF file \"") + aFileName + "\".");
        }

        // Set the number of bits per samples
        if (typeid(T) == typeid(char) || typeid(T) == typeid(unsigned char))
            number_of_bits_per_sample = 8;
        else if (typeid(T) == typeid(short) || typeid(T) == typeid(unsigned short))
            number_of_bits_per_sample = 16;
        else if (typeid(T) == typeid(float))
            number_of_bits_per_sample = 32;
        else if (typeid(T) == typeid(double))
            number_of_bits_per_sample = 64;

        // Set the number of bits per pixel
        if (TIFFSetField(p_file, TIFFTAG_BITSPERSAMPLE, number_of_bits_per_sample) != 1)
            {
                TIFFClose(p_file);
                std::string error_message("Could not set the number of bits per pixel in the TIFF file (");
                error_message += aFileName;
                error_message += ")";
                throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
            }

        // Set the number of samples per pixel, 1 for greyscale
        if (TIFFSetField(p_file, TIFFTAG_SAMPLESPERPIXEL, 1) != 1)
            {
                TIFFClose(p_file);
                std::string error_message("Could not set the number of bits per pixel in the TIFF file (");
                error_message += aFileName;
                error_message += ")";
                throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
            }

        // Retrieve information about the image
        TIFFGetField(p_file, TIFFTAG_IMAGEWIDTH, &cols);
        TIFFGetField(p_file, TIFFTAG_IMAGELENGTH, &rows);
        TIFFGetField(p_file, TIFFTAG_IMAGEDEPTH, &image_depth);

        // Allocate the image size
        m_width            = cols;
        m_height           = rows;
        m_number_of_slices = 1;

        // Release the memory if needed
        if (m_p_image)
        {
            delete [] m_p_image;
            m_p_image = 0;
        }

        // Allocate the memory
        int number_of_pixels(m_width * m_height * m_number_of_slices);
        m_p_image = new T[number_of_pixels];

        // Out of memory
        if (m_width && m_height && m_number_of_slices && !m_p_image)
        {
            TIFFClose(p_file);
            throw OutOfMemoryException(__FILE__, __FUNCTION__, __LINE__);
        }

        // Load the data line by line
        for (unsigned int row = 0; row < rows; row++)
        {
            TIFFReadScanline(p_file, m_p_image + row * cols, row);
        }

        // Close the TIFF file
        TIFFClose(p_file);
    }
    // The TIFF file is not open
    else
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
            std::string(aFileName) + " is not a valid TIFF file.");
    }
#else
    throw Exception(__FILE__, __FUNCTION__, __LINE__,
        "gVitualXray was compiled without TIFF support. "
        "Please recompile with LibTiff on.");
    #endif
}


//------------------------------------------------------------------------
template<typename T> void Image<T>::loadTIFF(const std::string& aFileName)
//------------------------------------------------------------------------
{
    loadTIFF(aFileName.data());
}


//------------------------------------------------------------------
template<typename T> void Image<T>::save(const char* aFileName,
                                         bool anInvertLUTFlag,
                                         const char* aComment,
                                         bool useDeflateCompressionIfPossible) const
//------------------------------------------------------------------
{
	bool image_saved = false;

    if (isMAT(aFileName) || isDAT(aFileName) || isTXT(aFileName))
    {
        saveASCII(aFileName);
        image_saved = true;
    }

	else if (isPGM(aFileName))
	{
		savePGM(aFileName);
        image_saved = true;
	}

	else if (isRAW(aFileName))
	{
		saveRAW(aFileName, useDeflateCompressionIfPossible);
        image_saved = true;
	}

	else if (isMHD(aFileName))
	{
		saveMHD(aFileName, useDeflateCompressionIfPossible);
        image_saved = true;
	}

	else if (isMHA(aFileName))
	{
		saveMHA(aFileName, useDeflateCompressionIfPossible);
        image_saved = true;
	}

#ifdef HAS_TIFF
    else if (isTIFF(aFileName))
    {
        saveTIFF(aFileName, anInvertLUTFlag);
        image_saved = true;
    }
#endif

#ifdef HAS_JPEG
    else if (isJPEG(aFileName))
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
                "Saving JPEG files is not implemented, "
                "please contact Dr FP Vidal.");
        image_saved = true;
    }
#endif

#ifdef HAS_GDCM
    else if (isDCM(aFileName))
    {
        saveDCM(aFileName, anInvertLUTFlag, aComment);
        image_saved = true;
    }
#endif

#ifdef HAS_ITK
    if (!image_saved)
    {
    	saveUsingITK(aFileName, useDeflateCompressionIfPossible);
        image_saved = true;
    }
#endif

    if (!image_saved)
    {
		throw Exception(__FILE__, __FUNCTION__, __LINE__,
				"Unknown file format, to get it added to the library, "
				"please contact Dr FP Vidal.");
	}
}


//-----------------------------------------------------------------------
template<typename T> void Image<T>::loadMHD(const std::string& aFileName)
//-----------------------------------------------------------------------
{
    loadMHD(aFileName.data());
}


//----------------------------------------------------------------
template<typename T> void Image<T>::loadMHD(const char* aFileName)
//----------------------------------------------------------------
{
    // Load the meta header
    loadMetaHeader(aFileName);
}


//-----------------------------------------------------------------------
template<typename T> void Image<T>::loadMHA(const std::string& aFileName)
//-----------------------------------------------------------------------
{
    loadMHA(aFileName.data());
}


//----------------------------------------------------------------
template<typename T> void Image<T>::loadMHA(const char* aFileName)
//----------------------------------------------------------------
{
    // Load the meta header
    loadMetaHeader(aFileName);
}


//-------------------------------------------------------------------------
template<typename T> void Image<T>::loadRAW(const char* aFileName,
        unsigned int aWidth,
        unsigned int aHeight,
        unsigned int aNumberOfSlices,
        bool aChangeEndianessFlag,
        const char* anElementType)
//------------------------------------------------------------------------
{
    // Release memory if any
    destroy();

    // Open the file
    std::ifstream input(aFileName, std::ios::binary|std::ios::in );

    // The file cannot be opened
    if (!input.is_open())
    {
        // Throw an error
        throw FileDoesNotExistException(__FILE__, __FUNCTION__, __LINE__, aFileName);
    }

    loadRAW(input, aWidth, aHeight, aNumberOfSlices, aChangeEndianessFlag, aFileName, anElementType);
}

//-----------------------------------------------------------------------
template<typename T> void Image<T>::loadRAW(const std::string& aFileName,
                                            unsigned int aWidth,
                                            unsigned int aHeight,
                                            unsigned int aNumberOfSlices,
                                            bool aChangeEndianessFlag,
                                            const std::string& anElementType)
//-----------------------------------------------------------------------
{
    loadRAW(aFileName.data(), aWidth, aHeight, aNumberOfSlices, aChangeEndianessFlag, anElementType.data());
}


//-------------------------------------------------------------------------
template<typename T> void Image<T>::save(const std::string& aFileName,
                                         bool anInvertLUTFlag,
                                         const std::string& aComment,
                                     bool useDeflateCompressionIfPossible) const
//-------------------------------------------------------------------------
{
    save(aFileName.data(), anInvertLUTFlag, aComment.data());
}


//----------------------------------------------
template<typename T> void Image<T>::savePGM(const char* aFileName) const
//----------------------------------------------
{
    // Open the file
    std::ofstream output_file(aFileName);

    // The file does not exist
    if (!output_file.is_open())
        {
        // Build the error message
        std::stringstream error_message;
        error_message << "Cannot create the file \"" << aFileName << "\"";

        // Throw an error
        throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
        }
    // The file is open
    else
        {
        // Get the min/max/range values
        T min_value(getMinValue());
        T max_value(getMaxValue());
        T range_value(max_value - min_value);

        // Set the image type
        output_file << "P2" << std::endl;

        // Print a comment
        output_file << "gVirtualXRay" << std::endl;

        // The image size
        output_file << m_width << " " << m_height * m_number_of_slices << std::endl;

        // The get the max value
        output_file << "65535" << std::endl;

        // Process every line
        for (unsigned int k = 0; k < m_number_of_slices; ++k)
        {
            for (unsigned int j = 0; j < m_height; ++j)
            {
                // Process every column
                for (unsigned int i = 0; i < m_width; ++i)
                {
                    // Process the pixel
                    int pixel_value;
                    if (typeid(T) == typeid(unsigned char))
                    {
                        pixel_value = m_p_image[k * m_width * m_height + j * m_width + i];
                    }
                    else
                    {
                        pixel_value = 65535.0 * (m_p_image[k * m_width * m_height + j * m_width + i] - min_value) / range_value;
                    }

					pixel_value = std::max(0, pixel_value);
                    pixel_value = std::min(65535, pixel_value);

					output_file << pixel_value;

                    // It is not the last pixel of the line
                    if (i < (m_width - 1))
                    {
                        output_file << " ";
                    }
                }

                // It is not the last line of the image
                if (j < (m_height - 1))
                {
                    output_file << std::endl;
                }
            }
        }
    }
}


//-----------------------------------------------------
template<typename T> void Image<T>::savePGM(const std::string& aFileName) const
//-----------------------------------------------------
{
    savePGM(aFileName.data());
}


//----------------------------------------------
template<typename T> void Image<T>::saveRaw(const char* aFileName, bool useDeflateCompressionIfPossible) const
//----------------------------------------------
{
    // Open the file in binary
	if (!useDeflateCompressionIfPossible)
	{
		std::ofstream output_file (aFileName, std::ifstream::binary);

		// The file is not open
		if (!output_file.is_open())
			{
			std::string error_message("The file (");
			error_message += aFileName;
			error_message += ") cannot be created";

			throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
			}

		// Write content to file
		output_file.write(reinterpret_cast<char*>(m_p_image), m_width * m_height * m_number_of_slices * sizeof(T));
	}
	else
	{
		gzFile output_file = gzopen(aFileName,"wb");

		// The file is not open
		if (!output_file)
			{
			std::string error_message("The file (");
			error_message += aFileName;
			error_message += ") cannot be created";

			throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
			}

		int write_size = gzwrite( output_file, reinterpret_cast<char*>(m_p_image), m_width * m_height * m_number_of_slices * sizeof(T));
		int close_err  = gzclose( output_file );

		if (write_size == 0)
			{
			std::string error_message("The file (");
			error_message += aFileName;
			error_message += ") cannot be written";

			throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
			}

		if (close_err == Z_STREAM_ERROR)
			{
			std::string error_message("The file (");
			error_message += aFileName;
			error_message += ") is not valid";

			throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
			}

		if (close_err == Z_ERRNO)
			{
			std::string error_message("File operation error on the file (");
			error_message += aFileName;
			error_message += ").";

			throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
			}

		if (close_err == Z_MEM_ERROR)
			{
			std::string error_message("File operation error on the file (");
			error_message += aFileName;
			error_message += ").";

			throw OutOfMemoryException(__FILE__, __FUNCTION__, __LINE__);
			}

		if (close_err == Z_BUF_ERROR)
			{
			std::string error_message("The last read ended in the middle of a gzip stream when writing the file (");
			error_message += aFileName;
			error_message += ").";

			throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
			}

		if (close_err != Z_OK)
			{
			std::string error_message("Unspecified error when writing the file (");
			error_message += aFileName;
			error_message += ").";

			throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
			}
	}
}


//----------------------------------------------
template<typename T> void Image<T>::saveRAW(const char* aFileName, bool useDeflateCompressionIfPossible) const
//----------------------------------------------
{
    saveRaw(aFileName, useDeflateCompressionIfPossible);
}


//-----------------------------------------------------
template<typename T> void Image<T>::saveRaw(const std::string& aFileName,
bool useDeflateCompressionIfPossible) const
//-----------------------------------------------------
{
    saveRaw(aFileName.data(), useDeflateCompressionIfPossible);
}


//-----------------------------------------------------
template<typename T> void Image<T>::saveRAW(const std::string& aFileName, bool useDeflateCompressionIfPossible) const
//-----------------------------------------------------
{
    saveRaw(aFileName, useDeflateCompressionIfPossible);
}


//------------------------------------------------------------------------
template<typename T> void Image<T>::saveASCII(const char* aFileName) const
//------------------------------------------------------------------------
{
    // Open the file
    std::ofstream output_file (aFileName);

    // The file is not open
    if (!output_file.is_open())
        {
        std::string error_message("The file (");
        error_message += aFileName;
        error_message += ") cannot be created";

        throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
        }

    // Write content to file
    for (unsigned int k = 0; k < m_number_of_slices; ++k)
        {
        for (unsigned int j(0); j < m_height; ++j)
            {
            for (unsigned int i(0); i < m_width; ++i)
                {
                output_file << getPixel(i, j, k);

                // This is not the last pixel of the line
                if (i < m_width - 1)
                    {
                    output_file << " ";
                    }
                }

            // This is not the last line
            if (j < m_height - 1 || k < m_number_of_slices - 1)
                {
                output_file << std::endl;
                }
            }
        }
}


//-------------------------------------------------------
template<typename T> void Image<T>::saveASCII(const std::string& aFileName) const
//-------------------------------------------------------
{
    saveASCII(aFileName.data());
}


//----------------------------------------------------------------------
template<typename T> void Image<T>::saveMHD(const char* aFileName, bool useDeflateCompressionIfPossible) const
//----------------------------------------------------------------------
{
    // Save the header
    saveMetaHeader(aFileName, useDeflateCompressionIfPossible);
}


template<typename T> void Image<T>::saveMHD(const std::string& aFileName, bool useDeflateCompressionIfPossible) const
{
    saveMHD(aFileName.data(), useDeflateCompressionIfPossible);
}


template<typename T> void Image<T>::saveMHA(const char* aFileName, bool useDeflateCompressionIfPossible) const
{
    saveMetaHeader(aFileName, useDeflateCompressionIfPossible);
}


template<typename T> void Image<T>::saveMHA(const std::string& aFileName, bool useDeflateCompressionIfPossible) const
{
    saveMHA(aFileName.data(), useDeflateCompressionIfPossible);
}


//---------------------------------------------------------------------
template<typename T> void Image<T>::saveDCM(const char* aFileName,
                                            bool anInvertLUTFlag,
                                            const char* aComment) const
//---------------------------------------------------------------------
{
#ifdef HAS_GDCM
    T min_input_value(getMinValue());
    T max_input_value(getMaxValue());
    T difference_input(max_input_value - min_input_value);

    unsigned short min_output_value(((gdcm::PixelFormat)gdcm::PixelFormat::UINT16).GetMin());
    unsigned short max_output_value(((gdcm::PixelFormat)gdcm::PixelFormat::UINT16).GetMax());
    unsigned short difference_output(max_output_value - min_output_value);

    double shift(min_input_value);
    double scale(double(max_input_value - min_input_value) / max_output_value);

    if (m_number_of_slices == 1)
        {
        // Write the modified DataSet back to disk
        gdcm::ImageWriter writer;
        //writer.CheckFileMetaInformationOff(); // Do not attempt to reconstruct the file meta to preserve the file
                                              // as close to the original as possible.

        // Create the image (GDCM format)
        gdcm::Image& gdcm_image(writer.GetImage());

        // Set the size of the image
        unsigned int dims[3] = {};
        dims[0] = m_width;
        dims[1] = m_height;

        double spacing[3] = {};
        spacing[0] = m_spacing[0];
        spacing[1] = m_spacing[1];

        if (m_number_of_slices == 1)
            {
            gdcm_image.SetNumberOfDimensions(2);
            }
        else
            {
            dims[2] = m_number_of_slices;
            gdcm_image.SetNumberOfDimensions(3);

            spacing[2] = 1;//m_spacing[2];
            }

        gdcm_image.SetSpacing (spacing);

        gdcm_image.SetDimensions( dims );


        unsigned short* p_temp(new unsigned short[m_width * m_height * m_number_of_slices]);

#ifdef NDEBUG
        #pragma omp parallel for
#endif
#ifdef WIN32
        for (int i = 0; i < m_width * m_height * m_number_of_slices; ++i)
#else
        for (size_t i = 0; i < m_width * m_height * m_number_of_slices; ++i)
#endif
            {
            p_temp[i] = min_output_value + difference_output * (double(m_p_image[i] - min_input_value) / double(difference_input));
            }

        unsigned int input_length(m_width * m_height * m_number_of_slices * sizeof(T));
        unsigned int output_length(m_width * m_height * m_number_of_slices * sizeof(unsigned short));

        gdcm::PixelFormat input_pixel_format(gdcm::PixelFormat::UINT16);
        input_pixel_format.SetSamplesPerPixel(1);

        gdcm_image.SetSlope(scale);
        gdcm_image.SetIntercept(shift);

        //gdcm_image.SetPixelFormat(pixel_format.ComputeInterceptSlopePixelType());
        gdcm_image.SetPixelFormat(input_pixel_format);
        gdcm::PhotometricInterpretation photometric_interpretation;

        if (anInvertLUTFlag)
            {
            photometric_interpretation = gdcm::PhotometricInterpretation::MONOCHROME2;
            }
        else
            {
            photometric_interpretation = gdcm::PhotometricInterpretation::MONOCHROME1;
            }
        gdcm_image.SetPhotometricInterpretation(photometric_interpretation);
        gdcm_image.SetTransferSyntax(gdcm::TransferSyntax::ExplicitVRLittleEndian);



        // Disable compression
        gdcm_image.SetLossyFlag(false);


        // Window / Level
        addTag(difference_input,
                0x0028, 0x1051,
                writer.GetFile());

        addTag(min_input_value + difference_input / 2.0,
                0x0028,0x1050,
                writer.GetFile());

        // Study description
        addTag("X-ray simulation",
                0x0008,0x1030,
                writer.GetFile());

        // Patient
        addTag("Patient",
                0x0010,0x0010,
                writer.GetFile());

        // Modality
        addTag(aComment,
                0x0008,0x0060,
                writer.GetFile());

        // Manufacturer
        addTag("gVirtualXRay",
                0x0008,0x0070,
                writer.GetFile());

        // InstitutionName
        addTag("Bangor University",
                0x0008,0x0080,
                writer.GetFile());

        // InstitutionAddress
        addTag("http://gvirtualxray.sourceforge.net/",
                0x0008,0x0081,
                writer.GetFile());

        // DICOM fields for X-rays:
        // See http://dicomlookup.com/lookup.asp?sw=Ttable&q=C.8-27


        // Set pixel data
        gdcm::DataElement pixel_data(gdcm::Tag(0x7fe0,0x0010));
        pixel_data.SetByteValue((const char*)p_temp, uint32_t(output_length));
        gdcm_image.SetDataElement(pixel_data);

        // Set the file name
        writer.SetFileName( aFileName );

        // Save the data
        if( !writer.Write() )
            {
            delete [] p_temp;
            p_temp = 0;

            std::string error_message("Could not write the DICOM file (");
            error_message += aFileName;
            error_message += ")";
            throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
            }
        delete [] p_temp;
        p_temp = 0;
    }

#else

    throw Exception(__FILE__, __FUNCTION__, __LINE__,
            "Saving DICOM files is not implemented, "
            "please contact Dr FP Vidal.");

#endif
}


//----------------------------------------------------------------------------
template<typename T> void Image<T>::saveDCM(const std::string& aFileName,
                                            bool anInvertLUTFlag,
                                            const std::string& aComment) const
//----------------------------------------------------------------------------
{
    saveDCM(aFileName.data(), anInvertLUTFlag, aComment.data());
}


//----------------------------------------------------------------------
template<typename T> void Image<T>::saveTIFF(const char* aFileName,
                                             bool anInvertLUTFlag) const
//----------------------------------------------------------------------
{
#ifdef HAS_TIFF

    float* p_float_data(nullptr);
    unsigned int number_of_pixels(m_width * m_height);


    TIFF_UINT16_T number_of_bits_per_sample(0);

    if (typeid(T) == typeid(char) || typeid(T) == typeid(unsigned char))
        {
        number_of_bits_per_sample = 8;
        }
    else if (typeid(T) == typeid(short) || typeid(T) == typeid(unsigned short))
        {
        number_of_bits_per_sample = 16;
        }
    else if (typeid(T) == typeid(float))
        {
        number_of_bits_per_sample = 32;
        }
    else
        {
        number_of_bits_per_sample = 32;
        p_float_data = new float[number_of_pixels];
        }

    // Open the file
    TIFF* p_file = TIFFOpen(aFileName, "w");

    // The file is not open
    if (!p_file)
        {
        std::string error_message("The file (");
        error_message += aFileName;
        error_message += ") cannot be created";

        throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
        }

    // Write all the slices
    for (unsigned int k(0); k < m_number_of_slices; k++)
        {
        // Get the min/max/range values
        T min_value(getMinValue(k));
        T max_value(getMaxValue(k));
        T value_range(max_value - min_value);

        // Convert the image if necessary
        if (p_float_data)
            {
#ifdef NDEBUG
        #pragma omp parallel for
#endif
            for (int i = 0; i < int(number_of_pixels); ++i)
                {
                float temp(double(m_p_image[k * m_width * m_height + i] - min_value) / value_range);
                p_float_data[i] = temp;
                }
            }

        // Create a new directory
        if (m_number_of_slices > 1)
            {
            if (TIFFWriteDirectory(p_file) != 1)
                {
                    std::string error_message("Could not write the new directory in the TIFF file (");
                    error_message += aFileName;
                    error_message += ")";
                    throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
                }
            }

        // Set the image width
        TIFF_UINT32_T image_width(m_width);

        if (TIFFSetField(p_file, TIFFTAG_IMAGEWIDTH, image_width) != 1)
            {
                std::string error_message("Could not set the image width in the TIFF file (");
                error_message += aFileName;
                error_message += ")";
                throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
            }

        // Set the image height
        TIFF_UINT32_T image_height(m_height);

        if (TIFFSetField(p_file, TIFFTAG_IMAGELENGTH, image_height) != 1)
            {
                std::string error_message("Could not set the image height in the TIFF file (");
                error_message += aFileName;
                error_message += ")";
                throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
            }

        // Set the number of bits per pixel
        if (TIFFSetField(p_file, TIFFTAG_BITSPERSAMPLE, number_of_bits_per_sample) != 1)
            {
                std::string error_message("Could not set the number of bits per pixel in the TIFF file (");
                error_message += aFileName;
                error_message += ")";
                throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
            }

        // Set the compression
        TIFF_UINT16_T compression(COMPRESSION_LZW);

        if (TIFFSetField(p_file, TIFFTAG_COMPRESSION, compression) != 1)
            {
                std::string error_message("Could not set the compression in the TIFF file (");
                error_message += aFileName;
                error_message += ")";
                throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
            }

        // Set the photometric interpretation
        TIFF_UINT16_T photometric_interpretation;
            if (anInvertLUTFlag)
                {
                photometric_interpretation = PHOTOMETRIC_MINISWHITE;
                }
            else
                {
                photometric_interpretation = PHOTOMETRIC_MINISBLACK;
                }

        if (TIFFSetField(p_file, TIFFTAG_PHOTOMETRIC, photometric_interpretation) != 1)
            {
                std::string error_message("Could not set the photometric interpretation in the TIFF file (");
                error_message += aFileName;
                error_message += ")";
                throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
            }

        // Set the orientation
        TIFF_UINT16_T orientation(ORIENTATION_TOPLEFT);

        if (TIFFSetField(p_file, TIFFTAG_ORIENTATION, orientation) != 1)
            {
                std::string error_message("Could not set the orientation in the TIFF file (");
                error_message += aFileName;
                error_message += ")";
                throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
            }

        // Set the number of samples per pixel
        TIFF_UINT16_T number_of_samples_per_pixel(1);

        if (TIFFSetField(p_file, TIFFTAG_SAMPLESPERPIXEL, number_of_samples_per_pixel) != 1)
            {
                std::string error_message("Could not set the number of samples per pixel in the TIFF file (");
                error_message += aFileName;
                error_message += ")";
                throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
            }

        // Set the min sample value
        if (TIFFSetField(p_file, TIFFTAG_MINSAMPLEVALUE, min_value) != 1)
            {
                std::string error_message("Could not set the min sample value in the TIFF file (");
                error_message += aFileName;
                error_message += ")";
                throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
            }

        // Set the max sample value
        if (TIFFSetField(p_file, TIFFTAG_MAXSAMPLEVALUE, max_value) != 1)
            {
                std::string error_message("Could not set the max sample value in the TIFF file (");
                error_message += aFileName;
                error_message += ")";
                throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
            }

        // Set the planar configuration
        TIFF_UINT16_T planar_configuration(PLANARCONFIG_CONTIG);

        if (TIFFSetField(p_file, TIFFTAG_PLANARCONFIG, planar_configuration) != 1)
            {
                std::string error_message("Could not set the planar configuration in the TIFF file (");
                error_message += aFileName;
                error_message += ")";
                throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
            }

    std::string software;
    software += "gVirtualXRay-";
    software += gVirtualXRay_VERSION_MAJOR;
    software += ".";
    software += gVirtualXRay_VERSION_MINOR;
    software += ".";
    software += gVirtualXRay_VERSION_PATCH;

    if (TIFFSetField(p_file, TIFFTAG_COPYRIGHT, software.data()) != 1)
        {
            std::string error_message("Could not set the software value in the TIFF file (");
            error_message += aFileName;
            error_message += ")";
            throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
        }

    // Set the format of a sample
    TIFF_UINT16_T format_of_a_sample;

    if (typeid(T) == typeid(unsigned char) || typeid(T) == typeid(unsigned short))
        {
        format_of_a_sample = SAMPLEFORMAT_UINT;
        }
    else if (typeid(T) == typeid(char) || typeid(T) == typeid(short))
        {
        format_of_a_sample = SAMPLEFORMAT_INT;
        }
    else
        {
        format_of_a_sample = SAMPLEFORMAT_IEEEFP;
        }

    if (TIFFSetField(p_file, TIFFTAG_SAMPLEFORMAT, format_of_a_sample) != 1)
        {
            std::string error_message("Could not set the compression in the TIFF file (");
            error_message += aFileName;
            error_message += ")";
            throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
        }

    // Length in memory of one row of pixel in the image
    tsize_t number_of_bytes_per_line(number_of_samples_per_pixel * m_width);

    // We set the strip size of the file to be size of one row of pixels
    TIFFSetField(p_file, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(p_file, number_of_bytes_per_line));


        // Write the image data, line by line
        int error_code(0);
        for (unsigned int j(0); j < m_height; ++j)
            {
            if (p_float_data)
                {
                error_code = TIFFWriteScanline(p_file, p_float_data + m_width * j, j, 0);
                }
            else
                {
                error_code = TIFFWriteScanline(p_file, m_p_image + m_width * m_height * k + m_width * j, j, 0);
                }

            if (error_code != 1)
                {
                // Close the file
                TIFFClose(p_file);

                // Release the memory if necessary
                if (p_float_data)
                    {
                    delete [] p_float_data;
                    p_float_data = nullptr;
                    }

                // Generate an error
                std::stringstream error_message;
                error_message << "Could not write the pixel data (Line " << j << ") in the TIFF file (";
                error_message << aFileName;
                error_message << ")";
                throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
                }
            }

        // Flush the data in the TIFF file
        if (m_number_of_slices > 1)
            {
            if (TIFFFlush(p_file) != 1)
                {
                    std::string error_message("Could not flush the data in the TIFF file (");
                    error_message += aFileName;
                    error_message += ")";
                    throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
                }
            }
        }

    // Release the memory if necessary
    if (p_float_data)
        {
        delete [] p_float_data;
        p_float_data = nullptr;
        }

    // Close the file
    TIFFClose(p_file);

#else

    throw Exception(__FILE__, __FUNCTION__, __LINE__,
            "Saving TIFF files is not implemented, "
            "please contact Dr FP Vidal.");

#endif
}


//------------------------------------------------------------------------
template<typename T> void Image<T>::saveTIFF(const std::string& aFileName,
                                             bool anInvertLUTFlag) const
//------------------------------------------------------------------------
{
    saveTIFF(aFileName.data(), anInvertLUTFlag);
}


//------------------------------------------------------------------------------------------
template<typename T> void Image<T>::saveUsingITK(const char* aFileName, 
                                                 bool useDeflateCompressionIfPossible) const
//------------------------------------------------------------------------------------------
{
#ifdef HAS_ITK
	// 3D volume
	if (m_number_of_slices > 1 || isDCM(aFileName))
	{
		typedef itk::Image< T, 3 >  InputImageType;

		typename InputImageType::RegionType region;
		typename InputImageType::IndexType start;
		start[0] = 0;
		start[1] = 0;
		start[2] = 0;

		typename InputImageType::SizeType size;
		size[0] = m_width;
		size[1] = m_height;
		size[2] = m_number_of_slices;

		region.SetSize(size);
		region.SetIndex(start);

		typename InputImageType::Pointer p_image(InputImageType::New());
		p_image->SetRegions(region);
		p_image->Allocate();


		typename InputImageType::SpacingType spacing;
		spacing[0] = m_spacing[0];
		spacing[1] = m_spacing[1];
		spacing[2] = m_spacing[2];
		p_image->SetSpacing(spacing);

		// Copy the pixel data
		int number_of_pixels(m_width * m_height * m_number_of_slices);
		std::copy(m_p_image,
				m_p_image + number_of_pixels,
				p_image->GetBufferPointer());

		// Save the data in SHORT
		if (isDCM(aFileName))
		{
			typedef itk::Image< short, 3 >  ShortImageType;
			typedef itk::ImageFileWriter<   ShortImageType > ShortWriterType;

			typedef itk::RescaleIntensityImageFilter< InputImageType, ShortImageType > RescaleType;
			typename RescaleType::Pointer rescale = RescaleType::New();
			rescale->SetInput( p_image );
			rescale->Update();

			typename ShortWriterType::Pointer p_writer(ShortWriterType::New());
			p_writer->SetFileName( aFileName );
			p_writer->SetInput( rescale->GetOutput() );
			p_writer->SetUseCompression(useDeflateCompressionIfPossible);
			p_writer->Update();
		}
		// Save the data in its native format
		else
		{
			typedef itk::ImageFileWriter< InputImageType > InputWriterType;
			typename InputWriterType::Pointer p_writer(InputWriterType::New());
			p_writer->SetFileName( aFileName );
			p_writer->SetInput( p_image );
			p_writer->SetUseCompression(useDeflateCompressionIfPossible);
			p_writer->Update();
		}
	}
	// 2D image
	else
	{
		typedef itk::Image< T, 2 >  InputImageType;

		typename InputImageType::RegionType region;
		typename InputImageType::IndexType start;
		start[0] = 0;
		start[1] = 0;

		typename InputImageType::SizeType size;
		size[0] = m_width;
		size[1] = m_height;

		region.SetSize(size);
		region.SetIndex(start);

		typename InputImageType::Pointer p_image(InputImageType::New());
		p_image->SetRegions(region);
		p_image->Allocate();


		typename InputImageType::SpacingType spacing;
		spacing[0] = m_spacing[0];
		spacing[1] = m_spacing[1];
		p_image->SetSpacing(spacing);

		// Copy the pixel data
		int number_of_pixels(m_width * m_height);
		std::copy(m_p_image,
				m_p_image + number_of_pixels,
				p_image->GetBufferPointer());


		// Save the data in UCHAR
		if (isJPEG(aFileName) ||
				checkExtension(aFileName, "PNG") ||
				checkExtension(aFileName, "BMP"))
		{
			typedef itk::Image< unsigned char, 2 >  UByteImageType;
			typedef itk::ImageFileWriter<   UByteImageType > UByteWriterType;

			typedef itk::RescaleIntensityImageFilter< InputImageType, UByteImageType > RescaleType;
			typename RescaleType::Pointer rescale = RescaleType::New();
			rescale->SetInput( p_image );
			rescale->SetOutputMinimum( 0 );
			rescale->SetOutputMaximum( 255 );
			rescale->Update();

			typename UByteWriterType::Pointer p_writer(UByteWriterType::New());
			p_writer->SetFileName( aFileName );
			p_writer->SetInput( rescale->GetOutput() );
			p_writer->SetUseCompression(useDeflateCompressionIfPossible);
			p_writer->Update();
		}
		// Save the data in its native format
		else
		{
			typedef itk::ImageFileWriter<   InputImageType > InputWriterType;
			typename InputWriterType::Pointer p_writer(InputWriterType::New());
			p_writer->SetFileName( aFileName );
			p_writer->SetInput( p_image );
			p_writer->SetUseCompression(useDeflateCompressionIfPossible);
			p_writer->Update();
		}
	}
#else
    throw Exception(__FILE__, __FUNCTION__, __LINE__,
            "ITK is not supported.");
#endif
}


//------------------------------------------------------------------------------------------
template<typename T> void Image<T>::saveUsingITK(const std::string& aFileName,
                                                 bool useDeflateCompressionIfPossible) const
//------------------------------------------------------------------------------------------
{
    saveUsingITK(aFileName.data(), useDeflateCompressionIfPossible);
}


//------------------------------------------------
template<typename T> bool Image<T>::operator==(const Image<T>& anImage) const
//------------------------------------------------
{
    if (m_width != anImage.m_width)
        {
        return (false);
        }

    if (m_height != anImage.m_height)
        {
        return (false);
        }

    if (m_number_of_slices != anImage.m_number_of_slices)
        {
        return (false);
        }

    T const * p_data1(m_p_image);
    T const * p_data2(anImage.m_p_image);
    for (unsigned int i(0); i < m_width * m_height * m_number_of_slices; ++i)
        {
        if (std::abs(*p_data1++ - *p_data2++) > EPSILON)
            {
            return (false);
            }
        }

    return (true);
}


//------------------------------------------------
template<typename T> bool Image<T>::operator!=(const Image<T>& anImage) const
//------------------------------------------------
{
    return (!(operator==(anImage)));
}


//--------------------------
template<typename T> double Image<T>::getSum() const
//--------------------------
{
    // The image is empty
    if (!m_p_image)
        {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Empty image");
        }

    return (std::accumulate(&m_p_image[0],
                &m_p_image[m_width * m_height * m_number_of_slices],
                0.0));
}


//------------------------------
template<typename T> double Image<T>::getAverage() const
//------------------------------
{
    return (getSum() / (m_width * m_height));
}


//-------------------------------
template<typename T> double Image<T>::getVariance() const
//-------------------------------
{
    double variance(0.0);
    double average(getAverage());

#ifdef NDEBUG
    #pragma omp parallel for reduction( + : variance )
#endif
#ifdef WIN32
    for (int i = 0; i < m_width * m_height * m_number_of_slices; ++i)
#else
    for (size_t i = 0; i < m_width * m_height * m_number_of_slices; ++i)
#endif
        {
        variance += std::pow(m_p_image[i] - average, 2);
        }

    return (variance / (m_width * m_height));
}


//----------------------------------------
template<typename T> double Image<T>::getStandardDeviation() const
//----------------------------------------
{
    return (std::sqrt(getVariance()));
}


//-----------------------------------------------------------------------------
template<typename T> double Image<T>::computeSAE(const Image<T>& anImage) const
//-----------------------------------------------------------------------------
{
    if (m_width != anImage.m_width || m_height != anImage.m_height || m_number_of_slices != anImage.m_number_of_slices)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "The images have different sizes");
    }

    double sae(0.0);

#ifdef NDEBUG
    #pragma omp parallel for reduction( + : sae )
#endif
#ifdef WIN32
    for (int i = 0; i < m_width * m_height * m_number_of_slices; ++i)
#else
    for (size_t i = 0; i < m_width * m_height * m_number_of_slices; ++i)
#endif
    {
        sae += std::abs(m_p_image[i] - anImage.m_p_image[i]);
    }

    return (sae);
}


//-----------------------------------------------------------------------------
template<typename T> double Image<T>::computeMAE(const Image<T>& anImage) const
//-----------------------------------------------------------------------------
{
    return (computeSAE(anImage) / double(m_width * m_height * m_number_of_slices));
}


//-----------------------------------------------------------------------------
template<typename T> double Image<T>::computeSSE(const Image<T>& anImage) const
//-----------------------------------------------------------------------------
{
    if (m_width != anImage.m_width || m_height != anImage.m_height || m_number_of_slices != anImage.m_number_of_slices)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "The images have different sizes");
    }

    double sse(0.0);
    double temp;

#ifdef NDEBUG
    #pragma omp parallel for reduction( + : sse ) private(temp)
#endif
#ifdef WIN32
    for (int i = 0; i < m_width * m_height * m_number_of_slices; ++i)
#else
    for (size_t i = 0; i < m_width * m_height * m_number_of_slices; ++i)
#endif
    {
        temp = m_p_image[i] - anImage.m_p_image[i];
        temp *= temp;

        sse += temp;
    }

    return (sse);
}


//-----------------------------------------------------------------------------
template<typename T> double Image<T>::computeMSE(const Image<T>& anImage) const
//-----------------------------------------------------------------------------
{
    return (computeSSE(anImage) / double(m_width * m_height * m_number_of_slices));
}


//------------------------------------------------------------------------------
template<typename T> double Image<T>::computeRMSE(const Image<T>& anImage) const
//------------------------------------------------------------------------------
{
    return (std::sqrt(computeMSE(anImage) / double(m_width * m_height * m_number_of_slices)));
}


//--------------------------------------------
template<typename T> double Image<T>::computeNCC(const Image<T>& anImage) const
//--------------------------------------------
{
    if (m_width != anImage.m_width || m_height != anImage.m_height || m_number_of_slices != anImage.m_number_of_slices)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "The images have different sizes");
    }

    double average1(getAverage());
    double average2(anImage.getAverage());

    double standard_deviation_product(getStandardDeviation() * anImage.getStandardDeviation());
    double ncc(0.0);

#ifdef NDEBUG
    #pragma omp parallel for reduction( + : ncc )
#endif
#ifdef WIN32
    for (int i = 0; i < m_width * m_height * m_number_of_slices; ++i)
#else
    for (size_t i = 0; i < m_width * m_height * m_number_of_slices; ++i)
#endif
    {
        ncc += (m_p_image[i] - average1) * (anImage.m_p_image[i] - average2);
    }

    return (ncc / (m_width * m_height * standard_deviation_product));
}



template<typename T> FFT<T> Image<T>::getFFT() const
{
    return (FFT<T>::computeFFT(*this));
}


template<typename T> Image<T> Image<T>::transpose() const
{
    Image<T> temp(m_height, m_width, m_number_of_slices);
    temp.m_spacing = m_spacing; // Debatable

#ifdef NDEBUG
#ifdef WIN32
        #pragma omp parallel for
#else
        #pragma omp parallel for collapse(3)
#endif
#endif
    for (int k = 0; k < m_number_of_slices; ++k)
    {
        for (int j = 0; j < m_height; ++j)
        {
            for (int i = 0; i < m_width; ++i)
            {
                temp(j, i, k) = getPixel(i, j, k);
            }
        }
    }

    return (temp);
}


#ifdef HAS_GDCM
//-----------------------------------------------------------------
template<typename T> void Image<T>::addTag(double aValue,
                                           unsigned int anAddress1,
                                           unsigned int anAddress2,
                                           gdcm::File& aFile) const
//-----------------------------------------------------------------
{
    std::stringstream message;
    message << aValue;

    addTag(message.str(), anAddress1, anAddress2, aFile);
}


//-----------------------------------------------------------------
template<typename T> void Image<T>::addTag(const char* aValue,
                                           unsigned int anAddress1,
                                           unsigned int anAddress2,
                                           gdcm::File& aFile) const
//-----------------------------------------------------------------
{
    std::stringstream message;
    message << aValue;

    addTag(std::string(aValue), anAddress1, anAddress2, aFile);
}


//-------------------------------------------------------------------
template<typename T> void Image<T>::addTag(const std::string& aValue,
                                           unsigned int anAddress1,
                                           unsigned int anAddress2,
                                           gdcm::File& aFile) const
//-------------------------------------------------------------------
{
    gdcm::DataElement data_element(gdcm::Tag(anAddress1, anAddress2));


    data_element.SetByteValue((const char*)aValue.data(), uint32_t(aValue.size()));

    aFile.GetDataSet().Insert(data_element);
}
#endif


//------------------------------------------------------------------------------
template<typename T> Image<T> Image<T>::operator+(const Image<T>& anImage) const
//------------------------------------------------------------------------------
{
    // Deal with images of different sizes
    int min_width( std::min(m_width,  anImage.m_width));
    int min_height(std::min(m_height, anImage.m_height));
    int min_depth( std::min(m_number_of_slices,  anImage.m_number_of_slices));

    // Copy the instance into a temporary variable
    Image<T> temp(getROI(0, 0, 0, min_width, min_height, min_depth));
    temp.m_spacing = m_spacing;

    // Copy the data
#ifdef NDEBUG
#ifdef WIN32
        #pragma omp parallel for
#else
        #pragma omp parallel for collapse(3)
#endif
#endif
    for (int k = 0; k < min_depth; ++k)
        {
        for (int j = 0; j < min_height; ++j)
            {
            for (int i = 0; i < min_width; ++i)
                {
                temp(i, j, k) += anImage(i, j, k);
                }
            }
        }

    // Return the result
    return (temp);
}


//------------------------------------------------------------------------------
template<typename T> Image<T> Image<T>::operator-(const Image<T>& anImage) const
//------------------------------------------------------------------------------
{
    // Deal with images of different sizes
    int min_width( std::min(m_width,  anImage.m_width));
    int min_height(std::min(m_height, anImage.m_height));
    int min_depth( std::min(m_number_of_slices,  anImage.m_number_of_slices));

    // Copy the instance into a temporary variable
    Image<T> temp(getROI(0, 0, 0, min_width, min_height, min_depth));
    temp.m_spacing = m_spacing;

    // Copy the data
#ifdef NDEBUG
#ifdef WIN32
        #pragma omp parallel for
#else
        #pragma omp parallel for collapse(3)
#endif
#endif
    for (int k = 0; k < min_depth; ++k)
        {
        for (int j = 0; j < min_height; ++j)
            {
            for (int i = 0; i < min_width; ++i)
                {
                temp(i, j, k) -= anImage(i, j, k);
                }
            }
        }

    // Return the result
    return (temp);
}


//------------------------------------------------------------------------------
template<typename T> Image<T> Image<T>::operator*(const Image<T>& anImage) const
//------------------------------------------------------------------------------
{
    // Deal with images of different sizes
    int min_width( std::min(m_width,  anImage.m_width));
    int min_height(std::min(m_height, anImage.m_height));
    int min_depth( std::min(m_number_of_slices,  anImage.m_number_of_slices));

    // Copy the instance into a temporary variable
    Image<T> temp(getROI(0, 0, 0, min_width, min_height, min_depth));
    temp.m_spacing = m_spacing;

    // Copy the data
#ifdef NDEBUG
#ifdef WIN32
        #pragma omp parallel for
#else
        #pragma omp parallel for collapse(3)
#endif
#endif
	for (int k = 0; k < min_depth; ++k)
        {
        for (int j = 0; j < min_height; ++j)
            {
            for (int i = 0; i < min_width; ++i)
                {
                temp(i, j, k) *= anImage(i, j, k);
                }
            }
        }

    // Return the result
    return (temp);
}


//------------------------------------------------------------------------------
template<typename T> Image<T> Image<T>::operator/(const Image<T>& anImage) const
//------------------------------------------------------------------------------
{
    // Deal with images of different sizes
    unsigned int min_width( std::min(m_width,  anImage.m_width));
    unsigned int min_height(std::min(m_height, anImage.m_height));
    unsigned int min_depth( std::min(m_number_of_slices,  anImage.m_number_of_slices));

    // Copy the instance into a temporary variable
    Image<T> temp(getROI(0, 0, 0, min_width, min_height, min_depth));
    temp.m_spacing = m_spacing;

    // Copy the data
    // #pragma omp parallel for collapse(3)
    // Not parallel due to the exception
    for (unsigned int k = 0; k < min_depth; ++k)
        {
        for (unsigned int j = 0; j < min_height; ++j)
            {
            for (unsigned int i = 0; i < min_width; ++i)
                {
                double pixel_value(anImage(i, j, k));

                if (std::abs(pixel_value) < EPSILON)
                    {
                    throw Exception(__FILE__, __FUNCTION__, __LINE__, "Division by zero.");
                    }
                else
                    {
                    temp(i, j, k) /= pixel_value;
                    }
                }
            }
        }

    // Return the result
    return (temp);
}


//--------------------------------------------------------------------------
template<typename T> Image<T>& Image<T>::operator+=(const Image<T>& anImage)
//--------------------------------------------------------------------------
{
    // Re-use operator+
    *this = *this + anImage;

    // Return the result
    return (*this);
}


//--------------------------------------------------------------------------
template<typename T> Image<T>& Image<T>::operator-=(const Image<T>& anImage)
//--------------------------------------------------------------------------
{
    // Re-use operator-
    *this = *this - anImage;

    // Return the result
    return (*this);
}


//--------------------------------------------------------------------------
template<typename T> Image<T>& Image<T>::operator*=(const Image<T>& anImage)
//--------------------------------------------------------------------------
{
    // Re-use operator*
    *this = *this * anImage;

    // Return the result
    return (*this);
}


//--------------------------------------------------------------------------
template<typename T> Image<T>& Image<T>::operator/=(const Image<T>& anImage)
//--------------------------------------------------------------------------
{
    // Re-use operator/
    *this = *this / anImage;

    // Return the result
    return (*this);
}


//---------------------------------------------------------------
template<typename T> Image<T> Image<T>::operator+(T aValue) const
//---------------------------------------------------------------
{
    // Copy the instance into a temporary variable
    Image<T> temp(*this);
    temp.m_spacing = m_spacing;

#ifdef NDEBUG
        #pragma omp parallel for
#endif
#ifdef WIN32
    for (int i = 0; i < m_width * m_height * m_number_of_slices; ++i)
#else
    for (size_t i = 0; i < m_width * m_height * m_number_of_slices; ++i)
#endif
        {
        temp.m_p_image[i] += aValue;
        }

    // Return the result
    return (temp);
}


//---------------------------------------------------------------
template<typename T> Image<T> Image<T>::operator-(T aValue) const
//---------------------------------------------------------------
{
    // Copy the instance into a temporary variable
    Image<T> temp(*this);
    temp.m_spacing = m_spacing;

#ifdef NDEBUG
        #pragma omp parallel for
#endif
#ifdef WIN32
    for (int i = 0; i < m_width * m_height * m_number_of_slices; ++i)
#else
    for (size_t i = 0; i < m_width * m_height * m_number_of_slices; ++i)
#endif
        {
        temp.m_p_image[i] -= aValue;
        }

    // Return the result
    return (temp);
}


//---------------------------------------------------------------
template<typename T> Image<T> Image<T>::operator*(T aValue) const
//---------------------------------------------------------------
{
    // Copy the instance into a temporary variable
    Image<T> temp(*this);
    temp.m_spacing = m_spacing;

#ifdef NDEBUG
        #pragma omp parallel for
#endif
#ifdef WIN32
    for (int i = 0; i < m_width * m_height * m_number_of_slices; ++i)
#else
    for (size_t i = 0; i < m_width * m_height * m_number_of_slices; ++i)
#endif
        {
        temp.m_p_image[i] *= aValue;
        }

    // Return the result
    return (temp);
}


//---------------------------------------------------------------
template<typename T> Image<T> Image<T>::operator/(T aValue) const
//---------------------------------------------------------------
{
    // Division by zero
    if (std::abs(aValue) < EPSILON)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Division by zero.");
    }

    // Copy the instance into a temporary variable
    Image<T> temp(*this);
    temp.m_spacing = m_spacing;

#ifdef NDEBUG
        #pragma omp parallel for
#endif
#ifdef WIN32
    for (int i = 0; i < m_width * m_height * m_number_of_slices; ++i)
#else
    for (size_t i = 0; i < m_width * m_height * m_number_of_slices; ++i)
#endif
        {
        temp.m_p_image[i] /= aValue;
        }

    // Return the result
    return (temp);
}


//-----------------------------------------------------------
template<typename T> Image<T>& Image<T>::operator+=(T aValue)
//-----------------------------------------------------------
{
#ifdef NDEBUG
        #pragma omp parallel for
#endif
#ifdef WIN32
    for (int i = 0; i < m_width * m_height * m_number_of_slices; ++i)
#else
    for (size_t i = 0; i < m_width * m_height * m_number_of_slices; ++i)
#endif
        {
        m_p_image[i] += aValue;
        }

    // Return the result
    return (*this);
}


//-----------------------------------------------------------
template<typename T> Image<T>& Image<T>::operator-=(T aValue)
//-----------------------------------------------------------
{
#ifdef NDEBUG
        #pragma omp parallel for
#endif
#ifdef WIN32
    for (int i = 0; i < m_width * m_height * m_number_of_slices; ++i)
#else
    for (size_t i = 0; i < m_width * m_height * m_number_of_slices; ++i)
#endif
    {
        m_p_image[i] -= aValue;
    }

    // Return the result
    return (*this);
}


//-----------------------------------------------------------
template<typename T> Image<T>& Image<T>::operator*=(T aValue)
//-----------------------------------------------------------
{
#ifdef NDEBUG
        #pragma omp parallel for
#endif
#ifdef WIN32
    for (int i = 0; i < m_width * m_height * m_number_of_slices; ++i)
#else
    for (size_t i = 0; i < m_width * m_height * m_number_of_slices; ++i)
#endif
    {
        m_p_image[i] *= aValue;
    }

    // Return the result
    return (*this);
}


//-----------------------------------------------------------
template<typename T> Image<T>& Image<T>::operator/=(T aValue)
//-----------------------------------------------------------
{
    // Division by zero
    if (std::abs(aValue) < EPSILON)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Division by zero.");
    }

#ifdef NDEBUG
        #pragma omp parallel for
#endif
    for (int i = 0; i < int(m_width * m_height * m_number_of_slices); ++i)
        {
        m_p_image[i] /= aValue;
        }

    // Return the result
    return (*this);
}


//-------------------------------------------------------
template<typename T> Image<T> Image<T>::operator!() const
//-------------------------------------------------------
{
    // Copy the instance into a temporary variable
    Image<T> temp(*this);
    temp.m_spacing = m_spacing;

    T min_value(getMinValue());
    T max_value(getMaxValue());
    T range(max_value - min_value);

    // Process every pixel
#ifdef NDEBUG
        #pragma omp parallel for
#endif
    for (unsigned int i = 0;
            i < m_width * m_height * m_number_of_slices;
            ++i)
    {
        // Take care to preserve the dynamic of the image
        temp.m_p_image[i] = min_value + range * (1.0 - (m_p_image[i] - min_value) / range);
    }

    // Return the result
    return (temp);
}


//------------------------------------------------------------------------------------------
template<typename T> Image<T> Image<T>::setCanvasSize(unsigned int aNewWidth,
                                                      unsigned int aNewHeight,
                                                      unsigned int aNewNumberOfSlices) const
//------------------------------------------------------------------------------------------
{
    Image<T> temp(aNewWidth, aNewHeight, aNewNumberOfSlices);
    temp.m_spacing = m_spacing;

    int x_offset((int(aNewWidth)  - int(m_width))  / 2);
    int y_offset((int(aNewHeight) - int(m_height)) / 2);
    int z_offset((int(aNewNumberOfSlices) - int(m_number_of_slices)) / 2);

#ifdef NDEBUG
#ifdef WIN32
        #pragma omp parallel for
#else
        #pragma omp parallel for collapse(3)
#endif
#endif
	for (int k = 0; k < m_number_of_slices; ++k)
    {
        for (int j = 0; j < m_height; ++j)
        {
            for (int i = 0; i < m_width; ++i)
            {
                if (i + x_offset >= 0 && i + x_offset < aNewWidth &&
                    j + y_offset >= 0 && j + y_offset < aNewHeight &&
                    k + z_offset >= 0 && k + z_offset < aNewNumberOfSlices)
                {
                    temp(i + x_offset, j + y_offset, k + z_offset) = getPixel(i, j, k);
                }
            }
        }
    }

    return (temp);
}


//-----------------------------------------------------------------------------------
template<typename T> Image<T> Image<T>::resize(unsigned int aNewWidth,
                                               unsigned int aNewHeight,
                                               unsigned int aNewNumberOfSlices) const
//-----------------------------------------------------------------------------------
{
    Image temp(aNewWidth, aNewHeight, aNewNumberOfSlices);
    temp.m_spacing = m_spacing;
    temp.m_spacing[0] = m_spacing[0] * RATIONAL_NUMBER(m_width)            / RATIONAL_NUMBER(aNewWidth);
    temp.m_spacing[1] = m_spacing[1] * RATIONAL_NUMBER(m_height)           / RATIONAL_NUMBER(aNewHeight);
    temp.m_spacing[2] = m_spacing[2] * RATIONAL_NUMBER(m_number_of_slices) / RATIONAL_NUMBER(aNewNumberOfSlices);

#ifdef NDEBUG
#ifdef WIN32
        #pragma omp parallel for
#else
        #pragma omp parallel for collapse(3)
#endif
#endif
	for (int k = 0; k < aNewNumberOfSlices; ++k)
    {
        for (unsigned j = 0; j < aNewHeight; ++j)
        {
            for (unsigned i = 0; i < aNewWidth; ++i)
            {

                double x = double(i) * (m_width  - 1) / (aNewWidth  - 1);
                double y = double(j) * (m_height - 1) / (aNewHeight - 1);
                double z = double(k) * (m_number_of_slices - 1) / (aNewNumberOfSlices - 1);

                int x1 = floor(x);
                int y1 = floor(y);
                int z1 = floor(z);

                if (x1 < 0) x1 = 0;
                if (y1 < 0) y1 = 0;
                if (z1 < 0) z1 = 0;

                int x2 = x1 + 1.0;
                int y2 = y1 + 1.0;
                int z2 = z1 + 1.0;

                if (x2 >= m_width)
                {
                    x2 = m_width  - 1;
                }

                if (y2 >= m_height)
                {
                    y2 = m_height - 1;
                }

                if (z2 >= m_number_of_slices)
                {
                    z2 = m_number_of_slices - 1;
                }



                double c111 = getPixel(x1, y1, z1);
                double c121 = getPixel(x1, y2, z1);
                double c221 = getPixel(x2, y2, z1);
                double c211 = getPixel(x2, y1, z1);

                double cx11 = c111;
                double cx21 = c121;
                if (x1 != x2)
                {
                    cx11 += (c211 - c111) * (x - x1) / (x2 - x1);
                    cx21 += (c221 - c121) * (x - x1) / (x2 - x1);
                }

                double cy1  = cx11;
                if (y1 != y2)
                {
                    cy1 += (cx21 - cx11) * (y - y1) / (y2 - y1);
                }


                double c112 = getPixel(x1, y1, z2);
                double c122 = getPixel(x1, y2, z2);
                double c222 = getPixel(x2, y2, z2);
                double c212 = getPixel(x2, y1, z2);

                double cx12 = c112;
                double cx22 = c122;
                if (x1 != x2)
                {
                    cx12 += (c212 - c112) * (x - x1) / (x2 - x1);
                    cx22 += (c222 - c122) * (x - x1) / (x2 - x1);
                }

                double cy2  = cx12;
                if (y1 != y2)
                {
                    cy2 += (cx22 - cx12) * (y - y1) / (y2 - y1);
                }

                double cz  = cy1;
                if (z1 != z2)
                {
                    cz += (cy2 - cy1) * (z - z1) / (z2 - z1);
                }

                temp(i, j, k) = cz;
            }
        }
    }

    return (temp);
}


//--------------------------------------------------------------------------
template<typename T> Image<T> Image<T>::rotate(float anAngleInDegrees) const
//--------------------------------------------------------------------------
{
    Image<T> temp(m_width, m_height, m_number_of_slices);
    temp.m_spacing = m_spacing;

    float angle_in_radian(anAngleInDegrees * Pi / 180);

    //double center_x(0.5 * (m_width + 1) /*- 1*/);
    //double center_y(0.5 * (m_height + 1) /*- 1*/);

#ifdef NDEBUG
#ifdef WIN32
        #pragma omp parallel for
#else
        #pragma omp parallel for collapse(3)
#endif
#endif
	for (int k = 0; k < m_number_of_slices; ++k)
    {
        for (unsigned j = 0; j < m_height; ++j)
        {
            for (unsigned i = 0; i < m_width; ++i)
            {
                double temp_x(i);
                double temp_y(j);

                temp_x += 0.5;
                temp_y += 0.5;

                temp_x -= (m_width - 1.0) / 2.0;
                temp_y -= (m_height - 1.0) / 2.0;

                double x = temp_x * std::cos(angle_in_radian) + temp_y * std::sin(angle_in_radian);
                double y = temp_y * std::cos(angle_in_radian) - temp_x * std::sin(angle_in_radian);

                x += (m_width - 1.0) / 2.0;
                y += (m_height - 1.0) / 2.0;

                int x1 = floor(x);
                int y1 = floor(y);

                int x2 = x1 + 1;
                int y2 = y1 + 1;

                if (x1 >= 0 && x1 < m_width &&
                    y1 >= 0 && y1 < m_height)
                {
                temp(i, j, k) = getPixel(x1, y1, k);

                if (x2 >= 0 && x2 < m_width &&
                        y2 >= 0 && y2 < m_height)
                    {
                    double c11 = getPixel(x1, y1, k);
                    double c12 = getPixel(x1, y2, k);
                    double c22 = getPixel(x2, y2, k);
                    double c21 = getPixel(x2, y1, k);

                    double cx1 = c11 + (c21 - c11) * (x - double(x1)) / double(x2 - x1);
                    double cx2 = c12 + (c22 - c12) * (x - double(x1)) / double(x2 - x1);

                    double cy  = cx1 + (cx2 - cx1) * double(y - y1) / double(y2 - y1);

                    temp(i, j, k) = cy;
                    }
                }
            }
        }
    }

    return (temp);
}


//--------------------------------------------------------------------------------
template<typename T> Sinogram<T> Image<T>::radonTransform(double aFirstAngle,
                                                          double anAngleStep,
                                                          double aLastAngle) const
//--------------------------------------------------------------------------------
{
    unsigned int square_width(4 + std::max(m_width, m_height));
    square_width *= square_width;

    unsigned int sinogram_width(floor(std::sqrt(2.0 * square_width)));


    //unsigned int sinogram_width(floor(double(std::max(m_width, m_height)) / 2.0 * (2.0 * std::sqrt(2.0))));
    unsigned int sinogram_height(1);
    if (std::abs(anAngleStep) > EPSILON && std::abs(aLastAngle - aFirstAngle) > EPSILON)
        {
        sinogram_height += floor((aLastAngle - aFirstAngle) / anAngleStep);
        }

    double scaling_factor(double(sinogram_width) / double(m_width));

    Sinogram<T> sinogram(sinogram_width, sinogram_height, m_number_of_slices);

    Image<T> padded_image(setCanvasSize(sinogram_width, scaling_factor * m_height, m_number_of_slices));

#ifndef NDEBUG
    padded_image.save("padded.mhd");
#endif

    double angle(aFirstAngle);

    for (unsigned int angle_id(0); angle_id < sinogram_height; ++angle_id, angle += anAngleStep)
        {
        Image<T> rotated_image(padded_image.rotate(angle));
#ifndef NDEBUG
        {
            std::stringstream file_name;
            file_name << "rotated_" << angle << ".mhd";
            rotated_image.save(file_name.str());
        }
#endif

#ifdef NDEBUG
#ifdef WIN32
        #pragma omp parallel for
#else
		#pragma omp parallel for collapse(3)
#endif
#endif
		for (int j = 0; j < rotated_image.m_height; ++j)
            {
            for (int i = 0; i < rotated_image.m_width; ++i)
                {
                for (int k = 0; k < m_number_of_slices; ++k)
                    {
                    sinogram(i, angle_id, k) += rotated_image(i, j, k);
                    }
                }
            }

#ifndef NDEBUG
        {
            std::stringstream file_name;
            file_name << "sinogram_" << angle << ".mhd";
            sinogram.save(file_name.str());
        }
#endif

        }

    return (sinogram);
}


//-----------------------------------------------------------------------------------------
template<typename T> PolygonMesh Image<T>::marchingCubes(const T& aFirstThesholdValue,
                                                         const T& aSecondThresholdValue,
                                                         bool aTwoThresholdFlag,
                                                         unsigned char aVerboseLevel) const
//-----------------------------------------------------------------------------------------
{
    // We use a temporary copy of the volume that we pad.
    // This is to make sure the iso-surface is closed.
    Image<unsigned char> padded_image;

    unsigned char value_in = 255;
    unsigned char value_out = 0;
    unsigned char iso_value = 128;

    if (!aTwoThresholdFlag)
    {
        padded_image = this->threshold(aFirstThesholdValue, value_in, value_out).constantPadFilter(value_out);
    }
    else
    {
        padded_image = this->threshold(aFirstThesholdValue, aSecondThresholdValue, value_in, value_out, true).constantPadFilter(value_out);
    }


    PolygonMesh iso_surface;

    {

    struct GLvector
    {
            float fX;
            float fY;
            float fZ;
    };

    //a2fEdgeDirection lists the direction vector (vertex1-vertex0) for each edge in the cube
    static const float a2fEdgeDirection[12][3] =
    {
            {1.0, 0.0, 0.0},{0.0, 1.0, 0.0},{-1.0, 0.0, 0.0},{0.0, -1.0, 0.0},
            {1.0, 0.0, 0.0},{0.0, 1.0, 0.0},{-1.0, 0.0, 0.0},{0.0, -1.0, 0.0},
            {0.0, 0.0, 1.0},{0.0, 0.0, 1.0},{ 0.0, 0.0, 1.0},{0.0,  0.0, 1.0}
    };

    //a2iEdgeConnection lists the index of the endpoint vertices for each of the 12 edges of the cube
    static const int a2iEdgeConnection[12][2] =
    {
            {0,1}, {1,2}, {2,3}, {3,0},
            {4,5}, {5,6}, {6,7}, {7,4},
            {0,4}, {1,5}, {2,6}, {3,7}
    };

    //a2fVertexOffset lists the positions, relative to vertex0, of each of the 8 vertices of a cube
    static const unsigned int a2fVertexOffset[8][3] =
    {
            {0, 0, 0},{1, 0, 0},{1, 1, 0},{0, 1, 0},
            {0, 0, 1},{1, 0, 1},{1, 1, 1},{0, 1, 1}
    };

    // For any edge, if one vertex is inside of the surface and the other is outside of the surface
    //  then the edge intersects the surface
    // For each of the 8 vertices of the cube can be two possible states : either inside or outside of the surface
    // For any cube the are 2^8=256 possible sets of vertex states
    // This table lists the edges intersected by the surface for all 256 possible vertex states
    // There are 12 edges.  For each entry in the table, if edge #n is intersected, then bit #n is set to 1

    static const int aiCubeEdgeFlags[256]=
    {
            0x000, 0x109, 0x203, 0x30a, 0x406, 0x50f, 0x605, 0x70c, 0x80c, 0x905, 0xa0f, 0xb06, 0xc0a, 0xd03, 0xe09, 0xf00,
            0x190, 0x099, 0x393, 0x29a, 0x596, 0x49f, 0x795, 0x69c, 0x99c, 0x895, 0xb9f, 0xa96, 0xd9a, 0xc93, 0xf99, 0xe90,
            0x230, 0x339, 0x033, 0x13a, 0x636, 0x73f, 0x435, 0x53c, 0xa3c, 0xb35, 0x83f, 0x936, 0xe3a, 0xf33, 0xc39, 0xd30,
            0x3a0, 0x2a9, 0x1a3, 0x0aa, 0x7a6, 0x6af, 0x5a5, 0x4ac, 0xbac, 0xaa5, 0x9af, 0x8a6, 0xfaa, 0xea3, 0xda9, 0xca0,
            0x460, 0x569, 0x663, 0x76a, 0x066, 0x16f, 0x265, 0x36c, 0xc6c, 0xd65, 0xe6f, 0xf66, 0x86a, 0x963, 0xa69, 0xb60,
            0x5f0, 0x4f9, 0x7f3, 0x6fa, 0x1f6, 0x0ff, 0x3f5, 0x2fc, 0xdfc, 0xcf5, 0xfff, 0xef6, 0x9fa, 0x8f3, 0xbf9, 0xaf0,
            0x650, 0x759, 0x453, 0x55a, 0x256, 0x35f, 0x055, 0x15c, 0xe5c, 0xf55, 0xc5f, 0xd56, 0xa5a, 0xb53, 0x859, 0x950,
            0x7c0, 0x6c9, 0x5c3, 0x4ca, 0x3c6, 0x2cf, 0x1c5, 0x0cc, 0xfcc, 0xec5, 0xdcf, 0xcc6, 0xbca, 0xac3, 0x9c9, 0x8c0,
            0x8c0, 0x9c9, 0xac3, 0xbca, 0xcc6, 0xdcf, 0xec5, 0xfcc, 0x0cc, 0x1c5, 0x2cf, 0x3c6, 0x4ca, 0x5c3, 0x6c9, 0x7c0,
            0x950, 0x859, 0xb53, 0xa5a, 0xd56, 0xc5f, 0xf55, 0xe5c, 0x15c, 0x055, 0x35f, 0x256, 0x55a, 0x453, 0x759, 0x650,
            0xaf0, 0xbf9, 0x8f3, 0x9fa, 0xef6, 0xfff, 0xcf5, 0xdfc, 0x2fc, 0x3f5, 0x0ff, 0x1f6, 0x6fa, 0x7f3, 0x4f9, 0x5f0,
            0xb60, 0xa69, 0x963, 0x86a, 0xf66, 0xe6f, 0xd65, 0xc6c, 0x36c, 0x265, 0x16f, 0x066, 0x76a, 0x663, 0x569, 0x460,
            0xca0, 0xda9, 0xea3, 0xfaa, 0x8a6, 0x9af, 0xaa5, 0xbac, 0x4ac, 0x5a5, 0x6af, 0x7a6, 0x0aa, 0x1a3, 0x2a9, 0x3a0,
            0xd30, 0xc39, 0xf33, 0xe3a, 0x936, 0x83f, 0xb35, 0xa3c, 0x53c, 0x435, 0x73f, 0x636, 0x13a, 0x033, 0x339, 0x230,
            0xe90, 0xf99, 0xc93, 0xd9a, 0xa96, 0xb9f, 0x895, 0x99c, 0x69c, 0x795, 0x49f, 0x596, 0x29a, 0x393, 0x099, 0x190,
            0xf00, 0xe09, 0xd03, 0xc0a, 0xb06, 0xa0f, 0x905, 0x80c, 0x70c, 0x605, 0x50f, 0x406, 0x30a, 0x203, 0x109, 0x000
    };

    //  For each of the possible vertex states listed in aiCubeEdgeFlags there is a specific triangulation
    //  of the edge intersection points.  a2iTriangleConnectionTable lists all of them in the form of
    //  0-5 edge triples with the list terminated by the invalid value -1.
    //  For example: a2iTriangleConnectionTable[3] list the 2 triangles formed when corner[0]
    //  and corner[1] are inside of the surface, but the rest of the cube is not.
    //
    //  I found this table in an example program someone wrote long ago.  It was probably generated by hand

    static const int a2iTriangleConnectionTable[256][16] =
    {
            {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1},
            {3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1},
            {3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1},
            {3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1},
            {9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1},
            {1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1},
            {9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
            {2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1, -1},
            {8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1},
            {9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
            {4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1, -1},
            {3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1},
            {1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1, -1},
            {4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1, -1},
            {4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1},
            {9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1},
            {1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
            {5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1},
            {2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1, -1},
            {9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
            {0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
            {2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1, -1},
            {10, 3, 11, 10, 1, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1},
            {4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1, -1},
            {5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1, -1},
            {5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1},
            {9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1},
            {0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1},
            {1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1},
            {10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1, -1},
            {8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1, -1},
            {2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1},
            {7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1},
            {9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1, -1},
            {2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1, -1},
            {11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1, -1},
            {9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1, -1},
            {5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, -1},
            {11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, -1},
            {11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
            {1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1, -1},
            {9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1},
            {5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1, -1},
            {2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
            {0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
            {5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1, -1},
            {6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1},
            {0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1, -1},
            {3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1, -1},
            {6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1},
            {5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1},
            {1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
            {10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1, -1},
            {6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1},
            {1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1, -1},
            {8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1, -1},
            {7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, -1},
            {3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
            {5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1, -1},
            {0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1},
            {9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, -1},
            {8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1, -1},
            {5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, -1},
            {0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, -1},
            {6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1, -1},
            {10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1},
            {10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1},
            {8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1, -1},
            {1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1},
            {3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1, -1},
            {0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {8, 3, 2, 8, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1},
            {10, 4, 9, 10, 6, 4, 11, 2, 3, -1, -1, -1, -1, -1, -1, -1},
            {0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, -1, -1, -1, -1},
            {3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, -1, -1, -1, -1},
            {6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1, -1},
            {9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, -1, -1, -1, -1},
            {8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1, -1},
            {3, 11, 6, 3, 6, 0, 0, 6, 4, -1, -1, -1, -1, -1, -1, -1},
            {6, 4, 8, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {7, 10, 6, 7, 8, 10, 8, 9, 10, -1, -1, -1, -1, -1, -1, -1},
            {0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, -1, -1, -1, -1},
            {10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, -1, -1, -1, -1},
            {10, 6, 7, 10, 7, 1, 1, 7, 3, -1, -1, -1, -1, -1, -1, -1},
            {1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, -1, -1, -1, -1},
            {2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9, -1},
            {7, 8, 0, 7, 0, 6, 6, 0, 2, -1, -1, -1, -1, -1, -1, -1},
            {7, 3, 2, 6, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, -1, -1, -1, -1},
            {2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7, -1},
            {1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11, -1},
            {11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, -1, -1, -1, -1},
            {8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6, -1},
            {0, 9, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, -1, -1, -1, -1},
            {7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {3, 0, 8, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {0, 1, 9, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {8, 1, 9, 8, 3, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
            {10, 1, 2, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {1, 2, 10, 3, 0, 8, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
            {2, 9, 0, 2, 10, 9, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
            {6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, -1, -1, -1, -1},
            {7, 2, 3, 6, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {7, 0, 8, 7, 6, 0, 6, 2, 0, -1, -1, -1, -1, -1, -1, -1},
            {2, 7, 6, 2, 3, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1},
            {1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, -1, -1, -1, -1},
            {10, 7, 6, 10, 1, 7, 1, 3, 7, -1, -1, -1, -1, -1, -1, -1},
            {10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, -1, -1, -1, -1},
            {0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, -1, -1, -1, -1},
            {7, 6, 10, 7, 10, 8, 8, 10, 9, -1, -1, -1, -1, -1, -1, -1},
            {6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {3, 6, 11, 3, 0, 6, 0, 4, 6, -1, -1, -1, -1, -1, -1, -1},
            {8, 6, 11, 8, 4, 6, 9, 0, 1, -1, -1, -1, -1, -1, -1, -1},
            {9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, -1, -1, -1, -1},
            {6, 8, 4, 6, 11, 8, 2, 10, 1, -1, -1, -1, -1, -1, -1, -1},
            {1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, -1, -1, -1, -1},
            {4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, -1, -1, -1, -1},
            {10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3, -1},
            {8, 2, 3, 8, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1},
            {0, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, -1, -1, -1, -1},
            {1, 9, 4, 1, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1, -1},
            {8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, -1, -1, -1, -1},
            {10, 1, 0, 10, 0, 6, 6, 0, 4, -1, -1, -1, -1, -1, -1, -1},
            {4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3, -1},
            {10, 9, 4, 6, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {4, 9, 5, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {0, 8, 3, 4, 9, 5, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
            {5, 0, 1, 5, 4, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
            {11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, -1, -1, -1, -1},
            {9, 5, 4, 10, 1, 2, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
            {6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, -1, -1, -1, -1},
            {7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, -1, -1, -1, -1},
            {3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6, -1},
            {7, 2, 3, 7, 6, 2, 5, 4, 9, -1, -1, -1, -1, -1, -1, -1},
            {9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, -1, -1, -1, -1},
            {3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, -1, -1, -1, -1},
            {6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8, -1},
            {9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, -1, -1, -1, -1},
            {1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4, -1},
            {4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10, -1},
            {7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, -1, -1, -1, -1},
            {6, 9, 5, 6, 11, 9, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1},
            {3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, -1, -1, -1, -1},
            {0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, -1, -1, -1, -1},
            {6, 11, 3, 6, 3, 5, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1},
            {1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, -1, -1, -1, -1},
            {0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10, -1},
            {11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5, -1},
            {6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, -1, -1, -1, -1},
            {5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, -1, -1, -1, -1},
            {9, 5, 6, 9, 6, 0, 0, 6, 2, -1, -1, -1, -1, -1, -1, -1},
            {1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8, -1},
            {1, 5, 6, 2, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6, -1},
            {10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, -1, -1, -1, -1},
            {0, 3, 8, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {11, 5, 10, 7, 5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {11, 5, 10, 11, 7, 5, 8, 3, 0, -1, -1, -1, -1, -1, -1, -1},
            {5, 11, 7, 5, 10, 11, 1, 9, 0, -1, -1, -1, -1, -1, -1, -1},
            {10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, -1, -1, -1, -1},
            {11, 1, 2, 11, 7, 1, 7, 5, 1, -1, -1, -1, -1, -1, -1, -1},
            {0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, -1, -1, -1, -1},
            {9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, -1, -1, -1, -1},
            {7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2, -1},
            {2, 5, 10, 2, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1},
            {8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, -1, -1, -1, -1},
            {9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, -1, -1, -1, -1},
            {9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2, -1},
            {1, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {0, 8, 7, 0, 7, 1, 1, 7, 5, -1, -1, -1, -1, -1, -1, -1},
            {9, 0, 3, 9, 3, 5, 5, 3, 7, -1, -1, -1, -1, -1, -1, -1},
            {9, 8, 7, 5, 9, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {5, 8, 4, 5, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1},
            {5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, -1, -1, -1, -1},
            {0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, -1, -1, -1, -1},
            {10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4, -1},
            {2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, -1, -1, -1, -1},
            {0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11, -1},
            {0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5, -1},
            {9, 4, 5, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, -1, -1, -1, -1},
            {5, 10, 2, 5, 2, 4, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1},
            {3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9, -1},
            {5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, -1, -1, -1, -1},
            {8, 4, 5, 8, 5, 3, 3, 5, 1, -1, -1, -1, -1, -1, -1, -1},
            {0, 4, 5, 1, 0, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, -1, -1, -1, -1},
            {9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {4, 11, 7, 4, 9, 11, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1},
            {0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, -1, -1, -1, -1},
            {1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, -1, -1, -1, -1},
            {3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4, -1},
            {4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, -1, -1, -1, -1},
            {9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3, -1},
            {11, 7, 4, 11, 4, 2, 2, 4, 0, -1, -1, -1, -1, -1, -1, -1},
            {11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, -1, -1, -1, -1},
            {2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, -1, -1, -1, -1},
            {9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7, -1},
            {3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10, -1},
            {1, 10, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {4, 9, 1, 4, 1, 7, 7, 1, 3, -1, -1, -1, -1, -1, -1, -1},
            {4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, -1, -1, -1, -1},
            {4, 0, 3, 7, 4, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {9, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {3, 0, 9, 3, 9, 11, 11, 9, 10, -1, -1, -1, -1, -1, -1, -1},
            {0, 1, 10, 0, 10, 8, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1},
            {3, 1, 10, 11, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {1, 2, 11, 1, 11, 9, 9, 11, 8, -1, -1, -1, -1, -1, -1, -1},
            {3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, -1, -1, -1, -1},
            {0, 2, 11, 8, 0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {2, 3, 8, 2, 8, 10, 10, 8, 9, -1, -1, -1, -1, -1, -1, -1},
            {9, 10, 2, 0, 9, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, -1, -1, -1, -1},
            {1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {1, 3, 8, 9, 1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
            {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
    };


    if (padded_image.getWidth() && padded_image.getHeight() && padded_image.getDepth() && padded_image.getRawData())
        {
            unsigned int max_thread(1);
#ifdef HAS_OPENMP
            max_thread = omp_get_max_threads();
#endif

            std::vector<std::vector<float> > p_vertex_set(max_thread);
            std::vector<std::vector<float> > p_normal_vector_set(max_thread);

            if (aVerboseLevel)
            {
                LOGGER.logNow("Create the iso-surface using Marching Cubes") << std::endl;

                if (aTwoThresholdFlag)
                    LOGGER.logNow("use two thresholds") << std::endl;
                else
                    LOGGER.logNow("use a single threshold") << std::endl;
            }

            unsigned int number_of_voxels((padded_image.getWidth() - 1) * (padded_image.getHeight() - 1) * (padded_image.getDepth() - 1));
            unsigned int thread_progress = 0;

#ifdef NDEBUG
#ifdef WIN32
        #pragma omp parallel for
#else
        #pragma omp parallel for collapse(3)
#endif
#endif
            for (int k = 0; k < int(padded_image.getDepth()) - 1; ++k)
            {
            for (int j = 0; j < int(padded_image.getHeight()) - 1; ++j)
                {
                for (int i = 0; i < int(padded_image.getWidth()) - 1; ++i)
                    {
#ifdef HAS_OPENMP
                    int thread_id = omp_get_thread_num();
#else
                    int thread_id = 0;
#endif

                    // Deal with the progress bar
                    if (thread_id == 0)
                        {
                        thread_progress++;

                        if (aVerboseLevel)
                            {
#ifdef HAS_OPENMP
                            int number_of_threads = omp_get_num_threads();
#else
                            int number_of_threads = 1;
#endif
                            unsigned int tasks_per_thread = number_of_voxels / number_of_threads;

                            if (thread_progress == 1)
                                {
                                LOGGER.logNow("Number of threads: ") << number_of_threads << std::endl;
                                LOGGER.logNow("Number of voxels per thread: ") << tasks_per_thread << std::endl << std::flush;
                                }

                            if (aVerboseLevel > 1)
                                {
                                LOGGER.progressBar(thread_progress, tasks_per_thread);
                                }
                            }
                        }

                    int iCorner, iVertex, iVertexTest, iEdge, iTriangle, iFlagIndex, iEdgeFlags;
                    float fOffset;
                    float afCubeValue[8];
                    GLvector asEdgeVertex[12];

                    VEC3 voxel_position(padded_image.getPixelPosition(i, j, k));

                    //Make a local copy of the values at the cube's corners
                    for(iVertex = 0; iVertex < 8; iVertex++)
                        {
                            afCubeValue[iVertex] = padded_image.getPixelValue(i + a2fVertexOffset[iVertex][0],
                                                                 j + a2fVertexOffset[iVertex][1],
                                                                 k + a2fVertexOffset[iVertex][2]);
                        }

                    //Find which vertices are inside of the surface and which are outside
                    iFlagIndex = 0;
                    for(iVertexTest = 0; iVertexTest < 8; iVertexTest++)
                        {
                        if(!(iso_value <= afCubeValue[iVertexTest]))
                            {
                            iFlagIndex |= 1<<iVertexTest;
                            }
                        }

                    //Find which edges are intersected by the surface
                    iEdgeFlags = aiCubeEdgeFlags[iFlagIndex];

                    //If the cube is not entirely inside or outside of the surface, then there will be no intersections
                    if (iEdgeFlags != 0)
                        {
                        //Find the point of intersection of the surface with each edge
                        //Then find the normal to the surface at those points
                        for(iEdge = 0; iEdge < 12; iEdge++)
                            {
                            //if there is an intersection on this edge
                            if(iEdgeFlags & (1<<iEdge))
                                {
                                    double fDelta = afCubeValue[ a2iEdgeConnection[iEdge][1] ] - afCubeValue[ a2iEdgeConnection[iEdge][0] ];

                                    if (std::abs(fDelta) < EPSILON)
                                        {
                                        fOffset = 0.5;
                                        }
                                    else
                                        {
                                            fOffset = (iso_value - afCubeValue[ a2iEdgeConnection[iEdge][0] ])/fDelta;
                                        }

                                asEdgeVertex[iEdge].fX = voxel_position.getX() + (a2fVertexOffset[ a2iEdgeConnection[iEdge][0] ][0]  +  fOffset * a2fEdgeDirection[iEdge][0]) * padded_image.getSpacing()[0];
                                asEdgeVertex[iEdge].fY = voxel_position.getY() + (a2fVertexOffset[ a2iEdgeConnection[iEdge][0] ][1]  +  fOffset * a2fEdgeDirection[iEdge][1]) * padded_image.getSpacing()[1];
                                asEdgeVertex[iEdge].fZ = voxel_position.getZ() + (a2fVertexOffset[ a2iEdgeConnection[iEdge][0] ][2]  +  fOffset * a2fEdgeDirection[iEdge][2]) * padded_image.getSpacing()[2];

                                }
                            }

                        //Draw the triangles that were found.  There can be up to five per cube
                        for (iTriangle = 0; iTriangle < 5; iTriangle++)
                        {
                            if(a2iTriangleConnectionTable[iFlagIndex][3*iTriangle] < 0)
                                break;

                            for(iCorner = 0; iCorner < 3; iCorner++)
                            {
                                    iVertex = a2iTriangleConnectionTable[iFlagIndex][3*iTriangle+iCorner];

                                    p_vertex_set[thread_id].push_back(asEdgeVertex[iVertex].fX);
                                    p_vertex_set[thread_id].push_back(asEdgeVertex[iVertex].fY);
                                    p_vertex_set[thread_id].push_back(asEdgeVertex[iVertex].fZ);

                                    VEC3 normal(
                                            padded_image.getPixelValue(std::max(i - 1, 0), j, k) - padded_image.getPixelValue(std::min(i + 1, int(padded_image.getWidth()) - 1), j, k),
                                            padded_image.getPixelValue(i, std::max(j - 1, 0), k) - padded_image.getPixelValue(i, std::min(j + 1, int(padded_image.getHeight()) - 1), k),
                                            padded_image.getPixelValue(i, j, std::max(k - 1, 0)) - padded_image.getPixelValue(i, j, std::min(k + 1, int(padded_image.getDepth()) - 1)));

                                    normal.normalise();

                                    p_normal_vector_set[thread_id].push_back(normal.getX());
                                    p_normal_vector_set[thread_id].push_back(normal.getY());
                                    p_normal_vector_set[thread_id].push_back(normal.getZ());
                                }
                            }
                        }
                    }
                }
            }

        if (aVerboseLevel > 1)
            {
            LOGGER.progressBar(1.0, true);
            }
            
        // Store the results from all the threads if needed
        if (max_thread > 1)
            {
                std::vector<float> p_combined_vertex_set;
                std::vector<float> p_combined_normal_vector_set;

                for (unsigned int i = 0; i < max_thread; ++i)
                {
                    p_combined_vertex_set.insert(p_combined_vertex_set.end(), p_vertex_set[i].begin(), p_vertex_set[i].end());
                    p_combined_normal_vector_set.insert(p_combined_normal_vector_set.end(), p_normal_vector_set[i].begin(), p_normal_vector_set[i].end());
                }

                iso_surface.setInternalData(
                        g_mesh_type,
                        &p_combined_vertex_set,
                        &p_combined_normal_vector_set,
                        false,
                        g_vbo_type);
            }
        else
            {
            iso_surface.setInternalData(
                    g_mesh_type,
                    &(p_vertex_set[0]),
                    &(p_normal_vector_set[0]),
                    false,
                    g_vbo_type);
            }
    }

    }

    return (iso_surface);
}


//--------------------------------------------------------
template<typename T> void Image<T>::destroyOpenGLTexture()
//--------------------------------------------------------
{
	if (m_texture_id)
	{
#ifdef HAS_OPENGL
		glDeleteTextures(1, &m_texture_id);
#endif

		m_texture_id = 0;
	}
}


//--------------------------------------------------------------
template<typename T> unsigned int Image<T>::getTextureId() const
//--------------------------------------------------------------
{
	return m_texture_id;
}


//----------------------------------------------------
inline std::string getPixelType(const char* aFileName)
//----------------------------------------------------
{
#ifdef HAS_ITK
    // Create a type
    typedef itk::ImageIOBase::IOComponentType ScalarPixelType;

    // Create an imageIO based on the first file
    itk::ImageIOBase::Pointer imageIO(itk::ImageIOFactory::CreateImageIO(aFileName, itk::ImageIOFactory::ReadMode));

    // Cannot create the imageIO
    if (!imageIO)
        {
        std::stringstream error_message;
        error_message << "Could not CreateImageIO for \"" << aFileName << "\"";
        throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
        }
    imageIO->SetFileName(aFileName);
    imageIO->ReadImageInformation();

    const ScalarPixelType pixelType = imageIO->GetComponentType();

    return (std::string(imageIO->GetComponentTypeAsString(pixelType)));
#else
    return (std::string("unknown"));
#endif
}


//-----------------------------------------------------------
inline std::string getPixelType(const std::string& aFileName)
//-----------------------------------------------------------
{
    return (getPixelType(aFileName.data()));
}


//--------------------------------------------------------------
template<typename T> Image<T> operator*(const T& aValue,
                                        const Image<T>& anImage)
//--------------------------------------------------------------
{
    return (anImage * aValue);
}


//--------------------------------------------------------------
template<typename T> Image<T> operator/(const T& aValue,
                                        const Image<T>& anImage)
//--------------------------------------------------------------
{
    Image<T> temp_image(anImage);
    temp_image.m_spacing = anImage.m_spacing;

    unsigned int number_of_voxels(temp_image.getWidth() * temp_image.getHeight() * temp_image.getDepth());
    T* p_temp(temp_image.getRawData());
    for (unsigned int i(0); i < number_of_voxels; ++i, ++p_temp)
    {
        if (std::abs(*p_temp) < EPSILON)
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "Division by zero.");
        }

        *p_temp = aValue / *p_temp;
    }

    return (temp_image);
}


//--------------------------------------------------------------
template<typename T> Image<T> operator+(const T& aValue,
                                        const Image<T>& anImage)
//--------------------------------------------------------------
{
    return (anImage + aValue);
}


//--------------------------------------------------------------
template<typename T> Image<T> operator-(const T& aValue,
                                        const Image<T>& anImage)
//--------------------------------------------------------------
{
    Image<T> temp_image(anImage);
    temp_image.m_spacing = anImage.m_spacing;

    unsigned int number_of_voxels(temp_image.getWidth() * temp_image.getHeight() * temp_image.getDepth());
    T* p_temp(temp_image.getRawData());

#ifdef NDEBUG
        #pragma omp parallel for
#endif
    for (int i = 0; i < number_of_voxels; ++i)
    {
        p_temp[i] = aValue - p_temp[i];
    }

    return (temp_image);
}


//--------------------------------------------------------
template<typename T> Image<T> log(const Image<T>& anImage)
//--------------------------------------------------------
{
    return (anImage.log());
}


//--------------------------------------------------------
template<typename T> Image<T> abs(const Image<T>& anImage)
//--------------------------------------------------------
{
    return (anImage.abs());
}


//------------------------------------------------------------------------
template<typename T> VEC3 Image<T>::VertexInterp(const T& aThresholdValue,
                                                 const VEC3& p0,
                                                 const VEC3& p1,
                                                 const T& v0,
                                                 const T& v1) const
//------------------------------------------------------------------------
{
    double ratio((v1 - aThresholdValue) / (v1 - v0));

    return (p1 + (p0 - p1) * ratio);
}


//------------------------------------------------------------------------------
template<typename T> void Image<T>::loadMetaHeader(const std::string& aFileName)
//------------------------------------------------------------------------------
{
    loadMetaHeader(aFileName.data());
}


//-----------------------------------------------------------------------
template<typename T> void Image<T>::loadMetaHeader(const char* aFileName)
//-----------------------------------------------------------------------
{
    // Open the meta header
    std::ifstream meta_header(aFileName, std::ios::binary|std::ios::in);

    // The file cannot be opened
    if (!meta_header.is_open())
    {
        // Throw an error
        throw FileDoesNotExistException(__FILE__, __FUNCTION__, __LINE__, aFileName);
    }

    // Parse the meta header
    unsigned int number_of_dimensions(0);
    double spacing[3] = {0.0, 0.0, 0.0};
    unsigned int dimensions[3] = {1, 1, 1};
    std::string element_type;
    std::string raw_file_name;
    bool change_endianess(false);
    // bool deflate_compression(false);

    std::string info_type;
    while (meta_header >> info_type)
    {
        std::string equal_sign;
        meta_header >> equal_sign;

        // Remove the equal sign
        if (equal_sign != "=")
        {
            std::string error_message("The program was expecting \"=\" but received \"");
            error_message += equal_sign;
            error_message += "\".";
            throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
        }

        if (info_type == "ObjectType")
        {
            std::string object_type;
            meta_header >> object_type;

            if (object_type != "Image")
            {
                std::string error_message("The object type \"");
                error_message += object_type;
                error_message += "\" is not known. It should have been \"Image\".";
                throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
            }
        }
        else if (info_type == "NDims")
        {
            meta_header >> number_of_dimensions;

            // number_of_dimensions is always positive as
            // it is an unsigned int
            if (/*number_of_dimensions < 0 ||*/ 3 < number_of_dimensions)
            {
                std::stringstream error_message;
                error_message << "The number of dimensions \"" << number_of_dimensions <<
                "\" is not valid. It should have been in the range [0, 3].";
                throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
            }
        }
        else if (info_type == "BinaryData")
        {
            std::string data_type;
            meta_header >> data_type;

            if (data_type != "True")
            {
                std::string error_message("The data type is not binary.");
                throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
            }
        }
        else if (info_type == "BinaryDataByteOrderMSB" || info_type == "ElementByteOrderMSB")
        {
            std::string big_endian;
            meta_header >> big_endian;

            if (big_endian == "False" && isBigEndian())
            {
                change_endianess = true;
            }

            if (big_endian == "True" && isLittleEndian())
            {
                change_endianess = true;
            }
        }
        else if (info_type == "CompressedData")
        {
            std::string compression;
            meta_header >> compression;

            if (compression == "True")
            {
                // deflate_compression = true;
            }
        }
        else if (info_type == "TransformMatrix")
        {
            for (unsigned int j(0); j < number_of_dimensions; ++j)
            {
                for (unsigned int i(0); i < number_of_dimensions; ++i)
                {
                    double temp;
                    meta_header >> temp;
                }
            }
        }
        else if (info_type == "Offset")
        {
            for (unsigned int i(0); i < number_of_dimensions; ++i)
            {
                double temp;
                meta_header >> temp;
            }
        }
        else if (info_type == "CenterOfRotation")
        {
            for (unsigned int i(0); i < number_of_dimensions; ++i)
            {
                double temp;
                meta_header >> temp;
            }
        }
        else if (info_type == "AnatomicalOrientation")
        {
            std::string anatomical_orientation;
            meta_header >> anatomical_orientation;
        }
        else if (info_type == "ElementSpacing")
        {
            for (unsigned int i(0); i < number_of_dimensions; ++i)
            {
                meta_header >> spacing[i];
            }
        }
        else if (info_type == "DimSize")
        {
            for (unsigned int i(0); i < number_of_dimensions; ++i)
            {
                meta_header >> dimensions[i];
            }
        }
        else if (info_type == "ElementType")
        {
            std::string data_type_string;
            meta_header >> data_type_string;

            if (data_type_string == "MET_DOUBLE")
            {
                element_type = typeid(double).name();
            }
            else if (data_type_string == "MET_FLOAT")
            {
                element_type = typeid(float).name();
            }
            else if (data_type_string == "MET_CHAR")
            {
                element_type = typeid(char).name();
            }
            else if (data_type_string == "MET_UCHAR")
            {
                element_type = typeid(unsigned char).name();
            }
            else if (data_type_string == "MET_SHORT")
            {
                element_type = typeid(short).name();
            }
            else if (data_type_string == "MET_USHORT")
            {
                element_type = typeid(unsigned short).name();
            }
            else if (data_type_string == "MET_INT")
            {
                element_type = typeid(int).name();
            }
            else if (data_type_string == "MET_UINT")
            {
                element_type = typeid(unsigned int).name();
            }
            else
            {
                std::string error_message("The data type \"");
                error_message += data_type_string;
                error_message += "\" is not known.";
                throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
            }
        }
        else if (info_type == "HeaderSize")
        {
            int header_size;
            meta_header >> header_size;
        }
        else if (info_type == "ElementSize")
        {
            int element_size;

            for (unsigned int i(0); i < number_of_dimensions; ++i)
            {
                meta_header >> element_size;
            }
        }
        else if (info_type == "ElementDataFile")
        {
            meta_header >> raw_file_name;

            if (raw_file_name == "LOCAL")
            {
                loadRAW(meta_header,
                		dimensions[0], dimensions[1], dimensions[2],
						change_endianess,
						aFileName,
						element_type.data()/*,
						deflate_compression*/);
            }
            else
            {
                std::string meta_header_file_name(aFileName);
                size_t index1(meta_header_file_name.find_last_of('\\'));
                size_t index2(meta_header_file_name.find_last_of('/'));
                size_t index(0);

                if (index1 == std::string::npos)
                {
                    index1 = 0;
                }

                if (index2 == std::string::npos)
                {
                    index2 = 0;
                }

                index = std::max(index1, index2);

                if (index)
                {
                    raw_file_name = meta_header_file_name.substr(0, index) + "/" + raw_file_name;
                }

                loadRAW(raw_file_name,
                    dimensions[0], dimensions[1], dimensions[2],
                    change_endianess,
                    element_type/*,
                    deflate_compression*/);
            }

            setSpacing(spacing[0], spacing[1], spacing[2]);
        }
        else
        {
            std::string error_message("The field \"");
            error_message += info_type;
            error_message += "\" is not known.";
            throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
        }
    }
}


template<typename T> unsigned int Image<T>::sizeOf(const char* anElementType) const
{
    return (sizeOf(std::string(anElementType)));
}


template<typename T> unsigned int Image<T>::sizeOf(const std::string& anElementType) const
{
    // Double
    if (anElementType == typeid(double).name())
        return (sizeof(double));

    // Float
    else if (anElementType == typeid(float).name())
        return (sizeof(float));

    // char
    else if (anElementType == typeid(char).name())
        return (sizeof(char));

    // unsigned char
    else if (anElementType == typeid(unsigned char).name())
        return (sizeof(unsigned char));

    // short
    else if (anElementType == typeid(short).name())
        return (sizeof(short));

    // unsigned short
    else if (anElementType == typeid(unsigned short).name())
        return (sizeof(unsigned short));

    // int
    else if (anElementType == typeid(int).name())
        return (sizeof(int));

    // unsigned int
    else if (anElementType == typeid(unsigned int).name())
        return (sizeof(unsigned int));


    std::string error_message("The data type \"");
    error_message += anElementType;
    error_message += "\" is not known.";
    throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
}


//-----------------------------------------------------------------------
template<typename T> void Image<T>::loadRAW(std::ifstream& anInputStream,
                                            unsigned int aWidth,
                                            unsigned int aHeight,
                                            unsigned int aNumberOfSlices,
                                            bool aChangeEndianessFlag,
                                            const char* aFileName,
                                            const char* anElementType/*,
                                            bool aCompressionFlag*/)
//-----------------------------------------------------------------------
{
    // Release memory if needed
    destroy();

    // Store the image size
    m_width = aWidth;
    m_height = aHeight;
    m_number_of_slices = aNumberOfSlices;

    unsigned int number_of_voxels(m_width * m_height * m_number_of_slices);

    // No data to read
    if (!number_of_voxels)
    {
        return;
    }

    // Data size in native file format (in bytes)
    unsigned int requested_length_in_bytes(
            number_of_voxels *
            sizeOf(anElementType));

    // Allocate the memory
    char* p_temp_buffer(new char[requested_length_in_bytes + sizeOf(anElementType)]); // + sizeOf(anElementType) in case there is an offset

    // Out of memory
    if (!p_temp_buffer)
    {
        throw OutOfMemoryException(__FILE__, __FUNCTION__, __LINE__);
    }

    // Get file size
    int offset_current_position(anInputStream.tellg());
    anInputStream.seekg (0, std::ios::end);
    unsigned int length(int(anInputStream.tellg()) - offset_current_position);
    anInputStream.seekg (offset_current_position, std::ios::beg);

    if (length < requested_length_in_bytes)
    {
        // release the memory
        delete [] p_temp_buffer;

        std::string error_message("The file size of \"");
        error_message += aFileName;
        error_message += "\" is too small to contained the specified raw data.";
        throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
    }


    // Allocate the memory
    /*m_p_image = new T[m_width * m_height * m_number_of_slices];

    // Out of memory
    if (m_width && m_height && m_number_of_slices && !m_p_image)
    {
        throw OutOfMemoryException(__FILE__, __FUNCTION__, __LINE__);
    }*/



    // Skip the first byte if needed
    int offset_in_bytes(0);
    anInputStream.read(reinterpret_cast<char*>(p_temp_buffer), 1);
    if (*reinterpret_cast<char*>(p_temp_buffer) != 10)
    {
        offset_in_bytes = 1;
    }

    // Read the data
    anInputStream.read(reinterpret_cast<char*>(p_temp_buffer) + offset_in_bytes, requested_length_in_bytes);

    // Close the file
    anInputStream.close();

    // Change endianess
    if (aChangeEndianessFlag && sizeOf(anElementType) > 1)
    {
#ifdef NDEBUG
        #pragma omp parallel for
#endif
        for (int i = 0; i < int(m_width * m_height * m_number_of_slices); ++i)
        {
            swapBytes(p_temp_buffer[i]);
        }
    }

    // Decompress the data
    /*if (aCompressionFlag)
    {
        // Array to store the decompressed data
        char* p_dest(new char[requested_length_in_bytes]);

        // Decompress the data
        int z_lib_return_code = inflate(p_temp_buffer,
                                        requested_length_in_bytes,
                                        &p_dest);

        // It does not exist
        if (!z_lib_return_code)
        {
            delete [] p_dest;
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "No image data was decoded.");
        }

        // Release the compressed data
        delete [] p_temp_buffer;
		p_temp_buffer = 0;

        // Keep the decompressed data
        p_temp_buffer = p_dest;
    }*/


    // Same native file format is the same as T
    if (anElementType == typeid(T).name())
    {
        // Reassign the pointer
        m_p_image = reinterpret_cast<T*>(p_temp_buffer);
    }
    // Conversion is needed
    else
    {
        // Allocate the memory
        m_p_image = new T[number_of_voxels];

        // Out of memory
        if (!m_p_image)
        {
            // release the memory
            delete [] p_temp_buffer;

            throw OutOfMemoryException(__FILE__, __FUNCTION__, __LINE__);
        }

        // Double
        if (std::string(anElementType) == typeid(double).name())
        {
            double* p_temp = reinterpret_cast<double*> (p_temp_buffer);
            std::copy_n(p_temp, number_of_voxels, reinterpret_cast<double*>(m_p_image));
        }

        // Float
        else if (std::string(anElementType) == typeid(float).name())
        {
            float* p_temp = reinterpret_cast<float*> (p_temp_buffer);
            std::copy_n(p_temp, number_of_voxels, reinterpret_cast<float*>(m_p_image));
        }

        // char
        else if (std::string(anElementType) == typeid(char).name())
        {
            char* p_temp = reinterpret_cast<char*> (p_temp_buffer);
            std::copy_n(p_temp, number_of_voxels, reinterpret_cast<char*>(m_p_image));
        }

        // unsigned char
        else if (std::string(anElementType) == typeid(unsigned char).name())
        {
            unsigned char* p_temp = reinterpret_cast<unsigned char*> (p_temp_buffer);
            std::copy_n(p_temp, number_of_voxels, reinterpret_cast<unsigned char*>(m_p_image));
        }

        // short
        else if (std::string(anElementType) == typeid(short).name())
        {
            short* p_temp = reinterpret_cast<short*> (p_temp_buffer);
            std::copy_n(p_temp, number_of_voxels, reinterpret_cast<short*>(m_p_image));
        }

        // unsigned short
        else if (std::string(anElementType) == typeid(unsigned short).name())
        {
            unsigned short* p_temp = reinterpret_cast<unsigned short*> (p_temp_buffer);
            std::copy_n(p_temp, number_of_voxels, reinterpret_cast<unsigned short*>(m_p_image));
        }

        // int
        else if (std::string(anElementType) == typeid(int).name())
        {
            int* p_temp = reinterpret_cast<int*> (p_temp_buffer);
            std::copy_n(p_temp, number_of_voxels, reinterpret_cast<int*>(m_p_image));
        }

        // unsigned int
        else if (std::string(anElementType) == typeid(unsigned int).name())
        {
            unsigned int* p_temp = reinterpret_cast<unsigned int*> (p_temp_buffer);
            std::copy_n(p_temp, number_of_voxels, reinterpret_cast<unsigned int*>(m_p_image));
        }

        else
        {
            // release the memory
            delete [] p_temp_buffer;

            std::string error_message("The data type \"");
            error_message += anElementType;
            error_message += "\" is not known.";
            throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
        }

        // release the memory
        delete [] p_temp_buffer;
    }
}


//------------------------------------------------------------------------------------
template<typename T> void Image<T>::saveMetaHeader(const std::string& aFileName, bool useDeflateCompressionIfPossible) const
//------------------------------------------------------------------------------------
{
    saveMetaHeader(aFileName.data(), useDeflateCompressionIfPossible);
}


//-----------------------------------------------------------------------------
template<typename T> void Image<T>::saveMetaHeader(const char* aFileName, bool useDeflateCompressionIfPossible) const
//-----------------------------------------------------------------------------
{
    // Open the file which will contain the meta header
    std::ofstream meta_header(aFileName);

    // The file is not open
    if (!meta_header.is_open())
    {
        std::string error_message;
        error_message = "Cannot create the file (\"";
        error_message += aFileName;
        error_message += "\")";

        throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message);
    }

    meta_header << "ObjectType = Image" << std::endl;
    if (m_number_of_slices == 1)
    {
        meta_header << "NDims = 2" << std::endl;
        meta_header << "DimSize = " << m_width << " " << m_height << std::endl;
    }
    else
    {
        meta_header << "NDims = 3" << std::endl;
        meta_header << "DimSize = " << m_width << " " << m_height << " " << m_number_of_slices << std::endl;
    }

    std::string data_type_string;
    if (typeid(T) == typeid(double))
    {
        data_type_string = "MET_DOUBLE";
    }
    else if (typeid(T) == typeid(float))
    {
        data_type_string = "MET_FLOAT";
    }
    else if (typeid(T) == typeid(char))
    {
        data_type_string = "MET_CHAR";
    }
    else if (typeid(T) == typeid(unsigned char))
    {
        data_type_string = "MET_UCHAR";
    }
    else if (typeid(T) == typeid(short))
    {
        data_type_string = "MET_SHORT";
    }
    else if (typeid(T) == typeid(unsigned short))
    {
        data_type_string = "MET_USHORT";
    }
    else if (typeid(T) == typeid(int))
    {
        data_type_string = "MET_INT";
    }
    else if (typeid(T) == typeid(unsigned int))
    {
        data_type_string = "MET_UINT";
    }
    else
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Unsupported data type");
    }

    meta_header << "ElementType = " << data_type_string << std::endl;

    if (m_number_of_slices == 1)
    {
        meta_header << "ElementSize = 1 1" << std::endl;
        meta_header << "ElementSpacing = " << m_spacing[0] << " " << m_spacing[1] << std::endl;
    }
    else
    {
        meta_header << "ElementSize = 1 1 1" << std::endl;
        meta_header << "ElementSpacing = " << m_spacing[0] << " " << m_spacing[1] << " " << m_spacing[2] << std::endl;
    }

    meta_header << "ElementByteOrderMSB = " <<
        (isBigEndian()? "True":"False") << std::endl;

    meta_header << "CompressedData = " <<
        (useDeflateCompressionIfPossible? "True":"False") << std::endl;

    std::string raw_file_name(aFileName);
    if (isMHD(aFileName))
    {
        raw_file_name[raw_file_name.size() - 3] = 'r';
        raw_file_name[raw_file_name.size() - 2] = 'a';
        raw_file_name[raw_file_name.size() - 1] = 'w';
    }
    else
    {
        raw_file_name += ".raw";
    }
    if (useDeflateCompressionIfPossible)
    {
        raw_file_name += ".gz";
    }

    size_t index1(raw_file_name.find_last_of('\\'));
    size_t index2(raw_file_name.find_last_of('/'));
    size_t index(0);

    if (index1 != std::string::npos)
    {
        ++index1;
    }
    else
    {
        index1 = 0;
    }

    if (index2 != std::string::npos)
    {
        ++index2;
    }
    else
    {
        index2 = 0;
    }

    index = std::max(index1, index2);


    if (isMHD(aFileName))
    {
        meta_header << "ElementDataFile = " << raw_file_name.substr(index, raw_file_name.size() - index) << std::endl;

        saveRAW(raw_file_name, useDeflateCompressionIfPossible);
    }
    else
    {
        meta_header << "ElementDataFile = " << "LOCAL\n";// << std::endl;

        // Write content to file
        // Compressed data
        if (useDeflateCompressionIfPossible)
        {
        	char* compressed_data = new char[m_width * m_height * m_number_of_slices * sizeof(T)];
        	int data_size = deflate(
        			reinterpret_cast<char*>(m_p_image),
					m_width * m_height * m_number_of_slices * sizeof(T),
					&compressed_data);

            meta_header.write(compressed_data, data_size);
        	delete [] compressed_data;
        }
        // Uncompressed data
        else
        {
            meta_header.write(reinterpret_cast<char*>(m_p_image), m_width * m_height * m_number_of_slices * sizeof(T));
        }
    }
}


//----------------------------------------------------------------------------------------
inline void convert(const Image<float>& aSourceImage, Image<unsigned short>& aTargetImage)
//----------------------------------------------------------------------------------------
{
	// Make sure the target image got the same size as the source image
	aTargetImage = Image<unsigned short>(aSourceImage.getWidth(), aSourceImage.getHeight(), aSourceImage.getDepth());
	aTargetImage.setSpacing(aSourceImage.getSpacing());

	// Clamp and cast the data
	std::transform(aSourceImage.getRawData(),
		aSourceImage.getRawData() + aSourceImage.getWidth() * aSourceImage.getHeight() * aSourceImage.getDepth(),
		aTargetImage.getRawData(), [](float x) {
			return static_cast<unsigned short>(std::clamp(static_cast<int>(std::round(x)), 0, USHRT_MAX));
		});
}


//----------------------------------------------------------------------------------------
inline void convert(const Image<unsigned short>& aSourceImage, Image<float>& aTargetImage)
//----------------------------------------------------------------------------------------
{
	// Make sure the target image got the same size as the source image
	aTargetImage = Image<float>(aSourceImage.getWidth(), aSourceImage.getHeight(), aSourceImage.getDepth());
	aTargetImage.setSpacing(aSourceImage.getSpacing());

	// Cast the data
	std::transform(aSourceImage.getRawData(),
		aSourceImage.getRawData() + aSourceImage.getWidth() * aSourceImage.getHeight() * aSourceImage.getDepth(),
		aTargetImage.getRawData(), [](unsigned short x) {
			return static_cast<float>(x);
		});

}


} // namespace gVirtualXRay
