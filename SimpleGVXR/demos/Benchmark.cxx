/*

Copyright (c) 2025-present, Prof Franck P. Vidal (franck.vidal@stfc.ac.uk),
UK Research and Innovation, All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

3. Neither the name of UK Research and Innovation nor the names of its contributors
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
*   @file       Benchmark.cxx
*
*   @brief      Compare the use of various precisions for floating point numbers on GPU.
*               We try 32 bits (full-precision), 16 bits (half-precision), and
*               a combination of both (mixed-precision).
*               We use images and meshes of increasing sizes.
*               Our findings show that half-precision and mixed-precision are
*               twice as fast as full-precision. There is a significant loss
*               of image quality for half-precision, but not for mixed-precision.
*               Noise is executed on the CPU, and this is the main bottleneck.
*
*   @version    1.0
*
*   @date       23/09/2025
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
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, Sept 2025-present, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <string>
#include <vector>
#include <iostream>
#include <chrono>
#include <cmath>

#ifdef HAS_FILESYSTEM_HEADER
#include <filesystem>
#else
#include <experimental/filesystem>
#endif

#ifndef __SimpleGVXR_h
#include "SimpleGVXR.h"
#endif

#ifndef SGVXR_Internal_h
#include "internal.h"
#endif


//******************************************************************************
//  Namespaces
//******************************************************************************
#ifdef HAS_FILESYSTEM_HEADER
namespace fs = std::filesystem;
#else
namespace fs = std::experimental::filesystem;
#endif

using namespace std::chrono;


//******************************************************************************
//  Function declarations
//******************************************************************************
std::string getMatrixLabel(unsigned int i);
std::string getFoamLabel(unsigned int i);
std::string getParticleLabel(unsigned int i);
void setDetector();
void setSource();
void createSTL();
void createEnvironment(float aScalingFactor,
    unsigned int aNumberOfBitsPerFloat,
    unsigned int aMeshResolution,
    bool aUseNoiseFlag);


//******************************************************************************
//  Global variables
//******************************************************************************
const fs::path g_output_path("benchmark-results");

#ifdef NDEBUG
const std::vector<unsigned int> g_volume_resolution_set({15, 32, 64, 128});
const std::vector<float> g_detector_resolution_scaling_factor_set({0.75, 1.0, 1.25, 2, 3, 4, 5});
#else
const std::vector<unsigned int> g_volume_resolution_set({15, 32});
const std::vector<float> g_detector_resolution_scaling_factor_set({0.75, 1.0, 1.25});
#endif

const unsigned int g_total_runs = 50;


//******************************************************************************
//  Function definitions
//******************************************************************************


//-----------------------------------------------
inline std::string getMatrixLabel(unsigned int i)
//-----------------------------------------------
{
    return std::string("matrix-") + std::to_string(i) + "x" + std::to_string(i) + "x" + std::to_string(i);
}


//---------------------------------------------
inline std::string getFoamLabel(unsigned int i)
//---------------------------------------------
{
    return std::string("foam-") + std::to_string(i) + "x" + std::to_string(i) + "x" + std::to_string(i);
}

//-------------------------------------------------
inline std::string getParticleLabel(unsigned int i)
//-------------------------------------------------
{
    return std::string("particle-") + std::to_string(i) + "x" + std::to_string(i) + "x" + std::to_string(i);
}


//-------------------------------------------
inline void setDetector(float aScalingFactor)
//-------------------------------------------
{
    setDetectorPosition(10.0, 0.0, 0.0, "cm");
    setDetectorUpVector(0, 0, -1);
    setDetectorPixelSize(0.5 / aScalingFactor, 0.5 / aScalingFactor, "mm");
    setDetectorNumberOfPixels(lround(640 * aScalingFactor), lround(320 * aScalingFactor));
}


//---------------------
inline void setSource()
//---------------------
{
    setSourcePosition(-40.0,  0.0, 0.0, "cm");
    usePointSource();
    setVoltage(80, "kV");
    addFilter("Cu", 1, "mm");
    setmAs(0.5);
}


//--------------
void createSTL()
//--------------
{
    for (unsigned int volume_resolution: g_volume_resolution_set)
    {
        std::string particle_label = getParticleLabel(volume_resolution);
        std::string matrix_label = getMatrixLabel(volume_resolution);
        std::string foam_label = getFoamLabel(volume_resolution);

        float voxel_size = 5.0 / volume_resolution;
        makeFoamInCuboid(particle_label,
        "",
        5, 5, 5, // 5mm in total
        0.25,
        50,
        50,
        voxel_size, // voxel size:
        "mm",
        1);

        // Create SITK image stack from the array output by foam function
        //sitk_image = sitk.GetImageFromArray(getImplicitSurfaceLastVoxelisation(particle_label))
        //sitk_image.SetSpacing(3*[voxel_size])
        //sitk.WriteImage(sitk_image, os.path.join(output_path, particle_label + ".mhd"))

        // Use implicit surface to create foam mesh
        emptyMesh(foam_label);
        createMeshFromImplicitSurface(particle_label, particle_label, 0.05, foam_label, 1);
        createMeshFromImplicitSurface(particle_label, matrix_label, -10.0, 0.05, foam_label, 1);

        std::string file_name = (g_output_path / fs::path(particle_label + ".stl")).string();
        saveSTLfile(particle_label, file_name);

        saveSTLfile(matrix_label, (g_output_path / fs::path(matrix_label + ".stl")).string());
    }
}


//--------------------------------------------------------
void createEnvironment(float aScalingFactor,
                       unsigned int aNumberOfBitsPerFloat,
                       unsigned int aMeshResolution,
                       bool aUseNoiseFlag)
//--------------------------------------------------------
{
    // Create a simulation engine
    createOpenGLContext(0,
        4, 6,
        aNumberOfBitsPerFloat);

    setDetector(aScalingFactor);
    setSource();

    std::string foam_label = getFoamLabel(aMeshResolution);
    std::string matrix_label = getMatrixLabel(aMeshResolution);
    std::string particle_label = getParticleLabel(aMeshResolution);

    removePolygonMeshesFromSceneGraph();
    emptyMesh(foam_label);

    loadMeshFile(particle_label,
        (g_output_path / fs::path(particle_label + ".stl")).string(),
        "mm",
        true,
        foam_label);

    loadMeshFile(matrix_label,
        (g_output_path / fs::path(matrix_label + ".stl")).string(),
        "mm",
        true,
        foam_label);

    // Set the material composition

    // AlSi10Mg alloy
    setColor(matrix_label, 136 / 255,139 / 255,141 / 255, 0.5);
    setMixture(matrix_label,
        std::vector<std::string>({"Al", "Si", "Mg"}),
        std::vector<double>({100 - 9.0 - 0.25, 9.0, 0.25}));
    setDensity(matrix_label, 2.67, "g/cm3");

    // Silicon carbide
    setColor(particle_label, 136 / 255,139 / 255,141 / 255, 0.5);
    setCompound(particle_label, "SiC");
    setDensity(particle_label, 3.21, "g/cm3");

    float best_sod = getBestSOD(foam_label, "mm");
    applySOD(foam_label, best_sod, "mm");

    if (aUseNoiseFlag)
        enablePoissonNoise();
    else
        disablePoissonNoise();
}


//--------
int main()
//--------
{
    try {
        // Check if g_output_path directory exists
        if (!fs::is_directory(g_output_path) || !fs::exists(g_output_path))
        {
            // Create the output directory if needed
            fs::create_directory(g_output_path);
        }

        useLogFile((g_output_path / fs::path("log.txt")).string());

        // We set the random seed so that the foam is always the same
        setSeed(1234567890);
        createSTL();

        std::vector<float> p_result;
#ifdef NDEBUG
        std::vector<bool> p_noise_set({false, true});
#else
        std::vector<bool> p_noise_set({false});
#endif

        for (bool use_noise: p_noise_set)
        {
            for (unsigned int volume_resolution: g_volume_resolution_set)
            {
                for (float detector_resolution_scaling_factor: g_detector_resolution_scaling_factor_set)
                {
                    // Half-precision floating-point numbers
                    unsigned int number_of_bits_per_float = 16;
                    createEnvironment(detector_resolution_scaling_factor,
                        number_of_bits_per_float,
                        volume_resolution,
                        use_noise);

                    unsigned int cols = getDetectorNumberOfPixels()[0];
                    unsigned int rows = getDetectorNumberOfPixels()[1];

                    std::vector<float> p_x_ray_image_fp16(cols * rows);
                    computeXRayImage(&p_x_ray_image_fp16[0], rows, cols);
                    if (use_noise)
                        saveLastXRayImage((g_output_path / fs::path("x_ray_image-with_noise-fp16.mhd")).string());
                    else
                        saveLastXRayImage((g_output_path / fs::path("x_ray_image-without_noise-fp16.mhd")).string());

                    auto start_fp16 = high_resolution_clock::now();
                    for (unsigned int i = 0; i < g_total_runs; ++i)
                        computeXRayImage(&p_x_ray_image_fp16[0], rows, cols);
                    auto end_fp16 = high_resolution_clock::now();

                    destroy();

                    // Full-precision floating-point numbers
                    number_of_bits_per_float = 32;
                    createEnvironment(detector_resolution_scaling_factor,
                        number_of_bits_per_float,
                        volume_resolution,
                        use_noise);

                    std::vector<float> p_x_ray_image_fp32(cols * rows);
                    computeXRayImage(&p_x_ray_image_fp32[0], rows, cols);
                    if (use_noise)
                        saveLastXRayImage((g_output_path / fs::path("x_ray_image-with_noise-fp32.mhd")).string());
                    else
                        saveLastXRayImage((g_output_path / fs::path("x_ray_image-without_noise-fp32.mhd")).string());

                    auto start_fp32 = high_resolution_clock::now();
                    for (unsigned int i = 0; i < g_total_runs; ++i)
                        computeXRayImage(&p_x_ray_image_fp32[0], rows, cols);
                    auto end_fp32 = high_resolution_clock::now();

                    destroy();


                    // Mixed-precision floating-point numbers
                    number_of_bits_per_float = 0;
                    createEnvironment(detector_resolution_scaling_factor,
                        number_of_bits_per_float,
                        volume_resolution,
                        use_noise);
                    std::vector<float> p_x_ray_image_fp_mixed(cols * rows);
                    computeXRayImage(&p_x_ray_image_fp_mixed[0], rows, cols);
                    if (use_noise)
                        saveLastXRayImage((g_output_path / fs::path("x_ray_image-with_noise-fp_mixed.mhd")).string());
                    else
                        saveLastXRayImage((g_output_path / fs::path("x_ray_image-without_noise-fp_mixed.mhd")).string());

                    auto start_fp_mixed = high_resolution_clock::now();
                    for (unsigned int i = 0; i < g_total_runs; ++i)
                        computeXRayImage(&p_x_ray_image_fp_mixed[0], rows, cols);
                    auto end_fp_mixed = high_resolution_clock::now();

                    std::string matrix_label = getMatrixLabel(volume_resolution);
                    std::string particle_label = getParticleLabel(volume_resolution);

                    unsigned int number_of_triangles = getNumberOfPrimitives(matrix_label) + getNumberOfPrimitives(particle_label);
                    unsigned int number_of_vertices = getVertexNumber(matrix_label) + getVertexNumber(particle_label);

                    destroy();



                    float duration_fp32 =  duration_cast<milliseconds>(end_fp32 - start_fp32).count();
                    float duration_fp16 = duration_cast<milliseconds>(end_fp16 - start_fp16).count();
                    float duration_fp_mixed = duration_cast<milliseconds>(end_fp_mixed - start_fp_mixed).count();

                    float NRMSE_fp16 = computeRMSE(p_x_ray_image_fp32, p_x_ray_image_fp16, true);
                    float ZNCC_fp16 = computeZNCC(p_x_ray_image_fp32, p_x_ray_image_fp16);

                    float NRMSE_fp_mixed = computeRMSE(p_x_ray_image_fp32, p_x_ray_image_fp_mixed, true);
                    float ZNCC_fp_mixed = computeZNCC(p_x_ray_image_fp32, p_x_ray_image_fp_mixed);

                    p_result.push_back(use_noise);
                    p_result.push_back(32);
                    p_result.push_back(cols);
                    p_result.push_back(rows);
                    p_result.push_back(number_of_triangles);
                    p_result.push_back(number_of_vertices);
                    p_result.push_back(duration_fp32 / g_total_runs);
                    p_result.push_back(g_total_runs / (1e-3 * duration_fp32));
                    p_result.push_back(duration_fp32 / duration_fp32);
                    p_result.push_back(0);
                    p_result.push_back(1);

                    p_result.push_back(use_noise);
                    p_result.push_back(16);
                    p_result.push_back(cols);
                    p_result.push_back(rows);
                    p_result.push_back(number_of_triangles);
                    p_result.push_back(number_of_vertices);
                    p_result.push_back(duration_fp16 / g_total_runs);
                    p_result.push_back(g_total_runs / (1e-3 * duration_fp16));
                    p_result.push_back(duration_fp32 / duration_fp16);
                    p_result.push_back(NRMSE_fp16);
                    p_result.push_back(ZNCC_fp16);

                    p_result.push_back(use_noise);
                    p_result.push_back(1);
                    p_result.push_back(cols);
                    p_result.push_back(rows);
                    p_result.push_back(number_of_triangles);
                    p_result.push_back(number_of_vertices);
                    p_result.push_back(duration_fp_mixed / g_total_runs);
                    p_result.push_back(g_total_runs / (1e-3 * duration_fp_mixed));
                    p_result.push_back(duration_fp32 / duration_fp_mixed);
                    p_result.push_back(NRMSE_fp_mixed);
                    p_result.push_back(ZNCC_fp_mixed);
                }
            }
        }

        unsigned int number_of_cols = p_result.size() / (g_volume_resolution_set.size() * g_detector_resolution_scaling_factor_set.size()) / (3 * p_noise_set.size());
        unsigned int number_of_rows = p_result.size() / number_of_cols;



        std::cout << "gVXR:\t" << getVersionOfCoreGVXR() << std::endl;
        std::cout << "SimpleGVXR:\t" << getVersionOfSimpleGVXR() << std::endl;
        std::cout << "OpenGL renderer:\t" << getOpenGlRenderer() << std::endl;
        std::cout << "OpenGL version:\t" << getOpenGlVersion() << std::endl;
        std::cout << "OpenGL vendor:\t" << getOpenGlVendor() << std::endl;
        std::cout << "Shading language version:\t" << getShadingLanguageVersion() << std::endl;
        std::cout << "Poisson noise implementation:\t" << getNoiseGenerator() << std::endl;
        std::cout << "Noise\tPrecision\tCols\tRows\tTriangles\tVertices\tAvg (millisec)\tFPS\tSpeedup\tNRMSE\tZNCC" << std::endl;

        for (unsigned int i = 0; i < number_of_rows; ++i)
        {
            for (unsigned int j = 0; j < number_of_cols; ++j)
            {
                float value = p_result[i * number_of_cols + j];

                if (j == 0)
                {
                    if (bool(std::round(value)))
                        std::cout << "Yes" << "\t";
                    else
                        std::cout << "No" << "\t";
                }
                else if (j == 1 && value < 15)
                {
                    std::cout << "mixed" << "\t";
                }
                else
                {
                    if (j == number_of_cols - 4)
                        std::cout << std::round(value) << "\t";
                    else
                        std::cout << value << "\t";
                }
            }
            std::cout << std::endl;
        }

        // Interactive visualisation
        // The user can rotate the 3D scene and zoom-in and -out in the visualisation window.

        // - Keys are:
        //     - Q/Escape: to quit the event loop (does not close the window)
        //     - B: display/hide the X-ray beam
        //     - W: display the polygon meshes in solid or wireframe
        //     - N: display the X-ray image in negative or positive
        //     - H: display/hide the X-ray detector
        // - Mouse interactions:
        //     - Zoom in/out: mouse wheel
        //     - Rotation: Right mouse button down + move cursor```
        //renderLoop();

        return 0;
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}
