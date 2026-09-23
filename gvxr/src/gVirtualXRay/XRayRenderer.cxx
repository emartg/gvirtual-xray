/*

Copyright (c) 2014, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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


/**
********************************************************************************
*
*    @file        XRayRenderer.h
*
*    @brief        Class to compute and renderer X-ray images on GPU.
*               Now supports GLSL450 and OpenGL 4.5
*
*    @version    1.0
*
*    @date        25/02/2020
*
*    @author        Dr Franck P. Vidal
*
*    @section    License
*                BSD 3-Clause License.
*
*                For details on use and redistribution please refer
*                to http://opensource.org/licenses/BSD-3-Clause
*
*    @section    Copyright
*                (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*                http://www.fpvidal.net/, Feb 2020, 2020, version 1.0,
*                BSD 3-Clause License
*
*   @todo       - Fix the OpenMP backend
*
********************************************************************************
*/


//******************************************************************************
//    Include
//******************************************************************************
#include <exception>
#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <cfloat>
#include <cmath>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <chrono>

#include "../../../SimpleGVXR/include/SimpleGVXR.h"
#include "gVirtualXRay/filesystem.h"

#ifndef GVXR_XRAY_RENDERER_H
#include "gVirtualXRay/XRayRenderer.h"
#endif

#ifndef GVXR_EXCEPTION_H
#include "gVirtualXRay/Exception.h"
#endif

#ifndef GVXR_OUT_OF_MEMORY_EXCEPTION_H
#include "gVirtualXRay/OutOfMemoryException.h"
#endif

#ifndef GVXR_OUT_OF_BOUNDS_EXCEPTION_H
#include "gVirtualXRay/OutOfBoundsException.h"
#endif

#ifndef GVXR_INVALID_INTERNAL_FORMAT_EXCEPTION_H
#include "gVirtualXRay/InvalidInternalFormalException.h"
#endif

#ifndef GVXR_NO_DETECTOR_EXCEPTION_H
#include "gVirtualXRay/NoDetectorException.h"
#endif

#ifndef GVXR_NO_BEAM_EXCEPTION_H
#include "gVirtualXRay/NoBeamException.h"
#endif

#ifndef GVXR_INVALID_IMAGE_SIZE_EXCEPTION_H
#include "gVirtualXRay/InvalidImageSizeException.h"
#endif

#ifndef GVXR_FILE_DOES_NOT_EXIST_EXCEPTION_H
#include "gVirtualXRay/FileDoesNotExistException.h"
#endif

#ifndef GVXR_OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef GVXR_UTILITIES_H
#include "gVirtualXRay/Utilities.h"
#endif

#ifndef GVXR_ATOMIC_ELEMENT_H
#include "gVirtualXRay/AtomicElement.h"
#endif

#ifndef GVXR_RESOURCE_MANAGER_H
#include "gVirtualXRay/ResourceManager.h"
#endif

#ifndef GVXR_LOGGER_H
#include <gVirtualXRay/Logger.h>
#endif

#include "gVirtualXRay/framebufferObject.h"


#include "MUx_Dx_gl2.frag.h"
#include "MUx_Dx_gl2.vert.h"

#include "MUx_Dx_gl3.frag.h"
#include "MUx_Dx_gl3.vert.h"

#include "MUx_Dx_gl4.frag.h"
#include "MUx_Dx_gl4.vert.h"


#include "cumulated_l_buffer_gl2.frag.h"
#include "cumulated_l_buffer_gl2.vert.h"

#include "cumulated_l_buffer_gl3.frag.h"
#include "cumulated_l_buffer_gl3.vert.h"

#include "cumulated_l_buffer_gl4.frag.h"
#include "cumulated_l_buffer_gl4.vert.h"


#include "l_buffer_gl2.frag.h"
#include "l_buffer_gl2.vert.h"

#include "l_buffer_gl3.frag.h"
#include "l_buffer_gl3.vert.h"

#include "l_buffer_gl4.frag.h"
#include "l_buffer_gl4.vert.h"


#include "clean_l_buffer_gl2.frag.h"
#include "clean_l_buffer_gl2.vert.h"

#include "clean_l_buffer_gl3.frag.h"
#include "clean_l_buffer_gl3.vert.h"

#include "clean_l_buffer_gl4.frag.h"
#include "clean_l_buffer_gl4.vert.h"


#include "outer_surface_l_buffer_gl2.frag.h"
#include "outer_surface_l_buffer_gl2.vert.h"

#include "outer_surface_l_buffer_gl3.frag.h"
#include "outer_surface_l_buffer_gl3.vert.h"

#include "outer_surface_l_buffer_gl4.frag.h"
#include "outer_surface_l_buffer_gl4.vert.h"


#include "xray_attenuation_gl2.frag.h"
#include "xray_attenuation_gl2.vert.h"

#include "xray_attenuation_gl3.frag.h"
#include "xray_attenuation_gl3.vert.h"

#include "xray_attenuation_gl4.frag.h"
#include "xray_attenuation_gl4.vert.h"


#include "xray_display_gl2.frag.h"
#include "xray_display_gl2.vert.h"

#include "xray_display_gl3.frag.h"
#include "xray_display_gl3.vert.h"

#include "xray_display_gl4.frag.h"
#include "xray_display_gl4.vert.h"

#include "mu_water.csv.h"


//******************************************************************************
//    define
//******************************************************************************
// Counters
#define XRAY_DETECTOR_2D_FBO_NUMBER     4
#define XRAY_DETECTOR_3D_FBO_NUMBER     1


#define XRAY_DETECTOR_LBUFFER_PBO_ID                     0
#define XRAY_DETECTOR_ENERGY_FLUENCE_PBO_ID              1

#define XRAY_DETECTOR_LBUFFER_SHADER_ID                  0
#define XRAY_DETECTOR_CLEAN_LBUFFER_SHADER_ID            1
#define XRAY_DETECTOR_CUMULATED_LBUFFER_SHADER_ID        2
#define XRAY_DETECTOR_OUTER_SURFACE_LBUFFER_SHADER_ID    3
#define XRAY_DETECTOR_SUM_MUx_Dx_SHADER_ID               4
#define XRAY_DETECTOR_ENERGY_FLUENCE_SHADER_ID           5
#define XRAY_DETECTOR_DISPLAY_SHADER_ID                  6


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//  Static members
//******************************************************************************
std::vector<AttenuationCoefficient> XRayRenderer::m_mu_water_set;
unsigned int XRayRenderer::m_counter = 0;


//-------------------------------------------------------------------------------------------------
XRayRenderer::XRayRenderer():
//-------------------------------------------------------------------------------------------------
        m_p_detector(0),
        m_p_xray_beam(0),
        m_p_outer_surface(0),
        m_surface_per_material_set_needs_update(false),
        m_computing_mode(NONE),
        m_p_pbo_set(XRAY_DETECTOR_PBO_NUMBER, 0),
        m_p_texture_name_set(XRAY_DETECTOR_2D_TEXTURE_NUMBER + XRAY_DETECTOR_3D_TEXTURE_NUMBER, 0),
        m_shader_set(XRAY_DETECTOR_SHADER_NUMBER),
        m_min_value_in_simulated_xray_image(0),
        m_max_value_in_simulated_xray_image(1),
        m_negative_display_flag(1),
        m_use_l_buffer_artefact_filtering_on_gpu(true),
        m_use_l_buffer_artefact_filtering_on_cpu(false),
        m_max_radius_Lbuffer_artefact_filter(2),
        m_l_buffer_empty(false),
        m_energy_fluence_empty(false),
        m_zbuffer_texture_id(0),
        m_mixed_precision_flag(255), // All bits are equal to 1, i.e. Full-precision will be used for all floating-point textures
        m_p_random_number_generator(nullptr),
        m_p_uniform_distribution(nullptr),
        m_p_normal_distribution(nullptr),
        m_use_photon_count_detector(false),
        m_CT_first_angle(0.0),
        m_CT_include_last_angle_flag(false),
        m_CT_last_angle(0.0),
        m_CT_white_images_in_flat_field(0)
//-------------------------------------------------------------------------------------------------
{
        try
    {
//  #ifdef HAS_BOOST
//          // Create the shared memory name
//         std::stringstream memory_name;
//         memory_name << "progress_" << m_counter;
//         m_shared_memory_name = memory_name.str();

//         // Use default permissions
//         boost::interprocess::permissions  default_permissions;
//         default_permissions.set_default();
    
//         // Create the shared memory object
//         m_shared_memory_obj = boost::interprocess::shared_memory_object(boost::interprocess::create_only,
//             m_shared_memory_name.c_str(),
//             boost::interprocess::read_write,
//             default_permissions);
    
//         // Set the size of the shared memory
//         m_shared_memory_obj.truncate(sizeof(float));
    
//         // Create the mapped region
//         m_mapped_region = boost::interprocess::mapped_region(m_shared_memory_obj, boost::interprocess::read_write);
// #endif
    }
    catch (const std::exception& error)
    {
#ifdef HAS_BOOST
        LOGGER.logWarning("Cannot create the shared memory named \"") << m_shared_memory_name << "\". It will be disabled for this renderer." << std::endl;
        m_shared_memory_name.clear();
#endif
    }

    m_counter++;
}


//------------------------------------------------------------
void XRayRenderer::initialise(ComputingMode aComputingMode,
                              uint8_t aMixedPrecisionFlag,
                              const XRayDetector* apDetector,
                              const XRayBeam* apBeamSpectrum)
//------------------------------------------------------------
{
#ifndef NDEBUG
    size_t old_FBO_stack_size = g_frame_buffer_object_stack.size();
    size_t old_texture_size = g_texture_stack.size();
    size_t old_enable_disable_stack_size = g_enable_disable_state_stack.size();
    size_t old_shader_stack_size = g_shader_program_stack.size();
#endif

    // Save the mixed-precision flag
    m_mixed_precision_flag = aMixedPrecisionFlag;

    // There is no detector
    if (!apDetector)
    {
        throw NoDetectorException(__FILE__, __FUNCTION__, __LINE__);
    }

    // There is no beam
    if (!apBeamSpectrum)
    {
        throw NoBeamException(__FILE__, __FUNCTION__, __LINE__);
    }

    // Check the detector size
    if (!apDetector->getNumberOfPixels().getX() || !apDetector->getNumberOfPixels().getY())
    {
        throw InvalidImageSizeException(apDetector->getNumberOfPixels().getX(), apDetector->getNumberOfPixels().getY(), __FILE__, __FUNCTION__, __LINE__);
    }

    // Reset the X-ray renderer
    release();

    // Set the detector
    m_p_detector = const_cast<XRayDetector*>(apDetector);

    // Load mu water table
    loadMuWater();

    // Set the beam spectrum
    m_p_xray_beam = const_cast<XRayBeam*>(apBeamSpectrum);

    // Set the computing mode
    m_computing_mode = aComputingMode;

#ifdef HAS_CUDA
    // Initialise CUDA if needed
    if (m_computing_mode == CUDA)
    {
        if (InitialiseCUDA())
        {
            LOGGER.logNow("CUDA: initialised.") << std::endl;
        }
        else
        {
            LOGGER.logNow("CUDA ERROR: cannot initialise CUDA, set GPU computing using OpenGL.") << std::endl;
            m_computing_mode = OPENGL;
        }
    }
#endif // HAS_CUDA

    // Initialise OpenGL if needed
    if (m_computing_mode == OPENGL)
    {
#ifndef HAS_OPENGL
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
            "OpenGL backend required but not enabled at compile time.");
#endif

        // Check that the GPU is suitable
        checkGPUCapabilities();

        // Initialise shaders
        initShaders();

        // Create FBOs
        createFBOs();

        // Initialise textures
        init2DTextures();
        init3DData();

        // Initialise FBOs
        init2DFBO(XRAY_DETECTOR_UNCLEANED_LBUFFER_FBO_ID,  XRAY_DETECTOR_UNCLEANED_LBUFFER_TEXTURE_NAME_ID, m_zbuffer_texture_id);
        init2DFBO(XRAY_DETECTOR_CLEANED_LBUFFER_FBO_ID,    XRAY_DETECTOR_CLEANED_LBUFFER_TEXTURE_NAME_ID);
        init2DFBO(XRAY_DETECTOR_CUMULATED_LBUFFERS_FBO_ID, XRAY_DETECTOR_CUMULATED_LBUFFERS_TEXTURE_NAME_ID);

        init2DFBO(XRAY_DETECTOR_ENERGY_FLUENCE_FBO_ID, XRAY_DETECTOR_ENERGY_FLUENCE_TEXTURE_NAME_ID);
        init3DFBOs();
    }

    VEC3 pixel_size(m_p_detector->getSizeInUnitOfLength().getX() / m_p_detector->getNumberOfPixels().getX(),
            m_p_detector->getSizeInUnitOfLength().getY() / m_p_detector->getNumberOfPixels().getY(),
            1);

    // Convert the size in mm
    pixel_size[0] /= mm;
    pixel_size[1] /= mm;

    m_l_buffer_image = Image<PixelType>(
            m_p_detector->getNumberOfPixels().getX(),
            m_p_detector->getNumberOfPixels().getY(),
            3);

    m_cumulated_lbuffer_image = Image<PixelType>(
            m_p_detector->getNumberOfPixels().getX(),
            m_p_detector->getNumberOfPixels().getY(),
            3);

    m_sum_mux_dx_image = Image<PixelType>(
            m_p_detector->getNumberOfPixels().getX(),
            m_p_detector->getNumberOfPixels().getY(),
            m_p_xray_beam->getEnergyChannelNumber());

    m_energy_fluence_image = Image<PixelType>(
            m_p_detector->getNumberOfPixels().getX(),
            m_p_detector->getNumberOfPixels().getY(),
            1);

    m_energy_fluence_with_gain_image = Image<unsigned short>(
            m_p_detector->getNumberOfPixels().getX(),
            m_p_detector->getNumberOfPixels().getY(),
            1);

    // Set the pixel size
    m_l_buffer_image.setSpacing(pixel_size);
    m_cumulated_lbuffer_image.setSpacing(pixel_size);
    m_sum_mux_dx_image.setSpacing(pixel_size);
    m_energy_fluence_image.setSpacing(pixel_size);
    m_energy_fluence_with_gain_image.setSpacing(pixel_size);

    // Create the VBO
    updateVBO();

    // Allocate memory
    unsigned int depth(3);
    if (depth < m_p_xray_beam->getEnergyChannelNumber())
    {
        depth = m_p_xray_beam->getEnergyChannelNumber();
    }
    m_temp_raw_data = Image<PixelType>(
            m_p_detector->getNumberOfPixels().getX(),
            m_p_detector->getNumberOfPixels().getY(),
            depth);


    // Initialise the random number generators
    // Does not have OpenMP
#ifndef HAS_OPENMP
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    if (!m_p_random_number_generator)
        m_p_random_number_generator = new std::default_random_engine;
    
    if (!m_p_normal_distribution)
        m_p_normal_distribution = new std::normal_distribution<>(0.0, 1.0);

    *m_p_random_number_generator = std::default_random_engine(seed);
#else
    // Has OpenMP
    // See Section 7.1 of Random Numbers in Scientific Computing: An Introduction by Katzgraber (http://arxiv.org/pdf/1005.4117v1.pdf)
    // for seeding in parallel applications

    // Create the parallel region
#pragma omp parallel
    {
        // Get the thread ID
        int thread_id = omp_get_thread_num();

        // Get the current time
        int s = std::chrono::system_clock::now().time_since_epoch().count();

        int seed = std::abs(((s * 181) * ((thread_id - 83) * 359)) % 104729);

        // thread 0 must initialse pp_r
        if (thread_id == 0)
        {
            if (!m_p_random_number_generator)
                m_p_random_number_generator = new std::default_random_engine[omp_get_num_threads()];
            
            if (!m_p_uniform_distribution)
                m_p_uniform_distribution = new std::uniform_real_distribution<>[omp_get_num_threads()];
            
            if (!m_p_normal_distribution)
                m_p_normal_distribution = new std::normal_distribution<>[omp_get_num_threads()];
        }

        // Make sure all threads have reached this point. It means pp_r has been initialsed.
#pragma omp barrier

        m_p_random_number_generator[thread_id] = std::default_random_engine(seed);
        m_p_uniform_distribution[thread_id] = std::uniform_real_distribution<>(0.0, 1.0);
        m_p_normal_distribution[thread_id] = std::normal_distribution<>(0.0, 1.0);
    }
#endif

#ifndef NDEBUG
    assert(old_FBO_stack_size == g_frame_buffer_object_stack.size());
    assert(old_texture_size == g_texture_stack.size());
    assert(old_enable_disable_stack_size == g_enable_disable_state_stack.size());
    assert(old_shader_stack_size == g_shader_program_stack.size());
#endif
}


//-------------------------------------------------------------------------
void XRayRenderer::computeSinogram(MATRIX4& aModellingTransformationMatrix,
                                   VEC3 aRotationCenter,
                                   unsigned int aNumberOfAngle,
                                   RATIONAL_NUMBER anAngleOffset)
//-------------------------------------------------------------------------
{
    throw Exception(__FILE__, __FUNCTION__, __LINE__, "Rewrite if needed using computeCTAcquisition");

    // if (anAngleOffset == 0)
    // {
    //     throw Exception(__FILE__, __FUNCTION__, __LINE__, "The number of projections is zero.");
    // }

    // VEC3 pixel_size(m_p_detector->getSizeInUnitOfLength().getX() / m_p_detector->getNumberOfPixels().getX(),
    //         anAngleOffset,
    //         m_p_detector->getSizeInUnitOfLength().getY() / m_p_detector->getNumberOfPixels().getY());

    // if (std::abs(pixel_size.getZ()) < EPSILON)
    // {
    //     pixel_size[2] = 1.0;
    // }

    // // Convert the size in mm * degree * mm
    // pixel_size[0] /= mm;
    // pixel_size[1] /= 1.0;
    // pixel_size[2] /= mm;

    // // Allocate the memory
    // m_sinogram_image = Image<PixelType>(
    //         m_p_detector->getNumberOfPixels().getX(),
    //         aNumberOfAngle,
    //         m_p_detector->getNumberOfPixels().getY());

    // // Set the pixel size
    // m_sinogram_image.setSpacing(pixel_size);

    // // Compute the translation vector
    // VEC3 translation_vector(-aRotationCenter);

    // // Compute the sinogram
    // for (unsigned int angle(0); angle < aNumberOfAngle; ++angle)
    // {
    //     MATRIX4 transformation_matrix;

    //     // Translate the sample
    //     transformation_matrix.translate(-translation_vector);

    //     // Rotate the sample
    //     transformation_matrix.rotate(anAngleOffset * angle, m_p_detector->getUpVector());

    //     // Translate the sample
    //     transformation_matrix.translate(translation_vector);

    //     // Apply the modelling transformation
    //     transformation_matrix = transformation_matrix * aModellingTransformationMatrix;

    //     // Compute the X-ray image
    //     computeImage(transformation_matrix, true);

    //     // Load the FBO in the main memory
    //     getEnergyFluence();

    //     // Get the destination pointer
    //     PixelType* p_pixel_set(m_sinogram_image.getRawData());

    //     // Convert the data into a sinogram
    //     PixelType* p_input_data(m_temp_raw_data.getRawData());
    //     RATIONAL_NUMBER total_input_energy(m_p_xray_beam->getTotalEnergy());
    //     RATIONAL_NUMBER scaling_factor(
    //             RATIONAL_NUMBER(m_p_detector->getNumberOfPixels().getX()) / (m_p_detector->getSizeInUnitOfLength().getX() / cm)
    //     );

    //     for (unsigned j(0); j < m_p_detector->getNumberOfPixels().getY(); ++j)
    //     {
    //         for (unsigned i(0); i < m_p_detector->getNumberOfPixels().getX(); ++i, ++p_input_data)
    //         {
    //             RATIONAL_NUMBER voxel_value(*p_input_data);
    //             /*if (fabs(*p_input_data) <= 0.00001)
    //             {
    //                 voxel_value = 0;
    //                 unsigned int count(0);
    //                 for (unsigned temp_j(j - 1); temp_j <= j + 1; ++temp_j)
    //                 {
    //                     for (unsigned temp_i(i - 1); temp_i <= i + 1; ++temp_i)
    //                     {
    //                         if (temp_i >= 0 && temp_j >= 0 && temp_i < m_p_detector->getNumberOfPixels().getX() && temp_j < m_p_detector->getNumberOfPixels().getY())
    //                         {
    //                             if (m_p_temp_raw_data[temp_j * m_p_detector->getNumberOfPixels().getX() + temp_i] >= 0.00001)
    //                             {
    //                                 voxel_value += m_p_temp_raw_data[temp_j * m_p_detector->getNumberOfPixels().getX() + temp_i];
    //                                 ++count;
    //                             }
    //                         }
    //                     }
    //                 }
    //                 if (count)
    //                 {
    //                     voxel_value /= count;
    //                 }
    //             }*/
    //             *p_input_data = scaling_factor * std::log(total_input_energy / voxel_value);

    //         }
    //     }

    //     // Save the data in the sinogram
    //     //Process every line of the X-ray image
    //     for (unsigned j(0); j < m_p_detector->getNumberOfPixels().getY(); ++j)
    //     {
    //         // Compute the line position in the sinogram
    //         unsigned int offset(j * aNumberOfAngle * m_p_detector->getNumberOfPixels().getX() +
    //                 angle * m_p_detector->getNumberOfPixels().getX());

    //         // Copy the data
    //         memcpy(p_pixel_set + offset, m_temp_raw_data.getRawData() + j * m_p_detector->getNumberOfPixels().getX(), sizeof(PixelType) * m_p_detector->getNumberOfPixels().getX());
    //     }
    // }
}


//------------------------------------------------------------------------------
void XRayRenderer::computeProjectionSet(MATRIX4& aModellingTransformationMatrix,
                                        VEC3 aRotationCenter,
                                        unsigned int aNumberOfAngle,
                                        RATIONAL_NUMBER anAngleOffset)
//------------------------------------------------------------------------------
{
    throw Exception(__FILE__, __FUNCTION__, __LINE__, "Rewrite if needed using computeCTAcquisition");

    // if (aNumberOfAngle == 0)
    // {
    //     throw Exception(__FILE__, __FUNCTION__, __LINE__, "The number of projections is zero.");
    // }

    // VEC3 pixel_size(m_p_detector->getSizeInUnitOfLength().getX() / m_p_detector->getNumberOfPixels().getX(),
    //         m_p_detector->getSizeInUnitOfLength().getY() / m_p_detector->getNumberOfPixels().getY(),
    //         anAngleOffset);

    // if (std::abs(pixel_size[2]) < EPSILON)
    // {
    //     pixel_size[2] = 1.0;
    // }

    // // Convert the size in mm * degree * mm
    // pixel_size[0] /= mm;
    // pixel_size[1] /= mm;
    // pixel_size[2] /= 1.0;

    // // Allocate the memory
    // m_projection_set = Image<PixelType>(
    //         m_p_detector->getNumberOfPixels().getX(),
    //         m_p_detector->getNumberOfPixels().getY(),
    //         aNumberOfAngle);


    // // Set the pixel size
    // m_projection_set.setSpacing(pixel_size);

    // // Compute the translation vector
    // VEC3 translation_vector(-aRotationCenter);

    // // Compute the sinogram
    // for (unsigned int angle(0); angle < aNumberOfAngle; ++angle)
    // {
    //     MATRIX4 transformation_matrix;

    //     // Translate the sample
    //     transformation_matrix.translate(-translation_vector);

    //     // Rotate the sample
    //     transformation_matrix.rotate(anAngleOffset * angle, m_p_detector->getUpVector());

    //     // Translate the sample
    //     transformation_matrix.translate(translation_vector);

    //     // Apply the modelling transformation
    //     transformation_matrix = transformation_matrix * aModellingTransformationMatrix;

    //     // Compute the X-ray image
    //     computeImage(transformation_matrix, true);

    //     // Load the FBO in the main memory
    //     getFBO(XRAY_DETECTOR_ENERGY_FLUENCE_FBO_ID);

    //     // Get the destination pointer
    //     PixelType* p_pixel_set(m_projection_set.getRawData());

    //     // Copy the data
    //     memcpy(p_pixel_set + angle * m_p_detector->getNumberOfPixels().getX() * m_p_detector->getNumberOfPixels().getY(), m_temp_raw_data.getRawData(), sizeof(PixelType) * m_p_detector->getNumberOfPixels().getX() * m_p_detector->getNumberOfPixels().getY());
    // }

    // // Apply the LSF if needed
    // if (m_p_detector)
    // {
    //     if (m_p_detector->getLSF().size())
    //     {
    //         m_projection_set = m_projection_set.convolution1D(m_p_detector->getLSF());
    //     }
    // }
}


