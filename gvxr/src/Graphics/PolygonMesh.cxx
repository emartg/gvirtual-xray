/*

Copyright (c) 2016, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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
*   @file       PolygonMesh.cxx
*
*   @brief      Class to handle polygon (triangles) meshes.
*
*   @version    1.0
*
*   @date       24/08/2016
*
*   @author     Dr Franck P. Vidal
*
*   @todo       Make it possible to merge vertices if an array of indices exists
*               Convert triangle mesh into arbitrary polygon mesh
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
//  Include
//******************************************************************************
#ifndef GL_INVALID_INDEX
#define GL_INVALID_INDEX 0xFFFFFFFFu
#endif

#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <map>
#include <fstream>
#include <cstring>
#include <climits>
#include <sys/types.h>
#include <sys/stat.h>
//#include <unistd.h>

#ifndef GVXR_POLYGON_MESH_H
#include "gVirtualXRay/PolygonMesh.h"
#endif

#ifndef GVXR_UNITS_H
#include "gVirtualXRay/Units.h"
#endif

#ifndef GVXR_FILE_DOES_NOT_EXIST_EXCEPTION_H
#include "gVirtualXRay/FileDoesNotExistException.h"
#endif

#ifndef GVXR_OUT_OF_MEMORY_EXCEPTION_H
#include "gVirtualXRay/OutOfMemoryException.h"
#endif

#ifndef GVXR_NOT_IMPLEMENTED_EXCEPTION_H
#include "gVirtualXRay/NotImplementedException.h"
#endif

#ifndef GVXR_LOGGER_H
#include <gVirtualXRay/Logger.h>
#endif


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//  Constant variables
//******************************************************************************


//******************************************************************************
//  Global variables
//******************************************************************************


//******************************************************************************
//  Function declaration
//******************************************************************************


//------------------------------------------
PolygonMesh::PolygonMesh():
//------------------------------------------
        m_file_scale(-1),
        m_p_vertex_set(0),
        m_p_index_set(0),
        m_number_of_indices(0),
        m_number_of_vertices(0),
        m_index_data_type(GL_NONE),
        m_vertex_data_type(GL_NONE),
        m_polygon_type(GL_NONE),
        m_external_data_flag(false),
        m_display_flag(true),
        m_has_inverted_normal_vectors(false)
//------------------------------------------
{
}


//------------------------------------------------------------------------
PolygonMesh::PolygonMesh(const PolygonMesh& aPolygonMesh):
//------------------------------------------------------------------------
        m_filename(aPolygonMesh.m_filename),
        m_file_scale(aPolygonMesh.m_file_scale),
        m_photon_cross_section(aPolygonMesh.m_photon_cross_section),
        m_local_min_corner(aPolygonMesh.m_local_min_corner),
        m_local_max_corner(aPolygonMesh.m_local_max_corner),
        //m_p_face_normal_set(aPolygonMesh.m_p_face_normal_set),
        m_p_vertex_normal_set(aPolygonMesh.m_p_vertex_normal_set),
        m_p_vertex_set(0),
        m_p_index_set(0),
        m_number_of_indices(aPolygonMesh.m_number_of_indices),
        m_number_of_vertices(aPolygonMesh.m_number_of_vertices),
        m_index_data_type(aPolygonMesh.m_index_data_type),
        m_vertex_data_type(aPolygonMesh.m_vertex_data_type),
        m_polygon_type(aPolygonMesh.m_polygon_type),
        m_external_data_flag(aPolygonMesh.m_external_data_flag),
        m_display_flag(aPolygonMesh.m_display_flag),
        m_material(aPolygonMesh.m_material),
        m_has_inverted_normal_vectors(aPolygonMesh.m_has_inverted_normal_vectors)
//------------------------------------------------------------------------
{
    if (aPolygonMesh.m_p_vertex_set)
        {
        copyVertexSet(aPolygonMesh.m_p_vertex_set);
        }

    if (aPolygonMesh.m_p_index_set)
        {
        copyIndexSet(aPolygonMesh.m_p_index_set);
        }
}


//-----------------------
void PolygonMesh::reset()
//-----------------------
{
    // Reset class members to their default values
    resetVBOs();

    m_photon_cross_section.reset();

    m_local_min_corner = VEC3();
    m_local_max_corner = VEC3();

    //m_p_face_normal_set.clear();
    m_p_vertex_normal_set.clear();

    destroyIndexData();
    destroyVertexData();

    m_number_of_indices  = 0;
    m_number_of_vertices = 0;

    m_index_data_type = GL_NONE;
    m_vertex_data_type = GL_NONE;

    m_external_data_flag = false;

    m_file_scale = -1;

    m_has_inverted_normal_vectors = false;
}


//-------------------------
void PolygonMesh::display()
//-------------------------
{

    // It is not neccesary display this mesh
    if (!m_display_flag)
        return;

    // Create the VBO if needed
    if (!m_p_vbo.get())
        {
        m_p_vbo.reset(createVBO());
        }

    // Update the VBO if needed
    if (!m_p_vbo->isReady() || m_polygon_type != m_p_vbo->getPrimitiveType())
        {
        updateVBO(m_p_vbo->getBufferUsageHint(), m_polygon_type);
        }

    // Display the polygon mesh using a VBO
    if (m_p_vbo->isReady())
        {
        if (m_number_of_vertices)
            {
            m_p_vbo->display();
            }
        }
    // Do not use the VBO
    else
        {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
                "Cannot create the VBO.");
        }

    // Check the OpenGL error status
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
}


//----------------------------------
void PolygonMesh::displayWireFrame()
//----------------------------------
{
    // It is not neccesary display this mesh
    if (!m_display_flag)
        return;

#ifdef HAS_OPENGL
    // Turn on wireframe mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    display();

    // Turn off wireframe mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Check the OpenGL error status
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
#endif
}


//----------------------------------------------------------------
void PolygonMesh::displayVertexNormal(RATIONAL_NUMBER aNormalSize)
//----------------------------------------------------------------
{
    // There are as many normals as there are vertices
    if (getVertexNormalNumber() == m_number_of_vertices)
        {
        std::vector<float> p_normal_set;

        if (!m_p_vertex_normal_vbo.get())
            {
            m_p_vertex_normal_vbo.reset(new PolygonMesh());
            }

        for (unsigned int i(0); i < m_number_of_vertices; ++i)
            {
            VEC3 vertex(getVertex(i));
            VEC3 normal(vertex + getVertexNormal(i) * aNormalSize);

            p_normal_set.push_back(vertex.getX());
            p_normal_set.push_back(vertex.getY());
            p_normal_set.push_back(vertex.getZ());

            p_normal_set.push_back(normal.getX());
            p_normal_set.push_back(normal.getY());
            p_normal_set.push_back(normal.getZ());
            }

        m_p_vertex_normal_vbo->setInternalData(GL_LINES,
                &p_normal_set[0],
                p_normal_set.size() / 3,
                GL_FLOAT,
                0,
                0,
                GL_NONE,
                true,
                GL_STATIC_DRAW);

        // Check the OpenGL error status
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        m_p_vertex_normal_vbo->display();
        }
    // There are not as many normals as there are vertices
    else
        {
        std::stringstream error_message;
        error_message << "There are not as many normals (" <<
                getVertexNormalNumber() << ") as there are vertices (" <<
                m_number_of_vertices << ").";
        throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
        }
}


//------------------------------------------------------
void PolygonMesh::displayFaceNormal(GLfloat aNormalSize)
//------------------------------------------------------
{
    unsigned int number_vertex_per_face(0);

    switch (m_polygon_type)
        {
        case GL_TRIANGLES:
            number_vertex_per_face = 3;
            break;

        case GL_LINES:
            number_vertex_per_face = 2;
            break;

        default:
            throw Exception(__FILE__, __FUNCTION__, __LINE__,
                    "Invalid polygon type.");
        }

    if (getFaceNormalNumber() == getFaceNumber())
        {
        std::vector<float> p_normal_set;

        if (!m_p_face_normal_vbo.get())
            {
            m_p_face_normal_vbo.reset(new PolygonMesh());
            }

        // There is an index
        if (m_p_index_set && m_number_of_indices)
            {
            for (unsigned int i(0); i < getFaceNumber(); ++i)
                {
                VEC3 vertex;
                for (unsigned int j(0); j < number_vertex_per_face; ++j)
                {
                    vertex += getVertex(getIndex(i * number_vertex_per_face + j));
                }
                vertex /= float(number_vertex_per_face);

                VEC3 normal(vertex + getFaceNormal(i) * aNormalSize);

                p_normal_set.push_back(vertex.getX());
                p_normal_set.push_back(vertex.getY());
                p_normal_set.push_back(vertex.getZ());

                p_normal_set.push_back(normal.getX());
                p_normal_set.push_back(normal.getY());
                p_normal_set.push_back(normal.getZ());
                }
            }
        // There is no index
        else
            {
            // There are as many normals as there are vertices
            for (unsigned int i(0); i < getFaceNumber(); ++i)
                {
                VEC3 vertex;
                for (unsigned int j(0); j < number_vertex_per_face; ++j)
                {
                    vertex += getVertex(i * number_vertex_per_face + j);
                }
                vertex /= float(number_vertex_per_face);

                VEC3 normal(vertex + getFaceNormal(i) * aNormalSize);

                p_normal_set.push_back(vertex.getX());
                p_normal_set.push_back(vertex.getY());
                p_normal_set.push_back(vertex.getZ());

                p_normal_set.push_back(normal.getX());
                p_normal_set.push_back(normal.getY());
                p_normal_set.push_back(normal.getZ());
                }
            }

        m_p_face_normal_vbo->setInternalData(GL_LINES,
                &p_normal_set[0],
                p_normal_set.size() / 3,
                GL_FLOAT,
                0,
                0,
                GL_NONE,
                true,
                GL_STATIC_DRAW);

        // Check the OpenGL error status
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        m_p_face_normal_vbo->display();
        }
    // There are not as many normals as there are faces
    else
        {
        std::stringstream error_message;
        error_message << "There are not as many normals (" <<
                getVertexNormalNumber() << ") as there are vertices (" <<
                m_number_of_vertices << ").";
        throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
        }}


//---------------------------------------------
unsigned int PolygonMesh::getFaceNumber() const
//---------------------------------------------
{
    int number_vertex_per_face(0);

    switch (m_polygon_type)
        {
        case GL_TRIANGLES:
            number_vertex_per_face = 3;
            break;

        case GL_LINES:
            number_vertex_per_face = 2;
            break;

        default:
            throw Exception(__FILE__, __FUNCTION__, __LINE__,
                    "Invalid polygon type.");
        }

    // Return the number of faces
    if (m_p_index_set && m_number_of_indices)
        {
        // There are 3 or 4 indices per face
        return (m_number_of_indices / number_vertex_per_face);
        }
    else
        {
        // There are 3 or 4 vertices per face
        return (m_number_of_vertices / number_vertex_per_face);
        }
}


//-------------------------------------------------
unsigned int PolygonMesh::getTriangleNumber() const
//-------------------------------------------------
{
    switch (m_polygon_type)
        {
        case GL_TRIANGLES:
            // Return the number of faces
            if (m_p_index_set && m_number_of_indices)
                {
                // There are 3 indices per triangle
                return (m_number_of_indices / 3);
                }
            else
                {
                // There are 3 vertices per triangle
                return (m_number_of_vertices / 3);
                }

        case GL_LINES:
            // Return the number of faces
            if (m_p_index_set && m_number_of_indices)
                {
                // There are 2 indices per line
                return (m_number_of_indices / 2);
                }
            else
                {
                // There are 2 vertices per line
                return (m_number_of_vertices / 2);
                }

        default:
            return (0);
        }
}


//---------------------------------------------------------
void PolygonMesh::loadSTLFile(bool aMoveToCentreFlag,
                              bool anAutoComputeNormalFlag,
                              bool aPrintDebugInfoFlag,
                              bool aCreateVBOFlag,
                              RATIONAL_NUMBER aScale,
                              int aBufferUsageHing)
//---------------------------------------------------------
{
    unsigned int occurence_of_solid = 0;
    unsigned int occurence_of_vertex = 0;

    // Open the file in reading more
    std::ifstream input_stream(m_filename.c_str());
    std::string line;

    // The file is not open
    if (!input_stream.is_open())
        {
        throw FileDoesNotExistException(__FILE__, __FUNCTION__, __LINE__,
                m_filename);
        }

    // Read the file
    while(getline(input_stream, line))
        {
        if (line.find("solid", 0) != std::string::npos) ++occurence_of_solid;
        if (line.find("vertex", 0) != std::string::npos) ++occurence_of_vertex;
        }

    // This is an ASCII file
    if (occurence_of_vertex && occurence_of_solid)
        loadASCIISTLFile(aMoveToCentreFlag,
                         anAutoComputeNormalFlag,
                         aPrintDebugInfoFlag,
                         aCreateVBOFlag,
                         aScale,
                         aBufferUsageHing);
    // This is a binary file
    else
        loadBinarySTLFile(aMoveToCentreFlag,
                          anAutoComputeNormalFlag,
                          aPrintDebugInfoFlag,
                          aCreateVBOFlag,
                          aScale,
                          aBufferUsageHing);
}


//---------------------------------------------------------------
void PolygonMesh::loadASCIISTLFile(bool aMoveToCentreFlag,
                                   bool anAutoComputeNormalFlag,
                                   bool aPrintDebugInfoFlag,
                                   bool aCreateVBOFlag,
                                   RATIONAL_NUMBER aScale,
                                   int aBufferUsageHing)
//---------------------------------------------------------------
{
    // Reset the data
    reset();
    m_file_scale = aScale;

    // Open the file in reading more
    std::ifstream input_stream(m_filename.c_str(),
            std::ios::in | std::ios::binary);

    // The file is not open
    if (!input_stream.is_open())
        {
        throw FileDoesNotExistException(__FILE__, __FUNCTION__, __LINE__,
                m_filename);
        }

    struct stat stat_buf;
    int rc = stat(m_filename.c_str(), &stat_buf);

    if (rc != 0)
        {
        throw FileDoesNotExistException(__FILE__, __FUNCTION__, __LINE__,
                m_filename);
        }

    // Temp storage of vertex data
    std::vector<double> p_vertices;

    // Read the file
    std::string line;
    while(getline(input_stream, line))
        {
        if (line.find("vertex", 0) != std::string::npos)
            {
            std::stringstream temp_line;
            std::string temp_string;
            double temp_coord_x;
            double temp_coord_y;
            double temp_coord_z;
            temp_line << line;
            temp_line >> temp_string >> temp_coord_x >> temp_coord_y >> temp_coord_z;

            p_vertices.push_back(temp_coord_x);
            p_vertices.push_back(temp_coord_y);
            p_vertices.push_back(temp_coord_z);
            }
        }

    // Load the data
    setInternalData(GL_TRIANGLES,
            &p_vertices,
            aCreateVBOFlag,
            aBufferUsageHing);

    // Change scale and compute bounding box
    applyScale(aScale);
    computeBoundingBox();

    // Move the object to the center
    if (aMoveToCentreFlag)
        {
        moveToCentre();
        }

    // Display extra debug info
    if (aPrintDebugInfoFlag)
        {
        LOGGER.logNow("file_name:\t") << m_filename <<
                "\tnb_faces:\t" << getFaceNumber() <<
                "\tnb_vertices:\t" << m_number_of_vertices <<
                "\tbounding_box (in cm):\t(" <<
                        m_local_min_corner.getX() / cm << ", " <<
                        m_local_min_corner.getY() / cm << ", " <<
                        m_local_min_corner.getZ() / cm << ")\t(" <<
                        m_local_max_corner.getX() / cm << ", " <<
                        m_local_max_corner.getY() / cm << ", " <<
                        m_local_max_corner.getZ() / cm << ")" << std::endl;
        }

    // Create the VBO
    if (aCreateVBOFlag)
        {
        updateVBO(aBufferUsageHing, m_polygon_type);
        }
}


//---------------------------------------------------------------
void PolygonMesh::loadBinarySTLFile(bool aMoveToCentreFlag,
                                    bool anAutoComputeNormalFlag,
                                    bool aPrintDebugInfoFlag,
                                    bool aCreateVBOFlag,
                                    RATIONAL_NUMBER aScale,
                                    int aBufferUsageHing)
//---------------------------------------------------------------
{
    // Reset the data
    reset();
    m_file_scale = aScale;

    // Open the file in reading more
    std::ifstream input_stream(m_filename.c_str(),
            std::ios::in | std::ios::binary);

    // The file is not open
    if (!input_stream.is_open())
        {
        throw FileDoesNotExistException(__FILE__, __FUNCTION__, __LINE__,
                m_filename);
        }

    struct stat stat_buf;
    int rc = stat(m_filename.c_str(), &stat_buf);

    if (rc != 0)
        {
        throw FileDoesNotExistException(__FILE__, __FUNCTION__, __LINE__,
                m_filename);
        }

    char* p_data(0);
    p_data = new char[stat_buf.st_size];

    input_stream.read(p_data, stat_buf.st_size);

    // Load the data
    loadSTLDataFromBinaryStream(p_data,
            aMoveToCentreFlag,
            anAutoComputeNormalFlag,
            aPrintDebugInfoFlag,
            aCreateVBOFlag,
            aScale,
            aBufferUsageHing);

    delete [] p_data;

    // Close the STL file
    input_stream.close();
}


//------------------------------------------------------------------------
void PolygonMesh::loadSTLDataFromBinaryStream(const char* apInputData,
                                              bool aMoveToCentreFlag,
                                              bool anAutoComputeNormalFlag,
                                              bool aPrintDebugInfoFlag,
                                              bool aCreateVBOFlag,
                                              RATIONAL_NUMBER aScale,
                                              int aBufferUsageHing)
//------------------------------------------------------------------------
{
    m_file_scale = aScale;

    // Get the number of triangles in the file header
    // (the header is made of 80 bytes followded by the number of
    // triangles (4 bytes))
    char const* p_input_data(apInputData);
    p_input_data += 80;

    int triangle_number;

    memcpy(&triangle_number, p_input_data, sizeof(int));
    p_input_data += sizeof(int);

    // Set the kind of polygon
    m_polygon_type = GL_TRIANGLES;

    // Set the size of arrays to store the vertices and normal vectors
    m_external_data_flag = false;
    m_number_of_vertices = triangle_number * 3;
    GLfloat* p_vertex_set(new GLfloat[m_number_of_vertices * 3]);
    m_vertex_data_type = GL_FLOAT;
    m_p_vertex_set = p_vertex_set;

    //m_p_face_normal_set.resize(m_number_of_vertices * 3);
    m_p_vertex_normal_set.resize(m_number_of_vertices * 3);


    // Read the data
    for (unsigned int i(0); i < getFaceNumber(); i++)
        {
        // Read normals
        //memcpy(&m_p_face_normal_set[9 * i], p_input_data, sizeof(float) * 3);

        // Copy one normal per vertex
        if (!anAutoComputeNormalFlag)
            {
            const float* p_normal = (const float*)p_input_data;
            for (unsigned int k=1; k<3; k++)
                {
                //m_p_face_normal_set[9 * i + 3 * k]     =
                //        p_normal[0];

                //m_p_face_normal_set[9 * i + 3 * k + 1] =
                //        p_normal[1];

                //m_p_face_normal_set[9 * i + 3 * k + 2] =
                //        p_normal[2];


                m_p_vertex_normal_set[9 * i + 3 * k]     =
                        p_normal[0];

                m_p_vertex_normal_set[9 * i + 3 * k + 1] =
                        p_normal[1];

                m_p_vertex_normal_set[9 * i + 3 * k + 2] =
                        p_normal[2];
                }
            }

        p_input_data += sizeof(float) * 3;

        // Read vertices
        memcpy(&p_vertex_set[9 * i], p_input_data, sizeof(float) * 3 * 3);
        p_input_data += sizeof(float) * 3 * 3;

        // Skip data
        p_input_data += 2;
        }

    // Compute the normal vectors using the cross product
    if (anAutoComputeNormalFlag)
        {
        computeNormalVectors();
        normaliseNormals();
        }

    // Change scale and compute bounding box
    applyScale(aScale);
    computeBoundingBox();

    // Move the object to the center
    if (aMoveToCentreFlag)
        {
        moveToCentre();
        }

    // Display extra debug info
    if (aPrintDebugInfoFlag)
        {
        LOGGER.logNow("file_name:\t") << m_filename <<
                "\tnb_faces:\t" << getFaceNumber() <<
                "\tnb_vertices:\t" << m_number_of_vertices <<
                "\tbounding_box (in cm):\t(" <<
                        m_local_min_corner.getX() / cm << ", " <<
                        m_local_min_corner.getY() / cm << ", " <<
                        m_local_min_corner.getZ() / cm << ")\t(" <<
                        m_local_max_corner.getX() / cm << ", " <<
                        m_local_max_corner.getY() / cm << ", " <<
                        m_local_max_corner.getZ() / cm << ")" << std::endl;
        }

    // Create the VBO
    if (aCreateVBOFlag)
        {
        updateVBO(aBufferUsageHing, m_polygon_type);
        }
}


//----------------------------------------------
void PolygonMesh::writeSTLFile(bool aBinaryFlag)
//----------------------------------------------
{
    if (m_polygon_type != GL_TRIANGLES)
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
                "Cannot write a STL file, mesh is not made of triangles.");
    }

    if (!getTriangleNumber())
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
                "Cannot write a STL file, there is no triangle to save.");
    }

    if (!m_filename.size())
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
                "Cannot write a STL file, not file name provided.");
    }

    unsigned int triangle_number(getTriangleNumber());
    unsigned int number_of_indices = getIndexNumber();
    unsigned int number_of_faces = getFaceNumber();

    // Write a binary file
    if (aBinaryFlag)
    {
        // Open the file in writing more
        std::ofstream output_stream(m_filename.c_str(),
                std::ios::out | std::ios::binary);

        // The file is not open
        if (!output_stream.is_open())
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__,
                    "Cannot open the STL file in writing mode");
        }

        // The header is made of 80 bytes
        char temp(0);
        for (unsigned int i(0); i < 80; ++i)
        {
            output_stream.write(&temp, 1);
        }

        // The number of triangles
        unsigned int triangle_number(getTriangleNumber());
        output_stream.write((char*) &triangle_number, sizeof(triangle_number));

        // Use an index
        if (number_of_indices)
        {
            if (number_of_indices != number_of_faces * 3)
            {
                throw Exception(__FILE__, __FUNCTION__, __LINE__,
                        "Cannot write a STL file, there are inconsistencies between the number of faces and the number of indices.");
            }

            if (number_of_indices != triangle_number * 3)
            {
                throw Exception(__FILE__, __FUNCTION__, __LINE__,
                        "Cannot write a STL file, there are inconsistencies between the number of triangles and the number of indices.");
            }

            for (unsigned int i(0); i < triangle_number; i++)
            {
                int id0(getIndex(i * 3 + 0));
                int id1(getIndex(i * 3 + 1));
                int id2(getIndex(i * 3 + 2));

                VEC3 v0(getVertex(id0));
                VEC3 v1(getVertex(id1));
                VEC3 v2(getVertex(id2));

                VEC3 n0(getVertexNormal(id0));
                VEC3 n1(getVertexNormal(id1));
                VEC3 n2(getVertexNormal(id2));
                VEC3 normal(n0 + n1 + n2);

                // Normalise the vector, and prevent division by zero
                if (double norm = normal.length(); norm > EPSILON) {
                    normal /= norm;
                }

                float temp[3];
                temp[0] = normal.getX();
                temp[1] = normal.getY();
                temp[2] = normal.getZ();
                output_stream.write((char*) temp, sizeof(float) * 3);

                temp[0] = v0.getX();
                temp[1] = v0.getY();
                temp[2] = v0.getZ();
                output_stream.write((char*) temp, sizeof(float) * 3);

                temp[0] = v1.getX();
                temp[1] = v1.getY();
                temp[2] = v1.getZ();
                output_stream.write((char*) temp, sizeof(float) * 3);

                temp[0] = v2.getX();
                temp[1] = v2.getY();
                temp[2] = v2.getZ();
                output_stream.write((char*) temp, sizeof(float) * 3);

                // Write data
                char dummy(0);
                output_stream.write(&dummy, 1);
                output_stream.write(&dummy, 1);
            }
        }
        // Do not use an index
        else
        {
            // Read the data
            float* p_vertex_set((float*) m_p_vertex_set);

            for (unsigned int i(0); i < triangle_number; i++)
            {
                // Compute the normal vector

                // Get the vertex IDs
                int id_0(i * 3 + 0);
                int id_1(i * 3 + 1);
                int id_2(i * 3 + 2);

                // Get the vertices
                VEC3 v0(getVertex(id_0));
                VEC3 v1(getVertex(id_1));
                VEC3 v2(getVertex(id_2));

                // Compute the normal for the triangle
                VEC3 normal((v2 - v0)^(v2 - v1));

                // Normalise the vector, and prevent division by zero
                if (double norm = normal.length(); norm > EPSILON) {
                    normal /= norm;
                }

                // Write the data
                float p_normal[3];
                p_normal[0] = normal.getX();
                p_normal[1] = normal.getY();
                p_normal[2] = normal.getZ();
                output_stream.write((char*) p_normal, sizeof(float) * 3);

                // Write vertices
                output_stream.write((char*) &p_vertex_set[9 * i],
                        sizeof(float) * 3 * 3);

                // Write data
                output_stream.write(&temp, 1);
                output_stream.write(&temp, 1);
            }
        }

        // Close the STL file
        output_stream.close();
    }
    // Write an ASCII file
    else {
        // Open the file in writing more
        std::ofstream output_stream(m_filename.c_str(), std::ios::out);

        // The file is not open
        if (!output_stream.is_open())
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__,
                    "Cannot open the STL file in writing mode");
        }

        // The header
        output_stream << "solid make_with_gVXR-" <<
            gVirtualXRay_VERSION_MAJOR << "_" <<
            gVirtualXRay_VERSION_MINOR << "_" <<
            gVirtualXRay_VERSION_PATCH <<
            std::endl << std::endl;

        // Use an index
        if (number_of_indices)
        {
            for (unsigned int i(0); i < triangle_number; i++)
            {
                int id0(getIndex(i * 3 + 0));
                int id1(getIndex(i * 3 + 1));
                int id2(getIndex(i * 3 + 2));

                VEC3 v0(getVertex(id0));
                VEC3 v1(getVertex(id1));
                VEC3 v2(getVertex(id2));

                VEC3 n0(getVertexNormal(id0));
                VEC3 n1(getVertexNormal(id1));
                VEC3 n2(getVertexNormal(id2));
                VEC3 normal(n0 + n1 + n2);

                // Normalise the vector, and prevent division by zero
                if (double norm = normal.length(); norm > EPSILON) {
                    normal /= norm;
                }

                output_stream << "  facet normal " << normal[0] << " " << normal[1] << " " << normal[2] << std::endl;
                output_stream << "    outer loop" << std::endl;
                output_stream << "      vertex " << v0[0] << " " << v0[1] << " " << v0[2] << std::endl;
                output_stream << "      vertex " << v1[0] << " " << v1[1] << " " << v1[2] << std::endl;
                output_stream << "      vertex " << v2[0] << " " << v2[1] << " " << v2[2] << std::endl;
                output_stream << "    endloop" << std::endl;
                output_stream << "  endfacet" << std::endl;
            }
        }
        // Do not use an index
        else
        {
            for (unsigned int i(0); i < triangle_number; i++)
            {
                // Compute the normal vector

                // Get the vertex IDs
                int id_0(i * 3 + 0);
                int id_1(i * 3 + 1);
                int id_2(i * 3 + 2);

                // Get the vertices
                VEC3 v0(getVertex(id_0));
                VEC3 v1(getVertex(id_1));
                VEC3 v2(getVertex(id_2));

                // Compute the normal for the triangle
                VEC3 normal((v2 - v0)^(v2 - v1));

                // Normalise the vector, and prevent division by zero
                if (double norm = normal.length(); norm > EPSILON) {
                    normal /= norm;
                }

                output_stream << "  facet normal " << normal[0] << " " << normal[1] << " " << normal[2] << std::endl;
                output_stream << "    outer loop" << std::endl;
                output_stream << "      vertex " << v0[0] << " " << v0[1] << " " << v0[2] << std::endl;
                output_stream << "      vertex " << v1[0] << " " << v1[1] << " " << v1[2] << std::endl;
                output_stream << "      vertex " << v2[0] << " " << v2[1] << " " << v2[2] << std::endl;
                output_stream << "    endloop" << std::endl;
                output_stream << "  endfacet" << std::endl;
            }
        }

        output_stream << "endsolid" << std::endl;

        // Close the STL file
        output_stream.close();
    }
}


//--------------------------------------
void PolygonMesh::computeNormalVectors()
//--------------------------------------
{
    //m_p_face_normal_set.clear();
    m_p_vertex_normal_set.clear();

    switch (m_polygon_type)
        {
        case GL_TRIANGLES:
        // There is an index
        if (m_number_of_indices)
            {
            if (m_number_of_indices % 3 != 0)
                {
                throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
                }

            // Allocate the memory
            m_p_vertex_normal_set.resize(m_number_of_vertices * 3, 0.0);

            // Process every triangle
            for (unsigned int i(0); i < m_number_of_indices; i += 3)
                {
                // Get the vertex IDs
                int id_0(getIndex(i + 0));
                int id_1(getIndex(i + 1));
                int id_2(getIndex(i + 2));

                // Get the vertices
                VEC3 vec_A(getVertex(id_0));
                VEC3 vec_B(getVertex(id_1));
                VEC3 vec_C(getVertex(id_2));

                // Compute the normal for the triangle
                VEC3 normal((vec_C - vec_A)^(vec_C - vec_B));

                // Normalise the vector
                if (!(
                        (std::abs(normal.getX()) <= EPSILON) &&
                        (std::abs(normal.getY()) <= EPSILON) &&
                        (std::abs(normal.getZ()) <= EPSILON)
                     ))
                    {
                    normal.normalize();

                    // Add the new normal
                    m_p_vertex_normal_set[id_0 * 3 + 0] += normal.getX();
                    m_p_vertex_normal_set[id_0 * 3 + 1] += normal.getY();
                    m_p_vertex_normal_set[id_0 * 3 + 2] += normal.getZ();

                    m_p_vertex_normal_set[id_1 * 3 + 0] += normal.getX();
                    m_p_vertex_normal_set[id_1 * 3 + 1] += normal.getY();
                    m_p_vertex_normal_set[id_1 * 3 + 2] += normal.getZ();

                    m_p_vertex_normal_set[id_2 * 3 + 0] += normal.getX();
                    m_p_vertex_normal_set[id_2 * 3 + 1] += normal.getY();
                    m_p_vertex_normal_set[id_2 * 3 + 2] += normal.getZ();
                    }
                }

            normaliseNormals();
            }
        // There is not any index
        else
            {
            if (m_number_of_vertices % 3 != 0)
                {
                throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
                }

            // Allocate the memory
            //m_p_face_normal_set.resize(m_number_of_vertices * 3, 0.0);
            m_p_vertex_normal_set.resize(m_number_of_vertices * 3, 0.0);

            // Process every triangle
            for (unsigned int i(0); i < m_number_of_vertices; i += 3)
                {
                // Get the vertex IDs
                int id_0(i + 0);
                int id_1(i + 1);
                int id_2(i + 2);

                // Get the vertices
                VEC3 vec_A(getVertex(id_0));
                VEC3 vec_B(getVertex(id_1));
                VEC3 vec_C(getVertex(id_2));

                // Compute the normal for the triangle
                VEC3 normal((vec_C - vec_A)^(vec_C - vec_B));

                // Normalise the vector
                normal.normalize();

                // Add the new normal
                /*m_p_face_normal_set[id_0 * 3 + 0] = normal.getX();
                m_p_face_normal_set[id_0 * 3 + 1] = normal.getY();
                m_p_face_normal_set[id_0 * 3 + 2] = normal.getZ();

                m_p_face_normal_set[id_1 * 3 + 0] = normal.getX();
                m_p_face_normal_set[id_1 * 3 + 1] = normal.getY();
                m_p_face_normal_set[id_1 * 3 + 2] = normal.getZ();

                m_p_face_normal_set[id_2 * 3 + 0] = normal.getX();
                m_p_face_normal_set[id_2 * 3 + 1] = normal.getY();
                m_p_face_normal_set[id_2 * 3 + 2] = normal.getZ();*/

                m_p_vertex_normal_set[id_0 * 3 + 0] = normal.getX();
                m_p_vertex_normal_set[id_0 * 3 + 1] = normal.getY();
                m_p_vertex_normal_set[id_0 * 3 + 2] = normal.getZ();

                m_p_vertex_normal_set[id_1 * 3 + 0] = normal.getX();
                m_p_vertex_normal_set[id_1 * 3 + 1] = normal.getY();
                m_p_vertex_normal_set[id_1 * 3 + 2] = normal.getZ();

                m_p_vertex_normal_set[id_2 * 3 + 0] = normal.getX();
                m_p_vertex_normal_set[id_2 * 3 + 1] = normal.getY();
                m_p_vertex_normal_set[id_2 * 3 + 2] = normal.getZ();
                }
            }
        break;
        }
}


