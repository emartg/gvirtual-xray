#!/bin/bash

gedit VERSION.txt \
    gvxr/cmake/version.cmake \
    SimpleGVXR/version.cmake \
    Wrappers/cmake/version.cmake \
    Wrappers/gvxrPython3._version.py \
    setup.py \
    pyproject.toml \
    CHANGELOG \
    CMakeLists.txt