//-----------------------------------------------------------------------------------
void XRayRenderer::computeCTAcquisition(const std::string& aProjectionOutputPath,
                                        const std::string& aScreenshotOutputPath,
                                        unsigned int aNumberOfProjections,
                                        const RATIONAL_NUMBER& aFirstAngle,
                                        bool anIncludeLastAngleFlag,
                                        const RATIONAL_NUMBER& aLastAngle,
                                        unsigned int aNumberOfWhiteImagesInFlatField,
                                        const VEC3& aPositionOfCentreOfRotation,
                                        const VEC3& aAxisOfRotation,
                                        const MATRIX4& aModellingTransformationMatrix,
                                        bool anIntegrateEnergyFlag,
                                        unsigned short aWhiteValue,
                                        unsigned int aVerboseLevel)
//-----------------------------------------------------------------------------------
{
#ifndef NDEBUG
    size_t old_FBO_stack_size = g_frame_buffer_object_stack.size();
    size_t old_texture_size = g_texture_stack.size();
    size_t old_enable_disable_stack_size = g_enable_disable_state_stack.size();
    size_t old_shader_stack_size = g_shader_program_stack.size();
#endif

    namespace fs = std::filesystem;

    if (aNumberOfProjections == 0)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "The number of projections is zero.");
    }

    if (aProjectionOutputPath.empty() && anIntegrateEnergyFlag == false)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "For spectral imaging, due to the high memory consumption, please save the images on the disk instead of the main memory (RAM).");
    }

    m_use_photon_count_detector = !anIntegrateEnergyFlag;

    m_CT_projection_output_path = aProjectionOutputPath;
    m_CT_screenshot_output_path = aScreenshotOutputPath;

    bool use_gain = m_p_detector->getGain() > 1.000001f;
    if (use_gain && aVerboseLevel > 0)
    {
        LOGGER.logNow("Store the data as UINT16 using gain value: ") << m_p_detector->getGain() << "." << std::endl;
    }

    // Make sure the path exists
    fs::path fs_path_projection_output(m_CT_projection_output_path);
    // fs::path fs_path_screenshot_output(m_CT_screenshot_output_path);

    // fs_path_projection_output already exists
    if (!m_CT_projection_output_path.empty() && fs::exists(fs_path_projection_output))
    {
        if (aVerboseLevel)
        {
            LOGGER.logNow("The directory ") << fs_path_projection_output << " already exists." << std::endl;
        }

        // But fs_path_projection_output is not a directory
        if (!fs::is_directory(fs_path_projection_output))
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "The output path already exists but it's not a directory, I don't know what to do. You must fix this issue yourself.");
        }
        // Empty and delete the directory
        else
        {
            if (aVerboseLevel)
            {
                LOGGER.logNow("Empty and delete the directory ") << fs_path_projection_output << "." << std::endl;
            }
            fs::remove_all(fs_path_projection_output);
        }
    }

    // Create fs_path_projection_output
    if (!m_CT_projection_output_path.empty())
    {
        if (aVerboseLevel)
        {
            LOGGER.logNow("Create the directory ") << fs_path_projection_output << "." << std::endl;
        }
        fs::create_directory(fs_path_projection_output);
    }

    m_CT_first_angle = aFirstAngle;
    m_CT_include_last_angle_flag = anIncludeLastAngleFlag;
    m_CT_last_angle = aLastAngle;

    m_CT_white_images_in_flat_field = aNumberOfWhiteImagesInFlatField;

    m_CT_centre_of_rotation_position = aPositionOfCentreOfRotation;
    m_CT_rotation_axis = aAxisOfRotation;

    m_CT_angle_set.resize(aNumberOfProjections);

    double angular_step;

    // Compute the angular step
    if (m_CT_include_last_angle_flag)
    {
        // Example:
        //   m_CT_first_angle = 0
        //   m_CT_last_angle = 180
        //   aNumberOfProjections = 3
        //   angular_step = (180 - 0) / (3 - 1) = 90
        //   m_CT_angle_set = [0, 90, 180]
        angular_step = (m_CT_last_angle - m_CT_first_angle) / (aNumberOfProjections - 1);
    }
    else
    {
        // Example:
        //   m_CT_first_angle = 0
        //   m_CT_last_angle = 180
        //   aNumberOfProjections = 3
        //   angular_step = (180 - 0) / 3 = 60
        //   m_CT_angle_set = [0, 60, 120]
        angular_step = (m_CT_last_angle - m_CT_first_angle) / aNumberOfProjections;
    }

    // Compute the set of rotation angles
    Image<unsigned short> temp_image_ushort(m_energy_fluence_with_gain_image.getWidth(), m_energy_fluence_with_gain_image.getHeight());
    for (unsigned int i = 0; i < aNumberOfProjections; ++i)
    {
        m_CT_angle_set[i] = m_CT_first_angle + angular_step * i;
    }

    // Log the details if needed
    if (aVerboseLevel)
    {
        LOGGER.logNow("Performing CT Scan.") << std::endl;

        if (anIntegrateEnergyFlag)
            LOGGER.logNow("\tEnergy fluence (integrated energy):\tYES") << std::endl;
        else
            LOGGER.logNow("\tSpectral imaging (photon counting):\tYES") << std::endl;

        if (!m_CT_projection_output_path.empty())
        {
            LOGGER.logNow("\tX-ray projections will be stored in the RAM:\tNO") << std::endl;
            LOGGER.logNow("\tX-ray projections will be saved in:\t") << m_CT_projection_output_path << std::endl;
        }
        else
            LOGGER.logNow("\tX-ray projections will be stored in the RAM:\tYES") << std::endl;

        if (!m_CT_screenshot_output_path.empty())
        {
            LOGGER.logNow("\tScreenshots will be saved in:\t") << m_CT_screenshot_output_path << std::endl;
            LOGGER.logWarning("\tSaving screenshots is not currently supported.") << std::endl;
        }

        LOGGER.logNow("\tTotal number of X-ray projections:\t") << aNumberOfProjections << std::endl;
        LOGGER.logNow("\tFirst angle (in degrees):\t") << m_CT_first_angle << std::endl;
        LOGGER.logNow("\tLast angle (in degrees):\t") << m_CT_last_angle << std::endl;
        LOGGER.logNow("\tInclude the last angle:\t") << (m_CT_include_last_angle_flag?"YES":"NO") << std::endl;

        if (m_CT_angle_set.size() == 3)
        {
            LOGGER.logNow("\tRotation angles:\t[") <<
                m_CT_angle_set[0] << ", " <<
                m_CT_angle_set[1] << ", " <<
                m_CT_angle_set[2] << "]" << std::endl;
        }
        else if (m_CT_angle_set.size() == 4)
        {
            LOGGER.logNow("\tRotation angles:\t[") <<
                m_CT_angle_set[0] << ", " <<
                m_CT_angle_set[1] << ", " <<
                m_CT_angle_set[2] << ", " <<
                m_CT_angle_set[3] << "]" << std::endl;
        }
        else if (m_CT_angle_set.size() >= 5)
        {
            LOGGER.logNow("\tRotation angles:\t[") <<
                m_CT_angle_set[0] << ", " <<
                m_CT_angle_set[1] << ", " <<
                "..., " <<
                m_CT_angle_set[m_CT_angle_set.size() - 2] << ", " <<
                m_CT_angle_set[m_CT_angle_set.size() - 1] << "]" << std::endl;
        }

        if (m_CT_white_images_in_flat_field)
        {
            LOGGER.logNow("\tPerform the flat-field correction:\tYES") << std::endl;
            LOGGER.logNow("\tNumber of white images in the flat-field correction:\t") << m_CT_white_images_in_flat_field << std::endl;
        }
        else
            LOGGER.logNow("\tPerform the flat-field correction:\tNO") << std::endl;

        LOGGER.logNow("\tPosition of the centre of rotation (in mm):\t") << m_CT_centre_of_rotation_position / mm << std::endl;
        LOGGER.logNow("\tRotation axis:\t") << m_CT_rotation_axis << std::endl;

        if (!m_CT_projection_output_path.empty())
        {
#ifdef HAS_LIBTIFF
            LOGGER.logNow("\tProjections saved to:\t\"") << m_CT_projection_output_path << "/projection-xxxx.tiff\"" << std::endl;
#else
            LOGGER.logNow("\tProjections saved to:\t\"") << m_CT_projection_output_path << "/projection-xxxx.mha\"" << std::endl;
#endif
        }
    }

    VEC3 pixel_size(m_p_detector->getSizeInUnitOfLength().getX() / m_p_detector->getNumberOfPixels().getX(),
            m_p_detector->getSizeInUnitOfLength().getY() / m_p_detector->getNumberOfPixels().getY(),
            aNumberOfProjections);

    // Allocate the memory
    if (anIntegrateEnergyFlag == true && m_CT_projection_output_path.empty())
    {
        m_projection_set = Image<PixelType>(
            m_p_detector->getNumberOfPixels().getX(),
            m_p_detector->getNumberOfPixels().getY(),
            aNumberOfProjections);

        if (use_gain)
            throw NotImplementedException(__FILE__, __FUNCTION__, __LINE__);

        if (std::abs(pixel_size[2]) < EPSILON)
        {
            pixel_size[2] = 1.0;
        }

        // Set the pixel size
        m_projection_set.setSpacing(pixel_size);
    }
    m_energy_fluence_image.setSpacing(pixel_size);
    m_energy_fluence_with_gain_image.setSpacing(pixel_size);

    // White image
    Image<PixelType> white_image;
    Image<unsigned short> white_image_ushort;

    // If flat-field correction is used, accummulate the right number of white images
    // The code below takes care of the gain if it is applied
    for (unsigned int i = 0; i < m_CT_white_images_in_flat_field; ++i)
    {
        if (i == 0)
            if (use_gain) {
                white_image_ushort = getWhiteImageWithGain(anIntegrateEnergyFlag);
                convert(white_image_ushort, white_image);
            }
            else
                white_image = getWhiteImage(anIntegrateEnergyFlag);
        else
            if (use_gain) {
                white_image_ushort = getWhiteImageWithGain(anIntegrateEnergyFlag);

                // Convert the data type and add the values
                std::transform(white_image.getRawData(),
                    white_image.getRawData() + white_image.getWidth() * white_image.getHeight() * white_image.getDepth(),
                    white_image_ushort.getRawData(),
                    white_image.getRawData(),
                    std::plus<float>());
            }
            else
                white_image += getWhiteImage(anIntegrateEnergyFlag);
    }

    // If flat-field correction is used, compute the average white image
    if (m_CT_white_images_in_flat_field)
        white_image /= m_CT_white_images_in_flat_field;

    // Compute the translation vector
    VEC3 translation_vector(-m_CT_centre_of_rotation_position);

    // Clear the vectors
    m_source_positions_set.clear();
    m_detector_positions_set.clear();
    m_detector_right_vector_set.clear();
    m_detector_up_vector_set.clear();

    // Compute the sinogram
#ifdef HAS_BOOST
    int* p_progress = nullptr;
    if (m_shared_memory_name.size() > 0)
    {
        p_progress = static_cast<int*>(m_mapped_region.get_address());
        *p_progress = 0.0;
    }
#endif

    for (unsigned int i(0); i < aNumberOfProjections; ++i)
    {
        MATRIX4 transformation_matrix;

        // Translate the sample
        transformation_matrix.translate(-translation_vector);

        // Rotate the sample
        transformation_matrix.rotate(m_CT_angle_set[i], m_CT_rotation_axis);

        // Translate the sample
        transformation_matrix.translate(translation_vector);

        // Apply the modelling transformation matrix
        transformation_matrix *= aModellingTransformationMatrix;

        // Compute the X-ray image
        computeImage(transformation_matrix, anIntegrateEnergyFlag);

        // Save the source, and detector position/orientation
        MATRIX4 inverse_rotation_matrix;
        inverse_rotation_matrix.translate(-translation_vector);
        inverse_rotation_matrix.rotate(-m_CT_angle_set[i], m_CT_rotation_axis);
        inverse_rotation_matrix.translate(translation_vector);

        m_source_positions_set.push_back(inverse_rotation_matrix * m_p_detector->getXraySourceCentre());
        m_detector_positions_set.push_back(inverse_rotation_matrix * m_p_detector->getDetectorPosition());
        m_detector_right_vector_set.push_back(inverse_rotation_matrix * m_p_detector->getRightVector());
        m_detector_up_vector_set.push_back(inverse_rotation_matrix * aAxisOfRotation);

        // Load the FBO in the main memory
        if (!use_gain)
            getEnergyFluence(); // This call takes care of the noise and the impulse response of the detector
        else
            getEnergyFluenceWithGain(); // This call takes care of the noise and the impulse response of the detector

        // Apply the flat-field correction if needed
        if (m_CT_white_images_in_flat_field) {
            // Convert from ushort to float
            if (use_gain) {
                convert(m_energy_fluence_with_gain_image, m_energy_fluence_image);
            }

            // Apply the correction
            m_energy_fluence_image /= white_image;
        }

        // Save the image on the disk
        if (m_CT_projection_output_path.size() > 0)
        {
            // Create the filename
            std::stringstream filename;
            filename << m_CT_projection_output_path << "/" << "projection_" << std::setw(4) << std::setfill('0') << i;

#ifdef HAS_LIBTIFF
            filename << ".tiff";
#else
            filename << ".mha";
#endif

            if (m_use_photon_count_detector)
            {
                throw Exception(__FILE__, __FUNCTION__, __LINE__, "Not implemented for spectral imaging.");
            }
            else
            {
                // Flat-field correction was used, the image is in floating-point numbers between 0 and 1 (+/- the noise)
                if (aNumberOfWhiteImagesInFlatField > 0) {
                    // A white value was provided, the image will be saved in ushort
                    if (aWhiteValue > 0) {
                        // Apply the detector white value
                        // Clamp and cast the data
                        std::transform(m_energy_fluence_image.getRawData(),
                            m_energy_fluence_image.getRawData() + m_energy_fluence_image.getWidth() * m_energy_fluence_image.getHeight() * m_energy_fluence_image.getDepth(),
                            temp_image_ushort.getRawData(), [white_value=aWhiteValue](XRayRenderer::PixelType x) {
                                return static_cast<unsigned short>(std::clamp(static_cast<int>(std::round(x * white_value)), 0, USHRT_MAX));
                            });

                        temp_image_ushort.save(filename.str(),
                            false,
                            "CT scan simulated with gVirtualXray.",
                            true);
                    }
                    // A white value was not provided, the image is in floating-point numbers
                    else {
                        m_energy_fluence_image.save(filename.str(),
                            false,
                            "CT scan simulated with gVirtualXray.",
                            true);
                    }
                }
                // Flat-field correction was not used
                else {
                    // Gain was used, the image will be saved in ushort
                    if (use_gain) {
                        m_energy_fluence_with_gain_image.save(filename.str(),
                            false,
                            "CT scan simulated with gVirtualXray.",
                            true);
                    }
                    // Gain was not used, the image is in floating-point numbers
                    else {
                        m_energy_fluence_image.save(filename.str(),
                            false,
                            "CT scan simulated with gVirtualXray.",
                            true);
                    }
                }
            }
        }
        
        // Copy the data        
        if (anIntegrateEnergyFlag == true && m_CT_projection_output_path.empty())
        {
            if (use_gain)
                throw NotImplementedException(__FILE__, __FUNCTION__, __LINE__);

            // Get the destination pointer if needed
            PixelType* p_pixel_set(m_projection_set.getRawData());
            int nunmber_of_pixels = m_p_detector->getNumberOfPixels().getX() * m_p_detector->getNumberOfPixels().getY();
            p_pixel_set += i * nunmber_of_pixels;

            memcpy(p_pixel_set, m_energy_fluence_image.getRawData(), sizeof(PixelType) * nunmber_of_pixels);
        }

#ifdef HAS_BOOST
        if (p_progress != nullptr)
        {
            *p_progress = i + 1;
        }
#endif
    }

#ifndef NDEBUG
    assert(old_FBO_stack_size == g_frame_buffer_object_stack.size());
    assert(old_texture_size == g_texture_stack.size());
    assert(old_enable_disable_stack_size == g_enable_disable_state_stack.size());
    assert(old_shader_stack_size == g_shader_program_stack.size());
#endif
}


//--------------------------------------------------------------------------------------------
void XRayRenderer::computeCTAcquisitionWithGain(const std::string& aProjectionOutputPath,
                                                const std::string& aScreenshotOutputPath,
                                                unsigned int aNumberOfProjections,
                                                const RATIONAL_NUMBER& aFirstAngle,
                                                bool anIncludeLastAngleFlag,
                                                const RATIONAL_NUMBER& aLastAngle,
                                                unsigned int aNumberOfWhiteImagesInFlatField,
                                                const VEC3& aPositionOfCentreOfRotation,
                                                const VEC3& aAxisOfRotation,
                                                const MATRIX4& aModellingTransformationMatrix,
                                                bool anIntegrateEnergyFlag,
                                                unsigned short aWhiteValue,
                                                unsigned int aVerboseLevel)
//--------------------------------------------------------------------------------------------
{
#ifndef NDEBUG
    size_t old_FBO_stack_size = g_frame_buffer_object_stack.size();
    size_t old_texture_size = g_texture_stack.size();
    size_t old_enable_disable_stack_size = g_enable_disable_state_stack.size();
    size_t old_shader_stack_size = g_shader_program_stack.size();
#endif

    computeCTAcquisition(aProjectionOutputPath,
                         aScreenshotOutputPath,
                         aNumberOfProjections,
                         aFirstAngle,
                         anIncludeLastAngleFlag,
                         aLastAngle,
                         aNumberOfWhiteImagesInFlatField,
                         aPositionOfCentreOfRotation,
                         aAxisOfRotation,
                         aModellingTransformationMatrix,
                         anIntegrateEnergyFlag,
                         aWhiteValue,
                         aVerboseLevel);

#ifndef NDEBUG
    assert(old_FBO_stack_size == g_frame_buffer_object_stack.size());
    assert(old_texture_size == g_texture_stack.size());
    assert(old_enable_disable_stack_size == g_enable_disable_state_stack.size());
    assert(old_shader_stack_size == g_shader_program_stack.size());
#endif
}


//------------------------------
void XRayRenderer::loadMuWater()
//------------------------------
{
    // Clear the data
    m_mu_water_set.clear();

    char* p_dest(0);
    int z_lib_return_code = inflate(g_mu_water_csv, sizeof(g_mu_water_csv), &p_dest);

    // It does not exist
    if (!z_lib_return_code)
    {
        delete [] p_dest;
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "No mu water was decoded.");
    }

    // Open the file
    std::stringstream mu_stream;
    mu_stream << (char*)p_dest;
    delete [] p_dest;

    // It does not exist
    if (!mu_stream.str().size())
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "No mu water was provided.");
    }

    // Ignore the first line
    char first_line[1024];
    mu_stream.getline(first_line, 1024);

    // Process every record
    AttenuationCoefficient temp;
    while (!mu_stream.eof())
    {
        // Load the record
        mu_stream >> temp;

        // The record is valid
        if (!mu_stream.eof())
        {
            // Store the record
            m_mu_water_set.push_back(temp);
        }
    }
}


//--------------------------
void XRayRenderer::release()
//--------------------------
{
    m_p_detector = nullptr;
    m_p_xray_beam = nullptr;

    //std::vector<SceneGraphNode*> m_p_inner_surface_set;

    m_p_outer_surface = nullptr;

    //std::vector<SceneGraphNode*> m_p_internal_data_storage;

    m_p_surface_per_material_set.clear();

    m_surface_per_material_set_needs_update = true;
    m_computing_mode = NONE;

    // Delete PBOs
    for (std::vector<GLuint>::iterator ite(m_p_pbo_set.begin());
            ite != m_p_pbo_set.end();
            ++ite)
    {
        // Delete the PBO
        try
        {
            if (*ite != 0)
            {
                deletePBO(*ite);
            }
        }
        catch (std::exception& e)
        {
            LOGGER.logError(e.what()) << std::endl;
        }

        // Make sure the value is reset
        *ite = 0;
    }

    // Delete FBOs
    try
    {
        deleteFBOs();
    }
    catch (std::exception& e)
    {
        LOGGER.logError(e.what()) << std::endl;
    }
    m_p_fbo_set.clear();

#ifdef HAS_OPENGL
    Context* p_context = ResourceManager::getInstance().getGraphicsContext();

    // Delete the textures
    for (std::vector<GLuint>::iterator ite(m_p_texture_name_set.begin());
            ite != m_p_texture_name_set.end();
            ++ite)
    {
        try
        {
            if (*ite != 0)
            {
                if (p_context) glDeleteTextures(1, &(*ite));

                *ite = 0;
            }
        }
        catch (std::exception& e)
        {
            LOGGER.logError(e.what()) << std::endl;
        }
    }
#endif

#ifdef HAS_OPENGL
    if (m_zbuffer_texture_id && p_context) glDeleteTextures(1, &m_zbuffer_texture_id);
#endif

    m_zbuffer_texture_id = 0;

    // Delete the shaders
    for (std::vector<Shader>::iterator ite(m_shader_set.begin());
            ite != m_shader_set.end();
            ++ite)
    {
        try
        {
            ite->release();
        }
        catch (std::exception& e)
        {
            LOGGER.logError(e.what()) << std::endl;
        }
    }

    m_detector_temporary_geometry.reset(nullptr);
    m_source_positions_set.clear();
    m_detector_positions_set.clear();
    m_detector_right_vector_set.clear();
    m_detector_up_vector_set.clear();

    m_CT_angle_set.clear();
}


//--------------------------------
bool XRayRenderer::isReady() const
//--------------------------------
{
    // There is no error so far
    bool error(false);

    if (m_p_detector)
    {
        // Check the distance source/detector
        if ((m_p_detector->getXraySourceCentre() - m_p_detector->getDetectorPosition()).length() < std::numeric_limits<RATIONAL_NUMBER>::epsilon())
        {
            LOGGER.logWarning("the distance from the source to the far clipping plane is null.") << std::endl;
            error = true;
        }

        // Check the detector size
        if (m_p_detector->getNumberOfPixels().getX() * m_p_detector->getNumberOfPixels().getY() == 0)
        {
            LOGGER.logWarning("the detector size is invalid.") << std::endl;
            error = true;
        }
    }
    else
    {
        LOGGER.logWarning("the X-ray detector has not been registered.") << std::endl;
        error = true;
    }

    // Check the geometries
    if (!m_p_outer_surface && !m_p_inner_surface_set.size())
    {
        LOGGER.logWarning("there is no 3D object to render.") << std::endl;
        error = true;
    }

    // OpenGL renderer
    if (m_computing_mode == OPENGL)
    {
        // Check the shaders
        for (std::vector<Shader>::const_iterator ite(m_shader_set.begin());
                ite != m_shader_set.end();
                ++ite)
        {
            // The shader is not valid
            if (!ite->isValid())
            {
                LOGGER.logWarning("at least one of the shader programs is not valid.") << std::endl;
                error = true;
            }
        }

        // Check the FBOs
        for (std::vector<FramebufferObject*>::const_iterator ite(m_p_fbo_set.begin());
                ite != m_p_fbo_set.end();
                ++ite)
        {
            // The FBO is not valid
            FramebufferObject* p_fbo(const_cast<FramebufferObject*>(*ite));
            if (!p_fbo->IsValid())
            {
                LOGGER.logWarning("at least one of the FBOs is not valid.") << std::endl;
                error = true;
            }
        }
    }

    return (!error);
}


