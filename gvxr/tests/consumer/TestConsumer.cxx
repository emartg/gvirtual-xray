/**
********************************************************************************
*
*   @file       TestConsumer.cxx
*
*   @brief      Consumer test: use the core library of gVirtualXRay from another project. It needs
*               the headers and the preprocessor definitions that the target exports, and it links to
*               the library and to its dependencies. No OpenGL context is needed.
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <iostream>
#include <map>
#include <string>

#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#ifndef GVXR_MIXTURE_H
#include "gVirtualXRay/Mixture.h"
#endif


#ifdef HAS_GLEW
// The version of the GLEW that the target links (GlewVersion.cxx)
const char* getGlewVersion();
#endif


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;


//----------
int main()
//----------
{
    // Hydrogen, helium, lithium, and beryllium with the weights of TestMixture
    std::map<int, double> weight_set;

    weight_set[1] = 1.0;
    weight_set[2] = 2.0;
    weight_set[3] = 1.0;
    weight_set[4] = 2.0;

    Mixture mixture(weight_set);

    if (mixture.getLabel() != "H17He33Li17Be33")
    {
        std::cerr << "[ERROR] Unexpected label of the mixture: " << mixture.getLabel() << std::endl;
        return 1;
    }

    if (!(mixture.getMolarMass() > 0.0))
    {
        std::cerr << "[ERROR] The molar mass of the mixture is not positive" << std::endl;
        return 1;
    }

#ifdef HAS_GLEW
    // The GLEW that the target links (static, or shared with GLEW_USE_STATIC_LIBS OFF): its version needs no OpenGL context
    const char* p_glew_version = getGlewVersion();

    if (!p_glew_version)
    {
        std::cerr << "[ERROR] GLEW gives no version" << std::endl;
        return 1;
    }

    std::cout << "GLEW " << p_glew_version << std::endl;
#endif

    std::cout << "gVirtualXRay " << gVirtualXRay_VERSION_MAJOR << "." << gVirtualXRay_VERSION_MINOR << "."
              << gVirtualXRay_VERSION_PATCH << ": mixture " << mixture.getLabel() << ", molar mass "
              << mixture.getMolarMass() << std::endl;

    return 0;
}
