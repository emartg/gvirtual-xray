/*

Copyright (c) 2013-2023, Dr Franck P. Vidal, Bangor University, All rights reserved.
Copyright (c) 2023-present, Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), 
UK Research and Innovation, All rights reserved.


Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

3. Neither the name of Bangor University, UK Research and Innovation nor the 
names of its contributors may be used to endorse or promote products derived 
from this software without specific prior written permission.

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
*   @file       TestOpenGLContext
*
*   @brief      Unit testing: Check if the computer supports various verions of OpenGL.
*
*   @date       18/09/2024
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
*   (c) by Dr Franck P. Vidal, Bangor University, Sept 2022-Sept 2023, version 1.1, BSD 3-Clause License
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, Oct 2023-present, version 1.1, BSD 3-Clause License
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <iostream>
#include <vector>

#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include "gtest/gtest.h"

#ifndef GVXR_CONTEXT_H
#include "gVirtualXRay/Context.h"
#endif

#ifndef GVXR_SHADER_H
#include "gVirtualXRay/Shader.h"
#endif

#ifndef GVXR_LOGGER_H
#include <gVirtualXRay/Logger.h>
#endif

#ifndef GVXR_EXCEPTION_H
#include <gVirtualXRay/Exception.h>
#endif

#ifndef GVXR_utils_h
#include "utils.h"
#endif


//******************************************************************************
//  Macros
//******************************************************************************
#define TEXTURE_WIDTH 1024
#define TEXTURE_HEIGHT 512


//******************************************************************************
//  Function declarations
//******************************************************************************
void testShader();
void testComputeShaderFloat();
void testComputeShaderUInt32();


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//  Function definitions
//******************************************************************************

//--------------------------------------
TEST(TestOpenGL, TestDefaultConstructor)
//--------------------------------------
{
    // Instantiation
    Context GL_context;

#ifdef HAS_OPENGL
    // Unknown OpenGL version
    const GLubyte* error_message = glGetString(GL_VERSION);
    ASSERT_EQ(error_message == nullptr, true);

    ASSERT_EQ(GL_context.isReady(), false);
#endif

#ifdef HS_GLFW
    ASSERT_EQ(GL_context.getWindowHandle() == nullptr, true);
#endif

    GL_context.release();
}


//--------------------------------
TEST(TestOpenGL, TestConstructors)
//--------------------------------
{
    // List all the possible context versions
    std::vector<std::pair<unsigned int, unsigned int>> p_context_version_set;

    p_context_version_set.push_back(std::pair<unsigned int, unsigned int>(3, 1));

#ifndef GVXR_APPLE__
    p_context_version_set.push_back(std::pair<unsigned int, unsigned int>(3, 2));
    p_context_version_set.push_back(std::pair<unsigned int, unsigned int>(3, 3));
#endif

    p_context_version_set.push_back(std::pair<unsigned int, unsigned int>(4, 1));
    p_context_version_set.push_back(std::pair<unsigned int, unsigned int>(4, 2));

#ifndef GVXR_APPLE__
    p_context_version_set.push_back(std::pair<unsigned int, unsigned int>(4, 3));
    p_context_version_set.push_back(std::pair<unsigned int, unsigned int>(4, 4));
    p_context_version_set.push_back(std::pair<unsigned int, unsigned int>(4, 5));
    p_context_version_set.push_back(std::pair<unsigned int, unsigned int>(4, 6));
#endif

    // Go through all the possible context versions
    for (std::vector<std::pair<unsigned int, unsigned int>>::const_iterator ite = p_context_version_set.begin();
            ite != p_context_version_set.end();
            ++ite)
    {
        // Create an OpenGL context
        Context GL_context;

#ifdef HAS_OPENGL
        LOGGER.logNow("********************************************************************************") << std::endl;
        LOGGER.logNow("Try to create an OpenGL context for version: ") << ite->first << "." << ite->second << std::endl;
        LOGGER.logNow("********************************************************************************") << std::endl;
        LOGGER.logNow("") << std::endl;

        // Enable OpenGL
        initGL(GL_context, ite->first, ite->second);

        // Retrieve the OpenGL version
        LOGGER.logNow("OpenGL version: ") << std::string(reinterpret_cast<const char *>(glGetString(GL_VERSION))) << std::endl;

        // The context should be ready
        EXPECT_EQ(GL_context.isReady(), true);

        // Retrieve the OpenGL context version
        int major = 0;
        int minor = 0;

        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);

        // If major is greater than ite->first, then consider that the test was successful
        if (major > int(ite->first))
        {
            EXPECT_EQ(major > int(ite->first), true);
        }
        // major is the same as ite->first, make sure minor >= ite->second
        else if (major == int(ite->first))
        {
            EXPECT_EQ(major, int(ite->first));
            EXPECT_EQ(minor >= int(ite->second), true);
        }
        // major < ite->first: not good!
        else if (major < int(ite->first))
        {
            EXPECT_EQ(major, int(ite->first));
        }

        LOGGER.logNow("") << std::endl;
        LOGGER.logNow("") << std::endl;

        testShader();

        // If a context 4.3 or above was created, test if compute shaders worked
        if (ite->first == 4 && ite->second >= 3)
        {
            testComputeShaderFloat();
            testComputeShaderUInt32();
        }
#endif

        // Release the context, it shouldn't be ready now
        GL_context.release();
        EXPECT_EQ(GL_context.isReady(), false);
    }
}



//---------------
void testShader()
//---------------
{
    // Instantiate a shader
    Shader dummy_shader;

    // Vertex shader
    const GLchar* g_vertex_shader = "\
        #version 330 core\n \
        layout (location = 0) in vec3 g_position; \n \
        \n \
        out vec4 g_colour;\n \
        \n \
        void main()\n \
        {\n \
            gl_Position = vec4(g_position, 1.0);\n \
            g_colour = vec4(0.5, 0.0, 0.0, 1.0);\n \
        }\n \
    ";

    // Fragment shader
    const GLchar* g_fragment_shader = "\
        #version 330 core\n \
        out vec4 FragColor;\n \
        \n \
        in vec4 g_colour;\n \
        \n \
        void main()\n \
        {\n \
            FragColor = g_colour;\n \
        }\n \
    ";

    // Give a text ID to the  shader, it can be useful when debuging the shader
    dummy_shader.setLabel("g_shader");

    // Load the source code of the shader onto the GPU
    dummy_shader.loadSource(g_vertex_shader, g_fragment_shader);

    // Check that the compute shader is valid
    EXPECT_EQ(dummy_shader.isValid(), true);
}


//---------------------------
void testComputeShaderFloat()
//---------------------------
{
    // Make sure Compute shaders are supported
    EXPECT_EQ(useOpenGLCompute(), true);

    // Instantiate a compute shader
    Shader dummy_shader;

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
        vec4 pixel_value = vec4(pixel_coordinates.x, pixel_coordinates.y, 1000.0, -1000.0);\n \
        imageStore(output_image, pixel_coordinates, pixel_value);\n \
    }\n \
    ";

    // Give a text ID to the compute  shader, it can be useful when debuging the shader
    dummy_shader.setLabel("g_compute_shader");

    // Load the source code of the shader onto the GPU
    dummy_shader.loadSource(g_compute_shader);

    // Check that the compute shader is valid
    EXPECT_EQ(dummy_shader.isValid(), true);
    
    // Generate the texture name
#ifdef HAS_OPENGL
    GLuint texture_id;
    glGenTextures(1, &texture_id);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Set the texture properties
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Make sure the texture can be created using a texture proxy
    GLint texture_width(0);
    glTexImage2D(GL_PROXY_TEXTURE_2D, 0, GL_RGBA32F, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGBA, GL_FLOAT, 0);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &texture_width);
    EXPECT_EQ(texture_width, TEXTURE_WIDTH);

    // Create the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGBA, GL_FLOAT, 0);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Bind the texture
    glBindImageTexture(0, texture_id, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Enable the compute shader
    dummy_shader.enable();
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Dispatch the compute shader
    glDispatchCompute(TEXTURE_WIDTH, TEXTURE_HEIGHT, 1);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Make sure all the calculations are performed
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Retrieve the data
    GLfloat* p_buffer = new GLfloat[TEXTURE_WIDTH*TEXTURE_HEIGHT*4];
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, p_buffer);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_GREEN, GL_FLOAT, p_buffer + TEXTURE_WIDTH*TEXTURE_HEIGHT);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_BLUE, GL_FLOAT, p_buffer + TEXTURE_WIDTH*TEXTURE_HEIGHT*2);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_ALPHA, GL_FLOAT, p_buffer + TEXTURE_WIDTH*TEXTURE_HEIGHT*3);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Check the data is as expected
    for (int j = 0; j < TEXTURE_HEIGHT; ++j)
    {
        for (int i = 0; i < TEXTURE_WIDTH; ++i)
        {
            unsigned int offset = (j * TEXTURE_WIDTH + i);
            float red = p_buffer[offset];
            float green = p_buffer[offset + TEXTURE_WIDTH*TEXTURE_HEIGHT];
            float blue = p_buffer[offset + TEXTURE_WIDTH*TEXTURE_HEIGHT * 2];
            float alpha = p_buffer[offset + TEXTURE_WIDTH*TEXTURE_HEIGHT * 3];

            EXPECT_FLOAT_EQ(red, i);
            EXPECT_FLOAT_EQ(green, j);
            EXPECT_FLOAT_EQ(blue, 1000.0);
            EXPECT_FLOAT_EQ(alpha, -1000.0);
        }
    }

    // Release memory
    delete [] p_buffer;
#endif
}


//----------------------------
void testComputeShaderUInt32()
//----------------------------
{
    // Make sure Compute shaders are supported
    EXPECT_EQ(useOpenGLCompute(), true);

    // Instantiate a compute shader
    Shader dummy_shader;

    // Compute shader
    const GLchar* g_compute_shader = "\
    \n#version 430\n \
    \n \
    layout(local_size_x=1, local_size_y=1) in;\n \
    \n \
    layout(rgba32ui, binding=0) uniform uimage2D output_image;\n \
    \n \
    void main(void)\n \
    {\n \
        ivec2 pixel_coordinates = ivec2(gl_GlobalInvocationID.xy);\n \
        uvec4 pixel_value = uvec4(pixel_coordinates.x, pixel_coordinates.y, 1000, 0);\n \
        imageStore(output_image, pixel_coordinates, pixel_value);\n \
    }\n \
    ";

    // Give a text ID to the compute  shader, it can be useful when debuging the shader
    dummy_shader.setLabel("g_compute_shader");

    // Load the source code of the shader onto the GPU
    dummy_shader.loadSource(g_compute_shader);

    // Check that the compute shader is valid
    EXPECT_EQ(dummy_shader.isValid(), true);

#ifdef HAS_OPENGL
    // Generate the texture name
    GLuint texture_id;
    glGenTextures(1, &texture_id);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Set the texture properties
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Make sure the texture can be created using a texture proxy
    GLint texture_width(0);
    glTexImage2D(GL_PROXY_TEXTURE_2D, 0, GL_RGBA32UI, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGB_INTEGER, GL_UNSIGNED_INT, 0);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &texture_width);
    EXPECT_EQ(texture_width, TEXTURE_WIDTH);

    // Create the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32UI, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGB_INTEGER, GL_UNSIGNED_INT, 0);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Bind the texture
    glBindImageTexture(0, texture_id, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32UI);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Enable the compute shader
    dummy_shader.enable();
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Dispatch the compute shader
    glDispatchCompute(TEXTURE_WIDTH, TEXTURE_HEIGHT, 1);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Make sure all the calculations are performed
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Retrieve the data
    std::vector<GLuint> p_buffer(TEXTURE_WIDTH*TEXTURE_HEIGHT*3);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, &p_buffer[0]);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_GREEN_INTEGER, GL_UNSIGNED_INT, &p_buffer[TEXTURE_WIDTH*TEXTURE_HEIGHT]);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_BLUE_INTEGER, GL_UNSIGNED_INT, &p_buffer[TEXTURE_WIDTH*TEXTURE_HEIGHT*2]);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Check the data is as expected
    for (unsigned int j = 0; j < TEXTURE_HEIGHT; ++j)
    {
        for (unsigned int i = 0; i < TEXTURE_WIDTH; ++i)
        {
            unsigned int offset = (j * TEXTURE_WIDTH + i);
            unsigned int red = p_buffer[offset];
            unsigned int green = p_buffer[offset + TEXTURE_WIDTH*TEXTURE_HEIGHT];
            unsigned int blue = p_buffer[offset + TEXTURE_WIDTH*TEXTURE_HEIGHT * 2];

            EXPECT_EQ(red, i);
            EXPECT_EQ(green, j);
            EXPECT_EQ(blue, (unsigned int) 1000);
        }
    }
#endif
}

