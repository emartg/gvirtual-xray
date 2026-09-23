#!/usr/bin/env python3
import os
# Copyright (c) 2025-present, Prof Franck P. Vidal (franck.vidal@stfc.ac.uk),
# UK Research and Innovation, All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
# this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation and/or
# other materials provided with the distribution.
#
# 3. Neither the name of UK Research and Innovation nor the names of its contributors
# may be used to endorse or promote products derived from this software without
# specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


################################################################################
#
#   @file       benchmark.py
#
#   @brief      Compare the use of various precisions for floating point numbers on GPU.
#               We try 32 bits (full-precision), 16 bits (half-precision), and
#               a combination of both (mixed-precision).
#               We use images and meshes of increasing sizes.
#               Our findings show that half-precision and mixed-precision are
#               twice as fast as full-precision. There is a significant loss
#               of image quality for half-precision, but not for mixed-precision.
#               Noise is executed on the CPU, and this is the main bottleneck.
#
#   @version    1.0
#
#   @date       23/09/2025
#
#   @author     Prof Franck P. Vidal
#
#   License
#   BSD 3-Clause License.
#
#   For details on use and redistribution please refer
#   to http://opensource.org/licenses/BSD-3-Clause
#
#   Copyright
#   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, Sept 2025-present, version 1.1, BSD 3-Clause License
#
################################################################################


################################################################################
# Import packages
################################################################################
import sys
import os
from math import sqrt
import time
import numpy as np
import pandas as pd
from skimage.metrics import structural_similarity as ssim
from gvxrPython3 import gvxr
from codecarbon import EmissionsTracker


################################################################################
# Global variables
################################################################################

g_output_path = "benchmark-results";

g_volume_resolution_set = [15, 32, 64, 128];
g_detector_resolution_scaling_factor_set = [0.75, 1.0, 1.25, 2, 3, 4, 5];

# g_volume_resolution_set = [15, 32];
# g_detector_resolution_scaling_factor_set = [0.75, 1.0];

g_total_runs = 150;

g_OpenGL_renderer = "";
g_OpenGL_version = "";
g_OpenGL_vendor = "";
g_shading_language_version = "";


################################################################################
# Function declarations/definitions
################################################################################
def getMatrixLabel(i:int)->str:
    return "matrix-" + str(i) + "x" + str(i) + "x" + str(i)

def getFoamLabel(i:int)->str:
    return "foam-" + str(i) + "x" + str(i) + "x" + str(i)

def getParticleLabel(i:int)->str:
    return "particles-" + str(i) + "x" + str(i) + "x" + str(i)

def setDetector(aScalingFactor: float):
    gvxr.setDetectorPosition(10.0, 0.0, 0.0, "cm");
    gvxr.setDetectorUpVector(0, 0, -1);
    gvxr.setDetectorPixelSize(0.5 / aScalingFactor, 0.5 / aScalingFactor, "mm");
    gvxr.setDetectorNumberOfPixels(round(640 * aScalingFactor), round(320 * aScalingFactor));


def setSource():
    gvxr.setSourcePosition(-40.0,  0.0, 0.0, "cm");
    gvxr.usePointSource();
    gvxr.setVoltage(80, "kV");
    gvxr.addFilter("Cu", 1, "mm");
    gvxr.setmAs(0.5);


def createSTL():
    for ite in g_volume_resolution_set:

        particle_label = getParticleLabel(ite);
        matrix_label = getMatrixLabel(ite);
        foam_label = getFoamLabel(ite);

        voxel_size = 5.0 / ite;
        gvxr.makeFoamInCuboid(particle_label,
            "",
            5, 5, 5, # 5mm in total
            0.25,
            50,
            50,
            voxel_size, # voxel size:
            "mm",
            1);

        gvxr.emptyMesh(foam_label);
        gvxr.createMeshFromImplicitSurface(particle_label, particle_label, 0.05, foam_label, 1);
        gvxr.createMeshFromImplicitSurface(particle_label, matrix_label, -10.0, 0.05, foam_label, 1);

        gvxr.saveSTLfile(particle_label, os.path.join(g_output_path, particle_label + ".stl"));
        gvxr.saveSTLfile(matrix_label, os.path.join(g_output_path, matrix_label + ".stl"));


