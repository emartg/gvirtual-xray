/*

Copyright (c) 2019, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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
*   @file       TestSinogramFromMesh.cxx
*
*   @brief      Unit testing: Sinogram,
*   			the unit test corresponds to validation_08,
*   			last date of check: Passed test on 27 Sept 2019.
*
*   @version    1.0
*
*   @date       27/09/2019
*   @author     Dr Franck P. Vidal
*
*   @section    License
*               BSD 3-Clause License.
*
*               For details on use and redistribution please refer
*               to http://opensource.org/licenses/BSD-3-Clause
*
*   @section    Copyright
*               (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*               http://www.fpvidal.net/, Sept 2019, 2019, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <cmath>
#include <algorithm>
#include "gtest/gtest.h"

#ifndef GVXR_LOGGER_H
#include "gVirtualXRay/Logger.h"
#endif

#ifndef SGVXR_SimpleGVXR_h
#include "SimpleGVXR.h"
#endif


//******************************************************************************
//  Constant variables
//******************************************************************************
const double g_incident_energy_in_MeV(0.08);
const unsigned int g_number_of_pixels[]  = {367, 128};
const double g_resolution_in_mm[]        = {0.5, 1.0};
const double g_source_position_in_cm[]   = {0.0,  10.0, 0.0};
const double g_detector_position_in_cm[] = {0.0, -10.0, 0.0};
const double g_detector_up_vector[]      = {0.0,   0.0, 1.0};

const unsigned int g_angle_number(360);
const double g_angle_step(0.5);


//******************************************************************************
//  Global variables
//******************************************************************************
int g_main_window_width(1280 / 2.0);
int g_main_window_height(800 / 2.0);


std::vector<double> g_p_vertex_data;
std::vector<unsigned int> g_p_index_data;


//******************************************************************************
//  Function declaration
//******************************************************************************
void createPhantom();

void loadSTLFile();

void loadDetector();

void loadSource();


//----------------------------------------------
TEST(TestSinogramFromMesh, TestSinogramFromMesh)
//----------------------------------------------
{
    // Initialize random seed
    srand(time(0));

    // Create an OpenGL context
#ifdef HAS_OPENGL
#ifdef HAS_EGL
    createWindow(-1, 0,
        "EGL",
        3, 2,
        0); // Half-precision floating-point numbers is enough
#else // HAS_EGL
#ifdef __APPLE__
 // OpenGL 4.2 and above are not supported on MacOS (at least on my compute with MacOS Big Sur)
    createWindow(-1, 0,
        "OPENGL",
        4, 1,
        0); // Half-precision floating-point numbers is enough
#else // __APPLE__
    createWindow(-1, 0,
        "OPENGL",
        4, 6,
        0); // Half-precision floating-point numbers is enough
#endif // __APPLE__
#endif // HAS_EGL
#else // HAS_OPENGL
    createWindow(-1, 0, "OPENMP");
#endif // HAS_OPENGL

    setWindowSize(g_main_window_width, g_main_window_height);

    // Create the phantom
    createPhantom();

    // Display the mu coefficients
    gVirtualXRay::LOGGER.logNow() << "Linear attenuation coefficient at:\t" << g_incident_energy_in_MeV << " MeV" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "Mu(HU(0):     \t" << getMuFromHU(0, g_incident_energy_in_MeV, "MeV") << " cm-1" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "Mu(HU(-1000): \t" << getMuFromHU(-1000, g_incident_energy_in_MeV, "MeV") << " cm-1" << std::endl;

    // Load the data
    loadDetector();
    loadSource();

    // Add the geometry to the X-ray renderer
    loadSTLFile();
    addPolygonMeshAsInnerSurface("phantom");

    gVirtualXRay::LOGGER.logNow() << "Angular step: " << g_angle_step << " degrees" << std::endl;
    gVirtualXRay::LOGGER.logNow() << "Angular span: [0, " <<
        g_angle_step * (g_angle_number - 1) <<
        "] degrees" << std::endl;

    // Enable artefact filtering on GPU
    enableArtefactFilteringOnGPU();

#ifdef HAS_OPENGL
    // Compute the sinogram
    // std::vector<std::vector<std::vector<float> > > sinogram = computeSinogram(0, 0, 0,
    //     "cm",
    //     g_angle_number,
    //     g_angle_step);

    // // Save the sinogram
    // saveLastSinogram("TestSinogram_simulation_sinogram.mha");

    // Compute the projection set
    // std::vector<std::vector<std::vector<float> > > projection_set = computeProjectionSet(
    //     0, 0, 0,
    //     "cm",
    //     g_angle_number,
    //     g_angle_step);
    computeCTAcquisition("", "",
        g_angle_number,
        0, true, g_angle_step * (g_angle_number - 1),
        0,
        0, 0, 0, "cm",
        g_detector_up_vector[0], g_detector_up_vector[1], g_detector_up_vector[2],
        true,
        1);

    // Save the projection set
    saveLastProjectionSet("TestSinogram_simulation_projection_set.mhd");

    // // Load reference sinogram (computed using GNU octave)
    // std::vector<std::vector<std::vector<float> > > reference_sinogram(loadImage3D("data/phantom_sinogram.mhd"));

    // // Compare reference sinogram and simulated sinogram using NCC
    // ASSERT_NEAR(computeZNCC(reference_sinogram, sinogram), 1.0, 0.00001);

    // // Compare reference sinogram and simulated sinogram using RMSE
    // double RMSE = computeRMSE(reference_sinogram, sinogram);
    // double range1 = (getMaxValue(reference_sinogram) - getMinValue(reference_sinogram));
    // double range2 = (getMaxValue(sinogram) - getMinValue(sinogram));
    // ASSERT_TRUE(RMSE < 0.01f * std::min(range1, range2));

    // Load reference projection (computed using GNU octave)
    std::vector<std::vector<std::vector<float> > > reference_projections(loadImage3D("data/phantom_projections.mhd"));

    // Load reference projection (computed using GNU octave)
    // Reading MHD/MHA files written on Windows does not work
    // std::vector<std::vector<std::vector<float> > > test_projections(loadImage3D("TestSinogram_simulation_projection_set.mhd"));
    std::vector<std::vector<std::vector<float> > > test_projections(getLastProjectionSet());        
        
    // Compare reference projection and simulated projection using NCC
    ASSERT_NEAR(100 * computeZNCC(reference_projections, test_projections), 100.0, 0.01);

    // Compare reference projection and simulated projection using RMSE
    // double RMSE = computeRMSE(reference_projections, test_projections);
    // double range1 = (getMaxValue(reference_projections) - getMinValue(reference_projections));
    // double range2 = (getMaxValue(test_projections) - getMinValue(test_projections));

    // gVirtualXRay::LOGGER.logNow() << RMSE << std::endl;
    // gVirtualXRay::LOGGER.logNow() << range1 << std::endl;
    // gVirtualXRay::LOGGER.logNow() << range2 << std::endl;
    // ASSERT_TRUE(RMSE < 0.01f * std::min(range1, range2));
#endif

    // Close the window and shut GLFW
    destroyAllWindows();
}


//------------------
void createPhantom()
//------------------
{
    unsigned int phantom_size[] = {256, 256, 128};
    unsigned int number_of_voxels = phantom_size[0] * phantom_size[1] * phantom_size[2];

    short HU_air = -1000;

    double voxel_spacing[] = {0.5, 0.5, 1.0};
    std::vector<short> phantom(number_of_voxels, HU_air);

    // Set pixels in a square to one value
    unsigned int slice_size(phantom_size[2] / 4);
    unsigned int start_slice((phantom_size[2] - slice_size) / 2);

    unsigned int step_number(6);


    unsigned int step_height(((phantom_size[0] - (phantom_size[0] / 2) / 2)) / step_number);

    for(unsigned int s = start_slice; s < start_slice + slice_size; s++)
        {
        for (unsigned int step(0); step < step_number; ++step)
            {
            unsigned int row_size(phantom_size[1] / 2);
            unsigned int start_row((phantom_size[1]-row_size) / 4);
            start_row += step * step_height;

            for (unsigned int r(start_row);
                r < (start_row + step_height);
                ++r)
                {
                unsigned int column_size(phantom_size[0] / 2);
                unsigned int start_column((phantom_size[0]-column_size) / 4);

                unsigned int step_width(step_height * (1 + step));
                for (unsigned int c(start_column);
                    c < (start_column + step_width);
                    ++c)
                    {
                    phantom[s * phantom_size[0] * phantom_size[1] + c * phantom_size[0] + r] = 0;
                    }
                }
            }
        }

    // Extract the isosurface
    makeIsoSurface("phantom",
        -500,
        phantom,
        phantom_size[0],
        phantom_size[1],
        phantom_size[2],
        voxel_spacing[0],
        voxel_spacing[1],
        voxel_spacing[2],
        "mm",
        "root");

    moveToCenter("phantom");
    applyCurrentLocalTransformation("phantom");

    //phantom.save("TestSinogram_phantom.mha");
    saveSTLfile("phantom", "step_wedge.stl");
}


//----------------
void loadSTLFile()
//----------------
{
    // Set geometry
    setHounsfieldValue("phantom", 0);

    double red((double(rand()) / (RAND_MAX)) + 1);
    double green((double(rand()) / (RAND_MAX)) + 1);
    double blue((double(rand()) / (RAND_MAX)) + 1);
    double alpha(1.0);

    double base_colour[] = {red, green, blue};
    //base_colour.normalise();

    setColor("phantom",
        base_colour[0] * 0.19225,
        base_colour[1] * 0.19225,
        base_colour[2] * 0.19225,
        alpha);

    //setShininess("phantom", 50);

    // Compute the translation vector
    moveToCentre();
}


//-----------------
void loadDetector()
//-----------------
{
    setDetectorPixelSize(g_resolution_in_mm[0], g_resolution_in_mm[1], "mm");

    setDetectorNumberOfPixels(g_number_of_pixels[0], g_number_of_pixels[1]);

    setDetectorPosition(
        g_detector_position_in_cm[0],
        g_detector_position_in_cm[1],
        g_detector_position_in_cm[2],
        "cm");

        setDetectorUpVector(
            g_detector_up_vector[0],
            g_detector_up_vector[1],
            g_detector_up_vector[2]);
}


//---------------
void loadSource()
//---------------
{
    // Set the energy
    setMonoChromatic(g_incident_energy_in_MeV, "MeV", 1);

    // Set the source position
    setSourcePosition(g_source_position_in_cm[0],
        g_source_position_in_cm[1],
        g_source_position_in_cm[2],
        "cm");

    // Use a parallel source
    useParallelBeam();
}