//---------------------------------------
void XRayRenderer::checkGPUCapabilities()
//---------------------------------------
{
    // Check for OpenGL version
    /*if (!GLEW_VERSION_2_0)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "GPU capability error: your graphic card driver does not support OpenGL 2.0.");
    }

    if (!glewIsSupported("GL_VERSION_2_0"))
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "GPU capability error: your graphic card driver does not support OpenGL 2.0.");
    }

    if (!glewIsSupported("GL_VERSION_3_2"))
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "GPU capability error: your graphic card driver does not support OpenGL 3.2.");
    }

    // Check for OpenGL extensions
    if (!GLEW_ARB_texture_rectangle)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "GPU capability error: your graphic card driver does not support rectangular textures (ARB_texture_rectangle).");
    }

    if (!GLEW_ARB_framebuffer_object && !GLEW_EXT_framebuffer_object)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "GPU capability error: your graphic card driver does not support framebuffer objects (EXT_framebuffer_object and ARB_framebuffer_object).");
    }

    if (!GLEW_EXT_framebuffer_object)
    {
        std::cerr << "GPU capability error: your graphic card driver does not support framebuffer objects (EXT_framebuffer_object)." << std::endl;
    }

    if (!GLEW_ARB_framebuffer_object)
    {
        std::cerr << "GPU capability error: your graphic card driver does not support framebuffer objects (ARB_framebuffer_object)." << std::endl;
    }

    if (!GLEW_ARB_texture_float)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "GPU capability error: your graphic card driver does not support floating point textures (ARB_texture_float).");
    }

    if (!GLEW_ARB_shader_objects || !GLEW_ARB_fragment_program || !GLEW_ARB_vertex_program || !GLEW_ARB_fragment_shader || !GLEW_ARB_vertex_shader)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "GPU capability error: your graphic card driver does not support shader programs (ARB_shader_objects, ARB_fragment_program, ARB_vertex_program, ARB_fragment_shader or ARB_vertex_shader).");
    }

    if (!GLEW_ARB_texture_non_power_of_two)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "GPU capability error: your graphic card driver does not support non power of two textures (ARB_texture_non_power_of_two).");
    }

    if (!GLEW_ARB_pixel_buffer_object)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "GPU capability error: your graphic card driver does not support pixel buffer objects (PBOs) (GL_ARB_pixel_buffer_object).");
    }*/
/*
    if (!GLEW_EXT_texture3D)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "GPU capability error: your graphic card driver does not support 3D textures (GL_EXT_texture3D).");
    }*/
}


//----------------------------------------------------------------------------
void XRayRenderer::computeImage(const MATRIX4& aModellingTransformationMatrix,
                                bool anIntegrateEnergyFlag)
//----------------------------------------------------------------------------
{
#ifndef NDEBUG
    size_t old_FBO_stack_size = g_frame_buffer_object_stack.size();
    size_t old_texture_size = g_texture_stack.size();
    size_t old_enable_disable_stack_size = g_enable_disable_state_stack.size();
    size_t old_shader_stack_size = g_shader_program_stack.size();
#endif

    switch (m_computing_mode)
    {
    case OPENGL:
        computeImageUsingOpenGL(aModellingTransformationMatrix,
            anIntegrateEnergyFlag);
        break;

    case OPENMP:
        computeImageUsingOpenMP(aModellingTransformationMatrix,
            anIntegrateEnergyFlag);
            break;

    default:
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Unknown renderer used.");
    }

    m_use_photon_count_detector = !anIntegrateEnergyFlag;

#ifndef NDEBUG
    assert(old_FBO_stack_size == g_frame_buffer_object_stack.size());
    assert(old_texture_size == g_texture_stack.size());
    assert(old_enable_disable_stack_size == g_enable_disable_state_stack.size());
    assert(old_shader_stack_size == g_shader_program_stack.size());
#endif
}


//---------------------------------------------------------------------------------------
void XRayRenderer::computeImageUsingOpenGL(const MATRIX4& aModellingTransformationMatrix,
                                           bool anIntegrateEnergyFlag)
//---------------------------------------------------------------------------------------
{
#ifndef NDEBUG
    size_t old_FBO_stack_size = g_frame_buffer_object_stack.size();
    size_t old_texture_size = g_texture_stack.size();
    size_t old_enable_disable_stack_size = g_enable_disable_state_stack.size();
    size_t old_shader_stack_size = g_shader_program_stack.size();
#endif

    // The renderer is ready
    if (isReady())
    {
#ifdef HAS_OPENGL
        // Wait for all OpenGL execution to be completed
        glFinish();

        // Store OpenGL states
        GLint p_viewport[4];
        glGetIntegerv(GL_VIEWPORT, p_viewport);

        //glPushAttrib(GL_COLOR_BUFFER_BIT);

        // Store matrices
        pushProjectionMatrix();
        pushModelViewMatrix();

        // Update the viewport
        glViewport(0, 0, m_p_detector->getNumberOfPixels().getX(), m_p_detector->getNumberOfPixels().getY());

        // Reset the attenuation
        resetEnergyFluence();

        // Clear the FBO
        resetSumMuxDx();

        // Update m_p_surface_per_material_set if needed
        updateSurfacePerMaterialSet();

        // Process every sample of the X-ray source
        for (unsigned int sample_id(0);
                sample_id < m_p_detector->getNumberOfSourceSamples();
                ++sample_id)
        {
            {
                // Get the position of the current sample
                VEC3 xray_source_sample_centre(m_p_detector->getXraySourceCentre());
                VEC3 xray_source_sample_position(m_p_detector->getSourceSamplePosition(sample_id));
                VEC3 xray_source_detector_translation(xray_source_sample_position - xray_source_sample_centre);

                // Only update the cummulated L-buffers if the outer surface exists
                if (m_p_outer_surface && m_p_inner_surface_set.size())
                {
                    reset2DBuffer(XRAY_DETECTOR_CUMULATED_LBUFFERS_FBO_ID);
                }

                // For each object
                unsigned int object_number(0);
                //for (std::map<short, std::vector<PolygonMesh*> >::const_iterator ite(m_p_HU_polygon_mesh_set.begin());
                //    ite != m_p_HU_polygon_mesh_set.end();
                //    ++ite, ++object_number)

                for (std::vector<std::vector<SceneGraphNode*> >::const_iterator ite_material = m_p_surface_per_material_set.begin();
                        ite_material != m_p_surface_per_material_set.end();
                        ++ite_material, ++object_number)
                {
                    // Store GPU states
                    GLint p_viewport[4];
                    preComputeLBuffer(p_viewport, xray_source_detector_translation);

                    VEC3 min_corner( FLT_MAX,  FLT_MAX,  FLT_MAX);
                    VEC3 max_corner(-FLT_MAX, -FLT_MAX, -FLT_MAX);

                    VEC3 temp;

                    //unsigned int temp_id(0);
                    //for (std::vector<PolygonMesh*>::const_iterator ite_meshes(ite->second.begin());
                    //        ite_meshes != ite->second.end();
                    //        ++ite_meshes, ++temp_id)
                    for (std::vector<SceneGraphNode*>::const_iterator ite_meshes = ite_material->begin();
                            ite_meshes != ite_material->end();
                            ++ite_meshes)
                    {
                        SceneGraphNode* p_node = *ite_meshes;

                        // Compute its L-buffer
                        computeLBuffer(*p_node, xray_source_detector_translation, aModellingTransformationMatrix, false);

                        temp = p_node->getNodeAndChildrenWorldMaxCorner();
                        max_corner[0] = std::max(temp[0], max_corner[0]);
                        max_corner[1] = std::max(temp[1], max_corner[1]);
                        max_corner[2] = std::max(temp[2], max_corner[2]);

                        temp = p_node->getNodeAndChildrenWorldMinCorner();
                        min_corner[0] = std::min(temp[0], min_corner[0]);
                        min_corner[1] = std::min(temp[1], min_corner[1]);
                        min_corner[2] = std::min(temp[2], min_corner[2]);
                    }

                    // Restore GPU states
                    postComputeLBuffer(p_viewport, (max_corner - min_corner).length() / cm);

                    // Integrate the energy for every channel
                    computeSumMUxDx(object_number, ite_material->front()->getPhotonCrossSection());

                    // Only update the cummulated L-buffers if the outer surface exists
                    if (m_p_outer_surface)
                    {
                        updateCumulatedLBuffers(object_number);
                    }
                }

                // Compute the L-buffer of the outer surface that wraps the inner objects
                if (m_p_outer_surface)
                {
                    // Compute its L-buffer
                    computeLBuffer(*m_p_outer_surface, xray_source_detector_translation, aModellingTransformationMatrix);

                    // Integrate the energy for every channel
                    computeSumMUxDx(m_p_inner_surface_set.size(), m_p_outer_surface->getPhotonCrossSection());
                }

                computeIntegratedEnergy(anIntegrateEnergyFlag);
            }
        }

        // Restore the matrices
        popProjectionMatrix();
        popModelViewMatrix();

        // Restore OpenGL states
        //glPopAttrib();
        glViewport(p_viewport[0], p_viewport[1], p_viewport[2], p_viewport[3]);

        // Wait for all OpenGL execution to be completed
        glFinish();
#endif
    }
    // The renderer is not ready
    else
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "The OpenGL renderer is not ready to simulate X-ray images.");
    }

#ifndef NDEBUG
    assert(old_FBO_stack_size == g_frame_buffer_object_stack.size());
    assert(old_texture_size == g_texture_stack.size());
    assert(old_enable_disable_stack_size == g_enable_disable_state_stack.size());
    assert(old_shader_stack_size == g_shader_program_stack.size());
#endif
}


//---------------------------------------------------------------------------------------
void XRayRenderer::computeImageUsingOpenMP(const MATRIX4& aModellingTransformationMatrix,
                                           bool anIntegrateEnergyFlag)
//---------------------------------------------------------------------------------------
{
#ifndef NDEBUG
    size_t old_FBO_stack_size = g_frame_buffer_object_stack.size();
    size_t old_texture_size = g_texture_stack.size();
    size_t old_enable_disable_stack_size = g_enable_disable_state_stack.size();
    size_t old_shader_stack_size = g_shader_program_stack.size();
#endif

    // The renderer is ready
    if (isReady())
    {
        // Store matrices
        pushProjectionMatrix();
        pushModelViewMatrix();

        // Reset the attenuation
        resetEnergyFluence();

        // Clear the FBO
        resetSumMuxDx();

        // Update m_p_surface_per_material_set if needed
        updateSurfacePerMaterialSet();

        // Process every sample of the X-ray source
        for (unsigned int sample_id(0);
                sample_id < m_p_detector->getNumberOfSourceSamples();
                ++sample_id)
        {
            {
                // Get the position of the current sample
                VEC3 xray_source_sample_centre(m_p_detector->getXraySourceCentre());
                VEC3 xray_source_sample_position(m_p_detector->getSourceSamplePosition(sample_id));
                VEC3 xray_source_detector_translation(xray_source_sample_position - xray_source_sample_centre);

                // Only update the cummulated L-buffers if the outer surface exists
                if (m_p_outer_surface && m_p_inner_surface_set.size())
                {
                    reset2DBuffer(XRAY_DETECTOR_CUMULATED_LBUFFERS_FBO_ID);
                }

                // For each object
                unsigned int object_number(0);
                //for (std::map<short, std::vector<PolygonMesh*> >::const_iterator ite(m_p_HU_polygon_mesh_set.begin());
                //    ite != m_p_HU_polygon_mesh_set.end();
                //    ++ite, ++object_number)

                for (std::vector<std::vector<SceneGraphNode*> >::const_iterator ite_material = m_p_surface_per_material_set.begin();
                        ite_material != m_p_surface_per_material_set.end();
                        ++ite_material, ++object_number)
                {
                    // Store GPU states
                    GLint p_viewport[4];
                    preComputeLBuffer(p_viewport, xray_source_detector_translation);

                    VEC3 min_corner( FLT_MAX,  FLT_MAX,  FLT_MAX);
                    VEC3 max_corner(-FLT_MAX, -FLT_MAX, -FLT_MAX);

                    VEC3 temp;

                    //unsigned int temp_id(0);
                    //for (std::vector<PolygonMesh*>::const_iterator ite_meshes(ite->second.begin());
                    //        ite_meshes != ite->second.end();
                    //        ++ite_meshes, ++temp_id)
                    for (std::vector<SceneGraphNode*>::const_iterator ite_meshes = ite_material->begin();
                            ite_meshes != ite_material->end();
                            ++ite_meshes)
                    {
                        SceneGraphNode* p_node = *ite_meshes;

                        // Compute its L-buffer
                        computeLBuffer(*p_node, xray_source_detector_translation, aModellingTransformationMatrix, false);

                        temp = p_node->getNodeAndChildrenWorldMaxCorner();
                        max_corner[0] = std::max(temp[0], max_corner[0]);
                        max_corner[1] = std::max(temp[1], max_corner[1]);
                        max_corner[2] = std::max(temp[2], max_corner[2]);

                        temp = p_node->getNodeAndChildrenWorldMinCorner();
                        min_corner[0] = std::min(temp[0], min_corner[0]);
                        min_corner[1] = std::min(temp[1], min_corner[1]);
                        min_corner[2] = std::min(temp[2], min_corner[2]);
                    }

                    // Restore GPU states
                    postComputeLBuffer(p_viewport, (max_corner - min_corner).length() / cm);

                    // Integrate the energy for every channel
                    computeSumMUxDx(object_number, ite_material->front()->getPhotonCrossSection());

                    // Only update the cummulated L-buffers if the outer surface exists
                    if (m_p_outer_surface)
                    {
                        updateCumulatedLBuffers(object_number);
                    }
                }

                // Compute the L-buffer of the outer surface that wraps the inner objects
                if (m_p_outer_surface)
                {
                    // Compute its L-buffer
                    computeLBuffer(*m_p_outer_surface, xray_source_detector_translation, aModellingTransformationMatrix);

                    // Integrate the energy for every channel
                    computeSumMUxDx(m_p_inner_surface_set.size(), m_p_outer_surface->getPhotonCrossSection());
                }

                computeIntegratedEnergy(anIntegrateEnergyFlag);
            }
        }

        // Restore the matrices
        popProjectionMatrix();
        popModelViewMatrix();
    }
    // The renderer is not ready
    else
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "The OpenMP renderer is not ready to simulate X-ray images.");
    }

#ifndef NDEBUG
    assert(old_FBO_stack_size == g_frame_buffer_object_stack.size());
    assert(old_texture_size == g_texture_stack.size());
    assert(old_enable_disable_stack_size == g_enable_disable_state_stack.size());
    assert(old_shader_stack_size == g_shader_program_stack.size());
#endif
}


//------------------------------
void XRayRenderer::initShaders()
//------------------------------
{
#ifndef NDEBUG
    size_t old_FBO_stack_size = g_frame_buffer_object_stack.size();
    size_t old_texture_size = g_texture_stack.size();
    size_t old_enable_disable_stack_size = g_enable_disable_state_stack.size();
    size_t old_shader_stack_size = g_shader_program_stack.size();
#endif

#ifdef HAS_OPENGL
    // Store the current shader
    GLint current_program(0);
    glGetIntegerv(GL_CURRENT_PROGRAM, &current_program);

    // Initialise the shaders
    char* p_vertex_shader(0);
    char* p_fragment_shader(0);

    int z_lib_return_code_vertex(0);
    int z_lib_return_code_fragment(0);

    std::string vertex_shader;
    std::string fragment_shader;

    bool use_opengl45 = useOpenGL45();
    bool use_opengl32 = false;

    if (!use_opengl45)
    {
        use_opengl32 = useOpenGL32();
    }

    if (use_opengl45)
    {
        z_lib_return_code_vertex   = inflate(g_l_buffer_gl4_vert, sizeof(g_l_buffer_gl4_vert),   &p_vertex_shader);
        z_lib_return_code_fragment = inflate(g_l_buffer_gl4_frag, sizeof(g_l_buffer_gl4_frag), &p_fragment_shader);
    }
    else if (use_opengl32)
    {
        z_lib_return_code_vertex   = inflate(g_l_buffer_gl3_vert, sizeof(g_l_buffer_gl3_vert),   &p_vertex_shader);
        z_lib_return_code_fragment = inflate(g_l_buffer_gl3_frag, sizeof(g_l_buffer_gl3_frag), &p_fragment_shader);
    }
    else
    {
        z_lib_return_code_vertex   = inflate(g_l_buffer_gl2_vert, sizeof(g_l_buffer_gl2_vert),   &p_vertex_shader);
        z_lib_return_code_fragment = inflate(g_l_buffer_gl2_frag, sizeof(g_l_buffer_gl2_frag), &p_fragment_shader);
        // throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot create shader programs for your chosen OpenGL context.");
    }

    vertex_shader   = p_vertex_shader;
    fragment_shader = p_fragment_shader;
    delete [] p_vertex_shader;     p_vertex_shader = 0;
    delete [] p_fragment_shader; p_fragment_shader = 0;

    if (z_lib_return_code_vertex <= 0 || z_lib_return_code_fragment <= 0 || !vertex_shader.size() || !fragment_shader.size())
    {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot decode the shader using ZLib.");
    }
    m_shader_set[XRAY_DETECTOR_LBUFFER_SHADER_ID].setLabels("l_buffer.vert", "l_buffer.frag");
    m_shader_set[XRAY_DETECTOR_LBUFFER_SHADER_ID].loadSource(vertex_shader, fragment_shader);

    // Cumulated L-buffer
    if (use_opengl45)
    {
        z_lib_return_code_vertex   = inflate(g_cumulated_l_buffer_gl4_vert, sizeof(g_cumulated_l_buffer_gl4_vert),   &p_vertex_shader);
        z_lib_return_code_fragment = inflate(g_cumulated_l_buffer_gl4_frag, sizeof(g_cumulated_l_buffer_gl4_frag), &p_fragment_shader);
    }
    else if (use_opengl32)
    {
        z_lib_return_code_vertex   = inflate(g_cumulated_l_buffer_gl3_vert, sizeof(g_cumulated_l_buffer_gl3_vert),   &p_vertex_shader);
        z_lib_return_code_fragment = inflate(g_cumulated_l_buffer_gl3_frag, sizeof(g_cumulated_l_buffer_gl3_frag), &p_fragment_shader);
    }
    else
    {
        z_lib_return_code_vertex   = inflate(g_cumulated_l_buffer_gl2_vert, sizeof(g_cumulated_l_buffer_gl2_vert),   &p_vertex_shader);
        z_lib_return_code_fragment = inflate(g_cumulated_l_buffer_gl2_frag, sizeof(g_cumulated_l_buffer_gl2_frag), &p_fragment_shader);
        // throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot create shader programs for your chosen OpenGL context.");
    }

    vertex_shader   = p_vertex_shader;
    fragment_shader = p_fragment_shader;
    delete [] p_vertex_shader;     p_vertex_shader = 0;
    delete [] p_fragment_shader; p_fragment_shader = 0;

    if (z_lib_return_code_vertex <= 0 || z_lib_return_code_fragment <= 0 || !vertex_shader.size() || !fragment_shader.size())
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot decode the shader using ZLib.");
    }
    m_shader_set[XRAY_DETECTOR_CUMULATED_LBUFFER_SHADER_ID].setLabels("cumulated_l_buffer.vert", "cumulated_l_buffer.frag");
    m_shader_set[XRAY_DETECTOR_CUMULATED_LBUFFER_SHADER_ID].loadSource(vertex_shader, fragment_shader);

    // Clean L-buffer
    if (use_opengl45)
    {
        z_lib_return_code_vertex   = inflate(g_clean_l_buffer_gl4_vert, sizeof(g_clean_l_buffer_gl4_vert),   &p_vertex_shader);
        z_lib_return_code_fragment = inflate(g_clean_l_buffer_gl4_frag, sizeof(g_clean_l_buffer_gl4_frag), &p_fragment_shader);
    }
    else if (use_opengl32)
    {
        z_lib_return_code_vertex   = inflate(g_clean_l_buffer_gl3_vert, sizeof(g_clean_l_buffer_gl3_vert),   &p_vertex_shader);
        z_lib_return_code_fragment = inflate(g_clean_l_buffer_gl3_frag, sizeof(g_clean_l_buffer_gl3_frag), &p_fragment_shader);
    }
    else
    {
        z_lib_return_code_vertex   = inflate(g_clean_l_buffer_gl2_vert, sizeof(g_clean_l_buffer_gl2_vert),   &p_vertex_shader);
        z_lib_return_code_fragment = inflate(g_clean_l_buffer_gl2_frag, sizeof(g_clean_l_buffer_gl2_frag), &p_fragment_shader);
        // throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot create shader programs for your chosen OpenGL context.");
    }

    vertex_shader   = p_vertex_shader;
    fragment_shader = p_fragment_shader;
    delete [] p_vertex_shader;     p_vertex_shader = 0;
    delete [] p_fragment_shader; p_fragment_shader = 0;

    if (z_lib_return_code_vertex <= 0 || z_lib_return_code_fragment <= 0 || !vertex_shader.size() || !fragment_shader.size())
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot decode the shader using ZLib.");
    }
    m_shader_set[XRAY_DETECTOR_CLEAN_LBUFFER_SHADER_ID].setLabels("clean_l_buffer.vert", "clean_l_buffer.frag");
    m_shader_set[XRAY_DETECTOR_CLEAN_LBUFFER_SHADER_ID].loadSource(vertex_shader, fragment_shader);

        // Outer L-buffer
    if (use_opengl45)
    {
        z_lib_return_code_vertex   = inflate(g_outer_surface_l_buffer_gl4_vert, sizeof(g_outer_surface_l_buffer_gl4_vert),   &p_vertex_shader);
        z_lib_return_code_fragment = inflate(g_outer_surface_l_buffer_gl4_frag, sizeof(g_outer_surface_l_buffer_gl4_frag), &p_fragment_shader);
    }
    else if (use_opengl32)
    {
        z_lib_return_code_vertex   = inflate(g_outer_surface_l_buffer_gl3_vert, sizeof(g_outer_surface_l_buffer_gl3_vert),   &p_vertex_shader);
        z_lib_return_code_fragment = inflate(g_outer_surface_l_buffer_gl3_frag, sizeof(g_outer_surface_l_buffer_gl3_frag), &p_fragment_shader);
    }
    else
    {
        z_lib_return_code_vertex   = inflate(g_outer_surface_l_buffer_gl2_vert, sizeof(g_outer_surface_l_buffer_gl2_vert),   &p_vertex_shader);
        z_lib_return_code_fragment = inflate(g_outer_surface_l_buffer_gl2_frag, sizeof(g_outer_surface_l_buffer_gl2_frag), &p_fragment_shader);
        // throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot create shader programs for your chosen OpenGL context.");
    }

    vertex_shader   = p_vertex_shader;
    fragment_shader = p_fragment_shader;
    delete [] p_vertex_shader;     p_vertex_shader = 0;
    delete [] p_fragment_shader; p_fragment_shader = 0;

    if (z_lib_return_code_vertex <= 0 || z_lib_return_code_fragment <= 0 || !vertex_shader.size() || !fragment_shader.size())
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot decode the shader using ZLib.");
    }
    m_shader_set[XRAY_DETECTOR_OUTER_SURFACE_LBUFFER_SHADER_ID].setLabels("outer_surface_l_buffer.vert", "outer_surface_l_buffer.frag");
    m_shader_set[XRAY_DETECTOR_OUTER_SURFACE_LBUFFER_SHADER_ID].loadSource(vertex_shader, fragment_shader);

        // Mu X Dx
    if (use_opengl45)
    {
        z_lib_return_code_vertex   = inflate(g_MUx_Dx_gl4_vert, sizeof(g_MUx_Dx_gl4_vert),   &p_vertex_shader);
        z_lib_return_code_fragment = inflate(g_MUx_Dx_gl4_frag, sizeof(g_MUx_Dx_gl4_frag), &p_fragment_shader);
    }
    else if (use_opengl32)
    {
        z_lib_return_code_vertex   = inflate(g_MUx_Dx_gl3_vert, sizeof(g_MUx_Dx_gl3_vert),   &p_vertex_shader);
        z_lib_return_code_fragment = inflate(g_MUx_Dx_gl3_frag, sizeof(g_MUx_Dx_gl3_frag), &p_fragment_shader);
    }
    else
    {
        z_lib_return_code_vertex   = inflate(g_MUx_Dx_gl2_vert, sizeof(g_MUx_Dx_gl2_vert),   &p_vertex_shader);
        z_lib_return_code_fragment = inflate(g_MUx_Dx_gl2_frag, sizeof(g_MUx_Dx_gl2_frag), &p_fragment_shader);
        // throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot create shader programs for your chosen OpenGL context.");
    }

    vertex_shader   = p_vertex_shader;
    fragment_shader = p_fragment_shader;
    delete [] p_vertex_shader;     p_vertex_shader = 0;
    delete [] p_fragment_shader; p_fragment_shader = 0;

    if (z_lib_return_code_vertex <= 0 || z_lib_return_code_fragment <= 0 || !vertex_shader.size() || !fragment_shader.size())
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot decode the shader using ZLib.");
    }
    m_shader_set[XRAY_DETECTOR_SUM_MUx_Dx_SHADER_ID].setLabels("MUx_Dx.vert", "MUx_Dx.frag");
    m_shader_set[XRAY_DETECTOR_SUM_MUx_Dx_SHADER_ID].loadSource(vertex_shader, fragment_shader);

    // Energy fluence
    if (use_opengl45)
    {
        z_lib_return_code_vertex   = inflate(g_xray_attenuation_gl4_vert, sizeof(g_xray_attenuation_gl4_vert),   &p_vertex_shader);
        z_lib_return_code_fragment = inflate(g_xray_attenuation_gl4_frag, sizeof(g_xray_attenuation_gl4_frag), &p_fragment_shader);
    }
    else if (use_opengl32)
    {
        z_lib_return_code_vertex   = inflate(g_xray_attenuation_gl3_vert, sizeof(g_xray_attenuation_gl3_vert),   &p_vertex_shader);
        z_lib_return_code_fragment = inflate(g_xray_attenuation_gl3_frag, sizeof(g_xray_attenuation_gl3_frag), &p_fragment_shader);
    }
    else
    {
        z_lib_return_code_vertex   = inflate(g_xray_attenuation_gl2_vert, sizeof(g_xray_attenuation_gl2_vert),   &p_vertex_shader);
        z_lib_return_code_fragment = inflate(g_xray_attenuation_gl2_frag, sizeof(g_xray_attenuation_gl2_frag), &p_fragment_shader);
        // throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot create shader programs for your chosen OpenGL context.");
    }

    vertex_shader   = p_vertex_shader;
    fragment_shader = p_fragment_shader;
    delete [] p_vertex_shader;     p_vertex_shader = 0;
    delete [] p_fragment_shader; p_fragment_shader = 0;

    if (z_lib_return_code_vertex <= 0 || z_lib_return_code_fragment <= 0 || !vertex_shader.size() || !fragment_shader.size())
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot decode the shader using ZLib.");
    }
    m_shader_set[XRAY_DETECTOR_ENERGY_FLUENCE_SHADER_ID].setLabels("xray_attenuation.vert", "xray_attenuation.frag");
    m_shader_set[XRAY_DETECTOR_ENERGY_FLUENCE_SHADER_ID].loadSource(vertex_shader, fragment_shader);

    // Display
    if (use_opengl45)
    {
        z_lib_return_code_vertex   = inflate(g_xray_display_gl4_vert, sizeof(g_xray_display_gl4_vert),   &p_vertex_shader);
        z_lib_return_code_fragment = inflate(g_xray_display_gl4_frag, sizeof(g_xray_display_gl4_frag), &p_fragment_shader);
    }
    else if (use_opengl32)
    {
        z_lib_return_code_vertex   = inflate(g_xray_display_gl3_vert, sizeof(g_xray_display_gl3_vert),   &p_vertex_shader);
        z_lib_return_code_fragment = inflate(g_xray_display_gl3_frag, sizeof(g_xray_display_gl3_frag), &p_fragment_shader);
    }
    else
    {
        z_lib_return_code_vertex   = inflate(g_xray_display_gl2_vert, sizeof(g_xray_display_gl2_vert),   &p_vertex_shader);
        z_lib_return_code_fragment = inflate(g_xray_display_gl2_frag, sizeof(g_xray_display_gl2_frag), &p_fragment_shader);
        // throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot create shader programs for your chosen OpenGL context.");
    }

    vertex_shader   = p_vertex_shader;
    fragment_shader = p_fragment_shader;
    delete [] p_vertex_shader;     p_vertex_shader = 0;
    delete [] p_fragment_shader; p_fragment_shader = 0;

    if (z_lib_return_code_vertex <= 0 || z_lib_return_code_fragment <= 0 || !vertex_shader.size() || !fragment_shader.size())
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot decode the shader using ZLib.");
    }
    m_shader_set[XRAY_DETECTOR_DISPLAY_SHADER_ID].setLabels("xray_display.vert", "xray_display.frag");
    m_shader_set[XRAY_DETECTOR_DISPLAY_SHADER_ID].loadSource(vertex_shader, fragment_shader);

    // Restore the current shader
    glUseProgram(current_program);
