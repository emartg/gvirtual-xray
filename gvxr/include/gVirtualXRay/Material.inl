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
*	@file		Material.inl
*
*	@brief		Class to handle a material.
*
*	@version	1.0
*
*	@date		21/10/2014
*
*	@author		Dr Franck P. Vidal
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net), 
*   http://www.fpvidal.net/, Dec 2014, 2014, version 1.0, BSD 3-Clause License
*
********************************************************************************
*/

//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//-------------------------
inline Material::Material():
        m_shininess(50.0)
//-------------------------
{
    /// Ambient colour
    m_p_ambient_colour[0] = 1.0;
    m_p_ambient_colour[1] = 1.0;
    m_p_ambient_colour[2] = 1.0;
    m_p_ambient_colour[3] = 1.0;

    /// Diffuse colour
    m_p_diffuse_colour[0] = 1.0;
    m_p_diffuse_colour[1] = 1.0;
    m_p_diffuse_colour[2] = 1.0;
    m_p_diffuse_colour[3] = 1.0;

    /// Specular colour
    m_p_specular_colour[0] = 1.0;
    m_p_specular_colour[1] = 1.0;
    m_p_specular_colour[2] = 1.0;
    m_p_specular_colour[3] = 1.0;
}


//--------------------------------------------------
inline Material::Material(const Material& aMaterial)
//--------------------------------------------------
{
	*this = aMaterial;
}


//--------------------------
inline Material::~Material()
//--------------------------
{
}


//-------------------------------------------------------------
inline Material& Material::operator=(const Material& aMaterial)
//-------------------------------------------------------------
{
	// Ambient colour
	memcpy(m_p_ambient_colour, aMaterial.m_p_ambient_colour, sizeof(m_p_ambient_colour));

	// Diffuse colour
	memcpy(m_p_diffuse_colour, aMaterial.m_p_diffuse_colour, sizeof(m_p_diffuse_colour));

	// Specular colour
	memcpy(m_p_specular_colour, aMaterial.m_p_specular_colour, sizeof(m_p_specular_colour));

	// Shininess
	m_shininess = aMaterial.m_shininess;

	// Return the instance
	return (*this);
}


//---------------------------------------------------------
inline void Material::setAmbientColour(float aRedChannel,
                                       float aGreenChannel,
                                       float aBlueChannel,
                                       float aAlphaChannel)
//---------------------------------------------------------
{
    m_p_ambient_colour[0] = aRedChannel;
    m_p_ambient_colour[1] = aGreenChannel;
    m_p_ambient_colour[2] = aBlueChannel;
    m_p_ambient_colour[3] = aAlphaChannel;
}


//---------------------------------------------------------
inline void Material::setDiffuseColour(float aRedChannel,
                                       float aGreenChannel,
                                       float aBlueChannel,
                                       float aAlphaChannel)
//---------------------------------------------------------
{
    m_p_diffuse_colour[0] = aRedChannel;
    m_p_diffuse_colour[1] = aGreenChannel;
    m_p_diffuse_colour[2] = aBlueChannel;
    m_p_diffuse_colour[3] = aAlphaChannel;
}



//----------------------------------------------------------
inline void Material::setSpecularColour(float aRedChannel,
                                        float aGreenChannel,
                                        float aBlueChannel,
                                        float aAlphaChannel)
//----------------------------------------------------------
{
    m_p_specular_colour[0] = aRedChannel;
    m_p_specular_colour[1] = aGreenChannel;
    m_p_specular_colour[2] = aBlueChannel;
    m_p_specular_colour[3] = aAlphaChannel;
}


//-------------------------------------------------------
inline void Material::setShininess(float aShininessValue)
//-------------------------------------------------------
{
    m_shininess = aShininessValue;
}

//----------------------------------------------------
inline const float* Material::getAmbientColour() const
//----------------------------------------------------
{
    return (m_p_ambient_colour);
}


//----------------------------------------------------
inline const float* Material::getDiffuseColour() const
//----------------------------------------------------
{
    return (m_p_diffuse_colour);
}


//-----------------------------------------------------
inline const float* Material::getSpecularColour() const
//-----------------------------------------------------
{
    return (m_p_specular_colour);
}


//-----------------------------------------
inline float Material::getShininess() const
//-----------------------------------------
{
    return (m_shininess);
}


} // namespace gVirtualXRay
