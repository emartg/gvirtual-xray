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
*   @file       ImplicitSurface.cxx
*
*   @brief      Class to build 3D implicit surfaces. To know what implicit
*               surfaces are, please visit Paul Bourke's tutorial available at
*               http://paulbourke.net/geometry/implicitsurf/
*
*   @version    1.0
*
*   @date       08/03/2014
*
*   @author     Dr Franck P. Vidal
*
*   @section    License
*               BSD 3-Clause License.
*
*               For details on use and redistribution please refer
*               to http://opensource.org/licenses/BSD-3-Clause
*
*   @section    Copyright
*               (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*               http://www.fpvidal.net/, Dec 2014, 2014, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/

//******************************************************************************
//    Include
//******************************************************************************
#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <sstream>

#ifndef GVXR_IMPLICIT_SURFACE_H
#include "gVirtualXRay/ImplicitSurface.h"
#endif

#ifndef GVXR_EXCEPTION_H
#include "gVirtualXRay/Exception.h"
#endif

#ifndef GVXR_LOGGER_H
#include "gVirtualXRay/Logger.h"
#endif


//******************************************************************************
//    Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//    Constant variables
//******************************************************************************


//******************************************************************************
//    Global variables
//******************************************************************************


//******************************************************************************
//    Function declaration
//******************************************************************************


//--------------------------------
ImplicitSurface::ImplicitSurface()
//--------------------------------
{
}


//---------------------------------
ImplicitSurface::~ImplicitSurface()
//---------------------------------
{
}


//-----------------------------------------------------------
void ImplicitSurface::voxelise(const Vec3ui& aNumberOfVoxels,
                               const VEC3& aCenter,
                               const VEC3& aVoxelSize,
                               unsigned char aVerboseLevel)
//-----------------------------------------------------------
{

    VEC3 half_voxel_size(aVoxelSize.getX() / 2.0,
            aVoxelSize.getY() / 2.0,
            aVoxelSize.getZ() / 2.0);

    VEC3 half_volume_size(half_voxel_size.getX() * aNumberOfVoxels.getX(),
            half_voxel_size.getY() * aNumberOfVoxels.getY(),
            half_voxel_size.getZ() * aNumberOfVoxels.getZ());

    m_centre = aCenter - half_volume_size + half_voxel_size;


    // Get the current size of the volume
    unsigned int previous_width(0);
    unsigned int previous_height(0);
    unsigned int previous_depth(0);

    if (m_volume.getRawData())
    {
        previous_width  = m_volume.getWidth();
        previous_height = m_volume.getHeight();
        previous_depth  = m_volume.getDepth();
    }

    // The size has changed
    if (previous_width != aNumberOfVoxels.getX() ||
            previous_height != aNumberOfVoxels.getY() ||
            previous_depth != aNumberOfVoxels.getZ())
    {
        m_volume = Image<RATIONAL_NUMBER>(
                aNumberOfVoxels.getX(),
                aNumberOfVoxels.getY(),
                aNumberOfVoxels.getZ());
    }

    m_volume.setSpacing(aVoxelSize);

    /*double origin[3];
    origin[0] = offset.getX();
    origin[1] = offset.getY();
    origin[2] = offset.getZ();
    m_p_volume->SetOrigin(origin);
*/

    int width  = aNumberOfVoxels.getX();
    int height = aNumberOfVoxels.getY();
    int depth  = aNumberOfVoxels.getZ();

    unsigned int number_of_voxels = width * height * depth;
    unsigned int thread_progress = 0;

    if (aVerboseLevel)
    {
        LOGGER.logNow("Voxelise the implicit model") << std::endl;
    }

#ifdef NDEBUG
#ifdef WIN32
        #pragma omp parallel for
#else
        #pragma omp parallel for collapse(3)
#endif
#endif
    for (int z = 0; z < depth; ++z)
    {
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                // Deal with the progress bar
#ifdef HAS_OPENMP
                int thread_id = omp_get_thread_num();
#else
                int thread_id = 0;
#endif
                if (thread_id == 0)
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
                }

                RATIONAL_NUMBER voxel_value(0);

                VEC3 voxel_center;

                RATIONAL_NUMBER r(0);

                VEC3 point_1;
                VEC3 point_2;

                VEC3 v1;
                VEC3 v2;

                voxel_center.setX(m_centre.getX() + x * aVoxelSize.getX());
                voxel_center.setY(m_centre.getY() + y * aVoxelSize.getY());
                voxel_center.setZ(m_centre.getZ() + z * aVoxelSize.getZ());

                // Process all the points
                for (std::vector<std::pair<DensityFunctionParameterType, VEC3> >::const_iterator point_ite(m_control_point_set.begin());
                        point_ite != m_control_point_set.end();
                        ++point_ite)
                {
                    r = (voxel_center - point_ite->second).length();
                    voxel_value += evaluate(r, point_ite->first);
                }

                // Process all the lines
                for (std::vector<std::pair<DensityFunctionParameterType, std::pair<VEC3, VEC3> > >::const_iterator line_ite(m_control_line_set.begin());
                        line_ite != m_control_line_set.end();
                        ++line_ite)
                {
                    point_1 = line_ite->second.first;
                    point_2 = line_ite->second.second;

                    const RATIONAL_NUMBER length_squared(pow((point_1 - point_2).length(), 2.0));
                    if (fabs(length_squared) < EPSILON)
                    {
                        r = (voxel_center - point_2).length();
                    }
                    else
                    {
                        const RATIONAL_NUMBER normalised_dot_product(((voxel_center - point_1).dotProduct(point_2 - point_1)) / length_squared);
                        if (normalised_dot_product < 0.0)
                        {
                            r = (voxel_center - point_1).length();
                        }
                        else if (normalised_dot_product > 1.0)
                        {
                            r = (voxel_center - point_2).length();
                        }
                        else
                        {
                            const VEC3 projection(point_1 + normalised_dot_product * (point_2 - point_1));
                            r = (voxel_center - projection).length();
                        }
                    }

                    voxel_value += evaluate(r, line_ite->first);
                }

                m_volume.setPixel(x, y, z, voxel_value);
            }
        }
    }

    if (aVerboseLevel > 1)
    {
        LOGGER.progressBar(1.0, true);
    }
}


//---------------------------------------------------------------
void ImplicitSurface::writeVoxelData(const char* aFileName) const
//---------------------------------------------------------------
{
    m_volume.save(aFileName);
}


//-----------------------------------------------------------------------------
void ImplicitSurface::writePolygonMesh(const char* aFileName, bool aBinaryFlag)
//-----------------------------------------------------------------------------
{
    m_mesh_data.setFileName(aFileName);
    m_mesh_data.writeSTLFile(aBinaryFlag);
}


//------------------------------------------------------------------------
void ImplicitSurface::extractIsoSurface(const RATIONAL_NUMBER& aThreshold,
                                        unsigned char aVerboseLevel)
//------------------------------------------------------------------------
{
    m_mesh_data = m_volume.marchingCubes(aThreshold, 0, false, aVerboseLevel);
    m_mesh_data.applyTranslation(m_centre);
}


//-----------------------------------------------------------------------------
void ImplicitSurface::extractIsoSurface(const RATIONAL_NUMBER& aLowerThreshold,
                                        const RATIONAL_NUMBER& aUpperThreshold,
                                        unsigned char aVerboseLevel)
//-----------------------------------------------------------------------------
{
    m_mesh_data = m_volume.marchingCubes(aLowerThreshold, aUpperThreshold, true, aVerboseLevel);
    m_mesh_data.applyTranslation(m_centre);
}