#endif

#ifndef NDEBUG
    assert(old_FBO_stack_size == g_frame_buffer_object_stack.size());
    assert(old_texture_size == g_texture_stack.size());
    assert(old_enable_disable_stack_size == g_enable_disable_state_stack.size());
    assert(old_shader_stack_size == g_shader_program_stack.size());
#endif
}


//-----------------------------
void XRayRenderer::createFBOs()
//-----------------------------
{
#ifndef NDEBUG
    size_t old_FBO_stack_size = g_frame_buffer_object_stack.size();
    size_t old_texture_size = g_texture_stack.size();
    size_t old_enable_disable_stack_size = g_enable_disable_state_stack.size();
    size_t old_shader_stack_size = g_shader_program_stack.size();
#endif

    // Delete the FBOs if they already exist
    deleteFBOs();

    // Create the FBO (the number depends on the computing mode)
    switch (m_computing_mode)
    {
    case OPENGL:
        for (int i(0); i < XRAY_DETECTOR_2D_FBO_NUMBER + XRAY_DETECTOR_3D_FBO_NUMBER; ++i)
        {
            m_p_fbo_set.push_back(new FramebufferObject);
        }
        break;

    case CUDA:
#ifdef HAS_CUDA
        m_p_fbo_set.resize(XRAY_DETECTOR_2D_FBO_NUMBER - 1);
#endif // HAS_CUDA
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "CUDA is not supported.");
        break;

    case OPENCL:
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "OpenCL is not supported.");
        break;

    default:
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Unsupported computing mode.");
        break;
    }

#ifndef NDEBUG
    assert(old_FBO_stack_size == g_frame_buffer_object_stack.size());
    assert(old_texture_size == g_texture_stack.size());
    assert(old_enable_disable_stack_size == g_enable_disable_state_stack.size());
    assert(old_shader_stack_size == g_shader_program_stack.size());
#endif
}


//-----------------------------
void XRayRenderer::deleteFBOs()
//-----------------------------
{
#ifndef NDEBUG
    size_t old_FBO_stack_size = g_frame_buffer_object_stack.size();
    size_t old_texture_size = g_texture_stack.size();
    size_t old_enable_disable_stack_size = g_enable_disable_state_stack.size();
    size_t old_shader_stack_size = g_shader_program_stack.size();
#endif

    // Delete the FBOs
    for (std::vector<FramebufferObject*>::iterator ite(m_p_fbo_set.begin());
            ite != m_p_fbo_set.end();
            ++ite)
    {
        if (*ite)
        {
            delete *ite;
            *ite = 0;
        }
    }

    m_p_fbo_set.clear();

#ifndef NDEBUG
    assert(old_FBO_stack_size == g_frame_buffer_object_stack.size());
    assert(old_texture_size == g_texture_stack.size());
    assert(old_enable_disable_stack_size == g_enable_disable_state_stack.size());
    assert(old_shader_stack_size == g_shader_program_stack.size());
#endif
}


//----------------------------------------
void XRayRenderer::createPBO(GLuint& aPBO)
//----------------------------------------
{
#ifndef NDEBUG
    size_t old_FBO_stack_size = g_frame_buffer_object_stack.size();
    size_t old_texture_size = g_texture_stack.size();
    size_t old_enable_disable_stack_size = g_enable_disable_state_stack.size();
    size_t old_shader_stack_size = g_shader_program_stack.size();
#endif

#ifdef HAS_OPENGL
    // Store the current buffer
    GLint current_buffer(0);
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &current_buffer);

    // Set up vertex data parameter
    GLuint texture_size(m_p_detector->getNumberOfPixels().getX() * m_p_detector->getNumberOfPixels().getY());
    GLfloat* p_data(new GLfloat[texture_size]);

    // create buffer object
    glGenBuffers(1, &aPBO);
    glBindBuffer(GL_ARRAY_BUFFER, aPBO);
    glBufferData(GL_ARRAY_BUFFER, texture_size * sizeof(GLfloat), p_data, GL_DYNAMIC_DRAW);
    delete [] p_data;

#ifdef HAS_CUDA
    // register this buffer object with CUDA
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    cudaGLRegisterBufferObject(aPBO);
    if (CheckCudaError())
    {
        LOGGER.logError("Cannot bind a PBO to CUDA.") << std::endl;
        return (false);
    }
#endif // HAS_CUDA

    // Restore the current buffer
    glBindBuffer(GL_ARRAY_BUFFER, current_buffer);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
#endif

#ifndef NDEBUG
    assert(old_FBO_stack_size == g_frame_buffer_object_stack.size());
    assert(old_texture_size == g_texture_stack.size());
    assert(old_enable_disable_stack_size == g_enable_disable_state_stack.size());
    assert(old_shader_stack_size == g_shader_program_stack.size());
#endif
}


//----------------------------------------
void XRayRenderer::deletePBO(GLuint& aPBO)
//----------------------------------------
{
#ifndef NDEBUG
    size_t old_FBO_stack_size = g_frame_buffer_object_stack.size();
    size_t old_texture_size = g_texture_stack.size();
    size_t old_enable_disable_stack_size = g_enable_disable_state_stack.size();
    size_t old_shader_stack_size = g_shader_program_stack.size();
#endif

#ifdef HAS_OPENGL
    // Store the current buffer
    GLint current_buffer(0);
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &current_buffer);

    // Check the value
    if (current_buffer < 0)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid index.");
    }

    // Unbind the buffer if necessary
    if (GLuint(current_buffer) == aPBO)
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
    }

#ifdef HAS_CUDA
    // Unregister this buffer object with CUDA
    cudaGLUnregisterBufferObject(aPBO);
    if (CheckCudaError())
    {
        return_value = false;
    }
#endif // HAS_CUDA

    // Destroy the buffer
    glBindBuffer(GL_ARRAY_BUFFER, aPBO);
    glDeleteBuffers(1, &aPBO);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Unbind the buffer if necessary
    if (GLuint(current_buffer) == aPBO)
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    else
    {
        glBindBuffer(GL_ARRAY_BUFFER, current_buffer);
    }
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
#endif

    aPBO = 0;

#ifndef NDEBUG
    assert(old_FBO_stack_size == g_frame_buffer_object_stack.size());
    assert(old_texture_size == g_texture_stack.size());
    assert(old_enable_disable_stack_size == g_enable_disable_state_stack.size());
    assert(old_shader_stack_size == g_shader_program_stack.size());
#endif
}


//-----------------------------
void XRayRenderer::init3DData()
//-----------------------------
{
#ifndef NDEBUG
    size_t old_FBO_stack_size = g_frame_buffer_object_stack.size();
    size_t old_texture_size = g_texture_stack.size();
    size_t old_enable_disable_stack_size = g_enable_disable_state_stack.size();
    size_t old_shader_stack_size = g_shader_program_stack.size();
#endif

    // Store the texturing flags
    pushTexture3D();

    switch (m_computing_mode)
    {
#ifdef HAS_OPENGL
        case OPENGL:
        {
            // Use full-precision
            int internal_format = GL_RGB32F;

            // Use half-precision
            if (!getPrecisionSumMuXdX(m_mixed_precision_flag))
            {
                internal_format = GL_RGB16F;
            }
                
            // Delete the texture if it exists
            if (m_p_texture_name_set[XRAY_DETECTOR_SUM_MUx_Dx_TEXTURE_NAME_ID])
            {
                glDeleteTextures(1, &m_p_texture_name_set[XRAY_DETECTOR_SUM_MUx_Dx_TEXTURE_NAME_ID]);
            }

            // Generate the texture name.
            glGenTextures(1, &m_p_texture_name_set[XRAY_DETECTOR_SUM_MUx_Dx_TEXTURE_NAME_ID]);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            // Set the texture properties
            glBindTexture(GL_TEXTURE_3D, m_p_texture_name_set[XRAY_DETECTOR_SUM_MUx_Dx_TEXTURE_NAME_ID]);
            glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            // Check the texture size
            GLint texture_size;
            glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &texture_size);
            if (m_p_detector->getNumberOfPixels().getX() > GLuint(texture_size) ||
                m_p_detector->getNumberOfPixels().getY() > GLuint(texture_size) ||
                m_p_xray_beam->getEnergyChannelNumber() > GLuint(texture_size))
            {
                std::stringstream error_message;
                error_message << "Cannot create a " << "(" << m_p_detector->getNumberOfPixels().getX() << ", " << m_p_detector->getNumberOfPixels().getY() << ", " << m_p_xray_beam->getEnergyChannelNumber() << ") floating point render texture." << std::endl;
                error_message << "The biggest 3D texture with this graphics card is " << "(" << texture_size << ", " << texture_size << ", " << texture_size << ").";
                throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
            }

            GLint width(0);
            glTexImage3D(GL_PROXY_TEXTURE_3D, 0, internal_format, m_p_detector->getNumberOfPixels().getX(), m_p_detector->getNumberOfPixels().getY(), m_p_xray_beam->getEnergyChannelNumber(), 0, GL_RGB, GL_FLOAT, 0);
            glGetTexLevelParameteriv(GL_PROXY_TEXTURE_3D, 0, GL_TEXTURE_WIDTH, &width);
            if ((unsigned int)(width) == m_p_detector->getNumberOfPixels().getX())
            {
                glTexImage3D(GL_TEXTURE_3D, 0, internal_format, m_p_detector->getNumberOfPixels().getX(), m_p_detector->getNumberOfPixels().getY(), m_p_xray_beam->getEnergyChannelNumber(), 0, GL_RGB, GL_FLOAT, 0);
            }
            else
            {
                std::stringstream error_message;
                error_message << "Cannot create a " << "(" << m_p_detector->getNumberOfPixels().getX() << ", " << m_p_detector->getNumberOfPixels().getY()  << ", " << m_p_xray_beam->getEnergyChannelNumber() << ") floating point render texture.";
                throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
            }

            // Check if any OpenGL error had occured
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            break;
        }
#endif

#ifdef HAS_OPENMP
        case OPENMP:
            break;
#endif

        case CUDA:
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "CUDA is not supported.");
            break;

        case OPENCL:
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "OpenCL is not supported.");
            break;

        default:
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "Unsupported computing mode.");
            break;
    }

    // Restore the texturing flags
    popTexture();

#ifndef NDEBUG
    assert(old_FBO_stack_size == g_frame_buffer_object_stack.size());
    assert(old_texture_size == g_texture_stack.size());
    assert(old_enable_disable_stack_size == g_enable_disable_state_stack.size());
    assert(old_shader_stack_size == g_shader_program_stack.size());
#endif
}


//------------------------------------------------------------------------------
void XRayRenderer::init2DFBO(unsigned int aFBOIndex, unsigned int aColourTextureIndex, unsigned int aZBufferTextureName)
//------------------------------------------------------------------------------
{
#ifndef NDEBUG
    size_t old_FBO_stack_size = g_frame_buffer_object_stack.size();
    size_t old_texture_size = g_texture_stack.size();
    size_t old_enable_disable_stack_size = g_enable_disable_state_stack.size();
    size_t old_shader_stack_size = g_shader_program_stack.size();
#endif

    // Save the current FBO
    pushFBO();

    // Bind the FBO if needed
    m_p_fbo_set[aFBOIndex]->Bind();
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Attach color texture to FBO
    if (!aZBufferTextureName)
    {
        m_p_fbo_set[aFBOIndex]->AttachTexture(GL_TEXTURE_2D, m_p_texture_name_set[aColourTextureIndex], GL_COLOR_ATTACHMENT0);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
    }
    else
    {
        GLenum texTarget[] = {GL_TEXTURE_2D, GL_TEXTURE_2D};
        GLuint texId[] = {m_p_texture_name_set[aColourTextureIndex], aZBufferTextureName};
        GLenum attachment[] = {GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT};

        m_p_fbo_set[aFBOIndex]->AttachTextures(2, texTarget, texId, attachment);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
    }

#ifdef HAS_OPENGL
    // Change draw/read buffers
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
#endif

    // Check the FBO
    if (!m_p_fbo_set[aFBOIndex]->IsValid())
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "The FBO has not been successfully created.");
    }

    // Clear the FBO
    //Reset2DBuffer(aFBOIndex);

    // Restore the current FBO
    popFBO();

#ifndef NDEBUG
    assert(old_FBO_stack_size == g_frame_buffer_object_stack.size());
    assert(old_texture_size == g_texture_stack.size());
    assert(old_enable_disable_stack_size == g_enable_disable_state_stack.size());
    assert(old_shader_stack_size == g_shader_program_stack.size());
#endif
}


//-----------------------------
void XRayRenderer::init3DFBOs()
//-----------------------------
{
#ifndef NDEBUG
    size_t old_FBO_stack_size = g_frame_buffer_object_stack.size();
    size_t old_texture_size = g_texture_stack.size();
    size_t old_enable_disable_stack_size = g_enable_disable_state_stack.size();
    size_t old_shader_stack_size = g_shader_program_stack.size();
#endif

    // Save the current FBO
    pushFBO();

    if (m_computing_mode == OPENGL)
    {
        // Initialise FBOs
        for (int i(0); i < XRAY_DETECTOR_3D_FBO_NUMBER; ++i)
        {
            // Bind the FBO
            m_p_fbo_set[XRAY_DETECTOR_2D_FBO_NUMBER + i]->Bind();
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            // Attach color texture to FBO
            // if (m_p_fbo_set[XRAY_DETECTOR_2D_FBO_NUMBER + i]->GetAttachedId(GL_COLOR_ATTACHMENT0) != m_p_texture_name_set[XRAY_DETECTOR_2D_TEXTURE_NUMBER + i])
            {
                m_p_fbo_set[XRAY_DETECTOR_2D_FBO_NUMBER + i]->AttachTexture(GL_TEXTURE_3D, m_p_texture_name_set[XRAY_DETECTOR_2D_TEXTURE_NUMBER + i], GL_COLOR_ATTACHMENT0, 0, 0);
                //glFramebufferTexture3DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, m_3D_texture_target, m_p_texture_name_set[XRAY_DETECTOR_2D_TEXTURE_NUMBER + i], 0, 0);
                checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
            }

#ifdef HAS_OPENGL
            // Change draw/read buffers
            glDrawBuffer(GL_COLOR_ATTACHMENT0);
            glReadBuffer(GL_COLOR_ATTACHMENT0);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
#endif

            // Check the FBO
            if (!m_p_fbo_set[XRAY_DETECTOR_2D_FBO_NUMBER + i]->IsValid())
            {
                throw Exception(__FILE__, __FUNCTION__, __LINE__, "The FBO has not been successfully created.");
            }

            // Clear the FBO
            //Reset3DBuffer(XRAY_DETECTOR_2D_FBO_NUMBER + i);
        }
    }

    // Restore the current FBO
    popFBO();

#ifndef NDEBUG
    assert(old_FBO_stack_size == g_frame_buffer_object_stack.size());
    assert(old_texture_size == g_texture_stack.size());
    assert(old_enable_disable_stack_size == g_enable_disable_state_stack.size());
    assert(old_shader_stack_size == g_shader_program_stack.size());
#endif
}


//-------------------------------------------------------------
void XRayRenderer::create2DTexture(GLuint* apTextureID,
                                   int anInternalTextureFormat,
                                   int aType)
//-------------------------------------------------------------
{
#ifndef NDEBUG
    size_t old_FBO_stack_size = g_frame_buffer_object_stack.size();
    size_t old_texture_size = g_texture_stack.size();
    size_t old_enable_disable_stack_size = g_enable_disable_state_stack.size();
    size_t old_shader_stack_size = g_shader_program_stack.size();
#endif

    // Store the texturing flags
    pushTexture2D();
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

#ifdef HAS_OPENGL
    // Generate the texture name.
    glGenTextures(1, apTextureID);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Set the texture properties
    glBindTexture(GL_TEXTURE_2D, *apTextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    GLint width(0);
    GLenum data_type = GL_FLOAT;

    // Z-buffer may use unsigned integers rather than floats
    if (anInternalTextureFormat == GL_DEPTH_COMPONENT16)
    {
        data_type = GL_UNSIGNED_SHORT;
    }
    else if (anInternalTextureFormat == GL_DEPTH_COMPONENT24)
    {
        data_type = GL_UNSIGNED_INT;
    }
    else if (anInternalTextureFormat == GL_DEPTH_COMPONENT32)
    {
        data_type = GL_UNSIGNED_INT;
    }

    glTexImage2D(GL_PROXY_TEXTURE_2D, 0, anInternalTextureFormat, m_p_detector->getNumberOfPixels().getX(), m_p_detector->getNumberOfPixels().getY(), 0, aType, data_type, 0);
    glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    if ((unsigned int)(width) == m_p_detector->getNumberOfPixels().getX())
    {
        glTexImage2D(GL_TEXTURE_2D, 0, anInternalTextureFormat, m_p_detector->getNumberOfPixels().getX(), m_p_detector->getNumberOfPixels().getY(), 0, aType, data_type, 0);
    }
    else
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot create a floating point render texture.");
    }

    // Check if any OpenGL error had occurred
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
#endif

    // Restore the texturing flags
    popTexture();

#ifndef NDEBUG
    assert(old_FBO_stack_size == g_frame_buffer_object_stack.size());
    assert(old_texture_size == g_texture_stack.size());
    assert(old_enable_disable_stack_size == g_enable_disable_state_stack.size());
    assert(old_shader_stack_size == g_shader_program_stack.size());
#endif
}


//---------------------------------------------------
void XRayRenderer::reset2DBuffer(GLuint aFrameBuffer)
//---------------------------------------------------
{
#ifndef NDEBUG
    size_t old_FBO_stack_size = g_frame_buffer_object_stack.size();
    size_t old_texture_size = g_texture_stack.size();
    size_t old_enable_disable_stack_size = g_enable_disable_state_stack.size();
    size_t old_shader_stack_size = g_shader_program_stack.size();
#endif

    switch (m_computing_mode)
    {
#ifdef HAS_OPENGL
        case OPENGL:
        {
            // Save the current FBO
            pushFBO();

            // Bind frame buffer object
            m_p_fbo_set[aFrameBuffer]->Bind();

            // Store the current clear color
            GLfloat p_clear_color[4];
            glGetFloatv(GL_COLOR_CLEAR_VALUE, p_clear_color);

            // Save the viwport
            GLint p_viewport[4];
            glGetIntegerv(GL_VIEWPORT, &p_viewport[0]);

            // Change the viewport
            glViewport(0, 0, m_p_detector->getNumberOfPixels().getX(), m_p_detector->getNumberOfPixels().getY());

            // Make sure the clear color is black (to reset FBOs to 0)
            glClearColor(0.0, 0.0, 0.0, 0.0);

            // Clear the frame buffer
            glClear(GL_COLOR_BUFFER_BIT);

            // Wait for all OpenGL execution to be completed
            glFinish();

            // Restore
            glClearColor(p_clear_color[0], p_clear_color[1], p_clear_color[2], p_clear_color[3]);

            // Restore the viewport
            glViewport(p_viewport[0], p_viewport[1], p_viewport[2], p_viewport[3]);

            // Check if any OpenGL error had occured
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            // Restore the current FBO
            popFBO();
        }
        break;
#endif

    case OPENMP:
        {
            switch (aFrameBuffer)
            {
                case XRAY_DETECTOR_ENERGY_FLUENCE_FBO_ID:
                    m_energy_fluence_image.makeBlank();
                    break;

                case XRAY_DETECTOR_UNCLEANED_LBUFFER_FBO_ID:
                    m_l_buffer_image.makeBlank();
                    break;

                case XRAY_DETECTOR_CUMULATED_LBUFFERS_FBO_ID:
                    m_cumulated_lbuffer_image.makeBlank();
                    break;

                case XRAY_DETECTOR_SUM_MUx_Dx_FBO_ID:
                    m_sum_mux_dx_image.makeBlank();
                    break;

                default:
                    throw Exception(__FILE__, __FUNCTION__, __LINE__, "Unsupported buffer.");
            }
        }
        break;

        default:
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "Unsupported renderer.");
    }

