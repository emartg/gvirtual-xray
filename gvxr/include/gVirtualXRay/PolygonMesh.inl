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
*   @file       PolygonMesh.inl
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
#ifndef GVXR_OUT_OF_BOUNDS_EXCEPTION_H
#include "gVirtualXRay/OutOfBoundsException.h"
#endif

#ifndef GVXR_UTILITIES_H
#include "gVirtualXRay/Utilities.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//--------------------------------
inline PolygonMesh::~PolygonMesh()
//--------------------------------
{
    // Make sure the VBO and the dynamically allocated memory are released
    reset();
}


//----------------------------------
inline void PolygonMesh::resetVBOs()
//----------------------------------
{
    if (m_p_vbo.get())
    {
        m_p_vbo->destroy();
    }

    if (m_p_vertex_normal_vbo.get())
    {
        m_p_vertex_normal_vbo->reset();
    }

    if (m_p_face_normal_vbo.get())
    {
        m_p_face_normal_vbo->reset();
    }
}


//--------------------------------------------------------------------------
inline void PolygonMesh::setDisplayFlag(bool display)
//--------------------------------------------------------------------------
{
    //Change the value of the display flag
    m_display_flag = display;
}

//--------------------------------------------------------------------------
inline bool PolygonMesh::getDisplayFlag() const
//--------------------------------------------------------------------------
{
    // Return the display flag value
    return m_display_flag;
}

//---------------------------------------------------------
inline void PolygonMesh::setFilename(const char* aFilename)
//---------------------------------------------------------
{
    // Store the name of the file that contains the polygon mesh
    m_filename = aFilename;
}


//----------------------------------------------------------------
inline void PolygonMesh::setFilename(const std::string& aFilename)
//----------------------------------------------------------------
{
    // Store the name of the file that contains the polygon mesh
    m_filename = aFilename;
}


//---------------------------------------------------------
inline void PolygonMesh::setFileName(const char* aFilename)
//---------------------------------------------------------
{
    // Store the name of the file that contains the polygon mesh
    m_filename = aFilename;
}


//----------------------------------------------------------------
inline void PolygonMesh::setFileName(const std::string& aFilename)
//----------------------------------------------------------------
{
    // Store the name of the file that contains the polygon mesh
    m_filename = aFilename;
}


//--------------------------------------------------------
inline const std::string& PolygonMesh::getFilename() const
//--------------------------------------------------------
{
    // Return the name of the file that contains the polygon mesh
    return (m_filename);
}


//----------------------------------------------------------------
inline const RATIONAL_NUMBER& PolygonMesh::getUnitOfLength() const
//----------------------------------------------------------------
{
    return m_file_scale;
}


//-------------------------------------
inline void PolygonMesh::moveToCenter()
//-------------------------------------
{
    moveToCentre();
}


//-------------------------------------------------------------------
inline bool PolygonMesh::useVBO(int aTypeOfVBO, int aTypeOfPrimitive)
//-------------------------------------------------------------------
{
    // Create the VBO if needed
    if (!m_p_vbo.get())
        {
        m_p_vbo.reset(createVBO());
        }

    // There is no data to store in a VBO
    if (m_number_of_vertices && (getVertexNormalNumber() ||
            getFaceNormalNumber()))
        {
        // Create a VBO if possible
        updateVBO(aTypeOfVBO, aTypeOfPrimitive);
        }

    return (m_p_vbo->isReady());
}


inline void PolygonMesh::setHU(short HU)
{
    m_photon_cross_section.useHU(HU);
}


inline void PolygonMesh::setHounsfieldUnit(short HU)
{
    setHU(HU);
}


inline void PolygonMesh::setHounsfieldValue(short HU)
{
    setHU(HU);
}


inline void PolygonMesh::setElement(unsigned short Z)
{
    m_photon_cross_section.useElement(Z);
}


inline void PolygonMesh::setElement(const std::string& aName)
{
    m_photon_cross_section.useElement(aName);
}


