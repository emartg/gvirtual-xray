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
*   @file       TextRenderer.h
*
*   @brief      Class to render text using OpenGL and FreeType2.
*
*   @version    1.0
*
*   @date       16/11/2014
*
*   @author     Dr Franck P. Vidal
*
*   @section	License
*               BSD 3-Clause License.
*
*               For details on use and redistribution please refer
*               to http://opensource.org/licenses/BSD-3-Clause
*
*   @section    Copyright
*               (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*               http://www.fpvidal.net/, Dec 2014, 2014, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <iostream>

#ifndef GVXR_OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef GVXR_TEXT_RENDERER_H
#include "gVirtualXRay/TextRenderer.h"
#endif

#ifndef GVXR_UTILITIES_H
#include "gVirtualXRay/Utilities.h"
#endif

#ifdef HAS_FREETYPE
#include "FreeSans.ttf.h"

#ifndef GVXR_LOGGER_H
#include <gVirtualXRay/Logger.h>
#endif

#include "text_display_gl2.frag.h"
#include "text_display_gl2.vert.h"

#include "text_display_gl3.frag.h"
#include "text_display_gl3.vert.h"

#include "text_display_gl4.frag.h"
#include "text_display_gl4.vert.h"
#endif


//******************************************************************************
//	Name space
//******************************************************************************
using namespace gVirtualXRay;


//---------------------------------------------------
void TextRenderer::initialise(unsigned int aFontSize)
//---------------------------------------------------
{
    if (!m_initialised)
    {
#ifdef HAS_FREETYPE
        if (FT_Init_FreeType(&m_free_type))
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "Could not initialise the FreeType library.");
        }

        int file_size(0);
        static FT_Byte* gvxr_p_font_file_data(0);

        file_size = inflate(g_FreeSans_ttf, sizeof(g_FreeSans_ttf), (char**) &gvxr_p_font_file_data);

        FT_Error error(FT_New_Memory_Face(m_free_type, (FT_Byte*) gvxr_p_font_file_data, file_size, 0, &m_face));
        if (error == FT_Err_Unknown_File_Format)
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "Could not open the font: unknown file format.");
        }
        else if (error)
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "Could not open the font.");
        }

        delete [] gvxr_p_font_file_data;
        gvxr_p_font_file_data = 0;

        initialiseInternalStates(aFontSize);

#endif // HAS_FREETYPE

        m_initialised = true;
    }
}


//--------------------------------------------------------
void TextRenderer::initialise(const std::string aFontFile,
                              unsigned int aFontSize)
//--------------------------------------------------------
{
    if (!m_initialised)
    {
#ifdef HAS_FREETYPE
        if (FT_Init_FreeType(&m_free_type))
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "Could not initialise the FreeType library.");
        }

        if (FT_New_Face(m_free_type, aFontFile.data(), 0, &m_face))
        {
        	std::stringstream error_message;
        	error_message << "Could not open the font in file \"" << aFontFile << "\".";

            throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
        }

        initialiseInternalStates(aFontSize);

#endif // HAS_FREETYPE

        m_initialised = true;
    }
}


//-----------------------------------------------------------------------
void TextRenderer::renderText(const std::string& aText,
                              float x, float y,
                              Matrix4x4<GLfloat>& aProjectionMatrix,
                              GLfloat* apBackgroundColour,
                              GLfloat* apFontColour)
//-----------------------------------------------------------------------
{
#ifdef HAS_FREETYPE
    if (!m_initialised)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot render text, the font has not been initialised.");
    }

    // Save the current texture
    pushTexture2D();

    m_display_text_shader.enable();
    GLint shader_id(m_display_text_shader.getProgramHandle());

    GLuint handle(glGetUniformLocation(shader_id, "g_projection_matrix"));
    glUniformMatrix4fv(handle, 1, GL_FALSE, aProjectionMatrix.get());
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader_id, "imageTexture"), 0);

    glUniform4fv(glGetUniformLocation(shader_id, "backgroundColour"), 1, apBackgroundColour);
    glUniform4fv(glGetUniformLocation(shader_id, "fontColour"), 1, apFontColour);

    // Process all the characters
    for (unsigned int i(0); i < aText.size(); ++i)
    {
        // Find the character
        std::map<char, Character>::const_iterator ite(m_character_set.find(aText[i]));

        // The character is not in the set yet
        if (ite == m_character_set.end())
        {
            FT_GlyphSlot  slot(m_face->glyph);

            if (FT_Load_Char(m_face, aText[i], FT_LOAD_RENDER))
            {
                LOGGER.logWarning("Could not load character ") << aText[i] << std::endl;
            }
            else
            {
                m_character_set.insert(std::pair<char, Character>(aText[i], Character(0, slot->bitmap.width, slot->bitmap.rows, slot->bitmap_left, slot->bitmap_top,
                        slot->advance.x >> 6,
                        slot->advance.y >> 6)));

                glGenTextures(1, &(m_character_set[aText[i]].m_texture_id));
                glBindTexture(GL_TEXTURE_2D, m_character_set[aText[i]].m_texture_id);


                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                glPixelStorei(GL_PACK_ALIGNMENT, 1);
                glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

                unsigned char* p_input_buffer(slot->bitmap.buffer);
                unsigned char* p_output_buffer(new unsigned char[slot->bitmap.width * slot->bitmap.rows]);
                unsigned char* p_temp(p_output_buffer);
                for (unsigned int j(0); j < slot->bitmap.width * slot->bitmap.rows; ++j, ++p_input_buffer)
                {
                    if (*p_input_buffer == 0)
                    {
                        *p_temp++ = 0;
                    }
                    else
                    {
                        *p_temp++ = 255;
                    }
                }

                glTexImage2D(
                        GL_TEXTURE_2D,
                        0,
                        GL_RED,
                        slot->bitmap.width,
                        slot->bitmap.rows,
                        0,
                        GL_RED,
                        GL_UNSIGNED_BYTE,
                        p_output_buffer);
            }
        }

        const Character& character(m_character_set[aText[i]]);

        glBindTexture(GL_TEXTURE_2D, character.m_texture_id);

        MATRIX4 model_view;
        model_view.translate(x, y - character.m_vy, 0);
        model_view.scale(character.m_width, character.m_height, 1);

        GLuint handle(glGetUniformLocation(shader_id, "g_modelview_matrix"));
        glUniformMatrix4fv(handle, 1, GL_FALSE, model_view.get());
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        m_text_vbo->display();

        x += character.m_x_offset;
        y += character.m_y_offset;
    }

    // Restore the saved texture
    popTexture();

