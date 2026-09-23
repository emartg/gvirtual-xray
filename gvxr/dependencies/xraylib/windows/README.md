# How XRayLib was built

## On Linux

### MXE (M cross environment)

1. `cd /usr/local/share/`
2. `sudo git clone https://github.com/mxe/mxe.git`
3. `cd mxe`
4. `sudo make -j16 cc MXE_TARGETS='x86_64-w64-mingw32.static x86_64-w64-mingw32.shared'`

### xraylib

1. `cd $HOME`
2. `git clone git@github.com:tschoonj/xraylib.git`
3. `cd xraylib`
4. `autoreconf -i`
5. `export PATH=/usr/local/share/mxe/usr/bin:$PATH`
6. `CC=/usr/local/share/mxe/usr/bin/x86_64-w64-mingw32.static-gcc CXX=/usr/local/share/mxe/usr/bin/x86_64-w64-mingw32.static-g++ ./configure --host=x86_64-w64-mingw32.static --disable-static --enable-shared --enable-output-def`
7. `make`
8. `cp ./src/libxrl-11.def ./src/.libs/libxrl-11.dll windows`

### On Windows

1. From the Visual Studio Command Prompt, execute another command:

`lib /def:libxrl-11.def /out:libxrl-11.lib /machine:x86_64`
