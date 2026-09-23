/*

Copyright (c) 2024-present, Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), 
UK Research and Innovation, All rights reserved.
Copyright (c) 2024-present, Mr Nalin Gupta, UK Research and Innovation, 
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
*   @file       StepWedgeMesh.inl
*
*   @brief      Class to handle 3D meshes of step wedges.
*
*   @version    1.0
*
*   @date       10/07/2024
*
*   @author     Prof Franck P. Vidal
*   @author     Mr Nalin Gupta
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, Jul 2024-present, version 1.1, BSD 3-Clause License
*   (c) by Mr Nalin Gupta, UK Research and Innovation, Jul 2024-present, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifndef GVXR_LOGGER_H
#include "gVirtualXRay/Logger.h"
#endif


//******************************************************************************
//	namespace
//******************************************************************************
namespace gVirtualXRay {


//-------------------------------------------------------------------------------
template<typename T> StepWedgeMesh<T>::StepWedgeMesh(unsigned int aNumberOfSteps,
                                                     T aTotalLength,
                                                     T aWidth,
                                                     T aFirstStepHeight,
                                                     T anOtherStepHeight,
                                                     int anIndexDataType):
//-------------------------------------------------------------------------------
		PolygonMesh()
//-------------------------------------------------------------------------------
{
	create(aNumberOfSteps,
		aTotalLength,
		aWidth,
		aFirstStepHeight,
		anOtherStepHeight,
		anIndexDataType);
}


//-----------------------------------------------------------------------------
template<typename T> void StepWedgeMesh<T>::create(unsigned int aNumberOfSteps,
                                                   T aTotalLength,
                                                   T aWidth,
                                                   T aFirstStepHeight,
                                                   T anOtherStepHeight,
                                                   int anIndexDataType)
//-----------------------------------------------------------------------------
{
    unsigned int double_step = 2 * aNumberOfSteps;
    T single_step_length = aTotalLength / aNumberOfSteps;
    T half_length = aTotalLength * 0.5;
    unsigned int number_of_vertices = aNumberOfSteps * (aNumberOfSteps + 5) + 2;
    unsigned int number_of_triangles = 2 * aNumberOfSteps * (aNumberOfSteps + 5);

    // INITIALISE 1D VECTORS OF SIZE
    std::vector<T> p_vertex_set;
    p_vertex_set.reserve(3 * number_of_vertices);   // 3 points per vertex in 3D

    // 2^8 is too small to hold indices as bit as number_of_vertices
    // Make use of 2^16
    if (anIndexDataType == GL_UNSIGNED_BYTE && number_of_vertices > 255)
    {
        LOGGER.logWarning("The data type of the index set is UCHAR. The number of vertices (") << 
            number_of_vertices << ") is bigger than 255. We'll try using USHORT." << std::endl;

        anIndexDataType = GL_UNSIGNED_SHORT;
    }

    // 2^16 is too small to hold indices as bit as number_of_vertices
    // Make use of 2^32
    if (anIndexDataType == GL_UNSIGNED_SHORT && number_of_vertices > 65536)
    {
        LOGGER.logWarning("The data type of the index set is USHORT. The number of vertices (") << 
            number_of_vertices << ") is bigger than 255. We'll try using UINT." << std::endl;
            
        anIndexDataType = GL_UNSIGNED_INT;
    }

    std::vector<unsigned int> p_uint_index_set;
    p_uint_index_set.reserve(3 * number_of_triangles);  // 3 indices per triangle

    // CREATE VERTICES FOR THE FLOOR
    for (unsigned int f = 0; f < aNumberOfSteps + 1; ++f) {
        T floorX = f * single_step_length - half_length;
        T floorY = aWidth * 0.5;
        T floorZ = 0;

        p_vertex_set.insert(p_vertex_set.end(), {floorX, +floorY, floorZ,
                                         floorX, -floorY, floorZ});
    }


    // THE REST OF IT
    for (unsigned int i = 0; i < aNumberOfSteps; ++i) {
        // VERTICES FOR THE STEPS
        unsigned int stepLayer = aNumberOfSteps - i;

        for (unsigned int j = 0; j < stepLayer + 1; ++j) {
            T stepX = j * single_step_length - half_length;
            T stepY = aWidth * 0.5;
            T stepZ = (i * anOtherStepHeight) + aFirstStepHeight;

            p_vertex_set.insert(p_vertex_set.end(), {stepX, +stepY, stepZ,
                                             stepX, -stepY, stepZ});
        }

        // TRIANGLES TO CREATE FACES
        int iDoubled = i * 2;
        int iSquared = i * i;
        unsigned int iSwitch = (i > 0) ? 1 : 0;

        // FOR ALL PARTS EXCEPT SIDES
        //
        // unique indices (a,b,c,d) uses 2 triangles
        // (a,b,c) and (c,d,a)

        // bottom indices
        unsigned int bottom1 = 1 + iDoubled;
        unsigned int bottom2 = 0 + iDoubled;
        unsigned int bottom3 = 2 + iDoubled;
        unsigned int bottom4 = 3 + iDoubled;

        // backward indices
        unsigned int back1 = (double_step + 3) * (i + 1) - iSquared;
        unsigned int back2 = back1 - 1;
        unsigned int back3 = ((double_step + 5) * i - iSquared - 2) * iSwitch;
        unsigned int back4 = back3 + 1;

        // front-top indices
        unsigned int frontTop1 = (double_step + 1) * (i + 2) - iSquared;
        unsigned int frontTop2 = frontTop1 - 2;
        unsigned int frontTop3 = frontTop1 - 1;
        unsigned int frontTop4 = frontTop1 + 1;

        // forward indices
        unsigned int forward1 = back2 - 2 * (iSwitch + 1);
        unsigned int forward2 = frontTop1;
        unsigned int forward3 = frontTop4;
        unsigned int forward4 = forward1 + 1;

        p_uint_index_set.insert(p_uint_index_set.end(), {bottom1, bottom2, bottom3,
                                           bottom3, bottom4, bottom1,        // Bottom face

                                           back1, back2, back3,
                                           back3, back4, back1,              // Backward face

                                           frontTop1, frontTop2, frontTop3,
                                           frontTop3, frontTop4, frontTop1,  // Front-top face

                                           forward1, forward2, forward3,
                                           forward3, forward4, forward1});   // Forward face
        // int size = p_uint_index_set.size();

        // THE FACES FOR THE SIDES
        for (unsigned int k = 0; k < stepLayer; ++k) {
            int kSwitch = (k > 0) ? 1 : -1;
            int kSquared = k * k;

            unsigned int shiftValue = (double_step + 5) * k - kSquared - 1;
            unsigned int lowerShift = iDoubled + (shiftValue * kSwitch);
            unsigned int upperShift = iDoubled + shiftValue + double_step - 2 * (k - 2);

            // Faces in the -y direction
            unsigned int negativeYSide4 = lowerShift;
            unsigned int negativeYSide3 = upperShift;
            unsigned int negativeYSide2 = negativeYSide3 + 2;
            unsigned int negativeYSide1 = negativeYSide4 + 2;

            // Faces in the +y direction
            unsigned int positiveYSide4 = negativeYSide1 - 1;
            unsigned int positiveYSide3 = negativeYSide2 - 1;
            unsigned int positiveYSide2 = negativeYSide3 - 1;
            unsigned int positiveYSide1 = negativeYSide4 - 1;

            p_uint_index_set.insert(p_uint_index_set.end(), {negativeYSide1, negativeYSide2, negativeYSide3,
                                               negativeYSide3, negativeYSide4, negativeYSide1,     // -y direction faces

                                               positiveYSide1, positiveYSide2, positiveYSide3,
                                               positiveYSide3, positiveYSide4, positiveYSide1});   // +y direction faces
        }
    }
    



    if (anIndexDataType)
	{
        switch (anIndexDataType)
            {
            case GL_UNSIGNED_INT:
                {
                    setInternalData(GL_TRIANGLES,
                            &p_vertex_set,
                            &p_uint_index_set,
                            false,
                            GL_STATIC_DRAW);
                }
                break;

            case GL_UNSIGNED_SHORT:
                {
                    std::vector<GLushort> p_ushort_index_set;

                    for (std::vector<GLuint>::const_iterator ite(p_uint_index_set.begin());
                            ite != p_uint_index_set.end();
                            ++ite)
                    {
                        p_ushort_index_set.push_back(*ite);
                    }

                    setInternalData(GL_TRIANGLES,
                            &p_vertex_set,
                            &p_ushort_index_set,
                            false,
                            GL_STATIC_DRAW);
                }
                break;

            case GL_UNSIGNED_BYTE:
                {
                    std::vector<GLubyte> p_ubyte_index_set;

                    for (std::vector<GLuint>::const_iterator ite(p_uint_index_set.begin());
                            ite != p_uint_index_set.end();
                            ++ite)
                    {
                        p_ubyte_index_set.push_back(*ite);
                    }

                    setInternalData(GL_TRIANGLES,
                            &p_vertex_set,
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

	    int normal_id = 0;
    	for (std::vector<unsigned int>::const_iterator ite = p_uint_index_set.begin();
    			ite != p_uint_index_set.end();
    			++ite, ++normal_id)
    	{
    		p_vertex_set2.push_back(p_vertex_set[*ite * 3 + 0]);
    		p_vertex_set2.push_back(p_vertex_set[*ite * 3 + 1]);
    		p_vertex_set2.push_back(p_vertex_set[*ite * 3 + 2]);
    	}

        setInternalData(GL_TRIANGLES,
                        &p_vertex_set2,
                        false,
                        GL_STATIC_DRAW);
    }
}


} // namespace gVirtualXRay
