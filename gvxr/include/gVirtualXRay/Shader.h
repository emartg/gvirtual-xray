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


#ifndef GVXR_SHADER_H
#define GVXR_SHADER_H


/**
********************************************************************************
*
*   @file       Shader.h
*
*   @brief      Class to handle GLSL shaders.
*
*   @version    1.0
*
*   @date       13/01/2017
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
*   http://www.fpvidal.net/, Jan 2017, 2017, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/

//******************************************************************************
//  Include
//******************************************************************************
#include <string>


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*   @class  Shader
*   @brief  Shader is a class to handle shaders written in GLSL.
*/
//==============================================================================
class Shader
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default constructor.
    //--------------------------------------------------------------------------
    Shader();


    //--------------------------------------------------------------------------
    /// Destructor.
    //--------------------------------------------------------------------------
    ~Shader();


    //--------------------------------------------------------------------------
    /// Enable the current shader
    //--------------------------------------------------------------------------
    void enable() const;


    //--------------------------------------------------------------------------
    /// Disable the current shader (if any)
    //--------------------------------------------------------------------------
    static void disable();


    //--------------------------------------------------------------------------
    /// Reset (delete the current shader if it is loaded, and restore default
    /// values).
    //--------------------------------------------------------------------------
    void release();


    //--------------------------------------------------------------------------
    /// Load the vertex and fragment shaders from files.
    /**
     *  @param aVertexShaderFilename: file name of the vertex shader
     *  @param aFragmentShaderFilename: file name of the fragment shader
     */
    //--------------------------------------------------------------------------
    void loadFiles(const std::string& aVertexShaderFilename,
                   const std::string& aFragmentShaderFilename);


    //--------------------------------------------------------------------------
    /// Load the compute shader from a file.
    /**
     *  @param aComputeShaderFilename: file name of the compute shader
     */
    //--------------------------------------------------------------------------
    void loadFile(const std::string& aComputeShaderFilename);


    //--------------------------------------------------------------------------
    /// Set the vertex and fragment shader labels. This is useful for debugging.
    /**
     *  @param aVertexShaderLabel: label of the vertex shader
     *  @param aFragmentShaderLabel: label of the fragment shader
     */
    //--------------------------------------------------------------------------
    void setLabels(const std::string& aVertexShaderLabel,
                   const std::string& aFragmentShaderLabel);


    //--------------------------------------------------------------------------
    /// Set the compute shader label. This is useful for debugging.
    /**
     *  @param aComputeShaderLabel: label of the compute shader
     */
    //--------------------------------------------------------------------------
    void setLabel(const std::string& aComputeShaderLabel);


    //--------------------------------------------------------------------------
    /// Load the vertex shader from a file
    /**
     *  @param aVertexShaderFilename: file name of the vertex shader
     */
    //--------------------------------------------------------------------------
    void loadVertexShaderFile(const std::string& aVertexShaderFilename);


    //--------------------------------------------------------------------------
    /// Load the fragment shader from a file
    /**
     *  @param aFragmentShaderFilename: file name of the fragment shader
     */
    //--------------------------------------------------------------------------
    void loadFragmentShaderFile(const std::string& aFragmentShaderFilename);


    //--------------------------------------------------------------------------
    /// Load the compute shader from a file
    /**
     *  @param aComputeShaderFilename: file name of the compute shader
     */
    //--------------------------------------------------------------------------
    void loadComputeShaderFile(const std::string& aComputeShaderFilename);


    //--------------------------------------------------------------------------
    /// Load the shader source code
    /**
     *  @param aVertexShaderSourceCode: source code of the vertex shader
     *  @param aFragmentShaderSourceCode: source code of the fragment shader
     */
     //--------------------------------------------------------------------------
    void loadSource(const std::string& aVertexShaderSourceCode,
        const std::string& aFragmentShaderSourceCode);


    //--------------------------------------------------------------------------
    /// Load the shader source code
    /**
     *  @param aComputeShaderSourceCode: source code of the compute shader
     */
     //--------------------------------------------------------------------------
    void loadSource(const std::string& aComputeShaderSourceCode);


    //--------------------------------------------------------------------------
    /// Load the vertex shader from a string
    /**
     *  @param aVertexShaderSourceCode: the vertex shader source code
     */
    //--------------------------------------------------------------------------
    void loadVertexShaderSource(const std::string& aVertexShaderSourceCode);


    //--------------------------------------------------------------------------
    /// Load the fragment shader from a string
    /**
     *  @param aFragmentShaderSourceCode: the fragment shader source code
     */
    //--------------------------------------------------------------------------
    void loadFragmentShaderSource(const std::string& aFragmentShaderSourceCode);


    //--------------------------------------------------------------------------
    /// Load the compute shader from a string
    /**
     *  @param aComputeShaderSourceCode: the compute shader source code
     */
    //--------------------------------------------------------------------------
    void loadComputeShaderSource(const std::string& aComputeShaderSourceCode);


    //--------------------------------------------------------------------------
    /// Accessor on the shader program handle
    /**
     *  @return the shader program handle
     */
    //--------------------------------------------------------------------------
    unsigned int getProgramHandle() const;


    //--------------------------------------------------------------------------
    /// Accessor on the validity status of the shader
    /**
     *  @return true if the shader is valid, false if it is not
     */
    //--------------------------------------------------------------------------
    bool isValid() const;


