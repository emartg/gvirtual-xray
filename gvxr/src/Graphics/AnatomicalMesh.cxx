/**
********************************************************************************
*
*	@file		AnatomicalMesh.cxx
*
*	@brief      Class to handle a polygon mesh for tissue.
*
*	@version	1.0
*
*	@date		18/07/2016
*
*	@author		Andréas Meuleman
*
********************************************************************************
*/


//******************************************************************************
//	Include
//******************************************************************************
#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#ifndef GVXR_ANATOMICAL_MESH_H
#include "gVirtualXRay/AnatomicalMesh.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay{

//-------------------------------
AnatomicalMesh::AnatomicalMesh():
//-------------------------------
		PolygonMesh(),
		m_use_texture(0),
		m_use_bump_map(0),
	    m_label("unknown")
//-------------------------------
{
	getMaterial().setSpecularColour(1.0f,1.0f,1.0f,1.0f);
}

//-------------------------------
AnatomicalMesh::~AnatomicalMesh()
//-------------------------------
{
	// TODO Auto-generated destructor stub
}

//-------------------------------------------------------------------
void AnatomicalMesh::defineDisplayShaderInput(unsigned int aShaderID)
//-------------------------------------------------------------------
{
	// Get the material
	Material& material(getMaterial());

	// Define colours in shader
	GLint handle(glGetUniformLocation(aShaderID, "material_ambient"));
	glUniform4fv(handle, 1, &(material.getAmbientColour()[0]));
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	handle = glGetUniformLocation(aShaderID, "material_diffuse");
	glUniform4fv(handle, 1, &(material.getDiffuseColour()[0]));
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	handle = glGetUniformLocation(aShaderID, "material_specular");
	glUniform4fv(handle, 1, &(material.getSpecularColour()[0]));
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	handle = glGetUniformLocation(aShaderID, "material_shininess");
	glUniform1f(handle, material.getShininess());
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	handle = glGetUniformLocation(aShaderID, "g_use_texture");
	glUniform1iv(handle, 1, &m_use_texture);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

	handle = glGetUniformLocation(aShaderID, "g_use_bump_map");
	glUniform1iv(handle, 1, &m_use_bump_map);
	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}


//---------------------------------------------------------------
void AnatomicalMesh::copyFrom(const AnatomicalMesh& aPolygonMesh)
//---------------------------------------------------------------
{
    PolygonMesh::copyFrom(aPolygonMesh);

    m_use_texture = aPolygonMesh.m_use_texture;
    m_use_bump_map = aPolygonMesh.m_use_bump_map;
    m_label = aPolygonMesh.m_label;
}


//------------------------------------------------------------------------
AnatomicalMesh& AnatomicalMesh::operator=(const PolygonMesh& aPolygonMesh)
//------------------------------------------------------------------------
{
    PolygonMesh::copyFrom(aPolygonMesh);

    m_use_texture = 0;
    m_use_bump_map = 0;
    m_label = "unknown";

    getMaterial().setSpecularColour(1.0f,1.0f,1.0f,1.0f);

    return (*this);
}


//---------------------------------------------------------------------------
AnatomicalMesh& AnatomicalMesh::operator=(const AnatomicalMesh& aPolygonMesh)
//---------------------------------------------------------------------------
{
    AnatomicalMesh::copyFrom(aPolygonMesh);

    return (*this);
}


//-------------------------------------------------
const std::string& AnatomicalMesh::getLabel() const
//-------------------------------------------------
{
	return (m_label);
}


} // namespace gVirtualXRay
