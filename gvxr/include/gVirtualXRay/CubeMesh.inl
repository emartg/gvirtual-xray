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
*	@file		CubeMesh.inl
*
*	@brief		Class to handle 3D meshes of cubes.
*
*	@version	1.0
*
*	@date		23/03/2014
*
*	@author		Dr Franck P. Vidal
*
*	@todo		Make it possible to merge vertices if an array of indices exists
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


//-------------------------------------------------------------
template<typename T> CubeMesh<T>::CubeMesh(double aLength,
                                           int anIndexDataType):
//-------------------------------------------------------------
		PolygonMesh()
//-------------------------------------------------------------
{
	create(aLength, anIndexDataType);
}


//----------------------------------------------------------------
template<typename T> void CubeMesh<T>::create(double aLength,
                                              int anIndexDataType)
//----------------------------------------------------------------
{
    // Reset the mesh
    reset();
    m_external_data_flag = false;

    // Cache the half length
    double half_length(aLength / 2.0);

    m_local_min_corner = VEC3(-half_length, -half_length, -half_length);
    m_local_max_corner = VEC3( half_length,  half_length,  half_length);

    // Find the 8 corners

    // Top
    VEC3 top_far_left(  -half_length, half_length, -half_length);
    VEC3 top_near_left( -half_length, half_length,  half_length);
    VEC3 top_far_right(  half_length, half_length, -half_length);
    VEC3 top_near_right( half_length, half_length,  half_length);

    // Bottom
    VEC3 bottom_far_left(  top_far_left.getX(),   -top_far_left.getY(),   top_far_left.getZ());
    VEC3 bottom_near_left( top_near_left.getX(),  -top_near_left.getY(),  top_near_left.getZ());
    VEC3 bottom_far_right( top_far_right.getX(),  -top_far_right.getY(),  top_far_right.getZ());
    VEC3 bottom_near_right(top_near_right.getX(), -top_near_right.getY(), top_near_right.getZ());

    // Store vertices, normal vectors and indices
    std::vector<T> p_vertex_set;
    std::vector<T> p_normal_set;
    std::vector<unsigned char> p_ubyte_index_set;

    // Vertices
    p_vertex_set.push_back(top_far_left.getX());
    p_vertex_set.push_back(top_far_left.getY());
    p_vertex_set.push_back(top_far_left.getZ());

    p_vertex_set.push_back(top_near_left.getX());
    p_vertex_set.push_back(top_near_left.getY());
    p_vertex_set.push_back(top_near_left.getZ());

    p_vertex_set.push_back(top_far_right.getX());
    p_vertex_set.push_back(top_far_right.getY());
    p_vertex_set.push_back(top_far_right.getZ());

    p_vertex_set.push_back(top_near_right.getX());
    p_vertex_set.push_back(top_near_right.getY());
    p_vertex_set.push_back(top_near_right.getZ());

    p_vertex_set.push_back(bottom_far_left.getX());
    p_vertex_set.push_back(bottom_far_left.getY());
    p_vertex_set.push_back(bottom_far_left.getZ());

    p_vertex_set.push_back(bottom_near_left.getX());
    p_vertex_set.push_back(bottom_near_left.getY());
    p_vertex_set.push_back(bottom_near_left.getZ());

    p_vertex_set.push_back(bottom_far_right.getX());
    p_vertex_set.push_back(bottom_far_right.getY());
    p_vertex_set.push_back(bottom_far_right.getZ());

    p_vertex_set.push_back(bottom_near_right.getX());
    p_vertex_set.push_back(bottom_near_right.getY());
    p_vertex_set.push_back(bottom_near_right.getZ());

    // Indices and normal vectors

    // Top face
    p_ubyte_index_set.push_back(1);
    p_ubyte_index_set.push_back(2);
    p_ubyte_index_set.push_back(0);

    p_ubyte_index_set.push_back(2);
    p_ubyte_index_set.push_back(1);
    p_ubyte_index_set.push_back(3);

    for (unsigned int i = 0; i < 2; ++i)
    {
    	p_normal_set.push_back(0);
		p_normal_set.push_back(1);
		p_normal_set.push_back(0);
    }

    // Bottom face
    p_ubyte_index_set.push_back(6);
    p_ubyte_index_set.push_back(5);
    p_ubyte_index_set.push_back(4);

    p_ubyte_index_set.push_back(5);
    p_ubyte_index_set.push_back(6);
    p_ubyte_index_set.push_back(7);

    for (unsigned int i = 0; i < 2; ++i)
    {
    	p_normal_set.push_back(0);
		p_normal_set.push_back(-1);
		p_normal_set.push_back(0);
    }

    // Front face
    p_ubyte_index_set.push_back(5);
    p_ubyte_index_set.push_back(3);
    p_ubyte_index_set.push_back(1);

    p_ubyte_index_set.push_back(3);
    p_ubyte_index_set.push_back(5);
    p_ubyte_index_set.push_back(7);

    for (unsigned int i = 0; i < 2; ++i)
    {
    	p_normal_set.push_back(0);
		p_normal_set.push_back(0);
		p_normal_set.push_back(1);
    }

    // Back face
    p_ubyte_index_set.push_back(2);
    p_ubyte_index_set.push_back(4);
    p_ubyte_index_set.push_back(0);

    p_ubyte_index_set.push_back(4);
    p_ubyte_index_set.push_back(2);
    p_ubyte_index_set.push_back(6);

    for (unsigned int i = 0; i < 2; ++i)
    {
    	p_normal_set.push_back(0);
		p_normal_set.push_back(0);
		p_normal_set.push_back(-1);
    }

    // Left face
    p_ubyte_index_set.push_back(5);
    p_ubyte_index_set.push_back(0);
    p_ubyte_index_set.push_back(4);

    p_ubyte_index_set.push_back(0);
    p_ubyte_index_set.push_back(5);
    p_ubyte_index_set.push_back(1);

    for (unsigned int i = 0; i < 2; ++i)
    {
    	p_normal_set.push_back(-1);
		p_normal_set.push_back(0);
		p_normal_set.push_back(0);
    }

    // Right face
    p_ubyte_index_set.push_back(2);
    p_ubyte_index_set.push_back(7);
    p_ubyte_index_set.push_back(6);

    p_ubyte_index_set.push_back(7);
    p_ubyte_index_set.push_back(2);
    p_ubyte_index_set.push_back(3);

    for (unsigned int i = 0; i < 2; ++i)
    {
    	p_normal_set.push_back(1);
		p_normal_set.push_back(0);
		p_normal_set.push_back(0);
    }

    if (anIndexDataType)
	{
    	// Use vertex normals
    	std::vector<T> p_normal_set2;

    	for (unsigned int vertex_id = 0; vertex_id < p_vertex_set.size() / 3; ++vertex_id)
    	{
    		double x = 0;
    		double y = 0;
    		double z = 0;

    		unsigned int counter = 0;

        	for (unsigned int face_id = 0; face_id < p_ubyte_index_set.size() / 3; ++face_id)
    		{
        		if (p_ubyte_index_set[face_id * 3] == vertex_id ||
        				p_ubyte_index_set[face_id * 3 + 1] == vertex_id ||
						p_ubyte_index_set[face_id * 3 + 2] == vertex_id)
        		{
        			counter++;
        			x += p_normal_set[face_id * 3];
        			y += p_normal_set[face_id * 3 + 1];
        			z += p_normal_set[face_id * 3 + 2];
        		}
    		}

    		if (counter > 0)
    		{
    			x /= counter;
    			y /= counter;
    			z /= counter;
    		}

    		p_normal_set2.push_back(x);
    		p_normal_set2.push_back(y);
    		p_normal_set2.push_back(z);
    	}

        switch (anIndexDataType)
            {
            case GL_UNSIGNED_INT:
                {
                    std::vector<GLuint> p_uint_index_set;

                    for (std::vector<GLubyte>::const_iterator ite(p_ubyte_index_set.begin());
                            ite != p_ubyte_index_set.end();
                            ++ite)
                    {
                        p_uint_index_set.push_back(*ite);
                    }

                    setInternalData(GL_TRIANGLES,
                            &p_vertex_set,
                            &p_normal_set2,
                            &p_uint_index_set,
                            false,
                            GL_STATIC_DRAW);
                }
                break;

            case GL_UNSIGNED_SHORT:
                {
                    std::vector<GLushort> p_ushort_index_set;

                    for (std::vector<GLubyte>::const_iterator ite(p_ubyte_index_set.begin());
                            ite != p_ubyte_index_set.end();
                            ++ite)
                    {
                        p_ushort_index_set.push_back(*ite);
                    }

                    setInternalData(GL_TRIANGLES,
                            &p_vertex_set,
                            &p_normal_set2,
                            &p_ushort_index_set,
                            false,
                            GL_STATIC_DRAW);
                }
                break;

            case GL_UNSIGNED_BYTE:
                {
                    setInternalData(GL_TRIANGLES,
                            &p_vertex_set,
                            &p_normal_set2,
                            &p_ubyte_index_set,
                            false,
                            GL_STATIC_DRAW);
                }
                break;

            default:
                throw Exception(__FILE__, __FUNCTION__, __LINE__,
                        "Invalid data type.");
            }
        }
    // Don't use an index
    else
    {
        std::vector<T> p_vertex_set2;
    	std::vector<T> p_normal_set2;

	    int normal_id = 0;
    	for (std::vector<unsigned char>::const_iterator ite = p_ubyte_index_set.begin();
    			ite != p_ubyte_index_set.end();
    			++ite, ++normal_id)
    	{
    		p_vertex_set2.push_back(p_vertex_set[*ite * 3 + 0]);
    		p_vertex_set2.push_back(p_vertex_set[*ite * 3 + 1]);
    		p_vertex_set2.push_back(p_vertex_set[*ite * 3 + 2]);

    		p_normal_set2.push_back(p_normal_set[3 * (normal_id / 3) + 0]);
    		p_normal_set2.push_back(p_normal_set[3 * (normal_id / 3) + 1]);
    		p_normal_set2.push_back(p_normal_set[3 * (normal_id / 3) + 2]);
    	}

        setInternalData(GL_TRIANGLES,
                        &p_vertex_set2,
                        &p_normal_set2,
                        false,
                        GL_STATIC_DRAW);
    }
}

} // namespace gVirtualXRay
