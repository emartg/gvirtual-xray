#!/usr/bin/bash

rm -f windows_test_results.dat

for PYTHON_EXE in "C:\Program Files\Python37\python.exe" "C:\Program Files\Python38\python.exe" "C:\Program Files (x86)\Microsoft Visual Studio\Shared\Python39_64\python.exe" "C:\Program Files\Python310\python.exe" "C:\Program Files\Python311\python.exe" "C:\Program Files\Python312\python.exe";
do
    
    echo TEST $PYTHON_EXE
    PYTHON_VER=`echo $PYTHON_EXE | sed 's/.*\\Python\(3[0-9]*\).*\\python.exe/\1/'`


    GVXR=`ls gvxr-2.0.7-cp$PYTHON_VER-cp$PYTHON_VER*-win_amd64.whl`
    #"$PYTHON_EXE" -m pip install numpy matplotlib > /dev/null 2> /dev/null
    "$PYTHON_EXE" -m pip install --force-reinstall $GVXR > /dev/null 2> /dev/null
    "$PYTHON_EXE" -m gvxrPython3.test > /dev/null 2> /dev/null

    echo "$PYTHON_EXE $GVXR $?" >> windows_test_results.dat
    echo
done
exit
