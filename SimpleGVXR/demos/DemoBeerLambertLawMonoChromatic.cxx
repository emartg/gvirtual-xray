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
*   @file       DemoBeerLambertLawMonoChromatic.cxx
*
*   @brief      .
*
*   @version    1.0
*
*   @date       01/07/2022
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
*               http://www.fpvidal.net/, Jul 2022, 2022, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifdef HAS_GLEW
#include <GL/glew.h>
#endif

#include <cmath>

#ifndef __SimpleGVXR_h
#include "SimpleGVXR.h"
#endif

#ifndef SGVXR_Internal_h
#include "internal.h"
#endif


//******************************************************************************
//  Global variables
//******************************************************************************
int g_main_window_width(1280 / 2.0);
int g_main_window_height(800 / 2.0);

double g_incident_energy_in_keV(80.0);
const double g_scaling_factor(2.0);
double g_resolution_in_mm = (0.15 * g_scaling_factor); // 1mm per pixel
int g_number_of_pixels[] = {int(600 / g_scaling_factor), int(600 / g_scaling_factor)};
double g_source_position_in_cm[]   = {10.0, 0.0, 0.0};
double g_detector_position_in_cm[] = {-10.0, 0.0, 0.0};
double g_detector_up_vector[]      = {0.0, 0.0,  1.0};

double g_literature_result_in_keV(0.0);
double g_expected_result_in_keV(0.0);


//******************************************************************************
//  Function declaration
//******************************************************************************
void loadSTLFile();
void loadDetector();
void loadSource();


//--------
int main()
//--------
{
    // Make sure the detector has an odd number of columns
    if ((g_number_of_pixels[0] % 2) == 0)
        {
        g_number_of_pixels[0] = g_number_of_pixels[0] + 1;
        }

    // Make sure the detector has an odd number of rows
    if ((g_number_of_pixels[1] % 2) == 0)
        {
        g_number_of_pixels[1] = g_number_of_pixels[1] + 1;
        }

    // Create a windowed mode window and its OpenGL context
/*#ifdef HAS_EGL
    createWindow(-1, 0, "OPENGL");
#else
    createWindow();
#endif
*/
    createWindow(-1, 1, 
        "EGL", 
        4, 6, 
        0); // Half-precision floating-point numbers is enough



    setWindowSize(g_main_window_width, g_main_window_height);

    // Load the data
    loadDetector();
    loadSource();

    // Add the geometry to the X-ray renderer
    loadSTLFile();
    addPolygonMeshAsInnerSurface("inside");
    addPolygonMeshAsOuterSurface("outside");

    // Enable artefact filtering on GPU
    enableArtefactFilteringOnGPU();

    // Compute the X-ray image
    std::vector<std::vector<float> > x_ray_image = computeXRayImage();

    renderLoop();
    destroyAllWindows();
}


//----------------
void loadSTLFile()
//----------------
{
    // Set the geometry
    makeCube("outside",
            3.0,
            "cm",
            "root");

    makeCylinder("inside",
            20,
            3.0,
            1.0,
            "cm",
            "root");

    // HU values
    double HU_bone(1329.885683);
    double HU_soft_tissue(51.714752);

    setHU( "inside", HU_bone);
    setHU("outside", HU_soft_tissue);
}


//-----------------
void loadDetector()
//-----------------
{
    setDetectorPixelSize(g_resolution_in_mm, g_resolution_in_mm , "mm");

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
    setMonoChromatic(g_incident_energy_in_keV, "keV", 1);

    // Set the source position
    usePointSource();
    setSourcePosition(g_source_position_in_cm[0],
        g_source_position_in_cm[1],
        g_source_position_in_cm[2],
        "cm");

    // Use a parallel source
    //useParallelBeam();
}
