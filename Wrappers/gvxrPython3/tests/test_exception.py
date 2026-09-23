#!/usr/bin/env python3

import numpy as np
import pytest
from gvxrPython3 import gvxr  # Simulate X-ray images


def test_out_of_bound_access() -> None:
    image = np.zeros((40, 40))
    with pytest.raises(Exception, match="Error: Out of bounds Memory Access."):
        gvxr.getImageRow(image, 41)


def test_invalid_rotation_axis() -> None:
    with pytest.raises(Exception, match="Error: Invalid rotation axis,"):
        gvxr.rotateModelView(90, 0, 0, 0)


def test_invalid_scaling_factors() -> None:
    with pytest.raises(Exception, match="Invalid scaling factors, all of them are null: 0, 0, 0"):
        gvxr.scaleScene(0, 0, 0)


def test_destroy() -> None:
    gvxr.destroy()


if __name__ == "__main__":
    test_out_of_bound_access()
    test_invalid_rotation_axis()
    test_invalid_scaling_factors()
    test_destroy()
