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
*    @file        OpenGLUtilities.cxx
*
*    @brief        Some utility functions about OpenGL.
*               Now supports GLSL450 and OpenGL 4.5
*
*    @version    1.0
*
*    @date        25/02/2020
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
*                http://www.fpvidal.net/, Feb 2020, 2020, version 1.0,
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

#ifndef GVXR_OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef GVXR_OPENGL_EXCEPTION_H
#include "gVirtualXRay/OpenGLException.h"
#endif

#ifndef GVXR_FBO_EXCEPTION_H
#include "gVirtualXRay/FBOException.h"
#endif

#ifndef GVXR_OUT_OF_BOUNDS_EXCEPTION_H
#include "gVirtualXRay/OutOfBoundsException.h"
#endif

#ifndef GVXR_LOGGER_H
#include <gVirtualXRay/Logger.h>
#endif

#ifndef GVXR_SHADER_H
#include "gVirtualXRay/Shader.h"
#endif

#ifndef GVXR_RESOURCE_MANAGER_H
#include "gVirtualXRay/ResourceManager.h"
#endif


//******************************************************************************
//    namespace
//******************************************************************************
namespace gVirtualXRay {


std::vector<MATRIX4> g_p_modelview_matrix_stack;
std::vector<MATRIX4> g_p_projection_matrix_stack;
MATRIX4 g_current_modelview_matrix;
MATRIX4 g_current_projection_matrix;
std::vector<int> g_frame_buffer_object_stack;
std::vector<std::pair<int, int> > g_texture_stack;
std::vector<std::pair<int, bool> > g_enable_disable_state_stack;
std::vector<int> g_shader_program_stack;

bool g_has_been_initialised(false);
bool g_use_gl_4_5(true);
bool g_use_gl_3_2(true);
bool g_use_gl_compute(true);


//-------------------------------------------
void initializeGLAD(const std::string& anAPI)
//-------------------------------------------
{
    initialiseGLAD(anAPI);
}


//-------------------
void initializeGLEW()
//-------------------
{
    initialiseGLEW();
}


//----------------------------------------------------
void checkOpenGLErrorStatus(const char* aFileName,
                            const char* aFunctionName,
                            int aLineNumber)
//----------------------------------------------------
{
#ifdef HAS_OPENGL
    // No OpenGL error checking in Release modes
#ifndef NDEBUG
    // Get the OpenGL error code
    GLenum error_code(glGetError());

    // There was an error
    if (error_code != GL_NO_ERROR)
    {
#ifdef HAS_GLU
            LOGGER.logError(std::string("OpenGL error: ") + gluErrorString(error_code) + "\n");
#else // HAS_GLU
            LOGGER.logError("OpenGL error") ;
#endif // HAS_GLU
        throw OpenGLException(aFileName, aFunctionName, aLineNumber, error_code);
    }
#endif // NDEBUG
#endif // HAS_OPENGL
}


//-------------------------------------------------
void checkFBOErrorStatus(const char* aFileName,
                         const char* aFunctionName,
                         int aLineNumber)
//-------------------------------------------------
{
#ifdef HAS_OPENGL
    // Get the OpenGL error code
    GLenum error_code(glCheckFramebufferStatus(GL_FRAMEBUFFER));

    // There was an error
    if (error_code != GL_FRAMEBUFFER_COMPLETE)
    {
        throw FBOException(aFileName, aFunctionName, aLineNumber, error_code);
    }
#endif // HAS_OPENGL
}


//------------
void pushFBO()
//------------
{
#ifdef HAS_OPENGL
    // Save the current FBO
    GLint fbo_id(0);
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbo_id);

    g_frame_buffer_object_stack.push_back(fbo_id);
#endif // HAS_OPENGL
}


