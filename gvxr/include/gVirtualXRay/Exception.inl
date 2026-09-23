/*

Copyright (c) 2014, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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


/**
********************************************************************************
*
*	@file		Exception.inl
*
*	@brief		Generic class to handle exceptions.
*
*	@version	1.0
*
*	@date		07/11/2013
*
*	@author		Dr Franck P. Vidal
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
//	Include
//******************************************************************************
#include <sstream>


//******************************************************************************
//	define
//******************************************************************************


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//-----------------------------------------------------------
inline std::ostream& operator<<(std::ostream& anOutputStream,
								const Exception& anException)
//-----------------------------------------------------------
{
	anOutputStream << anException.what();
	
	return (anOutputStream);
}


//------------------------------------------------------
inline Exception::Exception(const char* aFileName,
							const char* aFunctionName,
							int anErrorLine,
							const char* anErrorMessage):
//------------------------------------------------------
		std::exception()
//------------------------------------------------------
{
	std::stringstream error_message;

	if (anErrorMessage)
	{
		error_message << "Error: " << anErrorMessage << std::endl; 
	}
	else
	{
		error_message << "Error: " << std::endl; 
	}
	error_message << "\t- in File: " << aFileName << std::endl; 
	error_message << "\t- in Function: " << aFunctionName << std::endl; 
	error_message << "\t- at Line: " << anErrorLine << std::endl;
	
	m_error_message = error_message.str();
}


//-------------------------------------------------------------
inline Exception::Exception(const char* aFileName,
							const char* aFunctionName,
							int anErrorLine,
							const std::string& anErrorMessage):
//-------------------------------------------------------------
		std::exception()
//-------------------------------------------------------------
{
	std::stringstream error_message;
	error_message << "Error: " << anErrorMessage << std::endl;
	error_message << "\t- in File: " << aFileName << std::endl;
	error_message << "\t- in Function: " << aFunctionName << std::endl;
	error_message << "\t- at Line: " << anErrorLine << std::endl;

	m_error_message = error_message.str();
}


//------------------------------------
inline Exception::~Exception() throw()
//------------------------------------
{}


//------------------------------------------------
inline const char* Exception::what() const throw()
//------------------------------------------------
{
	return (m_error_message.data());
}


} // namespace gVirtualXRay