inline void PolygonMesh::setMixture(const Mixture& aMixture)
{
    m_photon_cross_section.useMixture(aMixture);
}


inline void PolygonMesh::setMixture(const std::map<int, double>& aMixture)
{
    m_photon_cross_section.useMixture(aMixture);
}


inline void PolygonMesh::setMixture(const std::map<std::string, double>& aMixture)
{
    m_photon_cross_section.useMixture(aMixture);
}


inline void PolygonMesh::setMixture(const std::string& aName)
{
    m_photon_cross_section.useMixture(aName);
}


inline void PolygonMesh::setCompound(const std::string& aName)
{
    m_photon_cross_section.useCompound(aName);
}


inline void PolygonMesh::setMassAttenuationCoefficient(double aCoefficient)
{
    m_photon_cross_section.useMassAttenuationCoefficient(aCoefficient);
}


inline void PolygonMesh::setLinearAttenuationCoefficient(double aCoefficient)
{
    m_photon_cross_section.useLinearAttenuationCoefficient(aCoefficient);
}


inline void PolygonMesh::setDensity(double aDensity)
{
    m_photon_cross_section.setDensity(aDensity);
}


inline const PhotonCrossSection& PolygonMesh::getPhotonCrossSection() const
{
    return (m_photon_cross_section);
}


inline void PolygonMesh::setPhotonCrossSection(const PhotonCrossSection& aPhotonCrossSection)
{
    m_photon_cross_section = aPhotonCrossSection;
}


//-------------------------------------
inline double PolygonMesh::getDensity()
//-------------------------------------
{
    return (m_photon_cross_section.getDensity());
}


//-------------------------------------------------------------
inline const std::string& PolygonMesh::getMaterialLabel() const
//-------------------------------------------------------------
{
    return (m_photon_cross_section.getLabel());
}


//-------------------------------------------------
inline std::string PolygonMesh::getCompound() const
//-------------------------------------------------
{
    return (m_photon_cross_section.getCompound());
}


//---------------------------------------------------------------
inline std::vector<int> PolygonMesh::getMixtureElementSet() const
//---------------------------------------------------------------
{
    return (m_photon_cross_section.getMixtureElementSet());
}


//-----------------------------------------------------------------
inline std::vector<double> PolygonMesh::getMixtureWeightSet() const
//-----------------------------------------------------------------
{
    return (m_photon_cross_section.getMixtureWeightSet());
}


//-------------------------------------------------------
inline bool PolygonMesh::hasNormalVectorsInverted() const
//-------------------------------------------------------
{
    return m_has_inverted_normal_vectors;
}


//-------------------------------------------------------
inline const VEC3& PolygonMesh::getLocalMinCorner() const
//-------------------------------------------------------
{
    // Return the bounding box lower corner
    return (m_local_min_corner);
}


//-------------------------------------------------------
inline const VEC3& PolygonMesh::getLocalMaxCorner() const
//-------------------------------------------------------
{
    // Return the bounding box upper corner
    return (m_local_max_corner);
}


//----------------------------------------------------------
inline unsigned int PolygonMesh::getFaceNormalNumber() const
//----------------------------------------------------------
{
    // Return the number of normal vectors
    // There are 3 GLfloat values per normal
    //return (m_p_face_normal_set.size() / 3);
    return 0;
}


//------------------------------------------------------------
inline unsigned int PolygonMesh::getVertexNormalNumber() const
//------------------------------------------------------------
{
    // Return the number of normal vectors
    // There are 3 GLfloat values per normal
    return (m_p_vertex_normal_set.size() / 3);
}


//------------------------------------------------------
inline unsigned int PolygonMesh::getVertexNumber() const
//------------------------------------------------------
{
    return (m_number_of_vertices);
}


//-----------------------------------------------------
inline unsigned int PolygonMesh::getIndexNumber() const
//-----------------------------------------------------
{
    return (m_number_of_indices);
}


