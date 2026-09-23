#!/usr/bin/env bash

#set -e -u -x


repair_wheel() {
    wheel="$1"
    if ! auditwheel show "$wheel"; then
        echo "Skipping non-platform wheel $wheel"
    else
        #$CPYTHON_PATH/bin/auditwheel repair "$wheel" --plat manylinux_2_28_x86_64 -w /io/pip/wheelhouse/repaired
        # shellcheck disable=SC3010
        if [[ "$AUDITWHEEL_PLAT" == *"manylinux2014_"* ]]; then
            echo "Use Platform: manylinux2014"
            auditwheel repair "$wheel"  --plat $AUDITWHEEL_PLAT --exclude libOpenGL.so,libGLX.so -w /io/pip/wheelhouse/repaired
        elif [[ "$AUDITWHEEL_PLAT" == *"manylinux_2_34_"* ]]; then
            auditwheel repair "$wheel"  --plat $AUDITWHEEL_PLAT --exclude libOpenGL.so* --exclude libxcb*.so* --exclude libGLX.so* -w /io/pip/wheelhouse/repaired
#            auditwheel repair "$wheel"  --plat $AUDITWHEEL_PLAT --exclude libOpenGL.so* --exclude libGLX.so* -w /io/pip/wheelhouse/repaired
            echo "Use Platform: manylinux_2_34"
        else
            echo "Use Platform: UNSUPPORTED"
            exit 1
       fi
    fi
}




# Create the output directory if needed
if [ ! -d /io/pip/wheelhouse/repaired ]
then
    mkdir -p /io/pip/wheelhouse/repaired
fi

# Delete the old builds
rm -rf /io/_skbuild/
rm -rf /io/Wrappers/gVXR.egg-info
#rm -rf /io/pip/wheelhouse/gvxr-*.*.*-*-*-linux_x86_64.whl
#rm -rf /io/pip/wheelhouse/repaired/gvxr-*.*.*-*-*-manylinux*.manylinux*_x86_64.whl

if [[ "$AUDITWHEEL_PLAT" == *"manylinux2014_"* ]]; then
    export CMAKE_CXX_COMPILER=/opt/rh/devtoolset-10/root/usr/bin/g++
    export CMAKE_C_COMPILER=/opt/rh/devtoolset-10/root/usr/bin/gcc
fi

# Create all the wheels
#for PYSUBDIR in "cp311-cp311" ; do
for PYSUBDIR in `manylinux-interpreters list --installed `; do

    if [ $PYSUBDIR != "cp39-cp39" ];
    then
        PYDIR=/opt/python/$PYSUBDIR
        #rm -rf /io/_skbuild
        "${PYDIR}/bin/pip" install --upgrade --user pip
        "${PYDIR}/bin/pip" install --user -r /io/pip/dev-requirements.txt

        export Python_ROOT_DIR="${PYDIR}"
        export PYTHON_INCLUDE_DIRS="${PYDIR}/include"
        export Python_EXECUTABLE="${PYDIR}/bin/python"
        # export PYTHON_LIBRARIES='/usr/lib64/libpython3.so'
        # export PYTHON_LIBRARIES=`${PYDIR}/bin/python3-config --libs`
        export PYTHON_LIBRARY=$PYTHON_LIBRARIES

        # rm -f /usr/local/bin/cmake
        # ln -s "${PYDIR}/bin/cmake" /usr/local/bin/cmake

        # Enable parallel build
        MAKEFLAGS="-j $(nproc)" \
        PYTHON_LIBRARY=$PYTHON_LIBRARIES \
        PYTHON_LIBRARIES=$PYTHON_LIBRARIES \
        Python_ROOT_DIR=$Python_ROOT_DIR \
        PYTHON_INCLUDE_DIRS=$PYTHON_INCLUDE_DIRS \
        Python_EXECUTABLE=$Python_EXECUTABLE \
        "${PYDIR}/bin/pip" wheel  /io/ --no-clean --no-deps -w /io/pip/wheelhouse/

        if [ $? -ne 0 ]; then
          echo "FATAL ERROR: Could not build a Python wheel for $PYSUBDIR"
          echo "Abort..."
          exit
        fi
    fi
done

CPYTHON_PATH="/opt/_internal/`ls /opt/_internal/ | grep cpython-3.11`"

if [[ "$AUDITWHEEL_PLAT" == *"manylinux2014_"* ]]; then
    $CPYTHON_PATH/bin/python3 -m pip install --user git+https://github.com/rossant/auditwheel.git@include-exclude
elif [[ "$AUDITWHEEL_PLAT" == *"manylinux_2_34_"* ]]; then
    $CPYTHON_PATH/bin/python3 -m pip install auditwheel
fi

# Bundle external shared libraries into the wheels
for whl in /io/pip/wheelhouse/*$(uname -m).whl; do
    repair_wheel "$whl"
done

$CPYTHON_PATH/bin/python3 -m pip install twine
$CPYTHON_PATH/bin/python3 -m twine check /io/pip/wheelhouse/repaired/*.whl

# Stopped working
# Make the source distribution
#$CPYTHON_PATH/bin/python3 -m pip install --upgrade build
#$CPYTHON_PATH/bin/python3 -m build /io/
