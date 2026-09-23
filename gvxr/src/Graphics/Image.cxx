/**
********************************************************************************
*
*   @file       Image.cpp
*
*   @brief      Class to handle a greyscale image.
*
*   @version    1.0
*
*   @todo       Write here anything you did not implement.
*
*   @date       11/11/2016
*
*   @author     Franck Vidal
*
*
********************************************************************************
*/


//******************************************************************************
//  Define
//******************************************************************************
#define LINE_SIZE 4096


//******************************************************************************
//  Include
//******************************************************************************
#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <fstream>

#ifndef GVXR_IMAGE_H
#include "gVirtualXRay/Image.h"
#endif

#ifndef GVXR_CUBE_MESH_H
#include "gVirtualXRay/CubeMesh.h"
#endif

#ifndef GVXR_EXCEPTION_H
#include "gVirtualXRay/Exception.h"
#endif

#ifndef GVXR_XRAY_RENDERER_H
#include "gVirtualXRay/XRayRenderer.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


template<> unsigned int Image<char>::g_mesh_type = GL_TRIANGLES;
template<> unsigned int Image<unsigned char>::g_mesh_type = GL_TRIANGLES;
template<> unsigned int Image<short>::g_mesh_type = GL_TRIANGLES;
template<> unsigned int Image<unsigned short>::g_mesh_type = GL_TRIANGLES;
template<> unsigned int Image<int>::g_mesh_type = GL_TRIANGLES;
template<> unsigned int Image<unsigned int>::g_mesh_type = GL_TRIANGLES;
template<> unsigned int Image<float>::g_mesh_type = GL_TRIANGLES;
template<> unsigned int Image<double>::g_mesh_type = GL_TRIANGLES;

template<> unsigned int Image<char>::g_vbo_type = GL_STATIC_DRAW;
template<> unsigned int Image<unsigned char>::g_vbo_type = GL_STATIC_DRAW;
template<> unsigned int Image<short>::g_vbo_type = GL_STATIC_DRAW;
template<> unsigned int Image<unsigned short>::g_vbo_type = GL_STATIC_DRAW;
template<> unsigned int Image<int>::g_vbo_type = GL_STATIC_DRAW;
template<> unsigned int Image<unsigned int>::g_vbo_type = GL_STATIC_DRAW;
template<> unsigned int Image<float>::g_vbo_type = GL_STATIC_DRAW;
template<> unsigned int Image<double>::g_vbo_type = GL_STATIC_DRAW;


//-----------------------------------------------------------------------
template<> void Image<float>::saveMHD(const std::string& aFileName,
    bool useDeflateCompressionIfPossible) const
//-----------------------------------------------------------------------
    {
    std::ofstream header_file(aFileName.data());

    if (!header_file.is_open())
        {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot open the file");
        }

    // Save the header file
    header_file << "ObjectType = Image" << std::endl;
    header_file << "NDims = 3" << std::endl;
    header_file << "BinaryData = True" << std::endl;
    header_file << "BinaryDataByteOrderMSB = False" << std::endl;
    header_file << "CompressedData = " << (useDeflateCompressionIfPossible? "True":"False") << std::endl;
    header_file << "TransformMatrix = 1 0 0 0 1 0 0 0 1" << std::endl;
    header_file << "Offset = 0 0 0" << std::endl;
    header_file << "CenterOfRotation = 0 0 0" << std::endl;
    header_file << "AnatomicalOrientation = RAI" << std::endl;
    header_file << "ElementSpacing = " << m_spacing.getX() << " " << m_spacing.getY() << " " << m_spacing.getZ() << std::endl;
    header_file << "DimSize = " << m_width << " " << m_height << " " << m_number_of_slices << std::endl;
    header_file << "ElementType = MET_FLOAT" << std::endl;

    std::string file_name_with_out_extension(aFileName.substr(0, aFileName.size() - 4));
    header_file << "ElementDataFile = " << file_name_with_out_extension + ".raw" << std::endl;

    // Save the RAW file
    saveRAW(file_name_with_out_extension + ".raw",
        useDeflateCompressionIfPossible);
}


