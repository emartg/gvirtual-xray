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
*    @file        Shader.cxx
*
*    @brief        Class to handle GLSL shaders.
*
*    @version    1.0
*
*    @date        29/11/2013
*
*    @author        Dr Franck P. Vidal
*
*    @section    License
*                BSD 3-Clause License.
*
*                For details on use and redistribution please refer
*                to http://opensource.org/licenses/BSD-3-Clause
*
*    @section    Copyright
*                (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*                http://www.fpvidal.net/, Dec 2014, 2014, version 1.0,
*                BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//    Include
//******************************************************************************
#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <fstream>
#include <sstream>
#include <algorithm>

#ifdef HAS_FILESYSTEM_HEADER
#include <filesystem>
#else
#include <experimental/filesystem>
#endif

#ifndef GVXR_SHADER_H
#include "gVirtualXRay/Shader.h"
#endif

#ifndef GVXR_FILE_DOES_NOT_EXIST_EXCEPTION_H
#include "gVirtualXRay/FileDoesNotExistException.h"
#endif

#ifndef GVXR_OUT_OF_MEMORY_EXCEPTION_H
#include "gVirtualXRay/OutOfMemoryException.h"
#endif

#ifndef GVXR_RESOURCE_MANAGER_H
#include <gVirtualXRay/ResourceManager.h>
#endif


//******************************************************************************
//    Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//    Static members
//******************************************************************************
bool Shader::m_shader_capable = false;


//---------------------------------------
Shader::Shader():
//---------------------------------------
    m_vertex_source_number_of_lines(0),
    m_fragment_source_number_of_lines(0),
    m_compute_source_number_of_lines(0),

    m_valid(false),

    m_shader_program_id(0),
    m_vertex_shader_id(0),
    m_fragment_shader_id(0),
    m_compute_shader_id(0)
//---------------------------------------
{
    ResourceManager::getInstance().addShader(this);
}


//---------------
Shader::~Shader()
//---------------
{
    release();
    ResourceManager::getInstance().destroyAndRemoveShader(this);
}


//--------------------
void Shader::release()
//--------------------
{
    Context* p_context = ResourceManager::getInstance().getGraphicsContext();

    // Get the current program
    GLint current_shader(0);

    if (m_shader_program_id && p_context)
    {
        if (p_context->isReady())
        {
#ifdef HAS_OPENGL
            glGetIntegerv(GL_CURRENT_PROGRAM, &current_shader);
#endif
        }
    }

    // Check the value
    if (current_shader < 0)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid program handle.");
    }

    // The current program is the same as the one that being deleted.
    // Make sure it is disable.
    if (m_shader_program_id && GLuint(current_shader) == m_shader_program_id)
    {
        disable();
    }

    // Delete the fragment shader
    if (m_fragment_shader_id && p_context)
    {
        if (p_context->isReady())
        {
#ifdef HAS_OPENGL
            glDeleteShader(m_fragment_shader_id);
#endif
        }
    }
    m_fragment_shader_id = 0;

    // Delete the vertex shader
    if (m_vertex_shader_id && p_context)
    {
        if (p_context->isReady())
        {
#ifdef HAS_OPENGL
            glDeleteShader(m_vertex_shader_id);
#endif
        }
    }
    m_vertex_shader_id = 0;

    // Delete the compute shader
    if (m_compute_shader_id && p_context)
    {
        if (p_context->isReady())
        {
#ifdef HAS_OPENGL
            glDeleteShader(m_compute_shader_id);
#endif
        }
    }
    m_compute_shader_id = 0;

    // Delete the shader program
    if (m_shader_program_id && p_context)
    {
        if (p_context->isReady())
        {
#ifdef HAS_OPENGL
            glDeleteProgram(m_shader_program_id);
#endif
        }
    }
    m_shader_program_id = 0;

    // Release memory
    m_vertex_source.clear();
    m_fragment_source.clear();
    m_compute_source.clear();

    m_valid = false;
}


//-------------------------
void Shader::enable() const
//-------------------------
{
    // Shaders are supported
    if (m_shader_capable)
    {
        if (m_valid && m_shader_program_id)
        {
#ifdef HAS_OPENGL
            // Get the current program
            GLint current_shader(0);
            glGetIntegerv(GL_CURRENT_PROGRAM, &current_shader);

            // Check the value
            if (current_shader < 0)
            {
                throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid program handle.");
            }

            // The current program is not he same as the one that being deleted.
            if (GLuint(current_shader) != m_shader_program_id)
            {
                glUseProgram(m_shader_program_id);
            }
#endif
        }
        else
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "Trying to enable a shader program that is not ready.");
        }
    }
    else
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Shader programs are not supported by your OpenGL implementation.");
    }
}


