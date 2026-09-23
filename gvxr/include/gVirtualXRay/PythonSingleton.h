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


#ifndef GVXR_PYTHON_SINGLETON_H
#define GVXR_PYTHON_SINGLETON_H


/**
********************************************************************************
*
*   @file       PythonSingleton.h
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
//  Include
//******************************************************************************
#include <string>
#include <memory>

#ifdef HAS_PythonLibs
#include <Python.h>
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


class PythonSingleton{
public:
  static PythonSingleton& getInstance()
  {
        if (!m_p_instance)
        {
            // std::make_unique will not work
            m_p_instance = std::unique_ptr<PythonSingleton>(new PythonSingleton());
            m_p_instance->initPython();
        }
        return *m_p_instance.get();
    }

    bool hasSpekpy();
    bool hasXpecgen();

#ifdef HAS_PythonLibs
    PyObject* getModule();
#endif

    ~PythonSingleton();

private:
    PythonSingleton();

    void initPython();
    void releasePython();


    PythonSingleton(const PythonSingleton&)= delete;
    PythonSingleton& operator=(const PythonSingleton&)= delete;

    static std::unique_ptr<PythonSingleton> m_p_instance;
    bool m_initialised;
    bool m_has_spekpy;
    bool m_has_xpecgen;
    std::string m_python_interpretor;

#ifdef HAS_PythonLibs
    PyObject* m_p_global;
    PyObject* m_p_module;
    PyObject* m_p_local;
#endif

};


} // namespace gVirtualXRay


//******************************************************************************
#include "PythonSingleton.inl"


#endif // GVXR_PYTHON_SINGLETON_H
