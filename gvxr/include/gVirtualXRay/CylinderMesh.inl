/*

Copyright (c) 2017-2023, Dr Franck P. Vidal, Bangor University, All rights reserved.
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
*   @file       CylinderMesh.inl
*
*   @brief      Class to handle 3D meshes of cylinders.
*
*   @version	1.0
*
*   @date       17/04/2024
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
*   (c) by Dr Franck P. Vidal, Bangor University, May 2017-Sept 2023, version 1.1, BSD 3-Clause License
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, Oct 2023-present, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//--------------------------------------------------------------
template<typename T> CylinderMesh<T>::CylinderMesh(unsigned int aNumberOfSectors,
                                  double aHeight,
                                  double aRadius,
						  		  int anIndexDataType):
//--------------------------------------------------------------
        PolygonMesh()
//--------------------------------------------------------------
{
    create(aNumberOfSectors, aHeight, aRadius, anIndexDataType);
}


//------------------------------------------------------
template<typename T> void CylinderMesh<T>::create(unsigned int aNumberOfSectors,
                          double aHeight,
                          double aRadius,
				  		  int anIndexDataType)
//------------------------------------------------------
{
    if (aNumberOfSectors < 3)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid parameter, the number of sectors should be at least 3");
    }

	// Reset the mesh
	reset();
	m_external_data_flag = false;

	m_radius = aRadius;

	m_local_min_corner = VEC3(-aRadius, -aHeight / 2.0, -aRadius);
    m_local_max_corner = VEC3( aRadius,  aHeight / 2.0,  aRadius);

    std::vector<T> p_vertex_set;
    std::vector<T> p_normal_set;
    std::vector<unsigned int> p_index_set;

    double theta(0);
    double angle(Pi * 2.0 / aNumberOfSectors);

    p_vertex_set.push_back(0);
    p_vertex_set.push_back(-aHeight / 2.0);
    p_vertex_set.push_back(0);

    p_vertex_set.push_back(0);
    p_vertex_set.push_back(aHeight / 2.0);
    p_vertex_set.push_back(0);


    // The side of the cylinder
    for(unsigned int s(0); s < aNumberOfSectors; ++s, theta += angle)
    {
        double const x = cos(theta);
        double const z = sin(theta);

        p_vertex_set.push_back(x * aRadius);
        p_vertex_set.push_back(-aHeight / 2.0);
        p_vertex_set.push_back(z * aRadius);

        p_normal_set.push_back(x);
        p_normal_set.push_back(0);
        p_normal_set.push_back(z);

        p_vertex_set.push_back(x * aRadius);
        p_vertex_set.push_back(aHeight / 2.0);
        p_vertex_set.push_back(z * aRadius);

        p_normal_set.push_back(x);
        p_normal_set.push_back(0);
        p_normal_set.push_back(z);


        if (s < aNumberOfSectors - 1)
        {
			p_index_set.push_back(2 + s * 2);
			p_index_set.push_back(2 + s * 2 + 1);
			p_index_set.push_back(2 + s * 2 + 2);

			p_index_set.push_back(2 + s * 2 + 2);
			p_index_set.push_back(2 + s * 2 + 1);
			p_index_set.push_back(2 + s * 2 + 3);
        }
        else
        {
			p_index_set.push_back(2 + s * 2);
			p_index_set.push_back(2 + s * 2 + 1);
			p_index_set.push_back(2 + 0);

			p_index_set.push_back(2 + 0);
			p_index_set.push_back(2 + s * 2 + 1);
			p_index_set.push_back(2 + 1);
        }
    }

    // Caps
    theta = 0;
    for(unsigned int s(0); s < aNumberOfSectors; ++s, theta += angle)
    {
        p_normal_set.push_back(0);
        p_normal_set.push_back(-1);
        p_normal_set.push_back(0);

        p_normal_set.push_back(0);
        p_normal_set.push_back(1);
        p_normal_set.push_back(0);

        if (s < aNumberOfSectors - 1)
        {
			p_index_set.push_back(0);
			p_index_set.push_back(2 + s * 2);
			p_index_set.push_back(2 + s * 2 + 2);

			p_index_set.push_back(1);
			p_index_set.push_back(2 + s * 2 + 3);
            p_index_set.push_back(2 + s * 2 + 1);
        }
        else
        {
			p_index_set.push_back(0);
			p_index_set.push_back(2 + s * 2);
			p_index_set.push_back(2);

			p_index_set.push_back(1);
			p_index_set.push_back(2 + 1);
            p_index_set.push_back(2 + s * 2 + 1);
        }
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

			for (unsigned int face_id = 0; face_id < p_index_set.size() / 3; ++face_id)
			{
				if (p_index_set[face_id * 3] == vertex_id ||
						p_index_set[face_id * 3 + 1] == vertex_id ||
						p_index_set[face_id * 3 + 2] == vertex_id)
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
                    setInternalData(GL_TRIANGLES,
                            &p_vertex_set,
                            &p_normal_set2,
                            &p_index_set,
                            false,
                            GL_STATIC_DRAW);
                }
                break;

            case GL_UNSIGNED_SHORT:
                {
                    std::vector<GLushort> p_ushort_index_set;

                    for (std::vector<GLuint>::const_iterator ite(p_index_set.begin());
                            ite != p_index_set.end();
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
                    std::vector<GLubyte> p_ubyte_index_set;

                    for (std::vector<GLuint>::const_iterator ite(p_index_set.begin());
                            ite != p_index_set.end();
                            ++ite)
                    {
                    	p_ubyte_index_set.push_back(*ite);
                    }

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
    else
    {
        std::vector<T> p_vertex_set2;
    	std::vector<T> p_normal_set2;

	    int normal_id = 0;
    	for (std::vector<unsigned int>::const_iterator ite = p_index_set.begin();
    			ite != p_index_set.end();
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
