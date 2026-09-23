/*

Copyright (c) 2017, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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


#ifndef GVXR_XRAY_RENDERER_H
#define GVXR_XRAY_RENDERER_H


/**
********************************************************************************
*
*   @file       XRayRenderer.h
*
*   @brief      Class to compute and renderer X-ray images on GPU.
*
*   @version    1.0
*
*   @date       13/01/2017
*
*   @author     Dr Franck P. Vidal
*
*   @todo       Make sure m_CT_centre_of_rotation_position works well

Functions to port to OpenMP:
resetLBuffer
preComputeLBuffer
postComputeLBuffer
computeInnerSurfaceLBuffer
computeOuterSurfaceLBuffer
computeSumMUxDx

*               - pushProjectionMatrix
*               - pushModelViewMatrix
*               - popProjectionMatrix
*               - popModelViewMatrix
*               -
*               -
*               -
*               -
*               - updateCumulatedLBuffers
*               - pushFBO
*               - pushTexture3D
*               - pushEnableDisableState
*               - popTexture
*               - popEnableDisableState
*               - popFBO
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*   http://www.fpvidal.net/, Jan 2017, 2017, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <random>
#include <cstdint>

#ifdef HAS_BOOST
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#endif // HAS_BOOST

#ifndef GVXR_IMAGE_H
#include "gVirtualXRay/Image.h"
#endif

#ifndef GVXR_SINOGRAM_H
#include "gVirtualXRay/Sinogram.h"
#endif

#ifndef GVXR_SCENEGRAPH_NODE_H
#include "gVirtualXRay/SceneGraphNode.h"
#endif

#ifndef GVXR_POLYGON_MESH_H
#include "gVirtualXRay/PolygonMesh.h"
#endif

#ifndef GVXR_XRAY_DETECTOR_H
#include "gVirtualXRay/XRayDetector.h"
#endif

#ifndef GVXR_XRAY_BEAM_H
#include "gVirtualXRay/XRayBeam.h"
#endif

#ifndef GVXR_SHADER_H
#include "gVirtualXRay/Shader.h"
#endif

#ifndef GVXR_ATTENUATION_COEFFICIENT_H
#include "gVirtualXRay/AttenuationCoefficient.h"
#endif

#ifndef GVXR_VBO_h
#include "gVirtualXRay/VBO.h"
#endif


//******************************************************************************
//  Definitions
//******************************************************************************
#define XRAY_DETECTOR_PBO_NUMBER        2   ///< The total number of PBOs
#define XRAY_DETECTOR_2D_TEXTURE_NUMBER 4   ///< The total number of 2D textures
#define XRAY_DETECTOR_3D_TEXTURE_NUMBER 1   ///< The total number of 3D textures
#define XRAY_DETECTOR_SHADER_NUMBER     7   ///< The total number of
                                            ///< program shaders


//******************************************************************************
//  Class definition
//******************************************************************************
class FramebufferObject;


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


    namespace MixedPrecisionFlags {
        constexpr uint8_t UncleanedLBuffer = 1 << 0;
        constexpr uint8_t CleanedLBuffer   = 1 << 1;
        constexpr uint8_t CumulatedLBuffer = 1 << 2;
        constexpr uint8_t EnergyFluence    = 1 << 3;
        constexpr uint8_t SumMuXdX         = 1 << 4;
        constexpr uint8_t ZBuffer          = 1 << 5;
    };


    bool getPrecisionUncleanedLBuffer(uint8_t aFlag);
    bool getPrecisionCleanedLBuffer(uint8_t aFlag);
    bool getPrecisionCumulatedLBuffer(uint8_t aFlag);
    bool getPrecisionEnergyFluence(uint8_t aFlag);
    bool getPrecisionSumMuXdX(uint8_t aFlag);
    bool getPrecisionZBuffer(uint8_t aFlag);
    bool isFullPrecision(uint8_t aFlag);
    bool isHalfPrecision(uint8_t aFlag);
    bool isMixedPrecision(uint8_t aFlag);


//==============================================================================
/**
*   @class  XRayRenderer
*   @brief  XRayRenderer is a class to compute and renderer X-ray images on GPU.
*/
//==============================================================================
class XRayRenderer
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    typedef RATIONAL_NUMBER PixelType; ///< Pixel type


    //--------------------------------------------------------------------------
    /// Type to store the computation engine (OpenGL, CUDA, OpenCL, etc.).
    //--------------------------------------------------------------------------
    typedef enum ComputingModeTag
    {
        NONE, ///< Not set as yet
        OPENGL, ///< Computation using OpenGL
        VULKAN, ///< Computation using Vulkan
        CUDA,   ///< Computation using CUDA
        OPENCL,  ///< Computation using OpenCL
        OPENMP  ///< Computation on CPU using OpenMP
    } ComputingMode;


    //--------------------------------------------------------------------------
    /// Default constructor.
    //--------------------------------------------------------------------------
    XRayRenderer();


    //--------------------------------------------------------------------------
    /// Destructor.
    //--------------------------------------------------------------------------
    ~XRayRenderer();


    //--------------------------------------------------------------------------
    /// Initialise the renderer.
    /**
     *  @param aComputingMode:          the computing mode (OPENGL, CUDA, or
     *                                  OPENCL)
     *  @param aMixedPrecisionFlag: the flag that stores the internal format in which 
     *                                  each texture will be stored (GL_RGB16F_ARB,
     *                                  or GL_RGB32F_ARB)
     *  @param apDetector:              the X-ray detector that will be used
     *  @param apBeamSpectrum:          the X-ray beam spectrum
     */
    //--------------------------------------------------------------------------
    void initialise(ComputingMode aComputingMode,
            uint8_t aMixedPrecisionFlag,
            const XRayDetector* apDetector,
            const XRayBeam* apBeamSpectrum);


    //--------------------------------------------------------------------------
    /// Reset the renderer (release memory on the host and the device,
    /// and restore default values).
    //--------------------------------------------------------------------------
    void release();


    //--------------------------------------------------------------------------
    /// Set the required computing mode. supported values are:
    /// - OPENGL, and
    /// - OPENMP.
     //--------------------------------------------------------------------------
    void setComputingMode(ComputingMode aMode);


    //--------------------------------------------------------------------------
    /// Accessor on the computing mode. supported values are:
    /// - OPENGL, and
    /// - OPENMP
    /**
     *  @return the computing mode
     */
    //--------------------------------------------------------------------------
    const ComputingMode& getComputingMode() const;


    //--------------------------------------------------------------------------
    /// Check if the renderer is ready to simulate X-ray images.
    /**
     *  @return true if the renderer is ready, false if it is not
     */
    //--------------------------------------------------------------------------
    bool isReady() const;


    //--------------------------------------------------------------------------
    /// Accessor on a total energy of the beam when the detector response is applied.
    /**
     *  @param anApplyFiltrationFlag: true if the number of photons apply to the filtered spectrum;
     *                                false if the number of photons apply to the unfiltered spectrum
     *  @return the total energy
     */
    //--------------------------------------------------------------------------
    RATIONAL_NUMBER getTotalEnergyWithDetectorResponse(bool anApplyFiltrationFlag) const;


    //--------------------------------------------------------------------------
    /// Accessor on a total energy of the beam when the detector response is not applied.
    /**
     *  @param anApplyFiltrationFlag: true if the number of photons apply to the filtered spectrum;
     *                                false if the number of photons apply to the unfiltered spectrum
     *  @return the total energy
     */
    //--------------------------------------------------------------------------
    RATIONAL_NUMBER getTotalEnergyWithoutDetectorResponse(bool anApplyFiltrationFlag) const;


    //--------------------------------------------------------------------------
    /// Add a 3D object to be rendered. It will be used as an internal surface.
    /**
     *  @param apSurface:   the 3D object
     */
    //--------------------------------------------------------------------------
    void addInnerSurface(const SceneGraphNode* apSurface);


    //--------------------------------------------------------------------------
    /// Add a 3D object to be rendered. It will be used as an external surface.
    /**
     *  @param apSurface:   the 3D object
     */
    //--------------------------------------------------------------------------
    void addOuterSurface(const SceneGraphNode* apSurface);


    //--------------------------------------------------------------------------
    /// Add a 3D object to be rendered. It will be used as an internal surface.
    /**
     *  @param apSurface:   the 3D object
     */
    //--------------------------------------------------------------------------
    void addInnerSurface(const PolygonMesh* apSurface);


    //--------------------------------------------------------------------------
    /// Add a 3D object to be rendered. It will be used as an external surface.
    /**
     *  @param apSurface:   the 3D object
     */
    //--------------------------------------------------------------------------
    void addOuterSurface(const PolygonMesh* apSurface);


    //--------------------------------------------------------------------------
    /// Check if a given PolygonMesh is an inner surface.
    /**
     *  @param apNode:   the node in the scenegraph
     *  @return true if the PolygonMesh is an inner surface, return false otherwise
     */
    //--------------------------------------------------------------------------
    bool isInnerSurface(const SceneGraphNode* apNode);


    //--------------------------------------------------------------------------
    /// Check if a given PolygonMesh is an outer surface.
    /**
     *  @param apNode:   the node in the scenegraph
     *  @return true if the PolygonMesh is an outer surface, return false otherwise
     */
    //--------------------------------------------------------------------------
    bool isOuterSurface(const SceneGraphNode* apNode);


    //--------------------------------------------------------------------------
    /// Remove the external surface from the renderer if any.
    //--------------------------------------------------------------------------
    void removeInnerSurfaces();


    //--------------------------------------------------------------------------
    /// Remove the internal surfaces from the renderer if any.
    //--------------------------------------------------------------------------
    void removeOuterSurface();


    //--------------------------------------------------------------------------
    /// Compute the L-buffer of a 3D object
    /**
     *  @param anObject:                        the 3D object to process
     *  @param aXRaySourceDetectorTranslation:  the translation vector to use
     *                                          when the source is not aligned
     *                                          with the detector
     *  @param aModellingTransformationMatrix:  the transformation matrix of
     *                                          the 3D object
     *  @param aGPUStateConservationFlag:   if true, then restore the OpenGL 
     *                                      states as they were before 
     *                                      the method was called. If false,
     *                                      leave them as they are. It is useful
     *                                      to leave them when the method is 
     *                                      called repetitively, i.e. to save time.
     */
    //--------------------------------------------------------------------------
    void computeLBuffer(SceneGraphNode& anObject,
                        const VEC3& aXRaySourceDetectorTranslation,
                        const MATRIX4& aModellingTransformationMatrix,
                        bool aGPUStateConservationFlag = true);


    //--------------------------------------------------------------------------
    /// Compute the L-buffer of a 3D object
    /**
     *  @param anObject:                        the 3D object to process
     *  @param aXRaySourceDetectorTranslation:  the translation vector to use
     *                                          when the source is not aligned
     *                                          with the detector
     *  @param aModellingTransformationMatrix:  the transformation matrix of
     *                                          the 3D object
     *  @param aGPUStateConservationFlag:   if true, then restore the OpenGL 
     *                                      states as they were before 
     *                                      the method was called. If false,
     *                                      leave them as they are. It is useful
     *                                      to leave them when the method is 
     *                                      called repetitively, i.e. to save time.
     */
    //--------------------------------------------------------------------------
    void computeLBuffer(PolygonMesh& anObject,
                        const VEC3& aXRaySourceDetectorTranslation,
                        const MATRIX4& aModellingTransformationMatrix,
                        bool aGPUStateConservationFlag = true);


    //--------------------------------------------------------------------------
    /// Compute the X-ray image.
    /**
     *  @param aModellingTransformationMatrix:  a modelling transformation
     *                                          matrix that will be used on all
     *                                          the 3D objects
     *  @param anIntegrateEnergyFlag: if true the energy fluence is returned,
     *                                otherwise the number of photons is returned
     *                                (default value: true)
     */
    //--------------------------------------------------------------------------
    void computeImage(const MATRIX4& aModellingTransformationMatrix,
        bool anIntegrateEnergyFlag = true);


    //--------------------------------------------------------------------------
    /// Save a FBO into a file.
    /**
     *  @param aFBOID:      the ID of the FBO
     *  @param aFileName:   the file name
     *  @param useCompression:  use compression if the file format supports it (default value: false)
     */
    //--------------------------------------------------------------------------
    void printFBO(unsigned int aFBOID,
        const char* aFileName,
        bool useCompression = false);


    //--------------------------------------------------------------------------
    /// Save a FBO into a file.
    /**
     *  @param aFBOID:      the ID of the FBO
     *  @param aFileName:   the file name
     *  @param useCompression:  use compression if the file format supports it (default value: false)
     */
    //--------------------------------------------------------------------------
    void printFBO(unsigned int aFBOID,
        const std::string& aFileName,
        bool useCompression = false);


    //--------------------------------------------------------------------------
    /// Save a texture into a file.
    /**
     *  @param aTextureID:  the ID of the texture
     *  @param aFileName:   the file name
     *  @param useCompression:  use compression if the file format supports it (default value: false)
     */
    //--------------------------------------------------------------------------
    void print2DTexture(unsigned int aTextureID,
        const char* aFileName,
        bool useCompression = false);


    //--------------------------------------------------------------------------
    /// Save a texture into a file.
    /**
     *  @param aTextureID:  the ID of the texture
     *  @param aFileName:   the file name
     *  @param useCompression:  use compression if the file format supports it (default value: false)
     */
    //--------------------------------------------------------------------------
    void print2DTexture(unsigned int aTextureID,
        const std::string& aFileName,
        bool useCompression = false);


    //--------------------------------------------------------------------------
    /// Save the current L-buffer into a file.
    /**
     *  @param aFileName:   the file name
     *  @param useCompression:  use compression if the file format supports it (default value: false)
     */
    //--------------------------------------------------------------------------
    void printLBuffer(const char* aFileName, bool useCompression = false);


    //--------------------------------------------------------------------------
    /// Save the current L-buffer into a file.
    /**
     *  @param aFileName:   the file name
     *  @param useCompression:  use compression if the file format supports it (default value: false)
     */
    //--------------------------------------------------------------------------
    void printLBuffer(const std::string& aFileName, bool useCompression = false);


    //--------------------------------------------------------------------------
    /// Save the current cumulated L-buffer into a file.
    /**
     *  @param aFileName:   the file name
     *  @param useCompression:  use compression if the file format supports it (default value: false)
     */
    //--------------------------------------------------------------------------
    void printCumulatedLBuffer(const char* aFileName, bool useCompression = false);


    //--------------------------------------------------------------------------
    /// Save the current cumulated L-buffer into a file.
    /**
     *  @param aFileName:   the file name
     *  @param useCompression:  use compression if the file format supports it (default value: false)
     */
    //--------------------------------------------------------------------------
    void printCumulatedLBuffer(const std::string& aFileName, bool useCompression = false);


    //--------------------------------------------------------------------------
    /// Save the current Mux Dx into a file.
    /**
     *  @param aFileName:   the file name
     *  @param useCompression:  use compression if the file format supports it (default value: false)
     */
    //--------------------------------------------------------------------------
    void printSumMuxDx(const char* aFileName, bool useCompression = false);


    //--------------------------------------------------------------------------
    /// Save the current Mux Dx into a file.
    /**
     *  @param aFileName:   the file name
     *  @param useCompression:  use compression if the file format supports it (default value: false)
     */
    //--------------------------------------------------------------------------
    void printSumMuxDx(const std::string& aFileName, bool useCompression = false);


    //--------------------------------------------------------------------------
    /// Save the current energy fluence into a file (same as printXRayImage()).
    /**
     *  @param aFileName:   the file name
     *  @param useCompression:  use compression if the file format supports it (default value: false)
     */
    //--------------------------------------------------------------------------
    void printEnergyFluence(const char* aFileName, bool useCompression = false);


    //--------------------------------------------------------------------------
    /// Save the current energy fluence into a file (same as printXRayImage()).
    /**
     *  @param aFileName:   the file name
     *  @param useCompression:  use compression if the file format supports it (default value: false)
     */
    //--------------------------------------------------------------------------
    void printEnergyFluence(const std::string& aFileName, bool useCompression = false);


    //--------------------------------------------------------------------------
    /// Save the current energy fluence into a file
    /// (same as printEnergyFluence()).
    /**
     *  @param aFileName:   the file name
     *  @param useCompression:  use compression if the file format supports it (default value: false)
     */
    //--------------------------------------------------------------------------
    void printXRayImage(const char* aFileName, bool useCompression = false);


    //--------------------------------------------------------------------------
    /// Save the current energy fluence into a file
    /// (same as printEnergyFluence()).
    /**
     *  @param aFileName:   the file name
     *  @param useCompression:  use compression if the file format supports it (default value: false)
     */
    //--------------------------------------------------------------------------
    void printXRayImage(const std::string& aFileName, bool useCompression = false);


    //--------------------------------------------------------------------------
    /// Accessor on the raw data from a FBO.
    /**
     *  @param aFBOID:  the ID of the FBO
     */
     //--------------------------------------------------------------------------
    RATIONAL_NUMBER* getFBO(unsigned int aFBOID);


    //--------------------------------------------------------------------------
    /// Accessor on the raw data from a FBO.
    /**
     *  @param aFBOID:  the ID of the FBO
     */
    //--------------------------------------------------------------------------
    void getFBO(unsigned int aFBOID,
        float* apImage,
        unsigned int aWidth,
        unsigned int aHeight);


    //--------------------------------------------------------------------------
    /// Accessor on the raw data from a FBO.
    /**
     *  @param aFBOID:  the ID of the FBO
     */
    //--------------------------------------------------------------------------
    void getFBO(unsigned int aFBOID,
        double* apImage,
        unsigned int aWidth,
        unsigned int aHeight);


    //--------------------------------------------------------------------------
    /// Accessor on the OpenGL ID of the FBO.
    /**
     *  @param aFBOID:  the ID of the FBO
     *  @return the OpenGL ID of the FBO
     */
    //--------------------------------------------------------------------------
    GLuint getFboId(unsigned int aFBOID);


    //--------------------------------------------------------------------------
    /// Accessor on the OpenGL ID of the texture.
    /**
     *  @param aTextureID:  the ID of the texture
     *  @return the OpenGL ID of the texture
     */
    //--------------------------------------------------------------------------
    GLuint getTextureId(unsigned int aTextureID);


    //--------------------------------------------------------------------------
    /// Accessor on the raw data from a texture.
    /**
     *  @param aTextureID:  the ID of the texture
     */
    //--------------------------------------------------------------------------
    RATIONAL_NUMBER* get2DTexture(unsigned int aTextureID);


    //--------------------------------------------------------------------------
    /// Display a 2D texture using OpenGL.
    /**
     *  @param aTextureID:  the ID of the texture
     */
    //--------------------------------------------------------------------------
    void display2DTexture(unsigned int aTextureID);


	//--------------------------------------------------------------------------
	/// Get id 2D texture using OpenGL.
	//--------------------------------------------------------------------------
	unsigned int getID2DTexture() const;


    //--------------------------------------------------------------------------
    /// Display the L-buffer using OpenGL.
    //--------------------------------------------------------------------------
    void displayLBuffer();


    //--------------------------------------------------------------------------
    /// Display the energy fluence using OpenGL (same as displayXRayImage()).
    //--------------------------------------------------------------------------
    void displayEnergyFluence();


    //--------------------------------------------------------------------------
    /// Display the energy fluence using OpenGL
    /// (same as displayEnergyFluence()).
    //--------------------------------------------------------------------------
    void displayXRayImage();


    //--------------------------------------------------------------------------
    /// Normalise the final result to display in OpenGL.
    //--------------------------------------------------------------------------
    void getEnergyFluenceMinMax();


    //--------------------------------------------------------------------------
    /// Display the X-ray image using OpenGL. First the shift/scale filter is 
    /// used so that The pixel \f$I(x,y)\f$ will be replaced with \f$I_t(x,y) = $(I(x,y) + shift) \times scale\f$.
    /// Then a log scale or a power law can be applied (optional). 
    /// \p useLogScale and \p usePowerLaw cannot be used at the same time.
    /**
     *  @param useNormalisation: use the min/max normalisation.
     *  @param useLogScale: use the log scale.
     *  @param usePowerLaw: use the power law scale.
     *  @param shift: the defalut value of shift in the shift/scale filter. It is used if \p useNormalisation is false. If \p useNormalisation is true, then the value is computed automatically (default value: 0)
     *  @param scale: the defalut value of scale in the shift/scale filter. It is used if \p useNormalisation is false. If \p useNormalisation is true, then the value is computed automatically (default value: 1)
     *  @param gamma: the \f$\gamma\f$ value used in the power law if \p usePowerLaw is true. The pixel \f$I_t(x,y)\f$ will be replaced with the \f$I_t(x,y)^\gamma\f$ in the visualisation. (default value: (1.0 / 2.5))
     */
    //--------------------------------------------------------------------------
    void display(bool useNormalisation,
        bool useLogScale,
        bool usePowerLaw,
        double shift = 0.0,
        double scale = 1.0,
        double gamma = (1.0 / 2.5));


    //--------------------------------------------------------------------------
    /// Get the attenuation coefficient of water for a given energy.
    /// The value is computed using linear interpolation.
    /**
     *  @param anIncidentEnergy:    the energy
     *  @return the corresponding attenuation coefficient of water
     */
    //--------------------------------------------------------------------------
    RATIONAL_NUMBER getMuWaterLinearInterpolation(
            const RATIONAL_NUMBER& anIncidentEnergy);


    //--------------------------------------------------------------------------
    /// Get the attenuation coefficient of water for a given energy.
    /// The value is computed using linear interpolation on a logarithmic scale.
    /**
     *  @param anIncidentEnergy:    the energy
     *  @return the corresponding attenuation coefficient of water
     */
    //--------------------------------------------------------------------------
    static RATIONAL_NUMBER getMuWaterLogLinearInterpolation(
            const RATIONAL_NUMBER& anIncidentEnergy);


    //--------------------------------------------------------------------------
    /// Convert a Hounsfield value into a linear attenuation coefficient (\f$\mu\f$).
    /**
     *  @param HU:  the Hounsfield value to convert
     *  @param mu_water:    the attenuation coefficient of water at
    *                       a given energy
     *  @return the corresponding attenuation coefficient
     */
    //--------------------------------------------------------------------------
    static RATIONAL_NUMBER getMuFromHUAndMuWater(const RATIONAL_NUMBER& HU,
            const RATIONAL_NUMBER& mu_water);


    //--------------------------------------------------------------------------
    /// Convert a Hounsfield value into a linear attenuation coefficient (\f$\mu\f$).
    /**
     *  @param HU:  the Hounsfield value to convert
     *  @param anEnergy:    a given energy
     *  @return the corresponding attenuation coefficient
     */
    //--------------------------------------------------------------------------
    static RATIONAL_NUMBER getMuFromHU(const RATIONAL_NUMBER& HU,
                                const RATIONAL_NUMBER& anEnergy);


    //--------------------------------------------------------------------------
    /// Convert a Hounsfield value into a mass attenuation coefficient (\f$\mu / \rho\f$).
    /**
     *  @param HU:  the Hounsfield value to convert
     *  @param anEnergy:    a given energy
     *  @return the corresponding attenuation coefficient
     */
    //--------------------------------------------------------------------------
    static RATIONAL_NUMBER getMassAttenuationFromHU(const RATIONAL_NUMBER& HU,
            const RATIONAL_NUMBER& anEnergy);


    //--------------------------------------------------------------------------
    /// Get the density (\f$\rho\f$) of a tissue given a Hounsfield value.
    /**
     *  @param HU:  the Hounsfield value to consider
     *  @return the corresponding density
     */
    //--------------------------------------------------------------------------
    static RATIONAL_NUMBER getDensityFromHU(const RATIONAL_NUMBER& HU);


    //--------------------------------------------------------------------------
    /// Get the linear attenuation coefficient (\f$\mu\f$) of a chemical element.
    /**
     *  @param anElementSymbol:  the symbol of the chemical element to consider
     *  @param anEnergy:    a given energy
     *  @return the corresponding attenuation coefficient
     */
    //--------------------------------------------------------------------------
    static RATIONAL_NUMBER getElementMu(const char* anElementSymbol,
                                       const RATIONAL_NUMBER& anEnergy);


    //--------------------------------------------------------------------------
    /// Get the mass attenuation coefficient (\f$\mu / \rho\f$) of a chemical element.
    /**
     *  @param anElementSymbol:  the symbol of the chemical element to consider
     *  @param anEnergy:    a given energy
     *  @return the corresponding attenuation coefficient
     */
    //--------------------------------------------------------------------------
    RATIONAL_NUMBER getElementMassAttenuation(const char* anElementSymbol,
                                             const RATIONAL_NUMBER& anEnergy);


    //--------------------------------------------------------------------------
    /// Get the density (\f$\rho\f$) of a chemical element.
    /**
     *  @param anElementSymbol:  the symbol of the chemical element to consider
     *  @return the corresponding density
     */
    //--------------------------------------------------------------------------
    RATIONAL_NUMBER getElementDensity(const char* anElementSymbol);


    //--------------------------------------------------------------------------
    /// Display the final image in negative or not.
    /**
     *  @param aFlag:   true is the final image is displayed in negative,
     *                  false it is not (default value: true)
     */
    //--------------------------------------------------------------------------
    void useNegativeFilteringFlag(bool aFlag = true);


    //--------------------------------------------------------------------------
    /// Accessor on the flag control the display of the final in negative.
    /**
     *  @return true if the final image is displayed in negative,
     *          false it is not
     */
    //--------------------------------------------------------------------------
    bool getNegativeFilteringFlag() const;


    //--------------------------------------------------------------------------
    /// Output the talbe of attenuation coefficients of water into a data stream
    /**
     *  @param anOutputStream:  the output data stream
     */
    //--------------------------------------------------------------------------
    void printMuWaterTable(std::ostream& anOutputStream);


    //--------------------------------------------------------------------------
    /// Accessor on the number of inner surfaces
    /// (e.g. an internal organ in case of a medical application).
    /**
     *  @return the number of inner surfaces
     */
    //--------------------------------------------------------------------------
    unsigned int getNumberOfInnerSurfaces() const;


    //--------------------------------------------------------------------------
    /// Accessor on the number of outer surfaces
    /// (e.g. the skin surface in case of a medical application).
    /// This value is either equal to 0 or 1.
    /**
     *  @return the number of outer surfaces
     */
    //--------------------------------------------------------------------------
    unsigned int getNumberOfOuterSurfaces() const;


    //--------------------------------------------------------------------------
    /// Display a given inner surface (e.g. an internal organ in case of
    /// a medical application).
    /**
     *  @param anIndex: the index of the inner surface to display
     */
    //--------------------------------------------------------------------------
    void displayInnerSurface(unsigned int anIndex) const;


    //--------------------------------------------------------------------------
    /// Display the outer surface (e.g. the skin surface in case of
    /// a medical application).
    //--------------------------------------------------------------------------
    void displayOuterSurface() const;


    //--------------------------------------------------------------------------
    /// Accessor on the smallest pixel intensity in
    /// the latest simulated X-ray image.
    /**
     *  @return the min value in simulated X-ray image
     */
    //--------------------------------------------------------------------------
    RATIONAL_NUMBER getMinPixelValue() const;


    //--------------------------------------------------------------------------
    /// Accessor on the greatest pixel intensity in the latest simulated
    /// X-ray image.
    /**
     *  @return the max value in simulated X-ray image
     */
    //--------------------------------------------------------------------------
    RATIONAL_NUMBER getMaxPixelValue() const;


    void computeSinogram(MATRIX4& aModellingTransformationMatrix,
            VEC3 aRotationCenter = VEC3(),
            unsigned int aNumberOfAngle = 180,
            RATIONAL_NUMBER anAngleOffset = 1.0);


    void printSinogram(const char* aFileName, bool useCompression = false);
    void printSinogram(const std::string& aFileName, bool useCompression = false);


    const Sinogram<XRayRenderer::PixelType>& getSinogram() const;
    Sinogram<XRayRenderer::PixelType>& getSinogram();


    const PixelType* getSinogramRawData() const;


    void computeProjectionSet(MATRIX4& aModellingTransformationMatrix,
            VEC3 aRotationCenter = VEC3(),
            unsigned int aNumberOfAngle = 180,
            RATIONAL_NUMBER anAngleOffset = 1.0);



    //--------------------------------------------------------------------------
    /// Compute the X-ray projections for a CT acquisition.
    /**
     *  @param aProjectionOutputPath:   the path where the X-ray projections will be saved. 
     *                                  If the path is empty, the data will be stored in the main memory, but not saved on the disk.
     *                                  If the path is provided, the data will be saved on the disk, and the main memory released.
     *  @param aScreenshotOutputPath:   the path where the screenshots will be saved.
     *                                  If kept empty, not screenshot will be saved.
     *  @param aNumberOfProjections:    The total number of projections to simulate.
     *  @param aFirstAngle:             The rotation angle corresponding to the first projection.
     *  @param anIncludeLastAngleFlag:  A boolean flag to include or exclude the last angle. It is used to calculate the angular step between successive projections.
     *  @param aLastAngle:              The rotation angle corresponding to the last projection. Note that depending on the value of anIncludeLastAngleFlag, this angle may be included or excluded
     *  @param aNumberOfWhiteImagesInFlatField: The number of white images used to perform the flat-field correction. If zero, then no correction will be performed.
     *  @param aPositionOfCentreOfRotation: The location of the rotation centre,
     *  @param aAxisOfRotation: The rotation axis.
     *  @param anIntegrateEnergyFlag: if true the energy fluence is returned,
     *                                otherwise the number of photons is returned
     *                                (default value: true)
     *  @param aWhiteValue: the value of white when flat-field correction is applied.
     *                      If it is equal to zero, images will be saved as floating-point numbers.
     *                      If it is greater than zero, images will be saved as unsigned short numbers.
     *                      (default value = 0)
     *  @param aVerboseLevel:   The verbose level. If greater than zero, details will be outputed (default value = 0)
     */
    //--------------------------------------------------------------------------
    void computeCTAcquisition(const std::string& aProjectionOutputPath,
            const std::string& aScreenshotOutputPath,
            unsigned int aNumberOfProjections,
            const RATIONAL_NUMBER& aFirstAngle,
            bool anIncludeLastAngleFlag,
            const RATIONAL_NUMBER& aLastAngle,
            unsigned int aNumberOfWhiteImagesInFlatField,
            const VEC3& aPositionOfCentreOfRotation,
            const VEC3& aAxisOfRotation,
            const MATRIX4& aModellingTransformationMatrix = MATRIX4(),
            bool anIntegrateEnergyFlag = true,
            unsigned short aWhiteValue = 0,
            unsigned int aVerboseLevel = 0);
    

    //--------------------------------------------------------------------------
    /// Compute the X-ray projections with detector gain for a CT acquisition.
    /**
     *  @param aProjectionOutputPath:   the path where the X-ray projections will be saved.
     *                                  If the path is empty, the data will be stored in the main memory, but not saved on the disk.
     *                                  If the path is provided, the data will be saved on the disk, and the main memory released.
     *  @param aScreenshotOutputPath:   the path where the screenshots will be saved.
     *                                  If kept empty, not screenshot will be saved.
     *  @param aNumberOfProjections:    The total number of projections to simulate.
     *  @param aFirstAngle:             The rotation angle corresponding to the first projection.
     *  @param anIncludeLastAngleFlag:  A boolean flag to include or exclude the last angle. It is used to calculate the angular step between successive projections.
     *  @param aLastAngle:              The rotation angle corresponding to the last projection. Note that depending on the value of anIncludeLastAngleFlag, this angle may be included or excluded
     *  @param aNumberOfWhiteImagesInFlatField: The number of white images used to perform the flat-field correction. If zero, then no correction will be performed.
     *  @param aPositionOfCentreOfRotation: The location of the rotation centre,
     *  @param aAxisOfRotation: The rotation axis.
     *  @param anIntegrateEnergyFlag: if true the energy fluence is returned,
     *                                otherwise the number of photons is returned
     *                                (default value: true)
     *  @param aWhiteValue: the value of white when flat-field correction is applied.
     *                      If it is equal to zero, images will be saved as floating-point numbers.
     *                      If it is greater than zero, images will be saved as unsigned short numbers.
     *                      (default value = 60000)
     *  @param aVerboseLevel:   The verbose level. If greater than zero, details will be outputed (default value = 0)
     */
    //--------------------------------------------------------------------------
    void computeCTAcquisitionWithGain(const std::string& aProjectionOutputPath,
            const std::string& aScreenshotOutputPath,
            unsigned int aNumberOfProjections,
            const RATIONAL_NUMBER& aFirstAngle,
            bool anIncludeLastAngleFlag,
            const RATIONAL_NUMBER& aLastAngle,
            unsigned int aNumberOfWhiteImagesInFlatField,
            const VEC3& aPositionOfCentreOfRotation,
            const VEC3& aAxisOfRotation,
            const MATRIX4& aModellingTransformationMatrix = MATRIX4(),
            bool anIntegrateEnergyFlag = true,
            unsigned short aWhiteValue = 60000,
            unsigned int aVerboseLevel = 0);


    //--------------------------------------------------------------------------
    /// Accessor on the path where X-ray projections are saved during a CT scan 
    /// acquisition simulation.
    /**
     *  @return the path where X-ray projections are saved
     */
    //--------------------------------------------------------------------------
    const std::string& getProjectionOutputPathCT() const;


    //--------------------------------------------------------------------------
    /// Accessor on the path where screenshots are saved during a CT scan 
    /// acquisition simulation.
    /**
     *  @return the path where screenshots are saved
     */
    //--------------------------------------------------------------------------
    const std::string& getScreenshotOutputPathCT() const;


    //--------------------------------------------------------------------------
    /// Accessor on the number of X-ray projections computed during a CT scan 
    /// acquisition simulation.
    /**
     *  @return the number of X-ray projections
     */
    //--------------------------------------------------------------------------
    unsigned int getNumberOfProjectionsCT() const;


    //--------------------------------------------------------------------------
    /// Compute the field of view (FOV) of a CT scan.
    /**
     *  @return the FOV
     */
    //--------------------------------------------------------------------------
    RATIONAL_NUMBER getFOV() const;


    RATIONAL_NUMBER getSourceDetectorDistance() const;
    RATIONAL_NUMBER getSDD() const;
    RATIONAL_NUMBER getObjectDetectorDistance() const;
    RATIONAL_NUMBER getODD() const;
    RATIONAL_NUMBER getSourceObjectDistance() const;
    RATIONAL_NUMBER getSOD() const;


    //--------------------------------------------------------------------------
    /// Accessor on the number of X-ray projections needed to compute a CT scan 
    /// acquisition that satisfies the Nyquist theorem. Note that the user 
    /// may choose a different number.
    /**
     *  @return the optimal number of X-ray projections
     */
    //--------------------------------------------------------------------------
    unsigned int getOptimalNumberOfProjectionsCT() const;


    //--------------------------------------------------------------------------
    /// Accessor on the first angle of the CT scan acquisition simulation.
    /**
     *  @return the first angle
     */
    //--------------------------------------------------------------------------
    RATIONAL_NUMBER getFirstAngleCT() const;


    //--------------------------------------------------------------------------
    /// Accessor on the last angle of the CT scan acquisition simulation.
    /**
     *  @return the last angle
     */
    //--------------------------------------------------------------------------
    RATIONAL_NUMBER getLastAngleCT() const;


    //--------------------------------------------------------------------------
    /// Accessor on the flag used to include the last angle or not during 
    /// the CT scan acquisition simulation.
    /**
     *  @return the flag used to include the last angle or not
     */
    //--------------------------------------------------------------------------
    bool getIncludeLastAngleFlagCT() const;


    //--------------------------------------------------------------------------
    /// Accessor on the number of white images used in the flat-field correction
    /// during the CT scan acquisition simulation. Zero means that 
    /// the flat-field correction was not used.
    /**
     *  @return the number of white images
     */
    //--------------------------------------------------------------------------
    unsigned int getWhiteImagesInFlatFieldCT() const;


    //--------------------------------------------------------------------------
    /// Accessor on the position of the centre of rotation used during 
    /// the CT scan acquisition simulation.
    /**
     *  @return the position of the centre of rotation
     */
    //--------------------------------------------------------------------------
    const VEC3& getCentreOfRotationPositionCT() const;


    //--------------------------------------------------------------------------
    /// Accessor on the axis of rotation used during the CT scan 
    /// acquisition simulation.
    /**
     *  @return the axis of rotation
     */
    //--------------------------------------------------------------------------
    const VEC3& getRotationAxisCT() const;


    //--------------------------------------------------------------------------
    /// Accessor on the set of rotation angles used during the CT scan 
    /// acquisition simulation.
    /**
     *  @return the set of rotation angles
     */
    //--------------------------------------------------------------------------
    const std::vector<RATIONAL_NUMBER>& getAngleSetCT() const;


    void printProjectionSet(const char* aFileName, bool useCompression = false);
    void printProjectionSet(const std::string& aFileName, bool useCompression = false);


    const Image<XRayRenderer::PixelType>& getProjectionSet() const;
    Image<XRayRenderer::PixelType>& getProjectionSet();


    const PixelType* getProjectionSetRawData();


    // const Image<XRayRenderer::PixelType>& getEnergyFluence() const;
    Image<XRayRenderer::PixelType>& getEnergyFluence();


    Image<unsigned short>& getEnergyFluenceWithGain();


    const PixelType* getEnergyFluenceRawData();


    const Image<XRayRenderer::PixelType>& getLBuffer() const;
    Image<XRayRenderer::PixelType>& getLBuffer();


    const Image<XRayRenderer::PixelType>& getCumulatedLBuffer() const;
    Image<XRayRenderer::PixelType>& getCumulatedLBuffer();


    const PixelType* getLBufferRawData();

    //--------------------------------------------------------------------------
    /// Returns the actual Beam
    /**
    *  @return a const pointer to the actual Beam
    */
    //--------------------------------------------------------------------------
    const XRayBeam* getXRayBeam( ) const;


    //--------------------------------------------------------------------------
    /// Enable artefact filtering on GPU.
    //--------------------------------------------------------------------------
    void enableArtefactFilteringOnGPU();


    //--------------------------------------------------------------------------
    /// Enable artefact filtering on CPU.
    //--------------------------------------------------------------------------
    void enableArtefactFilteringOnCPU();


    //--------------------------------------------------------------------------
    /// Enable artefact filtering on GPU.
    //--------------------------------------------------------------------------
    void enableArtifactFilteringOnGPU();


    //--------------------------------------------------------------------------
    /// Enable artefact filtering on CPU.
    //--------------------------------------------------------------------------
    void enableArtifactFilteringOnCPU();


    //--------------------------------------------------------------------------
    /// Disable artefact filtering on GPU.
    //--------------------------------------------------------------------------
    void disableArtefactFilteringOnGPU();


    //--------------------------------------------------------------------------
    /// Disable artefact filtering on CPU.
    //--------------------------------------------------------------------------
    void disableArtefactFilteringOnCPU();


    //--------------------------------------------------------------------------
    /// Disable artefact filtering on GPU.
    //--------------------------------------------------------------------------
    void disableArtifactFilteringOnGPU();


    //--------------------------------------------------------------------------
    /// Disable artefact filtering on CPU.
    //--------------------------------------------------------------------------
    void disableArtifactFilteringOnCPU();


    void useArtifactFilteringOnGPU(bool aFlag);
    void useArtifactFilteringOnCPU(bool aFlag);

    void useArtefactFilteringOnGPU(bool aFlag);
    void useArtefactFilteringOnCPU(bool aFlag);

    bool useArtifactFilteringOnGPU() const;
    bool useArtifactFilteringOnCPU() const;

    bool useArtefactFilteringOnGPU() const;
    bool useArtefactFilteringOnCPU() const;

    //--------------------------------------------------------------------------
    /// Accessor on the max radius of the L-buffer artefact correction filter
    /**
     *  @return the radius in number of pixels
     */
    //--------------------------------------------------------------------------
    unsigned int getMaxRadiusLbufferArtefactFilter() const;


    //--------------------------------------------------------------------------
    /// Set the max radius of the L-buffer artefact correction filter
    /**
     *  @param aRadius: the radius in number of pixels
     */
    //--------------------------------------------------------------------------
    void setMaxRadiusLbufferArtefactFilter(unsigned int aRadius);


	//--------------------------------------------------------------------------
	/// If a surface changes material, it is needed update surfacepermaterial list
	//--------------------------------------------------------------------------
	void needUpdateSurfacePerMaterial();


    //--------------------------------------------------------------------------
    /// Compute the white image (X-ray image without the object).
    /**
     *  @param anIntegrateEnergyFlag: if true the energy fluence is returned,
     *                                otherwise the number of photons is returned
     *                                (default value: true)
     */
    //--------------------------------------------------------------------------
    Image<float> getWhiteImage(bool anIntegrateEnergyFlag = true);


    //--------------------------------------------------------------------------
    /// Compute the white image (X-ray image without the object) when the detector gain is applied.
    /**
     *  @param anIntegrateEnergyFlag: if true the energy fluence is returned,
     *                                otherwise the number of photons is returned
     *                                (default value: true)
     */
    //--------------------------------------------------------------------------
    Image<unsigned short> getWhiteImageWithGain(bool anIntegrateEnergyFlag = true);


    const std::vector<VEC3>& getSourcePositionSet() const;
    const std::vector<VEC3>& getDetectorPositionSet() const;
    const std::vector<VEC3>& getDetectorRightVectorSet() const;
    const std::vector<VEC3>& getDetectorUpVectorSet() const;