//------------------
void pushTexture1D()
//------------------
{
#ifdef HAS_OPENGL
    // Save the current texture
    GLint current_texture_id(0);
    glGetIntegerv(GL_TEXTURE_BINDING_1D, &current_texture_id);

    // Get the current 1D texture
    std::pair<GLint, GLint> current_texture_pair(GL_TEXTURE_1D, current_texture_id);

    // Add the last texture
    gVirtualXRay::g_texture_stack.push_back(current_texture_pair);
#endif // HAS_OPENGL
}


//------------------
void pushTexture2D()
//------------------
{
#ifdef HAS_OPENGL
    // Save the current texture
    GLint current_texture_id(0);
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &current_texture_id);

    // Get the current 2D texture
    std::pair<GLint, GLint> current_texture_pair(GL_TEXTURE_2D, current_texture_id);

    // Add the last texture
    gVirtualXRay::g_texture_stack.push_back(current_texture_pair);
#endif // HAS_OPENGL
}


//-------------------------
void pushTextureRectangle()
//-------------------------
{
#ifdef HAS_OPENGL
    // Save the current texture
    GLint current_texture_id(0);
    GLint current_texture_type(0);

#ifdef GL_TEXTURE_BINDING_RECTANGLE_ARB
    glGetIntegerv(GL_TEXTURE_BINDING_RECTANGLE_ARB, &current_texture_id);
#endif

#ifdef GL_TEXTURE_BINDING_RECTANGLE
    glGetIntegerv(GL_TEXTURE_BINDING_RECTANGLE, &current_texture_id);
#endif

    // Get the current 2D texture
#ifdef GL_TEXTURE_RECTANGLE_ARB
    current_texture_type = GL_TEXTURE_RECTANGLE_ARB;
#endif

#ifdef GL_TEXTURE_RECTANGLE
    current_texture_type = GL_TEXTURE_RECTANGLE;
#endif

    // Add the last texture
    std::pair<GLint, GLint> current_texture_pair(current_texture_type, current_texture_id);
    gVirtualXRay::g_texture_stack.push_back(current_texture_pair);
#endif // HAS_OPENGL
}


//------------------
void pushTexture3D()
//------------------
{
#ifdef HAS_OPENGL
    // Save the current texture
    GLint current_texture_id(0);
    glGetIntegerv(GL_TEXTURE_BINDING_3D, &current_texture_id);

    // Get the current 3D texture
    std::pair<GLint, GLint> current_texture_pair(GL_TEXTURE_3D, current_texture_id);

    // Add the last texture
    gVirtualXRay::g_texture_stack.push_back(current_texture_pair);
#endif // HAS_OPENGL
}


//-----------------------------------------
void pushEnableDisableState(int aParameter)
//-----------------------------------------
{
#ifdef HAS_OPENGL
    GLboolean state;
    glGetBooleanv(aParameter, &state);

    gVirtualXRay::g_enable_disable_state_stack.push_back(std::pair<int, bool>(aParameter, state));
#endif // HAS_OPENGL
}


//----------------------
void pushShaderProgram()
//----------------------
{
#ifdef HAS_OPENGL
    GLint current_shader(0);
    glGetIntegerv(GL_CURRENT_PROGRAM, &current_shader);

    gVirtualXRay::g_shader_program_stack.push_back(current_shader);
#endif // HAS_OPENGL
}


//-----------
void popFBO()
//-----------
{
#ifdef HAS_OPENGL
    // Make sure the stack is not empty
    if (g_frame_buffer_object_stack.size() == 0)
    {
        throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    // Save the current FBO
    GLint current_fbo_id(0);
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &current_fbo_id);

    // Get the previous FBO from the stack
    GLint previous_fbo_id(g_frame_buffer_object_stack.back());

    // The two FBOs are different
    if (previous_fbo_id != current_fbo_id)
    {
        // Restore the FBO
        glBindFramebuffer(GL_FRAMEBUFFER, previous_fbo_id);
        gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
    }

    // Remove the last FBO
    g_frame_buffer_object_stack.pop_back();
#endif // HAS_OPENGL
}


