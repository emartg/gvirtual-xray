/*

Copyright (c) 2023, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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
*   @file       LineMesh.inl
*
*   @brief      Class to handle 3D meshes of line segments.
*
*   @version    1.0
*
*   @date       20/11/2023
*
*   @author     Dr Franck P. Vidal
*
*   @todo       Make it possible to merge vertices if an array of indices exists
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*   http://www.fpvidal.net/, Nov 2023, 2023, version 1.0, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//-----------------------------------------------------------------
template<typename T> LineMesh<T>::LineMesh(const VEC3& aStartPoint,
                                           const VEC3& anEndPoint):
//-----------------------------------------------------------------
		PolygonMesh()
//-----------------------------------------------------------------
{
	create(aStartPoint, anEndPoint);
}


//--------------------------------------------------------------------
template<typename T> void LineMesh<T>::create(const VEC3& aStartPoint,
                                              const VEC3& anEndPoint)
//--------------------------------------------------------------------
{
    // Reset the mesh
    reset();
    m_external_data_flag = false;

    // Store vertices, normal vectors and indices
    std::vector<T> p_vertex_set;

    // Vertices
    p_vertex_set.push_back(aStartPoint.getX());
    p_vertex_set.push_back(aStartPoint.getY());
    p_vertex_set.push_back(aStartPoint.getZ());

    p_vertex_set.push_back(anEndPoint.getX());
    p_vertex_set.push_back(anEndPoint.getY());
    p_vertex_set.push_back(anEndPoint.getZ());

    setInternalData(GL_LINES,
                    &p_vertex_set,
                    false,
                    GL_STATIC_DRAW);
}

} // namespace gVirtualXRay
