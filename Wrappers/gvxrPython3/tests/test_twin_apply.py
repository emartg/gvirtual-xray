#!/usr/bin/env python3

import os

import numpy as np
import pytest
from gvxrPython3 import gvxr
from twins import DigitalTwin


def test_create_environment() -> None:
    # Create an OpenGL context
    # MS Windows
    if os.name == "nt":
        print("Creating an OpenGL context using EGL is not possible on Microsoft Windows")
        print("A window will be created but not displayed")
        gvxr.createWindow(0, False, "OPENGL")
    # MacOS
    elif str(os.uname()).find("Darwin") >= 0:
        print("Creating an OpenGL context using EGL is not possible on MacOS")
        print("A window will be created but not displayed")
        gvxr.createWindow(0, False, "OPENGL")
    # GNU/Linux
    else:
        print("Create an OpenGL context using EGL")
        gvxr.createWindow(0, 1, "EGL")


def test_twin_diad_apply() -> None:
    # Simple diad twin test
    diad: DigitalTwin = DigitalTwin.from_file("twins/models/diad.json5")

    diad.beam.keV = 20
    diad.detector.exposure = 0.5

    diad.apply()


def test_twin_diad_apply_invalid() -> None:
    # Simple diad twin test
    diad = DigitalTwin.from_file("twins/models/diad.json5")

    diad.beam.keV = 180
    diad.detector.exposure = -10

    with pytest.raises(ValueError, match="Incorrect Twin Settings"):
        diad.apply()


def test_twin_diad_compute() -> None:
    diad = DigitalTwin.from_file("twins/models/diad.json5")

    diad.beam.keV = 20
    diad.detector.exposure = 0.5
    diad.apply()

    # Create 1cm carbon cube
    gvxr.makeCube("cube", 10, "mm")
    gvxr.setElement("cube", "C")

    gvxr.addPolygonMeshAsInnerSurface("cube")

    image = np.asarray(gvxr.computeXRayImage())

    assert image.mean() > 0


if __name__ == "__main__":
    test_create_environment()
    test_twin_diad_apply()
    test_twin_diad_apply_invalid()
    test_twin_diad_compute()