//---------------------
void popShaderProgram()
//---------------------
{
#ifdef HAS_OPENGL
    // Make sure the stack is not empty
    if (gVirtualXRay::g_shader_program_stack.size() == 0)
    {
        throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    // Save the current program
    GLint current_shader(0);
    glGetIntegerv(GL_CURRENT_PROGRAM, &current_shader);

    // Get the previous program from the stack
    GLint previous_program(gVirtualXRay::g_shader_program_stack.back());

    // The two programs are different
    if (previous_program != current_shader)
    {
        // Restore the FBO
        glUseProgram(previous_program);
        gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
    }

    // Remove the last program
    gVirtualXRay::g_shader_program_stack.pop_back();
#endif // HAS_OPENGL
}


//---------------
void popTexture()
//---------------
{
#ifdef HAS_OPENGL
    // Make sure the stack is not empty
    if (gVirtualXRay::g_texture_stack.size() == 0)
    {
        throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    // Get the previous texture from the stack
    std::pair<int, int> previous_texture(gVirtualXRay::g_texture_stack.back());

    // Remove the last texture
    gVirtualXRay::g_texture_stack.pop_back();

    // Save the current texture
    GLint current_texture_id(0);

    // Get the texture binding according to the texture type
    switch (previous_texture.first)
    {
    case GL_TEXTURE_1D:
        glGetIntegerv(GL_TEXTURE_BINDING_1D, &current_texture_id);
        break;

    case GL_TEXTURE_2D:
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &current_texture_id);
        break;

    case GL_TEXTURE_3D:
        glGetIntegerv(GL_TEXTURE_BINDING_3D, &current_texture_id);
        break;

    case GL_TEXTURE_RECTANGLE:
        glGetIntegerv(GL_TEXTURE_BINDING_RECTANGLE, &current_texture_id);
        break;

    default:
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid texture type");
    }

    // The two textures are different
    if (previous_texture.second != current_texture_id)
    {
        //Restore the texture binding
        switch (previous_texture.first)
        {
        case GL_TEXTURE_1D:
            glBindTexture(GL_TEXTURE_1D, previous_texture.second);
            break;

        case GL_TEXTURE_2D:
            glBindTexture(GL_TEXTURE_2D, previous_texture.second);
            break;

        case GL_TEXTURE_3D:
            glBindTexture(GL_TEXTURE_3D, previous_texture.second);
            break;

            case GL_TEXTURE_RECTANGLE:
            glBindTexture(GL_TEXTURE_RECTANGLE, previous_texture.second);
            break;

        default:
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid texture type");
        }
    }
#endif // HAS_OPENGL
}


//--------------------------
void popEnableDisableState()
//--------------------------
{
#ifdef HAS_OPENGL
    // Make sure the stack is not empty
    if (g_enable_disable_state_stack.size() == 0)
    {
        throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    // Get the previous state from the stack
    std::pair<int, bool> previous_state(g_enable_disable_state_stack.back());

    // Remove the last state
    g_enable_disable_state_stack.pop_back();

    // Save the current state
    GLboolean current_state(0);

    // Get the state according to the state type
    glGetBooleanv(previous_state.first, &current_state);

    // The two states are different
    if (previous_state.second != bool(current_state))
    {
        if (previous_state.second)
        {
            glEnable(previous_state.first);
        }
        else
        {
            glDisable(previous_state.first);
        }
    }
#endif // HAS_OPENGL
}


//-------------------------
void applyModelViewMatrix()
//-------------------------
{
#ifdef HAS_OPENGL
    // Store the current buffer
    GLint program_handle(0);
    glGetIntegerv(GL_CURRENT_PROGRAM, &program_handle);

    // Load the matrices
    if (program_handle)
    {
        GLuint modelview_matrix_handle(glGetUniformLocation(program_handle, "g_modelview_matrix"));
        glUniformMatrix4fv(modelview_matrix_handle, 1, GL_FALSE, g_current_modelview_matrix.get());
        gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        MATRIX4 normal_matrix(g_current_modelview_matrix.getInverse().getTranspose());
        GLuint normal_matrix_handle(glGetUniformLocation(program_handle, "g_normal_matrix"));
        glUniformMatrix3fv(normal_matrix_handle, 1, GL_FALSE, normal_matrix.get3x3());
        gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
    }
#endif // HAS_OPENGL
}


//----------------
bool useOpenGL45()
//----------------
{
    Shader dummy_shader;
    g_use_gl_compute = false;

#ifdef HAS_OPENGL
    // This is the first time the method is called
    if (!g_has_been_initialised)
    {
        try
        {
            // Compile a dummy shader for OpenGL 3.2
            // Vertex shader
            const GLchar* g_vertex_shader = "\
            \n#version 450\n \
            \n \
            layout(location=0) in vec3 in_Vertex;\n \
            \n \
            layout(location=0) uniform mat4 g_projection_matrix;\n \
            layout(location=1) uniform mat4 g_modelview_matrix;\n \
            \n \
            void main(void)\n \
            {\n \
                gl_Position = g_projection_matrix * g_modelview_matrix * vec4(in_Vertex, 1.0);\n \
            }\n \
            ";

            // Fragment shader
            const GLchar* g_fragment_shader = "\
            \n#version 450\n \
            precision highp float;\n \
            \n \
            layout(location=0) out vec4 fragColor;\n \
            void main(void)\n \
            {\n \
                fragColor = vec4(1.0, 1.0, 1.0, 1.0);\n \
            }\n \
            ";

            // Give a text ID to the vertex and fragment shaders, it can be useful when debuging shaders
            dummy_shader.setLabels("g_vertex_shader", "g_fragment_shader");

            // Load the source code of the shaders onto the GPU
            dummy_shader.loadSource(g_vertex_shader, g_fragment_shader);

            // An error occurred
            if (!dummy_shader.isValid())
            {
                g_use_gl_4_5 = false;
            }
        }
        // An error occured, OpenGL 3.2 is not supported
        catch (const std::exception& e)
        {
            g_use_gl_4_5 = false;
        }

        if (g_use_gl_4_5)
        {
            LOGGER.logNow("Use OpenGL 4.5.") << std::endl;
        }
        else
        {
            LOGGER.logNow("Cannot use OpenGL 4.5. You live in the past.") << std::endl;
        }

        g_has_been_initialised = g_use_gl_4_5;

        // GL 4.5 is supported
        if (g_use_gl_4_5)
        {
            // Make sure GL Compute works 
            g_use_gl_compute = useOpenGLCompute();
        }
    }
#endif // HAS_OPENGL

    ResourceManager::getInstance().destroyAndRemoveShader(&dummy_shader);

    return (g_use_gl_4_5);
}


//----------------
bool useOpenGL32()
//----------------
{
    Shader dummy_shader;
    g_use_gl_compute = false;

#ifdef HAS_OPENGL
    // This is the first time the method is called
    if (!g_has_been_initialised)
    {
        try
        {
            // Compile a dummy shader for OpenGL 3.2
            // Vertex shader
            const GLchar* g_vertex_shader = "\
            \n#version 150\n \
            \n \
            in vec3 in_Vertex;\n \
            \n \
            uniform mat4 g_projection_matrix;\n \
            uniform mat4 g_modelview_matrix;\n \
            \n \
            void main(void)\n \
            {\n \
                gl_Position = g_projection_matrix * g_modelview_matrix * vec4(in_Vertex, 1.0);\n \
            }\n \
            ";

            // Fragment shader
            const GLchar* g_fragment_shader = "\
            \n#version 150\n \
            precision highp float;\n \
            \n \
            out vec4 fragColor;\n \
            void main(void)\n \
            {\n \
                fragColor = vec4(1.0, 1.0, 1.0, 1.0);\n \
            }\n \
            ";

            // Give a text ID to the vertex and fragment shaders, it can be useful when debuging shaders
            dummy_shader.setLabels("g_vertex_shader", "g_fragment_shader");

            // Load the source code of the shaders onto the GPU
            dummy_shader.loadSource(g_vertex_shader, g_fragment_shader);

            // An error occurred
            if (!dummy_shader.isValid())
            {
                g_use_gl_3_2 = false;
            }
        }
        // An error occured, OpenGL 3.2 is not supported
        catch (const std::exception& e)
        {
            g_use_gl_3_2 = false;
        }

        if (g_use_gl_3_2)
        {
            LOGGER.logNow("Use OpenGL 3.2.") << std::endl;
        }
        else
        {
            LOGGER.logNow("Cannot use OpenGL 3.2. You really live in the past.") << std::endl;
        }

        // GL 3.2 is supported
        if (g_use_gl_3_2)
        {
            // Try if GL Compute works, just in case 
            g_use_gl_compute = useOpenGLCompute();
        }
    }

    g_has_been_initialised = true;
#endif // HAS_OPENGL

    ResourceManager::getInstance().destroyAndRemoveShader(&dummy_shader);

    return (g_use_gl_3_2);
}

#ifdef HAS_OPENGL
void GLAPIENTRY
MessageCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
        type, severity, message);
}
#endif