//******************************************************************************
protected:
    //--------------------------------------------------------------------------
    /// Compute the X-ray image.
    /**
     *  @param aModellingTransformationMatrix:  a modelling transformation
     *                                          matrix that will be used on all
     *                                          the 3D objects
     *  @param anIntegrateEnergyFlag: if true the energy fluence is returned,
     *                                otherwise the number of photons is returned
     *                                (default value: true)
     */
    //--------------------------------------------------------------------------
    void computeImageUsingOpenGL(const MATRIX4& aModellingTransformationMatrix,
        bool anIntegrateEnergyFlag = true);


        //--------------------------------------------------------------------------
        /// Compute the X-ray image.
        /**
         *  @param aModellingTransformationMatrix:  a modelling transformation
         *                                          matrix that will be used on all
         *                                          the 3D objects
         *  @param anIntegrateEnergyFlag: if true the energy fluence is returned,
         *                                otherwise the number of photons is returned
         *                                (default value: true)
         */
        //--------------------------------------------------------------------------
        void computeImageUsingOpenMP(const MATRIX4& aModellingTransformationMatrix,
            bool anIntegrateEnergyFlag = true);


    //--------------------------------------------------------------------------
    /// Load the table of water attenuation coefficients.
    //--------------------------------------------------------------------------
    static void loadMuWater();


    //--------------------------------------------------------------------------
    /// Check that the device supports all the required OpenGL extensions
    /// that are needed.
    //--------------------------------------------------------------------------
    void checkGPUCapabilities();


    //--------------------------------------------------------------------------
    /// Initialise the shaders.
    //--------------------------------------------------------------------------
    void initShaders();


    //--------------------------------------------------------------------------
    /// Create the FBOs.
    //--------------------------------------------------------------------------
    void createFBOs();


    //--------------------------------------------------------------------------
    /// Delete the FBOs.
    //--------------------------------------------------------------------------
    void deleteFBOs();


    //--------------------------------------------------------------------------
    /// Create a PBO.
    /**
     *  @param aPBO:    ID of the PBO to create
     */
    //--------------------------------------------------------------------------
    void createPBO(unsigned int& aPBO);


    //--------------------------------------------------------------------------
    /// Delete a PBO.
    /**
     *  @param aPBO:    ID of the PBO to delete
     */
    //--------------------------------------------------------------------------
    void deletePBO(unsigned int& aPBO);


    //--------------------------------------------------------------------------
    /// Initialise the PBOs.
    //--------------------------------------------------------------------------
    void initialisePBOs();


    //--------------------------------------------------------------------------
    /// Initialise the 2D textures.
    //--------------------------------------------------------------------------
    void init2DTextures();


    //--------------------------------------------------------------------------
    /// Initialise the 3D textures.
    //--------------------------------------------------------------------------
    void init3DData();


    //--------------------------------------------------------------------------
    /// Initialise a FBO (e.g. attach textures, etc.).
    /**
     *  @param aFBOIndex:       the FBO to initialise
     *  @param aColourTextureIndex:   the colour texture to attach
     *  @param aZBufferTextureIndex:   the Z-buffer texture (optional) to attach (default value: 0)
     */
    //--------------------------------------------------------------------------
    void init2DFBO(unsigned int aFBOIndex,
        unsigned int aColourTextureIndex,
        unsigned int aZBufferTextureIndex = 0);


    //--------------------------------------------------------------------------
    /// Initialise a FBO with a 3D texture.
    //--------------------------------------------------------------------------
    void init3DFBOs();


    //--------------------------------------------------------------------------
    /// Create a 2D texture.
    /**
     *  @param apTextureID:             the address on the ID of the texture
     *  @param anInternalTextureFormat: the internal format in which the texture
     *                                  will be stored in the device
     *  @param aType:   the type of texls, e.g. GL_RGBA, GL_DEPTH_COMPONENT, etc.
     */
    //--------------------------------------------------------------------------
    void create2DTexture(unsigned int* apTextureID,
                         int anInternalTextureFormat,
						 int aType);


    //--------------------------------------------------------------------------
    /// Reset the value of a FBO to 0.
    /**
     *  @param aFrameBuffer:    the FBO to reset
     */
    //--------------------------------------------------------------------------
    void reset2DBuffer(unsigned int aFrameBuffer);


    //--------------------------------------------------------------------------
    /// Reset the L-buffer FBO to 0.
    //--------------------------------------------------------------------------
    void resetLBuffer();


    //--------------------------------------------------------------------------
    /// Reset the energy fluence FBO to 0.
    //--------------------------------------------------------------------------
    void resetEnergyFluence();


    //--------------------------------------------------------------------------
    /// Reset the Mu_x * D_x FBO to 0.
    //--------------------------------------------------------------------------
    void resetSumMuxDx();


    //--------------------------------------------------------------------------
    /// Compute the L-buffer of an internal object.
    /**
     *  @param anObject:                        the 3D object to process
     *  @param aXRaySourceSampleOffset:         the translation vector to use
     *                                          when the source is not aligned
     *                                          with the detector
     *  @param aModellingTransformationMatrix:  the transformation matrix of
     *                                          the 3D object
     *  @param aGPUStateConservationFlag:   if true, then restore the OpenGL 
     *                                      states as they were before 
     *                                      the method was called. If false,
     *                                      leave them as they are. It is useful
     *                                      to leave them when the method is 
     *                                      called repetitively, i.e. to save time.
     */
    //--------------------------------------------------------------------------
    void computeInnerSurfaceLBuffer(PolygonMesh& anObject,
            const VEC3& aXRaySourceSampleOffset,
            const MATRIX4& aModellingTransformationMatrix,
            bool aGPUStateConservationFlag);

    //--------------------------------------------------------------------------
    /// Compute the L-buffer of an external object.
    /**
     *  @param anObject:                        the 3D object to process
     *  @param aXRaySourceSampleOffset:         the translation vector to use
     *                                          when the source is not aligned
     *                                          with the detector
     *  @param aModellingTransformationMatrix:  the transformation matrix of
     *                                          the 3D object
     */
    //--------------------------------------------------------------------------
    void computeOuterSurfaceLBuffer(PolygonMesh& anObject,
            const VEC3& aXRaySourceSampleOffset,
            const MATRIX4& aModellingTransformationMatrix);


    //--------------------------------------------------------------------------
    /// Update the L-buffer of inner surfaces.
    /// It integrates the L-buffers of all the inner surfaces.
    /**
     *  @param anObjectID:  the ID of the surface to integrate. If anObjectID is
     *                      equal to zero, the cumulated L-buffer is
     *                      re-initialsed with the L-buffer of this surface.
     *                      If anObjectID is greater than zero, L-buffer of this
     *                      surface is integrated into the cumulated L-buffer.
     */
    //--------------------------------------------------------------------------
    void updateCumulatedLBuffers(unsigned int anObjectID);


    //--------------------------------------------------------------------------
    /// Integrate \f$\mu(anObjectID) \times \mathrm{d}x\f$ for a given object (\p anObjectID) 
    /// in \f$\sum \mu \times \mathrm{d}x\f$.
    /**
     *  @param anObjectID:  the ID of the surface to integrate.
     *  @param aPhotonCrossSection: the corresponding material composition.
     */
    //--------------------------------------------------------------------------
    void computeSumMUxDx(int anObjectID,
                         const PhotonCrossSection& aPhotonCrossSection);


    //--------------------------------------------------------------------------
    /// Compute the energy fluence.
    /**
     *  @param anIntegrateEnergyFlag: if true the energy fluence is returned,
     *                                otherwise the number of photons is returned
     */
    //--------------------------------------------------------------------------
    void computeIntegratedEnergy(bool anIntegrateEnergyFlag);


    //--------------------------------------------------------------------------
    /// Clean the L-buffer, i.e. remove rendering artefacts.
    /**
     *  @param aCutoffDistance: the cut-off distance. It is used to handle 
     *                          numerical inacuracies. If the L-buffer pixel 
     *                          value is less than aCutoffDistance, we leave it 
     *                          as it is.
     */
    //--------------------------------------------------------------------------
    void cleanLBuffer(const RATIONAL_NUMBER& aCutoffDistance);


    //--------------------------------------------------------------------------
    /// Update the VBO of the geometry.
    //--------------------------------------------------------------------------
    void updateVBO();


    void preComputeLBuffer(GLint* apViewport, const VEC3& aXRaySourceSampleOffset);
    void postComputeLBuffer(const GLint* apViewport, RATIONAL_NUMBER aDiagonal);

    void updateSurfacePerMaterialSet();


    //--------------------------------------------------------------------------
    /// Use the number of photons to add the Poisson noise.
    /**
     *  @param anImage:  Image to alter.
     *  @param aCutoffValue:    the value of lambda in a Poisson distribution 
     *                          where a Gaussian distribution is a good enough
     *                          approximation of the Poisson distribution
     *                          (default value: 50)
     */
    //--------------------------------------------------------------------------
    void addPoissonNoise(Image<PixelType>& anImage);


    std::pair<VEC3, VEC3> getSampleBoundingBox() const;
    VEC3 getSampleBoundingBoxCentre() const;


    /// The X-ray detector
    XRayDetector* m_p_detector;


    /// The X-ray beam
    XRayBeam* m_p_xray_beam;


    /// Set of 3D geometries
    std::vector<SceneGraphNode*> m_p_inner_surface_set;


    /// The 3D geometry that will wrap the ones in m_p_inner_surface_set
    SceneGraphNode* m_p_outer_surface;


    std::vector<SceneGraphNode*> m_p_internal_data_storage;


    std::vector<std::vector<SceneGraphNode*> > m_p_surface_per_material_set;
    bool m_surface_per_material_set_needs_update;


    /// The computing mode (OPENGL, CUDA, or OPENCL)
    ComputingMode m_computing_mode;


    /// Set of PBOs
    std::vector<unsigned int>        m_p_pbo_set;


    /// Set of textures
    std::vector<unsigned int> m_p_texture_name_set;


    /// Set of shaders
    std::vector<Shader> m_shader_set;


    /// The raw data of the last FBO that was loaded in the main memory
    Image<PixelType> m_temp_raw_data;


    /// The table of water energy fluence coefficients
    static std::vector<AttenuationCoefficient> m_mu_water_set;


    /// The set of FBOs
    std::vector<FramebufferObject*> m_p_fbo_set;


    /// L-buffer
    Image<PixelType> m_l_buffer_image;


    /// Cumulated L-buffer for all the inner surfaces
    Image<PixelType> m_cumulated_lbuffer_image;


    /// Sum(Mu_x * d_x)
    Image<PixelType> m_sum_mux_dx_image;


    /// Energy fluence
    Image<PixelType> m_energy_fluence_image;


    /// Energy fluence with gain
    Image<unsigned short> m_energy_fluence_with_gain_image;


    /// Sinogram
    Sinogram<PixelType> m_sinogram_image;


    /// Projection set
    Image<PixelType> m_projection_set;


    /// Min value in simulated X-ray image
    RATIONAL_NUMBER m_min_value_in_simulated_xray_image;


    /// Max value in simulated X-ray image
    RATIONAL_NUMBER m_max_value_in_simulated_xray_image;


    /// Flag to display the result in negative or not
    unsigned int m_negative_display_flag;


    /// Flag set to true to use L-buffer artefact filtering
    bool m_use_l_buffer_artefact_filtering_on_gpu;
    bool m_use_l_buffer_artefact_filtering_on_cpu;


    /// Max radius of L-buffer artefact filter
    unsigned int m_max_radius_Lbuffer_artefact_filter;


    /// Flag use for lazy evaluation of the L-Buffer
    bool m_l_buffer_empty;


    /// Flag use for lazy evaluation and blending of the energy fluence
    bool m_energy_fluence_empty;


    /// The VBO used to display a rectangle corresponding to the detector