//---------------------------------------------------------
void PolygonMesh::copyFrom(const PolygonMesh& aPolygonMesh)
//---------------------------------------------------------
{
    if (m_p_vertex_set)
    {
        destroyVertexData();
    }

    if (m_p_index_set)
    {
        destroyIndexData();
    }

    // Delete the VBO if needed
    m_p_vbo.reset(0);
    m_p_face_normal_vbo.reset(0);
    m_p_vertex_normal_vbo.reset(0);

    m_filename                = aPolygonMesh.m_filename;
    m_file_scale              = aPolygonMesh.m_file_scale;
    m_photon_cross_section    = aPolygonMesh.m_photon_cross_section;
    m_local_min_corner        = aPolygonMesh.m_local_min_corner;
    m_local_max_corner        =  aPolygonMesh.m_local_max_corner;
    //m_p_face_normal_set       = aPolygonMesh.m_p_face_normal_set;
    m_p_vertex_normal_set     = aPolygonMesh.m_p_vertex_normal_set;
    m_number_of_indices       = aPolygonMesh.m_number_of_indices;
    m_number_of_vertices      = aPolygonMesh.m_number_of_vertices;
    m_index_data_type         = aPolygonMesh.m_index_data_type;
    m_vertex_data_type        = aPolygonMesh.m_vertex_data_type;
    m_polygon_type            = aPolygonMesh.m_polygon_type;
    m_external_data_flag      = aPolygonMesh.m_external_data_flag;
    m_display_flag            = aPolygonMesh.m_display_flag;
    m_material                = aPolygonMesh.m_material;
    m_has_inverted_normal_vectors = aPolygonMesh.m_has_inverted_normal_vectors;

    if (m_number_of_vertices)
    {
        copyVertexSet(aPolygonMesh.m_p_vertex_set);
    }

    if (m_number_of_indices)
    {
        copyIndexSet(aPolygonMesh.m_p_index_set);
    }
}