//--------------------
void Shader::disable()
//--------------------
{
    // Shaders are supported
    if (m_shader_capable)
    {
#ifdef HAS_OPENGL
        // Get the current program
        GLint current_shader(0);
        glGetIntegerv(GL_CURRENT_PROGRAM, &current_shader);

        // Disable the shader
        if (current_shader)
        {
            glUseProgram(0);
        }
#endif
    }
}


//----------------------------------------------------------------
void Shader::loadFiles(const std::string& aVertexShaderFilename,
                       const std::string& aFragmentShaderFilename)
//----------------------------------------------------------------
{
    // Reset the shader and restore the default values
    release();

    // Add the shader to the resource manager if needed
    ResourceManager::getInstance().addShader(this);

#ifdef HAS_OPENGL
    // Make sure a GL context exists
    Context* p_context = ResourceManager::getInstance().getGraphicsContext();

    if (p_context)
    {
        // Create the shader program
        m_shader_program_id = glCreateProgram();

        // Create the shader program has not been created
        if (!m_shader_program_id)
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot create a shader program.");
        }

        m_vertex_shader_label = aVertexShaderFilename;
        m_fragment_shader_label = aFragmentShaderFilename;
        loadVertexShaderFile(aVertexShaderFilename);
        loadFragmentShaderFile(aFragmentShaderFilename);
        linkProgram();

        m_valid = true;

        m_shader_capable = true;
    }
#endif
}


//--------------------------------------------------------------
void Shader::loadFile(const std::string& aComputeShaderFilename)
//--------------------------------------------------------------
{
    // Reset the shader and restore the default values
    release();

    // Add the shader to the resource manager if needed
    ResourceManager::getInstance().addShader(this);

#ifdef HAS_OPENGL
    // Make sure a GL context exists
    Context* p_context = ResourceManager::getInstance().getGraphicsContext();

    if (p_context)
    {
        // Create the shader program
        m_shader_program_id = glCreateProgram();

        // Create the shader program has not been created
        if (!m_shader_program_id)
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot create a shader program.");
        }

        m_compute_shader_label = aComputeShaderFilename;
        loadComputeShaderFile(aComputeShaderFilename);
        linkProgram();

        m_valid = true;

        m_shader_capable = true;
    }
#endif
}


//-------------------------------------------------------------------
void Shader::loadSource(const std::string& aVertexShaderSourceCode,
                        const std::string& aFragmentShaderSourceCode)
//-------------------------------------------------------------------
{
    // Reset the shader and restore the default values
    release();

    // Add the shader to the resource manager if needed
    ResourceManager::getInstance().addShader(this);

#ifdef HAS_OPENGL
    // Make sure a GL context exists
    Context* p_context = ResourceManager::getInstance().getGraphicsContext();

    if (p_context)
    {
        // Create the shader program
        m_shader_program_id = glCreateProgram();

        // Create the shader program has not been created
        if (!m_shader_program_id)
        {
            std::stringstream error_message;
            error_message << "Cannot create a shader program, function glCreateProgram () failed.";
            throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
        }

        loadVertexShaderSource(aVertexShaderSourceCode);
        loadFragmentShaderSource(aFragmentShaderSourceCode);
        linkProgram();

        m_valid = true;

        m_shader_capable = true;
    }
#endif
}


//------------------------------------------------------------------
void Shader::loadSource(const std::string& aComputeShaderSourceCode)
//------------------------------------------------------------------
{
    // Reset the shader and restore the default values
    release();

    // Add the shader to the resource manager if needed
    ResourceManager::getInstance().addShader(this);

#ifdef HAS_OPENGL
    // Make sure a GL context exists
    Context* p_context = ResourceManager::getInstance().getGraphicsContext();

    if (p_context)
    {
        // Create the shader program
        m_shader_program_id = glCreateProgram();

        // Create the shader program has not been created
        if (!m_shader_program_id)
        {
            std::stringstream error_message;
            error_message << "Cannot create a shader program, function glCreateProgram () failed.";
            throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
        }

        loadComputeShaderSource(aComputeShaderSourceCode);
        linkProgram();

        m_valid = true;

        m_shader_capable = true;
    }
#endif
}


//-------------------------------------------------------------------------
void Shader::loadVertexShaderFile(const std::string& aVertexShaderFilename)
//-------------------------------------------------------------------------
{
    // Store the name of the file
    m_vertex_shader_filename = aVertexShaderFilename;

    // The name has changed, therefore the shader is not loaded
    m_valid = false;

    // Load the file
    m_vertex_source_number_of_lines = loadFile(m_vertex_shader_filename, m_vertex_source);

    // Compile the shader
    m_vertex_shader_id = compileShader(GL_VERTEX_SHADER, m_vertex_source_number_of_lines, m_vertex_source);
}


