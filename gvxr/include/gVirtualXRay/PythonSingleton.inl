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
*   @file       PythonSingleton.inl
*
*   @brief      Singleton to handle the initialisation and release of Python.
*
*   @version    1.0
*
*   @date       21/09/2023
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
*   http://www.fpvidal.net/, Sept 2023, 2023, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {



//--------------------------------------
inline bool PythonSingleton::hasSpekpy()
//--------------------------------------
{
    return m_has_spekpy;
}


//---------------------------------------
inline bool PythonSingleton::hasXpecgen()
//---------------------------------------
{
    return m_has_xpecgen;
}


#ifdef HAS_PythonLibs
//-------------------------------------------
inline PyObject* PythonSingleton::getModule()
//-------------------------------------------
{
    return m_p_module;
}
#endif


//----------------------------------------
inline PythonSingleton::~PythonSingleton()
//----------------------------------------
{
    releasePython();
}


//----------------------------------------
inline PythonSingleton::PythonSingleton():
//----------------------------------------
    m_initialised(false),
#ifdef HAS_PythonLibs
    m_p_global(0),
    m_p_module(0),
    m_p_local(0),
#endif
    m_has_spekpy(false),
    m_has_xpecgen(false)
//----------------------------------------
{}


//------------------------------------------
inline void PythonSingleton::releasePython()
//------------------------------------------
{
    m_has_spekpy = false;
    m_has_xpecgen = false;

#ifdef HAS_PythonLibs
	Py_CLEAR(m_p_local);
	Py_CLEAR(m_p_module);
	Py_CLEAR(m_p_global);

    if (m_initialised) Py_Finalize();
    m_initialised = false;
#endif
}


} // namespace gVirtualXRay