//------------------------------------------------------------------
PolygonMesh& PolygonMesh::operator=(const PolygonMesh& aPolygonMesh)
//------------------------------------------------------------------
{
    copyFrom(aPolygonMesh);

    return (*this);
}


//-----------------------------------------------------------------------
PolygonMesh PolygonMesh::operator+(const PolygonMesh& aPolygonMesh) const
//-----------------------------------------------------------------------
{
    if (m_number_of_indices)
        {
        if (m_index_data_type != aPolygonMesh.m_index_data_type)
            {
            throw Exception(__FILE__, __FUNCTION__, __LINE__,
                    "Cannot combine the two polygon meshes, "
                    "their index data types are different.");

            }
        }

    if (m_number_of_vertices)
        {
        if (m_vertex_data_type != aPolygonMesh.m_vertex_data_type)
            {
            throw Exception(__FILE__, __FUNCTION__, __LINE__,
                    "Cannot combine the two polygon meshes, "
                    "their vertex data types are different.");

            }

        // Check the polygon types
        if (m_polygon_type != aPolygonMesh.m_polygon_type)
            {
            throw Exception(__FILE__, __FUNCTION__, __LINE__,
                    "Cannot combine the two polygon meshes, "
                    "their polygon types are different.");

            }
        }

    // Create the new instance
    PolygonMesh new_polygon_mesh;
    new_polygon_mesh.m_index_data_type  = aPolygonMesh.m_index_data_type;
    new_polygon_mesh.m_vertex_data_type = aPolygonMesh.m_vertex_data_type;
    new_polygon_mesh.m_polygon_type     = aPolygonMesh.m_polygon_type;

    // Recopy properties
    new_polygon_mesh.m_photon_cross_section = m_photon_cross_section;
    new_polygon_mesh.m_display_flag         = m_display_flag;
    new_polygon_mesh.m_material             = m_material;

    // The data is held by the instance
    new_polygon_mesh.m_external_data_flag = false;

    // Allocate memory space for the face normal set
    //new_polygon_mesh.m_p_face_normal_set.reserve(m_p_face_normal_set.size() +
    //        aPolygonMesh.m_p_face_normal_set.size());

    // Add the face normal vectors of the fist polygon mesh
    /*for (std::vector<RATIONAL_NUMBER>::const_iterator ite(
                    m_p_face_normal_set.begin());
            ite != m_p_face_normal_set.end();
            ++ite)
        {
        new_polygon_mesh.m_p_face_normal_set.push_back(*ite);
        }


    // Add the face normal vectors of the second polygon mesh
    for (std::vector<RATIONAL_NUMBER>::const_iterator ite(
                    aPolygonMesh.m_p_face_normal_set.begin());
            ite != aPolygonMesh.m_p_face_normal_set.end();
            ++ite)
        {
        new_polygon_mesh.m_p_face_normal_set.push_back(*ite);
        }*/

    // Allocate memory space for the vertex normal set
    new_polygon_mesh.m_p_vertex_normal_set.reserve(
            m_p_vertex_normal_set.size() +
            aPolygonMesh.m_p_vertex_normal_set.size());

    // Add the vertex normal vectors of the fist polygon mesh
    for (std::vector<RATIONAL_NUMBER>::const_iterator ite(
                    m_p_vertex_normal_set.begin());
            ite != m_p_vertex_normal_set.end();
            ++ite)
        {
        new_polygon_mesh.m_p_vertex_normal_set.push_back(*ite);
        }

    // Add the vertex normal vectors of the second polygon mesh
    for (std::vector<RATIONAL_NUMBER>::const_iterator ite(
                    aPolygonMesh.m_p_vertex_normal_set.begin());
            ite != aPolygonMesh.m_p_vertex_normal_set.end();
            ++ite)
        {
        new_polygon_mesh.m_p_vertex_normal_set.push_back(*ite);
        }

    // Allocate memory to store the vertex data
    new_polygon_mesh.m_number_of_vertices = m_number_of_vertices +
            aPolygonMesh.m_number_of_vertices;

    switch (new_polygon_mesh.m_vertex_data_type)
        {
        case GL_FLOAT:
            new_polygon_mesh.m_p_vertex_set =
                    new GLfloat[new_polygon_mesh.m_number_of_vertices * 3];
            break;

        case GL_DOUBLE:
            new_polygon_mesh.m_p_vertex_set =
                    new GLdouble[new_polygon_mesh.m_number_of_vertices * 3];
            break;

        default:
            throw Exception(__FILE__, __FUNCTION__, __LINE__,
                    "Invalid data type.");
        }

    if (new_polygon_mesh.m_number_of_vertices &&
            !new_polygon_mesh.m_p_vertex_set)
        {
        throw OutOfMemoryException(__FILE__, __FUNCTION__, __LINE__);
        }

    // Copy the data
    switch (new_polygon_mesh.m_vertex_data_type)
        {
        case GL_FLOAT:
            {
            GLfloat* p_temp(static_cast<GLfloat*>(new_polygon_mesh.m_p_vertex_set));

            std::memcpy(p_temp,
                    m_p_vertex_set,
                    m_number_of_vertices * sizeof(GLfloat) * 3);

            std::memcpy(p_temp + m_number_of_vertices * 3,
                    aPolygonMesh.m_p_vertex_set,
                    aPolygonMesh.m_number_of_vertices * sizeof(GLfloat) * 3);
            }
            break;

        case GL_DOUBLE:
            {
            GLdouble* p_temp(static_cast<GLdouble*>(new_polygon_mesh.m_p_vertex_set));

            std::memcpy(p_temp,
                    m_p_vertex_set,
                    m_number_of_vertices * sizeof(GLdouble) * 3);

            std::memcpy(p_temp + m_number_of_vertices * 3,
                    aPolygonMesh.m_p_vertex_set,
                    aPolygonMesh.m_number_of_vertices * sizeof(GLdouble) * 3);
            }
            break;
        }

    // Allocate memory to store the index data
    new_polygon_mesh.m_number_of_indices = m_number_of_indices +
            aPolygonMesh.m_number_of_indices;

    switch (new_polygon_mesh.m_index_data_type)
        {
        case 0:
            break;

        case GL_UNSIGNED_INT:
            new_polygon_mesh.m_p_index_set =
                    new GLuint[new_polygon_mesh.m_number_of_indices];
            break;

        case GL_UNSIGNED_SHORT:
            new_polygon_mesh.m_p_index_set =
                    new GLushort[new_polygon_mesh.m_number_of_indices];
            break;

        case GL_UNSIGNED_BYTE:
            new_polygon_mesh.m_p_index_set =
                    new GLubyte[new_polygon_mesh.m_number_of_indices];
            break;

        default:
            throw Exception(__FILE__, __FUNCTION__, __LINE__,
                    "Invalid data type.");
        }

    // Copy the data
    switch (new_polygon_mesh.m_index_data_type)
        {
        case GL_UNSIGNED_INT:
            {
            GLuint* p_temp1(reinterpret_cast<GLuint*>(
                    new_polygon_mesh.m_p_index_set));

            GLuint* p_temp2(reinterpret_cast<GLuint*>(
                    aPolygonMesh.m_p_index_set));

            std::memcpy(p_temp1,
                    m_p_index_set,
                    m_number_of_indices * sizeof(GLuint));

            p_temp1 += m_number_of_indices;

            for (unsigned int i(0); i < aPolygonMesh.m_number_of_indices; ++i)
                {
                *p_temp1++ = *p_temp2++ + getVertexNumber();
                }
            }
            break;

        case GL_UNSIGNED_SHORT:
            {
            GLushort* p_temp1(reinterpret_cast<GLushort*>(
                    new_polygon_mesh.m_p_index_set));

            GLushort* p_temp2(reinterpret_cast<GLushort*>(
                    aPolygonMesh.m_p_index_set));

            std::memcpy(p_temp1,
                    m_p_index_set,
                    m_number_of_indices * sizeof(GLshort));

            p_temp1 += m_number_of_indices;

            for (unsigned int i(0); i < aPolygonMesh.m_number_of_indices; ++i)
                {
                *p_temp1++ = *p_temp2++ + getVertexNumber();
                }
            }
            break;

        case GL_UNSIGNED_BYTE:
            {
            GLubyte* p_temp1(reinterpret_cast<GLubyte*>(
                    new_polygon_mesh.m_p_index_set));

            GLubyte* p_temp2(reinterpret_cast<GLubyte*>(
                    aPolygonMesh.m_p_index_set));

            std::memcpy(p_temp1,
                    m_p_index_set,
                    m_number_of_indices * sizeof(GLubyte));

            p_temp1 += m_number_of_indices;

            for (unsigned int i(0); i < aPolygonMesh.m_number_of_indices; ++i)
                {
                *p_temp1++ = *p_temp2++ + getVertexNumber();
                }
            }
            break;
        }

    // Update internal data
    new_polygon_mesh.computeBoundingBox();

    // Return the new polygon mesh
    return (new_polygon_mesh);
}


