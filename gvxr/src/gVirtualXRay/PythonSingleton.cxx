/*

Copyright (c) 2023, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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
*   @file       PythonSingleton.cxx
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
#ifndef GVXR_PYTHON_SINGLETON_H
#include <gVirtualXRay/PythonSingleton.h>
#endif

#ifndef GVXR_LOGGER_H
#include <gVirtualXRay/Logger.h>
#endif

#ifdef HAS_PythonLibs
    #ifndef GVXR_UTILITIES_H
    #include "gVirtualXRay/Utilities.h"
    #endif

    #include "getSpectrum.py.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//	static attribute
//******************************************************************************
std::unique_ptr<PythonSingleton> PythonSingleton::m_p_instance = 0;


//--------------------------------
void PythonSingleton::initPython()
//--------------------------------
{
#ifdef HAS_PythonLibs
    Py_Initialize();

    m_has_spekpy = false;
    m_has_xpecgen = false;
    
    char* p_python_script(0);
    int z_lib_return_code(0);

    z_lib_return_code = inflate(g_getSpectrum_py, sizeof(g_getSpectrum_py), &p_python_script);


	PyRun_SimpleString("import sys\nprint(sys.version)");

    m_p_global = PyDict_New();

    // Create a new module object
    m_p_module = PyModule_New("mymod");
    PyModule_AddStringConstant(m_p_module, "__file__", "");

    // Get the dictionary object from my module so I can pass this to PyRun_String
    m_p_local = PyModule_GetDict(m_p_module);

    // Define my function in the newly created module
    PyObject* p_value = PyRun_String(p_python_script,
        Py_file_input, m_p_global, m_p_local);

    // p_value would be null if the Python syntax is wrong, for example
    if (p_value == NULL)
    {
        if (PyErr_Occurred())
        {
            PyErr_Print();
        }

        LOGGER.logWarning("The Python syntax is wrong. This function ") << 
            __FUNCTION__ << " in " << __FILE__ << " at Line " << __LINE__ << " won't have any effect." << std::endl;

        return;
    }

    // p_value is the result of the executing code, 
    // chuck it away because we've only declared a function
    Py_CLEAR(p_value);

    // Get a pointer to the function I just defined
    PyObject* p_func = PyObject_GetAttrString(m_p_module, "getInterpretor");

    //Double check we have actually found it and it is callable
    if (!p_func || !PyCallable_Check(p_func))
    {
        if (PyErr_Occurred())
        {
            PyErr_Print();
        }

        LOGGER.logWarning("Cannot find function \"getInterpretor\". This function ") << 
            __FUNCTION__ << " in " << __FILE__ << " at Line " << __LINE__ << " won't have any effect." << std::endl;
    }

    // Call my function
    p_value = PyObject_CallObject(p_func, nullptr);

    m_python_interpretor = PyUnicode_AsUTF8(p_value);

    Py_CLEAR(p_func);

    // Get a pointer to the function I just defined
    p_func = PyObject_GetAttrString(m_p_module, "hasSpekpy");

    //Double check we have actually found it and it is callable
    if (!p_func || !PyCallable_Check(p_func))
    {
        if (PyErr_Occurred())
        {
            PyErr_Print();
        }

        LOGGER.logWarning("Cannot find function \"hasSpekpy\". This function ") << 
            __FUNCTION__ << " in " << __FILE__ << " at Line " << __LINE__ << " won't have any effect." << std::endl;

        m_has_spekpy = false;
    }

    // Call my function
    p_value = PyObject_CallObject(p_func, nullptr);

    if (PyLong_AsLong(p_value) == 1)
    {
        m_has_spekpy = true;
    }
    else
    {
        m_has_spekpy = false;
        LOGGER.logWarning("Spekpy is not installed. Install it using ") << m_python_interpretor <<
            " -m pip install git+https://bitbucket.org/spekpy/spekpy_release.git" << std::endl;
    }


    // Get a pointer to the function I just defined
    p_func = PyObject_GetAttrString(m_p_module, "hasXpecgen");

    //Double check we have actually found it and it is callable
    if (!p_func || !PyCallable_Check(p_func))
    {
        if (PyErr_Occurred())
        {
            PyErr_Print();
        }

        LOGGER.logWarning("Cannot find function \"hasXpecgen\". This function ") << 
            __FUNCTION__ << " in " << __FILE__ << " at Line " << __LINE__ << " won't have any effect." << std::endl;
    }

    // Call my function
    p_value = PyObject_CallObject(p_func, nullptr);

    if (PyLong_AsLong(p_value) == 1)
    {
        m_has_xpecgen = true;
    }
    else
    {
        m_has_xpecgen = false;
        LOGGER.logWarning("Xpecgen is not installed. Install it using ") << m_python_interpretor <<
            " -m pip install xpecgen" << std::endl;
    }

    Py_CLEAR(p_func);

    // Get a pointer to the function I just defined
    p_func = PyObject_GetAttrString(m_p_module, "getSpectrumSpekpy");

    //Double check we have actually found it and it is callable
    if (!p_func || !PyCallable_Check(p_func))
    {
        if (PyErr_Occurred())
        {
            PyErr_Print();
        }

        m_has_spekpy = false;
        LOGGER.logWarning("Cannot find function \"getSpectrumSpekpy\". This function ") << 
            __FUNCTION__ << " in " << __FILE__ << " at Line " << __LINE__ << " won't have any effect." << std::endl;
    }

    Py_CLEAR(p_func);

    // Get a pointer to the function I just defined
    p_func = PyObject_GetAttrString(m_p_module, "getSpectrumXpecgen");

    //Double check we have actually found it and it is callable
    if (!p_func || !PyCallable_Check(p_func))
    {
        if (PyErr_Occurred())
        {
            PyErr_Print();
        }

        m_has_xpecgen = false;
        LOGGER.logWarning("Cannot find function \"getSpectrumXpecgen\". This function ") << 
            __FUNCTION__ << " in " << __FILE__ << " at Line " << __LINE__ << " won't have any effect." << std::endl;
    }

    Py_CLEAR(p_func);

	PyRun_SimpleString("import sys\nprint(sys.version)");
#endif
}
