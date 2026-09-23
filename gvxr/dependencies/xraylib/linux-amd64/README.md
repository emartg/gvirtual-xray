# How XRayLib was built

1. `git clone git@github.com:tschoonj/xraylib.git`
2. `cd xraylib`
3. `autoreconf -i`
6. `CFLAGS=-fPIC CXXFLAGS=-fPIC ./configure --enable-static --enable-shared --disable-all-bindings --enable-python`
7. `make`
8. `make install`
