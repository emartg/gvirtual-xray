/**
********************************************************************************
*
*	@file		SkinMesh.h
*
*	@brief      Class to handle a polygon mesh for Skin.
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
#ifndef GVXR_SKIN_MESH_H
#include "gVirtualXRay/SkinMesh.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {

//----------------------------------------
SkinMesh::SkinMesh():
//----------------------------------------
		AnatomicalMesh()
//----------------------------------------
{
	getMaterial().setShininess(5);
	getMaterial().setSpecularColour(0.1,0.1,0.1,0.05);
	getMaterial().setDiffuseColour(0.3,0.3,0.3,0.05);
	getMaterial().setAmbientColour(0.3,0.3,0.3,0.05);
	m_label = "skin";
}

//----------------------------------------
SkinMesh::~SkinMesh()
//----------------------------------------
{
	// TODO Auto-generated destructor stub
}

//----------------------------------------
bool SkinMesh::isTransparent() const
//----------------------------------------
{
	return true;
}

//----------------------------------------
bool SkinMesh::isSoftTissue() const
//----------------------------------------
{
	return true;
}

//----------------------------------------------
void SkinMesh::initialize()
//----------------------------------------------
{
	if(!m_external_data_flag && !m_p_index_set && !m_number_of_indices)
	{
		mergeVertices();
	}
}


} // namespace gVirtualXRay