def createEnvironment(aScalingFactor: float,
                      aNumberOfBitsPerFloat: int,
                      aMeshResolution: int,
                      aUseNoiseFlag: bool):

    # Global variables
    global g_OpenGL_renderer, g_OpenGL_version, g_OpenGL_vendor, g_shading_language_version

    # Create a simulation engine
    gvxr.createOpenGLContext(0,
                             4, 6,
                             aNumberOfBitsPerFloat);

    g_OpenGL_renderer = gvxr.getOpenGlRenderer();
    g_OpenGL_version = gvxr.getOpenGlVersion();
    g_OpenGL_vendor = gvxr.getOpenGlVendor();
    g_shading_language_version = gvxr.getShadingLanguageVersion();

    setDetector(aScalingFactor);
    setSource();

    foam_label = getFoamLabel(aMeshResolution);
    matrix_label = getMatrixLabel(aMeshResolution);
    particle_label = getParticleLabel(aMeshResolution);

    gvxr.removePolygonMeshesFromSceneGraph();
    gvxr.emptyMesh(foam_label);

    gvxr.loadMeshFile(particle_label,
        os.path.join(g_output_path, particle_label + ".stl"),
        "mm",
        True,
        foam_label);

    gvxr.loadMeshFile(matrix_label,
        os.path.join(g_output_path, matrix_label + ".stl"),
        "mm",
        True,
        foam_label);

    # Set the material composition

    # AlSi10Mg alloy
    gvxr.setColor(matrix_label, 136 / 255,139 / 255,141 / 255, 0.5);
    gvxr.setMixture(matrix_label, ["Al", "Si", "Mg"], [100 - 9.0 - 0.25, 9.0, 0.25]);
    gvxr.setDensity(matrix_label, 2.67, "g/cm3");

    # Silicon carbide
    gvxr.setColor(particle_label, 136 / 255,139 / 255,141 / 255, 0.5);
    gvxr.setCompound(particle_label, "SiC");
    gvxr.setDensity(particle_label, 3.21, "g/cm3");

    best_sod = gvxr.getBestSOD(foam_label, "mm");
    gvxr.applySOD(foam_label, best_sod, "mm");

    if aUseNoiseFlag:
        gvxr.enablePoissonNoise();
    else:
        gvxr.disablePoissonNoise();


def computeRMSE(reference_image, test_image, normalise) -> float:
    mse = np.mean(np.square(reference_image.flatten() - test_image.flatten()));
    rmse = sqrt(mse);

    if normalise:
        rmse /= reference_image.flatten().mean();

    return rmse;

def computeZNCC(reference_image, test_image) -> float:
    tmp1 = reference_image.flatten()
    tmp1 -= tmp1.mean()
    tmp1 /= tmp1.std()

    tmp2 = test_image.flatten()
    tmp2 -= tmp2.mean()
    tmp2 /= tmp2.std()

    return np.mean(np.multiply(tmp1, tmp2))