//-----------------------------------------------------------------------------
void Shader::loadFragmentShaderFile(const std::string& aFragmentShaderFilename)
//-----------------------------------------------------------------------------
{
    // Store the name of the file
    m_fragment_shader_filename = aFragmentShaderFilename;

    // The name has changed, therefore the shader is not loaded
    m_valid = false;

    // Load the file
    m_fragment_source_number_of_lines = loadFile(m_fragment_shader_filename, m_fragment_source);

    // Compile the shader
    m_fragment_shader_id = compileShader(GL_FRAGMENT_SHADER, m_fragment_source_number_of_lines, m_fragment_source);
}


//---------------------------------------------------------------------------
void Shader::loadComputeShaderFile(const std::string& aComputeShaderFilename)
//---------------------------------------------------------------------------
{
    // Store the name of the file
    m_compute_shader_filename = aComputeShaderFilename;

    // The name has changed, therefore the shader is not loaded
    m_valid = false;

    // Load the file
    m_compute_source_number_of_lines = loadFile(m_compute_shader_filename, m_compute_source);

    // Compile the shader
    m_compute_shader_id = compileShader(GL_COMPUTE_SHADER, m_compute_source_number_of_lines, m_compute_source);
}


//-----------------------------------------------------------------------------
void Shader::loadVertexShaderSource(const std::string& aVertexShaderSourceCode)
//-----------------------------------------------------------------------------
{
    // Store the name of the file
    m_vertex_shader_filename = "Built in.";

    // The name has changed, therefore the shader is not loaded
    m_valid = false;

    // Load the file
    m_vertex_source = aVertexShaderSourceCode;

    // Count the number of new line characters ('\n')
    m_vertex_source_number_of_lines = 1;

    // Compile the shader
    m_vertex_shader_id = compileShader(GL_VERTEX_SHADER, m_vertex_source_number_of_lines, m_vertex_source);
}


//---------------------------------------------------------------------------------
void Shader::loadFragmentShaderSource(const std::string& aFragmentShaderSourceCode)
//---------------------------------------------------------------------------------
{
    // Store the name of the file
    m_fragment_shader_filename = "Built in.";

    // The name has changed, therefore the shader is not loaded
    m_valid = false;

    // Load the file
    m_fragment_source = aFragmentShaderSourceCode;

    // Count the number of new line characters ('\n')
    m_fragment_source_number_of_lines = 1;

    // Compile the shader
    m_fragment_shader_id = compileShader(GL_FRAGMENT_SHADER, m_fragment_source_number_of_lines, m_fragment_source);
}


//-------------------------------------------------------------------------------
void Shader::loadComputeShaderSource(const std::string& aComputeShaderSourceCode)
//-------------------------------------------------------------------------------
{
    // Store the name of the file
    m_compute_shader_filename = "Built in.";

    // The name has changed, therefore the shader is not loaded
    m_valid = false;

    // Load the file
    m_compute_source = aComputeShaderSourceCode;

    // Count the number of new line characters ('\n')
    m_compute_source_number_of_lines = 1;

    // Compile the shader
    m_compute_shader_id = compileShader(GL_COMPUTE_SHADER, m_compute_source_number_of_lines, m_compute_source);
}


//------------------------------------------------------
int Shader::loadFile(const std::string& aShaderFilename,
                     std::string& aShaderSource)
//------------------------------------------------------
{
    int line_number(0);

    // Open the file
    std::ifstream input_stream(aShaderFilename.data());

    // Check if the file is open
    if (!input_stream.is_open())
    {
        throw FileDoesNotExistException(__FILE__, __FUNCTION__, __LINE__, aShaderFilename);
    }

    // Count the number of new line characters ('\n')
    line_number = count(std::istreambuf_iterator<char>(input_stream), std::istreambuf_iterator<char>(), '\n');

    // Count the last line
    ++line_number;

    // Retrieve the file size
#ifdef HAS_FILESYSTEM_HEADER
    auto size = std::filesystem::file_size(aShaderFilename);
#else
    auto size = std::experimental::filesystem::file_size(aShaderFilename);
#endif
    // Allocate the string buffer and initialise it
    aShaderSource = std::string(size, '\0');

    // Read the file into the string buffer
    input_stream.read(&aShaderSource[0], size);

    // Close the file
    input_stream.close();

    return (line_number);
}


//------------------------------------------------------------------
unsigned int Shader::compileShader(int aShaderType,
                                   int aLineNumber,
                                   const std::string& aShaderSource)
