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
*	@file		OpenGLException.cxx
*
*	@brief		Class to handle exceptions related to frame buffer objects (FBOs).
*
*	@version	1.0
*
*	@date		07/11/2013
*
*	@author		Dr Franck P. Vidal
*
*	@section	License
*				BSD 3-Clause License.
*
*				For details on use and redistribution please refer
*				to http://opensource.org/licenses/BSD-3-Clause
*
*	@section	Copyright
*				(c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*				http://www.fpvidal.net/, Dec 2014, 2014, version 1.0,
*				BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <sstream>

#ifndef GVXR_FBO_EXCEPTION_H
#include "gVirtualXRay/FBOException.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
using namespace gVirtualXRay;

//------------------------------------------------------
FBOException::FBOException(const char* aFileName,
						   const char* aFunctionName,
						   int anErrorLine,
						   int anErrorCode):
//------------------------------------------------------
		Exception(aFileName, aFunctionName, anErrorLine)
//------------------------------------------------------
{
	std::stringstream error_message;

	switch (anErrorCode)
	{
    case GL_FRAMEBUFFER_UNDEFINED:
        error_message << "FBO error: the target is the default framebuffer, but the default framebuffer does not exist." << std::endl;
		break;

    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        error_message << "FBO error: any of the framebuffer attachment points are framebuffer incomplete." << std::endl;
		break;

    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        error_message << "FBO error: the framebuffer does not have at least one image attached to it." << std::endl;
		break;

    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        error_message << "FBO error: the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for any color attachment point(s) named by GL_DRAW_BUFFERi." << std::endl;
		break;

    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        error_message << "FBO error: GL_READ_BUFFER is not GL_NONE and the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for the color attachment point named by GL_READ_BUFFER." << std::endl;
		break;

    case GL_FRAMEBUFFER_UNSUPPORTED:
        error_message << "FBO error: the combination of internal formats of the attached images violates an implementation-dependent set of restrictions." << std::endl;
		break;

    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
        error_message << "FBO error:" << std::endl;
        error_message << "the value of GL_RENDERBUFFER_SAMPLES is not the same for all attached renderbuffers; if the value of GL_TEXTURE_SAMPLES is the not same for all attached textures; or, if the attached images are a mix of renderbuffers and textures, the value of GL_RENDERBUFFER_SAMPLES does not match the value of GL_TEXTURE_SAMPLES." << std::endl;
        error_message << "or" << std::endl;
        error_message << "the value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not the same for all attached textures; or, if the attached images are a mix of renderbuffers and textures, the value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not GL_TRUE for all attached textures." << std::endl;
		break;

    // case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS_ARB:
    //     error_message << "FBO error: any framebuffer attachment is layered, and any populated attachment is not layered, or if all populated color attachments are not from textures of the same target." << std::endl;
	// 	break;

    default:
		error_message << "FBO error: unknown." << std::endl;
		break;
	}

	error_message << "\t- in File: " << aFileName << std::endl;
	error_message << "\t- in Function: " << aFunctionName << std::endl;
	error_message << "\t- at Line: " << anErrorLine << std::endl;

	m_error_message = error_message.str();
}