//-----------------------------------------------------------------------------------------------
template<> void Image<short>::convertVoxelsToCubes(std::map<short, PolygonMesh>& aPolygonMeshSet,
                                                   short anAirThreshold,
                                                   unsigned char aVerboseLevel) const
//-----------------------------------------------------------------------------------------------
{
    // Make sure there is no data in aPolygonMeshSet
    aPolygonMeshSet.clear();

    // Create the base cube
    CubeMesh<float> base_cube(cm);

    // The cube size is the same as the voxel size
    base_cube.applyScale(m_spacing.getX() / cm, m_spacing.getY() / cm, m_spacing.getZ() / cm);

    // Translate the cube so that its lower corner is on (0, 0, 0)
    base_cube.applyTranslation(m_spacing / 2.0);

    if (aVerboseLevel)
    {
        LOGGER.logNow("Convert voxels to cubes") << std::endl;
    }

    unsigned int number_of_voxels(m_width * m_height * m_number_of_slices);
	unsigned int thread_progress = 0;

    // Process every voxel
    for (unsigned int k(0); k < m_number_of_slices; ++k)
        {
        for (unsigned int j(0); j < m_height; ++j)
            {
            for (unsigned int i(0); i < m_width; ++i)
                {
                thread_progress++;

                if (aVerboseLevel)
                    {
#ifdef HAS_OPENMP
                    int number_of_threads = omp_get_num_threads();
#else
                    int number_of_threads = 1;
#endif

                    unsigned int tasks_per_thread = number_of_voxels / number_of_threads;

                    if (thread_progress == 1)
                        {
                        LOGGER.logNow("Number of threads: ") << number_of_threads << std::endl;
                        LOGGER.logNow("Number of voxels per thread: ") << tasks_per_thread << std::endl << std::flush;
                        }

                    if (aVerboseLevel > 1)
                        {
                        LOGGER.progressBar(thread_progress, tasks_per_thread);
                        }
                    }

                // Get the voxel value
                short voxel_value(getPixel(i, j ,k));

                // Ignore air and padding
                if (voxel_value > anAirThreshold)
                    {
                    // Copy the cube and translate it to the position of the voxel
                    PolygonMesh translated_cube(base_cube);
                    translated_cube.applyTranslation(VEC3(m_spacing[0] * i, m_spacing[1] * j, m_spacing[2] * k));

                    // Add the voxel
                    aPolygonMeshSet[voxel_value] += translated_cube;
                    aPolygonMeshSet[voxel_value].setHounsfieldValue(voxel_value);
                    }
                }
            }
        }

    if (aVerboseLevel > 1)
        {
        LOGGER.progressBar(1.0, true);
        }
}


//-----------------------------------------------------------------------
template<> Image<float> Image<float>::convertHU2mu(double anEnergy) const
//-----------------------------------------------------------------------
{
    Image<float> temp(*this);

#ifdef NDEBUG
        #pragma omp parallel for
#endif
    for (int i = 0; i < int(m_width * m_height * m_number_of_slices); ++i)
        {
        temp.m_p_image[i] = XRayRenderer::getMuFromHU(m_p_image[i],
                anEnergy);
        }

    return (temp);
}


//-------------------------------------------------------------------------
template<> Image<double> Image<double>::convertHU2mu(double anEnergy) const
//-------------------------------------------------------------------------
{
    Image<double> temp(*this);

#ifdef NDEBUG
        #pragma omp parallel for
#endif
    for (int i = 0; i < int(m_width * m_height * m_number_of_slices); ++i)
        {
        temp.m_p_image[i] = XRayRenderer::getMuFromHU(m_p_image[i],
                anEnergy);
        }

    return (temp);
}


