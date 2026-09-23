#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#  Copyright 2026 United Kingdom Research and Innovation
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#
#   Authored by:    Franck Vidal (UKRI-STFC)


################################################################################
#
#   @file       test_CIL.py
#
#   @brief      This script helps user check that the gVXR reader for CIL works
#               well to simulate CT data with gVXR, and reconstruct it with CIL.
#               If Matplotlib is installed, plots will be shown. In all cases,
#               data will be saved on the disk.
#
#   @version    1.0
#
#   @date       08/06/2026
#
#   @author     Prof Franck P. Vidal
#
#   License
#   Apache License.
#
#   For details on use and redistribution please refer
#   to http://opensource.org/licenses/BSD-3-Clause
#
#   Copyright
#   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk),
#   UK Research and Innovation, June 2026-present, version 1.1,
#   Apache License
#
################################################################################


################################################################################
# Import packages
################################################################################
import sys, os
import numpy as np
from math import sqrt

# Check if Matplotlib is installed
try:
    import matplotlib  # To plot images
    import matplotlib.pyplot as plt  # Plotting

    font = {
        "family": "serif",
        "size": 15,
    }
    matplotlib.rc("font", **font)

    # Uncomment the line below to use LaTeX fonts
    # matplotlib.rc('text', usetex=True)

    has_mpl = True
except:
    has_mpl = False
    print("Matplotlib is not installed; Plots won't be shown. It's OK, data will be saved on the disk.",
          file=sys.stderr)
# Check if CIL is installed
try:
    has_CIL = True

except:
    has_CIL = False
    print("CIL is not installed; this test cannot be executed. See https://github.com/TomographicImaging/CIL to install CIL on your system.",
          file=sys.stderr)

# Simulate X-ray images
from gvxrPython3 import gvxr

# CIL reader for gVXR
from gvxrPython3.gVXRDataReader import gVXRDataReader

# CT reconstruction
from cil.plugins.astra import FBP
from cil.processors import TransmissionAbsorptionConverter
from cil.io import TIFFWriter


# Check if we are running the test using pytest
is_running_pytest = "pytest" in sys.modules


def init_simulation_engine() -> None:
    """
    Create an OpenGL context and initialise the simulation engine.
    """

    # Get the mixed-precision flag to speed up calculations
    # 255 for full-precision
    # Something else for mixed-precision
    mixed_precision_flag = gvxr.computeMixedPrecisionFlag(
        True,  # aUncleanedLBufferFlag,
        False,  # aCleanedLBufferFlag,
        False,  # aCumulatedLBufferFlag,
        True,  # anEnergyFluenceFlag,
        False,  # aSumMuXdXFlag,
        True  # aZBufferFlag
    )

    # Create a simulation engine
    gvxr.createOpenGLContext(0, 4, 5, mixed_precision_flag)

    gvxr.setEnergyBinSize(2, "keV")
    gvxr.setBinTotalEnergyCutoff(1, "eV")

def create_output_directories(output_path: str) -> None:
    """
    Create the directories where to save:
    - the STL files,
    - the simulated radiographs, and
    - the reconstructed CT data.

    :param output_path: Path to the output directory.
    """

    # Create the directory were the STL files will be saved
    if not os.path.exists(output_path):
        os.makedirs(output_path)

    # Create the directory were the projections will be saved
    if not os.path.exists(os.path.join(output_path, "projs")):
        os.mkdir(os.path.join(output_path, "projs"))

    # Create the directory were the CT data will be saved
    if not os.path.exists(os.path.join(output_path, "CT")):
        os.mkdir(os.path.join(output_path, "CT"))