#if __cplusplus < 199711L
    std::auto_ptr<VBO> m_detector_temporary_geometry;
#else
    std::unique_ptr<VBO> m_detector_temporary_geometry;
#endif

    unsigned int m_zbuffer_texture_id;

    uint8_t m_mixed_precision_flag;


    std::default_random_engine* m_p_random_number_generator;
    std::uniform_real_distribution<>* m_p_uniform_distribution;
    std::normal_distribution<>* m_p_normal_distribution;

    bool m_use_photon_count_detector;

    std::string m_CT_projection_output_path;
    std::string m_CT_screenshot_output_path;

    RATIONAL_NUMBER m_CT_first_angle;
    bool m_CT_include_last_angle_flag;
    RATIONAL_NUMBER m_CT_last_angle;

    unsigned int m_CT_white_images_in_flat_field;

    VEC3 m_CT_centre_of_rotation_position;
    VEC3 m_CT_rotation_axis;

    std::vector<VEC3> m_source_positions_set;
    std::vector<VEC3> m_detector_positions_set;
    std::vector<VEC3> m_detector_right_vector_set;
    std::vector<VEC3> m_detector_up_vector_set;

    std::vector<RATIONAL_NUMBER> m_CT_angle_set;

    static unsigned int m_counter;

#ifdef HAS_BOOST
    boost::interprocess::shared_memory_object m_shared_memory_obj;
    boost::interprocess::mapped_region m_mapped_region;
    std::string m_shared_memory_name;
#endif
};


} // namespace gVirtualXRay


//******************************************************************************
#include "XRayRenderer.inl"


#endif // GVXR_XRAY_RENDERER_H
