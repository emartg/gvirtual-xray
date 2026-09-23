#!/bin/sh

#set -e -u -x

function repair_wheel {
    wheel="$1"
    if ! auditwheel show "$wheel"; then
        echo "Skipping non-platform wheel $wheel"
    else
        echo Repair wheel for MacOS
        #/opt/_internal/cpython-3.9.13/bin/auditwheel repair "$wheel"  --plat manylinux_2_17_x86_64 --exclude libOpenGL.so,libGLX.so -w ./wheelhouse/repaired
    fi
}

# Create the output directory if needed
if [ ! -d wheelhouse/repaired ]
then
    mkdir -p wheelhouse/repaired
fi

# Enable parallel build
export MAKEFLAGS="-j$(sysctl -n hw.physicalcpu)"

# Delete the old builds
rm -rf ../Wrappers/gVXR.egg-info
rm -rf wheelhouse/gVXR-*.*.*-*-*-macosx_*_*_x86_64.whl
rm -rf wheelhouse/repaired/gVXR-*.*.*-*-*-macosx_*_*_x86_64.whl

# Create all the wheels
for PYTHON_EXE in /usr/local/bin/python3.*; do
    rm -rf ../_skbuild
    "${PYTHON_EXE}" -m pip install --upgrade pip
    # "${PYTHON_EXE}" -m pip uninstall -y auditwheel
    "${PYTHON_EXE}" -m pip install -r ./dev-requirements.txt
    PYTHON_EXECUTABLE="${PYTHON_EXE}" "${PYTHON_EXE}" -m pip wheel  ../ --no-deps -w ./wheelhouse/
done

#/opt/python/cp39-cp39/bin/python3 -m pip uninstall -y auditwheel
#/opt/python/cp39-cp39/bin/python3 -m pip install git+https://github.com/rossant/auditwheel.git@include-exclude
pip3 install delocate


# Bundle external shared libraries into the wheels
for whl in ./wheelhouse/gvxr-*.*.*-*-*-macosx_*_*_x86_64.whl; do
   delocate-wheel -w ./wheelhouse/repaired/ -v "$whl"
done

#/opt/python/cp39-cp39/bin/python3 -m twine check ./wheelhouse/repaired/*.whl
