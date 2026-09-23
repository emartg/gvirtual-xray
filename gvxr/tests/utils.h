#ifndef __utils_h
#define __utils_h

#ifndef __Context_h
#include "gVirtualXRay/Context.h"
#endif


void initGL(gVirtualXRay::Context& aGLContext, int aMajorOpenGLVersion = 4, int aMinorOpenGLVersion = 3);

#include "utils.inl"

#endif