//---------------------
bool useOpenGLCompute()
//---------------------
{
#ifdef HAS_OPENGL
    Shader dummy_shader;

    try
    {
        // Compile a dummy shader for OpenGL
        // Compute shader
        const GLchar* g_compute_shader = "\
        \n#version 430\n \
        \n \
        layout(local_size_x=1, local_size_y=1) in;\n \
        \n \
        layout(rgba32f, binding=0) uniform image2D output_image;\n \
        \n \
        void main(void)\n \
        {\n \
            ivec2 pixel_coordinates = ivec2(gl_GlobalInvocationID.xy);\n \
            vec4 pixel_value = vec4(pixel_coordinates.x, pixel_coordinates.y, 0.0, 1.0);\n \
            imageStore(output_image, pixel_coordinates, pixel_value);\n \
        }\n \
        ";

        // Give a text ID to the compute  shader, it can be useful when debuging the shader
        dummy_shader.setLabel("g_compute_shader");

        // Load the source code of the shader onto the GPU
        dummy_shader.loadSource(g_compute_shader);

        // An error occurred
        if (!dummy_shader.isValid())
        {
            g_use_gl_compute = false;
        }
        // All good!
        else
        {
            g_use_gl_compute = true;
        }
    }
    // An error occured, OpenGL 3.2 is not supported
    catch (const std::exception& e)
    {
        LOGGER.logNow("OpenGL Compute Shader error: ") << e.what() << std::endl;

        g_use_gl_compute = false;
    }

    if (g_use_gl_compute)
    {
        LOGGER.logNow("Use OpenGL Compute Shader.") << std::endl;
    }
    else
    {
        LOGGER.logNow("Cannot use OpenGL Compute Shader. Some computations will be performed on the CPU using multi-threading.") << std::endl;
    }
    

    ResourceManager::getInstance().destroyAndRemoveShader(&dummy_shader);
#endif // HAS_OPENGL

    return (g_use_gl_compute);
}


//--------------
VBO* createVBO()
//--------------
{
    if (useOpenGL45())
    {
        return (createOpenGL3VBO());
    }
    else if (useOpenGL32())
    {
        return (createOpenGL3VBO());
    }
        else
    {
        return (createOpenGL2VBO());
    }
/*    else
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot create a VBO for your chosen OpenGL context");
    }
*/
}


//----------------------------
OpenGL2VBO* createOpenGL2VBO()
//----------------------------
{
    return (new OpenGL2VBO);
}


//----------------------------
OpenGL3VBO* createOpenGL3VBO()
//----------------------------
{
    return (new OpenGL3VBO);
}


};