//------------------------------------------------
template<> void Image<float>::allocateHostMemory()
//------------------------------------------------
{
#ifdef HAS_OPENGL
    // Generate the texture name
    if (!m_texture_id)
	{
    	glGenTextures(1, &m_texture_id);
    	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

//    // 2D case
//    if (m_number_of_slices == 1)
//    {
//        // Set the texture properties
//		glBindTexture(GL_TEXTURE_2D, m_texture_id);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    }
//    // 3D case
//    else
    {
        // Set the texture properties
		glBindTexture(GL_TEXTURE_3D, m_texture_id);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Make sure the texture can be created using a texture proxy
    GLint texture_width(0);

//    // 2D case
//    if (m_number_of_slices == 1)
//    {
//    	glTexImage2D(GL_PROXY_TEXTURE_2D, 0, GL_R32F, m_width, m_height, 0, GL_RED, GL_FLOAT, 0);
//        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
//
//        glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &texture_width);
//    }
//    // 3D case
//    else
    {
    	glTexImage3D(GL_PROXY_TEXTURE_3D, 0, GL_R32F, m_width, m_height, m_number_of_slices, 0, GL_RED, GL_FLOAT, 0);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        glGetTexLevelParameteriv(GL_PROXY_TEXTURE_3D, 0, GL_TEXTURE_WIDTH, &texture_width);
    }

    if ((unsigned int)(texture_width) != m_width)
    {
        std::stringstream error_message;
        error_message << "Cannot create a " << "(" << m_width << " x " << m_height << " x " << m_number_of_slices << ") floating point render texture.";
        throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
    }

//    // 2D case
//    if (m_number_of_slices == 1)
//    {
//    	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, m_width, m_height, 0, GL_RED, GL_FLOAT, 0);
//        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
//    }
//    // 3D case
//    else
    {
    	glTexImage3D(GL_TEXTURE_3D, 0, GL_R32F, m_width, m_height, m_number_of_slices, 0, GL_RED, GL_FLOAT, 0);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
    }
#endif
}


//--------------------------------------------------------
template<> void Image<unsigned char>::allocateHostMemory()
//--------------------------------------------------------
{
#ifdef HAS_OPENGL
    // Generate the texture name
    if (!m_texture_id)
	{
    	glGenTextures(1, &m_texture_id);
    	checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
	}

//    // 2D case
//    if (m_number_of_slices == 1)
//    {
//        // Set the texture properties
//		glBindTexture(GL_TEXTURE_2D, m_texture_id);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    }
    // 3D case
    else
    {
        // Set the texture properties
		glBindTexture(GL_TEXTURE_3D, m_texture_id);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
    checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

    // Make sure the texture can be created using a texture proxy
    GLint texture_width(0);

    // 2D case
//    if (m_number_of_slices == 1)
//    {
//    	glTexImage2D(GL_PROXY_TEXTURE_2D, 0, GL_R8UI, m_width, m_height, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, 0);
//        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
//
//        glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &texture_width);
//    }
//    // 3D case
//    else
    {
    	glTexImage3D(GL_PROXY_TEXTURE_3D, 0, GL_R8UI, m_width, m_height, m_number_of_slices, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, 0);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);

        glGetTexLevelParameteriv(GL_PROXY_TEXTURE_3D, 0, GL_TEXTURE_WIDTH, &texture_width);
    }

    if ((unsigned int)(texture_width) != m_width)
    {
        std::stringstream error_message;
        error_message << "Cannot create a " << "(" << m_width << " x " << m_height << " x " << m_number_of_slices << ") floating point render texture.";
        throw Exception(__FILE__, __FUNCTION__, __LINE__, error_message.str());
    }

//    // 2D case
//    if (m_number_of_slices == 1)
//    {
//    	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, m_width, m_height, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, 0);
//        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
//    }
//    // 3D case
//    else
    {
    	glTexImage3D(GL_TEXTURE_3D, 0, GL_R8UI, m_width, m_height, m_number_of_slices, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, 0);
        checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
    }
#endif
}









};
