rm -f macos_test_results.dat

for i in {7..12};
do
    PYTHON_EXE="/usr/local/bin/python3.$i"
    echo TEST $PYTHON_EXE
    GVXR=`ls wheelhouse/repaired/gvxr-*-cp3$i-cp3$i*-macosx_*_x86_64.whl`
    $PYTHON_EXE -m pip install numpy matplotlib > /dev/null 2> /dev/null
    $PYTHON_EXE -m pip install --force-reinstall $GVXR > /dev/null 2> /dev/null
    $PYTHON_EXE -m gvxrPython3.test > /dev/null 2> /dev/null

    echo "$PYTHON_EXE $GVXR $?" >> macos_test_results.dat
done
exit
