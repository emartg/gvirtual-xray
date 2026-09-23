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


#ifndef GVXR_TEXT_RENDERER_H
#define GVXR_TEXT_RENDERER_H


/**
********************************************************************************
*
*   @file       TextRenderer.h
*
*   @brief      Class to render text using OpenGL and FreeType2.
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
#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <map>
#include <memory>

#ifdef HAS_FREETYPE
#include <ft2build.h>
#include FT_FREETYPE_H
#endif

#ifdef __VBO_h
#include "VBO.h"
#endif

#ifndef GVXR_SHADER_H
#include "gVirtualXRay/Shader.h"
#endif

#ifdef GVXR_VEC3_H
#include "Vec3.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*   @class  TextRenderer
*   @brief  TextRenderer is a class to render text using OpenGL and FreeType2.
*/
//==============================================================================
class TextRenderer
//------------------------------------------------------------------------------
{
//******************************************************************************
private:
    //==========================================================================
    /**
    *   @class  Character
    *   @brief  Character is a class to handle characters rendered using OpenGL
    *           and FreeType2.
    */
    //==========================================================================
    class Character
    {
    //**************************************************************************
    public:
        //----------------------------------------------------------------------
        /// Default constructor
        /**
         *  @param aTextureID   the ID of the texture (default value: 0)
         *  @param aWidth   the width of the bitmap (default value: 0)
         *  @param aHeight   the height of the bitmap (default value: 0)
         *  @param aVX   the horizontal alignment parameter (default value: 0)
         *  @param aVY   the vertical alignment parameter (default value: 0)
         *  @param aXOffset   the horizontal offset (default value: 0)
         *  @param aYOffset   the vertical offset (default value: 0)
         */
        //----------------------------------------------------------------------
        Character(unsigned int aTextureID = 0,
                  int aWidth = 0,
                  int aHeight = 0,
                  float aVX = 0,
                  float aVY = 0,
                  float aXOffset = 0,
                  float aYOffset = 0);


        //----------------------------------------------------------------------
        /// Destructor
        //----------------------------------------------------------------------
        virtual ~Character();


        void release();
            

        unsigned int m_texture_id;  ///< The ID of the texture
        int          m_width;       ///< The width of the bitmap
        int          m_height;      ///< The height of the bitmap
        float        m_vx;          ///< The horizontal alignment parameter
        float        m_vy;          ///< The vertical alignment parameter
        float        m_x_offset;    ///< The horizontal offset
        float        m_y_offset;    ///< The vertical offset
    };

    
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default constructor.
    //--------------------------------------------------------------------------
    inline TextRenderer();


    //--------------------------------------------------------------------------
    /// Destructor.
    //--------------------------------------------------------------------------
    virtual ~TextRenderer();


    void release();

    
    //--------------------------------------------------------------------------
    /// Initialise the default font.
    /**
    *   @param aFontSize   the font size (default value: 16*38)
    */
    //--------------------------------------------------------------------------
    void initialise(unsigned int aFontSize = 16*38);


    //--------------------------------------------------------------------------
    /// Initialise the default font.
    /**
    *   @param aFontSize   the font size (default value: 16*38)
    */
    //--------------------------------------------------------------------------
    void initialize(unsigned int aFontSize = 16*38);


    //--------------------------------------------------------------------------
    /// Initialise the given font.
    /**
    *   @param aFontFile   the font file
    *   @param aFontSize   the font size (default value: 16*38)
    */
    //--------------------------------------------------------------------------
    void initialise(const std::string aFontFile, unsigned int aFontSize = 16*38);


    //--------------------------------------------------------------------------
    /// Initialise the given font.
    /**
    *   @param aFontFile   the font file
    *   @param aFontSize   the font size (default value: 16*38)
    */
    //--------------------------------------------------------------------------
    void initialize(const std::string aFontFile, unsigned int aFontSize = 16*38);


    //--------------------------------------------------------------------------
    /// Display some text.
    /**
    *   @param aText    the text to display
    *   @param x    the x-position of the text
    *   @param y    the y-position of the text
    *   @param aProjectionMatrix    the projection matrix
    *   @param apBackgroundColour   the background colour (RGBA)
    *   @param apFontColour the font colour (RGBA)
    */
    //--------------------------------------------------------------------------
    void renderText(const std::string& aText,
            float x, float y,
            Matrix4x4<GLfloat>& aProjectionMatrix,
            GLfloat* apBackgroundColour,
            GLfloat* apFontColour);

    
    //--------------------------------------------------------------------------
    /// Accessor on the initialisation flag.
    /**
    *   @return true if the text renderer is initialised, false if it is not
    */
    //--------------------------------------------------------------------------
    bool isInitialised() const;


//******************************************************************************
protected:
    //--------------------------------------------------------------------------
    /// Initialise internal states.
    /**
    *   @param aFontSize   the font size
    */
    //--------------------------------------------------------------------------
    void initialiseInternalStates(unsigned int aFontSize);


#ifdef HAS_FREETYPE
    /// The FreeType library main object
    FT_Library m_free_type;


    /// The font face
    FT_Face m_face;


    /// The cache of characters
    std::map<char, Character> m_character_set;


    /// The VBO used to display the characters
#if __cplusplus < 199711L
    std::auto_ptr<VBO> m_text_vbo;
#else
    std::unique_ptr<VBO> m_text_vbo;
#endif


    /// The shader used to display the text
    Shader m_display_text_shader;
#endif

    /// Flag set to true when the fond is initialised
    bool m_initialised;
};


} // namespace gVirtualXRay


//******************************************************************************
#include "TextRenderer.inl"


#endif // GVXR_TEXT_RENDERER_H