//------------------------------------------------------------------
{
    GLuint shader_handle(0);

#ifdef HAS_OPENGL
    GLint compile_status(GL_TRUE);
    GLint log_size(0);
    char* p_log_message(0);

    shader_handle = glCreateShader(aShaderType);

    if (!shader_handle)
    {
        std::stringstream error_message;

        switch (aShaderType)
        {
        case GL_VERTEX_SHADER:
            error_message << "Cannot create the vertex shader, function glCreateShader (int) failed.";
            break;

        case GL_FRAGMENT_SHADER:
            error_message << "Cannot create the fragment shader, function glCreateShader (int) failed.";
            break;

        case GL_COMPUTE_SHADER:
            error_message << "Cannot create the compute shader, function glCreateShader (int) failed.";
            break;

        default:
            error_message << "Cannot create shader with an unknown type, function glCreateShader (int) failed.";
            break;
        }

        throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
    }

    // Assign the source code to the shader
    const GLchar* p_string = aShaderSource.c_str();
    glShaderSource(shader_handle, aLineNumber, &p_string, NULL);

    // Compile the source code
    glCompileShader(shader_handle);

    // Check the compilation status
    glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &compile_status);

    // there was at least one error
    if (compile_status != GL_TRUE)
    {
        // Get the size of the log message
        glGetShaderiv(shader_handle, GL_INFO_LOG_LENGTH, &log_size);

        // Allocate memory to store the log message
        p_log_message = new char[log_size + 1];

        // Get the label
        std::string label;
        switch (aShaderType)
        {
        case GL_VERTEX_SHADER:
            label = m_vertex_shader_label;
            break;

        case GL_FRAGMENT_SHADER:
            label = m_fragment_shader_label;
            break;

        case GL_COMPUTE_SHADER:
            label = m_compute_shader_label;
            break;

        default:
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot create shader with an unknown type.");
            break;
        }

        std::stringstream error_message;
        error_message << "Shader program compilation error in:" << std::endl;

        if (label.length())
        {
            error_message << "\t" << label << std::endl;
        }

        // The memory was not allocated
        if (!p_log_message)
        {
            error_message << aShaderSource;
            throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
        }

        // Get the log message
        glGetShaderInfoLog(shader_handle, log_size, &log_size, p_log_message);

        // Add the log to the error message
        error_message << "See error below:" << std::endl;
        error_message << p_log_message;

        // Release the memory used to store the log message
        delete [] p_log_message;

        // Delete the shader
        glDeleteShader(shader_handle);

        throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
    }

    // Attache the shader to the program
    if (shader_handle)
    {
        glAttachShader(m_shader_program_id, shader_handle);
    }
#endif

    return (shader_handle);
}


//------------------------
void Shader::linkProgram()
//------------------------
{
#ifdef HAS_OPENGL
    GLint link_status(0);
    GLint log_size(0);
    char* p_log_message(0);

    // Location binding code here
    if (m_vertex_shader_id && m_fragment_shader_id && !m_compute_shader_id)
    {
        glBindAttribLocation(m_shader_program_id, 0, "in_Vertex");
        glBindAttribLocation(m_shader_program_id, 1, "in_TexCoord");
        glBindAttribLocation(m_shader_program_id, 2, "in_Normal");
    }

    // Linkage
    glLinkProgram(m_shader_program_id);

    // Check the status of the linkage
    glGetProgramiv(m_shader_program_id, GL_LINK_STATUS, &link_status);

    // There was at least one error
    if (link_status != GL_TRUE)
    {
        std::stringstream error_message;
        error_message << "Shader program linkage error" << std::endl;

        if (m_vertex_shader_label.length() && m_fragment_shader_label.length())
        {
            error_message << " in:" << std::endl;
            error_message << "\t" << m_vertex_shader_label << " and " << m_fragment_shader_label << std::endl;
        }
        else if (m_compute_shader_id)
        {
            error_message << " in:" << std::endl;
            error_message << "\t" << m_compute_shader_label << std::endl;
        }

        // Get the size of the log message
        glGetProgramiv(m_shader_program_id, GL_INFO_LOG_LENGTH, &log_size);

        // The memory was not allocated
        if (!p_log_message)
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
        }

        // Allocate memory to store the log message
        p_log_message = new char[log_size + 1];

        // Get the log message
        glGetProgramInfoLog(m_shader_program_id, log_size, &log_size, p_log_message);

        error_message << "See error below:" << std::endl;
        error_message << p_log_message;

        // Release the memory used to store the log message
        delete [] p_log_message;

        throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
    }
#endif
}