//-----------------------------------------------------------------------
PolygonMesh PolygonMesh::operator-(const PolygonMesh& aPolygonMesh) const
//-----------------------------------------------------------------------
{
    if (m_index_data_type != aPolygonMesh.m_index_data_type)
        {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
                "Cannot combine the two polygon meshes, "
                "their index data types are different.");

        }

    if (m_vertex_data_type != aPolygonMesh.m_vertex_data_type)
        {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
                "Cannot combine the two polygon meshes, "
                "their vertex data types are different.");

        }

    // Check the polygon types
    if (m_polygon_type != aPolygonMesh.m_polygon_type)
        {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
                "Cannot combine the two polygon meshes, "
                "their polygon types are different.");

        }

    // Create the new instance
    PolygonMesh new_polygon_mesh;
    new_polygon_mesh.m_index_data_type  = m_index_data_type;
    new_polygon_mesh.m_vertex_data_type = m_vertex_data_type;
    new_polygon_mesh.m_polygon_type     = m_polygon_type;

    // Recopy properties
    new_polygon_mesh.m_photon_cross_section = m_photon_cross_section;
    new_polygon_mesh.m_display_flag = m_display_flag;
    new_polygon_mesh.m_material = m_material;

    // The data is held by the instance
    new_polygon_mesh.m_external_data_flag = false;

    // Allocate memory space for the face normal set
    /*new_polygon_mesh.m_p_face_normal_set.reserve(m_p_face_normal_set.size() +
            aPolygonMesh.m_p_face_normal_set.size());

    // Add the face normal vectors of the fist polygon mesh
    for (std::vector<RATIONAL_NUMBER>::const_iterator ite(
                    m_p_face_normal_set.begin());
            ite != m_p_face_normal_set.end();
            ++ite)
        {
        new_polygon_mesh.m_p_face_normal_set.push_back(*ite);
        }

    // Add the face normal vectors of the second polygon mesh
    for (std::vector<RATIONAL_NUMBER>::const_iterator ite(
                    aPolygonMesh.m_p_face_normal_set.begin());
            ite != aPolygonMesh.m_p_face_normal_set.end();
            ++ite)
        {
        // Inverse the normal
        new_polygon_mesh.m_p_face_normal_set.push_back(-*ite);
        }*/

    // Allocate memory space for the vertex normal set
    new_polygon_mesh.m_p_vertex_normal_set.reserve(
            m_p_vertex_normal_set.size() +
            aPolygonMesh.m_p_vertex_normal_set.size());

    // Add the vertex normal vectors of the fist polygon mesh
    for (std::vector<RATIONAL_NUMBER>::const_iterator ite(
                    m_p_vertex_normal_set.begin());
            ite != m_p_vertex_normal_set.end();
            ++ite)
        {
        new_polygon_mesh.m_p_vertex_normal_set.push_back(*ite);
        }

    // Add the vertex normal vectors of the second polygon mesh
    for (std::vector<RATIONAL_NUMBER>::const_reverse_iterator ite(
                    aPolygonMesh.m_p_vertex_normal_set.rbegin());
            ite != aPolygonMesh.m_p_vertex_normal_set.rend();
            ++ite)
        {
        // Inverse the normal
        new_polygon_mesh.m_p_vertex_normal_set.push_back(-(*ite));
        }

    // Allocate memory to store the vertex data
    new_polygon_mesh.m_number_of_vertices = m_number_of_vertices +
            aPolygonMesh.m_number_of_vertices;

    switch (new_polygon_mesh.m_vertex_data_type)
        {
        case GL_FLOAT:
            new_polygon_mesh.m_p_vertex_set =
                    new GLfloat[new_polygon_mesh.m_number_of_vertices * 3];
            break;

        case GL_DOUBLE:
            new_polygon_mesh.m_p_vertex_set =
                    new GLdouble[new_polygon_mesh.m_number_of_vertices * 3];
            break;

        default:
            throw Exception(__FILE__, __FUNCTION__, __LINE__,
                    "Invalid data type.");
        }

    if (new_polygon_mesh.m_number_of_vertices &&
            !new_polygon_mesh.m_p_vertex_set)
        {
        throw OutOfMemoryException(__FILE__, __FUNCTION__, __LINE__);
        }

    // Copy the data
    switch (new_polygon_mesh.m_vertex_data_type)
        {
        case GL_FLOAT:
            {
            GLfloat* p_temp(static_cast<GLfloat*>(new_polygon_mesh.m_p_vertex_set));

            std::memcpy(p_temp,
                    m_p_vertex_set,
                    m_number_of_vertices * sizeof(GLfloat) * 3);

            for (unsigned int i = 0; i < aPolygonMesh.m_number_of_vertices; ++i)
            {
                unsigned int id1 = i * 3 + m_number_of_vertices * 3;
                unsigned int id2 = (aPolygonMesh.m_number_of_vertices - 1 - i) * 3;

                p_temp[id1 + 0] = ((GLfloat*)aPolygonMesh.m_p_vertex_set)[id2 + 0];
                p_temp[id1 + 1] = ((GLfloat*)aPolygonMesh.m_p_vertex_set)[id2 + 1];
                p_temp[id1 + 2] = ((GLfloat*)aPolygonMesh.m_p_vertex_set)[id2 + 2];
            }
            //std::memcpy(p_temp + m_number_of_vertices * 3,
            //        aPolygonMesh.m_p_vertex_set,
            //        aPolygonMesh.m_number_of_vertices * sizeof(GLfloat) * 3);
            }
            break;

        case GL_DOUBLE:
            {
            GLdouble* p_temp(static_cast<GLdouble*>(new_polygon_mesh.m_p_vertex_set));

            std::memcpy(p_temp,
                    m_p_vertex_set,
                    m_number_of_vertices * sizeof(GLdouble) * 3);

            for (unsigned int i = 0; i < aPolygonMesh.m_number_of_vertices; ++i)
            {
                unsigned int id1 = i * 3 + m_number_of_vertices * 3;
                unsigned int id2 = (aPolygonMesh.m_number_of_vertices - 1 - i) * 3;

                p_temp[id1 + 0] = ((GLdouble*)aPolygonMesh.m_p_vertex_set)[id2 + 0];
                p_temp[id1 + 1] = ((GLdouble*)aPolygonMesh.m_p_vertex_set)[id2 + 1];
                p_temp[id1 + 2] = ((GLdouble*)aPolygonMesh.m_p_vertex_set)[id2 + 2];
            }
            //std::memcpy(p_temp + m_number_of_vertices * 3,
            //        aPolygonMesh.m_p_vertex_set,
            //        aPolygonMesh.m_number_of_vertices * sizeof(GLdouble) * 3);
            }
            break;
        }

    // Allocate memory to store the index data
    new_polygon_mesh.m_number_of_indices = m_number_of_indices +
            aPolygonMesh.m_number_of_indices;

    switch (m_index_data_type)
        {
        case 0:
            break;

        case GL_UNSIGNED_INT:
            new_polygon_mesh.m_p_index_set =
                    new GLuint[new_polygon_mesh.m_number_of_indices];
            break;

        case GL_UNSIGNED_SHORT:
            new_polygon_mesh.m_p_index_set =
                    new GLushort[new_polygon_mesh.m_number_of_indices];
            break;

        case GL_UNSIGNED_BYTE:
            new_polygon_mesh.m_p_index_set =
                    new GLubyte[new_polygon_mesh.m_number_of_indices];
            break;

        default:
            throw Exception(__FILE__, __FUNCTION__, __LINE__,
                    "Invalid data type.");
        }

    // Copy the data
    switch (new_polygon_mesh.m_index_data_type)
        {
        case GL_UNSIGNED_INT:
            {
            GLuint* p_temp1(reinterpret_cast<GLuint*>(
                    new_polygon_mesh.m_p_index_set));

            GLuint* p_temp2(reinterpret_cast<GLuint*>(
                    aPolygonMesh.m_p_index_set));

            std::memcpy(p_temp1,
                    m_p_index_set,
                    m_number_of_indices * sizeof(GLuint));

            p_temp1 += m_number_of_indices;

            for (unsigned int i(0); i < aPolygonMesh.m_number_of_indices; ++i)
                {
                *p_temp1++ = aPolygonMesh.getVertexNumber() - 1- p_temp2[aPolygonMesh.m_number_of_indices - 1 - i] + getVertexNumber();
                }
            }
            break;

        case GL_UNSIGNED_SHORT:
            {
            GLushort* p_temp1(reinterpret_cast<GLushort*>(
                    new_polygon_mesh.m_p_index_set));

            GLushort* p_temp2(reinterpret_cast<GLushort*>(
                    aPolygonMesh.m_p_index_set));

            std::memcpy(p_temp1,
                    m_p_index_set,
                    m_number_of_indices * sizeof(GLshort));

            p_temp1 += m_number_of_indices;

            for (unsigned int i(0); i < aPolygonMesh.m_number_of_indices; ++i)
                {
                *p_temp1++ = aPolygonMesh.getVertexNumber() - 1 - p_temp2[aPolygonMesh.m_number_of_indices - 1 - i] + getVertexNumber();
                }
            }
            break;

        case GL_UNSIGNED_BYTE:
            {
            GLubyte* p_temp1(reinterpret_cast<GLubyte*>(
                    new_polygon_mesh.m_p_index_set));

            GLubyte* p_temp2(reinterpret_cast<GLubyte*>(
                    aPolygonMesh.m_p_index_set));

            std::memcpy(p_temp1,
                    m_p_index_set,
                    m_number_of_indices * sizeof(GLubyte));

            p_temp1 += m_number_of_indices;

            for (unsigned int i(0); i < aPolygonMesh.m_number_of_indices; ++i)
                {
                *p_temp1++ = aPolygonMesh.getVertexNumber() - 1- p_temp2[aPolygonMesh.m_number_of_indices - 1 - i] + getVertexNumber();
                }
            }
            break;
        }

    // Update internal data
    new_polygon_mesh.computeBoundingBox();

    // Return the new polygon mesh
    return (new_polygon_mesh);
}


//-------------------------------------------------------------------
PolygonMesh& PolygonMesh::operator+=(const PolygonMesh& aPolygonMesh)
//-------------------------------------------------------------------
{
    PolygonMesh new_polygon(*this + aPolygonMesh);
    *this = new_polygon;

    return (*this);
}


//-------------------------------------------------------------------
PolygonMesh& PolygonMesh::operator-=(const PolygonMesh& aPolygonMesh)
//-------------------------------------------------------------------
{
    PolygonMesh new_polygon(*this - aPolygonMesh);
    *this = new_polygon;

    return (*this);
}


//----------------------------------------------------------------------
PolygonMesh PolygonMesh::operator+(const VEC3& aTranslationVector) const
//----------------------------------------------------------------------
{
    // Copy the current instance into a new polygon mesh
    PolygonMesh new_polygon_mesh(*this);

    // Translate all the vertices
    new_polygon_mesh.applyTranslation(aTranslationVector);

    // Return the new polygon mesh
    return (new_polygon_mesh);
}


//----------------------------------------------------------------------
PolygonMesh PolygonMesh::operator-(const VEC3& aTranslationVector) const
//----------------------------------------------------------------------
{
    // Copy the current instance into a new polygon mesh
    PolygonMesh new_polygon_mesh(*this);

    // Translate all the vertices
    new_polygon_mesh.applyTranslation(-aTranslationVector);

    // Return the new polygon mesh
    return (new_polygon_mesh);
}


//------------------------------------------------------------------
PolygonMesh& PolygonMesh::operator+=(const VEC3& aTranslationVector)
//------------------------------------------------------------------
{
    // Translate all the vertices
    applyTranslation(aTranslationVector);

    // Return the current instance
    return (*this);
}


//------------------------------------------------------------------
PolygonMesh& PolygonMesh::operator-=(const VEC3& aTranslationVector)
//------------------------------------------------------------------
{
    // Translate all the vertices
    applyTranslation(-aTranslationVector);

    // Return the current instance
    return (*this);
}


std::map<RATIONAL_NUMBER, VEC3> PolygonMesh::rayIntersect(
        const VEC3& aRayOrigin,
        const VEC3& aRayDirection,
        const MATRIX4& aTransformationMatrix) const
{
    // Implements the Möller–Trumbore ray-triangle intersection algorithm
    // https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
    std::map<RATIONAL_NUMBER, VEC3> p_intersection_set;


    VEC3 v0;
    VEC3 v1;
    VEC3 v2;
    VEC3 N;

    VEC3 edge1, edge2, h, s, q;
    double a,f,u,v;

    // Process all the triangles of the mesh

    for (unsigned int i = 0; i < getTriangleNumber(); ++i)
    {
        // Get the triangle vertices
        getTriangleVertices(v0, v1, v2, i);

        // Get the normal vector
        N = getFaceNormal(i);

        // Transform the vertices and normal vector
        v0 = aTransformationMatrix * v0;
        v1 = aTransformationMatrix * v1;
        v2 = aTransformationMatrix * v2;
        N = (aTransformationMatrix * N).normal();


        edge1 = v1 - v0;
        edge2 = v2 - v0;
        h = aRayDirection.crossProduct(edge2);
        a = edge1.dotProduct(h);
        if (!(a > -EPSILON && a < EPSILON))
        {
            f = 1.0 / a;
            s = aRayOrigin - v0;
            u = f * (s.dotProduct(h));
            if (!(u < 0.0 || u > 1.0))
            {
                q = s.crossProduct(edge1);
                v = f * aRayDirection.dotProduct(q);
                if (!(v < 0.0 || u + v > 1.0))
                {
                    // At this stage we can compute t to find out where the intersection point is on the line.
                    float t = f * edge2.dotProduct(q);
                    if (t > EPSILON) // ray intersection
                    {
                        //VEC3 outIntersectionPoint = aRayOrigin + aRayDirection * t;
                        p_intersection_set[t] = N;
                    }
                }
            }
        }
    }

    return p_intersection_set;
}


void PolygonMesh::getTriangleVertices(VEC3& v0,
                                      VEC3& v1,
                                      VEC3& v2,
                                      unsigned int anIndex) const
{
    // The polygon is made of triangles
    if (m_polygon_type == GL_TRIANGLES)
    {
        if (anIndex >= getFaceNumber())
        {
            throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
        }

        if (m_number_of_indices)
        {
            v0 = getVertex(getIndex(anIndex * 3 + 0));
            v1 = getVertex(getIndex(anIndex * 3 + 1));
            v2 = getVertex(getIndex(anIndex * 3 + 2));
        }
        else
        {
            v0 = getVertex(anIndex * 3 + 0);
            v1 = getVertex(anIndex * 3 + 1);
            v2 = getVertex(anIndex * 3 + 2);
        }
    }
    else
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
                "Polygon type not supported.");
    }
}


//----------------------------------
void PolygonMesh::normaliseNormals()
//----------------------------------
{
    if (!getFaceNormalNumber() && !getVertexNormalNumber())
        {
        throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
        }

    /*for (unsigned int i(0); i < getFaceNormalNumber(); ++i)
        {
        VEC3 normal(m_p_face_normal_set[i * 3 + 0],
                m_p_face_normal_set[i * 3 + 1],
                m_p_face_normal_set[i * 3 + 2]);

        normal.normalise();

        m_p_face_normal_set[i * 3 + 0] = normal.getX();
        m_p_face_normal_set[i * 3 + 1] = normal.getY();
        m_p_face_normal_set[i * 3 + 2] = normal.getZ();
        }*/

    for (unsigned int i(0); i < getVertexNormalNumber(); ++i)
        {
        VEC3 normal(m_p_vertex_normal_set[i * 3 + 0],
                m_p_vertex_normal_set[i * 3 + 1],
                m_p_vertex_normal_set[i * 3 + 2]);

        normal.normalise();

        m_p_vertex_normal_set[i * 3 + 0] = normal.getX();
        m_p_vertex_normal_set[i * 3 + 1] = normal.getY();
        m_p_vertex_normal_set[i * 3 + 2] = normal.getZ();
        }
}


struct VectorComparer
{
    // Return true if aLeftNode is less than aRightNode
    bool operator()(const VEC3 & aLeftNode, const VEC3 & aRightNode) const
    {
        double epsilon(EPSILON);

        if (fabs(aLeftNode.getX() - aRightNode.getX()) > epsilon)
            {
            if (aLeftNode.getX() < aRightNode.getX())
                {
                return (true);
                }
            else
                {
                return (false);
                }
            }

        if (fabs(aLeftNode.getY() - aRightNode.getY()) > epsilon)
            {
            if (aLeftNode.getY() < aRightNode.getY())
                {
                return (true);
                }
            else
                {
                return (false);
                }
            }

        if (fabs(aLeftNode.getZ() - aRightNode.getZ()) > epsilon)
            {
            if (aLeftNode.getZ() < aRightNode.getZ())
                {
                return (true);
                }
            else
                {
                return (false);
                }
            }

        return (false);
    }
};


class TempRecord
{
public:
    inline const TempRecord& operator += (const VEC3& aNormal)
    {
        m_normal += aNormal;

        return (*this);
    }


    inline const TempRecord& operator = (unsigned int anID)
    {
        m_id = anID;

        return (*this);
    }

    VEC3 m_normal;
    unsigned int m_id;
};


//----------------------------------------------------------
void PolygonMesh::applyScale(const RATIONAL_NUMBER& aScaleX,
                             const RATIONAL_NUMBER& aScaleY,
                             const RATIONAL_NUMBER& aScaleZ)
