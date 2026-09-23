/*

Copyright (c) 2025-present, Prof Franck P. Vidal (franck.vidal@stfc.ac.uk),
UK Research and Innovation, All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

3. Neither the name of UK Research and Innovation nor the names of its contributors
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


#ifndef GVXR_NOT_IMPLEMENTED_EXCEPTION_H
#define GVXR_NOT_IMPLEMENTED_EXCEPTION_H


/**
********************************************************************************
*
*   @file       NotImplementedException.h
*
*   @brief      Class to handle exceptions when accessing a function that is not implemented.
*
*   @version    1.0
*
*   @date       13/10/2025
*
*   @author     Prof Franck P. Vidal
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk),
*   https://gvirtualxray.sourceforge.io/, Oct 2025, 2025, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifndef GVXR_EXCEPTION_H
#include "gVirtualXRay/Exception.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*   @class  NotImplementedException
*   @brief  NotImplementedException is a class to handle exceptions when accessing
*           a function that is not implemented.
*/
//==============================================================================
class NotImplementedException: public Exception
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default constructor.
    /**
     *  @param  aFileName:      name of the source file where the exception was
     *                          thrown (default value: 0)
     *  @param  aFunctionName:  name of the function where the exception was
     *                          thrown  (default value: 0)
     *  @param  anErrorLine:    name at which the exception was thrown
     *                          (default value: 0)
     */
    //--------------------------------------------------------------------------
    NotImplementedException(const char* aFileName = 0,
                         const char* aFunctionName = 0,
                         int anErrorLine = 0);
};


} // namespace gVirtualXRay


//******************************************************************************
#include "NotImplementedException.inl"


#endif // GVXR_NOT_IMPLEMENTED_EXCEPTION_H
