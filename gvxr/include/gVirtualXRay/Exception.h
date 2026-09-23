/*

Copyright (c) 2017, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
http://www.fpvidal.net/
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


#ifndef GVXR_EXCEPTION_H
#define GVXR_EXCEPTION_H


/**
********************************************************************************
*
*   @file       Exception.h
*
*   @brief      Generic class to handle exceptions.
*
*   @version    1.0
*
*   @date       07/11/2013
*
*   @author     Dr Franck P. Vidal
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*   http://www.fpvidal.net/, Dec 2014, 2014, version 1.0, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <string>
#include <exception>


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//******************************************************************************
//  Class declaration
//******************************************************************************
class Exception;


//******************************************************************************
//  Function declaration
//******************************************************************************


//------------------------------------------------------------------------------
/// operator<<
/**
*   @param  anOutputStream: the output stream
*   @param  anException:    the exception to add to the output stream
*   @return the output stream
*/
//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& anOutputStream,
        const Exception& anException);


//==============================================================================
/**
*   @class  Exception
*   @brief  Exception is a class to handle exceptions.
*/
//==============================================================================
class Exception: public std::exception
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default constructor.
    /**
     *  @param  aFileName:      name of the source file where the exception
     *                          was thrown (default value: 0)
     *  @param  aFunctionName:  name of the function where the exception
     *                          was thrown  (default value: 0)
     *  @param  anErrorLine:    name at which the exception was thrown
     *                          (default value: 0)
     *  @param  anErrorMessage: error message corresponding to the exception
     *                          (default value: 0)
     */
    //--------------------------------------------------------------------------
    Exception(const char* aFileName = 0,
            const char* aFunctionName = 0,
            int anErrorLine = 0,
            const char* anErrorMessage = 0);


    //--------------------------------------------------------------------------
    /// Constructor.
    /**
     *  @param  aFileName:      name of the source file where the exception
     *                          was thrown
     *  @param  aFunctionName:  name of the function where the exception
     *                          was thrown
     *  @param  anErrorLine:    name at which the exception was thrown
     *  @param  anErrorMessage: error message corresponding to the exception
     */
    //--------------------------------------------------------------------------
    Exception(const char* aFileName,
            const char* aFunctionName,
            int anErrorLine,
            const std::string& anErrorMessage);


    //--------------------------------------------------------------------------
    /// Destructor.
    //--------------------------------------------------------------------------
    virtual ~Exception() throw();


    //--------------------------------------------------------------------------
    /// Accessor on the error message.
    /**
     *  @return the error message
     */
    //--------------------------------------------------------------------------
    virtual const char* what() const throw();


//******************************************************************************
protected:
    /// The error message
    std::string m_error_message;
};


} // namespace gVirtualXRay


//******************************************************************************
#include "Exception.inl"


#endif // GVXR_EXCEPTION_H