//----------------------------------------------------------
{
    // There are vertices
    if (m_number_of_vertices)
    {
        for (unsigned int i(0); i < m_number_of_vertices; ++i)
        {
            VEC3 vertex(getVertex(i));

            vertex[0] *= aScaleX;
            vertex[1] *= aScaleY;
            vertex[2] *= aScaleZ;

            setVertex(i, vertex);
        }

        // Update the bounding box
        m_local_min_corner[0] *= aScaleX;
        m_local_min_corner[1] *= aScaleY;
        m_local_min_corner[2] *= aScaleZ;

        m_local_max_corner[0] *= aScaleX;
        m_local_max_corner[1] *= aScaleY;
        m_local_max_corner[2] *= aScaleZ;

        // Create the VBO if needed
        /*if (!m_p_vbo.get())
        {
            m_p_vbo.reset(createVBO());
        }*/

        // Update the VBO
        if (m_p_vbo.get())
        {
            if (m_p_vbo->isReady())
            {
                updateVBO(m_p_vbo->getBufferUsageHint(), m_p_vbo->getPrimitiveType());
            }
        }
    }
}



//-------------------------------------------------------
void PolygonMesh::mergeVertices(bool aPrintDebugInfoFlag)
//-------------------------------------------------------
{
    // Merge vertices only if external data is not used
    if (!m_external_data_flag)
        {
        // It only works if no index has been set
        if (!m_p_index_set && !m_number_of_indices)
            {
            // The polygon is made of triangles
            if (m_polygon_type == GL_TRIANGLES)
                {
                // Allocate the memory of the index
                int number_of_vertices_per_face(3);
                m_number_of_indices = getFaceNumber() *
                        number_of_vertices_per_face;
                GLuint* p_index_set(new GLuint[m_number_of_indices]);
                m_index_data_type = GL_UNSIGNED_INT;
                m_p_index_set = p_index_set;

                // Add the vertices to the hash table
                std::map<VEC3, TempRecord, VectorComparer> hash_table;
                unsigned int number_of_vertices(m_number_of_vertices);
                for (unsigned int i(0); i < number_of_vertices; ++i)
                    {
                    VEC3 current_vertex(getVertex(i));
                    VEC3 normal(getVertexNormal(i));

                    hash_table[current_vertex].m_normal += normal;
                    }

                m_p_vertex_normal_set.clear();
                m_p_vertex_normal_set.resize(hash_table.size() * 3);
                unsigned int id(0);
                for (std::map<VEC3, TempRecord, VectorComparer>::iterator ite(
                                    hash_table.begin());
                        ite != hash_table.end();
                        ++ite, ++id)
                    {
                    VEC3 normal(ite->second.m_normal.normal());

                    m_p_vertex_normal_set[id * 3 + 0] = normal.getX();
                    m_p_vertex_normal_set[id * 3 + 1] = normal.getY();
                    m_p_vertex_normal_set[id * 3 + 2] = normal.getZ();
                    }

                // Set the index in the hash table
                id = 0;
                for (std::map<VEC3, TempRecord, VectorComparer>::iterator ite(
                                    hash_table.begin());
                        ite != hash_table.end();
                        ++ite, ++id)
                    {
                    ite->second.m_id = id;
                    }

                // Fill the index
                unsigned int number_of_triangles(getTriangleNumber());
                for (unsigned int i(0); i < number_of_triangles; ++i)
                    {
                    VEC3 vertex_0(getVertex(i * 3 + 0));
                    VEC3 vertex_1(getVertex(i * 3 + 1));
                    VEC3 vertex_2(getVertex(i * 3 + 2));

                    unsigned int id_0(hash_table.find(vertex_0)->second.m_id);
                    unsigned int id_1(hash_table.find(vertex_1)->second.m_id);
                    unsigned int id_2(hash_table.find(vertex_2)->second.m_id);

                    if (id_0 >= hash_table.size() ||
                            id_1 >= hash_table.size() ||
                            id_2 >= hash_table.size())
                        {
                        throw Exception(__FILE__, __FUNCTION__, __LINE__,
                                "Invalid vertex index");
                        }

                    p_index_set[i * 3 + 0] = id_0;
                    p_index_set[i * 3 + 1] = id_1;
                    p_index_set[i * 3 + 2] = id_2;
                    }

                // Set the index in the hash table and
                // create the list of vertices

                // Store vertex data type
                int old_type = m_vertex_data_type;
                destroyVertexData();

                // Restore the type
                m_vertex_data_type = old_type;

                // Set the number of vertices
                m_number_of_vertices = hash_table.size();

                GLfloat* p_float_vertex_set(0);
                GLdouble* p_double_vertex_set(0);
                switch (m_vertex_data_type)
                    {
                    case GL_FLOAT:
                        m_p_vertex_set = new GLfloat[m_number_of_vertices * 3];
                        p_float_vertex_set =
                                reinterpret_cast<GLfloat*>(m_p_vertex_set);
                        break;

                    case GL_DOUBLE:
                        m_p_vertex_set = new GLdouble[m_number_of_vertices * 3];
                        p_double_vertex_set =
                                reinterpret_cast<GLdouble*>(m_p_vertex_set);
                        break;

                    default:
                        throw Exception(__FILE__, __FUNCTION__, __LINE__,
                                "Invalid data type.");
                    }

                id = 0;
                for (std::map<VEC3, TempRecord, VectorComparer>::iterator ite(
                                    hash_table.begin());
                        ite != hash_table.end();
                        ++ite, ++id)
                    {
                    if (p_float_vertex_set)
                        {
                        p_float_vertex_set[id * 3 + 0] = ite->first.getX();
                        p_float_vertex_set[id * 3 + 1] = ite->first.getY();
                        p_float_vertex_set[id * 3 + 2] = ite->first.getZ();
                        }
                    if (p_double_vertex_set)
                        {
                        p_double_vertex_set[id * 3 + 0] = ite->first.getX();
                        p_double_vertex_set[id * 3 + 1] = ite->first.getY();
                        p_double_vertex_set[id * 3 + 2] = ite->first.getZ();
                        }
                    }

                // Normalise the normals in the normal set
                unsigned int number_of_normals(getVertexNormalNumber());
                for (unsigned int i(0); i < number_of_normals; ++i)
                    {
                    VEC3 normal(getVertexNormal(i));
                    normal.normalise();
                    m_p_vertex_normal_set[i * 3 + 0] = normal.getX();
                    m_p_vertex_normal_set[i * 3 + 1] = normal.getY();
                    m_p_vertex_normal_set[i * 3 + 2] = normal.getZ();
                    }

                // Reset the normal per face set
                //m_p_face_normal_set.clear();

                // Create the VBO if needed
                if (!m_p_vbo.get())
                    {
                    m_p_vbo.reset(createVBO());
                    }

                // Update the VBO
                if (m_p_vbo->isReady())
                    {
                    updateVBO(m_p_vbo->getBufferUsageHint(),
                            m_p_vbo->getPrimitiveType());
                    }

                // Display extra debug info
                if (aPrintDebugInfoFlag)
                    {
                    LOGGER.logNow("file_name:\t") << m_filename <<
                            "\tnb_faces:\t" << getFaceNumber() <<
                            "\tnb_vertices:\t" << m_number_of_vertices <<
                            "\tbounding_box (in cm):\t(" <<
                                m_local_min_corner.getX() / cm << ", " <<
                                m_local_min_corner.getY() / cm << ", " <<
                                m_local_min_corner.getZ() / cm << ")\t(" <<
                                m_local_max_corner.getX() / cm << ", " <<
                                m_local_max_corner.getY() / cm << ", " <<
                                m_local_max_corner.getZ() / cm << ")" << std::endl;
                    }
                }
            // Not supported
            else
                {
                throw NotImplementedException(__FILE__, __FUNCTION__, __LINE__);
                }
            }
        // Generate an error
        else
            {
            throw Exception(__FILE__, __FUNCTION__, __LINE__,
                    "There is no vertex to merge.");
            }
        }
    // Generate an error
    else
        {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
                "The operation cannot be performed on external data.");
        }
}


//----------------------------------------------------
void PolygonMesh::splitFaces(bool aPrintDebugInfoFlag)
//----------------------------------------------------
{
    // Split faces only if external data is not used
    if (!m_external_data_flag)
        {
        // It only works if no index has been set
        bool has_an_index(false);
        if (m_p_index_set && m_number_of_indices)
            {
            removeIndex(aPrintDebugInfoFlag);
            has_an_index = true;
            }

        // The polygon is made of triangles
        if (m_polygon_type == GL_TRIANGLES)
            {
            // Reset the normal per face set
            m_p_vertex_normal_set.clear();

            // Reset the normal per face set
            //m_p_face_normal_set.clear();

            // Store the new vertices
            std::vector<double> p_new_vertex_set;

            // Process every triangle
            unsigned int triangle_number(getTriangleNumber());
            for (unsigned int i(0); i < triangle_number; ++i)
                {
                // Get the current vertices
                VEC3 vertex_0(getVertex(i * 3 + 0));
                VEC3 vertex_1(getVertex(i * 3 + 1));
                VEC3 vertex_2(getVertex(i * 3 + 2));

                // Get the new vertices
                VEC3 vertex_3((vertex_0 + vertex_1) / 2.0);
                VEC3 vertex_4((vertex_1 + vertex_2) / 2.0);
                VEC3 vertex_5((vertex_2 + vertex_0) / 2.0);

                // Compute the normal vector
                VEC3 normal((vertex_2 - vertex_0)^(vertex_2 - vertex_1));
                normal.normalize();

                // Add the first triangle
                p_new_vertex_set.push_back(vertex_0.getX());
                p_new_vertex_set.push_back(vertex_0.getY());
                p_new_vertex_set.push_back(vertex_0.getZ());
                p_new_vertex_set.push_back(vertex_3.getX());
                p_new_vertex_set.push_back(vertex_3.getY());
                p_new_vertex_set.push_back(vertex_3.getZ());
                p_new_vertex_set.push_back(vertex_5.getX());
                p_new_vertex_set.push_back(vertex_5.getY());
                p_new_vertex_set.push_back(vertex_5.getZ());

                for (unsigned int j(0); j < 3; ++j)
                    {
                    m_p_vertex_normal_set.push_back(normal.getX());
                    m_p_vertex_normal_set.push_back(normal.getY());
                    m_p_vertex_normal_set.push_back(normal.getZ());
                    }

                // Add the second triangle
                p_new_vertex_set.push_back(vertex_3.getX());
                p_new_vertex_set.push_back(vertex_3.getY());
                p_new_vertex_set.push_back(vertex_3.getZ());
                p_new_vertex_set.push_back(vertex_1.getX());
                p_new_vertex_set.push_back(vertex_1.getY());
                p_new_vertex_set.push_back(vertex_1.getZ());
                p_new_vertex_set.push_back(vertex_4.getX());
                p_new_vertex_set.push_back(vertex_4.getY());
                p_new_vertex_set.push_back(vertex_4.getZ());

                for (unsigned int j(0); j < 3; ++j)
                    {
                    m_p_vertex_normal_set.push_back(normal.getX());
                    m_p_vertex_normal_set.push_back(normal.getY());
                    m_p_vertex_normal_set.push_back(normal.getZ());
                    }

                // Add the third triangle
                p_new_vertex_set.push_back(vertex_3.getX());
                p_new_vertex_set.push_back(vertex_3.getY());
                p_new_vertex_set.push_back(vertex_3.getZ());
                p_new_vertex_set.push_back(vertex_4.getX());
                p_new_vertex_set.push_back(vertex_4.getY());
                p_new_vertex_set.push_back(vertex_4.getZ());
                p_new_vertex_set.push_back(vertex_5.getX());
                p_new_vertex_set.push_back(vertex_5.getY());
                p_new_vertex_set.push_back(vertex_5.getZ());

                for (unsigned int j(0); j < 3; ++j)
                    {
                    m_p_vertex_normal_set.push_back(normal.getX());
                    m_p_vertex_normal_set.push_back(normal.getY());
                    m_p_vertex_normal_set.push_back(normal.getZ());
                    }

                // Add the fourth triangle
                p_new_vertex_set.push_back(vertex_5.getX());
                p_new_vertex_set.push_back(vertex_5.getY());
                p_new_vertex_set.push_back(vertex_5.getZ());
                p_new_vertex_set.push_back(vertex_4.getX());
                p_new_vertex_set.push_back(vertex_4.getY());
                p_new_vertex_set.push_back(vertex_4.getZ());
                p_new_vertex_set.push_back(vertex_2.getX());
                p_new_vertex_set.push_back(vertex_2.getY());
                p_new_vertex_set.push_back(vertex_2.getZ());

                for (unsigned int j(0); j < 3; ++j)
                    {
                    m_p_vertex_normal_set.push_back(normal.getX());
                    m_p_vertex_normal_set.push_back(normal.getY());
                    m_p_vertex_normal_set.push_back(normal.getZ());
                    }
                }

            // Store the data type
            int old_m_vertex_data_type = m_vertex_data_type;

            // Set the index in the hash table and create the list of vertices
            destroyVertexData();
            m_number_of_vertices = p_new_vertex_set.size() / 3;

            // Restore the datatype
            m_vertex_data_type = old_m_vertex_data_type;

            GLfloat* p_float_vertex_set(0);
            GLdouble* p_double_vertex_set(0);
            switch (m_vertex_data_type)
                {
                case GL_FLOAT:
                    m_p_vertex_set = new GLfloat[m_number_of_vertices * 3];
                    p_float_vertex_set =
                            reinterpret_cast<GLfloat*>(m_p_vertex_set);
                    break;

                case GL_DOUBLE:
                    m_p_vertex_set = new GLdouble[m_number_of_vertices * 3];
                    p_double_vertex_set =
                            reinterpret_cast<GLdouble*>(m_p_vertex_set);
                    break;

                default:
                    throw Exception(__FILE__, __FUNCTION__, __LINE__,
                            "Invalid data type.");
                }

            // Save the result
            for (unsigned int id(0); id < p_new_vertex_set.size(); ++id)
                {
                if (p_float_vertex_set)
                    {
                    p_float_vertex_set[id] = p_new_vertex_set[id];
                    }
                if (p_double_vertex_set)
                    {
                    p_double_vertex_set[id] = p_new_vertex_set[id];
                    }
                }

            // Restore the index
            if (has_an_index)
                {
                mergeVertices(aPrintDebugInfoFlag);
                }

            // Create the VBO if needed
            if (!m_p_vbo.get())
                {
                m_p_vbo.reset(createVBO());
                }

            // Update the VBO
            if (m_p_vbo->isReady())
                {
                updateVBO(m_p_vbo->getBufferUsageHint(),
                        m_p_vbo->getPrimitiveType());
                }

            // Display extra debug info
            if (aPrintDebugInfoFlag)
                {
                LOGGER.logNow("file_name:\t") << m_filename <<
                        "\tnb_faces:\t" << getFaceNumber() <<
                        "\tnb_vertices:\t" << m_number_of_vertices <<
                        "\tbounding_box (in cm):\t(" <<
                        m_local_min_corner.getX() / cm << ", " <<
                        m_local_min_corner.getY() / cm << ", " <<
                        m_local_min_corner.getZ() / cm << ")\t(" <<
                        m_local_max_corner.getX() / cm << ", " <<
                        m_local_max_corner.getY() / cm << ", " <<
                        m_local_max_corner.getZ() / cm << ")" << std::endl;
                }
            }
        // Not supported
        else
            {
            throw NotImplementedException(__FILE__, __FUNCTION__, __LINE__);
            }
        }
    // Generate an error
    else
        {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
                "The operation cannot be performed on external data.");
        }
}