def  main():
    try:
        # Check if g_output_path directory exists
        if not os.path.isdir(g_output_path) or not os.path.exists(g_output_path):
            # Create the output directory if needed
            os.makedirs(g_output_path);

        gvxr.useLogFile(os.path.join(g_output_path, "log.txt"));

        # We set the random seed so that the foam is always the same
        gvxr.setSeed(1234567890);
        createSTL();

        p_result = [];

        p_noise_set = [False, True];
        p_noise_set = [True];
        # p_noise_set = [False];

        precision_set = [32, 16, 0] # 32 must be the first item

        for use_noise_ite in p_noise_set:

            for number_of_bits_per_float in precision_set:

                tracker = EmissionsTracker()
                tracker.start()

                if number_of_bits_per_float == 32:
                    reference_duration = {}
                    reference_image = {}

                for volume_resolution_ite in g_volume_resolution_set:

                    if number_of_bits_per_float == 32:
                        reference_duration[volume_resolution_ite] = {}
                        reference_image[volume_resolution_ite] = {}

                    for detector_resolution_scaling_factor_ite in g_detector_resolution_scaling_factor_set:

                        createEnvironment(detector_resolution_scaling_factor_ite,
                            number_of_bits_per_float,
                            volume_resolution_ite,
                            use_noise_ite);

                        cols = gvxr.getDetectorNumberOfPixels()[0];
                        rows = gvxr.getDetectorNumberOfPixels()[1];

                        p_x_ray_image = np.zeros((rows, cols), dtype=np.single);
                        gvxr.computeXRayImage(p_x_ray_image);

                        start_performance_counter = time.perf_counter();
                        for i in range(g_total_runs):
                            gvxr.computeXRayImage(p_x_ray_image);
                        stop_performance_counter = time.perf_counter();

                        matrix_label = getMatrixLabel(volume_resolution_ite);
                        particle_label = getParticleLabel(volume_resolution_ite);

                        number_of_triangles = gvxr.getNumberOfPrimitives(matrix_label) + gvxr.getNumberOfPrimitives(
                            particle_label);
                        number_of_vertices = gvxr.getVertexNumber(matrix_label) + gvxr.getVertexNumber(particle_label);

                        gvxr.destroy();

                        duration = 1e3 * (stop_performance_counter - start_performance_counter);

                        if number_of_bits_per_float == 32:
                            reference_duration[volume_resolution_ite][detector_resolution_scaling_factor_ite] = duration;
                            reference_image[volume_resolution_ite][detector_resolution_scaling_factor_ite] = np.copy(p_x_ray_image)

                        duration_fp32 = reference_duration[volume_resolution_ite][detector_resolution_scaling_factor_ite]
                        p_x_ray_image_fp32 = reference_image[volume_resolution_ite][detector_resolution_scaling_factor_ite]

                        NRMSE = computeRMSE(p_x_ray_image_fp32, p_x_ray_image, True);
                        ZNCC = computeZNCC(p_x_ray_image_fp32, p_x_ray_image);
                        data_range=p_x_ray_image_fp32.max() - p_x_ray_image_fp32.min()
                        SSIM = ssim(p_x_ray_image_fp32, p_x_ray_image, data_range=data_range);

                        p_result.append(use_noise_ite);
                        p_result.append(number_of_bits_per_float);
                        p_result.append(cols);
                        p_result.append(rows);
                        p_result.append(number_of_triangles);
                        p_result.append(number_of_vertices);
                        p_result.append(duration_fp32 / g_total_runs);
                        p_result.append(g_total_runs / (1e-3 * duration));
                        p_result.append(duration_fp32 / duration);
                        p_result.append(NRMSE);
                        p_result.append(ZNCC);
                        p_result.append(SSIM);

                tracker.stop()

        number_of_cols = int(len(p_result) /
                          (
                              len(g_volume_resolution_set) * len(g_detector_resolution_scaling_factor_set) *
                              len(precision_set) * len(p_noise_set))
                          );
        print(number_of_cols)
        number_of_rows = int(len(p_result) / number_of_cols);

        print("gVXR:\t", gvxr.getVersionOfCoreGVXR());
        print("SimpleGVXR:\t", gvxr.getVersionOfSimpleGVXR());
        print("OpenGL renderer:\t", g_OpenGL_renderer);
        print("OpenGL version:\t", g_OpenGL_version);
        print("OpenGL vendor:\t", g_OpenGL_vendor);
        print("Shading language version:\t", g_shading_language_version);
        print("Poisson noise implementation:\t", gvxr.getNoiseGenerator());

        columns = [
            "Noise",
            "Precision",
            "Cols",
            "Rows",
            "Triangles",
            "Vertices",
            "Avg (millisec)",
            "FPS",
            "Speedup",
            "NRMSE",
            "ZNCC",
            "SSIM"
        ]

        print("Noise\tPrecision\tCols\tRows\tTriangles\tVertices\tAvg (millisec)\tFPS\tSpeedup\tNRMSE\tZNCC\tSSIM");

        for i in range(number_of_rows):
            for j in range(number_of_cols):

                value = p_result[i * number_of_cols + j];

                if j == 0:
                    if bool(round(value)):
                        print("Yes", "\t", sep='', end='');
                    else:
                        print("No", "\t", sep='', end='');

                elif j == 1 and value < 15:
                    print("mixed", "\t", sep='', end='');
                else:
                    if j == number_of_cols - 5:
                        print(round(value), "\t", sep='', end='');
                    else:
                        print(value, "\t", sep='', end='');

            print();

        p_result = np.asarray(p_result)
        p_result.shape = [number_of_rows, number_of_cols]
        df = pd.DataFrame(data=np.asarray(p_result), columns=columns)
        df.to_csv(os.path.join(g_output_path, "benchmark.csv"), encoding='utf-8', index=False)

    except Exception as e:
        print(f"{type(e).__name__} at line {e.__traceback__.tb_lineno} of {__file__}: {e}")
        return 1
    else:
        # emissions_fp32: float = tracker_fp32.stop()
        # emissions_fp16: float = tracker_fp16.stop()
        # emissions_fp_mixed: float = tracker_fp_mixed.stop()
        # print(emissions_fp32)
        # print(emissions_fp16)
        # print(emissions_fp_mixed)
        return 0


if __name__ == '__main__':
    sys.exit(main())


