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
*	@file		ImplicitSurface.inl
*
*	@brief		Class to build 3D implicit surfaces. To know what implicit
*				surfaces are, please visit Paul Bourke's tutorial available at
*				http://paulbourke.net/geometry/implicitsurf/
*
*	@version	1.0
*
*	@date		08/03/2014
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
//	Include
//******************************************************************************
#include <cmath>

#ifndef GVXR_OUT_OF_BOUNDS_EXCEPTION_H
#include "gVirtualXRay/OutOfBoundsException.h"
#endif


//******************************************************************************
//	define
//******************************************************************************


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {
	

//----------------------------------------------------------------------------------------------------
inline RATIONAL_NUMBER ImplicitSurface::evaluate(RATIONAL_NUMBER r,
												 const DensityFunctionParameterType& aParameter) const
//----------------------------------------------------------------------------------------------------
{
	switch (aParameter.m_density_function)
	{
	case SPHERE:
		return (evaluateSphere(r));
		
	case BLOBBY_MOLECULE:
		return (evaluateBlobbyMolecule(r, aParameter.m_a, aParameter.m_b));
		
	case META_BALL:
		return (evaluateMetaBall(r, aParameter.m_a, aParameter.m_b));
		
	case SOFT_OBJECT:
		return (evaluateSoftObject(r, aParameter.m_a, aParameter.m_b));

	default:
		return (0);
	}
}


//-----------------------------------------------
inline void ImplicitSurface::resetControlPoints()
//-----------------------------------------------
{
	m_control_point_set.clear();
}


//----------------------------------------------
inline void ImplicitSurface::resetControlLines()
//----------------------------------------------
{
	m_control_line_set.clear();	
}


//------------------------------------------------------------------------------------------
inline void ImplicitSurface::addControlPoint(const DensityFunctionParameterType& aParameter,
											 const VEC3& aControlPoint)
//------------------------------------------------------------------------------------------
{
	m_control_point_set.push_back(std::pair<DensityFunctionParameterType, VEC3>(aParameter, aControlPoint));
}


//-----------------------------------------------------------------------------------------
inline void ImplicitSurface::addControlLine(const DensityFunctionParameterType& aParameter,
											const VEC3& aControlPoint1,
											const VEC3& aControlPoint2)
//-----------------------------------------------------------------------------------------
{
	m_control_line_set.push_back(std::pair<DensityFunctionParameterType, std::pair<VEC3, VEC3> >(aParameter, std::pair<VEC3, VEC3>(aControlPoint1, aControlPoint2)));
}


//-------------------------------------------------------------------
inline unsigned int ImplicitSurface::getNumberOfControlPoints() const
//-------------------------------------------------------------------
{
	return m_control_point_set.size();
}


//-------------------------------------------------------------------------
inline unsigned int ImplicitSurface::getNumberOfControlLineSegments() const
//-------------------------------------------------------------------------
{
    return m_control_line_set.size();
}


//------------------------------------------------------------------------------------------
inline void ImplicitSurface::setControlPointParameters(unsigned int aPointID,
													   DensityFunctionType aDentityFunction,
													   float aParameter1,
													   float aParameter2)
//------------------------------------------------------------------------------------------
{
	if (aPointID >= m_control_point_set.size())
	{
		OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}

	m_control_point_set[aPointID].first.m_density_function = aDentityFunction;
	m_control_point_set[aPointID].first.m_a = aParameter1;
	m_control_point_set[aPointID].first.m_b = aParameter2;
}
                                

//------------------------------------------------------------------------------------------------
inline void ImplicitSurface::setControlLineSegmentParameters(unsigned int aLineSegmentID,
															 DensityFunctionType aDentityFunction,
															 float aParameter1,
															 float aParameter2)
//------------------------------------------------------------------------------------------------
{
	if (aLineSegmentID >= m_control_line_set.size())
	{
		OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}

	m_control_line_set[aLineSegmentID].first.m_density_function = aDentityFunction;
	m_control_line_set[aLineSegmentID].first.m_a = aParameter1;
	m_control_line_set[aLineSegmentID].first.m_b = aParameter2;
}


//-----------------------------------------------------------------------------
inline const VEC3& ImplicitSurface::getControlPoint(unsigned int anIndex) const
//-----------------------------------------------------------------------------
{
	if (anIndex >= m_control_point_set.size())
	{
		OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}

	return (m_control_point_set[anIndex].second);
}


//-----------------------------------------------------------------
inline VEC3& ImplicitSurface::getControlPoint(unsigned int anIndex)
//-----------------------------------------------------------------
{
	if (anIndex >= m_control_point_set.size())
	{
		OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}

	return (m_control_point_set[anIndex].second);
}


//---------------------------------------------------------------------------------------------
inline const std::pair<VEC3, VEC3>& ImplicitSurface::getControlLine(unsigned int anIndex) const
//---------------------------------------------------------------------------------------------
{
	if (anIndex >= m_control_line_set.size())
	{
		OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}
	
	return (m_control_line_set[anIndex].second);
}


//---------------------------------------------------------------------------------
inline std::pair<VEC3, VEC3>& ImplicitSurface::getControlLine(unsigned int anIndex)
//---------------------------------------------------------------------------------
{
	if (anIndex >= m_control_line_set.size())
	{
		OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
	}
	
	return (m_control_line_set[anIndex].second);
}


//------------------------------------------------------------------
inline void ImplicitSurface::voxelize(const Vec3ui& aNumberOfVoxels,
									  const VEC3& aCenter,
									  const VEC3& aVoxelSize,
									  unsigned char aVerboseLevel)
//------------------------------------------------------------------
{
	voxelise(aNumberOfVoxels, aCenter, aVoxelSize, aVerboseLevel);
}


//-----------------------------------------------------------------------------
inline void ImplicitSurface::writeVoxelData(const std::string& aFileName) const
//-----------------------------------------------------------------------------
{
	writeVoxelData(aFileName.data());
}


//-------------------------------------------------------------------------
inline void ImplicitSurface::writePolygonMesh(const std::string& aFileName)
//-------------------------------------------------------------------------
{
	writePolygonMesh(aFileName.data());
}

/*
//-------------------------------------------------------------------------------
inline const std::vector<RATIONAL_NUMBER>& ImplicitSurface::getVertexData() const
//-------------------------------------------------------------------------------
{
	return (m_p_vertex_data);
}


//---------------------------------------------------------------------------
inline const std::vector<unsigned int>& ImplicitSurface::getIndexData() const
//---------------------------------------------------------------------------
{
	return (m_p_index_data);
}
*/


//---------------------------------------------------
inline PolygonMesh& ImplicitSurface::getPolygonMesh()
//---------------------------------------------------
{
    return (m_mesh_data);
}


//---------------------------------------------------------------
inline const PolygonMesh& ImplicitSurface::getPolygonMesh() const
//---------------------------------------------------------------
{
    return (m_mesh_data);
}


//---------------------------------------------------------------------
inline const Image<RATIONAL_NUMBER>& ImplicitSurface::getVolume() const
//---------------------------------------------------------------------
{
    return (m_volume);
}


//---------------------------------------------------------
inline Image<RATIONAL_NUMBER>& ImplicitSurface::getVolume()
//---------------------------------------------------------
{
    return (m_volume);
}


//-----------------------------------------------------------------------------
inline RATIONAL_NUMBER ImplicitSurface::evaluateSphere(RATIONAL_NUMBER r) const
//-----------------------------------------------------------------------------
{
	if (0 <= r)
	{
		return (1.0 / (r * r));		
	}
	else
	{
		return (0);
	}
}


//-------------------------------------------------------------------------------------
inline RATIONAL_NUMBER ImplicitSurface::evaluateBlobbyMolecule(RATIONAL_NUMBER r,
															   RATIONAL_NUMBER a,
															   RATIONAL_NUMBER b) const
//-------------------------------------------------------------------------------------
{
	if (0 <= r)
	{
		return (a * exp(-b * r * r));		
	}
	else
	{
		return (0);
	}
}


//-------------------------------------------------------------------------------
inline RATIONAL_NUMBER ImplicitSurface::evaluateMetaBall(RATIONAL_NUMBER r,
														 RATIONAL_NUMBER a,
														 RATIONAL_NUMBER b) const
//-------------------------------------------------------------------------------
{
	if (0 <= r && r <= b / 3.0)
	{
		return (a * (1.0 - (3.0 * r * r) / (b * b)));
	}
	else if (b / 3.0 <= r && r <= b)
	{
		return (((3.0 * a) / 2.0) * pow((1.0 - (r / b)), 2.0));
	}
	else //if (m_b <= r)
	{
		return (0);
	}
}


//---------------------------------------------------------------------------------
inline RATIONAL_NUMBER ImplicitSurface::evaluateSoftObject(RATIONAL_NUMBER r,
														   RATIONAL_NUMBER a,
														   RATIONAL_NUMBER b) const
//---------------------------------------------------------------------------------
{
	if (0 <= r && r <= b)
	{
		return (a * 
				(1.0 - 
						(( 4.0 * std::pow(r, 6)) / (9.0 * std::pow(b, 6))) + 
						((17.0 * std::pow(r, 4)) / (9.0 * std::pow(b, 4))) - 
						((22.0 * std::pow(r, 2)) / (9.0 * std::pow(b, 2)))));
	}
	else
	{
		return (0);
	}
}


} // namespace gVirtualXRay
