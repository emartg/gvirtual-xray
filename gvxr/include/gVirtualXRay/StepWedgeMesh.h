/*

Copyright (c) 2024-present, Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), 
UK Research and Innovation, All rights reserved.
Copyright (c) 2024-present, Mr Nalin Gupta, UK Research and Innovation, 
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

3. Neither the name of the Bangor University nor the names of its contributors
may be used to endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/


#ifndef GVXR_STEP_WEDGE_MESH_H
#define GVXR_STEP_WEDGE_MESH_H


/**
********************************************************************************
*
*   @file       StepWedgeMesh.h
*
*   @brief      Class to handle 3D meshes of step wedges.
*
*   @version    1.0
*
*   @date       10/07/2024
*
*   @author     Prof Franck P. Vidal
*   @author     Mr Nalin Gupta
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, Jul 2024-present, version 1.1, BSD 3-Clause License
*   (c) by Mr Nalin Gupta, UK Research and Innovation, Jul 2024-present, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifndef GVXR_POLYGON_MESH_H
#include "gVirtualXRay/PolygonMesh.h"
#endif

#ifndef GVXR_CONSTANT_VALUES_H
#include "gVirtualXRay/ConstantValues.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*   @class    StepWedgeMesh
*   @brief    StepWedgeMesh is a class to handle 3D meshes of step wedges.
*/
//==============================================================================
template<typename T> class StepWedgeMesh: public PolygonMesh
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    ///    Default constructor.
    /*
    *  @param aNumberOfSteps:   the number of steps in the stepwedge (default value: 10)
    *  @param aTotalLength:  the length of the step wedge, i.e. its length along the X-axis (default value: 10)
    *  @param aWidth:   the width of the step wedge, i.e. its length along the Y-axis (default value: 1)
    *  @param aFirstStepHeight:  the height of the first step on the step wedge, i.e. its length along the Z-axis (default value: 1)
    *  @param anOtherStepHeight:  the height of the rest of the steps on the step wedge, i.e. its length along the Z-axis (default value: 0.1)
    */
    //--------------------------------------------------------------------------
    StepWedgeMesh(unsigned int aNumberOfSteps = 2,
		T aTotalLength = 10.0 * cm,
		T aWidth = 1.0 * cm,
		T aFirstStepHeight = 1.0 * cm,
		T anOtherStepHeight = 0.1 * cm,
		int anIndexDataType = 0);


    //--------------------------------------------------------------------------
    ///    Create a new step wedge.
    /*
    *  @param aNumberOfSteps:   the number of steps in the stepwedge (default value: 10)
    *  @param aTotalLength:  the length of the step wedge, i.e. its length along the X-axis (default value: 10)
    *  @param aWidth:   the width of the step wedge, i.e. its length along the Y-axis (default value: 1)
    *  @param aFirstStepHeight:  the height of the first step on the step wedge, i.e. its length along the Z-axis (default value: 1)
    *  @param anOtherStepHeight:  the height of the rest of the steps on the step wedge, i.e. its length along the Z-axis (default value: 0.1)
    */
    //--------------------------------------------------------------------------
    void create(unsigned int aNumberOfSteps = 2,
		T aTotalLength = 10.0 * cm,
		T aWidth = 1.0 * cm,
		T aFirstStepHeight = 1.0 * cm,
		T anOtherStepHeight = 0.1 * cm,
		int anIndexDataType = 0);
};


} // namespace gVirtualXRay


//******************************************************************************
#include "StepWedgeMesh.inl"


#endif // GVXR_STEP_WEDGE_MESH_H