def test_CIL() -> None:

    # Make sure CIL is installed
    if has_CIL:

        # Create a simulation engine
        init_simulation_engine()

        # Create the directories where to save
        create_output_directories("test_CIL")

        # Set up a monochromatic source
        gvxr.setSourcePosition(-40.0, 0.0, 0.0, "cm")
        gvxr.useParallelBeam()

        # 1000 phothons of 80 keV (i.e. 0.08 MeV) per ray
        gvxr.setMonoChromatic(0.08, "MeV", 1000)
        gvxr.disablePoissonNoise()

        # Set up the detector
        gvxr.setDetectorPosition(10.0, 0.0, 0.0, "cm")
        gvxr.setDetectorUpVector(0, 0, -1)
        gvxr.setDetectorNumberOfPixels(640, 3)
        gvxr.setDetectorPixelSize(0.5, 0.5, "mm")

        # Create the sample
        sample_size_in_mm = 100
        gvxr.makeCube("sample", sample_size_in_mm, "mm")
        gvxr.setCompound("sample", "H2O")
        gvxr.setDensity("sample", 1.0, "g/cm3")
        gvxr.addPolygonMeshAsInnerSurface("sample")
        mu = gvxr.getLinearAttenuationCoefficient("sample", 80, "keV")

        if gvxr.getSourceShape() == "PARALLEL":
            last_angle = 180
        else:
            last_angle = 360

        # Only use 75% of the theoretical number of projections
        number_of_projections = round(gvxr.getOptimalNumberOfProjectionsCT() * 0.75)

        # Simulate the CT scan
        projection_path = os.path.join("test_CIL", "projs")
        recons_path = os.path.join("test_CIL", "CT")

        # Perform the actual simulations
        rotation_centre_in_mm = gvxr.getNodeAndChildrenBoundingBoxCentre("sample", "mm")
        rotation_axis = gvxr.getDetectorUpVector()

        gvxr.computeCTAcquisition(
            projection_path,  # The path where the X-ray projections will be saved
            "",  # The path where the screenshots will be saved
            number_of_projections,  # The total number of projections to simulate
            0,  # The rotation angle corresponding to the first projection
            False,  # A boolean flag to include or exclude the last angle
            last_angle,  # The rotation angle corresponding to the first projection
            50,  # The number of white images used to perform the flat-field correction
            *rotation_centre_in_mm,  # The location of the rotation centre
            "mm",  # The corresponding unit of length
            *rotation_axis,  # The rotation axis
            True,
            1
        )

        # Read the simulated data with CIL
        reader = gVXRDataReader(gvxr.getProjectionOutputPathCT(),
                                gvxr.getAngleSetCT(),
                                rotation_axis,
                                rotation_centre_in_mm)

        data = reader.read()
        data.reorder(order='astra')

        # Apply the minus log
        data_corr = TransmissionAbsorptionConverter(white_level=1.0)(data)

        # We only want to reconstruct the slice in the middle of the volume
        ig = data_corr.geometry.get_ImageGeometry();
        ig.voxel_num_z = 1

        # Perform the reconstruction with CIL
        fbp = FBP(ig, data_corr.geometry)
        fbp.set_input(data_corr)
        reconstruction = fbp.get_output()

        # Save the reconstructed CT images
        writer = TIFFWriter(data=reconstruction, file_name=recons_path)
        writer.write()

        if has_mpl and not is_running_pytest:
            plt.figure(figsize=(20,15))
            plt.title("CT reconstruction\nDo you see a white square on a black background?")
            plt.imshow(reconstruction.array, cmap="gray")
            plt.show()

        # Create the reference
        ref_image = np.zeros(reconstruction.array.shape, dtype=np.single)

        detector_width_in_mm = gvxr.getDetectorSize("mm")[0]
        offset_in_mm = (detector_width_in_mm - sample_size_in_mm) / 2.0

        offset_in_pixel = round(offset_in_mm / gvxr.getDetectorPixelSpacing("mm")[0])
        sample_size_in_pixel = round(sample_size_in_mm / gvxr.getDetectorPixelSpacing("mm")[0])

        ref_image[offset_in_pixel:sample_size_in_pixel+offset_in_pixel,offset_in_pixel:sample_size_in_pixel+offset_in_pixel] = mu / 10.0

        avg_true = np.mean(ref_image)
        stddev_true = np.std(ref_image)
        min_true = np.min(ref_image)
        max_true = np.max(ref_image)

        avg_test = np.mean(reconstruction.array)
        stddev_test = np.std(reconstruction.array)

        zncc = np.mean(((ref_image - avg_true) * (reconstruction.array - avg_test)) /
                       (stddev_true * stddev_test))
        nrmse = sqrt(np.mean(np.square(ref_image - reconstruction.array))) / (max_true - min_true)

        print("zncc = ", zncc)
        print("nrmse = ", nrmse)

        assert(zncc > 0.999)
        assert(nrmse < 1.0 / 100.0) # i.e. less than 1%

    # All done
    # Destroy the window/context
    gvxr.destroy()

if __name__ == "__main__":
    test_CIL()
    exit()