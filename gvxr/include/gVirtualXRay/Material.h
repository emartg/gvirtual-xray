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


#ifndef GVXR_MATERIAL_H
#define GVXR_MATERIAL_H


/**
********************************************************************************
*
*   @file       Material.h
*
*   @brief      Class to handle a material.
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
*   http://www.fpvidal.net/, Jan 2017, 2017, version 1.0, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*   @class  Material
*   @brief  Material is a class to handle materials.
*/
//==============================================================================
class Material
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    //--------------------------------------------------------------------------
    /// Default constructor.
    //--------------------------------------------------------------------------
    Material();


    //--------------------------------------------------------------------------
    /// Copy constructor.
    //--------------------------------------------------------------------------
    Material(const Material& aMaterial);


    //--------------------------------------------------------------------------
    /// Destructor.
    //--------------------------------------------------------------------------
    ~Material();


    //--------------------------------------------------------------------------
    /// Copy operator.
    //--------------------------------------------------------------------------
    Material& operator=(const Material& aMaterial);


    //--------------------------------------------------------------------------
    /// Set the ambient colour.
    /**
     *  @param aRedChannel:     red channel of the colour
     *  @param aGreenChannel:   green channel of the colour
     *  @param aBlueChannel:    blue channel of the colour
     *  @param aAlphaChannel:   alpha channel of the colour
     */
    //--------------------------------------------------------------------------
    void setAmbientColour(float aRedChannel,
            float aGreenChannel,
            float aBlueChannel,
            float aAlphaChannel);


    //--------------------------------------------------------------------------
    /// Set the diffuse colour.
    /**
     *  @param aRedChannel:     red channel of the colour
     *  @param aGreenChannel:   green channel of the colour
     *  @param aBlueChannel:    blue channel of the colour
     *  @param aAlphaChannel:   alpha channel of the colour
     */
    //--------------------------------------------------------------------------
    void setDiffuseColour(float aRedChannel,
            float aGreenChannel,
            float aBlueChannel,
            float aAlphaChannel);


    //--------------------------------------------------------------------------
    /// Set the specular colour.
    /**
     *  @param aRedChannel:     red channel of the colour
     *  @param aGreenChannel:   green channel of the colour
     *  @param aBlueChannel:    blue channel of the colour
     *  @param aAlphaChannel:   alpha channel of the colour
     */
    //--------------------------------------------------------------------------
    void setSpecularColour(float aRedChannel,
            float aGreenChannel,
            float aBlueChannel,
            float aAlphaChannel);


    //--------------------------------------------------------------------------
    /// Set the shininess of the material.
    /**
     *  @param aShininessValue: amount of shininess
     */
    //--------------------------------------------------------------------------
    void setShininess(float aShininessValue);


    //--------------------------------------------------------------------------
    /// Accessor on the ambient colour.
    /**
     *  @return the ambient colour
     */
    //--------------------------------------------------------------------------
    const float* getAmbientColour() const;


    //--------------------------------------------------------------------------
    /// Accessor on the diffuse colour.
    /**
     *  @return the diffuse colour
     */
    //--------------------------------------------------------------------------
    const float* getDiffuseColour() const;


    //--------------------------------------------------------------------------
    /// Accessor on the specular colour.
    /**
     *  @return the specular colour
     */
    //--------------------------------------------------------------------------
    const float* getSpecularColour() const;


    //--------------------------------------------------------------------------
    /// Accessor on the shininess of the material.
    /**
     *  @return the amount of shininess
     */
    //--------------------------------------------------------------------------
    float getShininess() const;


//******************************************************************************
protected:
    /// Ambient colour
    float m_p_ambient_colour[4];


    /// Diffuse colour
    float m_p_diffuse_colour[4];


    /// Specular colour
    float m_p_specular_colour[4];


    /// Shininess
    float m_shininess;
};


} // namespace gVirtualXRay


//******************************************************************************
#include "Material.inl"


#endif // GVXR_MATERIAL_H