#endif
}


//-----------------------------------------------------------------
void TextRenderer::initialiseInternalStates(unsigned int aFontSize)
//-----------------------------------------------------------------
{
    if (!m_initialised)
    {
#ifdef HAS_FREETYPE
        if (FT_Set_Char_Size(m_face, // handle to face object
                0, // char_width in 1/64th of points
                aFontSize, // char_height in 1/64th of points
                300, // horizontal device resolution
                300 )) // vertical device resolution
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "Could not set the char size.");
        }

        std::vector<GLfloat> p_vertices;
        p_vertices.push_back(0);
        p_vertices.push_back(0);
        p_vertices.push_back(0);

        p_vertices.push_back(1);
        p_vertices.push_back(0);
        p_vertices.push_back(0);

        p_vertices.push_back(1);
        p_vertices.push_back(1);
        p_vertices.push_back(0);

        p_vertices.push_back(0);
        p_vertices.push_back(0);
        p_vertices.push_back(0);

        p_vertices.push_back(1);
        p_vertices.push_back(1);
        p_vertices.push_back(0);

        p_vertices.push_back(0);
        p_vertices.push_back(1);
        p_vertices.push_back(0);

        // Texture coordinates
        std::vector<float> p_texture_coordinates;

        p_texture_coordinates.push_back(0);
        p_texture_coordinates.push_back(0);
        p_texture_coordinates.push_back(0);

        p_texture_coordinates.push_back(1);
        p_texture_coordinates.push_back(0);
        p_texture_coordinates.push_back(0);

        p_texture_coordinates.push_back(1);
        p_texture_coordinates.push_back(1);
        p_texture_coordinates.push_back(0);

        p_texture_coordinates.push_back(0);
        p_texture_coordinates.push_back(0);
        p_texture_coordinates.push_back(0);

        p_texture_coordinates.push_back(1);
        p_texture_coordinates.push_back(1);
        p_texture_coordinates.push_back(0);

        p_texture_coordinates.push_back(0);
        p_texture_coordinates.push_back(1);
        p_texture_coordinates.push_back(0);

        if (!m_text_vbo.get())
        {
            m_text_vbo.reset(createVBO());
        }

        m_text_vbo->setBufferUsageHint(GL_STATIC_DRAW);
        m_text_vbo->setVertexData(GL_TRIANGLES,
            p_vertices.size() / 3, 3, GL_FLOAT, &p_vertices[0],
            0, 0, GL_NONE, 0,
            p_texture_coordinates.size() / 3, 3, GL_FLOAT, &p_texture_coordinates[0]);

        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        // Initialise the shaders
        char* p_vertex_shader(0);
        char* p_fragment_shader(0);

        int z_lib_return_code_vertex(0);
        int z_lib_return_code_fragment(0);

        std::string vertex_shader;
        std::string fragment_shader;

        if (useOpenGL45())
    	{
    	    z_lib_return_code_vertex   = inflate(g_text_display_gl4_vert, sizeof(g_text_display_gl4_vert),   &p_vertex_shader);
    	    z_lib_return_code_fragment = inflate(g_text_display_gl4_frag, sizeof(g_text_display_gl4_frag), &p_fragment_shader);
    	}
        else if (useOpenGL32())
    	{
    	    z_lib_return_code_vertex   = inflate(g_text_display_gl3_vert, sizeof(g_text_display_gl3_vert),   &p_vertex_shader);
    	    z_lib_return_code_fragment = inflate(g_text_display_gl3_frag, sizeof(g_text_display_gl3_frag), &p_fragment_shader);
    	}
    	else
    	{
          z_lib_return_code_vertex   = inflate(g_text_display_gl2_vert, sizeof(g_text_display_gl2_vert),   &p_vertex_shader);
          z_lib_return_code_fragment = inflate(g_text_display_gl2_frag, sizeof(g_text_display_gl2_frag), &p_fragment_shader);
            // throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot create shader programs for your chosen OpenGL context.");
    	}

        vertex_shader   = p_vertex_shader;
        fragment_shader = p_fragment_shader;
        delete [] p_vertex_shader;     p_vertex_shader = 0;
        delete [] p_fragment_shader; p_fragment_shader = 0;

        if (z_lib_return_code_vertex <= 0 || z_lib_return_code_fragment <= 0 || !vertex_shader.size() || !fragment_shader.size())
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot decode the shader using ZLib.");
        }
        m_display_text_shader.setLabels("text_display.vert", "text_display.frag");
        m_display_text_shader.loadSource(vertex_shader, fragment_shader);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
#endif
    }
}
