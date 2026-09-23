//******************************************************************************
//  Include
//******************************************************************************
#include <iostream>
#include <iomanip>

#include <fstream>
#include <vector>
#include <cmath>
#include <numeric>

#include "gtest/gtest.h"

#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#ifndef GVXR_IMAGE_H
#include "gVirtualXRay/Image.h"
#endif

#ifndef GVXR_CONTEXT_H
#include "gVirtualXRay/Context.h"
#endif

/*
#ifndef GVXR_INVALID_INTERNAL_FORMAT_EXCEPTION_H
#include "gVirtualXRay/InvalidInternalFormalException.h"
#endif

#ifndef GVXR_PHOTON_CROSS_SECTION_H
#include "gVirtualXRay/PhotonCrossSection.h"
#endif

#ifndef GVXR_INVALID_IMAGE_SIZE_EXCEPTION_H
#include "gVirtualXRay/InvalidImageSizeException.h"
#endif

#ifndef GVXR_NO_DETECTOR_EXCEPTION_H
#include "gVirtualXRay/NoDetectorException.h"
#endif

#ifndef GVXR_NO_BEAM_EXCEPTION_H
#include "gVirtualXRay/NoBeamException.h"
#endif
*/

#ifndef GVXR_utils_h
#include "utils.h"
#endif


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//  Function declaration
//******************************************************************************


//------------------------------------
TEST(TestImageClass, TestConstructors)
//------------------------------------
{
    // Image();
    // Image(const Image<T>& anImage);
    // Image(const char* aFileName);
    // Image(const std::string& aFileName);
    // Image(const T* apData,
    //              unsigned int aWidth,
    //              unsigned int aHeight,
    //              unsigned int aNumberOfSlices = 1,
    //              const VEC3& aVoxelSize = VEC3(1, 1, 1));
    // Image(unsigned int aWidth,
    //              unsigned int aHeight,
    //              unsigned int aNumberOfSlices = 1,
    //              T aDefaultValue = 0);
}


//---------------------------------------------
TEST(TestImageClass, TestOpenGLThresholdingGPU)
//---------------------------------------------
{
	double sigma = 10;
	int kernel_size = 2 * ceil(3 * sigma) + 1;

	Image<float> float_2d_kernel = gauss2D<float>(kernel_size, sigma).normalised(false);

    Image<unsigned char> filtered_cpu = float_2d_kernel.threshold(0.25f, 0.75f, (unsigned char)(128), (unsigned char)(255), false);

	// Create an OpenGL context
	Context GL_context;
	initGL(GL_context, 4, 3);
    Image<unsigned char> filtered_gpu = float_2d_kernel.threshold(0.25f, 0.75f, (unsigned char)(128), (unsigned char)(255), true);

	EXPECT_EQ(filtered_cpu, filtered_gpu);
}


//-------------------------------------------
TEST(TestImageClass, TestOpenGLShiftScaleGPU)
//-------------------------------------------
{
	double sigma = 10;
	int kernel_size = 2 * ceil(3 * sigma) + 1;

	Image<float> float_2d_kernel = gauss2D<float>(kernel_size, sigma).normalised(false);
//    float_2d_kernel.save("float_2d_kernel.mha");

    Image<float> filtered_cpu = float_2d_kernel.shiftScaleFilter(0.25f, 0.75f, false);
    filtered_cpu.save("filtered_cpu.mha");

	// Create an OpenGL context
	Context GL_context;
	initGL(GL_context, 4, 3);
    Image<float> filtered_gpu = float_2d_kernel.shiftScaleFilter(0.25f, 0.75f, true);
    filtered_gpu.save("filtered_gpu.mha");

	EXPECT_EQ(filtered_cpu, filtered_gpu);
}

//------------------------------------
TEST(TestImageClass, TestMakeBlank)
//------------------------------------
{
    // void makeBlank(const T& aDefaultColour = 0);
}


//-------------------------------------
TEST(TestImageClass, TestConvolution2D)
//-------------------------------------
{
}


//-------------------------------
TEST(TestImageClass, TestPadding)
//-------------------------------
{
    // const Image<unsigned char> input(2, 2, 2, 128);

    // Image<unsigned char> output = input.constantPadFilter(255);

    // const unsigned char* p_buffer = output.getRawData();

    // ASSERT_EQ(1, 1);
}


// //------------------------------------
// TEST(TestImageClass, Test)
// //------------------------------------
// {
// }


// //------------------------------------
// TEST(TestImageClass, Test)
// //------------------------------------
// {
// }


// //------------------------------------
// TEST(TestImageClass, Test)
// //------------------------------------
// {
// }


// //------------------------------------
// TEST(TestImageClass, Test)
// //------------------------------------
// {
// }


// //------------------------------------
// TEST(TestImageClass, Test)
// //------------------------------------
// {
// }


// //------------------------------------
// TEST(TestImageClass, Test)
// //------------------------------------
// {
// }


// //------------------------------------
// TEST(TestImageClass, Test)
// //------------------------------------
// {
// }


// //------------------------------------
// TEST(TestImageClass, Test)
// //------------------------------------
// {
// }


// //------------------------------------
// TEST(TestImageClass, Test)
// //------------------------------------
// {
// }


// //------------------------------------
// TEST(TestImageClass, Test)
// //------------------------------------
// {
// }


