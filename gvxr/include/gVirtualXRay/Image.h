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


#ifndef GVXR_IMAGE_H
#define GVXR_IMAGE_H


/**
********************************************************************************
*
*   @file       Image.h
*
*   @brief      Class to handle a greyscale image.
*
*   @version    1.0
*
*   @todo       Fix problem in marchingCubes (normals?).
*               See bug in demos (ImplicitSurface and SimpleBackProjection)
*
*   @date       30/09/2016
*
*   @author     Franck Vidal
*
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <string>
#include <map>

#ifdef HAS_GDCM
#include <gdcmFile.h>
#endif

#ifdef HAS_TIFF
#include <tiff.h>
#endif

#ifndef GVXR_TYPES_H
#include "gVirtualXRay/Types.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//******************************************************************************
//  Class/type declaration
//******************************************************************************
template<typename T> class Image;
template<typename T> class FFT;
template<typename T> class Sinogram;
class PolygonMesh;

// This is needed for MacOS
#ifndef TIFF_UINT16_T
#define TIFF_UINT16_T unsigned short
#endif

#ifndef TIFF_UINT32_T
#define TIFF_UINT32_T unsigned int
#endif


//******************************************************************************
//  Function declaration
//******************************************************************************
template<typename T> Image<T> operator*(const T& aValue,
                                        const Image<T>& anImage);


template<typename T> Image<T> operator/(const T& aValue,
                                        const Image<T>& anImage);


template<typename T> Image<T> operator+(const T& aValue,
                                        const Image<T>& anImage);


template<typename T> Image<T> operator-(const T& aValue,
                                        const Image<T>& anImage);


template<typename T> Image<T> log(const Image<T>& anImage);


template<typename T> Image<T> abs(const Image<T>& anImage);


std::string getPixelType(const std::string& aFileName);


std::string getPixelType(const char* aFileName);


template<typename T> Image<T> gauss2D(unsigned int aSize, double aSigmaValue);


//==============================================================================
/**
*   @class  Image
*   @brief  Image is a class to manage a greyscale image.
*/
//==============================================================================
template<typename T> class Image
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default constructor.
    //--------------------------------------------------------------------------
    Image();


    //--------------------------------------------------------------------------
    /// Copy constructor.
    /**
     *  @param anImage: the image to copy
     */
    //--------------------------------------------------------------------------
    Image(const Image<T>& anImage);


    //--------------------------------------------------------------------------
    /// Constructor.
    /**
     *  @param aFileName: the name of the file to load
     */
    //--------------------------------------------------------------------------
    Image(const char* aFileName);


    //--------------------------------------------------------------------------
    /// Constructor.
    /**
     *  @param aFileName: the name of the file to load
     */
    //--------------------------------------------------------------------------
    Image(const std::string& aFileName);


    //--------------------------------------------------------------------------
    /// Constructor from an array.
    /**
     *  @param apData:          the array to copy
     *  @param aWidth:          the width of the image
     *  @param aHeight:         the height of the image
     *  @param aNumberOfSlices: the number of slices (default value: 1)
     *  @param aVoxelSize:      the physical distance between the centres of 
     *                          each three-dimensional voxel, specified by three
     *                          numeric values (default value: 1, 1, 1)
     */
    //--------------------------------------------------------------------------
    Image(const T* apData,
                 unsigned int aWidth,
                 unsigned int aHeight,
                 unsigned int aNumberOfSlices = 1,
                 const VEC3& aVoxelSize = VEC3(1, 1, 1));


    //--------------------------------------------------------------------------
    /// Constructor to build a black image.
    /**
     *  @param aWidth:          the width of the image
     *  @param aHeight:         the height of the image
     *  @param aNumberOfSlices: the number of slices (default value: 1)
     *  @param aDefaultValue:   the pixel value (default value: 0)
     *  @param aVoxelSize:      the physical distance between the centres of 
     *                          each three-dimensional voxel, specified by three
     *                          numeric values (default value: 1, 1, 1)
     */
    //--------------------------------------------------------------------------
    Image(unsigned int aWidth,
                 unsigned int aHeight,
                 unsigned int aNumberOfSlices = 1,
                 T aDefaultValue = 0,
                 const VEC3& aVoxelSize = VEC3(1, 1, 1));


    //--------------------------------------------------------------------------
    /// Destructor.
    //--------------------------------------------------------------------------
    ~Image();


    //--------------------------------------------------------------------------
    /// Replace all the pixels of an image with a default value.
    /**
     *  @param aDefaultColour:  the pixel value (default value: 0)
     */
    //--------------------------------------------------------------------------
    void makeBlank(const T& aDefaultColour = 0);


    //--------------------------------------------------------------------------
    /// Compute a region of interest (ROI).
    /**
     *  @param i:       the position of the first pixel of the ROI along
     *                  the horizontal axis
     *  @param j:       the position of the first pixel of the ROI along
     *                  the vertical axis
     *  @param k:       the position of the first pixel of the ROI along
     *                  the depth axis
     *  @param aWidth:  the width of the ROI (in number of pixels)
     *  @param aHeight: the height of the ROI (in number of pixels)
     *  @param aDepth:  the depth of the ROI (in number of pixels)
     *  @return the ROI
     */
    //--------------------------------------------------------------------------
    Image getROI(unsigned int i,
            unsigned int j,
            unsigned int k,
            unsigned int aWidth,
            unsigned int aHeight,
            unsigned int aDepth) const;


    //--------------------------------------------------------------------------
    /// Set a pixel.
    /**
     *  @param i:   the position of the pixel along the x-axis
     *  @param j:   the position of the pixel along the y-axis
     *  @param k:   the position of the pixel along the z-axis
     *  @param aValue:  the new pixel value
     */
    //--------------------------------------------------------------------------
    void setPixel(unsigned int i, unsigned int j, unsigned int k, T aValue);


    //--------------------------------------------------------------------------
    /// Accessor on a pixel value.
    /**
     *  @param i:   the position of the pixel along the x-axis
     *  @param j:   the position of the pixel along the y-axis
     *  @param k:   the position of the pixel along the z-axis (default value: 0)
     *  @return the pixel value
     */
    //--------------------------------------------------------------------------
    T& getPixel(unsigned int i, unsigned int j, unsigned int k = 0);


    //--------------------------------------------------------------------------
    /// Accessor on a pixel value.
    /**
     *  @param i:   the position of the pixel along the x-axis
     *  @param j:   the position of the pixel along the y-axis
     *  @param k:   the position of the pixel along the z-axis (default value: 0)
     *  @return the pixel value
     */
    //--------------------------------------------------------------------------
    const T& getPixel(unsigned int i, unsigned int j, unsigned int k = 0) const;


    //--------------------------------------------------------------------------
    /// Accessor on a pixel value.
    /**
     *  @param i:   the position of the pixel along the x-axis
     *  @param j:   the position of the pixel along the y-axis
     *  @param k:   the position of the pixel along the z-axis (default value: 0)
     *  @return the pixel value
     */
    //--------------------------------------------------------------------------
    T& getPixelValue(unsigned int i, unsigned int j, unsigned int k = 0);


    //--------------------------------------------------------------------------
    /// Accessor on a pixel value.
    /**
     *  @param i:   the position of the pixel along the x-axis
     *  @param j:   the position of the pixel along the y-axis
     *  @param k:   the position of the pixel along the z-axis (default value: 0)
     *  @return the pixel value
     */
    //--------------------------------------------------------------------------
    const T& getPixelValue(unsigned int i,
                           unsigned int j,
                           unsigned int k = 0) const;


    //--------------------------------------------------------------------------
    /// Accessor on a pixel position.
    /**
     *  @param i:   the position of the pixel along the x-axis
     *  @param j:   the position of the pixel along the y-axis
     *  @param k:   the position of the pixel along the z-axis (default value: 0)
     *  @return the pixel position
     */
    //--------------------------------------------------------------------------
    VEC3 getPixelPosition(unsigned int i,
                          unsigned int j,
                          unsigned int k = 0) const;


    T& operator[](unsigned anIndex);


    const T& operator[](unsigned anIndex) const;


    T& operator()(unsigned int i, unsigned int j, unsigned int k);


    const T& operator()(unsigned int i, unsigned int j, unsigned int k) const;


    //--------------------------------------------------------------------------
    /// Assignment operator (also called copy operator).
    /**
     *  @param anImage: the image to copy
     *  @return the updated version of the current image
     */
    //--------------------------------------------------------------------------
    Image<T>& operator=(const Image<T>& anImage);


    //--------------------------------------------------------------------------
    /// Assignment operator (also called copy operator).
    /**
     *  @param apImage: the data to copy
     *  @return the updated version of the current image
     */
    //--------------------------------------------------------------------------
    Image<T>& operator=(const T* apImage);


    //--------------------------------------------------------------------------
    /// Release the memory.
    //--------------------------------------------------------------------------
    void destroy();


    //--------------------------------------------------------------------------
    /// Accessor on the raw data.
    /**
     *  @return the raw data
     */
    //--------------------------------------------------------------------------
    T* getRawData();


    //--------------------------------------------------------------------------
    /// Accessor on the raw data.
    /**
     *  @return the raw data
     */
    //--------------------------------------------------------------------------
    const T* getRawData() const;


    //--------------------------------------------------------------------------
    /// Number of pixels along the horizontal axis
    /**
     *  @return the width
     */
    //--------------------------------------------------------------------------
    unsigned int getWidth() const;


    //--------------------------------------------------------------------------
    /// Number of pixels along the vertical axis
    /**
     *  @return the height
     */
    //--------------------------------------------------------------------------
    unsigned int getHeight() const;


    //--------------------------------------------------------------------------
    /// Number of pixels along the Z axis
    /**
     *  @return the depth
     */
    //--------------------------------------------------------------------------
    unsigned int getDepth() const;


    void setSpacing(const VEC3& aPixelSize);
    void setSpacing(const double& aPixelWidth,
            const double& aPixelHeight = 0.0,
            const double& aPixelDepth = 0.0);


    const VEC3& getSpacing() const;


    //--------------------------------------------------------------------------
    /// Compute the minimum pixel value in the image
    /**
     *  @return the minimum pixel
     */
    //--------------------------------------------------------------------------
    T getMinValue() const;


    //--------------------------------------------------------------------------
    /// Compute the maximum pixel value in the image
    /**
     *  @return the maximum pixel
     */
    //--------------------------------------------------------------------------
    T getMaxValue() const;


    //--------------------------------------------------------------------------
    /// Compute the minimum pixel value in a given slice
    /**
     *  @param aSliceID: the slice ID
     *  @return the minimum pixel
     */
    //--------------------------------------------------------------------------
    T getMinValue(unsigned int aSliceID) const;


    //--------------------------------------------------------------------------
    /// Compute the maximum pixel value in a given slice
    /**
     *  @param aSliceID: the slice ID
     *  @return the maximum pixel
     */
    //--------------------------------------------------------------------------
    T getMaxValue(unsigned int aSliceID) const;


    //--------------------------------------------------------------------------
    /// Add aShiftValue to every pixel, then multiply every pixel
    /// by aScaleValue
    /**
     *  @param aShiftValue: the shift parameter of the filter
     *  @param aScaleValue: the scale parameter of the filter
     */
    //--------------------------------------------------------------------------
    Image shiftScaleFilter(double aShiftValue,
    		double aScaleValue,
    		bool aRunOnGPUFlag = true) const;


    Image<unsigned char> threshold(const T& aLowerThreshold, const T& aUpperThreshold,
            unsigned char anInValue,
            unsigned char anOutValue,
			bool aRunOnGPUFlag = true) const;

    Image<T> convolution1D(const std::vector<float>& aKernel) const;
    Image<T> convolution2D(const Image<float>& aKernel) const;
    Image<T> convolution2D(const Image<double>& aKernel) const;

    Image<unsigned char> opening(unsigned int aRadius) const;
    Image<unsigned char> closing(unsigned int aRadius) const;
    Image<float> getIsoTropic() const;
    Image<float> antiAliasBinaryImageFilter(double aMaximumRMSChange, unsigned int aNumberOfIterations, double aVariance) const;


    Image constantPadFilter(const T& aPadValue) const;


    Image<unsigned char> threshold(const T& aThreshold,
        unsigned char aValueIn = 1,
        unsigned char aValueOut = 0) const;
    
    
    Image<unsigned char> threshold(const T& aLowerThreshold,
        const T& aUpperThreshold,
        unsigned char aValueIn = 1,
        unsigned char aValueOut = 0) const;


    //--------------------------------------------------------------------------
    /// Normalise the image between 0 and 1
    //--------------------------------------------------------------------------
    Image normalised(bool aRunOnGPUFlag = true) const;


    //--------------------------------------------------------------------------
    /// Normalize the image between 0 and 1
    //--------------------------------------------------------------------------
    Image normalized(bool aRunOnGPUFlag = true) const;


    void load(const char* aFileName);


    void load(const std::string& aFileName);


    void loadUsingITK(const char* aFileName);


    void loadUsingITK(const std::string& aFileName);


    void loadSeries(const char* aPattern,
                    int aFirstSliceIndex,
                    int aLastSliceIndex,
                    int anIncrementIndex = 1);


    void loadSeries(const std::string& aPattern,
                    int aFirstSliceIndex,
                    int aLastSliceIndex,
                    int anIncrementIndex = 1);


    void loadDicomSeries(const char* aDirectory);


    void loadDicomSeries(const std::string& aDirectory);


    void loadASCII(const char* aFileName);


    void loadASCII(const std::string& aFileName);


    void loadASCII(const char* aFileName, unsigned int aWidth, unsigned int aHeight, unsigned int aDepth, const VEC3& aVoxelSize);


    void loadASCII(const std::string& aFileName, unsigned int aWidth, unsigned int aHeight, unsigned int aDepth, const VEC3& aVoxelSize);


    void loadMHD(const char* aFileName);


    void loadMHD(const std::string& aFileName);


    void loadMHA(const char* aFileName);


    void loadMHA(const std::string& aFileName);


    unsigned int sizeOf(const char* anElementType) const;

	unsigned int sizeOf(const std::string& anElementType) const;

    void loadRAW(const char* aFileName,
                 unsigned int aWidth,
                 unsigned int aHeight,
                 unsigned int aNumberOfSlices = 1,
                 bool aChangeEndianessFlag = false,
                 const char* anElementType = 0);


    void loadRAW(const std::string& aFileName,
                 unsigned int aWidth,
                 unsigned int aHeight,
                 unsigned int aNumberOfSlices = 1,
                 bool aChangeEndianessFlag = false,
                 const std::string& anElementType = "");


    void loadTIFF(const char* aFileName);

    
    void loadTIFF(const std::string& aFileName);


    //--------------------------------------------------------------------------
    /// Save the image in a file
    /**
     *  @param aFileName: the name of the file to write
     *  @param anInvertLUTFlag: invert the LUT if it is supported by 
     *                          the file format (default value: false)
     *  @param aComment:        add a comment if it is supported by 
     *                          the file format (default value: "")
     *  @param useDeflateCompressionIfPossible: use compression if possible
     *                                          (default value: false)
     */
    //--------------------------------------------------------------------------
    void save(const char* aFileName,
              bool anInvertLUTFlag = false,
              const char* aComment = "",
          bool useDeflateCompressionIfPossible = false) const;


    //--------------------------------------------------------------------------
    /// Save the image in a file
    /**
     *  @param aFileName: the name of the file to write
     *  @param anInvertLUTFlag: invert the LUT if it is supported by 
     *                          the file format (default value: false)
     *  @param aComment:        add a comment if it is supported by 
     *                          the file format (default value: "")
     *  @param useDeflateCompressionIfPossible: use compression if possible
     *                                          (default value: false)
     */
    //--------------------------------------------------------------------------
    void save(const std::string& aFileName,
              bool anInvertLUTFlag = false,
              const std::string& aComment = "",
              bool useDeflateCompressionIfPossible = false) const;


    //--------------------------------------------------------------------------
    /// Save the image in a PGM file
    /**
     *  @param aFileName: the name of the file to write
     */
    //--------------------------------------------------------------------------
    void savePGM(const char* aFileName) const;


    //--------------------------------------------------------------------------
    /// Save the image in a PGM file
    /**
     *  @param aFileName: the name of the file to write
     */
    //--------------------------------------------------------------------------
    void savePGM(const std::string& aFileName) const;


    //--------------------------------------------------------------------------
    /// Save the image in a Raw file
    /**
     *  @param aFileName: the name of the file to write
     *  @param useDeflateCompressionIfPossible: use compression (default value: false)
     */
    //--------------------------------------------------------------------------
    void saveRaw(const char* aFileName, bool useDeflateCompressionIfPossible) const;


    //--------------------------------------------------------------------------
    /// Save the image in a Raw file
    /**
     *  @param aFileName: the name of the file to write
     *  @param useDeflateCompressionIfPossible: use compression (default value: false)
     */
    //--------------------------------------------------------------------------
    void saveRaw(const std::string& aFileName, bool useDeflateCompressionIfPossible) const;


    //--------------------------------------------------------------------------
    /// Save the image in a Raw file
    /**
     *  @param aFileName: the name of the file to write
     *  @param useDeflateCompressionIfPossible: use compression (default value: false)
     */
    //--------------------------------------------------------------------------
    void saveRAW(const char* aFileName, bool useDeflateCompressionIfPossible) const;


    //--------------------------------------------------------------------------
    /// Save the image in a Raw file
    /**
     *  @param aFileName: the name of the file to write
     *  @param useDeflateCompressionIfPossible: use compression (default value: false)
     */
    //--------------------------------------------------------------------------
    void saveRAW(const std::string& aFileName, bool useDeflateCompressionIfPossible) const;


    //--------------------------------------------------------------------------
    /// Save the image in an ASCII file
    /**
     *  @param aFileName: the name of the file to write
     */
    //--------------------------------------------------------------------------
    void saveASCII(const char* aFileName) const;


    //--------------------------------------------------------------------------
    /// Save the image in an ASCII file
    /**
     *  @param aFileName: the name of the file to write
     */
    //--------------------------------------------------------------------------
    void saveASCII(const std::string& aFileName) const;


    //--------------------------------------------------------------------------
    /// Save the image in a MHD+RAW file
    /**
     *  @param aFileName: the name of the file to write
     *  @param useDeflateCompressionIfPossible: use compression (default value: false)
     */
    //--------------------------------------------------------------------------
    void saveMHD(const char* aFileName,
        bool useDeflateCompressionIfPossible = false) const;


    //--------------------------------------------------------------------------
    /// Save the image in a MHD+RAW file
    /**
     *  @param aFileName: the name of the file to write
     *  @param useDeflateCompressionIfPossible: use compression (default value: false)
     */
    //--------------------------------------------------------------------------
    void saveMHD(const std::string& aFileName,
        bool useDeflateCompressionIfPossible = false) const;


    //--------------------------------------------------------------------------
    /// Save the image in a MHA+RAW file
    /**
     *  @param aFileName: the name of the file to write
     *  @param useDeflateCompressionIfPossible: use compression (default value: false)
     */
    //--------------------------------------------------------------------------
    void saveMHA(const char* aFileName,
        bool useDeflateCompressionIfPossible = false) const;


    //--------------------------------------------------------------------------
    /// Save the image in a MHA+RAW file
    /**
     *  @param aFileName: the name of the file to write
     *  @param useDeflateCompressionIfPossible: use compression (default value: false)
     */
    //--------------------------------------------------------------------------
    void saveMHA(const std::string& aFileName,
        bool useDeflateCompressionIfPossible = false) const;


    //--------------------------------------------------------------------------
    /// Save the image in a DICOM file
    /**
     *  @param aFileName: the name of the file to write
     *  @param anInvertLUTFlag: invert the LUT (default value: false)
     *  @param aComment:        add a comment (default value: "")
     */
    //--------------------------------------------------------------------------
    void saveDCM(const char* aFileName,
                 bool anInvertLUTFlag = false,
                 const char* aComment = "") const;


    //--------------------------------------------------------------------------
    /// Save the image in a DICOM file
    /**
     *  @param aFileName: the name of the file to write
     *  @param anInvertLUTFlag: invert the LUT (default value: false)
     *  @param aComment:        add a comment (default value: "")
     */
    //--------------------------------------------------------------------------
    void saveDCM(const std::string& aFileName,
                 bool anInvertLUTFlag = false,
                 const std::string& aComment = "") const;


    //--------------------------------------------------------------------------
    /// Save the image in a TIFF file
    /**
     *  @param aFileName: the name of the file to write
     *  @param anInvertLUTFlag: invert the LUT (default value: false)
     */
    //--------------------------------------------------------------------------
    void saveTIFF(const char* aFileName,
                  bool anInvertLUTFlag = false) const;


    //--------------------------------------------------------------------------
    /// Save the image in a TIFF file
    /**
     *  @param aFileName: the name of the file to write
     *  @param anInvertLUTFlag: invert the LUT (default value: false)
     */
    //--------------------------------------------------------------------------
    void saveTIFF(const std::string& aFileName,
                  bool anInvertLUTFlag = false) const;


    //--------------------------------------------------------------------------
    /// Save the image in a file using ITK
    /**
     *  @param aFileName: the name of the file to write
     *  @param useDeflateCompressionIfPossible: use compression (default value: false)
     */
    //--------------------------------------------------------------------------
    void saveUsingITK(const char* aFileName,
        bool useDeflateCompressionIfPossible = false) const;


    //--------------------------------------------------------------------------
    /// Save the image in a file using ITK
    /**
     *  @param aFileName: the name of the file to write
     *  @param useDeflateCompressionIfPossible: use compression (default value: false)
     */
    //--------------------------------------------------------------------------
    void saveUsingITK(const std::string& aFileName,
        bool useDeflateCompressionIfPossible = false) const;


    //--------------------------------------------------------------------------
    /// Operator Equal to
    /**
     *  @param anImage: the image to compare with
     *  @return true if the images are similar,
     *         false if they are different
     */
    //--------------------------------------------------------------------------
    bool operator==(const Image<T>& anImage) const;


    //--------------------------------------------------------------------------
    /// Operator Not equal to
    /**
     *  @param anImage: the image to compare with
     *  @return true if the images are different,
     *         false if they are similar
     */
    //--------------------------------------------------------------------------
    bool operator!=(const Image<T>& anImage) const;


    //--------------------------------------------------------------------------
    /// Addition operator. Add anImage
    /**
     *  @param anImage: the image to add
     *  @return the resulting image
     */
    //--------------------------------------------------------------------------
    Image operator+(const Image<T>& anImage) const;


    //--------------------------------------------------------------------------
    /// Subtraction operator. Add anImage
    /**
     *  @param anImage: the image to subtract
     *  @return the resulting image
     */
    //--------------------------------------------------------------------------
    Image operator-(const Image<T>& anImage) const;


    //--------------------------------------------------------------------------
    /// Multiplication operator. Add anImage
    /**
     *  @param anImage: the image to add
     *  @return the resulting image
     */
    //--------------------------------------------------------------------------
    Image operator*(const Image<T>& anImage) const;


    //--------------------------------------------------------------------------
    /// Division operator. Add anImage
    /**
     *  @param anImage: the image to subtract
     *  @return the resulting image
     */
    //--------------------------------------------------------------------------
    Image operator/(const Image<T>& anImage) const;


    //--------------------------------------------------------------------------
    /// Addition assignment operator. Add anImage
    /**
     *  @param anImage: the image to add
     *  @return the updated version of the current image
     */
    //--------------------------------------------------------------------------
    Image<T>& operator+=(const Image<T>& anImage);


    //--------------------------------------------------------------------------
    /// Subraction assignment operator. Add anImage
    /**
     *  @param anImage: the image to subtract
     *  @return the updated version of the current image
     */
    //--------------------------------------------------------------------------
    Image<T>& operator-=(const Image<T>& anImage);


    //--------------------------------------------------------------------------
    /// Multiplication assignment operator. Add anImage
    /**
     *  @param anImage: the image to add
     *  @return the updated version of the current image
     */
    //--------------------------------------------------------------------------
    Image<T>& operator*=(const Image<T>& anImage);


    //--------------------------------------------------------------------------
    /// Division assignment operator. Add anImage
    /**
     *  @param anImage: the image to subtract
     *  @return the updated version of the current image
     */
    //--------------------------------------------------------------------------
    Image<T>& operator/=(const Image<T>& anImage);


    //--------------------------------------------------------------------------
    /// Addition operator. Add aValue to every pixel of the image
    /**
     *  @param aValue: the value to add
     *  @return the resulting image
     */
    //--------------------------------------------------------------------------
    Image operator+(T aValue) const;


    //--------------------------------------------------------------------------
    /// Subtraction operator. Subtract aValue to every pixel of the image
    /**
     *  @param aValue: the value to subtract
     *  @return the resulting image
     */
    //--------------------------------------------------------------------------
    Image operator-(T aValue) const;


    //--------------------------------------------------------------------------
    /// Multiplication operator. Multiply every pixel of the image by aValue
    /**
     *  @param aValue: the value for the multiplication
     *  @return the resulting image
     */
    //--------------------------------------------------------------------------
    Image operator*(T aValue) const;


    //--------------------------------------------------------------------------
    /// Division operator. Divide every pixel of the image by aValue
    /**
     *  @param aValue: the value for the division
     *  @return the resulting image
     */
    //--------------------------------------------------------------------------
    Image operator/(T aValue) const;


    //--------------------------------------------------------------------------
    /// Addition operator. Add aValue to every pixel of the image
    /**
     *  @param aValue: the value to add
     *  @return the updated version of the current image
     */
    //--------------------------------------------------------------------------
    Image<T>& operator+=(T aValue);


    //--------------------------------------------------------------------------
    /// Subtraction operator. Subtract aValue to every pixel of the image
    /**
     *  @param aValue: the value to subtract
     *  @return the updated version of the current image
     */
    //--------------------------------------------------------------------------
    Image<T>& operator-=(T aValue);


    //--------------------------------------------------------------------------
    /// Multiplication operator. Multiply every pixel of the image by aValue
    /**
     *  @param aValue: the value for the multiplication
     *  @return the updated version of the current image
     */
    //--------------------------------------------------------------------------
    Image<T>& operator*=(T aValue);


    //--------------------------------------------------------------------------
    /// Division operator. Divide every pixel of the image by aValue
    /**
     *  @param aValue: the value for the division
     *  @return the updated version of the current image
     */
    //--------------------------------------------------------------------------
    Image<T>& operator/=(T aValue);


    //--------------------------------------------------------------------------
    /// Negation operator. Compute the negative of the current image.
    /**
     *  @return the negative image
     */
    //--------------------------------------------------------------------------
    Image operator!() const;


    Image<T> abs() const;


    Image<T> log() const;


    Image<T> flipVertically() const;


    Image<T> flipHorizontally() const;


    double getSum() const;


    double getAverage() const;


    double getVariance() const;


    double getStandardDeviation() const;


    double computeSAE(const Image<T>& anImage) const;


    double computeMAE(const Image<T>& anImage) const;


    double computeSSE(const Image<T>& anImage) const;


    double computeMSE(const Image<T>& anImage) const;


    double computeRMSE(const Image<T>& anImage) const;


    double computeNCC(const Image<T>& anImage) const;


    gVirtualXRay::FFT<T> getFFT() const;


    Image<T> transpose() const;


    Image<T> resize(unsigned int aNewWidth,
                    unsigned int aNewHeightunsigned,
                    unsigned int aNewNumberOfSlices) const;


    Image<T> rotate(float anAngleInDegrees) const;


    Image<T> setCanvasSize(unsigned int aNewWidth,
                           unsigned int aNewHeight,
                           unsigned int aNewNumberOfSlices) const;


    gVirtualXRay::Sinogram<T> radonTransform(double aFirstAngle,
                                             double anAngleStep,
                                             double aLastAngle) const;


    //--------------------------------------------------------------------------
    /// Apply the marching cubes algorithm to extract the polygon mesh
    /// corresponding to voxel values in the range between aFirstThresholdValue
    /// and aSecondThresholdValue.
    /**
     *  @param aFirstThresholdValue:    the first threshold
     *  @param aSecondThresholdValue:   the second threshold (optional)
     *  @param aTwoThresholdFlag:   if the flag is false, then use 
     *                              aFirstThresholdValue as iso-value.
     *                              If the flag is true, then use voxel values 
     *                              in the range between aFirstThresholdValue
     *                              and aSecondThresholdValue.
     *                              (default value: false)
     *  @param aVerboseLevel:   the level of statements in the standard output. For no output, use 0 (default value: 0)
     *  @return the polygon mesh
     */
    //--------------------------------------------------------------------------
    PolygonMesh marchingCubes(const T& aFirstThresholdValue,
        const T& aSecondThresholdValue = 0,
        bool aTwoThresholdFlag = false,
        unsigned char aVerboseLevel = 0) const;


    //--------------------------------------------------------------------------
    /// Convert every voxel into a cube.
    /**
     *  @param aPolygonMeshSet: the set of polygon mesh
     *  @param aThreshold:      the iso-value. Any voxel above the iso-value
     *                          is part of the selected object. Other voxels
     *                          are ignored.
     *  @param aVerboseLevel:   the level of statements in the standard output. For no output, use 0 (default value: 0)
     */
    //--------------------------------------------------------------------------
    void convertVoxelsToCubes(std::map<T, PolygonMesh>& aPolygonMeshSet,                     
        T aThreshold = -1000,
        unsigned char aVerboseLevel = 0) const;


    Image<T> convertHU2mu(double anEnergy) const;


    void allocateHostMemory();


    // Load the image as an OpenGL texture
    void transferHostToDevice();


    void transferDeviceToHost();


    // Destroy the OpenGL texture of the image if any
    void destroyOpenGLTexture();


    unsigned int getTextureId() const;


