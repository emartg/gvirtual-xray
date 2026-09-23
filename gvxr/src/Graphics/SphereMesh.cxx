/*

Copyright (c) 2014-2023, Dr Franck P. Vidal, Bangor University, All rights reserved.
Copyright (c) 2023-present, Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), 
UK Research and Innovation, All rights reserved.


Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

3. Neither the name of Bangor University, UK Research and Innovation nor the 
names of its contributors may be used to endorse or promote products derived 
from this software without specific prior written permission.

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
*    @file       SphereMesh.cxx
*
*    @brief      Class to handle 3D meshes of spheres.
*
*    @version    1.0
*
*   @version    1.0
*
*   @date       10/04/2024
*
*   @author     Prof Franck P. Vidal
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Dr Franck P. Vidal, Bangor University, Mar 2014-Sept 2023, version 1.1, BSD 3-Clause License
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, Oct 2023-present, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//    Include
//******************************************************************************
#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <cmath>

#ifndef GVXR_SPHERE_MESH_H
#include "gVirtualXRay/SphereMesh.h"
#endif

#ifndef GVXR_OUT_OF_MEMORY_EXCEPTION_H
#include "gVirtualXRay/OutOfMemoryException.h"
#endif


//******************************************************************************
//    Name space
//******************************************************************************
using namespace gVirtualXRay;


//----------------------------------------------------
void SphereMesh::create(unsigned int aNumberOfRings,
                        unsigned int aNumberOfSectors,
                        float aRadius,
                        int anIndexDataType)
//----------------------------------------------------
{
    // Reset the mesh
    reset();
    m_external_data_flag = false;

    m_radius = aRadius;

    m_local_min_corner = VEC3(-aRadius, -aRadius, -aRadius);
    m_local_max_corner = VEC3( aRadius,  aRadius,  aRadius);

    const double R(1.0 / double(aNumberOfRings   - 1));
    const double S(1.0 / double(aNumberOfSectors - 1));

    GLdouble* p_vertices = new GLdouble[aNumberOfRings * aNumberOfSectors * 3];
    if (!p_vertices)
    {
        throw OutOfMemoryException(__FILE__, __FUNCTION__, __LINE__);
    }
    m_number_of_vertices = aNumberOfRings * aNumberOfSectors;
    m_p_vertex_set = p_vertices;
    m_vertex_data_type = GL_DOUBLE;

    // Set the kind of polygon
    m_polygon_type = GL_TRIANGLES;

    m_p_vertex_normal_set.resize(m_number_of_vertices * 3);

    GLdouble* v(p_vertices);
    GLfloat* n(&m_p_vertex_normal_set[0]);
    for(unsigned int r(0); r < aNumberOfRings; ++r)
    {
        for(unsigned int s(0); s < aNumberOfSectors; ++s)
        {
            double const y = sin(-Pi_2 + Pi * r * R);
            double const x = cos(2 * Pi * s * S) * sin(Pi * r * R);
            double const z = sin(2 * Pi * s * S) * sin(Pi * r * R);

            *v++ = x * aRadius;
            *v++ = y * aRadius;
            *v++ = z * aRadius;

            *n++ = x;
            *n++ = y;
            *n++ = z;
        }
    }

    GLuint* p_indices = new GLuint[(aNumberOfRings - 1) * (aNumberOfSectors - 1) * 6];
    if (!p_indices)
    {
        throw OutOfMemoryException(__FILE__, __FUNCTION__, __LINE__);
    }
    m_number_of_indices = (aNumberOfRings - 1) * (aNumberOfSectors - 1) * 6;
    m_p_index_set = p_indices;
    m_index_data_type = GL_UNSIGNED_INT;

    GLuint* i(p_indices);
    for (unsigned int r(0); r < aNumberOfRings - 1; ++r)
    {
        for (unsigned int s(0); s < aNumberOfSectors - 1; ++s)
        {
            *i++ =  r      * aNumberOfSectors +  s;
            *i++ = (r + 1) * aNumberOfSectors + (s + 1);
            *i++ =  r      * aNumberOfSectors + (s + 1);
            
            *i++ =  r      * aNumberOfSectors +  s;
            *i++ = (r + 1) * aNumberOfSectors +  s;
            *i++ = (r + 1) * aNumberOfSectors + (s + 1);
        }
    }
}


void SphereMesh::create(unsigned int aNumberOfSubdivision,
                        float aRadius,
                        int anIndexDataType)
{
    // Reset the mesh
    reset();
    m_external_data_flag = false;

    m_radius = aRadius;

    m_local_min_corner = VEC3(-aRadius, -aRadius, -aRadius);
    m_local_max_corner = VEC3( aRadius,  aRadius,  aRadius);

    m_number_of_vertices = 12;

    double X(0.525731112119133606);

    double Z(0.850650808352039932);

    double vertex_data[12 * 3] = {

       -X, 0, Z,    X, 0,  Z,   -X,  0, -Z,    X,  0, -Z,

        0, Z, X,    0, Z, -X,    0, -Z,  X,    0, -Z, -X,

        Z, X, 0,   -Z, X,  0,    Z, -X,  0,   -Z, -X,  0
    };

    m_number_of_indices = 20 * 3;
    unsigned int index_data[20 * 3] = {

       0, 4, 1,   0,9, 4,   9, 5,4,    4,5,8,   4,8,1,

       8,10, 1,   8,3,10,   5, 3,8,    5,2,3,   2,7,3,

       7,10, 3,   7,6,10,   7,11,6,   11,0,6,    0,1,6,

       6,1,10,    9,0,11,   9,11,2,    9,2,5,    7,2,11
    };

    GLdouble* p_vertices = new GLdouble[m_number_of_vertices * 3];
    if (!p_vertices)
    {
        throw OutOfMemoryException(__FILE__, __FUNCTION__, __LINE__);
    }
    m_p_vertex_set = p_vertices;
    m_vertex_data_type = GL_DOUBLE;

    // Set the kind of polygon
    m_polygon_type = GL_TRIANGLES;

    m_p_vertex_normal_set.resize(m_number_of_vertices * 3);

    //std::copy(vertex_data, vertex_data + m_number_of_vertices * 3, p_vertices);
    //std::copy(vertex_data, vertex_data + m_number_of_vertices * 3, m_p_vertex_normal_set.begin());
    double temp;
    for (unsigned int i(0); i < m_number_of_vertices *3; ++i)
    {
        temp = vertex_data[i];
        m_p_vertex_normal_set[i] = temp;
        p_vertices[i] = temp * aRadius;
    }

    GLuint* p_indices = new GLuint[m_number_of_indices];
    if (!p_indices)
    {
        throw OutOfMemoryException(__FILE__, __FUNCTION__, __LINE__);
    }
    m_p_index_set = p_indices;
    m_index_data_type = GL_UNSIGNED_INT;

    std::copy(index_data, index_data + m_number_of_indices, p_indices);

    for (unsigned int i(0); i < aNumberOfSubdivision; ++i)
    {
        subdivision();
    }
}


void SphereMesh::subdivision()
{
    unsigned int number_of_vertices(m_number_of_vertices + m_number_of_indices);
    GLdouble* p_vertices = new GLdouble[number_of_vertices * 3];
    if (!p_vertices)
    {
        throw OutOfMemoryException(__FILE__, __FUNCTION__, __LINE__);
    }

    // Copy previous vertex/normal data
    double* p_vertex_set(static_cast<double*>(m_p_vertex_set));
    std::vector<float> normal_vectors(m_p_vertex_normal_set);
    std::copy(p_vertex_set, p_vertex_set + m_number_of_vertices * 3, p_vertices);







    unsigned int number_of_indices(m_number_of_indices * 4);
    GLuint* p_indices = new GLuint[number_of_indices];
    if (!p_indices)
    {
        throw OutOfMemoryException(__FILE__, __FUNCTION__, __LINE__);
    }



    unsigned int next_added_vertex_id(m_number_of_vertices);

    GLuint* p_index_set(static_cast<GLuint*>(m_p_index_set));
    for (unsigned int triangle_id(0); triangle_id < m_number_of_indices / 3; ++triangle_id)
    {
        unsigned int idx0(p_index_set[triangle_id * 3 + 0]);
        unsigned int idx1(p_index_set[triangle_id * 3 + 1]);
        unsigned int idx2(p_index_set[triangle_id * 3 + 2]);

        VEC3 normal_vector0(m_p_vertex_normal_set[idx0 * 3 + 0],
                m_p_vertex_normal_set[idx0 * 3 + 1],
                m_p_vertex_normal_set[idx0 * 3 + 2]);

        VEC3 normal_vector1(m_p_vertex_normal_set[idx1 * 3 + 0],
                m_p_vertex_normal_set[idx1 * 3 + 1],
                m_p_vertex_normal_set[idx1 * 3 + 2]);

        VEC3 normal_vector2(m_p_vertex_normal_set[idx2 * 3 + 0],
                m_p_vertex_normal_set[idx2 * 3 + 1],
                m_p_vertex_normal_set[idx2 * 3 + 2]);

        VEC3 normal_vector3(normal_vector0 + normal_vector1);
        normal_vector3.normalise();

        VEC3 normal_vector4(normal_vector1 + normal_vector2);
        normal_vector4.normalise();

        VEC3 normal_vector5(normal_vector2 + normal_vector0);
        normal_vector5.normalise();

        VEC3 new_vertex3(normal_vector3 * m_radius);
        VEC3 new_vertex4(normal_vector4 * m_radius);
        VEC3 new_vertex5(normal_vector5 * m_radius);

        // Add normal
        normal_vectors.push_back(normal_vector3[0]);
        normal_vectors.push_back(normal_vector3[1]);
        normal_vectors.push_back(normal_vector3[2]);

        normal_vectors.push_back(normal_vector4[0]);
        normal_vectors.push_back(normal_vector4[1]);
        normal_vectors.push_back(normal_vector4[2]);

        normal_vectors.push_back(normal_vector5[0]);
        normal_vectors.push_back(normal_vector5[1]);
        normal_vectors.push_back(normal_vector5[2]);

        // Add vertex
        p_vertices[(next_added_vertex_id + 0) * 3 + 0] = new_vertex3[0];
        p_vertices[(next_added_vertex_id + 0) * 3 + 1] = new_vertex3[1];
        p_vertices[(next_added_vertex_id + 0) * 3 + 2] = new_vertex3[2];

        p_vertices[(next_added_vertex_id + 1) * 3 + 0] = new_vertex4[0];
        p_vertices[(next_added_vertex_id + 1) * 3 + 1] = new_vertex4[1];
        p_vertices[(next_added_vertex_id + 1) * 3 + 2] = new_vertex4[2];

        p_vertices[(next_added_vertex_id + 2) * 3 + 0] = new_vertex5[0];
        p_vertices[(next_added_vertex_id + 2) * 3 + 1] = new_vertex5[1];
        p_vertices[(next_added_vertex_id + 2) * 3 + 2] = new_vertex5[2];

        // Add triangles
        p_indices[triangle_id * 12 +  0] = idx0;
        p_indices[triangle_id * 12 +  1] = next_added_vertex_id;
        p_indices[triangle_id * 12 +  2] = next_added_vertex_id + 2;

        p_indices[triangle_id * 12 +  3] = next_added_vertex_id;
        p_indices[triangle_id * 12 +  4] = idx1;
        p_indices[triangle_id * 12 +  5] = next_added_vertex_id + 1;

        p_indices[triangle_id * 12 +  6] = next_added_vertex_id + 2;
        p_indices[triangle_id * 12 +  7] = next_added_vertex_id;
        p_indices[triangle_id * 12 +  8] = next_added_vertex_id + 1;

        p_indices[triangle_id * 12 +  9] = next_added_vertex_id + 2;
        p_indices[triangle_id * 12 + 10] = next_added_vertex_id + 1;
        p_indices[triangle_id * 12 + 11] = idx2;

        next_added_vertex_id += 3;
    }

    VEC3 min_corner(m_local_min_corner);
    VEC3 max_corner(m_local_max_corner);

  //  number_of_vertices = m_number_of_vertices;
  //  number_of_indices = m_number_of_indices;

    reset();

    m_external_data_flag = false;

    m_local_min_corner = min_corner;
    m_local_max_corner = max_corner;

    m_number_of_vertices = number_of_vertices;
    m_number_of_indices = number_of_indices;

    m_p_vertex_set = p_vertices;
    m_vertex_data_type = GL_DOUBLE;

    m_p_vertex_normal_set = normal_vectors;

    m_p_index_set = p_indices;
    m_index_data_type = GL_UNSIGNED_INT;
}