#ifndef NDEBUG
    assert(old_FBO_stack_size == g_frame_buffer_object_stack.size());
    assert(old_texture_size == g_texture_stack.size());
    assert(old_enable_disable_stack_size == g_enable_disable_state_stack.size());
    assert(old_shader_stack_size == g_shader_program_stack.size());
#endif
}


//-------------------------------
void XRayRenderer::resetLBuffer()
//-------------------------------
{
#ifndef NDEBUG
    size_t old_FBO_stack_size = g_frame_buffer_object_stack.size();
    size_t old_texture_size = g_texture_stack.size();
    size_t old_enable_disable_stack_size = g_enable_disable_state_stack.size();
    size_t old_shader_stack_size = g_shader_program_stack.size();
#endif

    // The L-buffer is not empty
    if (!m_l_buffer_empty)
    {
        switch (m_computing_mode)
        {
#ifdef HAS_OPENGL
            case OPENGL:
            reset2DBuffer(XRAY_DETECTOR_UNCLEANED_LBUFFER_FBO_ID);

            if (m_use_l_buffer_artefact_filtering_on_gpu || m_use_l_buffer_artefact_filtering_on_cpu)
            {
                reset2DBuffer(XRAY_DETECTOR_CLEANED_LBUFFER_FBO_ID);
            }
            break;
#endif

        case OPENMP:
            reset2DBuffer(XRAY_DETECTOR_UNCLEANED_LBUFFER_FBO_ID);
            break;

        case CUDA:
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "CUDA is not supported.");
            break;

        case OPENCL:
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "OpenCL is not supported.");
            break;

        default:
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "Unsupported computing mode.");
            break;
        }
    }

    // The L-buffer is now empty
    m_l_buffer_empty = true;

#ifndef NDEBUG
    assert(old_FBO_stack_size == g_frame_buffer_object_stack.size());
    assert(old_texture_size == g_texture_stack.size());
    assert(old_enable_disable_stack_size == g_enable_disable_state_stack.size());
    assert(old_shader_stack_size == g_shader_program_stack.size());
#endif
}


//------------------------------------------------
void XRayRenderer::printFBO(GLuint aFBOID,
                            const char* aFileName,
                            bool useCompression)
//------------------------------------------------
{
#ifndef NDEBUG
    size_t old_FBO_stack_size = g_frame_buffer_object_stack.size();
    size_t old_texture_size = g_texture_stack.size();
    size_t old_enable_disable_stack_size = g_enable_disable_state_stack.size();
    size_t old_shader_stack_size = g_shader_program_stack.size();
#endif

    // Get the image
    getFBO(aFBOID);

    // The filename is not empty
    if (aFileName)
    {
        Image<RATIONAL_NUMBER> temp_image;
        std::string message;
        bool invert_LUT(false);

        switch (aFBOID)
        {
        case XRAY_DETECTOR_UNCLEANED_LBUFFER_FBO_ID:
            m_l_buffer_image = m_temp_raw_data.getRawData();
            temp_image = m_l_buffer_image.flipVertically();
            message = "gVirtualXRay -- L-buffer";
            break;

        case XRAY_DETECTOR_CLEANED_LBUFFER_FBO_ID:
            m_l_buffer_image = m_temp_raw_data.getRawData();
            temp_image = m_l_buffer_image.flipVertically();
            message = "gVirtualXRay -- Cleaned L-buffer";
            break;

        case XRAY_DETECTOR_SUM_MUx_Dx_FBO_ID:
            m_sum_mux_dx_image = m_temp_raw_data.getRawData();
            temp_image = m_sum_mux_dx_image.flipVertically();
            message = "gVirtualXRay -- Sigma(m_x*dx)";
            break;

        case XRAY_DETECTOR_CUMULATED_LBUFFERS_FBO_ID:
            m_cumulated_lbuffer_image = m_temp_raw_data.getRawData();
            temp_image = m_cumulated_lbuffer_image.flipVertically();
            message = "gVirtualXRay -- Cumulative L-buffer";
            break;

        case XRAY_DETECTOR_ENERGY_FLUENCE_FBO_ID:
            m_energy_fluence_image = m_temp_raw_data.getRawData();
            temp_image = m_energy_fluence_image.flipVertically();
            message = "gVirtualXRay -- Energy fluence";
            break;

        default:
            {
                throw NotImplementedException(__FILE__, __FUNCTION__, __LINE__);
            }
            break;
        }

        // Save the image
        temp_image.save(aFileName, invert_LUT, message, useCompression);
    }

#ifndef NDEBUG
    assert(old_FBO_stack_size == g_frame_buffer_object_stack.size());
    assert(old_texture_size == g_texture_stack.size());
    assert(old_enable_disable_stack_size == g_enable_disable_state_stack.size());
    assert(old_shader_stack_size == g_shader_program_stack.size());
#endif
}


//------------------------------------------------------
void XRayRenderer::print2DTexture(GLuint aTextureID,
                                  const char* aFileName,
                                  bool useCompression)
//------------------------------------------------------
{
#ifndef NDEBUG
    size_t old_FBO_stack_size = g_frame_buffer_object_stack.size();
    size_t old_texture_size = g_texture_stack.size();
    size_t old_enable_disable_stack_size = g_enable_disable_state_stack.size();
    size_t old_shader_stack_size = g_shader_program_stack.size();
#endif

    // Get the image
    get2DTexture(aTextureID);

    // The filename is not empty
    if (aFileName)
    {
        Image<RATIONAL_NUMBER> temp_image;
        std::string message;
        bool invert_LUT(false);

        switch (aTextureID)
        {
        case XRAY_DETECTOR_UNCLEANED_LBUFFER_FBO_ID:
            m_l_buffer_image = m_temp_raw_data.getRawData();
            temp_image = m_l_buffer_image.flipVertically();
            message = "gVirtualXRay -- L-buffer";
            break;

        case XRAY_DETECTOR_CLEANED_LBUFFER_FBO_ID:
            m_l_buffer_image = m_temp_raw_data.getRawData();
            temp_image = m_l_buffer_image.flipVertically();
            message = "gVirtualXRay -- Cleaned L-buffer";
            break;

        case XRAY_DETECTOR_SUM_MUx_Dx_FBO_ID:
            m_sum_mux_dx_image = m_temp_raw_data.getRawData();
            temp_image = m_sum_mux_dx_image.flipVertically();
            message = "gVirtualXRay -- Sigma(m_x*dx)";
            break;

        case XRAY_DETECTOR_CUMULATED_LBUFFERS_FBO_ID:
            m_cumulated_lbuffer_image = m_temp_raw_data.getRawData();
            temp_image = m_cumulated_lbuffer_image.flipVertically();
            message = "gVirtualXRay -- Cumulative L-buffer";
            break;

        case XRAY_DETECTOR_ENERGY_FLUENCE_FBO_ID:
            m_energy_fluence_image = m_temp_raw_data.getRawData();
            temp_image = m_energy_fluence_image.flipVertically();
            message = "gVirtualXRay -- Energy fluence";
            break;

        default:
            {
                throw NotImplementedException(__FILE__, __FUNCTION__, __LINE__);
            }
            break;
        }

        // Save the image
        temp_image.save(aFileName, invert_LUT, message, useCompression);
    }

#ifndef NDEBUG
    assert(old_FBO_stack_size == g_frame_buffer_object_stack.size());
    assert(old_texture_size == g_texture_stack.size());
    assert(old_enable_disable_stack_size == g_enable_disable_state_stack.size());
    assert(old_shader_stack_size == g_shader_program_stack.size());
#endif
}


//--------------------------------------------------------
RATIONAL_NUMBER* XRayRenderer::getFBO(unsigned int aFBOID)
//--------------------------------------------------------
{
#ifndef NDEBUG
    size_t old_FBO_stack_size = g_frame_buffer_object_stack.size();
    size_t old_texture_size = g_texture_stack.size();
    size_t old_enable_disable_stack_size = g_enable_disable_state_stack.size();
    size_t old_shader_stack_size = g_shader_program_stack.size();
#endif

    switch (m_computing_mode)
    {
#ifdef HAS_OPENGL
        case OPENGL:
        {
            // The FBO does not exist
            if (aFBOID >= m_p_fbo_set.size())
            {
                throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
            }

            // Save the current FBO
            pushFBO();

            switch (aFBOID)
            {
                case XRAY_DETECTOR_UNCLEANED_LBUFFER_FBO_ID:
                    // Bind frame buffer object
                    m_p_fbo_set[aFBOID]->Bind();

                    // Read data from the FBO
                    glReadPixels(0, 0, m_p_detector->getNumberOfPixels().getX(), m_p_detector->getNumberOfPixels().getY(), GL_RED,  GL_FLOAT,  m_temp_raw_data.getRawData());
                    glReadPixels(0, 0, m_p_detector->getNumberOfPixels().getX(), m_p_detector->getNumberOfPixels().getY(), GL_GREEN, GL_FLOAT, m_temp_raw_data.getRawData() + m_p_detector->getNumberOfPixels().getX() * m_p_detector->getNumberOfPixels().getY());
                    glReadPixels(0, 0, m_p_detector->getNumberOfPixels().getX(), m_p_detector->getNumberOfPixels().getY(), GL_BLUE, GL_FLOAT,  m_temp_raw_data.getRawData() + 2 * m_p_detector->getNumberOfPixels().getX() * m_p_detector->getNumberOfPixels().getY());
                    break;

                case XRAY_DETECTOR_CLEANED_LBUFFER_FBO_ID:
                    // Bind frame buffer object
                    m_p_fbo_set[aFBOID]->Bind();

                    // Read data from the FBO
                    glReadPixels(0, 0, m_p_detector->getNumberOfPixels().getX(), m_p_detector->getNumberOfPixels().getY(), GL_RED,  GL_FLOAT,  m_temp_raw_data.getRawData());
                    // glReadPixels(0, 0, m_p_detector->getNumberOfPixels().getX(), m_p_detector->getNumberOfPixels().getY(), GL_GREEN, GL_FLOAT, m_temp_raw_data.getRawData() + m_p_detector->getNumberOfPixels().getX() * m_p_detector->getNumberOfPixels().getY());
                    // glReadPixels(0, 0, m_p_detector->getNumberOfPixels().getX(), m_p_detector->getNumberOfPixels().getY(), GL_BLUE, GL_FLOAT,  m_temp_raw_data.getRawData() + 2 * m_p_detector->getNumberOfPixels().getX() * m_p_detector->getNumberOfPixels().getY());
                    break;

            case XRAY_DETECTOR_SUM_MUx_Dx_FBO_ID:
                // Read data from the FBO
                for (unsigned int slice_number(0);
                        slice_number < m_p_xray_beam->getEnergyChannelNumber();
                        ++slice_number)
                {
                    // Bind default frame buffer
                    FramebufferObject::Disable();

                    // Attach the texture
                    m_p_fbo_set[XRAY_DETECTOR_SUM_MUx_Dx_FBO_ID]->UnattachAll();
                    m_p_fbo_set[XRAY_DETECTOR_SUM_MUx_Dx_FBO_ID]->AttachTexture(GL_TEXTURE_3D, m_p_texture_name_set[XRAY_DETECTOR_SUM_MUx_Dx_TEXTURE_NAME_ID], GL_COLOR_ATTACHMENT0, 0, slice_number);

                    // Bind the FBO
                    m_p_fbo_set[XRAY_DETECTOR_SUM_MUx_Dx_FBO_ID]->Bind();
                    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

                    glReadPixels(0, 0, m_p_detector->getNumberOfPixels().getX(), m_p_detector->getNumberOfPixels().getY(), GL_RED,  GL_FLOAT, m_temp_raw_data.getRawData() + slice_number * m_p_detector->getNumberOfPixels().getX() * m_p_detector->getNumberOfPixels().getY());
                }
                break;

            case XRAY_DETECTOR_ENERGY_FLUENCE_FBO_ID:
            case XRAY_DETECTOR_CUMULATED_LBUFFERS_FBO_ID:
                // Bind frame buffer object
                m_p_fbo_set[aFBOID]->Bind();

                // Read data from the FBO
                glReadPixels(0, 0, m_p_detector->getNumberOfPixels().getX(), m_p_detector->getNumberOfPixels().getY(), GL_RED,  GL_FLOAT, m_temp_raw_data.getRawData());
                // glReadPixels(0, 0, m_p_detector->getNumberOfPixels().getX(), m_p_detector->getNumberOfPixels().getY(), GL_GREEN, GL_FLOAT, m_temp_raw_data.getRawData() + m_p_detector->getNumberOfPixels().getX() * m_p_detector->getNumberOfPixels().getY());
                // glReadPixels(0, 0, m_p_detector->getNumberOfPixels().getX(), m_p_detector->getNumberOfPixels().getY(), GL_BLUE, GL_FLOAT,  m_temp_raw_data.getRawData() + 2 * m_p_detector->getNumberOfPixels().getX() * m_p_detector->getNumberOfPixels().getY());
                break;

            default:
                {
                    throw NotImplementedException(__FILE__, __FUNCTION__, __LINE__);
                }
                break;
            }

            // Check OpenGL
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            // Restore the current FBO
            popFBO();
        }
        break;
#endif

        case OPENMP:
        {
            m_temp_raw_data.makeBlank();
            switch (aFBOID)
            {
                case XRAY_DETECTOR_UNCLEANED_LBUFFER_FBO_ID:
                case XRAY_DETECTOR_CLEANED_LBUFFER_FBO_ID:
                    // Copy the data
                    memcpy(m_temp_raw_data.getRawData(),
                        m_l_buffer_image.getRawData(),
                        sizeof(PixelType) * m_p_detector->getNumberOfPixels().getX() * m_p_detector->getNumberOfPixels().getY());
                    break;

                case XRAY_DETECTOR_SUM_MUx_Dx_FBO_ID:
                    // Copy the data
                    memcpy(m_temp_raw_data.getRawData(),
                        m_sum_mux_dx_image.getRawData(),
                        sizeof(PixelType) * m_p_detector->getNumberOfPixels().getX() * m_p_detector->getNumberOfPixels().getY() * m_p_xray_beam->getEnergyChannelNumber());
                    break;

                case XRAY_DETECTOR_ENERGY_FLUENCE_FBO_ID:
                    // Copy the data
                    memcpy(m_temp_raw_data.getRawData(),
                        m_energy_fluence_image.getRawData(),
                        sizeof(PixelType) * m_p_detector->getNumberOfPixels().getX() * m_p_detector->getNumberOfPixels().getY());
                    break;

                case XRAY_DETECTOR_CUMULATED_LBUFFERS_FBO_ID:
                    // Copy the data
                    memcpy(m_temp_raw_data.getRawData(),
                        m_cumulated_lbuffer_image.getRawData(),
                        sizeof(PixelType) * m_p_detector->getNumberOfPixels().getX() * m_p_detector->getNumberOfPixels().getY());
                    break;

                default:
                    {
                        throw NotImplementedException(__FILE__, __FUNCTION__, __LINE__);
                    }
                    break;
            }
        }
        break;

    default:
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Unsupported computing mode.");
        break;
    }

#ifndef NDEBUG
    assert(old_FBO_stack_size == g_frame_buffer_object_stack.size());
    assert(old_texture_size == g_texture_stack.size());
    assert(old_enable_disable_stack_size == g_enable_disable_state_stack.size());
    assert(old_shader_stack_size == g_shader_program_stack.size());
#endif

    return (m_temp_raw_data.getRawData());
}


//--------------------------------------------
void XRayRenderer::getFBO(unsigned int aFBOID,
                         float* apImage,
                         unsigned int aWidth,
                         unsigned int aHeight)
//--------------------------------------------
{
#ifndef NDEBUG
    size_t old_FBO_stack_size = g_frame_buffer_object_stack.size();
    size_t old_texture_size = g_texture_stack.size();
    size_t old_enable_disable_stack_size = g_enable_disable_state_stack.size();
    size_t old_shader_stack_size = g_shader_program_stack.size();
#endif

    // The FBO does not exist
    if (aFBOID >= m_p_fbo_set.size())
    {
        throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    if (aWidth != m_p_detector->getNumberOfPixels().getX() || aHeight != m_p_detector->getNumberOfPixels().getY())
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Array size mismatched");
    }

    if (aFBOID == XRAY_DETECTOR_SUM_MUx_Dx_FBO_ID)
    {
        throw NotImplementedException(__FILE__, __FUNCTION__, __LINE__);
    }

    switch (m_computing_mode)
    {
#ifdef HAS_OPENGL
        case OPENGL:
        // Save the current FBO
        pushFBO();

        m_p_fbo_set[aFBOID]->Bind();
        glReadPixels(0, 0, m_p_detector->getNumberOfPixels().getX(), m_p_detector->getNumberOfPixels().getY(), GL_RED, GL_FLOAT, apImage);

        // Check OpenGL
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // Restore the current FBO
        popFBO();
        break;
#endif

    case OPENMP:
        switch (aFBOID)
        {
        case XRAY_DETECTOR_UNCLEANED_LBUFFER_FBO_ID:
        case XRAY_DETECTOR_CLEANED_LBUFFER_FBO_ID:
            // Copy the data
            std::copy(m_l_buffer_image.getRawData(),
                m_l_buffer_image.getRawData() + aWidth * aHeight,
                apImage);
            break;

        case XRAY_DETECTOR_ENERGY_FLUENCE_FBO_ID:
            // Copy the data
            std::copy(m_energy_fluence_image.getRawData(),
                m_energy_fluence_image.getRawData() + aWidth * aHeight,
                apImage);
            break;

        case XRAY_DETECTOR_CUMULATED_LBUFFERS_FBO_ID:
            // Copy the data
            std::copy(m_cumulated_lbuffer_image.getRawData(),
                m_cumulated_lbuffer_image.getRawData() + aWidth * aHeight,
                apImage);
            break;

        default:
            throw NotImplementedException(__FILE__, __FUNCTION__, __LINE__);
            break;
        }
        break;

    default:
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Unsupported computing mode.");
        break;
    }

#ifndef NDEBUG
    assert(old_FBO_stack_size == g_frame_buffer_object_stack.size());
    assert(old_texture_size == g_texture_stack.size());
    assert(old_enable_disable_stack_size == g_enable_disable_state_stack.size());
    assert(old_shader_stack_size == g_shader_program_stack.size());
#endif
}


//--------------------------------------------
void XRayRenderer::getFBO(unsigned int aFBOID,
                         double* apImage,
                         unsigned int aWidth,
                         unsigned int aHeight)
//--------------------------------------------
{
#ifndef NDEBUG
    size_t old_FBO_stack_size = g_frame_buffer_object_stack.size();
    size_t old_texture_size = g_texture_stack.size();
    size_t old_enable_disable_stack_size = g_enable_disable_state_stack.size();
    size_t old_shader_stack_size = g_shader_program_stack.size();
#endif

    // The FBO does not exist
    if (aFBOID >= m_p_fbo_set.size())
    {
        throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    if (aWidth != m_p_detector->getNumberOfPixels().getX() || aHeight != m_p_detector->getNumberOfPixels().getY())
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Array size mismatched");
    }

    if (aFBOID == XRAY_DETECTOR_SUM_MUx_Dx_FBO_ID)
    {
        throw NotImplementedException(__FILE__, __FUNCTION__, __LINE__);
    }

    switch (m_computing_mode)
    {
#ifdef HAS_OPENGL
    case OPENGL:
        {
        // Create a temporay buffer
        std::vector<float> p_temp(m_p_detector->getNumberOfPixels().getX() * m_p_detector->getNumberOfPixels().getY());

        // Save the current FBO
        pushFBO();

        m_p_fbo_set[aFBOID]->Bind();
        glReadPixels(0, 0, m_p_detector->getNumberOfPixels().getX(), m_p_detector->getNumberOfPixels().getY(),
            GL_RED, GL_FLOAT, p_temp.data());

        // Check OpenGL
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // Restore the current FBO
        popFBO();

        // Copy the data from float to double
        std::copy(p_temp.begin(), p_temp.end(), apImage);
        }
        break;
#endif

    case OPENMP:
        switch (aFBOID)
        {
        case XRAY_DETECTOR_UNCLEANED_LBUFFER_FBO_ID:
        case XRAY_DETECTOR_CLEANED_LBUFFER_FBO_ID:
            // Copy the data
            std::copy(m_l_buffer_image.getRawData(),
                m_l_buffer_image.getRawData() + aWidth * aHeight,
                apImage);
            break;

        case XRAY_DETECTOR_ENERGY_FLUENCE_FBO_ID:
            // Copy the data
            std::copy(m_energy_fluence_image.getRawData(),
                m_energy_fluence_image.getRawData() + aWidth * aHeight,
                apImage);
            break;

        case XRAY_DETECTOR_CUMULATED_LBUFFERS_FBO_ID:
            // Copy the data
            std::copy(m_cumulated_lbuffer_image.getRawData(),
                m_cumulated_lbuffer_image.getRawData() + aWidth * aHeight,
                apImage);
            break;

        default:
            throw NotImplementedException(__FILE__, __FUNCTION__, __LINE__);
            break;
        }
        break;

    default:
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Unsupported computing mode.");
        break;
    }

#ifndef NDEBUG
    assert(old_FBO_stack_size == g_frame_buffer_object_stack.size());
    assert(old_texture_size == g_texture_stack.size());
    assert(old_enable_disable_stack_size == g_enable_disable_state_stack.size());
    assert(old_shader_stack_size == g_shader_program_stack.size());
#endif
}


//------------------------------------------------
GLuint XRayRenderer::getFboId(unsigned int aFBOID)
//------------------------------------------------
{
        // The FBO does not exist
        if (aFBOID >= m_p_fbo_set.size())
        {
            throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
        }

        return m_p_fbo_set[aFBOID]->getID();
}


//--------------------------------------------------------
GLuint XRayRenderer::getTextureId(unsigned int aTextureID)
//--------------------------------------------------------
{
        // The FBO does not exist
        if (aTextureID >= m_p_texture_name_set.size())
        {
            throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
        }

        return m_p_texture_name_set[aTextureID];
}


//------------------------------------------------------------------
RATIONAL_NUMBER* XRayRenderer::get2DTexture(unsigned int aTextureID)
//------------------------------------------------------------------
{
#ifndef NDEBUG
    size_t old_FBO_stack_size = g_frame_buffer_object_stack.size();
    size_t old_texture_size = g_texture_stack.size();
    size_t old_enable_disable_stack_size = g_enable_disable_state_stack.size();
    size_t old_shader_stack_size = g_shader_program_stack.size();
#endif

    // The texture does not exist
    if (aTextureID >= m_p_texture_name_set.size())
    {
        throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    // Store the texturing flags
    pushTexture2D();
    pushTexture3D();

#ifdef HAS_OPENGL
    GLint texture_id(m_p_texture_name_set[aTextureID]);
    switch (aTextureID)
    {
    case XRAY_DETECTOR_UNCLEANED_LBUFFER_TEXTURE_NAME_ID:
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, m_temp_raw_data.getRawData());
        glGetTexImage(GL_TEXTURE_2D, 0, GL_GREEN, GL_FLOAT, m_temp_raw_data.getRawData() + m_p_detector->getNumberOfPixels().getX() * m_p_detector->getNumberOfPixels().getY());
        glGetTexImage(GL_TEXTURE_2D, 0, GL_BLUE, GL_FLOAT, m_temp_raw_data.getRawData() + 2 * m_p_detector->getNumberOfPixels().getX() * m_p_detector->getNumberOfPixels().getY());
        break;

    case XRAY_DETECTOR_CLEANED_LBUFFER_TEXTURE_NAME_ID:
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, m_temp_raw_data.getRawData());
        glGetTexImage(GL_TEXTURE_2D, 0, GL_GREEN, GL_FLOAT, m_temp_raw_data.getRawData() + m_p_detector->getNumberOfPixels().getX() * m_p_detector->getNumberOfPixels().getY());
        glGetTexImage(GL_TEXTURE_2D, 0, GL_BLUE, GL_FLOAT, m_temp_raw_data.getRawData() + 2 * m_p_detector->getNumberOfPixels().getX() * m_p_detector->getNumberOfPixels().getY());
        break;

    case XRAY_DETECTOR_SUM_MUx_Dx_TEXTURE_NAME_ID:
        glBindTexture(GL_TEXTURE_3D, texture_id);
        glGetTexImage(GL_TEXTURE_3D, 0, GL_RED, GL_FLOAT, m_temp_raw_data.getRawData());
        break;

    case XRAY_DETECTOR_ENERGY_FLUENCE_TEXTURE_NAME_ID:
    case XRAY_DETECTOR_CUMULATED_LBUFFERS_TEXTURE_NAME_ID:
    case XRAY_DETECTOR_ZBUFFER_TEXTURE_NAME_ID:
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, m_temp_raw_data.getRawData());
        break;

    default:
        {
            throw NotImplementedException(__FILE__, __FUNCTION__, __LINE__);
        }
        break;
    }
