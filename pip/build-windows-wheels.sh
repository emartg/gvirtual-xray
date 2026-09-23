#!/usr/bin/bash

# 1. Install git https://git-scm.com/download/win
# 2. Install various versions of cpython from https://www.python.org/

rm -rf ../_skbuild

for PYTHON_VERSION in "310" "311" "312" "313" "314" "315";
#for PYTHON_VERSION in "310";
do
    PYTHON_EXE="C:\Program Files\Python$PYTHON_VERSION\python.exe"
    echo BUILD $PYTHON_EXE

    "$PYTHON_EXE"  -m pip install --upgrade pip #> /dev/null 2> /dev/null
    "$PYTHON_EXE"  -m pip install --upgrade cmake numpy build scikit-build ninja #> /dev/null 2> /dev/null
    "$PYTHON_EXE"  -m pip wheel .. #> /dev/null 2> /dev/null
done
