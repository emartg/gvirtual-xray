mcs  -out:test.exe test.cs  gvxrCSharp.cs Vectori.cs Vectorf.cs Vectorff.cs Vectord.cs gvxrCSharpPINVOKE.cs

if [ ! -f gvxrCSharp ]; then
    echo Create link
    ln -s gvxrCSharp.so gvxrCSharp
fi

LD_LIBRARY_PATH=. mono test.exe