#endif

    // Restore the texturing flags
    popTexture();
    popTexture();

    if (aTextureID == XRAY_DETECTOR_ENERGY_FLUENCE_TEXTURE_NAME_ID)
    {
        if (m_p_xray_beam->usePoissonNoise())
        {
            addPoissonNoise(m_temp_raw_data);
        }
    }

#ifndef NDEBUG
    assert(old_FBO_stack_size == g_frame_buffer_object_stack.size());
    assert(old_texture_size == g_texture_stack.size());
    assert(old_enable_disable_stack_size == g_enable_disable_state_stack.size());
    assert(old_shader_stack_size == g_shader_program_stack.size());
#endif

    return (m_temp_raw_data.getRawData());
}


//----------------------------------------------------------
void XRayRenderer::display2DTexture(unsigned int aTexutreID)
//----------------------------------------------------------
{
#ifndef NDEBUG
    size_t old_FBO_stack_size = g_frame_buffer_object_stack.size();
    size_t old_texture_size = g_texture_stack.size();
    size_t old_enable_disable_stack_size = g_enable_disable_state_stack.size();
    size_t old_shader_stack_size = g_shader_program_stack.size();
#endif

    // Store the texturing flags
    pushTexture2D();

    // Bind the texture
#ifdef HAS_OPENGL
    glBindTexture(GL_TEXTURE_2D, m_p_texture_name_set[aTexutreID]);
#endif

    // Display the detector
    m_p_detector->displayDetector();

    // Store the texturing flags
    popTexture();

    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

#ifndef NDEBUG
    assert(old_FBO_stack_size == g_frame_buffer_object_stack.size());
    assert(old_texture_size == g_texture_stack.size());
    assert(old_enable_disable_stack_size == g_enable_disable_state_stack.size());
    assert(old_shader_stack_size == g_shader_program_stack.size());
#endif
}

//-----------------------------------------------
unsigned int XRayRenderer::getID2DTexture() const
//-----------------------------------------------
{
    return m_p_texture_name_set[XRAY_DETECTOR_ENERGY_FLUENCE_TEXTURE_NAME_ID];
}


//------------------------------------------------------------------------------
void XRayRenderer::computeLBuffer(SceneGraphNode& anObject,
                                  const VEC3& aXRaySourceDetectorTranslation,
                                  const MATRIX4& aModellingTransformationMatrix,
                                  bool aGPUStateConservationFlag)
//------------------------------------------------------------------------------
{
#ifndef NDEBUG
    size_t old_FBO_stack_size = g_frame_buffer_object_stack.size();
    size_t old_texture_size = g_texture_stack.size();
    size_t old_enable_disable_stack_size = g_enable_disable_state_stack.size();
    size_t old_shader_stack_size = g_shader_program_stack.size();
#endif

    computeLBuffer(anObject.getPolygonMesh(),
            aXRaySourceDetectorTranslation,
            aModellingTransformationMatrix * anObject.getWorldTransformationMatrix(),
            aGPUStateConservationFlag);

#ifndef NDEBUG
    assert(old_FBO_stack_size == g_frame_buffer_object_stack.size());
    assert(old_texture_size == g_texture_stack.size());
    assert(old_enable_disable_stack_size == g_enable_disable_state_stack.size());
    assert(old_shader_stack_size == g_shader_program_stack.size());
#endif
}


//------------------------------------------------------------------------------
void XRayRenderer::computeLBuffer(PolygonMesh& anObject,
                                  const VEC3& aXRaySourceDetectorTranslation,
                                  const MATRIX4& aModellingTransformationMatrix,
                                  bool aGPUStateConservationFlag)
//------------------------------------------------------------------------------
{
#ifndef NDEBUG
    size_t old_FBO_stack_size = g_frame_buffer_object_stack.size();
    size_t old_texture_size = g_texture_stack.size();
    size_t old_enable_disable_stack_size = g_enable_disable_state_stack.size();
    size_t old_shader_stack_size = g_shader_program_stack.size();
#endif

    bool internal_mesh = true;

    if (m_p_outer_surface)
    {
        if (&anObject == &m_p_outer_surface->getPolygonMesh())
        {
            internal_mesh = false;
        }
    }

    // This is an internal mesh
    if (internal_mesh)
    {
        computeInnerSurfaceLBuffer(anObject,
                aXRaySourceDetectorTranslation,
                aModellingTransformationMatrix,
                aGPUStateConservationFlag);
    }
    // This is the external mesh
    else
    {
        computeOuterSurfaceLBuffer(anObject,
                aXRaySourceDetectorTranslation,
                aModellingTransformationMatrix);
    }

#ifndef NDEBUG
    assert(old_FBO_stack_size == g_frame_buffer_object_stack.size());
    assert(old_texture_size == g_texture_stack.size());
    assert(old_enable_disable_stack_size == g_enable_disable_state_stack.size());
    assert(old_shader_stack_size == g_shader_program_stack.size());
#endif
}


//------------------------------------------------------------------------------------------
void XRayRenderer::computeInnerSurfaceLBuffer(PolygonMesh& anObject,
                                              const VEC3& aXRaySourceSampleOffset,
                                              const MATRIX4& aModellingTransformationMatrix,
                                              bool aGPUStateConservationFlag)
//------------------------------------------------------------------------------------------
{
#ifndef NDEBUG
    size_t old_FBO_stack_size = g_frame_buffer_object_stack.size();
    size_t old_texture_size = g_texture_stack.size();
    size_t old_enable_disable_stack_size = g_enable_disable_state_stack.size();
    size_t old_shader_stack_size = g_shader_program_stack.size();
#endif

    switch (m_computing_mode)
    {
#ifdef HAS_OPENGL
        case OPENGL:
        {
            // Store GPU states
            GLint p_viewport[4];
            if (aGPUStateConservationFlag)
                {
                preComputeLBuffer(p_viewport, aXRaySourceSampleOffset);
                }

            // The 3D object is not empty
            if (anObject.getVertexNumber())
            {
                // Load uniform variables to the GPU
                GLint program_handle(0);
                glGetIntegerv(GL_CURRENT_PROGRAM, &program_handle);
                checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

                GLint handle = glGetUniformLocation(program_handle, "g_modelling_matrix");
                glUniformMatrix4fv(handle, 1, GL_FALSE, aModellingTransformationMatrix.get());
                checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

                MATRIX4 modelview_matrix(aModellingTransformationMatrix);

                handle = glGetUniformLocation(program_handle, "g_normal_matrix");
                glUniformMatrix3fv(handle, 1, GL_FALSE, modelview_matrix.get3x3());
                checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

                // Display the object
                anObject.display();
                checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
            }

            if (m_use_l_buffer_artefact_filtering_on_cpu)
            {
                // Get the L-buffer
                get2DTexture(XRAY_DETECTOR_UNCLEANED_LBUFFER_TEXTURE_NAME_ID);

                const VEC3 detector_up_vector    = m_p_detector->getRotationMatrix() * m_p_detector->getUpVector();
                const VEC3 detector_centre       = m_p_detector->getRotationMatrix() * m_p_detector->getDetectorPosition();
                const VEC3 detector_right_vector =  m_p_detector->getRotationMatrix() * m_p_detector->getRightVector();

                const int width  = m_temp_raw_data.getWidth();
                const int height = m_temp_raw_data.getHeight();

                const double half_width_in_px  = m_p_detector->getSizeInUnitOfLength()[0] / 2.0;
                const double half_height_in_px = m_p_detector->getSizeInUnitOfLength()[1] / 2.0;

                std::vector<float> p_temp(m_temp_raw_data.getWidth() * m_temp_raw_data.getHeight());

// #ifdef NDEBUG
#ifdef WIN32
                #pragma omp parallel for
#else
                #pragma omp parallel for collapse(2)
#endif
// #endif
                for (int j = 0; j < height; ++j)
                {
                    for (int i = 0; i < width; ++i)
                    {
                        std::map<RATIONAL_NUMBER, VEC3> p_intersection_set;

                        VEC3 ray_origin;
                        VEC3 ray_direction;

                        double l_buffer_value = 0.0;

                        PixelType sign_accumulator = m_temp_raw_data.getPixel(i, j, 1);
                        if (sign_accumulator < -0.5 || sign_accumulator > 0.5)
                        {
                            double u = -half_width_in_px  + m_p_detector->getPixelSizeInUnitOfLength()[0] * (0.5 + i);
                            double v = -half_height_in_px + m_p_detector->getPixelSizeInUnitOfLength()[1] * (0.5 + j);

                            VEC3 pixel_position = detector_centre + detector_up_vector * v + detector_right_vector * u;
                            if (!m_p_detector->useParallelBeam())
                            {
                                ray_origin = m_p_detector->getXraySourceCentre() + aXRaySourceSampleOffset;
                                ray_direction = (pixel_position - ray_origin).normal();
                            }
                            else
                            {
                                // Cast the ray from the pixel position, not the source
                                ray_direction =  -(m_p_detector->getDetectorPosition() - m_p_detector->getXraySourceCentre()).normal();
                                ray_origin = pixel_position;
                            }

                            p_intersection_set = anObject.rayIntersect(
                                    ray_origin,
                                    ray_direction,
                                    aModellingTransformationMatrix);

                            // There is an even number of intersections
                            if (p_intersection_set.size() && !(p_intersection_set.size() % 2))
                            {
                                for (std::map<RATIONAL_NUMBER, VEC3>::const_iterator ite = p_intersection_set.begin();
                                        ite != p_intersection_set.end();
                                        ++ite)
                                {
                                    l_buffer_value += ite->first * sgn(ite->second.dotProduct(ray_direction)) / cm;
                                }
                            }
                        }
                        else
                        {
                            l_buffer_value = m_temp_raw_data.getPixel(i, j, 0);
                        }

                        p_temp[i + j * m_temp_raw_data.getHeight()] = l_buffer_value;
                    }
                }

                //if (has_worked)
                {
                    // Store the texturing flags
                    pushTexture2D();

                    glBindTexture(GL_TEXTURE_2D, m_p_texture_name_set[XRAY_DETECTOR_CLEANED_LBUFFER_TEXTURE_NAME_ID]);

                    // Use FP32
                    if (getPrecisionCleanedLBuffer(m_mixed_precision_flag))
                    {
                        glTexImage2D(GL_TEXTURE_2D,
                                0,
                                GL_R32F,
                                m_temp_raw_data.getWidth(),
                                m_temp_raw_data.getHeight(),
                                0,
                                GL_RED,
                                GL_FLOAT,
                                p_temp.data());
                    }
                    // Use FP16
                    else
                    {
                        glTexImage2D(GL_TEXTURE_2D,
                                0,
                                GL_R16F,
                                m_temp_raw_data.getWidth(),
                                m_temp_raw_data.getHeight(),
                                0,
                                GL_RED,
                                GL_FLOAT,
                                p_temp.data());
                    }
                    // Restore the texturing flags
                    popTexture();
                }
            }

            // Restore GPU states
            if (aGPUStateConservationFlag)
                {

                RATIONAL_NUMBER bbox_diagonal((anObject.getLocalMaxCorner() - anObject.getLocalMinCorner()).length() / cm);
                postComputeLBuffer(p_viewport, bbox_diagonal);
                }

            // The L-buffer is not empty
            m_l_buffer_empty = false;
        }
        break;
#endif

    case OPENMP:
        {
            const VEC3 detector_up_vector    = m_p_detector->getRotationMatrix() * m_p_detector->getUpVector();
            const VEC3 detector_centre       = m_p_detector->getRotationMatrix() * m_p_detector->getDetectorPosition();
            const VEC3 detector_right_vector =  m_p_detector->getRotationMatrix() * m_p_detector->getRightVector();

            const unsigned int width  = m_temp_raw_data.getWidth();
            const unsigned int height = m_temp_raw_data.getHeight();

            const double half_width_in_px  = m_p_detector->getSizeInUnitOfLength()[0] / 2.0;
            const double half_height_in_px = m_p_detector->getSizeInUnitOfLength()[1] / 2.0;

// #ifdef NDEBUG
#ifdef WIN32
            #pragma omp parallel for
#else
            #pragma omp parallel for collapse(2)
#endif
// #endif
            for (int j = 0; j < int(height); ++j)
            {
                for (int i = 0; i < int(width); ++i)
                {
                    std::map<RATIONAL_NUMBER, VEC3> p_intersection_set;

                    VEC3 ray_origin;
                    VEC3 ray_direction;

                    double l_buffer_value = 0.0;

                    double u = -half_width_in_px  + m_p_detector->getPixelSizeInUnitOfLength()[0] * (0.5 + i);
                    double v = -half_height_in_px + m_p_detector->getPixelSizeInUnitOfLength()[1] * (0.5 + j);

                    VEC3 pixel_position = detector_centre + detector_up_vector * v + detector_right_vector * u;
                    if (!m_p_detector->useParallelBeam())
                    {
                        ray_origin = m_p_detector->getXraySourceCentre() + aXRaySourceSampleOffset;
                        ray_direction = (pixel_position - ray_origin).normal();
                    }
                    else
                    {
                        // Cast the ray from the pixel position, not the source
                        ray_direction =  -(m_p_detector->getDetectorPosition() - m_p_detector->getXraySourceCentre()).normal();
                        ray_origin = pixel_position;
                    }

                    p_intersection_set = anObject.rayIntersect(
                            ray_origin,
                            ray_direction,
                            aModellingTransformationMatrix);

                    // There is an even number of intersections
                    if (p_intersection_set.size() && !(p_intersection_set.size() % 2))
                    {
                        for (std::map<RATIONAL_NUMBER, VEC3>::const_iterator ite = p_intersection_set.begin();
                                ite != p_intersection_set.end();
                                ++ite)
                        {
                            l_buffer_value += ite->first * sgn(ite->second.dotProduct(ray_direction)) / cm;
                        }
                    }

                    m_l_buffer_image(i, j, 0) = l_buffer_value;
                }
            }

        }
        break;

    default:
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Unsupported computing mode.");
        break;
    }

#ifndef NDEBUG
    assert(old_FBO_stack_size == g_frame_buffer_object_stack.size());
    assert(old_texture_size == g_texture_stack.size());
    assert(old_enable_disable_stack_size == g_enable_disable_state_stack.size());
    assert(old_shader_stack_size == g_shader_program_stack.size());
#endif
}


//------------------------------------------------------------------------------------
void XRayRenderer::computeOuterSurfaceLBuffer(PolygonMesh& anObject,
                                              const VEC3& aXRaySourceSampleOffset,
                                              const MATRIX4& aModellingTransformationMatrix)
//------------------------------------------------------------------------------------
{
#ifndef NDEBUG
    size_t old_FBO_stack_size = g_frame_buffer_object_stack.size();
    size_t old_texture_size = g_texture_stack.size();
    size_t old_enable_disable_stack_size = g_enable_disable_state_stack.size();
    size_t old_shader_stack_size = g_shader_program_stack.size();
#endif

    switch (m_computing_mode)
    {
#ifdef HAS_OPENGL
        case OPENGL:
        {
            // Save the viwport
            GLint p_viewport[4];
            glGetIntegerv(GL_VIEWPORT, &p_viewport[0]);

            // Change the viewport
            glViewport(0, 0, m_p_detector->getNumberOfPixels().getX(), m_p_detector->getNumberOfPixels().getY());

            // Reset its L-buffer
            resetLBuffer();

            // Compute the L-buffer of the external surface
            computeInnerSurfaceLBuffer(anObject, aXRaySourceSampleOffset, aModellingTransformationMatrix, true);

            // Substract the L-buffer of the internal surfaces
            // Save the current FBO
            pushFBO();

            // Store the texturing flags
            pushTexture2D();

            // Store the state
            pushEnableDisableState(GL_BLEND);

            // Bind frame buffer object
            if (m_use_l_buffer_artefact_filtering_on_gpu || m_use_l_buffer_artefact_filtering_on_cpu)
            {
                m_p_fbo_set[XRAY_DETECTOR_CLEANED_LBUFFER_FBO_ID]->Bind();
            }
            else
            {
                m_p_fbo_set[XRAY_DETECTOR_UNCLEANED_LBUFFER_FBO_ID]->Bind();
            }
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            // Store the transformation matrices
            pushProjectionMatrix();
            pushModelViewMatrix();

            // Set the projection matrix
            loadIdentityProjectionMatrix();
            loadOrthoProjectionMatrix(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);

            loadIdentityModelViewMatrix();

            // Enable the shader
            pushShaderProgram();
            m_shader_set[XRAY_DETECTOR_OUTER_SURFACE_LBUFFER_SHADER_ID].enable();
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            // Values to pass to the shader
            GLint program_handle(0);
            glGetIntegerv(GL_CURRENT_PROGRAM, &program_handle);

            GLint saved_texture;
            glGetIntegerv(GL_ACTIVE_TEXTURE, &saved_texture);

            GLint handle(glGetUniformLocation(program_handle, "g_cumulated_l_buffer_texture"));
            glUniform1i(handle, saved_texture - GL_TEXTURE0);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            handle = glGetUniformLocation(program_handle, "g_x_offset");
            glUniform1f(handle, 1.0 / m_p_detector->getNumberOfPixels().getX());
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            handle = glGetUniformLocation(program_handle, "g_y_offset");
            glUniform1f(handle, 1.0 / m_p_detector->getNumberOfPixels().getY());
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            handle = glGetUniformLocation(program_handle, "g_projection_matrix");
            glUniformMatrix4fv(handle, 1, GL_FALSE, g_current_projection_matrix.get());
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            handle = glGetUniformLocation(program_handle, "g_modelview_matrix");
            glUniformMatrix4fv(handle, 1, GL_FALSE, g_current_modelview_matrix.get());
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            glBindTexture(GL_TEXTURE_2D, m_p_texture_name_set[XRAY_DETECTOR_CUMULATED_LBUFFERS_TEXTURE_NAME_ID]);

            // Enable blending
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE);

            // Display the textured square
            m_detector_temporary_geometry->display();
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            // Restore the transformation matrices
            popProjectionMatrix();
            popModelViewMatrix();

            // Restore the FBO
            popFBO();

            // Store the state
            popEnableDisableState();

            // Restore the texturing flags
            popTexture();

            // Restore OpenGL states
            glViewport(p_viewport[0], p_viewport[1], p_viewport[2], p_viewport[3]);

            // Restore the shader program
            popShaderProgram();
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            // Wait for all OpenGL execution to be completed
            glFinish();
        }
        break;
#endif

        case OPENMP:
        {
            // Reset its L-buffer
            resetLBuffer();

            // Compute the L-buffer of the external surface
            computeInnerSurfaceLBuffer(anObject, aXRaySourceSampleOffset, aModellingTransformationMatrix, true);

            // Perform the subtraction
            m_l_buffer_image -= m_cumulated_lbuffer_image;
        }
        break;

    default:
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Unsupported computing mode.");
        break;
    }

#ifndef NDEBUG
    assert(old_FBO_stack_size == g_frame_buffer_object_stack.size());
    assert(old_texture_size == g_texture_stack.size());
    assert(old_enable_disable_stack_size == g_enable_disable_state_stack.size());
    assert(old_shader_stack_size == g_shader_program_stack.size());
#endif
}


//-----------------------------------------------------------------
void XRayRenderer::updateCumulatedLBuffers(unsigned int anObjectID)
//-----------------------------------------------------------------
{
#ifndef NDEBUG
    size_t old_FBO_stack_size = g_frame_buffer_object_stack.size();
    size_t old_texture_size = g_texture_stack.size();
    size_t old_enable_disable_stack_size = g_enable_disable_state_stack.size();
    size_t old_shader_stack_size = g_shader_program_stack.size();
#endif

    switch (m_computing_mode)
    {
#ifdef HAS_OPENGL
        case OPENGL:
        {
            // Save the current FBO
            pushFBO();

            // Store the texturing flags
            pushTexture2D();

            // Store the state
            pushEnableDisableState(GL_BLEND);

            // Bind frame buffer object
            m_p_fbo_set[XRAY_DETECTOR_CUMULATED_LBUFFERS_FBO_ID]->Bind();
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            // Store the transformation matrices
            pushProjectionMatrix();
            pushModelViewMatrix();

            // Set the projection matrix
            loadIdentityProjectionMatrix();
            loadOrthoProjectionMatrix(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);

            loadIdentityModelViewMatrix();

            // Enable the shader
            pushShaderProgram();
            m_shader_set[XRAY_DETECTOR_CUMULATED_LBUFFER_SHADER_ID].enable();
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            // Values to pass to the shader
            GLint program_handle(0);
            glGetIntegerv(GL_CURRENT_PROGRAM, &program_handle);

            GLint saved_texture;
            glGetIntegerv(GL_ACTIVE_TEXTURE, &saved_texture);

            GLint handle(glGetUniformLocation(program_handle, "g_l_buffer_texture"));
            glUniform1i(handle, saved_texture - GL_TEXTURE0);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            handle = glGetUniformLocation(program_handle, "g_x_offset");
            glUniform1f(handle, 1.0 / m_p_detector->getNumberOfPixels().getX());
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            handle = glGetUniformLocation(program_handle, "g_y_offset");
            glUniform1f(handle, 1.0 / m_p_detector->getNumberOfPixels().getY());
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            handle = glGetUniformLocation(program_handle, "g_projection_matrix");
            glUniformMatrix4fv(handle, 1, GL_FALSE, g_current_projection_matrix.get());
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            handle = glGetUniformLocation(program_handle, "g_modelview_matrix");
            glUniformMatrix4fv(handle, 1, GL_FALSE, g_current_modelview_matrix.get());
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            if (m_use_l_buffer_artefact_filtering_on_gpu || m_use_l_buffer_artefact_filtering_on_cpu)
            {
                glBindTexture(GL_TEXTURE_2D, m_p_texture_name_set[XRAY_DETECTOR_CLEANED_LBUFFER_TEXTURE_NAME_ID]);
            }
            else
            {
                glBindTexture(GL_TEXTURE_2D, m_p_texture_name_set[XRAY_DETECTOR_UNCLEANED_LBUFFER_TEXTURE_NAME_ID]);
            }
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            // Enable blending
            if (anObjectID != 0)
            {
                glEnable(GL_BLEND);
                glBlendFunc(GL_ONE, GL_ONE);
            }
            // Disable blending
            else
            {
                glDisable(GL_BLEND);
            }

            // Display the textured square
            m_detector_temporary_geometry->display();
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            // Restore the transformation matrices
            popProjectionMatrix();
            popModelViewMatrix();

            // Restore the FBO
            popFBO();

            // Store the state
            popEnableDisableState();

            // Restore the texturing flags
            popTexture();

            // Restore the shader program
            popShaderProgram();
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            // Wait for all OpenGL execution to be completed
            glFinish();
        }
        break;
#endif

        case OPENMP:
        {
#ifdef NDEBUG
#ifdef WIN32
            #pragma omp parallel for
#else
            #pragma omp parallel for collapse(2)
#endif
#endif
            for (int j = 0; j < int(m_p_detector->getNumberOfPixels().getY()); ++j)
                for (int i = 0; i < int(m_p_detector->getNumberOfPixels().getX()); ++i)
                {
                    // Perform the subtraction
                    m_cumulated_lbuffer_image(i, j, 0) += m_l_buffer_image(i, j, 0);
                }
        }
        break;

    default:
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Unsupported computing mode.");
        break;
    }

#ifndef NDEBUG
    assert(old_FBO_stack_size == g_frame_buffer_object_stack.size());
    assert(old_texture_size == g_texture_stack.size());
    assert(old_enable_disable_stack_size == g_enable_disable_state_stack.size());
    assert(old_shader_stack_size == g_shader_program_stack.size());
#endif
}


