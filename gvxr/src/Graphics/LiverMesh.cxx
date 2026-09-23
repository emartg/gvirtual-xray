/**
********************************************************************************
*
*	@file		LiverMesh.cxx
*
*	@brief      Class to handle a polygon mesh for a liver.
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
#ifndef GVXR_LIVER_MESH_H
#include "gVirtualXRay/LiverMesh.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//-----------------------------------------------------
LiverMesh::LiverMesh(bool optimize_texture_coordinates,
                     unsigned int texture_2D_size,
                     unsigned int texture_3D_size):
//-----------------------------------------------------
        SoftTissueMesh(optimize_texture_coordinates,
                texture_2D_size,
                texture_3D_size)
//-----------------------------------------------------
{
	getMaterial().setDiffuseColour(0.3,0.03,0.03,1.);
	getMaterial().setAmbientColour(0.3,0.03,0.03,1.);
	m_use_3D_bump_map = 1;
	m_label ="liver";
}


//----------------------------------------------
LiverMesh::~LiverMesh()
//----------------------------------------------
{
	// TODO Auto-generated destructor stub
}


} // namespace gVirtualXRay