//******************************************************************************
protected:
    VEC3 VertexInterp(const T& aThresholdValue,
            const VEC3& p0,
            const VEC3& p1,
            const T& v0,
            const T& v1) const;


    /// Number of pixel along the horizontal axis
    unsigned int m_width;


    /// Number of pixel along the vertical axis
    unsigned int m_height;


    /// Number of slices
    unsigned int m_number_of_slices;


    /// The space between two successive pixels
    VEC3 m_spacing;


    /// The pixel data
    T* m_p_image;


    /// OpenGL texture ID
    unsigned int m_texture_id;


    static unsigned int g_mesh_type;


    static unsigned int g_vbo_type;


//******************************************************************************
private:
    void loadMetaHeader(const char* aFileName);


    void loadMetaHeader(const std::string& aFileName);


    void loadRAW(std::ifstream& anInputStream,
                 unsigned int aWidth,
                 unsigned int aHeight,
                 unsigned int aNumberOfSlices,
                 bool aChangeEndianessFlag,
                 const char* aFileName,
                 const char* anElementType/*,
                 bool aCompressionFlag*/);


    void saveMetaHeader(const char* aFileName, bool useDeflateCompressionIfPossible) const;


    void saveMetaHeader(const std::string& aFileName, bool useDeflateCompressionIfPossible) const;


#ifdef HAS_GDCM
    void addTag(double aValue,
                unsigned int anAddress1,
                unsigned int anAddress2,
                gdcm::File& aFile) const;


    void addTag(const char* aValue,
                unsigned int anAddress1,
                unsigned int anAddress2,
                gdcm::File& aFile) const;


    void addTag(const std::string& aValue,
                unsigned int anAddress1,
                unsigned int anAddress2,
                gdcm::File& aFile) const;
#endif

};


void convert(const Image<float>& aSourceImage, Image<unsigned short>& aTargetImage);
void convert(const Image<unsigned short>& aSourceImage, Image<float>& aTargetImage);


} // namespace gVirtualXRay


//******************************************************************************
#include "Image.inl"


#endif // GVXR_IMAGE_H
