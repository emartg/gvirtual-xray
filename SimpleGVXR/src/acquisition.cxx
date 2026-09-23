/*

Copyright (c) 2024-present, Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), 
UK Research and Innovation, All rights reserved.

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


/**
********************************************************************************
*
*   @file       acquisition.cxx
*
*   @brief      Expose here all the functions of SimpleGVXR related to 
*               the X-ray image acquisition, inc. radiographs, path lenghts and CT.
*
*   @version    1.0
*
*   @date       05/06/2024
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
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, May 2024-present, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/

//******************************************************************************
//  Include
//******************************************************************************
#include <cmath>

#include <map>
#include <iostream>
#include <sstream>
#include <memory>
#include <vector>
#include <algorithm>
// #include <ranges> // C++ 23

#ifndef GVXR_XRAY_RENDERER_H
#include "gVirtualXRay/XRayRenderer.h"
#endif

#ifndef GVXR_MATRIX_4X4_H
#include "gVirtualXRay/Matrix4x4.h"
#endif

#ifndef GVXR_XRAY_BEAM_H
#include "gVirtualXRay/XRayBeam.h"
#endif

#ifndef GVXR_IMAGE_H
#include "gVirtualXRay/Image.h"
#endif

#ifndef GVXR_SCENEGRAPH_BINDER_H
#include "gVirtualXRay/SceneGraphBinder.h"
#endif

#ifndef GVXR_SCENEGRAPH_NODE_H
#include "gVirtualXRay/SceneGraphNode.h"
#endif

#ifndef GVXR_NOT_IMPLEMENTED_EXCEPTION_H
#include "gVirtualXRay/NotImplementedException.h"
#endif

#ifndef SGVXR_Internal_h
#include "internal.h"
#endif

#ifndef SGVXR_Acquisition_h
#include "acquisition.h"
#endif

#ifndef SGVXR_Detector_h
#include "detector.h"
#endif

#ifndef SGVXR_Sample_h
#include "sample.h"
#endif

#ifndef SGVXR_Source_h
#include "source.h"
#endif

#ifndef SGVXR_Window_h
#include "Window.h"
#endif

#ifndef SGVXR_SimpleGVXR_h
#include "SimpleGVXR.h"
#endif


//******************************************************************************
//  Namespace
//******************************************************************************


//******************************************************************************
//  Global variables
//******************************************************************************
extern gVirtualXRay::XRayRenderer g_xray_renderer; // X-ray graphics engine
extern gVirtualXRay::XRayDetector g_xray_detector; // X-ray detector
extern bool g_has_own_gl_context;
extern std::map<int, std::unique_ptr<SimpleGVXR::Window> > g_p_window_set;
extern std::vector<int> g_p_window_id_stack;
extern gVirtualXRay::Matrix4x4<GLfloat> g_sample_rotation_matrix;

extern bool g_beam_energy_initialised;
extern bool g_beam_shape_initialised;
extern bool g_source_position_initialised;
extern bool g_detector_up_vector_initialised;
extern bool g_detector_size_initialised;
extern bool g_detector_pixel_size_initialised;
extern bool g_renderer_initialised;

//extern int g_number_of_bits_per_float;

extern gVirtualXRay::XRayBeam g_xray_beam;
extern std::unique_ptr<gVirtualXRay::SceneGraphBinder> g_p_scene_graph;

unsigned short g_white_level = 0;


//******************************************************************************
//  Function definitions
//******************************************************************************


//-------------------------------------------------------
void internalComputeXRayImage(bool anIntegrateEnergyFlag)
//-------------------------------------------------------
{
    // There is no OpenGL context
    if (!g_has_own_gl_context && (g_p_window_id_stack.empty() || g_p_window_set.empty()))
    {
        // Create a window
        createWindow();
    }

    // The renderer is ready
    if (g_xray_renderer.isReady())
    {
        // Compute the X-ray image
        g_xray_renderer.computeImage(g_sample_rotation_matrix, anIntegrateEnergyFlag);
    }
    // The renderer is not ready
    else
    {
        gVirtualXRay::LOGGER.logWarning("X-ray renderer not ready.") <<  std::endl;

        if (!g_beam_energy_initialised)
            gVirtualXRay::LOGGER.logWarning("The energy spectrum has not been set.") <<  std::endl;

        if (!g_beam_shape_initialised)
            gVirtualXRay::LOGGER.logWarning("The beam shape has not been set.") <<  std::endl;

        if (!g_source_position_initialised)
            gVirtualXRay::LOGGER.logWarning("The source's position has not been set.") <<  std::endl;

        if (!g_detector_up_vector_initialised)
            gVirtualXRay::LOGGER.logWarning("The detector's orientation has not been set.") <<  std::endl;

        if (!g_detector_size_initialised)
            gVirtualXRay::LOGGER.logWarning("The detector's pixel resolution has not been set.") <<  std::endl;

        if (!g_detector_pixel_size_initialised)
            gVirtualXRay::LOGGER.logWarning("The detector's pixel spacing has not been set.") <<  std::endl;
    }
}


//---------------------------------------------------------------------------
std::vector<std::vector<float> > computeXRayImage(bool anIntegrateEnergyFlag)
//---------------------------------------------------------------------------
{
    internalComputeXRayImage(anIntegrateEnergyFlag);

    // The renderer is not ready
    if (!g_xray_renderer.isReady()) return {};

#ifdef HAS_CUDA
    if (g_samples_per_pixels == 1)
    {
        return (getVectorImage2D(g_xray_renderer.getEnergyFluence()));
    }
    else
    {
        resampleUsingCUDA();
        gVirtualXRay::LOGGER.logWarning("Image size: ") << g_xray_renderer.getEnergyFluence().getWidth() << "x" << g_xray_renderer.getEnergyFluence().getHeight() << "x" << g_xray_renderer.getEnergyFluence().getDepth() <<  std::endl;
        gVirtualXRay::LOGGER.logWarning("Image size: ") << g_cpu_memory.getWidth() << "x" << g_cpu_memory.getHeight() << "x" << g_cpu_memory.getDepth() <<  std::endl;
        return (getVectorImage2D(g_cpu_memory));
    }
#else
    return (getVectorImage2D(g_xray_renderer.getEnergyFluence()));
#endif
}


//--------------------------------------------------------------------------------------------
std::vector<std::vector<unsigned short> > computeXRayImageWithGain(bool anIntegrateEnergyFlag)
//--------------------------------------------------------------------------------------------
{
    internalComputeXRayImage(anIntegrateEnergyFlag);

    // The renderer is not ready
    if (!g_xray_renderer.isReady()) return {};

#ifdef HAS_CUDA
    if (g_samples_per_pixels == 1)
    {
        return (getVectorImage2D(g_xray_renderer.getEnergyFluenceWithGain()));
    }
    else
    {
        resampleUsingCUDA();
        gVirtualXRay::LOGGER.logWarning("Image size: ") << g_xray_renderer.getEnergyFluenceWithGain().getWidth() << "x" << g_xray_renderer.getEnergyFluence().getHeight() << "x" << g_xray_renderer.getEnergyFluence().getDepth() <<  std::endl;
        gVirtualXRay::LOGGER.logWarning("Image size: ") << g_cpu_memory.getWidth() << "x" << g_cpu_memory.getHeight() << "x" << g_cpu_memory.getDepth() <<  std::endl;
        return (getVectorImage2D(g_cpu_memory));
    }
#else
    return (getVectorImage2D(g_xray_renderer.getEnergyFluenceWithGain()));
#endif
}


//----------------------------------------------------------------------------------------
void computeXRayImage(float* apImage, int aHeight, int aWidth, bool anIntegrateEnergyFlag)
//----------------------------------------------------------------------------------------
{
    // There is no OpenGL context
    if (!g_has_own_gl_context && (g_p_window_id_stack.empty() || g_p_window_set.empty())) {
        // Create a window
        createWindow();
    }

    // The renderer is ready
    if (g_xray_renderer.isReady()) {

        std::vector<int> detector_size = getDetectorNumberOfPixels();

        if (aWidth == detector_size[0] && aHeight == detector_size[1]) {
            // Compute the X-ray image
            g_xray_renderer.computeImage(g_sample_rotation_matrix, anIntegrateEnergyFlag);

#ifdef HAS_CUDA
            if (g_samples_per_pixels == 1)
            {
                return (getVectorImage2D(g_xray_renderer.getEnergyFluence()));
            }
            else
            {
                resampleUsingCUDA();
                gVirtualXRay::LOGGER.logWarning("Image size: ") << g_xray_renderer.getEnergyFluence().getWidth() << "x" << g_xray_renderer.getEnergyFluence().getHeight() << "x" << g_xray_renderer.getEnergyFluence().getDepth() << std::endl;
                gVirtualXRay::LOGGER.logWarning("Image size: ") << g_cpu_memory.getWidth() << "x" << g_cpu_memory.getHeight() << "x" << g_cpu_memory.getDepth() << std::endl;
                return (getVectorImage2D(g_cpu_memory));
            }
#else
            // We use this call to make sure LSF and noise are applied
            if (!g_xray_detector.getLSF().empty() || g_xray_detector.getPSF().getRawData() || g_xray_beam.usePoissonNoise())
                std::copy_n(g_xray_renderer.getEnergyFluence().getRawData(),
                    aHeight * aWidth,
                    apImage);
            else
                g_xray_renderer.getFBO(XRAY_DETECTOR_ENERGY_FLUENCE_FBO_ID, apImage, aWidth, aHeight);
#endif
        }
        else {
            gVirtualXRay::LOGGER.logWarning("The detector size is ") << detector_size[0] << "x" << detector_size[1] << " whereas the output array is " << aWidth << "x" << aHeight << std::endl;
        }
    }
    // The renderer is not ready
    else {
        gVirtualXRay::LOGGER.logWarning("X-ray renderer not ready.") << std::endl;

        if (!g_beam_energy_initialised)
            gVirtualXRay::LOGGER.logWarning("The energy spectrum has not been set.") << std::endl;

        if (!g_beam_shape_initialised)
            gVirtualXRay::LOGGER.logWarning("The beam shape has not been set.") << std::endl;

        if (!g_source_position_initialised)
            gVirtualXRay::LOGGER.logWarning("The source's position has not been set.") << std::endl;

        if (!g_detector_up_vector_initialised)
            gVirtualXRay::LOGGER.logWarning("The detector's orientation has not been set.") << std::endl;

        if (!g_detector_size_initialised)
            gVirtualXRay::LOGGER.logWarning("The detector's pixel resolution has not been set.") << std::endl;

        if (!g_detector_pixel_size_initialised)
            gVirtualXRay::LOGGER.logWarning("The detector's pixel spacing has not been set.") << std::endl;
    }
}


//-----------------------------------------------------------------------------------------
void computeXRayImage(double* apImage, int aHeight, int aWidth, bool anIntegrateEnergyFlag)
//-----------------------------------------------------------------------------------------
{
    // There is no OpenGL context
    if (!g_has_own_gl_context && (g_p_window_id_stack.empty() || g_p_window_set.empty())) {
        // Create a window
        createWindow();
    }

    // The renderer is ready
    if (g_xray_renderer.isReady()) {

        std::vector<int> detector_size = getDetectorNumberOfPixels();

        if (aWidth == detector_size[0] && aHeight == detector_size[1]) {
            // Compute the X-ray image
            g_xray_renderer.computeImage(g_sample_rotation_matrix, anIntegrateEnergyFlag);

#ifdef HAS_CUDA
            if (g_samples_per_pixels == 1)
            {
                return (getVectorImage2D(g_xray_renderer.getEnergyFluence()));
            }
            else
            {
                resampleUsingCUDA();
                gVirtualXRay::LOGGER.logWarning("Image size: ") << g_xray_renderer.getEnergyFluence().getWidth() << "x" << g_xray_renderer.getEnergyFluence().getHeight() << "x" << g_xray_renderer.getEnergyFluence().getDepth() << std::endl;
                gVirtualXRay::LOGGER.logWarning("Image size: ") << g_cpu_memory.getWidth() << "x" << g_cpu_memory.getHeight() << "x" << g_cpu_memory.getDepth() << std::endl;
                return (getVectorImage2D(g_cpu_memory));
            }
#else
            // We use this call to make sure LSF and noise are applied
            if (!g_xray_detector.getLSF().empty() || g_xray_detector.getPSF().getRawData() || g_xray_beam.usePoissonNoise())
                std::copy_n(g_xray_renderer.getEnergyFluence().getRawData(),
                    aHeight * aWidth,
                    apImage);
            else
                g_xray_renderer.getFBO(XRAY_DETECTOR_ENERGY_FLUENCE_FBO_ID, apImage, aWidth, aHeight);
#endif
        }
        else {
            gVirtualXRay::LOGGER.logWarning("The detector size is ") << detector_size[0] << "x" << detector_size[1] << " whereas the output array is " << aWidth << "x" << aHeight << std::endl;
        }
    }
    // The renderer is not ready
    else {
        gVirtualXRay::LOGGER.logWarning("X-ray renderer not ready.") << std::endl;

        if (!g_beam_energy_initialised)
            gVirtualXRay::LOGGER.logWarning("The energy spectrum has not been set.") << std::endl;

        if (!g_beam_shape_initialised)
            gVirtualXRay::LOGGER.logWarning("The beam shape has not been set.") << std::endl;

        if (!g_source_position_initialised)
            gVirtualXRay::LOGGER.logWarning("The source's position has not been set.") << std::endl;

        if (!g_detector_up_vector_initialised)
            gVirtualXRay::LOGGER.logWarning("The detector's orientation has not been set.") << std::endl;

        if (!g_detector_size_initialised)
            gVirtualXRay::LOGGER.logWarning("The detector's pixel resolution has not been set.") << std::endl;

        if (!g_detector_pixel_size_initialised)
            gVirtualXRay::LOGGER.logWarning("The detector's pixel spacing has not been set.") << std::endl;
    }
}


//---------------------------------------------------------------------------------------------------------
void computeXRayImageWithGain(unsigned short* apImage, int aHeight, int aWidth, bool anIntegrateEnergyFlag)
//---------------------------------------------------------------------------------------------------------
{
    internalComputeXRayImage(anIntegrateEnergyFlag);

    // The renderer is not ready
    if (!g_xray_renderer.isReady()) {
        std::fill(apImage, apImage + aWidth * aHeight, 0);
    }

    std::copy_n(g_xray_renderer.getEnergyFluenceWithGain().getRawData(),
        aHeight * aWidth,
        apImage);
}


//-------------------------------------------------------------------
std::vector<std::vector<std::vector<float> > > computeSpectralImage()
//-------------------------------------------------------------------
{
    std::vector<std::vector<std::vector<float> > > spectral_image;

    // There is no OpenGL context
    if (!g_has_own_gl_context && (g_p_window_id_stack.empty() || g_p_window_set.empty())) {
        // Create a window
        createWindow();
    }

    // The renderer is ready
    if (g_xray_renderer.isReady()) {

        for (unsigned int i = 0; i < g_xray_beam.getNumberOfPhotonCountingBands(); ++i)
        {
            g_xray_beam.activatePhotonCountingBand(i);

            // Compute the X-ray image
            g_xray_renderer.computeImage(g_sample_rotation_matrix, false);

            spectral_image.push_back(getVectorImage2D(g_xray_renderer.getEnergyFluence()));
        }
// #ifdef HAS_CUDA
//         if (g_samples_per_pixels == 1)
//         {
//             return (getVectorImage2D(g_xray_renderer.getEnergyFluence()));
//         }
//         else
//         {
//             resampleUsingCUDA();
//             gVirtualXRay::LOGGER.logWarning("Image size: ") << g_xray_renderer.getEnergyFluence().getWidth() << "x" << g_xray_renderer.getEnergyFluence().getHeight() << "x" << g_xray_renderer.getEnergyFluence().getDepth() <<  std::endl;
//             gVirtualXRay::LOGGER.logWarning("Image size: ") << g_cpu_memory.getWidth() << "x" << g_cpu_memory.getHeight() << "x" << g_cpu_memory.getDepth() <<  std::endl;
//             return (getVectorImage2D(g_cpu_memory));
//         }
// #else
//         return (getVectorImage2D(g_xray_renderer.getEnergyFluence()));
// #endif

    }
    // The renderer is not ready
    else {
        gVirtualXRay::LOGGER.logWarning("X-ray renderer not ready.") <<  std::endl;

        if (!g_beam_energy_initialised)
            gVirtualXRay::LOGGER.logWarning("The energy spectrum has not been set.") <<  std::endl;

        if (!g_beam_shape_initialised)
            gVirtualXRay::LOGGER.logWarning("The beam shape has not been set.") <<  std::endl;

        if (!g_source_position_initialised)
            gVirtualXRay::LOGGER.logWarning("The source's position has not been set.") <<  std::endl;

        if (!g_detector_up_vector_initialised)
            gVirtualXRay::LOGGER.logWarning("The detector's orientation has not been set.") <<  std::endl;

        if (!g_detector_size_initialised)
            gVirtualXRay::LOGGER.logWarning("The detector's pixel resolution has not been set.") <<  std::endl;

        if (!g_detector_pixel_size_initialised)
            gVirtualXRay::LOGGER.logWarning("The detector's pixel spacing has not been set.") <<  std::endl;

        return {};
    }

    return spectral_image;
}


//------------------------------------------------------------------------
std::vector<std::vector<float> > computeLBuffer(const std::string& aLabel)
//------------------------------------------------------------------------
{
    // There is no OpenGL context
    if (!g_has_own_gl_context && (g_p_window_id_stack.empty() || g_p_window_set.empty()))
    {
        // Create a window
        createWindow();
    }

    // The renderer is ready
    if (g_xray_renderer.isReady())
    {
        if (g_p_scene_graph)
        {
            // Find the mesh
            gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(
            aLabel);

            // It has been found
            if (p_node)
            {
                // Compute the L-buffer
                g_xray_renderer.computeLBuffer(*p_node, gVirtualXRay::VEC3(), g_sample_rotation_matrix, true);

                // Compute its L-buffer
                return (getVectorImage2D(g_xray_renderer.getLBuffer()));
            }
            // It has not been found
            else
            {
                gVirtualXRay::LOGGER.logWarning("PolygonMesh ") << aLabel << " not found in g_p_polygon_mesh_set." <<  std::endl;
            }
        }
        else
        {
            gVirtualXRay::LOGGER.logWarning("The scenegraph is empty.") <<  std::endl;
        }
    }
    // The renderer is not ready
    else
    {
        gVirtualXRay::LOGGER.logWarning("X-ray renderer not ready.") <<  std::endl;

        if (!g_beam_energy_initialised)
            gVirtualXRay::LOGGER.logWarning("The energy spectrum has not been set.") <<  std::endl;

        if (!g_beam_shape_initialised)
            gVirtualXRay::LOGGER.logWarning("The beam shape has not been set.") <<  std::endl;

        if (!g_source_position_initialised)
            gVirtualXRay::LOGGER.logWarning("The source's position has not been set.") <<  std::endl;

        if (!g_detector_up_vector_initialised)
            gVirtualXRay::LOGGER.logWarning("The detector's orientation has not been set.") <<  std::endl;

        if (!g_detector_size_initialised)
            gVirtualXRay::LOGGER.logWarning("The detector's pixel resolution has not been set.") <<  std::endl;

        if (!g_detector_pixel_size_initialised)
            gVirtualXRay::LOGGER.logWarning("The detector's pixel spacing has not been set.") <<  std::endl;
    }

    return {};
}


//---------------------------------------------------------------------------
std::vector<std::vector<float> > computePathLength(const std::string& aLabel)
//---------------------------------------------------------------------------
{
    return computeLBuffer(aLabel);
}


std::vector<std::vector<std::vector<float> > > computeSinogram(double x, double y, double z,
    const std::string& aUnitOfLength, unsigned int aNumberOfAngle,
    double anAngleOffset) {
    // There is no OpenGL context
    if (!g_has_own_gl_context
    && (g_p_window_id_stack.empty() || g_p_window_set.empty())) {
    // Create a window
    createWindow();
    }

    // The renderer is ready
    if (g_xray_renderer.isReady()) {
    // Compute the X-ray image

    double unit_of_length = getUnitOfLength(aUnitOfLength);
    gVirtualXRay::VEC3 rotation_centre(x * unit_of_length,
    y * unit_of_length, z * unit_of_length);

    g_xray_renderer.computeSinogram(g_sample_rotation_matrix,
    rotation_centre, aNumberOfAngle, anAngleOffset);
    }
    // The renderer is not ready
    else {
    gVirtualXRay::LOGGER.logWarning("X-ray renderer not ready.") <<  std::endl;
    }

    return (getVectorImage3D(g_xray_renderer.getSinogram()));
}


std::vector<std::vector<std::vector<float> > > computeProjectionSet(double x, double y, double z,
    const std::string& aUnitOfLength, unsigned int aNumberOfAngle,
    double anAngleOffset) {
    // There is no OpenGL context
    if (!g_has_own_gl_context
    && (g_p_window_id_stack.empty() || g_p_window_set.empty())) {
    // Create a window
    createWindow();
    }

    // The renderer is ready
    if (g_xray_renderer.isReady()) {
    // Compute the X-ray image

    double unit_of_length = getUnitOfLength(aUnitOfLength);
    gVirtualXRay::VEC3 rotation_centre(x * unit_of_length,
    y * unit_of_length, z * unit_of_length);

    g_xray_renderer.computeProjectionSet(g_sample_rotation_matrix,
    rotation_centre, aNumberOfAngle, anAngleOffset);
    }
    // The renderer is not ready
    else {
    gVirtualXRay::LOGGER.logWarning("X-ray renderer not ready.") <<  std::endl;
    }

    return (getVectorImage3D(g_xray_renderer.getProjectionSet()));
}


//--------------------------------------------------------------------
void computeCTAcquisition(const std::string& aProjectionOutputPath,
                          const std::string& aScreenshotOutputPath,
                          unsigned int aNumberOfProjections,
                          float aFirstAngle,
                          bool anIncludeLastAngleFlag,
                          float aLastAngle,
                          unsigned int aNumberOfWhiteImagesInFlatField,
                          float aPositionOfCentreOfRotationX,
                          float aPositionOfCentreOfRotationY,
                          float aPositionOfCentreOfRotationZ,
                          const std::string& aUnitOfLength,
                          float aAxisOfRotationX,
                          float aAxisOfRotationY,
                          float aAxisOfRotationZ,
                          bool anIntegrateEnergyFlag,
                          unsigned int aVerboseLevel)
//--------------------------------------------------------------------
{
    // Backup the gain value
    unsigned short old_gain = getDetectorGain();

    // Disable the gain
    setDetectorGain(1.0);

    g_xray_renderer.computeCTAcquisition(aProjectionOutputPath,
        aScreenshotOutputPath,
        aNumberOfProjections,
        aFirstAngle,
        anIncludeLastAngleFlag,
        aLastAngle,
        aNumberOfWhiteImagesInFlatField,
        gVirtualXRay::VEC3(
            aPositionOfCentreOfRotationX * getUnitOfLength(aUnitOfLength),
            aPositionOfCentreOfRotationY * getUnitOfLength(aUnitOfLength),
            aPositionOfCentreOfRotationZ * getUnitOfLength(aUnitOfLength)),
        gVirtualXRay::VEC3(aAxisOfRotationX, aAxisOfRotationY, aAxisOfRotationZ),
        g_sample_rotation_matrix,
        anIntegrateEnergyFlag,
        g_white_level,
        aVerboseLevel);

    // Restore the gain
    setDetectorGain(old_gain);
}


//----------------------------------------------------------------------------
void computeCTAcquisitionWithGain(const std::string& aProjectionOutputPath,
                                  const std::string& aScreenshotOutputPath,
                                  unsigned int aNumberOfProjections,
                                  float aFirstAngle,
                                  bool anIncludeLastAngleFlag,
                                  float aLastAngle,
                                  unsigned int aNumberOfWhiteImagesInFlatField,
                                  float aPositionOfCentreOfRotationX,
                                  float aPositionOfCentreOfRotationY,
                                  float aPositionOfCentreOfRotationZ,
                                  const std::string& aUnitOfLength,
                                  float aAxisOfRotationX,
                                  float aAxisOfRotationY,
                                  float aAxisOfRotationZ,
                                  bool anIntegrateEnergyFlag,
                                  unsigned int aVerboseLevel)
//----------------------------------------------------------------------------
{
    g_xray_renderer.computeCTAcquisitionWithGain(aProjectionOutputPath,
        aScreenshotOutputPath,
        aNumberOfProjections,
        aFirstAngle,
        anIncludeLastAngleFlag,
        aLastAngle,
        aNumberOfWhiteImagesInFlatField,
        gVirtualXRay::VEC3(
            aPositionOfCentreOfRotationX * getUnitOfLength(aUnitOfLength),
            aPositionOfCentreOfRotationY * getUnitOfLength(aUnitOfLength),
            aPositionOfCentreOfRotationZ * getUnitOfLength(aUnitOfLength)),
        gVirtualXRay::VEC3(aAxisOfRotationX, aAxisOfRotationY, aAxisOfRotationZ),
        g_sample_rotation_matrix,
        anIntegrateEnergyFlag,
        g_white_level,
        aVerboseLevel);
}


//--------------------------------------------
void setWhiteLevel(unsigned short aWhiteLevel)
//--------------------------------------------
{
    g_white_level = aWhiteLevel;
}


//----------------------------
unsigned short getWhiteLevel()
//----------------------------
{
    return g_white_level;
}

//------------------------------------------------------------------------------------
std::vector<std::vector<float>> getSourcePositionSet(const std::string& aUnitOfLength)
//------------------------------------------------------------------------------------
{
    std::vector<gVirtualXRay::VEC3> input = g_xray_renderer.getSourcePositionSet();
    std::vector<std::vector<float>> output;

    auto unit_of_length = static_cast<float>(getUnitOfLength(aUnitOfLength));

    for (auto& ite : input)
    {
        std::vector<float> temp(3);
        temp[0] = ite[0] / unit_of_length;
        temp[1] = ite[1] / unit_of_length;
        temp[2] = ite[2] / unit_of_length;

        output.push_back(temp);
    }

    return output;
}


//--------------------------------------------------------------------------------------
std::vector<std::vector<float>> getDetectorPositionSet(const std::string& aUnitOfLength)
//--------------------------------------------------------------------------------------
{
    std::vector<gVirtualXRay::VEC3> input = g_xray_renderer.getDetectorPositionSet();
    std::vector<std::vector<float>> output;

    auto unit_of_length = static_cast<float>(getUnitOfLength(aUnitOfLength));

    for (auto& ite : input)
    {
        std::vector<float> temp(3);
        temp[0] = ite[0] / unit_of_length;
        temp[1] = ite[1] / unit_of_length;
        temp[2] = ite[2] / unit_of_length;

        output.push_back(temp);
    }

    return output;
}


//---------------------------------------------------------
std::vector<std::vector<float>> getDetectorRightVectorSet()
//---------------------------------------------------------
{
    std::vector<gVirtualXRay::VEC3> input = g_xray_renderer.getDetectorRightVectorSet();
    std::vector<std::vector<float>> output;

    for (auto& ite : input)
    {
        std::vector<float> temp(3);
        temp[0] = ite[0];
        temp[1] = ite[1];
        temp[2] = ite[2];

        output.push_back(temp);
    }

    return output;
}


//------------------------------------------------------
std::vector<std::vector<float>> getDetectorUpVectorSet()
//------------------------------------------------------
{
    std::vector<gVirtualXRay::VEC3> input = g_xray_renderer.getDetectorUpVectorSet();
    std::vector<std::vector<float>> output;

    for (auto& ite : input)
    {
        std::vector<float> temp(3);
        temp[0] = ite[0];
        temp[1] = ite[1];
        temp[2] = ite[2];

        output.push_back(temp);
    }

    return output;
}


//--------------------------------------------
const std::string& getProjectionOutputPathCT()
//--------------------------------------------
{
    return g_xray_renderer.getProjectionOutputPathCT();
}


//--------------------------------------------
const std::string& getScreenshotOutputPathCT()
//--------------------------------------------
{
    return g_xray_renderer.getScreenshotOutputPathCT();
}


//-------------------------------------
unsigned int getNumberOfProjectionsCT()
//-------------------------------------
{
    return g_xray_renderer.getNumberOfProjectionsCT();
}


//--------------------------------------------
unsigned int getOptimalNumberOfProjectionsCT()
//--------------------------------------------
{
    return g_xray_renderer.getOptimalNumberOfProjectionsCT();
}


//---------------------
float getFirstAngleCT()
//---------------------
{
    return g_xray_renderer.getFirstAngleCT();
}


//-------------------
float getLastAngleCT()
//-------------------
{
    return g_xray_renderer.getLastAngleCT();
}


//-------------------------------
bool getIncludeLastAngleFlagCT()
//-------------------------------
{
    return g_xray_renderer.getIncludeLastAngleFlagCT();
}


//----------------------------------------
unsigned int getWhiteImagesInFlatFieldCT()
//----------------------------------------
{
    return g_xray_renderer.getWhiteImagesInFlatFieldCT();
}


//--------------------------------------------------------------------------------
std::vector<float> getCentreOfRotationPositionCT(const std::string& aUnitOfLength)
//--------------------------------------------------------------------------------
{
    float unit_of_length = getUnitOfLength(aUnitOfLength);

    std::vector<float> temp(3);
    
    temp[0] = g_xray_renderer.getCentreOfRotationPositionCT()[0] / unit_of_length;
    temp[1] = g_xray_renderer.getCentreOfRotationPositionCT()[1] / unit_of_length;
    temp[2] = g_xray_renderer.getCentreOfRotationPositionCT()[2] / unit_of_length;

    return temp;
}


//------------------------------------
std::vector<float> getRotationAxisCT()
//------------------------------------
{
    std::vector<float> temp(3);
    
    temp[0] = g_xray_renderer.getRotationAxisCT()[0];
    temp[1] = g_xray_renderer.getRotationAxisCT()[1];
    temp[2] = g_xray_renderer.getRotationAxisCT()[2];

    return temp;
}


//--------------------------------
std::vector<float> getAngleSetCT()
//--------------------------------
{
    std::vector<float> temp;

    for (auto& it : g_xray_renderer.getAngleSetCT())
    {
        temp.push_back(it);
    }

    return temp;
}


std::vector<std::vector<float> > getLastXRayImage() {
    return (getVectorImage2D(g_xray_renderer.getEnergyFluence()));
}

std::vector<std::vector<float> > getLastLBuffer() {
    return (getVectorImage2D(g_xray_renderer.getLBuffer()));
}

std::vector<std::vector<std::vector<float> > >  getLastSinogram() {
    return (getVectorImage3D(g_xray_renderer.getSinogram()));
}


std::vector<std::vector<std::vector<float> > > getLastProjectionSet() {
    return (getVectorImage3D(g_xray_renderer.getProjectionSet()));
}


std::vector<std::vector<std::vector<unsigned short> > > getLastProjectionSetWithGain() {
    throw gVirtualXRay::NotImplementedException(__FILE__, __FUNCTION__, __LINE__);
    // return (getVectorImage3D(g_xray_renderer.getProjectionSet()));
}


//--------------------------------------------------
void saveLastXRayImage(const std::string& aFileName,
                       bool useCompression)
//--------------------------------------------------
{
    static int file_counter = 0;
    std::string file_name;

    if (!aFileName.empty())
        file_name = aFileName;
    else
        file_name = "xray_image-" + std::to_string(file_counter++) + ".mha";

    g_xray_renderer.getEnergyFluence().save(
        file_name, false, "X-ray projection", useCompression);
}


//----------------------------------------------------------
void saveLastXRayImageWithGain(const std::string& aFileName,
                               bool useCompression)
//----------------------------------------------------------
{
    static int file_counter = 0;
    std::string file_name;

    if (!aFileName.empty())
        file_name = aFileName;
    else
        file_name = "xray_image-with_gain-" + std::to_string(file_counter++) + ".mha";

    g_xray_renderer.getEnergyFluenceWithGain().save(
        file_name, false, "X-ray projection", useCompression);
}


//------------------------------------------------
void saveLastLBuffer(const std::string& aFileName,
                     bool useCompression)
//------------------------------------------------
{
    static int file_counter = 0;
    std::string file_name;

    if (!aFileName.empty())
        file_name = aFileName;
    else
        file_name = "lbuffer-" + std::to_string(file_counter++) + ".mha";

    g_xray_renderer.getLBuffer().save(
        file_name, false, "L-buffer", useCompression);
}


//---------------------------------------------------
void saveLastPathLength(const std::string& aFileName,
                        bool useCompression)
//---------------------------------------------------
{
    saveLastLBuffer(aFileName, useCompression);
}


//---------------------------------------------------------
void saveLastCumulatedLBuffer(const std::string& aFileName,
                              bool useCompression)
//---------------------------------------------------------
{
    static int file_counter = 0;
    std::string file_name;

    if (!aFileName.empty())
        file_name = aFileName;
    else
        file_name = "cumulated_lbuffer-" + std::to_string(file_counter++) + ".mha";

    g_xray_renderer.getCumulatedLBuffer().save(
        file_name, false, "Cumulated L-buffer", useCompression);
}


//-------------------------------------------------
void saveLastSinogram(const std::string& aFileName,
                      bool useCompression)
//-------------------------------------------------
{
    static int file_counter = 0;
    std::string file_name;

    if (!aFileName.empty())
        file_name = aFileName;
    else
        file_name = "sinogram-" + std::to_string(file_counter++) + ".mha";

    g_xray_renderer.getSinogram().save(
        file_name, false, "Sinogram", useCompression);
}


//-----------------------------------------------------
void saveLastProjectionSet(const std::string& aFileName,
                           bool useCompression)
//-----------------------------------------------------
{
    static int file_counter = 0;
    std::string file_name;

    if (!aFileName.empty())
        file_name = aFileName;
    else
        file_name = "projection_set-" + std::to_string(file_counter++) + ".mha";

    g_xray_renderer.getProjectionSet().save(
        file_name, false, "ProjectionSet", useCompression);
}


//--------------------------------------------------------------------------
std::vector<double> getProjectionPixelSize(const std::string& aUnitOfLength)
//--------------------------------------------------------------------------
{
    std::vector<double> projection_pixel_size(2);

    gVirtualXRay::VEC2 detector_pixel_pitch = g_xray_detector.getPixelSpacingInUnitOfLength();
    double unit_of_length = getUnitOfLength(aUnitOfLength);

    if (isUsingParallelBeam())
    {
        projection_pixel_size[0] = detector_pixel_pitch[0] / unit_of_length;
        projection_pixel_size[1] = detector_pixel_pitch[1] / unit_of_length;
    }
    else
    {
        gVirtualXRay::VEC3 sample_centre = convertVec3D(getNodeAndChildrenBoundingBoxCentre("root", "mm"));
        sample_centre *= getUnitOfLength("mm");
        
        gVirtualXRay::VEC3 source_position = g_xray_detector.getXraySourceCentre();
        gVirtualXRay::VEC3 detector_position = g_xray_detector.getDetectorPosition();

        double sdd = (source_position - detector_position).length();
        double sod = (source_position - sample_centre).length();

        projection_pixel_size[0] = detector_pixel_pitch[0] * sod / sdd / unit_of_length;
        projection_pixel_size[1] = detector_pixel_pitch[1] * sod / sdd / unit_of_length;
    }

    return projection_pixel_size;
}


//--------------------------------------------------------------------
std::string getVisibilityCheckReport(const std::string& aUnitOfLength,
                                     const std::string& aNode)
//--------------------------------------------------------------------
{
    std::stringstream report;
    std::vector<double> projection_pixel_size = getProjectionPixelSize("mm");
    std::vector<double> detector_pixel_spacing = getDetectorPixelSpacing("mm");

    report << "Detector pixel spacing [" << aUnitOfLength << "]: " << detector_pixel_spacing[0] << ", " << detector_pixel_spacing[1] << std::endl;
    report << "Radiograph pixel size [" << aUnitOfLength << "]: " << projection_pixel_size[0] << ", " << projection_pixel_size[1] << " (taken in the middle of the whole sample; we also assume the centre is on the line from the source to the middle of the detector)" << std::endl;

    // The scenegraph exists
    if (g_p_scene_graph)
    {
        std::vector<std::string> p_waiting_list;
        p_waiting_list.push_back(aNode);

        while (!p_waiting_list.empty())
        {
            std::string node = p_waiting_list.back();
            p_waiting_list.pop_back();

            // Find the mesh
            // It has been found
            if (gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(node))
            {
                // Add children
                unsigned int number_of_children = getNumberOfChildren(node);
                for (unsigned int i = 0; i < number_of_children; ++i)
                {
                    p_waiting_list.push_back(getChildLabel(node, i));
                }

                // Ignore the root
                if (node != "root")
                {
                    // Compute the L-buffer
                    g_xray_renderer.computeLBuffer(*p_node, gVirtualXRay::VEC3(), g_sample_rotation_matrix, true);

                    // Get its L-buffer
                    const gVirtualXRay::Image<gVirtualXRay::XRayRenderer::PixelType>& l_buffer = g_xray_renderer.getLBuffer();

                    // Get the object position onto the detector
                    unsigned int min_x = l_buffer.getWidth();
                    unsigned int max_x = 0;

                    unsigned int min_y = l_buffer.getHeight();
                    unsigned int max_y = 0;

                    for (unsigned int j = 0; j < l_buffer.getHeight(); ++j)
                        for (unsigned int i = 0; i < l_buffer.getWidth(); ++i)
                        {
                            gVirtualXRay::XRayRenderer::PixelType pixel_value = l_buffer.getPixel(i, j);

                            if (pixel_value > EPSILON)
                            {
                                if (min_x > i) min_x = i;
                                if (max_x < i) max_x = i;
                                if (min_y > j) min_y = j;
                                if (max_y < j) max_y = j;
                            }
                        }

                    // Get the size of the object in pixels
                    unsigned int object_width_in_pixels = max_x - min_x;
                    unsigned int object_height_in_pixels = max_y - min_y;

                    // Get the size of the object in aUnitOfLength
                    auto object_width_in_unit_of_length = static_cast<float>(projection_pixel_size[0] * object_width_in_pixels);
                    auto object_height_in_unit_of_length = static_cast<float>(projection_pixel_size[1] * object_height_in_pixels);

                    report << std::endl;
                    report << "Size of " << node << " [pixel]: " << object_width_in_pixels << "x" << object_height_in_pixels << std::endl;
                    report << "Size of " << node << " [" << aUnitOfLength << "]: " << object_width_in_unit_of_length << "x" << object_height_in_unit_of_length << std::endl;
                }
            }
            // It has not been found
            else
            {
                gVirtualXRay::LOGGER.logWarning("PolygonMesh ") << node << " not found in g_p_polygon_mesh_set." <<  std::endl;
            }
        }
    }
    else
    {
        gVirtualXRay::LOGGER.logWarning("The scenegraph is empty.") <<  std::endl;
    }

    return report.str();
}


//--------------------------------------------------------------------
float getBHPI(unsigned int aNumberOfRotation, float anAngularStepSize)
//--------------------------------------------------------------------
{
    // Make sure there is at least one radiograph
    unsigned int number_of_rotation = std::max(static_cast<unsigned int>(1), aNumberOfRotation);

    // Store the noise status
    bool poisson_noise_enable = g_xray_beam.usePoissonNoise();

    // Store the spectrum
    gVirtualXRay::XRayBeam xray_beam_bak = g_xray_beam;

    // Disable the Poisson noise
    disablePoissonNoise();

    // Fix the number of photons
    setNumberOfPhotonsPerPixelAtSDD(50000);

    // Retrieve the material of all the samples
    // Retrieve the highest effective Z number
    // Look for all the meshes in the scenegraph
    std::vector<gVirtualXRay::SceneGraphNode*> p_temp_pull;
    p_temp_pull.push_back(g_p_scene_graph->getNode("root"));
    std::vector<std::pair<gVirtualXRay::SceneGraphNode*, gVirtualXRay::PhotonCrossSection> > p_material_backup;

    double highest_effective_z_number = 0.0;
    while (p_temp_pull.size())
    {
        // Get the last node
        gVirtualXRay::SceneGraphNode* p_node = p_temp_pull.back();

        // Remove the node
        p_temp_pull.pop_back();

        // Add its children
        for (unsigned int i = 0; i < p_node->getNumberOfChildren(); ++i)
        {
            p_temp_pull.push_back(&p_node->getChild(i));
        }

        // The node is either an inner or outer surface
        if (g_xray_renderer.isInnerSurface(p_node) || g_xray_renderer.isOuterSurface(p_node)) {
            highest_effective_z_number = std::max(highest_effective_z_number,
                p_node->getPolygonMesh().getPhotonCrossSection().getEffectiveAtomicNumber());

            p_material_backup.push_back(std::pair<gVirtualXRay::SceneGraphNode*, gVirtualXRay::PhotonCrossSection>(
                p_node, p_node->getPolygonMesh().getPhotonCrossSection()));
        }
    }

    // Change the Z number of all the meshes. They will all have the highest effective Z number, but will retain
    // their densities
    for (const auto& material_backup : p_material_backup) {
        material_backup.first->getPolygonMesh().setElement(std::round(highest_effective_z_number));
        material_backup.first->getPolygonMesh().setDensity(material_backup.second.getDensity());
    }

    // Update material
    g_xray_renderer.needUpdateSurfacePerMaterial();


    // Create the image buffers
    size_t image_width = g_xray_detector.getNumberOfPixels()[0];
    size_t image_height = g_xray_detector.getNumberOfPixels()[1];
    gVirtualXRay::Image<float> polychromatic_xray_image(image_width,
        image_height,
        number_of_rotation,
        0.0);
    gVirtualXRay::Image<float> monochromatic_xray_image(image_width,
        image_height,
        number_of_rotation,
        0.0);
    float* p_polychromatic_xray_image = polychromatic_xray_image.getRawData();
    float* p_monochromatic_xray_image = monochromatic_xray_image.getRawData();

    // Get the rotation axis
    gVirtualXRay::VEC3 up_vector = g_xray_detector.getUpVector();

    // Save the current transformation matrices
    std::vector<std::vector<double> > transform_set;

    for (size_t i = 0; i < getNumberOfChildren("root"); ++i) {
        transform_set.push_back(getLocalTransformationMatrix(getChildLabel("root", i)));
    }

    // Get the position of the sample
    std::vector<double> sample_location = getNodeAndChildrenBoundingBoxCenter("root", "mm");

    // Compute the images with polychromatism
    for (unsigned int i = 0; i < number_of_rotation; ++i)
    {
        // Rotate the sample on itself
        for (size_t j = 0; j < getNumberOfChildren("root"); ++j) {
            std::string child_label = getChildLabel("root", j);

            translateNode(child_label, sample_location[0], sample_location[1], sample_location[2], "mm");
            rotateNode(child_label, anAngularStepSize * i, up_vector[0], up_vector[1], up_vector[2]);
            translateNode(child_label, -sample_location[0], -sample_location[1], -sample_location[2], "mm");
        }

        // Compute the X-ray image
        computeXRayImage(p_polychromatic_xray_image + (i * image_width * image_height),
            image_height,
            image_width,
            true);

        // Restore the transformation
        for (size_t j = 0; j < getNumberOfChildren("root"); ++j) {
            setLocalTransformationMatrix(getChildLabel("root", j), transform_set[j]);
        }
    }

    // Normalise the image
    polychromatic_xray_image /= getTotalEnergyWithDetectorResponse(true);

    // Compute the average energy of a photon
    std::vector<double> energy_bins = getEnergyBins("keV");
    std::vector<double> photon_counts = getPhotonCountsPerCm2At1m(true);

    double total_energy_polychromatic_beam = 0.0;
    double number_of_photons_polychromatic_beam = 0;

    // C++ 23
    // for (auto [energy, count]: std::views::zip(energy_bins, photon_counts)) {
    //     total_energy_polychromatic_beam += energy * count;
    //     number_of_photons_polychromatic_beam += count;
    // }
    for (size_t i = 0; i < photon_counts.size(); ++i) {
        auto count = photon_counts[i];
        total_energy_polychromatic_beam += energy_bins[i] * count;
        number_of_photons_polychromatic_beam += count;
    }

    double photon_energy_in_keV = total_energy_polychromatic_beam / number_of_photons_polychromatic_beam;

    // Make sure the spectrum is monochromatic
    setMonoChromaticPerCm2At1m(photon_energy_in_keV,
        "keV",
        number_of_photons_polychromatic_beam);

    // Compute the images with monochromatism
    for (unsigned int i = 0; i < number_of_rotation; ++i)
    {
        // Rotate the sample on itself
        for (size_t j = 0; j < getNumberOfChildren("root"); ++j) {
            std::string child_label = getChildLabel("root", j);

            translateNode(child_label, sample_location[0], sample_location[1], sample_location[2], "mm");
            rotateNode(child_label, anAngularStepSize * i, up_vector[0], up_vector[1], up_vector[2]);
            translateNode(child_label, -sample_location[0], -sample_location[1], -sample_location[2], "mm");
        }

        // Compute the X-ray image
        computeXRayImage(p_monochromatic_xray_image + (i * image_width * image_height),
            image_height,
            image_width,
            true);

        // Restore the transformation
        for (size_t j = 0; j < getNumberOfChildren("root"); ++j) {
            setLocalTransformationMatrix(getChildLabel("root", j), transform_set[j]);
        }
    }

    // Normalise the image
    monochromatic_xray_image /= getTotalEnergyWithDetectorResponse(true);

    // Restore the spectrum
    g_xray_beam = xray_beam_bak;

    // Restore the materials
    for (const auto& material_backup : p_material_backup) {
        material_backup.first->getPolygonMesh().setPhotonCrossSection(material_backup.second);
    }
    g_xray_renderer.needUpdateSurfacePerMaterial();

    // Compute the beam hardening indicator
    gVirtualXRay::Image<float> beam_hardening_image_descriptor(monochromatic_xray_image.getWidth(),
        monochromatic_xray_image.getHeight(),
        monochromatic_xray_image.getDepth(),
        0.0);

    // The background pixels should not be taken into account to make sure we
    // have a BMPI that is independent of the sample coverage on the detector
    double sum = 0.0;
    unsigned int counter = 0;
    size_t number_of_pixels = beam_hardening_image_descriptor.getWidth() *
        beam_hardening_image_descriptor.getHeight() *
        beam_hardening_image_descriptor.getDepth();

    float* p_beam_hardening_image_descriptor = beam_hardening_image_descriptor.getRawData();

    // Compute the number of pixels behind the sample and their sum
    double beam_hardening_value = 0.0;
    double epsilon = EPSILON;
#ifdef NDEBUG
    #pragma omp parallel for private(beam_hardening_value) reduction(+ : sum) reduction(+ : counter)
#endif
#ifdef WIN32
    for (int i = 0; i < number_of_pixels; ++i)
#else
    for (size_t i = 0; i < number_of_pixels; ++i)
#endif
    {
        if (p_monochromatic_xray_image[i] < 0.999)
        {
            // Avoid log of zero and division by zero
            beam_hardening_value = log(epsilon + p_polychromatic_xray_image[i]) /
                log(epsilon + p_monochromatic_xray_image[i]);

            beam_hardening_image_descriptor[i] = beam_hardening_value;

            sum += beam_hardening_value;
            counter += 1;
        }
    }

    // Compute the standard deviation of the pixels behind the sample
    // Note that their mean is now 1.0
    double mean = sum / double(counter);
    double variance = 0.0;
    double pixel_value;
#ifdef NDEBUG
    #pragma omp parallel for private(pixel_value) reduction(+ : variance)
#endif
#ifdef WIN32
    for (int i = 0; i < number_of_pixels; ++i)
#else
    for (size_t i = 0; i < number_of_pixels; ++i)
#endif
    {
        if (p_monochromatic_xray_image[i] < 0.999)
        {
            // Normalise the image
            pixel_value = p_beam_hardening_image_descriptor[i];
            variance += std::pow(pixel_value - mean, 2);
        }
    }
    variance /= counter;
    double std_dev = sqrt(variance);

    // Restore the Poisson noise status
    if (poisson_noise_enable)
    {
        enablePoissonNoise();
    }


    // beam_hardening_image_descriptor.save("beam_hardening_image_descriptor.mha");

    if (counter > 0)
        return std_dev / mean;
    else
        return 1.0;
}


//--------------------------------------------------------------------
float getPSNR(unsigned int aNumberOfRotation, float anAngularStepSize)
//--------------------------------------------------------------------
{
    // Make sure there is at least one radiograph
    unsigned int number_of_rotation = std::max(static_cast<unsigned int>(1), aNumberOfRotation);

    // Store the noise status
    bool poisson_noise_enable = g_xray_beam.usePoissonNoise();

    // Create the image buffers
    size_t image_width = g_xray_detector.getNumberOfPixels()[0];
    size_t image_height = g_xray_detector.getNumberOfPixels()[1];
    size_t number_of_pixels = image_width * image_height * number_of_rotation;

    gVirtualXRay::Image<float> noise_free_xray_image(image_width,
        image_height,
        number_of_rotation,
        0.0);
    gVirtualXRay::Image<float> noisy_xray_image(image_width,
        image_height,
        number_of_rotation,
        0.0);

    float* p_noise_free_xray_image = noise_free_xray_image.getRawData();
    float* p_noisy_xray_image = noisy_xray_image.getRawData();

    // Get the rotation axis
    gVirtualXRay::VEC3 up_vector = g_xray_detector.getUpVector();

    // Save the current transformation matrices
    std::vector<std::vector<double> > transform_set;

    for (size_t i = 0; i < getNumberOfChildren("root"); ++i) {
        transform_set.push_back(getLocalTransformationMatrix(getChildLabel("root", i)));
    }

    // Get the position of the sample
    std::vector<double> sample_location = getNodeAndChildrenBoundingBoxCenter("root", "mm");

    // Compute the images with polychromatism
    for (unsigned int i = 0; i < number_of_rotation; ++i)
    {
        // Rotate the sample on itself
        for (size_t j = 0; j < getNumberOfChildren("root"); ++j) {
            std::string child_label = getChildLabel("root", j);

            translateNode(child_label, sample_location[0], sample_location[1], sample_location[2], "mm");
            rotateNode(child_label, anAngularStepSize * i, up_vector[0], up_vector[1], up_vector[2]);
            translateNode(child_label, -sample_location[0], -sample_location[1], -sample_location[2], "mm");
        }

        // Enable the Poisson noise
        enablePoissonNoise();

        // Compute the X-ray image
        computeXRayImage(p_noisy_xray_image + (i * image_width * image_height),
            image_height,
            image_width,
            true);

        // Disable the Poisson noise
        disablePoissonNoise();

        // Compute the X-ray image
        computeXRayImage(p_noise_free_xray_image + (i * image_width * image_height),
            image_height,
            image_width,
            true);

        // Restore the transformation
        for (size_t j = 0; j < getNumberOfChildren("root"); ++j) {
            setLocalTransformationMatrix(getChildLabel("root", j), transform_set[j]);
        }
    }

    // Restore the Poisson noise status
    if (poisson_noise_enable)
    {
        enablePoissonNoise();
    }

    // Normalise the images
    float total_energy = getTotalEnergyWithDetectorResponse(true);
    noisy_xray_image /= total_energy;
    noise_free_xray_image /= total_energy;

    // Compute the number of pixels behind the sample and their sum
    float MSE = 0.0;
#ifdef NDEBUG
    #pragma omp parallel for reduction(+ : MSE)
#endif
#ifdef WIN32
    for (int i = 0; i < number_of_pixels; ++i)
#else
    for (size_t i = 0; i < number_of_pixels; ++i)
#endif
    {
        MSE += std::pow(p_noisy_xray_image[i] - noise_free_xray_image[i], 2);
    }
    MSE /= number_of_pixels;

    return 10.0 * std::log10(1/ MSE);
}


//---------------------------------------------------------------
float getSourceDetectorDistance(const std::string& aUnitOfLength)
//---------------------------------------------------------------
{
    return g_xray_renderer.getSourceDetectorDistance() / getUnitOfLength(aUnitOfLength);
}


//--------------------------------------------
float getSDD(const std::string& aUnitOfLength)
//--------------------------------------------
{
    return getSourceDetectorDistance(aUnitOfLength);
}


//---------------------------------------------------------------
float getObjectDetectorDistance(const std::string& aUnitOfLength)
//---------------------------------------------------------------
{
    return g_xray_renderer.getObjectDetectorDistance() / getUnitOfLength(aUnitOfLength);
}


//--------------------------------------------
float getODD(const std::string& aUnitOfLength)
//--------------------------------------------
{
    return getObjectDetectorDistance(aUnitOfLength);
}


//-------------------------------------------------------------
float getSourceObjectDistance(const std::string& aUnitOfLength)
//-------------------------------------------------------------
{
    return g_xray_renderer.getSourceObjectDistance() / getUnitOfLength(aUnitOfLength);
}


//--------------------------------------------
float getSOD(const std::string& aUnitOfLength)
//--------------------------------------------
{
    return getSourceObjectDistance(aUnitOfLength);
}


//----------------------
float getMagnification()
//----------------------
{
    if (g_xray_detector.useParallelBeam())
        return 1.0;
    else
        return g_xray_renderer.getSDD() / g_xray_renderer.getSOD();
}


//--------------------
float getFieldOfView(const std::string& aUnitOfLength)
//--------------------
{
    return g_xray_renderer.getFOV() / getUnitOfLength(aUnitOfLength);
}


//------------
float getFOV(const std::string& aUnitOfLength)
//------------
{
    return getFieldOfView(aUnitOfLength);
}