//----------------------------------------------------------------
inline VEC3 PolygonMesh::getFaceNormal(unsigned int anIndex) const
//----------------------------------------------------------------
{
    if (anIndex >= getFaceNumber())
    {
        throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    if (m_number_of_indices)
    {
        VEC3 temp = (getVertexNormal(getIndex(anIndex * 3 + 0)) +
                    getVertexNormal(getIndex(anIndex * 3 + 1)) +
                    getVertexNormal(getIndex(anIndex * 3 + 2))) / 3.0;

        VEC3 normal((getVertex(getIndex(anIndex * 3 + 2)) - getVertex(getIndex(anIndex * 3 + 0)))^(getVertex(getIndex(anIndex * 3 + 2)) - getVertex(getIndex(anIndex * 3 + 1))));
        normal.normalize();

        if (sgn(normal.dotProduct(temp)) > 0)
        {
            return (normal);
        }
        else
        {
            return (-normal);
        }
    }
    else
    {
        VEC3 normal(
            (getVertex(anIndex * 3 + 2) - getVertex(anIndex * 3 + 1)) ^
            (getVertex(anIndex * 3 + 2) - getVertex(anIndex * 3 + 0))
        );
        normal.normalize();

        return (-normal);
    }
}


//------------------------------------------------------------------
inline VEC3 PolygonMesh::getVertexNormal(unsigned int anIndex) const
//------------------------------------------------------------------
{
    if (getVertexNormalNumber() <= anIndex)
    {
        throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    return (VEC3(m_p_vertex_normal_set[anIndex * 3 + 0],
            m_p_vertex_normal_set[anIndex * 3 + 1],
            m_p_vertex_normal_set[anIndex * 3 + 2]));
}


//-------------------------------------------
inline void PolygonMesh::computeBoundingBox()
//-------------------------------------------
{
    // There are vertices
    if (m_number_of_vertices)
    {
        m_local_min_corner = getVertex(0);
        m_local_max_corner = m_local_min_corner;

        for (unsigned int i(1); i < m_number_of_vertices; ++i)
        {
            VEC3 vertex(getVertex(i));

            if (m_local_min_corner.getX() > vertex.getX())
                m_local_min_corner.setX(vertex.getX());

            if (m_local_max_corner.getX() < vertex.getX())
                m_local_max_corner.setX(vertex.getX());

            if (m_local_min_corner.getY() > vertex.getY())
                m_local_min_corner.setY(vertex.getY());

            if (m_local_max_corner.getY() < vertex.getY())
                m_local_max_corner.setY(vertex.getY());

            if (m_local_min_corner.getZ() > vertex.getZ())
                m_local_min_corner.setZ(vertex.getZ());

            if (m_local_max_corner.getZ() < vertex.getZ())
                m_local_max_corner.setZ(vertex.getZ());
        }
    }
}


//----------------------------------------------------------------
inline void PolygonMesh::applyScale(const RATIONAL_NUMBER& aScale)
//----------------------------------------------------------------
{
    applyScale(aScale, aScale, aScale);
}


//-----------------------------------------------------------------------
inline void PolygonMesh::applyTranslation(const VEC3& aTranslationVector)
//-----------------------------------------------------------------------
{
    if (m_number_of_vertices)
        {
        for (unsigned int i(0); i < m_number_of_vertices; i++)
            {
            VEC3 vertex(getVertex(i) + aTranslationVector);
            setVertex(i, vertex);
            }

        m_local_min_corner += aTranslationVector;
        m_local_max_corner += aTranslationVector;

        // The VBO exists
        if (m_p_vbo.get())
            {
            // Update the VBO
            if (m_p_vbo->isReady())
                {
                updateVBO(m_p_vbo->getBufferUsageHint(), m_p_vbo->getPrimitiveType());
                }
            // Reset the VBO to be on the safe side
            else
                {
                m_p_vbo.reset(nullptr);
                }
            }
        }
}


//------------------------------------------------------
inline void PolygonMesh::applyScaling(RATIONAL_NUMBER x,
                                      RATIONAL_NUMBER y,
                                      RATIONAL_NUMBER z)
//------------------------------------------------------
{
    if (m_number_of_vertices)
        {
        VEC3 scaling_factors(x, y, z);
        for (unsigned int i(0); i < m_number_of_vertices; i++)
            {
            VEC3 vertex(getVertex(i).elementWiseProduct(scaling_factors));
            setVertex(i, vertex);
            }

        m_local_min_corner = m_local_min_corner.elementWiseProduct(scaling_factors);
        m_local_max_corner = m_local_max_corner.elementWiseProduct(scaling_factors);

        // The VBO exists
        if (m_p_vbo.get())
            {
            // Update the VBO
            if (m_p_vbo->isReady())
                {
                updateVBO(m_p_vbo->getBufferUsageHint(), m_p_vbo->getPrimitiveType());
                }
            // Reset the VBO to be on the safe side
            else
                {
                m_p_vbo.reset(nullptr);
                }
            }
        }
}


//----------------------------------------------------------
inline void PolygonMesh::setFaceNormal(unsigned int anIndex,
                                       const VEC3& aVertex)
//----------------------------------------------------------
{
    throw Exception(__FILE__, __FUNCTION__, __LINE__, "Do Not Use Face Normal");
/*
    if (getFaceNormalNumber() <= anIndex && m_p_face_normal_set.size())
        {
        throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
        }

    m_p_face_normal_set[anIndex * 3 + 0] = aVertex.getX();
    m_p_face_normal_set[anIndex * 3 + 1] = aVertex.getY();
    m_p_face_normal_set[anIndex * 3 + 2] = aVertex.getZ();*/
}


//------------------------------------------------------------
inline void PolygonMesh::setVertexNormal(unsigned int anIndex,
                                         const VEC3& aVertex)
//------------------------------------------------------------
{
    if (getVertexNormalNumber() <= anIndex && m_p_vertex_normal_set.size())
        {
        throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
        }

    m_p_vertex_normal_set[anIndex * 3 + 0] = aVertex.getX();
    m_p_vertex_normal_set[anIndex * 3 + 1] = aVertex.getY();
    m_p_vertex_normal_set[anIndex * 3 + 2] = aVertex.getZ();
}


//-----------------------------------------------------------------------------
inline void PolygonMesh::applyRotation(const VEC3& aRotationAxis,
                                       RATIONAL_NUMBER aRotationAngleInDegrees)
//-----------------------------------------------------------------------------
{
    if (m_number_of_vertices)
        {
        MATRIX4 rotation_matrix(MATRIX4::buildRotationMatrix(
                aRotationAngleInDegrees,
                aRotationAxis));

        for (unsigned int i(0); i < m_number_of_vertices; i++)
            {
            VEC3 vertex(rotation_matrix * getVertex(i));
            setVertex(i, vertex);
            }

        for (unsigned int i(0); i < getFaceNormalNumber(); i++)
            {
            VEC3 normal_vector(rotation_matrix * getFaceNormal(i));
            setFaceNormal(i, normal_vector);
            }

        for (unsigned int i(0); i < getVertexNormalNumber(); i++)
            {
            VEC3 normal_vector(rotation_matrix * getVertexNormal(i));
            setVertexNormal(i, normal_vector);
            }

        // Update the bounding box
        computeBoundingBox();

        // The VBO exists
        if (m_p_vbo.get())
            {
            // Update the VBO
            if (m_p_vbo->isReady())
                {
                updateVBO(m_p_vbo->getBufferUsageHint(), m_p_vbo->getPrimitiveType());
                }
            // Reset the VBO to be on the safe side
            else
                {
                m_p_vbo.reset(nullptr);
                }
            }
        }
}


//---------------------------------------------------------------------------
inline void PolygonMesh::applyTransform(const MATRIX4& aTransformationMatrix)
//---------------------------------------------------------------------------
{
    if (m_number_of_vertices)
        {
        for (unsigned int i(0); i < m_number_of_vertices; i++)
            {
            VEC3 vertex(aTransformationMatrix * getVertex(i));
            setVertex(i, vertex);
            }

        for (unsigned int i(0); i < getFaceNormalNumber(); i++)
            {
            VEC3 normal_vector(aTransformationMatrix * getFaceNormal(i));
            normal_vector.normalize();
            setFaceNormal(i, normal_vector);
            }

        for (unsigned int i(0); i < getVertexNormalNumber(); i++)
            {
            VEC3 normal_vector(aTransformationMatrix * getVertexNormal(i));
            normal_vector.normalize();
            setVertexNormal(i, normal_vector);
            }

        // Update the bounding box
        computeBoundingBox();

        // The VBO exists
        if (m_p_vbo.get())
            {
            // Update the VBO
            if (m_p_vbo->isReady())
                {
                updateVBO(m_p_vbo->getBufferUsageHint(), m_p_vbo->getPrimitiveType());
                }
            // Reset the VBO to be on the safe side
            else
                {
                m_p_vbo.reset(nullptr);
                }
            }
        }
}


//-------------------------------------
inline void PolygonMesh::moveToCentre()
//-------------------------------------
{
    VEC3 translation_vector(m_local_min_corner + (m_local_max_corner - m_local_min_corner) / 2.0);
    translation_vector *= -1.0;

    applyTranslation(translation_vector);
}


//--------------------------------------------
inline void PolygonMesh::invertNormalVectors()
//--------------------------------------------
{
    m_has_inverted_normal_vectors = !m_has_inverted_normal_vectors;
    
    // Re-order the vertex normals
    for (std::vector<RATIONAL_NUMBER>::iterator ite(
            m_p_vertex_normal_set.begin());
            ite != m_p_vertex_normal_set.end();
            ++ite)
    {
        *ite = -(*ite);
    }

    // // Re-order the face normals
    // for (std::vector<RATIONAL_NUMBER>::iterator ite(
    //         m_p_face_normal_set.begin());
    //         ite != m_p_face_normal_set.end();
    //         ++ite)
    // {
    //     *ite = -(*ite);
    // }

    // We are working with a triangle mesh
    if (m_polygon_type == GL_TRIANGLES)
    {
        // An index is used
        if (m_p_index_set && m_number_of_indices)
        {
            // Re-order the index
            for (unsigned int i = 0; i < m_number_of_indices; i += 3)
            {
                // Swap two indices
                unsigned int idx2 = getIndex(i + 1);
                unsigned int idx3 = getIndex(i + 2);

                setIndex(i + 1, idx3);
                setIndex(i + 2, idx2);
            }
        }
        // An index is not used
        else if (m_p_vertex_set && m_number_of_vertices)
        {
            // Re-order the vertices
            for (unsigned int i = 0; i < m_number_of_vertices; i += 3)
            {
                // Swap two vertices
                VEC3 vec2 = getVertex(i + 1);
                VEC3 vec3 = getVertex(i + 2);

                setVertex(i + 1, vec3);
                setVertex(i + 2, vec2);
            }
        }
    }

    // Update the VBO
    if (m_p_vbo.get())
    {
        if (m_p_vbo->isReady())
        {
            updateVBO(m_p_vbo->getBufferUsageHint(), m_p_vbo->getPrimitiveType());
        }
    }
}


//-----------------------------------------------------
inline const Material& PolygonMesh::getMaterial() const
//-----------------------------------------------------
{
    return (m_material);
}


//-----------------------------------------
inline Material& PolygonMesh::getMaterial()
//-----------------------------------------
{
    return (m_material);
}


} // namespace gVirtualXRay