//---------------------------------------------------------------
void PolygonMesh::updateVBO(int aTypeOfVBO, int aTypeOfPrimitive)
//---------------------------------------------------------------
{
    if (m_p_vertex_set && m_number_of_vertices)
        {
        // Create the VBO if needed
        if (!m_p_vbo.get())
            {
            m_p_vbo.reset(createVBO());
            }

        // Set the type of VBO
        m_p_vbo->setBufferUsageHint(aTypeOfVBO);

        if (getVertexNormalNumber())
            {
            m_p_vbo->setVertexData(aTypeOfPrimitive,
                    m_number_of_vertices,
                    3, m_vertex_data_type,
                    m_p_vertex_set,
                    getVertexNormalNumber(),
                    3,
                    GL_FLOAT,
                    &m_p_vertex_normal_set[0],
                    0,
                    0,
                    GL_NONE,
                    0);
            }
        else if (getFaceNormalNumber())
            {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "Do not use face normal.");

/*
            m_p_vbo->setVertexData(aTypeOfPrimitive,
                    m_number_of_vertices,
                    3,
                    m_vertex_data_type,
                    m_p_vertex_set,
                    getFaceNormalNumber(),
                    3,
                    GL_FLOAT,
                    &m_p_face_normal_set[0],
                    0,
                    0,
                    GL_NONE,
                    0);*/
            }
        else
            {
            m_p_vbo->setVertexData(aTypeOfPrimitive,
                    m_number_of_vertices,
                    3,
                    m_vertex_data_type,
                    m_p_vertex_set,
                    0,
                    0,
                    GL_NONE,
                    0,
                    0,
                    0,
                    GL_NONE,
                    0);
            }

        if (m_p_index_set && m_number_of_indices &&
                m_index_data_type != GL_NONE)
            {
            m_p_vbo->setIndexData(m_number_of_indices,
                    m_index_data_type,
                    m_p_index_set);
            }
        }
}
//---------------------------------------------------------------
void PolygonMesh::updateVBOs(unsigned int aVertexVBO,
    unsigned int aNormalVBO,
    unsigned int aTextureVBO,
    unsigned int anIndexVBO)
    //---------------------------------------------------------------
{
    if (m_p_vertex_set && m_number_of_vertices)
    {
        // Create the VBO if needed
        if (!m_p_vbo.get())
        {
            m_p_vbo.reset(createVBO());
        }

        // Set the type of VBO
        m_p_vbo->setBufferUsageHint(GL_DYNAMIC_DRAW);

        OpenGL3VBO* aux_vbo = dynamic_cast<OpenGL3VBO*>(m_p_vbo.get());
        if (aux_vbo != NULL)

            aux_vbo->setVertexVBO(
            m_number_of_vertices,
            aVertexVBO,
            aNormalVBO,
            aTextureVBO);


        if (anIndexVBO && m_number_of_indices &&
            m_index_data_type != GL_NONE)
        {
            aux_vbo->setIndexVBO(
                m_number_of_indices,
                m_index_data_type,
                anIndexVBO);
        }


    }
}


//-----------------------------------------------------
VEC3 PolygonMesh::getVertex(unsigned int anIndex) const
//-----------------------------------------------------
{
    if (m_number_of_vertices <= anIndex && m_p_vertex_set)
    {
        throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    double x;
    double y;
    double z;

    switch (m_vertex_data_type)
        {
        case GL_FLOAT:
            {
            GLfloat* p_temp(reinterpret_cast<GLfloat*>(m_p_vertex_set));
            x = p_temp[anIndex * 3 + 0];
            y = p_temp[anIndex * 3 + 1];
            z = p_temp[anIndex * 3 + 2];
            }
            break;

        case GL_DOUBLE:
            {
            GLdouble* p_temp(reinterpret_cast<GLdouble*>(m_p_vertex_set));
            x = p_temp[anIndex * 3 + 0];
            y = p_temp[anIndex * 3 + 1];
            z = p_temp[anIndex * 3 + 2];
            }
            break;

        default:
            throw Exception(__FILE__, __FUNCTION__, __LINE__,
                    "Invalid data type.");
        }

    return (VEC3(x, y, z));
}


//-----------------------------------------------
void PolygonMesh::setVertex(unsigned int anIndex,
                            const VEC3& aVertex)
//-----------------------------------------------
{
    if (m_number_of_vertices <= anIndex && m_p_vertex_set)
        {
        throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
        }

    switch (m_vertex_data_type)
        {
        case GL_FLOAT:
            {
            GLfloat* p_temp(reinterpret_cast<GLfloat*>(m_p_vertex_set));
            p_temp[anIndex * 3 + 0] = aVertex.getX();
            p_temp[anIndex * 3 + 1] = aVertex.getY();
            p_temp[anIndex * 3 + 2] = aVertex.getZ();
            }
            break;

        case GL_DOUBLE:
            {
            GLdouble* p_temp(reinterpret_cast<GLdouble*>(m_p_vertex_set));
            p_temp[anIndex * 3 + 0] = aVertex.getX();
            p_temp[anIndex * 3 + 1] = aVertex.getY();
            p_temp[anIndex * 3 + 2] = aVertex.getZ();
            }
            break;

        default:
            throw Exception(__FILE__, __FUNCTION__, __LINE__,
                    "Invalid data type.");
        }
}


//---------------------------------------------------
int PolygonMesh::getIndex(unsigned int anIndex) const
//---------------------------------------------------
{
    if (m_number_of_indices <= anIndex && m_p_index_set)
        {
        throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
        }

    switch (m_index_data_type)
        {
        case GL_UNSIGNED_INT:
            {
            GLuint* p_temp(reinterpret_cast<GLuint*>(m_p_index_set));
            return (int(p_temp[anIndex]));
            }
            break;

        case GL_UNSIGNED_SHORT:
            {
            GLushort* p_temp(reinterpret_cast<GLushort*>(m_p_index_set));
            return (int(p_temp[anIndex]));
            }
            break;

        case GL_UNSIGNED_BYTE:
            {
            GLubyte* p_temp(reinterpret_cast<GLubyte*>(m_p_index_set));
            return (int(p_temp[anIndex]));
            }
            break;

        default:
            throw Exception(__FILE__, __FUNCTION__, __LINE__,
                    "Invalid data type.");
        }
}


//-------------------------------------------------------------------
void PolygonMesh::setIndex(unsigned int anIndex, unsigned int aValue)
//-------------------------------------------------------------------
{
    if (m_number_of_indices <= anIndex && m_p_index_set)
    {
        throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    if (m_p_index_set)
    {
        switch (m_index_data_type)
        {
        case GL_UNSIGNED_INT:
            {
                reinterpret_cast<GLuint*>(m_p_index_set)[anIndex] = aValue;
            }
            break;

        case GL_UNSIGNED_SHORT:
            {
                if (aValue > USHRT_MAX)
                {
                    throw Exception(__FILE__, __FUNCTION__, __LINE__,
                            "Overflow.");
                }

                reinterpret_cast<GLushort*>(m_p_index_set)[anIndex] = aValue;
            }
            break;

        case GL_UNSIGNED_BYTE:
            {
                if (aValue > UCHAR_MAX)
                {
                    throw Exception(__FILE__, __FUNCTION__, __LINE__,
                            "Overflow.");
                }

                reinterpret_cast<GLubyte*>(m_p_index_set)[anIndex] = aValue;
            }
            break;

        default:
            throw Exception(__FILE__, __FUNCTION__, __LINE__,
                    "Invalid data type.");
        }
    }
    else
    {
    throw Exception(__FILE__, __FUNCTION__, __LINE__, "The index is empty.");
    }
}


//-----------------------------------------------------------------------
void PolygonMesh::setExternalData(int aTypeOfPrimitive,
                                  const std::vector<float>* aVertexArray,
                                  bool aCreateVBOFlag,
                                  int aTypeOfVBO)
//-----------------------------------------------------------------------
{
    setExternalData(aTypeOfPrimitive,
            &((*aVertexArray)[0]),
            aVertexArray->size() / 3,
            GL_FLOAT,
            0,
            0,
            GL_NONE,
            aCreateVBOFlag,
            aTypeOfVBO);
}


//------------------------------------------------------------------------
void PolygonMesh::setExternalData(int aTypeOfPrimitive,
                                  const std::vector<double>* aVertexArray,
                                  bool aCreateVBOFlag,
                                  int aTypeOfVBO)
//------------------------------------------------------------------------
{
    setExternalData(aTypeOfPrimitive,
            &((*aVertexArray)[0]),
            aVertexArray->size() / 3,
            GL_DOUBLE,
            0,
            0,
            GL_NONE,
            aCreateVBOFlag,
            aTypeOfVBO);
}


//------------------------------------------------------------------------------
void PolygonMesh::setExternalData(int aTypeOfPrimitive,
                                  const std::vector<float>* aVertexArray,
                                  const std::vector<unsigned int>* anIndexArray,
                                  bool aCreateVBOFlag,
                                  int aTypeOfVBO)
//------------------------------------------------------------------------------
{
    setExternalData(aTypeOfPrimitive,
            &((*aVertexArray)[0]),
            aVertexArray->size() / 3,
            GL_FLOAT,
            &((*anIndexArray)[0]),
            anIndexArray->size(),
            GL_UNSIGNED_INT,
            aCreateVBOFlag,
            aTypeOfVBO);
}


//------------------------------------------------------------------------------
void PolygonMesh::setExternalData(int aTypeOfPrimitive,
                                  const std::vector<double>* aVertexArray,
                                  const std::vector<unsigned int>* anIndexArray,
                                  bool aCreateVBOFlag,
                                  int aTypeOfVBO)
//------------------------------------------------------------------------------
{
    setExternalData(aTypeOfPrimitive,
            &((*aVertexArray)[0]),
            aVertexArray->size() / 3,
            GL_DOUBLE,
            &((*anIndexArray)[0]),
            anIndexArray->size(),
            GL_UNSIGNED_INT,
            aCreateVBOFlag,
            aTypeOfVBO);
}


//------------------------------------------------------
void PolygonMesh::setExternalData(
        int aTypeOfPrimitive,
        const std::vector<float>* aVertexArray,
        const std::vector<unsigned short>* anIndexArray,
        bool aCreateVBOFlag,
        int aTypeOfVBO)
//------------------------------------------------------
{
    setExternalData(aTypeOfPrimitive,
            &((*aVertexArray)[0]),
            aVertexArray->size() / 3,
            GL_FLOAT,
            &((*anIndexArray)[0]),
            anIndexArray->size(),
            GL_UNSIGNED_SHORT,
            aCreateVBOFlag,
            aTypeOfVBO);
}


//------------------------------------------------------
void PolygonMesh::setExternalData(
        int aTypeOfPrimitive,
        const std::vector<double>* aVertexArray,
        const std::vector<unsigned short>* anIndexArray,
        bool aCreateVBOFlag,
        int aTypeOfVBO)
//------------------------------------------------------
{
    setExternalData(aTypeOfPrimitive,
            &((*aVertexArray)[0]),
            aVertexArray->size() / 3,
            GL_DOUBLE,
            &((*anIndexArray)[0]),
            anIndexArray->size(),
            GL_UNSIGNED_SHORT,
            aCreateVBOFlag,
            aTypeOfVBO);
}


//-----------------------------------------------------
void PolygonMesh::setExternalData(
        int aTypeOfPrimitive,
        const std::vector<float>* aVertexArray,
        const std::vector<unsigned char>* anIndexArray,
        bool aCreateVBOFlag,
        int aTypeOfVBO)
//-----------------------------------------------------
{
    setExternalData(aTypeOfPrimitive,
            &((*aVertexArray)[0]),
            aVertexArray->size() / 3,
            GL_FLOAT,
            &((*anIndexArray)[0]),
            anIndexArray->size(),
            GL_UNSIGNED_BYTE,
            aCreateVBOFlag,
            aTypeOfVBO);
}


//-----------------------------------------------------
void PolygonMesh::setExternalData(
        int aTypeOfPrimitive,
        const std::vector<double>* aVertexArray,
        const std::vector<unsigned char>* anIndexArray,
        bool aCreateVBOFlag,
        int aTypeOfVBO)
//-----------------------------------------------------
{
    setExternalData(aTypeOfPrimitive,
            &((*aVertexArray)[0]),
            aVertexArray->size() / 3,
            GL_DOUBLE,
            &((*anIndexArray)[0]),
            anIndexArray->size(),
            GL_UNSIGNED_BYTE,
            aCreateVBOFlag,
            aTypeOfVBO);
}


//---------------------------------------------------------------
void PolygonMesh::setExternalData(int aTypeOfPrimitive,
                                  const void* aVertexArray,
                                  unsigned int aNumberOfVertices,
                                  int aVertexDataType,
                                  const void* anIndexArray,
                                  unsigned int aNumberOfIndices,
                                  int anIndexDataType,
                                  bool aCreateVBOFlag,
                                  int aTypeOfVBO)
//---------------------------------------------------------------
{
    reset();

    m_external_data_flag = true;
    m_polygon_type       = aTypeOfPrimitive;
    m_p_vertex_set       = const_cast<void*>(aVertexArray);
    m_number_of_vertices = aNumberOfVertices;
    m_vertex_data_type   = aVertexDataType;

    // There are index data
    if (anIndexArray && anIndexDataType != GL_NONE && aNumberOfIndices != 0)
        {
        m_p_index_set       = const_cast<void*>(anIndexArray);
        m_number_of_indices = aNumberOfIndices;
        m_index_data_type   = anIndexDataType;
        }
    else
        {
        m_p_index_set       = 0;
        m_number_of_indices = 0;
        m_index_data_type   = GL_NONE;
        }

    // Compute the bounding box
    computeBoundingBox();

    // Compute the normal vectors
    computeNormalVectors();

    // Update the VBO
    if (aCreateVBOFlag)
        {
        updateVBO(aTypeOfVBO, m_polygon_type);
        }
}

//---------------------------------------------------------------
void PolygonMesh::setVBOData(
    const void* aVertexArray,
    unsigned int aNumberOfVertices,
    unsigned int aVertexVBO,
    unsigned int aNormalVBO,
    unsigned int aTextureVBO,
    unsigned int anIndexVBO,
    unsigned int aNumberOfIndices,
    int anIndexDataType,
    const void* aIndexArray,
    const void* aNormalArray
    )
    //---------------------------------------------------------------
{
    reset();

    m_external_data_flag = true;

    m_p_vertex_set = const_cast<void*>(aVertexArray);


    m_number_of_vertices = aNumberOfVertices;
    m_vertex_data_type = GL_FLOAT;
    m_polygon_type = GL_TRIANGLES;

    // Compute the normal vectors
    if (aNormalVBO == GL_INVALID_INDEX || aNormalArray == NULL)
        computeNormalVectors();
    else
    {
        m_p_vertex_normal_set = std::vector< RATIONAL_NUMBER>((float*)aNormalArray, (float*)aNormalArray + (aNumberOfVertices*3));

    }



    // There are index data
    if (anIndexVBO != GL_INVALID_INDEX && anIndexDataType != GL_NONE && aNumberOfIndices != 0)
    {
        m_p_index_set = const_cast<void*>( aIndexArray );
        m_number_of_indices = aNumberOfIndices;
        m_index_data_type = anIndexDataType;
    }
    else
    {
        m_p_index_set = 0;
        m_number_of_indices = 0;
        m_index_data_type = GL_NONE;
    }

    // Compute the bounding box
    computeBoundingBox();



    // Update the VBO

    updateVBOs(aVertexVBO, aNormalVBO, aTextureVBO, anIndexVBO);
    /*if (aCreateVBOFlag)
    {
    updateVBO(aTypeOfVBO, m_polygon_type);
    }*/
}



//-----------------------------------------------------------------------
void PolygonMesh::setInternalData(int aTypeOfPrimitive,
        const std::vector<float>* aVertexArray,
        const std::vector<float>* aNormalArray,
                                  bool aCreateVBOFlag,
                                  int aTypeOfVBO)
//-----------------------------------------------------------------------
{
    setInternalData(aTypeOfPrimitive,
            &((*aVertexArray)[0]),
            aVertexArray->size() / 3,
            GL_FLOAT,
            0,
            0,
            GL_NONE,
            aCreateVBOFlag,
            aTypeOfVBO);

    m_p_vertex_normal_set.clear();
    //m_p_face_normal_set.clear();

    if (aVertexArray->size() == aNormalArray->size())
    {
        m_p_vertex_normal_set = *aNormalArray;
    }
    /*else if (aNormalArray->size() == anIndexArray->size())
    {
        m_p_face_normal_set = *aNormalArray;
    }*/
    else
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid number of normal vectors.");
    }

    // Update the VBO
    if (aCreateVBOFlag)
        {
        updateVBO(aTypeOfVBO, aTypeOfPrimitive);
        }
}


//-----------------------------------------------------------------------
void PolygonMesh::setInternalData(int aTypeOfPrimitive,
                                  const std::vector<float>* aVertexArray,
                                  bool aCreateVBOFlag,
                                  int aTypeOfVBO)
//-----------------------------------------------------------------------
{
    setInternalData(aTypeOfPrimitive,
            &((*aVertexArray)[0]),
            aVertexArray->size() / 3,
            GL_FLOAT,
            0,
            0,
            GL_NONE,
            aCreateVBOFlag,
            aTypeOfVBO);
}


//-----------------------------------------------------------------------
void PolygonMesh::setInternalData(int aTypeOfPrimitive,
        const std::vector<double>* aVertexArray,
        const std::vector<double>* aNormalArray,
                                  bool aCreateVBOFlag,
                                  int aTypeOfVBO)
//-----------------------------------------------------------------------
{
    setInternalData(aTypeOfPrimitive,
            &((*aVertexArray)[0]),
            aVertexArray->size() / 3,
            GL_DOUBLE,
            0,
            0,
            GL_NONE,
            aCreateVBOFlag,
            aTypeOfVBO);

    m_p_vertex_normal_set.clear();
    //m_p_face_normal_set.clear();

    if (aVertexArray->size() == aNormalArray->size())
    {
        for (std::vector<double>::const_iterator ite(aNormalArray->begin());
                ite != aNormalArray->end();
                ++ite)
        {
            m_p_vertex_normal_set.push_back(*ite);
        }
    }
    /*else if (aNormalArray->size() == anIndexArray->size())
    {
        for (std::vector<double>::const_iterator ite(aNormalArray->begin());
                ite != aNormalArray->end();
                ++ite)
        {
            m_p_face_normal_set.push_back(*ite);
        }
    }*/
    else
    {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid number of normal vectors.");
    }

    // Update the VBO
    if (aCreateVBOFlag)
        {
        updateVBO(aTypeOfVBO, aTypeOfPrimitive);
        }
}


//------------------------------------------------------------------------
void PolygonMesh::setInternalData(int aTypeOfPrimitive,
                                  const std::vector<double>* aVertexArray,
                                  bool aCreateVBOFlag,
                                  int aTypeOfVBO)
//------------------------------------------------------------------------
{
    setInternalData(aTypeOfPrimitive,
            &((*aVertexArray)[0]),
            aVertexArray->size() / 3,
            GL_DOUBLE,
            0,
            0,
            GL_NONE,
            aCreateVBOFlag,
            aTypeOfVBO);
}


//------------------------------------------------------------------------------
void PolygonMesh::setInternalData(int aTypeOfPrimitive,
                                  const std::vector<float>* aVertexArray,
                                  const std::vector<unsigned int>* anIndexArray,
                                  bool aCreateVBOFlag,
                                  int aTypeOfVBO)
//------------------------------------------------------------------------------
{
    setInternalData(aTypeOfPrimitive,
            &((*aVertexArray)[0]),
            aVertexArray->size() / 3,
            GL_FLOAT,
            &((*anIndexArray)[0]),
            anIndexArray->size(),
            GL_UNSIGNED_INT,
            aCreateVBOFlag,
            aTypeOfVBO);
}


//------------------------------------------------------------------------------
void PolygonMesh::setInternalData(int aTypeOfPrimitive,
                                  const std::vector<float>* aVertexArray,
                                  const std::vector<float>* aNormalArray,
                                  const std::vector<unsigned int>* anIndexArray,
                                  bool aCreateVBOFlag,
                                  int aTypeOfVBO)
//------------------------------------------------------------------------------
{
    setInternalData(aTypeOfPrimitive,
            &((*aVertexArray)[0]),
            aVertexArray->size() / 3,
            GL_FLOAT,
            &((*anIndexArray)[0]),
            anIndexArray->size(),
            GL_UNSIGNED_INT,
            aCreateVBOFlag,
            aTypeOfVBO);

    if (aNormalArray)
    {
        //m_p_face_normal_set.clear();
        m_p_vertex_normal_set.clear();

        if (aNormalArray->size() == aVertexArray->size())
        {
            m_p_vertex_normal_set = *aNormalArray;
        }
        /*else if (aNormalArray->size() == anIndexArray->size())
        {
            m_p_face_normal_set = *aNormalArray;
        }*/
        else
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid number of normal vectors.");
        }
    }

    // Update the VBO
    if (aCreateVBOFlag)
        {
        updateVBO(aTypeOfVBO, aTypeOfPrimitive);
        }
}