//******************************************************************************
protected:
    //--------------------------------------------------------------------------
    /// Load a text file and store its content
    /**
     *  @param aShaderFilename: file name of the vertex or fragment shader
     *  @param aShaderSource: source code of the vertex or fragment shader
     *  @return the number of lines in the file
     */
    //--------------------------------------------------------------------------
    int loadFile(const std::string& aShaderFilename, std::string& aShaderSource);

    
    //--------------------------------------------------------------------------
    /// Compile the vertex shader or the fragment shader
    /**
     *  @param aShaderType:     type of shader (GL_VERTEX_SHADER,
     *                          GL_FRAGMENT_SHADER or GL_COMPUTE_SHADER)
     *  @param aLineNumber:     the number of lines of the source code
     *  @param aShaderSource:   source code of the vertex or fragment shader
     *  @return the corresponding ID
     */
    //--------------------------------------------------------------------------
    unsigned int compileShader(int aShaderType,
                               int aLineNumber,
                               const std::string& aShaderSource);


    //--------------------------------------------------------------------------
    /// Link the vertex and fragment programs
    //--------------------------------------------------------------------------
    void linkProgram();


    /// Flag set to true when a shader if fully valid
    static bool m_shader_capable;


    /// Label of the vertex shader
    std::string m_vertex_shader_label;


    /// Label of the fragment shader
    std::string m_fragment_shader_label;


    /// Label of the compute shader
    std::string m_compute_shader_label;


    /// Name of the file containing the vertex shader
    std::string m_vertex_shader_filename;


    /// Name of the file containing the fragment shader
    std::string m_fragment_shader_filename;


    /// Name of the file containing the compute shader
    std::string m_compute_shader_filename;


    /// Source code of the vertex shader
    std::string m_vertex_source;


    /// Number of line of the vertex shader
    int m_vertex_source_number_of_lines;


    /// Source code of the fragment shader
    std::string m_fragment_source;


    /// Number of line of the fragment shader
    int m_fragment_source_number_of_lines;


    /// Source code of the vertex shader
    std::string m_compute_source;


    /// Number of line of the vertex shader
    int m_compute_source_number_of_lines;


    /// Flag set to true if the vertex shader,
    /// the fragment shader and
    /// the shader program are all valid
    bool m_valid;
    

    /// ID of the shader program
    unsigned int m_shader_program_id;


    /// ID of the vertex shader
    unsigned int m_vertex_shader_id;


    /// ID of the fragment shader
    unsigned int m_fragment_shader_id;


    /// ID of the compute shader
    unsigned int m_compute_shader_id;
};


} // namespace gVirtualXRay


//******************************************************************************
#include "Shader.inl"


#endif // GVXR_SHADER_H
