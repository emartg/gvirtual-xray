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


#ifndef GVXR_XRAY_DETECTOR_H
#define GVXR_XRAY_DETECTOR_H


/**
********************************************************************************
*
*   @file       XRayDetector.h
*
*   @brief      Class to handle virtual X-Ray detectors.
*
*   @version    1.0
*
*   @date       13/01/2017
*
*   @author     Dr Franck P. Vidal
*
*   @todo       Create a class to draw spheres
*               Use this class to display point sources
*               (see line 80 in file XRayDetector.inl)
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

#include <memory>
#include <vector> // To store the energy response
#include <utility> // To create STL pairs

#ifndef GVXR_TYPES_H
#include "gVirtualXRay/Types.h"
#endif

#ifndef GVXR_MATRIX_4X4_H
#include "gVirtualXRay/Matrix4x4.h"
#endif

#ifndef GVXR_VBO_h
#include "gVirtualXRay/VBO.h"
#endif

#ifndef GVXR_SPHERE_MESH_H
#include "gVirtualXRay/SphereMesh.h"
#endif

#ifndef GVXR_CUBE_MESH_H
#include "gVirtualXRay/CubeMesh.h"
#endif

#ifndef GVXR_SCINTILLATOR_H
#include "gVirtualXRay/Scintillator.h"
#endif

#ifndef GVXR_IMAGE_H
#include "gVirtualXRay/Image.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


//==============================================================================
/**
*   @class  XRayDetector
*   @brief  XRayDetector is a class to handle a X-ray detector.
*/
//==============================================================================
class XRayDetector
//------------------------------------------------------------------------------
{
//******************************************************************************
public:
    /// SourceShape is a type that defines the shape of X-ray sources.
    typedef enum SourceShapeTag
    {
        POINT,        ///< Infinitively small point source
        SQUARE,       ///< Source whose shape corresponds to a square
        RECTANGLE,    ///< Source whose shape corresponds to a rectangle
        CUBE,         ///< Source whose shape corresponds to a cube
        LINE,         ///< Source whose shape corresponds to a line segment
        PARALLEL,     ///< Parallel source,
                      ///< i.e. a point source infinitely far away
        UNKNOWN_SHAPE ///< Unknown type
    } SourceShape;


    //--------------------------------------------------------------------------
    /// Default constructor.
    //--------------------------------------------------------------------------
    XRayDetector();


    //--------------------------------------------------------------------------
    /// Destructor.
    //--------------------------------------------------------------------------
    ~XRayDetector();


    //--------------------------------------------------------------------------
    /// Release the data.
    //--------------------------------------------------------------------------
        void release();


    //--------------------------------------------------------------------------
    /// Display the detector using OpenGL.
    //--------------------------------------------------------------------------
    void displayDetector();


    void setWidgetLength(const RATIONAL_NUMBER& aLength);


    //--------------------------------------------------------------------------
    /// Display the source using OpenGL.
    //--------------------------------------------------------------------------
    void displaySource();


    //--------------------------------------------------------------------------
    /// Display the source using OpenGL.
    //--------------------------------------------------------------------------
    void displaySourceWireframe();


    //--------------------------------------------------------------------------
    /// Display the beam using OpenGL.
    //--------------------------------------------------------------------------
    void displayBeam();


    //--------------------------------------------------------------------------
    /// Display a sphere on the position of the first pixel of the image using OpenGL.
    //--------------------------------------------------------------------------
    void displayFirstPixelPosition();


    //--------------------------------------------------------------------------
    /// Display a sphere on the position of the centre of the detecotr using OpenGL.
    //--------------------------------------------------------------------------
    void displayDetectorPosition();


    //--------------------------------------------------------------------------
    /// Set the pixel resolution (in units of length per pixel,
    /// i.e. the inverse of the pixel size)
    /**
    *   @param aResolution: the pixel resolution
    */
    //--------------------------------------------------------------------------
    void setResolutionInUnitsOfLengthPerPixel(
            const RATIONAL_NUMBER& aResolution);


    //--------------------------------------------------------------------------
    /// Set the pixel resolution (in units of length per pixel,
    /// i.e. the inverse of the pixel size)
    /**
    *   @param aResolution: the pixel resolution
    */
    //--------------------------------------------------------------------------
    void setResolutionInUnitsOfLengthPerPixel(const VEC2& aResolution);


    //--------------------------------------------------------------------------
    /// Set the number of pixels.
    /**
     *  @param aSizeInPixels:   the number of pixels
     */
    //--------------------------------------------------------------------------
    void setNumberOfPixels(const Vec2ui& aSizeInPixels);


    //--------------------------------------------------------------------------
    /// Set the position of the X-ray detector.
    /**
     *  @param aDetectorPosition:   the position of the X-ray detector
     */
    //--------------------------------------------------------------------------
    void setDetectorPosition(const VEC3& aDetectorPosition);


    //--------------------------------------------------------------------------
    /// Set the position of the X-ray source.
    /**
     *  @param aXraySourcePosition: the position of the X-ray source
     */
    //--------------------------------------------------------------------------
    void setXrayPointSource(const VEC3& aXraySourcePosition);


    //--------------------------------------------------------------------------
    /// Set the shape of the source as a cube.
    /**
     *  @param aXraySourcePosition:     the position of the centre of the source
     *  @param aNumberOfSamplesAlongOneDirection:   the number of point sources
     *                      along one axis (in total  there will be
     *                      aNumberOfSamplesAlongOneDirection^3 point sources
     *                      in the cube)
     *  @param aLength: the length of the cube
     */
    //--------------------------------------------------------------------------
    void setCubicSource(const VEC3& aXraySourcePosition,
                        unsigned int aNumberOfSamplesAlongOneDirection,
                        RATIONAL_NUMBER aLength);


    //--------------------------------------------------------------------------
    /// Set the shape of the source as a square (in total there will be
    /// aNumberOfSamplesAlongOneDirection^2 point sources in the square).
    /**
     *  @param aXraySourcePosition:     the position of the centre of the source
     *  @param aNumberOfSamplesAlongOneDirection:   the number of point sources
     *                          along one axis
     *  @param aLength: the length of the square
     */
    //--------------------------------------------------------------------------
    void setSquareSource(const VEC3& aXraySourcePosition,
                         unsigned int aNumberOfSamplesAlongOneDirection,
                         RATIONAL_NUMBER aLength);


    //--------------------------------------------------------------------------
    /// Set the shape of the source as a rectangle (in total there will be
    /// aNumberOfSamplesAlongLength * aNumberOfSamplesAlongHeight point sources 
    /// in the rectangle).
    /**
     *  @param aXraySourcePosition:     the position of the centre of the source
     *  @param aNumberOfSamplesAlongLength: the number of point sources along 
     *                                      the axis defined by the cross 
     *                                      product between the source-detector 
     *                                      centres and the detector up-vector.
     *  @param aNumberOfSamplesAlongHeight: the number of point sources along 
     *                                      the detector up-vector direction.
     *  @param aLongitudinalLength: the length of the rectangle along the axis
     *                              defined by the cross product between 
     *                              the source-detector centres and 
     *                              the detector up-vector.
     *  @param aHeight: the height of the source along the detector up-vector 
     *                  direction.
     */
    //--------------------------------------------------------------------------
    void setRectangleSource(const VEC3& aXraySourcePosition,
                                        unsigned int aNumberOfSamplesAlongLength,
                                        unsigned int aNumberOfSamplesAlongHeight,
                                        RATIONAL_NUMBER aLongitudinalLength,
                                        RATIONAL_NUMBER aHeight);


    //--------------------------------------------------------------------------
    /// Set the shape of the source as a line segment.
    /**
     *  @param aXraySourcePosition: the position of the centre of the source
     *  @param aDirection:  the direction vector of the line
     *  @param aNumberOfSamplesAlongOneDirection:   the number of point sources
     *  @param aLength: the length of the line
     */
    //--------------------------------------------------------------------------
    void setLineSource(const VEC3& aXraySourcePosition,
                       const VEC3& aDirection,
                       unsigned int aNumberOfSamplesAlongOneDirection,
                       RATIONAL_NUMBER aLength);


    //--------------------------------------------------------------------------
    /// Set the shape of the source as an infinitely small point.
    //--------------------------------------------------------------------------
    void setXrayPointSource();


    //--------------------------------------------------------------------------
    /// Set the shape of the source as a cube.
    //--------------------------------------------------------------------------
    void setCubicSource();


    //--------------------------------------------------------------------------
    /// Set the shape of the source as a square.
    //--------------------------------------------------------------------------
    void setSquareSource();


    //--------------------------------------------------------------------------
    /// Set the shape of the source as a rectangle.
    //--------------------------------------------------------------------------
    void setRectangleSource();


    //--------------------------------------------------------------------------
    /// Set the shape of the source as a line segment.
    //--------------------------------------------------------------------------
    void setLineSource();


    //--------------------------------------------------------------------------
    /// Set the up vector that defines the detector's orientation.
    /**
     *  @param anUpVector:  the up vector
     */
    //--------------------------------------------------------------------------
    void setUpVector(const VEC3& anUpVector);


    //--------------------------------------------------------------------------
    /// Set the right vector that defines the detector's orientation.
    /**
     *  @param aRightVector:  the right vector
     */
    //--------------------------------------------------------------------------
    void setRightVector(const VEC3& aRightVector);


    void autoAlignDetector(bool aFlag = true);


    void rotateDetector(const RATIONAL_NUMBER& anAngle, const VEC3& aRotationAxis);
    

    //--------------------------------------------------------------------------
    /// Set the rotation matrix.
    /**
     *  @param aMatrix: the rotation matrix
     */
    //--------------------------------------------------------------------------
    void setRotationMatrix(const MATRIX4& aMatrix);


    //--------------------------------------------------------------------------
    /// Set the projection mode to parallel beam.
    //--------------------------------------------------------------------------
    void setParallelBeam();


    //--------------------------------------------------------------------------
    /// Set the projection mode to cone beam (same as point source).
    //--------------------------------------------------------------------------
    void setConeBeam();


    //--------------------------------------------------------------------------
    /// Set the projection mode to point source (same as cone beam).
    //--------------------------------------------------------------------------
    void setPointSource();


    //--------------------------------------------------------------------------
    /// Accessor on the number of pixels.
    /**
     *  @return the number of pixels
     */
    //--------------------------------------------------------------------------
    const Vec2ui& getNumberOfPixels() const;


    //--------------------------------------------------------------------------
    /// Accessor on the size of the detector (in unit of length).
    /**
     *  @return the size of the detector
     */
    //--------------------------------------------------------------------------
    const VEC2& getSizeInUnitOfLength() const;


    //--------------------------------------------------------------------------
    /// Accessor on the pixel spacing (in unit of length).
    /**
     *  @return the pixel spacing
     */
    //--------------------------------------------------------------------------
    const VEC2& getPixelSpacingInUnitOfLength() const;


    //--------------------------------------------------------------------------
    /// Accessor on the detector position.
    /**
     *  @return the detector position
     */
    //--------------------------------------------------------------------------
    const VEC3& getDetectorPosition() const;


    //--------------------------------------------------------------------------
    /// Accessor on the centre of the source.
    /**
     *  @return the source position
     */
    //--------------------------------------------------------------------------
    const VEC3& getXraySourceCentre() const;


    unsigned int getNumberOfSourceSamples()const;


    const VEC3& getSourceSamplePosition(unsigned int aSampleID) const;


    //--------------------------------------------------------------------------
    /// Accessor on the up vector, which defines the orientation of the detector.
    /**
     *  @return the up vector
     */
    //--------------------------------------------------------------------------
    const VEC3& getUpVector() const;


    //--------------------------------------------------------------------------
    /// Accessor on the rotation matrix.
    /**
     *  @return the rotation matrix
     */
    //--------------------------------------------------------------------------
    const MATRIX4& getRotationMatrix() const;


    //--------------------------------------------------------------------------
    /// Accessor on the vector normal to the detector.
    //--------------------------------------------------------------------------
    const VEC3& getNormal() const;


    //--------------------------------------------------------------------------
    /// Accessor on the cross product between m_look_at_vector and m_up_vector.
    /**
     *  @return the cross product
     */
    //--------------------------------------------------------------------------
    const VEC3& getRightVector() const;


    //--------------------------------------------------------------------------
    /// Accessor on the plane equation (Ax + By + Cz + D = 0).
    /**
     *  @param A: the 1st component of the plane equation
     *  @param B: the 2nd component of the plane equation
     *  @param C: the 3rd component of the plane equation
     *  @param D: the 4th component of the plane equation
     */
    //--------------------------------------------------------------------------
    void getPlaneEquation(RATIONAL_NUMBER& A,
                                RATIONAL_NUMBER& B,
                                RATIONAL_NUMBER& C,
                                RATIONAL_NUMBER& D);


    //------------------------------------------------------------------------------
    /// Accessor on the four corners of the X-ray detector.
    /**
     *  @return the corresponding vertices
     */
    //------------------------------------------------------------------------------
    const std::vector<float>& getVertices() const;


    //--------------------------------------------------------------------------
    /// Accessor on the shape of the source.
    /**
     *  @return true if a parallel beam is used (e.g. orthographic projection),
     *          false if perspective projection is used
     */
    //--------------------------------------------------------------------------
    SourceShape getSourceShape() const;


    const std::vector<VEC3>& getSourcePositionSet() const;


    void setSourcePositions(const std::vector<VEC3>& aSourcePostionSet);


    //--------------------------------------------------------------------------
    /// Rotate the focal spot around its centre.
    /**
     *  @param aRotationAngle: the rotation angle in degrees
     *  @param aRotationAxis: the rotaton axis
     */
    //--------------------------------------------------------------------------
    void rotateFocalSpot(const RATIONAL_NUMBER& aRotationAngle, const VEC3& aRotationAxis);


    //--------------------------------------------------------------------------
    /// Rotate the source around and arbitrary point.
    /**
     *  @param aRotationAngle: the rotation angle in degrees
     *  @param aRotationCentre: the rotaton centre
     *  @param aRotationAxis: the rotaton axis
     */
    //--------------------------------------------------------------------------
    void rotateSource(const RATIONAL_NUMBER& aRotationAngle, 
        const VEC3& aRotationCentre,
        const VEC3& aRotationAxis);


    //--------------------------------------------------------------------------
    /// Accessor on the flag about projection.
    /**
     *  @return true if a parallel beam is used (e.g. orthographic projection),
     *          false if perspective projection is used
     */
    //--------------------------------------------------------------------------
    bool useParallelBeam() const;


    //--------------------------------------------------------------------------
    /// Accessor on the view along the Y-axis.
    /**
     *  @return the view along the Y-axis
     */
    //--------------------------------------------------------------------------
    const RATIONAL_NUMBER& getFOVY() const;


    const VEC2& getPixelSizeInUnitOfLength() const;


    //--------------------------------------------------------------------------
    /// Clear the energy response of the detector.
    //--------------------------------------------------------------------------
    void clearEnergyResponse();


    //--------------------------------------------------------------------------
    /// Load the energy response of the detector from a TSV file.
    /**
     *  @param aFileName: name of the file. Each line of the file is formatted as follows:
     *                    input_energy  output_energy
     *  @param aUnitOfEnergy: the unit of energy corresponding to the data in the file
     */
    //--------------------------------------------------------------------------
    void loadEnergyResponse(const std::string& aFileName,
        const RATIONAL_NUMBER& aUnitOfEnergy);


    void setEnergyResponse(const std::vector<std::pair<float, float> >& anEnergyResponse);


    void setEnergyResponse(const std::vector<std::pair<double, double> >& anEnergyResponse);


    void setScintillator(const Scintillator& aScintillator);


    void setScintillator(const std::string& aMaterial,
        const float& aThickness);
    
    
    const std::string& getScintillatorMaterial();


    double getScintillatorThickness();

    std::vector<std::pair<RATIONAL_NUMBER, RATIONAL_NUMBER> > getEnergyResponse() const;
    

    //--------------------------------------------------------------------------
    /// Apply the energy response if any.
    /**
    *  @param anEnergy: the input energy
    *  @return the output energy
     */
    //--------------------------------------------------------------------------
    RATIONAL_NUMBER applyEnergyResponse(const RATIONAL_NUMBER& anEnergy) const;


    //--------------------------------------------------------------------------
    /// Set the line spread function from a file.
    /**
    *  @param aFileName: the input file name
    */
    //--------------------------------------------------------------------------
    void setLSF(const std::string& aFileName);


    //--------------------------------------------------------------------------
    /// Set the line spread function from an array.
    /**
    *  @param aLSF: the LSF coefficients
    */
    //--------------------------------------------------------------------------
    void setLSF(const std::vector<RATIONAL_NUMBER>& aLSF);


    //--------------------------------------------------------------------------
    /// Accessor on the line spread function.
    /**
    *  @return the LSF coefficients
    */
    //--------------------------------------------------------------------------
    const std::vector<RATIONAL_NUMBER>& getLSF() const;


    //--------------------------------------------------------------------------
    /// Clear the LSF to use a Dirac as impulse response.
    //--------------------------------------------------------------------------
    void clearLSF();


    //--------------------------------------------------------------------------
    /// Set the point spread function from a 2D image.
    /**
    *  @param aPSF: the PSF coefficients
    */
    //--------------------------------------------------------------------------
    void setPSF(const Image<RATIONAL_NUMBER>& aPSF);


    //--------------------------------------------------------------------------
    /// Accessor on the point spread function.
    /**
    *  @return the PSF coefficients as a 2D image
    */
    //--------------------------------------------------------------------------
    const Image<RATIONAL_NUMBER>& getPSF() const;


    //--------------------------------------------------------------------------
    /// Clear the PSF to use a Dirac as impulse response.
    //--------------------------------------------------------------------------
    void clearPSF();


    //--------------------------------------------------------------------------
    /// Convert a photon count per cm2 at 1m into per pixel at the source-detector distance.
    /**
    *  @param aPhotonCount: the number of photons to convert
    *  @return the converted number of photons
    */
    //--------------------------------------------------------------------------
    RATIONAL_NUMBER convertPerCm2At1m2PerPixelAtSDD(const RATIONAL_NUMBER& aPhotonCount) const;


    //--------------------------------------------------------------------------
    /// Convert a photon count per pixel at the source-detector distance into per cm2 at 1m.
    /**
    *  @param aPhotonCount: the number of photons to convert
    *  @return the converted number of photons
    */
    //--------------------------------------------------------------------------
    RATIONAL_NUMBER convertPerPixelAtSDD2PerCm2At1m(const RATIONAL_NUMBER& aPhotonCount) const;


    //------------------------------------------------------------------------------
    /// Accessor on the detector gain.
    /**
     *  @return the gain
     */
    //------------------------------------------------------------------------------
    RATIONAL_NUMBER getGain();


    //------------------------------------------------------------------------------
    /// Set the detector gain.
    /**
     *  @param aGain:   the gain
     */
    //------------------------------------------------------------------------------
    void setGain(RATIONAL_NUMBER aGain);


    //------------------------------------------------------------------------------
    /// Accessor on the detector bias.
    /**
     *  @return the bias
     */
    //------------------------------------------------------------------------------
    RATIONAL_NUMBER getBias();


    //------------------------------------------------------------------------------
    /// Set the detector bias.
    /**
     *  @param aBias:   the bias
     */
    //------------------------------------------------------------------------------
    void setBias(RATIONAL_NUMBER aBias);


    //------------------------------------------------------------------------------
    /// Accessor on the standard deviation of the detector bias.
    /**
     *  @return the standard deviation of the detector bias
     */
    //------------------------------------------------------------------------------
    RATIONAL_NUMBER getStdDevBias();


    //------------------------------------------------------------------------------
    /// Set the standard deviation of the detector bias.
    /**
     *  @param aStdDevBias: the standard deviation of the detector bias
     */
    //------------------------------------------------------------------------------
    void setStdDevBias(RATIONAL_NUMBER aStdDevBias);


//******************************************************************************
protected:
    //--------------------------------------------------------------------------
    /// Compute the size of the detector.
    //--------------------------------------------------------------------------
    void updateSizeInUnitOfLength();


    //--------------------------------------------------------------------------
    /// Compute the distance between the source and the detector, etc.
    //--------------------------------------------------------------------------
    void updateInternalValues();


    //--------------------------------------------------------------------------
    /// Update the VBOs.
    //--------------------------------------------------------------------------
    void updateVBOs();


    //--------------------------------------------------------------------------
    /// Update the VBO of the detector.
    //--------------------------------------------------------------------------
    void updateDetectorVBO();


    //--------------------------------------------------------------------------
    /// Update the VBO of the parallel beam.
    //--------------------------------------------------------------------------
    void updateParallelBeamVBO();


    //--------------------------------------------------------------------------
    /// Update the VBO of the cone beam.
    //--------------------------------------------------------------------------
    void updateConeBeamVBO();


    //--------------------------------------------------------------------------
    /// Update the view along the Y-axis.
    //--------------------------------------------------------------------------
    void updateFOVY();


     /// The position of the detector centre
    VEC3            m_detector_position;


    /// The position of the source centre
    VEC3            m_xray_source_centre;


    /// The sample positions of the source
    std::vector<VEC3> m_p_xray_source_position_set;


    /// The "up" vector the the detector
    VEC3            m_up_vector;


    /// The rotation matrix
    MATRIX4         m_rotation_matrix;


    /// The size of the detector
    VEC2            m_size_in_unit_of_length;


    /// Resolution (in unit of length per pixel)
    VEC2 m_resolution_in_unit_of_length_per_pixel;


    /// Distance between the source and the detector
    RATIONAL_NUMBER m_distance_source_detector;


    /// Size of the detector (in number of pixels)
    Vec2ui          m_size_in_number_of_pixels;


    /// A flag set to true when the VBOs need to be updated
    bool m_vbo_need_update;


    /// The triangle mesh corresponding to the detector
#if __cplusplus < 199711L
    std::auto_ptr<VBO> m_detector_geometry;
#else
    std::unique_ptr<VBO> m_detector_geometry;
#endif

    std::vector<GLfloat> m_p_detector_geometry_vertices;

    /// The triangle mesh corresponding to the parallel beam
#if __cplusplus < 199711L
    std::auto_ptr<VBO> m_parallel_beam;
#else
    std::unique_ptr<VBO> m_parallel_beam;
#endif


    /// The triangle mesh corresponding to the cone beam
#if __cplusplus < 199711L
    std::auto_ptr<VBO> m_cone_beam;
#else
    std::unique_ptr<VBO> m_cone_beam;
#endif


    /// The triangle mesh corresponding to the parallel beam highlight
#if __cplusplus < 199711L
    std::auto_ptr<VBO> m_parallel_beam_highlight;
#else
    std::unique_ptr<VBO> m_parallel_beam_highlight;
#endif


    /// The triangle mesh corresponding to the cone beam highlight
#if __cplusplus < 199711L
    std::auto_ptr<VBO> m_cone_beam_highlight;
#else
    std::unique_ptr<VBO> m_cone_beam_highlight;
#endif

    
    ///  The length of the widgets
    RATIONAL_NUMBER m_widget_length;


    /// Number of subdivisions of the spheres that are drawn as widgets (3 gives 1,280 triangles; 10 gave more than 20 million)
    static const unsigned int WIDGET_SPHERE_SUBDIVISIONS = 3;


    /// Polygon mesh of the source
    SphereMesh m_source_mesh;


    /// Polygon mesh of the first pixel of the detector
    CubeMesh<float> m_first_pixel_mesh;
    SphereMesh m_detector_centre_mesh;


    /// Source type
    SourceShape m_source_shape;


    /// The vector normal to the detector
    VEC3 m_normal;


    /// The viewing direction (i.e. the unit vector from the X-ray source
    /// to the centre of the detector)
    VEC3 m_look_at_vector;


    /// The cross product between m_look_at_vector and m_up_vector
    VEC3 m_right_vector;


    /// Field of view along the Y-axis
    RATIONAL_NUMBER m_fovy;


    /// Scintillator
    Scintillator m_scintillator;


    /// Impulse response of the detector as a line spread function
    std::vector<RATIONAL_NUMBER> m_LSF;


    /// Impulse response of the detector as a point spread function
    Image<RATIONAL_NUMBER> m_PSF;


    /// If true, the right vector is perpendicular to both the detector up-vector and 
    /// the vector from the source to the centre of the detector
    bool m_update_right_vector_flag;

    /// The gain is a multiplicative factor by which the pixel values are amplified
    RATIONAL_NUMBER m_gain;

    /// The bias of value of the detector
    RATIONAL_NUMBER m_bias;

    /// The standard deviation of the bias of value of the detector
    RATIONAL_NUMBER m_std_dev_bias;
};


} // namespace gVirtualXRay


//******************************************************************************
#include "XRayDetector.inl"


#endif // GVXR_XRAY_DETECTOR_H