//------------------------------------------------------------------------------
void PolygonMesh::setInternalData(int aTypeOfPrimitive,
                                  const std::vector<double>* aVertexArray,
                                  const std::vector<unsigned int>* anIndexArray,
                                  bool aCreateVBOFlag,
                                  int aTypeOfVBO)
//------------------------------------------------------------------------------
{
    setInternalData(aTypeOfPrimitive,
            &((*aVertexArray)[0]),
            aVertexArray->size() / 3,
            GL_DOUBLE,
            &((*anIndexArray)[0]),
            anIndexArray->size(),
            GL_UNSIGNED_INT,
            aCreateVBOFlag,
            aTypeOfVBO);
}


//------------------------------------------------------------------------------
void PolygonMesh::setInternalData(int aTypeOfPrimitive,
                                  const std::vector<double>* aVertexArray,
                                  const std::vector<float>* aNormalArray,
                                  const std::vector<unsigned int>* anIndexArray,
                                  bool aCreateVBOFlag,
                                  int aTypeOfVBO)
//------------------------------------------------------------------------------
{
    setInternalData(aTypeOfPrimitive,
            &((*aVertexArray)[0]),
            aVertexArray->size() / 3,
            GL_DOUBLE,
            &((*anIndexArray)[0]),
            anIndexArray->size(),
            GL_UNSIGNED_INT,
            aCreateVBOFlag,
            aTypeOfVBO);

    if (aNormalArray)
    {
        ////m_p_face_normal_set.clear();
        m_p_vertex_normal_set.clear();

        if (aNormalArray->size() == aVertexArray->size())
        {
            m_p_vertex_normal_set = *aNormalArray;
        }
        /*else if (aNormalArray->size() == anIndexArray->size())
        {
            m_p_face_normal_set = *aNormalArray;
        }*/
        else
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid number of normal vectors.");
        }
    }

    // Update the VBO
    if (aCreateVBOFlag)
        {
        updateVBO(aTypeOfVBO, aTypeOfPrimitive);
        }
}


//------------------------------------------------------
void PolygonMesh::setInternalData(
        int aTypeOfPrimitive,
        const std::vector<float>* aVertexArray,
        const std::vector<unsigned short>* anIndexArray,
        bool aCreateVBOFlag,
        int aTypeOfVBO)
//------------------------------------------------------
{
    setInternalData(aTypeOfPrimitive,
            &((*aVertexArray)[0]),
            aVertexArray->size() / 3,
            GL_FLOAT,
            &((*anIndexArray)[0]),
            anIndexArray->size(),
            GL_UNSIGNED_SHORT,
            aCreateVBOFlag,
            aTypeOfVBO);
}


//------------------------------------------------------------------------------
void PolygonMesh::setInternalData(int aTypeOfPrimitive,
                                  const std::vector<float>* aVertexArray,
                                  const std::vector<float>* aNormalArray,
                                  const std::vector<unsigned short>* anIndexArray,
                                  bool aCreateVBOFlag,
                                  int aTypeOfVBO)
//------------------------------------------------------------------------------
{
    setInternalData(aTypeOfPrimitive,
            &((*aVertexArray)[0]),
            aVertexArray->size() / 3,
            GL_FLOAT,
            &((*anIndexArray)[0]),
            anIndexArray->size(),
            GL_UNSIGNED_SHORT,
            aCreateVBOFlag,
            aTypeOfVBO);

    if (aNormalArray)
    {
        //m_p_face_normal_set.clear();
        m_p_vertex_normal_set.clear();

        if (aNormalArray->size() == aVertexArray->size())
        {
            m_p_vertex_normal_set = *aNormalArray;
        }
/*        else if (aNormalArray->size() == anIndexArray->size())
        {
            m_p_face_normal_set = *aNormalArray;
        }
 */       else
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid number of normal vectors.");
        }
    }

    // Update the VBO
    if (aCreateVBOFlag)
        {
        updateVBO(aTypeOfVBO, aTypeOfPrimitive);
        }
}


//------------------------------------------------------
void PolygonMesh::setInternalData(
        int aTypeOfPrimitive,
        const std::vector<double>* aVertexArray,
        const std::vector<unsigned short>* anIndexArray,
        bool aCreateVBOFlag,
        int aTypeOfVBO)
//------------------------------------------------------
{
    setInternalData(aTypeOfPrimitive,
            &((*aVertexArray)[0]),
            aVertexArray->size() / 3,
            GL_DOUBLE,
            &((*anIndexArray)[0]),
            anIndexArray->size(),
            GL_UNSIGNED_SHORT,
            aCreateVBOFlag,
            aTypeOfVBO);
}


//------------------------------------------------------------------------------
void PolygonMesh::setInternalData(int aTypeOfPrimitive,
                                  const std::vector<double>* aVertexArray,
                                  const std::vector<float>* aNormalArray,
                                  const std::vector<unsigned short>* anIndexArray,
                                  bool aCreateVBOFlag,
                                  int aTypeOfVBO)
//------------------------------------------------------------------------------
{
    setInternalData(aTypeOfPrimitive,
            &((*aVertexArray)[0]),
            aVertexArray->size() / 3,
            GL_DOUBLE,
            &((*anIndexArray)[0]),
            anIndexArray->size(),
            GL_UNSIGNED_SHORT,
            aCreateVBOFlag,
            aTypeOfVBO);

    if (aNormalArray)
    {
        //m_p_face_normal_set.clear();
        m_p_vertex_normal_set.clear();

        if (aNormalArray->size() == aVertexArray->size())
        {
            m_p_vertex_normal_set = *aNormalArray;
        }
        /*else if (aNormalArray->size() == anIndexArray->size())
        {
            m_p_face_normal_set = *aNormalArray;
        }*/
        else
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid number of normal vectors.");
        }
    }

    // Update the VBO
    if (aCreateVBOFlag)
        {
        updateVBO(aTypeOfVBO, aTypeOfPrimitive);
        }
}


//-----------------------------------------------------
void PolygonMesh::setInternalData(
        int aTypeOfPrimitive,
        const std::vector<float>* aVertexArray,
        const std::vector<unsigned char>* anIndexArray,
        bool aCreateVBOFlag,
        int aTypeOfVBO)
//-----------------------------------------------------
{
    setInternalData(aTypeOfPrimitive,
            &((*aVertexArray)[0]),
            aVertexArray->size() / 3,
            GL_FLOAT,
            &((*anIndexArray)[0]),
            anIndexArray->size(),
            GL_UNSIGNED_BYTE,
            aCreateVBOFlag,
            aTypeOfVBO);
}


//------------------------------------------------------------------------------
void PolygonMesh::setInternalData(int aTypeOfPrimitive,
                                  const std::vector<float>* aVertexArray,
                                  const std::vector<float>* aNormalArray,
                                  const std::vector<unsigned char>* anIndexArray,
                                  bool aCreateVBOFlag,
                                  int aTypeOfVBO)
//------------------------------------------------------------------------------
{
    setInternalData(aTypeOfPrimitive,
            &((*aVertexArray)[0]),
            aVertexArray->size() / 3,
            GL_FLOAT,
            &((*anIndexArray)[0]),
            anIndexArray->size(),
            GL_UNSIGNED_BYTE,
            aCreateVBOFlag,
            aTypeOfVBO);

    if (aNormalArray)
    {
        //m_p_face_normal_set.clear();
        m_p_vertex_normal_set.clear();

        if (aNormalArray->size() == aVertexArray->size())
        {
            m_p_vertex_normal_set = *aNormalArray;
        }
        /*else if (aNormalArray->size() == anIndexArray->size())
        {
            m_p_face_normal_set = *aNormalArray;
        }*/
        else
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid number of normal vectors.");
        }
    }

    // Update the VBO
    if (aCreateVBOFlag)
        {
        updateVBO(aTypeOfVBO, aTypeOfPrimitive);
        }
}


//-----------------------------------------------------
void PolygonMesh::setInternalData(
        int aTypeOfPrimitive,
        const std::vector<double>* aVertexArray,
        const std::vector<unsigned char>* anIndexArray,
        bool aCreateVBOFlag,
        int aTypeOfVBO)
//-----------------------------------------------------
{
    setInternalData(aTypeOfPrimitive,
            &((*aVertexArray)[0]),
            aVertexArray->size() / 3,
            GL_DOUBLE,
            &((*anIndexArray)[0]),
            anIndexArray->size(),
            GL_UNSIGNED_BYTE,
            aCreateVBOFlag,
            aTypeOfVBO);
}


//------------------------------------------------------------------------------
void PolygonMesh::setInternalData(int aTypeOfPrimitive,
                                  const std::vector<double>* aVertexArray,
                                  const std::vector<float>* aNormalArray,
                                  const std::vector<unsigned char>* anIndexArray,
                                  bool aCreateVBOFlag,
                                  int aTypeOfVBO)
//------------------------------------------------------------------------------
{
    setInternalData(aTypeOfPrimitive,
            &((*aVertexArray)[0]),
            aVertexArray->size() / 3,
            GL_DOUBLE,
            &((*anIndexArray)[0]),
            anIndexArray->size(),
            GL_UNSIGNED_BYTE,
            aCreateVBOFlag,
            aTypeOfVBO);

    if (aNormalArray)
    {
        //m_p_face_normal_set.clear();
        m_p_vertex_normal_set.clear();

        if (aNormalArray->size() == aVertexArray->size())
        {
            m_p_vertex_normal_set = *aNormalArray;
        }
        /*else if (aNormalArray->size() == anIndexArray->size())
        {
            m_p_face_normal_set = *aNormalArray;
        }*/
        else
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid number of normal vectors.");
        }
    }

    // Update the VBO
    if (aCreateVBOFlag)
        {
        updateVBO(aTypeOfVBO, aTypeOfPrimitive);
        }
}