//-------------------------------------------------------------------------------
void XRayRenderer::computeSumMUxDx(int anObjectID,
                                   const PhotonCrossSection& aPhotonCrossSection)
//-------------------------------------------------------------------------------
{
#ifndef NDEBUG
    size_t old_FBO_stack_size = g_frame_buffer_object_stack.size();
    size_t old_texture_size = g_texture_stack.size();
    size_t old_enable_disable_stack_size = g_enable_disable_state_stack.size();
    size_t old_shader_stack_size = g_shader_program_stack.size();
#endif

    // The number of energy channels has changed since last time
    if (m_sum_mux_dx_image.getDepth() != m_p_xray_beam->getEnergyChannelNumber())
    {
        // Update the image size
        VEC3 spacing = m_sum_mux_dx_image.getSpacing();

        m_sum_mux_dx_image = Image<PixelType>(
                m_p_detector->getNumberOfPixels().getX(),
                m_p_detector->getNumberOfPixels().getY(),
                m_p_xray_beam->getEnergyChannelNumber());

        m_sum_mux_dx_image.setSpacing(spacing);

        unsigned int depth(3);
        if (depth < m_p_xray_beam->getEnergyChannelNumber())
        {
            depth = m_p_xray_beam->getEnergyChannelNumber();
        }
        m_temp_raw_data = Image<PixelType>(
                m_p_detector->getNumberOfPixels().getX(),
                m_p_detector->getNumberOfPixels().getY(),
                depth);

        init3DData();
        init3DFBOs();
    }

    // Save the current FBO
    pushFBO();

    // Store the texturing flags
    pushTexture2D();

    // Store the state
    pushEnableDisableState(GL_BLEND);

    switch (m_computing_mode)
    {
#ifdef HAS_OPENGL
    case OPENGL:
        {
            // Bind frame buffer object
            m_p_fbo_set[XRAY_DETECTOR_SUM_MUx_Dx_FBO_ID]->Bind();
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            // Store the transformation matrices
            pushProjectionMatrix();
            pushModelViewMatrix();

            // Display a textured square
            loadIdentityProjectionMatrix();
            loadOrthoProjectionMatrix(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);

            loadIdentityModelViewMatrix();

            // Enable the shader
            pushShaderProgram();
            m_shader_set[XRAY_DETECTOR_SUM_MUx_Dx_SHADER_ID].enable();
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            // Values to pass to the shader
            GLint program_handle(0);
            glGetIntegerv(GL_CURRENT_PROGRAM, &program_handle);

            GLint saved_texture;
            glGetIntegerv(GL_ACTIVE_TEXTURE, &saved_texture);

            GLint handle(glGetUniformLocation(program_handle, "g_l_buffer_texture"));
            glUniform1i(handle, saved_texture - GL_TEXTURE0);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            handle = glGetUniformLocation(program_handle, "g_x_offset");
            glUniform1f(handle, 1.0 / m_p_detector->getNumberOfPixels().getX());
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            handle = glGetUniformLocation(program_handle, "g_y_offset");
            glUniform1f(handle, 1.0 / m_p_detector->getNumberOfPixels().getY());
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            handle = glGetUniformLocation(program_handle, "g_projection_matrix");
            glUniformMatrix4fv(handle, 1, GL_FALSE, g_current_projection_matrix.get());
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            handle = glGetUniformLocation(program_handle, "g_modelview_matrix");
            glUniformMatrix4fv(handle, 1, GL_FALSE, g_current_modelview_matrix.get());
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            // Rebind textures
            // Cumulated L-buffers
            glActiveTexture(saved_texture);

            if (m_use_l_buffer_artefact_filtering_on_gpu || m_use_l_buffer_artefact_filtering_on_cpu)
            {
                glBindTexture(GL_TEXTURE_2D, m_p_texture_name_set[XRAY_DETECTOR_CLEANED_LBUFFER_TEXTURE_NAME_ID]);
            }
            else
            {
                glBindTexture(GL_TEXTURE_2D, m_p_texture_name_set[XRAY_DETECTOR_UNCLEANED_LBUFFER_TEXTURE_NAME_ID]);
            }
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            // This is not the first object, enable blending
            if (anObjectID > 0)
            {
                glEnable(GL_BLEND);
                glBlendFunc(GL_ONE, GL_ONE);
            }
            // This is the first object, disable blending if needed
            else
            {
                // Blending is enable, disable it
                glDisable(GL_BLEND);
            }

            // Proceed each energy channel
            for (unsigned int current_slice(0);
                    current_slice < m_p_xray_beam->getEnergyChannelNumber();
                    ++current_slice)
            {
                // Get the energy and the corresponding attenuation coefficient
                RATIONAL_NUMBER energy = m_p_xray_beam->getEnergyChannelPerPixelAtSDD(current_slice, m_p_detector, true).getPhotonEnergy();
                RATIONAL_NUMBER mu = aPhotonCrossSection.getLinearAttenuationCoefficient(energy);

                // Set the attenuation coefficient
                // Convert mu in cm-1
                mu /= 1.0 / cm; // 1/cm because the unit of mu is cm-1

                // Bind default frame buffer
                FramebufferObject::Disable();

                // Attach the texture
                m_p_fbo_set[XRAY_DETECTOR_SUM_MUx_Dx_FBO_ID]->UnattachAll();
                m_p_fbo_set[XRAY_DETECTOR_SUM_MUx_Dx_FBO_ID]->AttachTexture(GL_TEXTURE_3D, m_p_texture_name_set[XRAY_DETECTOR_SUM_MUx_Dx_TEXTURE_NAME_ID], GL_COLOR_ATTACHMENT0, 0, current_slice);

                // Bind the FBO
                m_p_fbo_set[XRAY_DETECTOR_SUM_MUx_Dx_FBO_ID]->Bind();
                checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

                handle = glGetUniformLocation(program_handle, "g_attenuation_coefficient");
                glUniform1f(handle, mu);
                checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

                // Display the textured square
                m_detector_temporary_geometry->display();
                checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
            }

            // Restore the transformation matrices
            popProjectionMatrix();
            popModelViewMatrix();

            // Restore the shader program
            popShaderProgram();
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            // Wait for all OpenGL execution to be completed
            glFinish();
        }
        break;
#endif

    case CUDA:
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "CUDA is not supported.");
        break;

    case OPENCL:
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "OpenCL is not supported.");
        break;

    case OPENMP:
        {
            // Proceed each energy channel
            for (unsigned int current_slice(0);
                    current_slice < m_p_xray_beam->getEnergyChannelNumber();
                    ++current_slice)
            {
                // Get the energy and the corresponding attenuation coefficient
                RATIONAL_NUMBER energy = m_p_xray_beam->getEnergyChannelPerPixelAtSDD(current_slice, m_p_detector, true).getPhotonEnergy();
                RATIONAL_NUMBER mu = aPhotonCrossSection.getLinearAttenuationCoefficient(energy);

                // Set the attenuation coefficient
                // Convert mu in cm-1
                mu /= 1.0 / cm; // 1/cm because the unit of mu is cm-1

#ifdef NDEBUG
#ifdef WIN32
                #pragma omp parallel for
#else
                #pragma omp parallel for collapse(2)
#endif
#endif
                for (int j = 0; j < int(m_p_detector->getNumberOfPixels().getY()); ++j)
                    for (int i = 0; i < int(m_p_detector->getNumberOfPixels().getX()); ++i)
                    {
                        // This is not the first object, enable blending
                        if (anObjectID > 0)
                            m_sum_mux_dx_image(i, j, current_slice) += m_l_buffer_image(i, j, 0) * mu;
                        else
                            m_sum_mux_dx_image(i, j, current_slice) = m_l_buffer_image(i, j, 0) * mu;
                    }

            }
        }
        break;

    default:
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Unsupported computing mode.");
        break;
    }

    // Restore the FBO
    popFBO();

    // Restore the texturing flags
    popTexture();

    // Restore the state
    popEnableDisableState();

#ifndef NDEBUG
    assert(old_FBO_stack_size == g_frame_buffer_object_stack.size());
    assert(old_texture_size == g_texture_stack.size());
    assert(old_enable_disable_stack_size == g_enable_disable_state_stack.size());
    assert(old_shader_stack_size == g_shader_program_stack.size());
#endif
}


//--------------------------------------------------------------------
void XRayRenderer::computeIntegratedEnergy(bool anIntegrateEnergyFlag)
//--------------------------------------------------------------------
{
#ifndef NDEBUG
    size_t old_FBO_stack_size = g_frame_buffer_object_stack.size();
    size_t old_texture_size = g_texture_stack.size();
    size_t old_enable_disable_stack_size = g_enable_disable_state_stack.size();
    size_t old_shader_stack_size = g_shader_program_stack.size();
#endif

    // Save the current FBO
    pushFBO();

    // Store the texturing flags
    pushTexture3D();

    // Store the state
    pushEnableDisableState(GL_BLEND);

    switch (m_computing_mode)
    {
#ifdef HAS_OPENGL
    case OPENGL:
        {
            // Disable blending
            if (m_energy_fluence_empty)
            {
                glDisable(GL_BLEND);
            }

            // Bind frame buffer object
            m_p_fbo_set[XRAY_DETECTOR_ENERGY_FLUENCE_FBO_ID]->Bind();
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            // Activate shaders
            pushShaderProgram();
            m_shader_set[XRAY_DETECTOR_ENERGY_FLUENCE_SHADER_ID].enable();
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            // Values to pass to the shader
            GLint program_handle(0);
            glGetIntegerv(GL_CURRENT_PROGRAM, &program_handle);

            GLint saved_texture;
            glGetIntegerv(GL_ACTIVE_TEXTURE, &saved_texture);

            GLint handle(glGetUniformLocation(program_handle, "g_sum_mu_x_dx_texture"));
            glUniform1i(handle, saved_texture - GL_TEXTURE0);

            // Rebind textures
            // Cumulated L-buffers
            glActiveTexture(saved_texture);
            glBindTexture(GL_TEXTURE_3D, m_p_texture_name_set[XRAY_DETECTOR_SUM_MUx_Dx_TEXTURE_NAME_ID]);

            // Store the transformation matrices
            pushProjectionMatrix();
            pushModelViewMatrix();

            // Display a textured square
            loadIdentityProjectionMatrix();
            loadOrthoProjectionMatrix(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);

            loadIdentityModelViewMatrix();

            handle = glGetUniformLocation(program_handle, "g_projection_matrix");
            glUniformMatrix4fv(handle, 1, GL_FALSE, g_current_projection_matrix.get());
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            handle = glGetUniformLocation(program_handle, "g_modelview_matrix");
            glUniformMatrix4fv(handle, 1, GL_FALSE, g_current_modelview_matrix.get());
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            // Proceed each energy channel
            for (unsigned int current_slice(0);
                    current_slice < m_p_xray_beam->getEnergyChannelNumber();
                    ++current_slice)
            {
                if (!m_energy_fluence_empty)
                {
                    // Enable blending
                    glEnable(GL_BLEND);
                    glBlendFunc(GL_ONE, GL_ONE);
                    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
                }

                // Get the incident energy
                SpectrumRecord spectrum_record(m_p_xray_beam->getEnergyChannelPerPixelAtSDD(current_slice, m_p_detector, true));
                RATIONAL_NUMBER incident_energy(spectrum_record.getPhotonEnergy());

                // Apply the energy response
                RATIONAL_NUMBER corrected_energy = m_p_detector->applyEnergyResponse(incident_energy);

                // Convert in MeV
                corrected_energy /= MeV;

                // Compute the number of photons, not the energy fluence
                if (!anIntegrateEnergyFlag)
                {
                        corrected_energy = 1.0;
                }

                // Get the number of photons
                RATIONAL_NUMBER photon_number(spectrum_record.getPhotonNumber());

                // Compute the energy
                RATIONAL_NUMBER total_energy(photon_number * corrected_energy / RATIONAL_NUMBER(m_p_detector->getNumberOfSourceSamples()));

                handle = glGetUniformLocation(program_handle, "g_n_input_energy");
                glUniform1f(handle, total_energy);

                // Transform the texture coordinates
                GLdouble z((0.5 + current_slice) / GLdouble(m_p_xray_beam->getEnergyChannelNumber()));
                MATRIX4 translation_matrix(MATRIX4::buildTranslationMatrix(0, 0, z));
                handle = glGetUniformLocation(program_handle, "g_text_coord_matrix");
                glUniformMatrix4fv(handle, 1, GL_FALSE, translation_matrix.get());
                checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

                // Display the textured square
                m_detector_temporary_geometry->display();

                // The energy fluence is not empty
                m_energy_fluence_empty = false;
            }

            // Restore the transformation matrices
            popProjectionMatrix();
            popModelViewMatrix();

            // Restore the shader program
            popShaderProgram();
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            // Wait for all OpenGL execution to be completed
            glFinish();
        }
        break;
#endif

    case CUDA:
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "CUDA is not supported.");
        break;

    case OPENCL:
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "OpenCL is not supported.");
        break;

    case OPENMP:
        {
            // Proceed each energy channel
            for (unsigned int current_slice(0);
                    current_slice < m_p_xray_beam->getEnergyChannelNumber();
                    ++current_slice)
            {
                // Get the incident energy
                SpectrumRecord spectrum_record(m_p_xray_beam->getEnergyChannelPerPixelAtSDD(current_slice, m_p_detector, true));
                RATIONAL_NUMBER incident_energy(spectrum_record.getPhotonEnergy());

                // Apply the energy response
                RATIONAL_NUMBER corrected_energy = m_p_detector->applyEnergyResponse(incident_energy);

                // Convert in MeV
                corrected_energy /= MeV;

                // Compute the number of photons, not the energy fluence
                if (!anIntegrateEnergyFlag)
                {
                        corrected_energy = 1.0;
                }

                // Get the number of photons
                RATIONAL_NUMBER photon_number(spectrum_record.getPhotonNumber());

                // Compute the energy
                RATIONAL_NUMBER total_energy(photon_number * corrected_energy / RATIONAL_NUMBER(m_p_detector->getNumberOfSourceSamples()));


#ifdef NDEBUG
#ifdef WIN32
                    #pragma omp parallel for
#else
                    #pragma omp parallel for collapse(2)
#endif
#endif
                for (int j = 0; j < int(m_p_detector->getNumberOfPixels().getY()); ++j)
                    for (int i = 0; i < int(m_p_detector->getNumberOfPixels().getX()); ++i)
                    {
                        m_energy_fluence_image(i, j, 0) += total_energy * exp(-m_sum_mux_dx_image(i, j, current_slice));
                    }
            }
        }
        break;

    default:
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Unsupported computing mode.");
        break;
    }

    // Restore the texturing flags
    popTexture();

    // Restore the state
    popEnableDisableState();

    // Restore the FBO
    popFBO();

#ifndef NDEBUG
    assert(old_FBO_stack_size == g_frame_buffer_object_stack.size());
    assert(old_texture_size == g_texture_stack.size());
    assert(old_enable_disable_stack_size == g_enable_disable_state_stack.size());
    assert(old_shader_stack_size == g_shader_program_stack.size());
#endif
}


//-----------------------------------------
void XRayRenderer::getEnergyFluenceMinMax()
//-----------------------------------------
{
#ifndef NDEBUG
    size_t old_FBO_stack_size = g_frame_buffer_object_stack.size();
    size_t old_texture_size = g_texture_stack.size();
    size_t old_enable_disable_stack_size = g_enable_disable_state_stack.size();
    size_t old_shader_stack_size = g_shader_program_stack.size();
#endif

    // Get the energy fluence map
    getEnergyFluence();

    // Get image statistics
    m_min_value_in_simulated_xray_image =  FLT_MAX;
    m_max_value_in_simulated_xray_image = -FLT_MAX;
    for (unsigned int i(0);
            i < m_p_detector->getNumberOfPixels().getX() * m_p_detector->getNumberOfPixels().getY();
            ++i)
    {
        PixelType pixel_value(m_temp_raw_data.getRawData()[i]);

#if defined(isinf) && defined(isnan)
        if (!isinf(pixel_value) && !isnan(pixel_value))
#else
        if (!std::isinf(pixel_value) && !std::isnan(pixel_value))
#endif
        {
            if (m_min_value_in_simulated_xray_image > pixel_value) m_min_value_in_simulated_xray_image = pixel_value;
            if (m_max_value_in_simulated_xray_image < pixel_value) m_max_value_in_simulated_xray_image = pixel_value;
        }
    }

    // If there is no object, make a white image
    if (m_min_value_in_simulated_xray_image == m_max_value_in_simulated_xray_image)
    {
        m_min_value_in_simulated_xray_image = 0.0;
    }
    else
    {
        // Make sure the min value is not negative (in which case it is an artefact due to the dot product sign)
        if (m_min_value_in_simulated_xray_image < 0)
        {
            m_min_value_in_simulated_xray_image = 0;
        }
    }

    m_max_value_in_simulated_xray_image = std::min(m_max_value_in_simulated_xray_image, m_p_xray_beam->getTotalEnergy());

#ifndef NDEBUG
    assert(old_FBO_stack_size == g_frame_buffer_object_stack.size());
    assert(old_texture_size == g_texture_stack.size());
    assert(old_enable_disable_stack_size == g_enable_disable_state_stack.size());
    assert(old_shader_stack_size == g_shader_program_stack.size());
#endif
}


//-----------------------------------------------
void XRayRenderer::display(bool useNormalisation,
                           bool useLogScale,
                           bool usePowerLaw,
                           double shift,
                           double scale,
                           double gamma)
//-----------------------------------------------
{
#ifndef NDEBUG
    size_t old_FBO_stack_size = g_frame_buffer_object_stack.size();
    size_t old_texture_size = g_texture_stack.size();
    size_t old_enable_disable_stack_size = g_enable_disable_state_stack.size();
    size_t old_shader_stack_size = g_shader_program_stack.size();
#endif

    if (useLogScale && usePowerLaw)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot use both the log scale and the power law at the same time");
    }

    if (useNormalisation)
    {
        getEnergyFluenceMinMax();
    }

#ifdef HAS_OPENGL
    // Activate shaders
    pushShaderProgram();
    m_shader_set[XRAY_DETECTOR_DISPLAY_SHADER_ID].enable();
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Values to pass to the shader
    GLint program_handle(0);
    glGetIntegerv(GL_CURRENT_PROGRAM, &program_handle);

    // Get the current texture unit
    GLint saved_texture;
    glGetIntegerv(GL_ACTIVE_TEXTURE, &saved_texture);

    // Pass the current texture unit to the shader
    GLint handle(glGetUniformLocation(program_handle, "g_attenuation"));
    glUniform1i(handle, saved_texture - GL_TEXTURE0);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Pass the negative rendering mode to the shader
    handle = glGetUniformLocation(program_handle, "g_display_negative_image");
    glUniform1i(handle, m_negative_display_flag);

    // Compute the filter parameters
    GLfloat shift_filter(shift);
    GLfloat scale_filter(scale);
    double range(m_max_value_in_simulated_xray_image - m_min_value_in_simulated_xray_image);

    GLfloat log_epsilon(1.0e-9);
    GLfloat incident_energy(m_p_xray_beam->getTotalEnergy());

    if (fabs(range) > EPSILON /*&& useNormalisation*/)
    {
        if (!useLogScale && !usePowerLaw)
        {
            //shift_filter = -std::pow(10.0, -std::log10(incident_energy / m_min_value_in_simulated_xray_image));
            //scale_filter = 1.0 / (std::pow(10.0, -std::log10(incident_energy / m_max_value_in_simulated_xray_image)) + shift_filter);

            shift_filter = -m_min_value_in_simulated_xray_image;
            scale_filter = 1.0 / (m_max_value_in_simulated_xray_image + shift_filter);
        }
        else if (useLogScale)
        {
            shift_filter = -std::log10(m_min_value_in_simulated_xray_image + log_epsilon);
            scale_filter = 1.0 / (std::log10(m_max_value_in_simulated_xray_image + log_epsilon) - std::log10(m_min_value_in_simulated_xray_image + log_epsilon));
        }
        else
        {
            shift_filter = std::pow(m_min_value_in_simulated_xray_image, gamma);
            scale_filter = 1.0 / (std::pow(m_max_value_in_simulated_xray_image, gamma) - std::pow(m_min_value_in_simulated_xray_image, gamma));
        }
    }

    // Pass the incident energy to the shader
    handle = glGetUniformLocation(program_handle, "g_incident_energy");
    glUniform1f(handle, incident_energy);

    // Pass the shift parameter of the shift-scale filter to the shader
    handle = glGetUniformLocation(program_handle, "g_shift_filter");
    glUniform1f(handle, shift_filter);

    // Pass the scale parameter of the shift-scale filter to the shader
    handle = glGetUniformLocation(program_handle, "g_scale_filter");
    glUniform1f(handle, scale_filter);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Pass the flag of the log-scale filter to the shader
    handle = glGetUniformLocation(program_handle, "g_use_log_scale");
    glUniform1i(handle, useLogScale);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Pass the epsilon parameter of the logfilter to the shader
    handle = glGetUniformLocation(program_handle, "g_log_epsilon");
    glUniform1f(handle, log_epsilon);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Pass the flag of the power law filter to the shader
    handle = glGetUniformLocation(program_handle, "g_use_power_law");
    glUniform1i(handle, usePowerLaw);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Pass the gamma parameter of the power law filter to the shader
    handle = glGetUniformLocation(program_handle, "g_gamma");
    glUniform1f(handle, gamma);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    handle = glGetUniformLocation(program_handle, "g_projection_matrix");
    glUniformMatrix4fv(handle, 1, GL_FALSE, g_current_projection_matrix.get());
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    handle = glGetUniformLocation(program_handle, "g_modelview_matrix");
    glUniformMatrix4fv(handle, 1, GL_FALSE, g_current_modelview_matrix.get());
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Display the texture
    displayEnergyFluence();

    // Restore the shader program
    popShaderProgram();
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Wait for all OpenGL execution to be completed
    glFinish();
#endif

#ifndef NDEBUG
    assert(old_FBO_stack_size == g_frame_buffer_object_stack.size());
    assert(old_texture_size == g_texture_stack.size());
    assert(old_enable_disable_stack_size == g_enable_disable_state_stack.size());
    assert(old_shader_stack_size == g_shader_program_stack.size());
#endif
}


//--------------------------------------------------------------------------------------------------
RATIONAL_NUMBER XRayRenderer::getMuWaterLinearInterpolation(const RATIONAL_NUMBER& anIncidentEnergy)
//--------------------------------------------------------------------------------------------------
{
    // The table of mu water is empty
    if (m_mu_water_set.size() == 0)
    {
        // Load the table
        loadMuWater();
    }

    // The table of mu water is empty
    if (m_mu_water_set.size() == 0)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "The mu water table is empty.");
    }

    // Get an interpolated value
    RATIONAL_NUMBER mu_water(0);

    // The required energy is too low, force to the minimum value
    if (m_mu_water_set.front().getEnergy() >= anIncidentEnergy)
    {
        mu_water = m_mu_water_set.front().getAttenuationCoefficient();
    }
    // The required energy is too high, force to the maximum value
    else if (m_mu_water_set.back().getEnergy() <= anIncidentEnergy)
    {
        mu_water = m_mu_water_set.back().getAttenuationCoefficient();
    }
    // Use an interpolated value
    else
    {
        for (std::vector<AttenuationCoefficient>::const_iterator ite(m_mu_water_set.begin() + 1);
            ite != m_mu_water_set.end();
            ++ite)
        {
            if (ite->getEnergy() >= anIncidentEnergy)
            {
                // Get the interpolated value
                mu_water = (((ite - 1)->getAttenuationCoefficient()) + ((ite->getAttenuationCoefficient()) - ((ite - 1)->getAttenuationCoefficient())) * ((anIncidentEnergy - (ite - 1)->getEnergy()) / (ite->getEnergy() - (ite - 1)->getEnergy())));

                // Return mu value
                return (mu_water);
            }
        }
    }

    // Return mu value
    return (mu_water);
}


