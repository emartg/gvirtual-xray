/*

Copyright (c) 2026-present, Prof Franck P. Vidal (franck.vidal@stfc.ac.uk),
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
*   @file       DemoDragon.cxx
*
*   @brief      Use the dragon STL file.
*
*   @version    1.0
*
*   @date       10/03/2026
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
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, March 2026-present, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
//#include <string>
#include <vector>
//#include <iostream>
#include <chrono>
//#include <cmath>
//
#ifdef HAS_FILESYSTEM_HEADER
#include <filesystem>
#else
#include <experimental/filesystem>
#endif

#ifndef GVXR_LOGGER_H
#include "gVirtualXRay/Logger.h"
#endif

#ifndef SGVXR_SimpleGVXR_h
#include "SimpleGVXR.h"
#endif
//
//#ifndef SGVXR_Internal_h
//#include "internal.h"
//#endif


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
void createEnvironment(const std::string& aRenderer,
    unsigned int aMixedPrecisionFlag);


//******************************************************************************
//  Global variables
//******************************************************************************
const fs::path g_output_path("demo-dragon");
const size_t REPEAT_CALCULATIONS = 100;

//******************************************************************************
//  Function definitions
//******************************************************************************

//--------------------------------------------------------
void createEnvironment(const std::string& aRenderer,
                       unsigned int aMixedPrecisionFlag)
//--------------------------------------------------------
{
    // Create a simulation environment
    gVirtualXRay::LOGGER.logNow("Create a simulation environment") << std::endl;
    
    createWindow(-1,
        true,
        aRenderer,
        4, 6, aMixedPrecisionFlag);

    gVirtualXRay::LOGGER.logNow("OpenGL renderer:\t") << getOpenGlRenderer() << std::endl;
    gVirtualXRay::LOGGER.logNow("OpenGL version:\t") << getOpenGlVersion() << std::endl;
    gVirtualXRay::LOGGER.logNow("OpenGL vendor:\t") << getOpenGlVendor() << std::endl;
    gVirtualXRay::LOGGER.logNow("Shading language version:\t") << getShadingLanguageVersion() << std::endl;
    gVirtualXRay::LOGGER.logNow("Poisson noise implementation:\t") << getNoiseGenerator() << std::endl;

    // Set the simulation parameters

    // Set up the detector
    gVirtualXRay::LOGGER.logNow("Set up the detector");
    setDetectorPosition(40.0, 0.0, 0.0, "cm");
    setDetectorUpVector(0, 0, -1);

#ifdef _DEBUG
    setDetectorNumberOfPixels(512 / 8, 512 / 8);
    setDetectorPixelSize(500 * 12, 500 * 8, "um");
#else
    setDetectorNumberOfPixels(256, 256);
    setDetectorPixelSize(1000, 1000, "um");
#endif // _DEBUG


    // Set the impulse response of the detector, a convolution kernel
    std::vector<float> p_LSF = { 0.00110698, 0.00122599, 0.00136522, 0.00152954, 0.00172533, 0.00196116, 0.0022487, 0.00260419, 0.00305074, 0.00362216, 0.00436939, 0.00537209, 0.00676012, 0.0087564, 0.01176824, 0.01659933, 0.02499446, 0.04120158, 0.0767488, 0.15911699, 0.24774516, 0.15911699, 0.0767488, 0.04120158, 0.02499446, 0.01659933, 0.01176824, 0.0087564, 0.00676012, 0.00537209, 0.00436939, 0.00362216, 0.00305074, 0.00260419, 0.0022487, 0.00196116, 0.00172533, 0.00152954, 0.00136522, 0.00122599, 0.00110698 };
    setLSF(p_LSF);

    // Set the scintillator
    setScintillator("CsI", 500, "um");

    // Create a source
    gVirtualXRay::LOGGER.logNow("Set up the beam");
    setSourcePosition(-80.0, 0.0, 0.0, "cm");
    usePointSource();
    //  For a parallel source, use useParallelBeam();

    // Set its spectrum, here a monochromatic beam with 1000 photons of 80 keV(i.e. 0.08 MeV) per ray
    setMonoChromatic(15, "keV", 1000);
    // The following is equivalent : setMonoChromatic(80, "keV", 1000);

    // Or use a polychromatic beam:
    // - The tube voltage is 160 keV
    // - The inherent tube filtration is 0.5 mm of aluminium (Al)
    // - The filtration is 1 mm of tin (Sn)
    // - The anode angle is 12 degrees
    // - mAs is 0.5
    // - The source to detector distance in 50 cm
    // setVoltage(60, "kV",
    //     0.5, "mm", "Al");

    // addFilter("Sn", 1.0, "mm");

    // setmAs(0.5);

    // // Poisson noise will be enable
    // enablePoissonNoise(); // Not needed as mAs was used in the function call above








    // The sample
    std::string fname = "welsh-dragon-small.stl";

    removePolygonMeshesFromSceneGraph();
    loadMeshFile("Dragon", fname, "mm");
    moveToCentre("Dragon");
    applyCurrentLocalTransformation("Dragon");

    float best_sod = getBestSOD("Dragon", "mm");
    applySOD("Dragon", best_sod, "mm");

    // Change the sample's colour
    // By default the object is white, which is not always pretty.Let's change it to gold.
    float red = 1.0;
    float green = 215.0 / 255.0;
    float blue = 0.0;
    setColour("Dragon", red, green, blue, 1.0);

    // Material properties

    // Liquid water
    setCompound("Dragon", "H2O");
    setDensity("Dragon", 1.0, "g/cm3");
    // setDensity("Dragon", 1.0, "g.cm-3");


    // Change to a white background(it could be useful for putting screenshots in papers)
    setWindowBackGroundColour(1.0, 1.0, 1.0, -1);
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

        useLogFile((g_output_path / fs::path("demo-dragon-log.txt")).string());

        // We set the random seed so that the foam is always the same
        setSeed(1234567890);

        gVirtualXRay::LOGGER.logNow("gVXR:\t") << getVersionOfCoreGVXR() << std::endl;
        gVirtualXRay::LOGGER.logNow("SimpleGVXR:\t") << getVersionOfSimpleGVXR() << std::endl;

         //OpenGL 32 bits
        uint8_t full_precision_flag = computeMixedPrecisionFlag(true, // Uncleaned L-buffer
            true, // Cleaned L-buffer
            true, // Cumulated L-buffer
            true, // Energy fluence
            true, // Sum muX dX
            false); // Z-buffer
        // full_precision_flag = 32; will work too
        
         auto start_setup_opengl_32 = high_resolution_clock::now();
         createEnvironment("OPENGL", full_precision_flag);
         auto stop_setup_opengl_32 = high_resolution_clock::now();

         // Compute an X - ray image
         auto start_compute_opengl_32 = high_resolution_clock::now();
         std::vector<std::vector<float> > x_ray_image_32 = computeXRayImage();//).astype(np.single) / getWhiteImage();
         for (size_t i = 1; i < REPEAT_CALCULATIONS; ++i)
         {
             x_ray_image_32 = computeXRayImage();
         }
         auto stop_compute_opengl_32 = high_resolution_clock::now();

         saveLastXRayImage((g_output_path / fs::path("xray_image-opengl-32.tif")).string());
         saveLastLBuffer((g_output_path / fs::path("lbuffer-opengl-32.mhd")).string());

         // Interactive visualisation
         // The user can rotate the 3D scene and zoom - in and -out in the visualisation window.
         // It can be useful to rotate the visualisation of the 3D environment and zoom in / out
         // to take the best posible screenshots

         // - Keys are:
         //     - Q/Escape: to quit the event loop (does not close the window)
         //     - B: display/hide the X-ray beam
         //     - W: display the polygon meshes in solid or wireframe
         //     - N: display the X-ray image in negative or positive
         //     - H: display/hide the X-ray detector
         // - Mouse interactions:
         //     - Zoom in/out: mouse wheel
         //     - Rotation: Right mouse button down + move cursor```
         renderLoop();

         destroy();

         // OpenGL 16 bits
         uint8_t half_precision_flag = computeMixedPrecisionFlag(false, // Uncleaned L-buffer
             false, // Cleaned L-buffer
             false, // Cumulated L-buffer
             false, // Energy fluence
             false, // Sum muX dX
             true); // Z-buffer
         // half_precision_flag = 0; will work too

         auto start_setup_opengl_16 = high_resolution_clock::now();
         createEnvironment("OPENGL", half_precision_flag);
         auto stop_setup_opengl_16 = high_resolution_clock::now();

         // Compute an X - ray image
         auto start_compute_opengl_16 = high_resolution_clock::now();
         std::vector<std::vector<float> > x_ray_image_16 = computeXRayImage();//).astype(np.single) / getWhiteImage();
         for (size_t i = 1; i < REPEAT_CALCULATIONS; ++i)
         {
             x_ray_image_16 = computeXRayImage();
         }
         auto stop_compute_opengl_16 = high_resolution_clock::now();

         saveLastXRayImage((g_output_path / fs::path("xray_image-opengl-16.tif")).string());
         saveLastLBuffer((g_output_path / fs::path("lbuffer-opengl-16.mhd")).string());

         // Interactive visualisation
         renderLoop();

         destroy();

         // OpenGL mixed precision
         uint8_t mixed_precision_flag = computeMixedPrecisionFlag(true, // Uncleaned L-buffer
             false, // Cleaned L-buffer
             false, // Cumulated L-buffer
             true, // Energy fluence
             false, // Sum muX dX
             false); // Z-buffer

         auto start_setup_opengl_mixed = high_resolution_clock::now();
         createEnvironment("OPENGL", mixed_precision_flag);
         auto stop_setup_opengl_mixed = high_resolution_clock::now();

         // Compute an X - ray image
         auto start_compute_opengl_mixed = high_resolution_clock::now();
         std::vector<std::vector<float> > x_ray_image_mixed = computeXRayImage();//).astype(np.single) / getWhiteImage();
         for (size_t i = 1; i < REPEAT_CALCULATIONS; ++i)
         {
             x_ray_image_mixed = computeXRayImage();
         }
         auto stop_compute_opengl_mixed = high_resolution_clock::now();

         saveLastXRayImage((g_output_path / fs::path("xray_image-opengl-mixed.tif")).string());
         saveLastLBuffer((g_output_path / fs::path("lbuffer-opengl-mixed.mhd")).string());

         // Interactive visualisation
         renderLoop();
         
         destroy();

        //// OpenMP
        //auto start_setup_openmp = high_resolution_clock::now();
        //createEnvironment("OPENMP", 0);
        //auto stop_setup_openmp = high_resolution_clock::now();

        //// Compute an X - ray image
        //auto start_compute_openmp = high_resolution_clock::now();
        //std::vector<std::vector<float> > x_ray_image_openmp = computeXRayImage();//).astype(np.single) / getWhiteImage();
        //auto stop_compute_openmp = high_resolution_clock::now();

        //saveLastXRayImage((g_output_path / fs::path("xray_image-openmp.tif")).string());
        //saveLastLBuffer((g_output_path / fs::path("lbuffer-openmp.mhd")).string());

        //destroy();


         gVirtualXRay::LOGGER.logNow("OpenGL 32 bits:\t") << std::endl;
         gVirtualXRay::LOGGER.logNow("\tSetting up:\t") << duration_cast<milliseconds>(stop_setup_opengl_32 - start_setup_opengl_32).count() << std::endl;
         gVirtualXRay::LOGGER.logNow("\tCompute:\t") << duration_cast<milliseconds>(stop_compute_opengl_32 - start_compute_opengl_32).count() / REPEAT_CALCULATIONS << std::endl;

         gVirtualXRay::LOGGER.logNow("OpenGL 16 bits:\t") << std::endl;
         gVirtualXRay::LOGGER.logNow("\tSetting up:\t") << duration_cast<milliseconds>(stop_setup_opengl_16 - start_setup_opengl_16).count() << std::endl;
         gVirtualXRay::LOGGER.logNow("\tCompute:\t") << duration_cast<milliseconds>(stop_compute_opengl_16 - start_compute_opengl_16).count() / REPEAT_CALCULATIONS << std::endl;

         gVirtualXRay::LOGGER.logNow("OpenGL mixed precision:\t") << std::endl;
         gVirtualXRay::LOGGER.logNow("\tSetting up:\t") << duration_cast<milliseconds>(stop_setup_opengl_mixed - start_setup_opengl_mixed).count() << std::endl;
         gVirtualXRay::LOGGER.logNow("\tCompute:\t") << duration_cast<milliseconds>(stop_compute_opengl_mixed - start_compute_opengl_mixed).count() / REPEAT_CALCULATIONS << std::endl;

        //gVirtualXRay::LOGGER.logNow("OpenMP:\t") << std::endl;
        //gVirtualXRay::LOGGER.logNow("\tSetting up:\t") << duration_cast<milliseconds>(stop_setup_openmp - start_setup_openmp).count() << std::endl;
        //gVirtualXRay::LOGGER.logNow("\tCompute:\t") << duration_cast<milliseconds>(stop_compute_openmp - start_compute_openmp).count() << std::endl;

        return 0;
    }
    catch (std::exception& e) {
        gVirtualXRay::LOGGER.logError("") << e.what() << std::endl;
    }
}