//---------------------------------------------------------------
void PolygonMesh::setInternalData(int aTypeOfPrimitive,
                                  const void* aVertexArray,
                                  unsigned int aNumberOfVertices,
                                  int aVertexDataType,
                                  const void* anIndexArray,
                                  unsigned int aNumberOfIndices,
                                  int anIndexDataType,
                                  bool aCreateVBOFlag,
                                  int aTypeOfVBO)
//---------------------------------------------------------------
{
    reset();

    m_external_data_flag = false;
    m_polygon_type       = aTypeOfPrimitive;
    m_number_of_vertices = aNumberOfVertices;
    m_vertex_data_type   = aVertexDataType;
    copyVertexSet(aVertexArray);

    // There are index data
    if (anIndexArray && anIndexDataType != GL_NONE && aNumberOfIndices != 0)
        {
        m_number_of_indices = aNumberOfIndices;
        m_index_data_type   = anIndexDataType;
        copyIndexSet(anIndexArray);
        }
    else
        {
        m_p_index_set       = 0;
        m_number_of_indices = 0;
        m_index_data_type   = GL_NONE;
        }

    // Compute the bounding box
    computeBoundingBox();

    // Compute the normal vectors
    computeNormalVectors();

    // Update the VBO
    if (aCreateVBOFlag)
        {
        updateVBO(aTypeOfVBO, m_polygon_type);
        }
}


//------------------------------------------------------------------------------
void PolygonMesh::setInternalData(int aTypeOfPrimitive,
                                  const std::vector<double>* aVertexArray,
                                  const std::vector<double>* aNormalArray,
                                  const std::vector<unsigned char>* anIndexArray,
                                  bool aCreateVBOFlag,
                                  int aTypeOfVBO)
//------------------------------------------------------------------------------
{
    setInternalData(aTypeOfPrimitive,
                    &((*aVertexArray)[0]),
                    aVertexArray->size() / 3,
                    GL_DOUBLE,
                    &((*anIndexArray)[0]),
                    anIndexArray->size(),
                    GL_UNSIGNED_BYTE,
                    aCreateVBOFlag,
                    aTypeOfVBO);


    m_p_vertex_normal_set.clear();
    //m_p_face_normal_set.clear();

    if (aNormalArray)
    {
        if (aNormalArray->size() == aVertexArray->size())
        {
            for (std::vector<double>::const_iterator ite(aNormalArray->begin());
                 ite != aNormalArray->end();
                 ++ite)
            {
                m_p_vertex_normal_set.push_back(*ite);
            }
        }
        /*else if (aNormalArray->size() == anIndexArray->size())
        {
            for (std::vector<double>::const_iterator ite(aNormalArray->begin());
                 ite != aNormalArray->end();
                 ++ite)
            {
                m_p_face_normal_set.push_back(*ite);
            }
        }*/
        else
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid number of normal vectors.");
        }
    }

    // Update the VBO
    if (aCreateVBOFlag)
    {
        updateVBO(aTypeOfVBO, aTypeOfPrimitive);
    }
}


//------------------------------------------------------------------------------
void PolygonMesh::setInternalData(int aTypeOfPrimitive,
                                  const std::vector<double>* aVertexArray,
                                  const std::vector<double>* aNormalArray,
                                  const std::vector<unsigned short>* anIndexArray,
                                  bool aCreateVBOFlag,
                                  int aTypeOfVBO)
//------------------------------------------------------------------------------
{
    setInternalData(aTypeOfPrimitive,
                    &((*aVertexArray)[0]),
                    aVertexArray->size() / 3,
                    GL_DOUBLE,
                    &((*anIndexArray)[0]),
                    anIndexArray->size(),
                    GL_UNSIGNED_SHORT,
                    aCreateVBOFlag,
                    aTypeOfVBO);


    m_p_vertex_normal_set.clear();
    //m_p_face_normal_set.clear();

    if (aNormalArray)
    {
        if (aNormalArray->size() == aVertexArray->size())
        {
            for (std::vector<double>::const_iterator ite(aNormalArray->begin());
                 ite != aNormalArray->end();
                 ++ite)
            {
                m_p_vertex_normal_set.push_back(*ite);
            }
        }
        /*else if (aNormalArray->size() == anIndexArray->size())
        {
            for (std::vector<double>::const_iterator ite(aNormalArray->begin());
                 ite != aNormalArray->end();
                 ++ite)
            {
                m_p_face_normal_set.push_back(*ite);
            }
        }*/
        else
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid number of normal vectors.");
        }
    }

    // Update the VBO
    if (aCreateVBOFlag)
    {
        updateVBO(aTypeOfVBO, aTypeOfPrimitive);
    }
}


//------------------------------------------------------------------------------
void PolygonMesh::setInternalData(int aTypeOfPrimitive,
                                  const std::vector<double>* aVertexArray,
                                  const std::vector<double>* aNormalArray,
                                  const std::vector<unsigned int>* anIndexArray,
                                  bool aCreateVBOFlag,
                                  int aTypeOfVBO)
//------------------------------------------------------------------------------
{
    setInternalData(aTypeOfPrimitive,
                    &((*aVertexArray)[0]),
                    aVertexArray->size() / 3,
                    GL_DOUBLE,
                    &((*anIndexArray)[0]),
                    anIndexArray->size(),
                    GL_UNSIGNED_INT,
                    aCreateVBOFlag,
                    aTypeOfVBO);

    m_p_vertex_normal_set.clear();
    //m_p_face_normal_set.clear();

    if (aNormalArray)
    {
        if (aNormalArray->size() == aVertexArray->size())
        {
            for (std::vector<double>::const_iterator ite(aNormalArray->begin());
                 ite != aNormalArray->end();
                 ++ite)
            {
                m_p_vertex_normal_set.push_back(*ite);
            }
        }
        /*else if (aNormalArray->size() == anIndexArray->size())
        {
            for (std::vector<double>::const_iterator ite(aNormalArray->begin());
                 ite != aNormalArray->end();
                 ++ite)
            {
                m_p_face_normal_set.push_back(*ite);
            }
        }*/
        else
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, "Invalid number of normal vectors.");
        }
    }

    // Update the VBO
    if (aCreateVBOFlag)
    {
        updateVBO(aTypeOfVBO, aTypeOfPrimitive);
    }
}


//-----------------------------------
void PolygonMesh::destroyVertexData()
//-----------------------------------
{
    // The data is manage externally
    if (m_external_data_flag)
        {
        m_p_vertex_set = 0;
        }
    // The data is manage internally
    else
        {
        if (m_p_vertex_set)
            {
            // Cannot release the memory of an array whose type is void*
            switch (m_vertex_data_type)
                {
                case GL_FLOAT:
                    delete [] reinterpret_cast<GLfloat*>(m_p_vertex_set);
                    break;

                case GL_DOUBLE:
                    delete [] reinterpret_cast<GLdouble*>(m_p_vertex_set);
                    break;

                default:
                    throw Exception(__FILE__, __FUNCTION__, __LINE__,
                            "Invalid data type.");
                }

            m_vertex_data_type = 0;
            m_p_vertex_set = 0;
            }
        }

    m_number_of_vertices = 0;
}


//----------------------------------
void PolygonMesh::destroyIndexData()
//----------------------------------
{
    // The data is manage externally
    if (m_external_data_flag)
        {
        m_p_index_set  = 0;
        }
    // The data is manage internally
    else
        {
        if (m_p_index_set)
            {
            // Cannot release the memory of an array whose type is void*
            switch (m_index_data_type)
                {
                case GL_UNSIGNED_INT:
                    delete [] reinterpret_cast<GLuint*>(m_p_index_set);
                    break;

                case GL_UNSIGNED_SHORT:
                    delete [] reinterpret_cast<GLushort*>(m_p_index_set);
                    break;

                case GL_UNSIGNED_BYTE:
                    delete [] reinterpret_cast<GLubyte*>(m_p_index_set);
                    break;

                default:
                    throw Exception(__FILE__, __FUNCTION__, __LINE__,
                            "Invalid data type.");
                }

            m_index_data_type =0;
            m_p_index_set = 0;
            }
        }
    m_number_of_indices = 0;
}


//-----------------------------------------------------
void PolygonMesh::removeIndex(bool aPrintDebugInfoFlag)
//-----------------------------------------------------
{
    // Split faces only if external data is not used
    if (!m_external_data_flag)
        {
        // It only works if an index has been set
        if (m_p_index_set && m_number_of_indices)
            {
            // The polygon is made of triangles
            if (m_polygon_type == GL_TRIANGLES)
                {
                // Reset the normal per face set
                m_p_vertex_normal_set.clear();

                // Reset the normal per face set
                //m_p_face_normal_set.clear();

                // Store the new vertices
                std::vector<double> p_new_vertex_set;

                // Process every triangle
                unsigned int triangle_number(getTriangleNumber());
                for (unsigned int i(0); i < triangle_number; ++i)
                    {
                    // Get the current vertices
                    int id_0(getIndex(i * 3 + 0));
                    int id_1(getIndex(i * 3 + 1));
                    int id_2(getIndex(i * 3 + 2));

                    VEC3 vertex_0(getVertex(id_0));
                    VEC3 vertex_1(getVertex(id_1));
                    VEC3 vertex_2(getVertex(id_2));

                    // Compute the normal vector
                    VEC3 normal((vertex_2 - vertex_0)^(vertex_2 - vertex_1));
                    normal.normalize();

                    // Add the first triangle
                    p_new_vertex_set.push_back(vertex_0.getX());
                    p_new_vertex_set.push_back(vertex_0.getY());
                    p_new_vertex_set.push_back(vertex_0.getZ());
                    p_new_vertex_set.push_back(vertex_1.getX());
                    p_new_vertex_set.push_back(vertex_1.getY());
                    p_new_vertex_set.push_back(vertex_1.getZ());
                    p_new_vertex_set.push_back(vertex_2.getX());
                    p_new_vertex_set.push_back(vertex_2.getY());
                    p_new_vertex_set.push_back(vertex_2.getZ());

                    for (unsigned int j(0); j < 3; ++j)
                        {
                        m_p_vertex_normal_set.push_back(normal.getX());
                        m_p_vertex_normal_set.push_back(normal.getY());
                        m_p_vertex_normal_set.push_back(normal.getZ());
                        }
                    }

                // Set the index in the hash table and
                // create the list of vertices
                destroyIndexData();
                destroyVertexData();
                m_number_of_vertices = p_new_vertex_set.size() / 3;

                GLfloat* p_float_vertex_set(0);
                GLdouble* p_double_vertex_set(0);
                switch (m_vertex_data_type)
                    {
                    case GL_FLOAT:
                        m_p_vertex_set = new GLfloat[m_number_of_vertices * 3];
                        p_float_vertex_set =
                                reinterpret_cast<GLfloat*>(m_p_vertex_set);
                        break;

                    case GL_DOUBLE:
                        m_p_vertex_set = new GLdouble[m_number_of_vertices * 3];
                        p_double_vertex_set =
                                reinterpret_cast<GLdouble*>(m_p_vertex_set);
                        break;

                    default:
                        throw Exception(__FILE__, __FUNCTION__, __LINE__,
                                "Invalid data type.");
                    }

                // Save the result
                for (unsigned int id(0); id < p_new_vertex_set.size(); ++id)
                    {
                    if (p_float_vertex_set)
                        {
                        p_float_vertex_set[id] = p_new_vertex_set[id];
                        }
                    if (p_double_vertex_set)
                        {
                        p_double_vertex_set[id] = p_new_vertex_set[id];
                        }
                    }

                // Create the VBO if needed
                if (!m_p_vbo.get())
                    {
                    m_p_vbo.reset(createVBO());
                    }

                // Update the VBO
                if (m_p_vbo->isReady())
                    {
                    updateVBO(m_p_vbo->getBufferUsageHint(),
                            m_p_vbo->getPrimitiveType());
                    }

                // Display extra debug info
                if (aPrintDebugInfoFlag)
                    {
                    LOGGER.logNow("file_name:\t") << m_filename <<
                            "\tnb_faces:\t" << getFaceNumber() <<
                            "\tnb_vertices:\t" << m_number_of_vertices <<
                            "\tbounding_box (in cm):\t(" <<
                            m_local_min_corner.getX() / cm << ", " <<
                            m_local_min_corner.getY() / cm << ", " <<
                            m_local_min_corner.getZ() / cm << ")\t(" <<
                            m_local_max_corner.getX() / cm << ", " <<
                            m_local_max_corner.getY() / cm << ", " <<
                            m_local_max_corner.getZ() / cm << ")" << std::endl;
                    }
                }
            // Not supported
            else
                {
                throw NotImplementedException(__FILE__, __FUNCTION__, __LINE__);
                }
            }
        }
    // Generate an error
    else
        {
        throw Exception(__FILE__, __FUNCTION__, __LINE__,
                "The operation cannot be performed on external data.");
        }
}


//------------------------------------------------------
void PolygonMesh::copyVertexSet(const void* apVertexSet)
//------------------------------------------------------
{
    // Store the number of vertices
    unsigned int temp(m_number_of_vertices);

    // Destroy data if needed
    destroyVertexData();

    // Restore the number of vertices
    m_number_of_vertices = temp;

    // The data is manage externally
    if (m_external_data_flag)
        {
        m_p_vertex_set = const_cast<void*>(apVertexSet);
        }
    // The data is not manage externally
    else
        {
        unsigned int size_to_copy(m_number_of_vertices * 3);

        switch (m_vertex_data_type)
            {
            case GL_FLOAT:
                m_p_vertex_set = new GLfloat[size_to_copy];
                size_to_copy *= sizeof(GLfloat);
                break;

            case GL_DOUBLE:
                m_p_vertex_set = new GLdouble[size_to_copy];
                size_to_copy *= sizeof(GLdouble);
                break;

            default:
                throw Exception(__FILE__, __FUNCTION__, __LINE__,
                        "Invalid data type.");
            }

        // Copy data if needed
        if (size_to_copy)
            {
            memcpy (m_p_vertex_set, apVertexSet, size_to_copy);
            }
        }
}


//----------------------------------------------------
void PolygonMesh::copyIndexSet(const void* apIndexSet)
//----------------------------------------------------
{
    // Store the number of indices
    unsigned int temp(m_number_of_indices);

    // Destroy data if needed
    destroyIndexData();

    // Restore the number of indices
    m_number_of_indices = temp;

    // The data is manage externally
    if (m_external_data_flag)
        {
        m_p_index_set  = const_cast<void*>(apIndexSet);
        }
    // The data is not manage externally
    else
        {
        unsigned int size_to_copy(m_number_of_indices);

        switch (m_polygon_type)
            {
            case GL_TRIANGLES:
                break;

            case GL_LINES:
                break;

            default:
                throw Exception(__FILE__, __FUNCTION__, __LINE__,
                        "Invalid data type.");
            }

        switch (m_index_data_type)
            {
            case 0:
                break;

            case GL_UNSIGNED_INT:
                m_p_index_set = new GLuint[size_to_copy];
                size_to_copy *= sizeof(GLuint);
                break;

            case GL_UNSIGNED_SHORT:
                m_p_index_set = new GLushort[size_to_copy];
                size_to_copy *= sizeof(GLushort);
                break;

            case GL_UNSIGNED_BYTE:
                m_p_index_set = new GLubyte[size_to_copy];
                size_to_copy *= sizeof(GLubyte);
                break;

            default:
                throw Exception(__FILE__, __FUNCTION__, __LINE__,
                        "Invalid data type.");
            }

        // Copy data if needed
        if (size_to_copy)
            {
            memcpy (m_p_index_set, apIndexSet, size_to_copy);
            }
        }
}