//-----------------------------------------------------------------------------------------------------
RATIONAL_NUMBER XRayRenderer::getMuWaterLogLinearInterpolation(const RATIONAL_NUMBER& anIncidentEnergy)
//-----------------------------------------------------------------------------------------------------
{
    // The table of mu water is empty
    if (m_mu_water_set.size() == 0)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "The mu water table is empty.");
    }

    // Get an interpolated value
    RATIONAL_NUMBER mu_water(0);

    // The required energy is too low, force to the minimum value
    if (m_mu_water_set.front().getEnergy() >= anIncidentEnergy)
    {
        mu_water = m_mu_water_set.front().getAttenuationCoefficient();
    }
    // The required energy is too high, force to the maximum value
    else if (m_mu_water_set.back().getEnergy() <= anIncidentEnergy)
    {
        mu_water = m_mu_water_set.back().getAttenuationCoefficient();
    }
    // Use an interpolated value
    else
    {
        for (std::vector<AttenuationCoefficient>::const_iterator ite(m_mu_water_set.begin() + 1);
            ite != m_mu_water_set.end();
            ++ite)
        {
            if (ite->getEnergy() >= anIncidentEnergy)
            {
                // Get the interpolated value
                mu_water = exp(std::log((ite - 1)->getAttenuationCoefficient()) + (std::log(ite->getAttenuationCoefficient()) - std::log((ite - 1)->getAttenuationCoefficient())) * ((anIncidentEnergy - (ite - 1)->getEnergy()) / (ite->getEnergy() - (ite - 1)->getEnergy())));

                // Return mu value
                return (mu_water);
            }
        }
    }

    // Return mu value
    return (mu_water);
}


//----------------------------
void XRayRenderer::updateVBO()
//----------------------------
{
#ifndef NDEBUG
    size_t old_FBO_stack_size = g_frame_buffer_object_stack.size();
    size_t old_texture_size = g_texture_stack.size();
    size_t old_enable_disable_stack_size = g_enable_disable_state_stack.size();
    size_t old_shader_stack_size = g_shader_program_stack.size();
#endif

    if (m_computing_mode == OPENGL)
    {
        // Vertices
        std::vector<GLfloat> p_vertices;

        p_vertices.push_back(0);
        p_vertices.push_back(0);
        p_vertices.push_back(0);

        p_vertices.push_back(1);
        p_vertices.push_back(0);
        p_vertices.push_back(0);

        p_vertices.push_back(1);
        p_vertices.push_back(1);
        p_vertices.push_back(0);

        p_vertices.push_back(0);
        p_vertices.push_back(0);
        p_vertices.push_back(0);

        p_vertices.push_back(1);
        p_vertices.push_back(1);
        p_vertices.push_back(0);

        p_vertices.push_back(0);
        p_vertices.push_back(1);
        p_vertices.push_back(0);


        // Texture coordinates
        std::vector<float> p_texture_coordinates;

        p_texture_coordinates.push_back(0);
        p_texture_coordinates.push_back(0);
        p_texture_coordinates.push_back(0);

        p_texture_coordinates.push_back(1);
        p_texture_coordinates.push_back(0);
        p_texture_coordinates.push_back(0);

        p_texture_coordinates.push_back(1);
        p_texture_coordinates.push_back(1);
        p_texture_coordinates.push_back(0);

        p_texture_coordinates.push_back(0);
        p_texture_coordinates.push_back(0);
        p_texture_coordinates.push_back(0);

        p_texture_coordinates.push_back(1);
        p_texture_coordinates.push_back(1);
        p_texture_coordinates.push_back(0);

        p_texture_coordinates.push_back(0);
        p_texture_coordinates.push_back(1);
        p_texture_coordinates.push_back(0);

        if (!m_detector_temporary_geometry.get())
        {
            m_detector_temporary_geometry.reset(createVBO());
        }

        m_detector_temporary_geometry->setBufferUsageHint(GL_STATIC_DRAW);
        m_detector_temporary_geometry->setVertexData(GL_TRIANGLES,
            p_vertices.size() / 3, 3, GL_FLOAT, &p_vertices[0],
            0, 0, GL_NONE, 0,
            p_texture_coordinates.size() / 3, 3, GL_FLOAT, &p_texture_coordinates[0]);
    }

#ifndef NDEBUG
    assert(old_FBO_stack_size == g_frame_buffer_object_stack.size());
    assert(old_texture_size == g_texture_stack.size());
    assert(old_enable_disable_stack_size == g_enable_disable_state_stack.size());
    assert(old_shader_stack_size == g_shader_program_stack.size());
#endif
}


//---------------------------------------------------------------------
void XRayRenderer::cleanLBuffer(const RATIONAL_NUMBER& aCutoffDistance)
//---------------------------------------------------------------------
{
#ifndef NDEBUG
    size_t old_FBO_stack_size = g_frame_buffer_object_stack.size();
    size_t old_texture_size = g_texture_stack.size();
    size_t old_enable_disable_stack_size = g_enable_disable_state_stack.size();
    size_t old_shader_stack_size = g_shader_program_stack.size();
#endif

    // Save the current FBO
    pushFBO();

    // Store the texturing flags
    pushTexture2D();

    switch (m_computing_mode)
    {
#ifdef HAS_OPENGL
    case OPENGL:
        {
            // Bind frame buffer object
            m_p_fbo_set[XRAY_DETECTOR_CLEANED_LBUFFER_FBO_ID]->Bind();
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            // Store the transformation matrices
            pushProjectionMatrix();
            pushModelViewMatrix();

            // Display a textured square
            loadIdentityProjectionMatrix();
            loadOrthoProjectionMatrix(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);

            loadIdentityModelViewMatrix();

            // Enable the shader
            pushShaderProgram();
            m_shader_set[XRAY_DETECTOR_CLEAN_LBUFFER_SHADER_ID].enable();
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            // Values to pass to the shader
            GLint program_handle(0);
            glGetIntegerv(GL_CURRENT_PROGRAM, &program_handle);

            GLint saved_texture;
            glGetIntegerv(GL_ACTIVE_TEXTURE, &saved_texture);

            GLint handle(glGetUniformLocation(program_handle, "g_l_buffer_texture"));
            glUniform1i(handle, saved_texture - GL_TEXTURE0);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            handle = glGetUniformLocation(program_handle, "g_x_offset");
            glUniform1f(handle, 1.0 / m_p_detector->getNumberOfPixels().getX());
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            handle = glGetUniformLocation(program_handle, "g_y_offset");
            glUniform1f(handle, 1.0 / m_p_detector->getNumberOfPixels().getY());
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            handle = glGetUniformLocation(program_handle, "g_projection_matrix");
            glUniformMatrix4fv(handle, 1, GL_FALSE, g_current_projection_matrix.get());
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            handle = glGetUniformLocation(program_handle, "g_modelview_matrix");
            glUniformMatrix4fv(handle, 1, GL_FALSE, g_current_modelview_matrix.get());
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            handle = glGetUniformLocation(program_handle, "g_cutoff");
            glUniform1f(handle, aCutoffDistance);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            handle = glGetUniformLocation(program_handle, "g_max_radius_Lbuffer_artefact_filter");
            glUniform1i(handle, m_max_radius_Lbuffer_artefact_filter);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            // Rebind textures
            glActiveTexture(saved_texture);
            glBindTexture(GL_TEXTURE_2D, m_p_texture_name_set[XRAY_DETECTOR_UNCLEANED_LBUFFER_TEXTURE_NAME_ID]);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            // Display the textured square
            m_detector_temporary_geometry->display();
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            // Restore the transformation matrices
            popProjectionMatrix();
            popModelViewMatrix();

            // Restore the shader program
            popShaderProgram();
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            // Wait for all OpenGL execution to be completed
            glFinish();
        }
        break;
#endif

    case CUDA:
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "CUDA is not supported.");
        break;

    case OPENCL:
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "OpenCL is not supported.");
        break;

    default:
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Unsupported computing mode.");
        break;
    }

    // Restore the FBO
    popFBO();

    // Restore the texturing flags
    popTexture();

#ifndef NDEBUG
    assert(old_FBO_stack_size == g_frame_buffer_object_stack.size());
    assert(old_texture_size == g_texture_stack.size());
    assert(old_enable_disable_stack_size == g_enable_disable_state_stack.size());
    assert(old_shader_stack_size == g_shader_program_stack.size());
#endif
}


//-----------------------------------------------------
void XRayRenderer::preComputeLBuffer(GLint* apViewport,
        const VEC3& aXRaySourceSampleOffset)
//-----------------------------------------------------
{
    switch (m_computing_mode)
    {
#ifdef HAS_OPENGL
    case OPENGL:
        {
            // Save the current FBO
            pushFBO();

            // Store the state
            pushEnableDisableState(GL_BLEND);
            pushEnableDisableState(GL_DEPTH_TEST);

            // Store the current transformation matrix
            pushModelViewMatrix();

            // Store the current projection matrix
            pushProjectionMatrix();

            // Save the viwport
            glGetIntegerv(GL_VIEWPORT, apViewport);

            // Change the viewport
            glViewport(0, 0, m_p_detector->getNumberOfPixels().getX(), m_p_detector->getNumberOfPixels().getY());

            // Reset its L-buffer
            resetLBuffer();

            // Bind frame buffer object
            m_p_fbo_set[XRAY_DETECTOR_UNCLEANED_LBUFFER_FBO_ID]->Bind();
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            // Activate the first shader
            pushShaderProgram();
            m_shader_set[XRAY_DETECTOR_LBUFFER_SHADER_ID].enable();
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            // Disable depth buffer
            glDisable(GL_DEPTH_TEST);

            // Enable blending
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            // Values to pass to the shader
            GLint program_handle(0);
            glGetIntegerv(GL_CURRENT_PROGRAM, &program_handle);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            VEC3 source_sample_position(m_p_detector->getXraySourceCentre() + aXRaySourceSampleOffset);
            GLuint handle(glGetUniformLocation(program_handle, "g_source_position"));
            glUniform3f(handle, source_sample_position.getX(), source_sample_position.getY(), source_sample_position.getZ());
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            if (!m_p_detector->useParallelBeam())
            {
                handle = glGetUniformLocation(program_handle, "g_projection_type");
                glUniform1i(handle, 0);
                checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
            }
            else
            {
                handle = glGetUniformLocation(program_handle, "g_projection_type");
                glUniform1i(handle, 1);
                checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

                VEC3 ray_direction((m_p_detector->getDetectorPosition() - source_sample_position).normal());
                handle = glGetUniformLocation(program_handle, "g_ray_direction");
                glUniform3f(handle, ray_direction.getX(), ray_direction.getY(), ray_direction.getZ());
                checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
            }

            // Load uniform variables to the GPU
            GLfloat p_float_4[4];
            m_p_detector->getPlaneEquation(p_float_4[0], p_float_4[1], p_float_4[2], p_float_4[3]);
            handle = glGetUniformLocation(program_handle,"g_detector_plane");
            glUniform4fv(handle, 1, p_float_4);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            p_float_4[0] = m_p_detector->getDetectorPosition().getX();
            p_float_4[1] = m_p_detector->getDetectorPosition().getY();
            p_float_4[2] = m_p_detector->getDetectorPosition().getZ();
            handle = glGetUniformLocation(program_handle,"g_detector_center");
            glUniform3fv(handle, 1, p_float_4);
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            VEC3 up_vector((m_p_detector->getRotationMatrix() * m_p_detector->getUpVector()));
            handle = glGetUniformLocation(program_handle,"g_detector_up_vector");
            glUniform3f(handle, -up_vector.getX(), -up_vector.getY(), -up_vector.getZ());
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            VEC3 right_vector((m_p_detector->getRotationMatrix() * m_p_detector->getRightVector()));
            right_vector.normalise();
            handle = glGetUniformLocation(program_handle,"g_detector_right_vector");
            glUniform3f(handle, right_vector.getX(), right_vector.getY(), right_vector.getZ());
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            handle = glGetUniformLocation(program_handle,"g_detector_width_in_units_of_length");
            glUniform1f(handle, m_p_detector->getSizeInUnitOfLength().getX());
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            handle = glGetUniformLocation(program_handle,"g_detector_height_in_units_of_length");
            glUniform1f(handle, m_p_detector->getSizeInUnitOfLength().getY());
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

            handle = glGetUniformLocation(program_handle,"g_to_cm_scale");
            glUniform1f(handle, GLfloat(1.0 / cm));
            checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
        }
        break;
#endif

    case OPENMP:
        // Reset its L-buffer
        resetLBuffer();
        break;

    case CUDA:
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "CUDA is not supported.");
        break;

    case OPENCL:
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "OpenCL is not supported.");
        break;

    default:
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Unsupported computing mode.");
        break;
    }
}


//--------------------------------------------------------------
void XRayRenderer::postComputeLBuffer(const GLint* apViewport,
                                      RATIONAL_NUMBER aDiagonal)
//--------------------------------------------------------------
{
#ifdef HAS_OPENGL
    if (m_computing_mode == OPENGL)
    {
        // Clean the L-Buffer
        if (m_use_l_buffer_artefact_filtering_on_gpu)
        {
            cleanLBuffer(1.0 * aDiagonal);
        }
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // Restore the shader program
        popShaderProgram();
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // Restore the viewport
        glViewport(apViewport[0], apViewport[1], apViewport[2], apViewport[3]);

        // Restore the current FBO
        popFBO();

        // Restore the current transformation matrix
        popModelViewMatrix();

        // Restore the current projection matrix
        popProjectionMatrix();

        // Restore the state
        popEnableDisableState();
        popEnableDisableState();

        // Wait for all OpenGL execution to be completed
        glFinish();
    }
#endif
}


//----------------------------------------------
void XRayRenderer::needUpdateSurfacePerMaterial()
//----------------------------------------------
{
    m_surface_per_material_set_needs_update = true;
}


//------------------------------------------------------------------
Image<float> XRayRenderer::getWhiteImage(bool anIntegrateEnergyFlag)
//------------------------------------------------------------------
{
#ifndef NDEBUG
    size_t old_FBO_stack_size = g_frame_buffer_object_stack.size();
    size_t old_texture_size = g_texture_stack.size();
    size_t old_enable_disable_stack_size = g_enable_disable_state_stack.size();
    size_t old_shader_stack_size = g_shader_program_stack.size();
#endif

    Image<float> white_image; // Empty image

    // Save the flag
    m_use_photon_count_detector = !anIntegrateEnergyFlag;

    // The detector exists
    if (m_p_detector)
    {
        // Allocate and initialise the image
        // The energy is integrated by the detector
        if (anIntegrateEnergyFlag)
        {
            white_image = Image<float>(m_p_detector->getNumberOfPixels()[0],
                m_p_detector->getNumberOfPixels()[1],
                1,
                getTotalEnergyWithDetectorResponse(true));
        }
        // Allocate and initialise the image
        // A photon counting detector was used
        else
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "Not implemented as yet for photon counting detectors");
            // white_image = Image<float>(m_p_detector->getNumberOfPixels()[0],
            //     m_p_detector->getNumberOfPixels()[1],
            //     1, Replase with the number of bands
            //     m_p_xray_beam->getNumberOfPhotonsPerPixelAtSDD()); Replace with the number of photons per bands
        }

        // The source exists
        if (m_p_xray_beam)
        {
            // Poisson noise is enable
            if (m_p_xray_beam->usePoissonNoise())
            {
                // Add Poisson noise
                addPoissonNoise(white_image);
            }
        }

        // Apply the impulse response of the detector if needed
        if (m_p_detector)
        {
            // Favour PSF over LSF
            if (m_p_detector->getPSF().getWidth() * m_p_detector->getPSF().getHeight() > 0)
            {
                white_image = white_image.convolution2D(m_p_detector->getPSF());
            }
            else if (m_p_detector->getLSF().size())
            {
                white_image = white_image.convolution1D(m_p_detector->getLSF());
            }
        }
    }

#ifndef NDEBUG
    assert(old_FBO_stack_size == g_frame_buffer_object_stack.size());
    assert(old_texture_size == g_texture_stack.size());
    assert(old_enable_disable_stack_size == g_enable_disable_state_stack.size());
    assert(old_shader_stack_size == g_shader_program_stack.size());
#endif

    // Return the flat field image
    return white_image;
}


//-----------------------------------------------------------------------------------
Image<unsigned short> XRayRenderer::getWhiteImageWithGain(bool anIntegrateEnergyFlag)
//-----------------------------------------------------------------------------------
{
#ifndef NDEBUG
    size_t old_FBO_stack_size = g_frame_buffer_object_stack.size();
    size_t old_texture_size = g_texture_stack.size();
    size_t old_enable_disable_stack_size = g_enable_disable_state_stack.size();
    size_t old_shader_stack_size = g_shader_program_stack.size();
#endif

    Image<PixelType> white_image_float = getWhiteImage();
    Image<unsigned short> white_image_with_gain(white_image_float.getWidth(),
        white_image_float.getHeight(),
        white_image_float.getDepth());

    // Apply the detector gain
    // Clamp and cast the data
    std::transform(white_image_float.getRawData(),
        white_image_float.getRawData() + white_image_float.getWidth() * white_image_float.getHeight() * white_image_float.getDepth(),
        white_image_with_gain.getRawData(), [gain=m_p_detector->getGain()](XRayRenderer::PixelType x) {
            return static_cast<unsigned short>(std::clamp(static_cast<int>(std::round(x * gain)), 0, USHRT_MAX));
        });

    // Apply the bias
    RATIONAL_NUMBER bias = std::abs(m_p_detector->getBias());
    if (std::abs(bias) > EPSILON) {

        RATIONAL_NUMBER std_dev_bias = std::abs(m_p_detector->getStdDevBias());

        // Use a constant
        if (std::abs(std_dev_bias) > EPSILON) {
            std::transform(white_image_with_gain.getRawData(),
                white_image_with_gain.getRawData() + white_image_with_gain.getWidth() * white_image_with_gain.getHeight() * white_image_with_gain.getDepth(),
                white_image_with_gain.getRawData(), [bias=bias](XRayRenderer::PixelType x) {
                    return static_cast<unsigned short>(std::clamp(static_cast<int>(std::round(x + bias)), 0, USHRT_MAX));
            });
        }
        // Use an additive Gaussian noise
        else {
            // Has OpenMP
            #ifdef NDEBUG
                #ifdef HAS_OPENMP
                    #ifdef WIN32
                        #pragma omp parallel for
                    #else
                        #pragma omp parallel for collapse(3) //schedule(static,1)
                    #endif
                #endif
            #endif
            for (int k = 0; k < int(white_image_with_gain.getDepth()); ++k)
            {
                for (int j = 0; j < int(white_image_with_gain.getHeight()); ++j)
                {
                    for (int i = 0; i < int(white_image_with_gain.getWidth()); ++i) {
                        RATIONAL_NUMBER pixel_value = white_image_with_gain.getPixel(i, j, k);

#ifndef HAS_OPENMP
                        pixel_value += bias + std_dev_bias * (*m_p_normal_distribution)(*m_p_random_number_generator);
#else
                        pixel_value += bias + std_dev_bias * m_p_normal_distribution[omp_get_thread_num()](m_p_random_number_generator[omp_get_thread_num()]);
#endif

                        // Update the pixel value with the additive Gaussian noise
                        white_image_with_gain.setPixel(i, j, k, pixel_value);
                    }
                }
            }
        }
    }

#ifndef NDEBUG
    assert(old_FBO_stack_size == g_frame_buffer_object_stack.size());
    assert(old_texture_size == g_texture_stack.size());
    assert(old_enable_disable_stack_size == g_enable_disable_state_stack.size());
    assert(old_shader_stack_size == g_shader_program_stack.size());
#endif

    // Return the flat field image
    return white_image_with_gain;
}


//----------------------------------------------
void XRayRenderer::updateSurfacePerMaterialSet()
//----------------------------------------------
{
    if (m_surface_per_material_set_needs_update)
    {
        m_p_surface_per_material_set.clear();
        m_surface_per_material_set_needs_update = false;

        // Look at all the meshes
        for (std::vector<SceneGraphNode*>::const_iterator ite = m_p_inner_surface_set.begin();
                ite != m_p_inner_surface_set.end();
                ++ite)
        {
            bool added = false;

            SceneGraphNode* p_temp1 = *ite;

            if (m_p_surface_per_material_set.size())
            {
                unsigned int i = 0;

                while (!added && i < m_p_surface_per_material_set.size())
                {
                    SceneGraphNode* p_temp2 = m_p_surface_per_material_set[i].front();

                    if (p_temp2->getPhotonCrossSection() == p_temp1->getPhotonCrossSection())
                    {
                        added = true;
                        m_p_surface_per_material_set[i].push_back(p_temp1);
                    }
                    ++i;
                }
            }

            if (!added)
            {
                m_p_surface_per_material_set.push_back(std::vector<SceneGraphNode*>(1, p_temp1));
            }
        }
    }
}


//-----------------------------------------------------------
void XRayRenderer::addPoissonNoise(Image<PixelType>& anImage)
//-----------------------------------------------------------
{
#ifndef NDEBUG
    size_t old_FBO_stack_size = g_frame_buffer_object_stack.size();
    size_t old_texture_size = g_texture_stack.size();
    size_t old_enable_disable_stack_size = g_enable_disable_state_stack.size();
    size_t old_shader_stack_size = g_shader_program_stack.size();
#endif

    // We implement the method presented by Gaetan Lehmann (INRAe, France) in "Noise Simulation" published in the Insight Journal.
    // For details, see https://doi.org/10.54294/vh6vbw

    // Get the total energy
    double total_energy = getTotalEnergyWithDetectorResponse(true);

    // Convert an energy into a number of photons
    // 1.0 is used for photon counting detectors
    double energy2photons = 1.0;

    // The energy was integrated
    if (!m_use_photon_count_detector)
    {
        energy2photons = m_p_xray_beam->getNumberOfPhotonsPerPixelAtSDD(m_p_detector, true) / total_energy;
    }

    // Convert the image in energy into the targetted number of photons
    Image<PixelType> x_ray_image_photons = anImage * energy2photons;

    // Has OpenMP
    #ifdef NDEBUG
        #ifdef HAS_OPENMP
            #ifdef WIN32
                #pragma omp parallel for
            #else
                #pragma omp parallel for collapse(3) //schedule(static,1)
            #endif
        #endif
    #endif
    for (int k = 0; k < int(x_ray_image_photons.getDepth()); ++k)
    {
        for (int j = 0; j < int(x_ray_image_photons.getHeight()); ++j)
        {
            for (int i = 0; i < int(x_ray_image_photons.getWidth()); ++i)
            {
                // Convert energy into number of photons
                int number_of_photons = anImage.getPixel(i, j, k) * energy2photons;

                if (number_of_photons > 0)
                {
                    // Use the Poisson distribution
                    if (number_of_photons < m_p_xray_beam->getLambdaCutoff())
                    {
                        const double L = std::exp(-number_of_photons);
                        unsigned long k = 0;
                        double p = 1.0;

                        do
                        {
                            k += 1;

                            #ifndef HAS_OPENMP
                                p *= (*m_p_uniform_distribution)(*m_p_random_number_generator);
                            #else
                                p *= m_p_uniform_distribution[omp_get_thread_num()](m_p_random_number_generator[omp_get_thread_num()]);
                            #endif

                        } while (p > L);

                        number_of_photons = k;
                    }
                    else
                    {
                        //number_of_photons = number_of_photons + std::sqrt(number_of_photons) * randn->GetVariate();
                        #ifndef HAS_OPENMP
                            number_of_photons = number_of_photons + std::sqrt(number_of_photons) * (*m_p_normal_distribution)(*m_p_random_number_generator);
                        #else
                            number_of_photons = number_of_photons + std::sqrt(number_of_photons) * m_p_normal_distribution[omp_get_thread_num()](m_p_random_number_generator[omp_get_thread_num()]);
                        #endif
                    }

                    // Convert number of photons into energy
                    anImage.setPixel(i, j, k, number_of_photons / energy2photons);
                }
                // No photon
                else
                    anImage.setPixel(i, j, k, 0);
            }
        }
    }

#ifndef NDEBUG
    assert(old_FBO_stack_size == g_frame_buffer_object_stack.size());
    assert(old_texture_size == g_texture_stack.size());
    assert(old_enable_disable_stack_size == g_enable_disable_state_stack.size());
    assert(old_shader_stack_size == g_shader_program_stack.size());
#endif
}
