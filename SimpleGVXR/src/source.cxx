/**
********************************************************************************
*
*   @file       source.cxx
*
*   @brief      Expose here all the functions of SimpleGVXR related to 
*               the X-ray source. It includes the beam spectrum.
*
*   @version    1.0
*
*   @date       14/05/2025
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
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, May 2024-present, version 1.1, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <map>

#ifndef GVXR_LOGGER_H
#include "gVirtualXRay/Logger.h"
#endif

#ifndef GVXR_XRAY_BEAM_H
#include "gVirtualXRay/XRayBeam.h"
#endif

#ifndef GVXR_XRAY_DETECTOR_H
#include "gVirtualXRay/XRayDetector.h"
#endif

#ifndef GVXR_XRAY_RENDERER_H
#include "gVirtualXRay/XRayRenderer.h"
#endif

#ifndef GVXR_LOGGER_H
#include <gVirtualXRay/Logger.h>
#endif

#ifndef GVXR_OPENGL_UTILITIES_H
#include "gVirtualXRay/OpenGLUtilities.h"
#endif

#ifndef SGVXR_Internal_h
#include "internal.h"
#endif

#ifndef SGVXR_Utils_h
#include "Utils.h"
#endif

#ifndef SGVXR_Detector_h
#include "detector.h"
#endif

#ifndef SGVXR_Source_h
#include "source.h"
#endif


//******************************************************************************
//  Global variables
//******************************************************************************
extern std::map<std::string, unsigned int> g_warning_message_counters;
extern gVirtualXRay::XRayDetector g_xray_detector; // X-ray detector
extern gVirtualXRay::XRayRenderer g_xray_renderer; // X-ray graphics engine

extern uint8_t g_mixed_precision_flag;

gVirtualXRay::XRayBeam g_xray_beam;         // X-ray beam

bool g_beam_shape_initialised = false;
bool g_beam_energy_initialised = false;
bool g_source_position_initialised = false;


//------------------------------------------------------
void setSourcePosition(double x,
                       double y,
                       double z,
                       const std::string& aUnitOfLength)
//------------------------------------------------------
{
    double unit_of_length = getUnitOfLength(aUnitOfLength);

    // Save the current X-ray source shape
    gVirtualXRay::XRayDetector::SourceShape source_shape_backup = g_xray_detector.getSourceShape();

    // Update the position
    g_xray_detector.setXrayPointSource(
            gVirtualXRay::VEC3(x * unit_of_length, y * unit_of_length,
                    z * unit_of_length));

    // The position has been initialised
    g_source_position_initialised = true;

    // Restore the X-ray source shape
    switch (source_shape_backup)
    {
      case gVirtualXRay::XRayDetector::POINT:
        g_xray_detector.setXrayPointSource();
        break;

      case gVirtualXRay::XRayDetector::PARALLEL:
        g_xray_detector.setParallelBeam();
        break;

      case gVirtualXRay::XRayDetector::SQUARE:
        throw gVirtualXRay::Exception(__FILE__,
                                      __FUNCTION__,
                                      __LINE__,
                                      "This method should not be used with a square source, call `setFocalSpotAsSquare` instead.");

      case gVirtualXRay::XRayDetector::RECTANGLE:
        throw gVirtualXRay::Exception(__FILE__,
                                      __FUNCTION__,
                                      __LINE__,
                                      "This method should not be used with a square source, call `setFocalSpotAsRectangle` instead.");

      case gVirtualXRay::XRayDetector::CUBE:
        throw gVirtualXRay::Exception(__FILE__,
                                      __FUNCTION__,
                                      __LINE__,
                                      "This method should not be used with a cube source, call `setFocalSpot` instead");

      case gVirtualXRay::XRayDetector::LINE:
        throw gVirtualXRay::Exception(__FILE__,
                                      __FUNCTION__,
                                      __LINE__,
                                      "This method should not be used with a line source.");

      default: // This case should never occur, but just in case
        g_xray_detector.setXrayPointSource();
    }

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer(g_mixed_precision_flag);
}


//--------------------------
std::string getSourceShape()
//--------------------------
{
    switch (g_xray_detector.getSourceShape())
    {
    case gVirtualXRay::XRayDetector::POINT:
        return std::string("POINT");

    case gVirtualXRay::XRayDetector::PARALLEL:
        return std::string("PARALLEL");

    case gVirtualXRay::XRayDetector::SQUARE:
        return std::string("SQUARE");

    case gVirtualXRay::XRayDetector::CUBE:
        return std::string("CUBE");

    case gVirtualXRay::XRayDetector::LINE:
        return std::string("LINE");

    default: // This case should never occur, but just in case
        return std::string("UNKNOWN");
    }
}


//---------------------------------------------------------------------
std::vector<double> getSourcePosition(const std::string& aUnitOfLength)
//---------------------------------------------------------------------
{
    double unit_of_length = getUnitOfLength(aUnitOfLength);

    std::vector<double> position(3);

    position[0] = g_xray_detector.getXraySourceCentre()[0] / unit_of_length;
    position[1] = g_xray_detector.getXraySourceCentre()[1] / unit_of_length;
    position[2] = g_xray_detector.getXraySourceCentre()[2] / unit_of_length;

    return (position);
}


//-------------------
void usePointSource()
//-------------------
{
    g_xray_detector.setXrayPointSource();
    g_beam_shape_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer(g_mixed_precision_flag);
}


//----------------
void useConeBeam()
//----------------
{
    usePointSource();
}


//--------------------
void useParallelBeam()
//--------------------
{
    // Use a parallel source
    g_xray_detector.setParallelBeam();
    g_beam_shape_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer(g_mixed_precision_flag);
}


//----------------------
void useParallelSource()
//----------------------
{
    useParallelBeam();
}


//------------------------
bool isUsingParallelBeam()
//------------------------
{
    gVirtualXRay::XRayDetector::SourceShape source_shape = g_xray_detector.getSourceShape();

    return source_shape == gVirtualXRay::XRayDetector::PARALLEL;
}


//--------------------------
bool isUsingParallelSource()
//--------------------------
{
    return isUsingParallelBeam();
}


//-----------------------
bool isUsingPointSource()
//-----------------------
{
    gVirtualXRay::XRayDetector::SourceShape source_shape = g_xray_detector.getSourceShape();

    return source_shape == gVirtualXRay::XRayDetector::POINT;
}


//--------------------
bool isUsingConeBeam()
//--------------------
{
    gVirtualXRay::XRayDetector::SourceShape source_shape = g_xray_detector.getSourceShape();

    return source_shape == gVirtualXRay::XRayDetector::POINT ||
        source_shape == gVirtualXRay::XRayDetector::SQUARE || 
        source_shape == gVirtualXRay::XRayDetector::RECTANGLE || 
        source_shape == gVirtualXRay::XRayDetector::CUBE ||
        source_shape == gVirtualXRay::XRayDetector::LINE;
}


//---------------------
bool isUsingFocalSpot()
//---------------------
{
    gVirtualXRay::XRayDetector::SourceShape source_shape = g_xray_detector.getSourceShape();

    return source_shape == gVirtualXRay::XRayDetector::SQUARE || 
        source_shape == gVirtualXRay::XRayDetector::RECTANGLE || 
        source_shape == gVirtualXRay::XRayDetector::CUBE ||
        source_shape == gVirtualXRay::XRayDetector::LINE;
}


//--------------------------------------------------------------------------------------------
void setFocalSpot(float x, float y, float z, float l, const std::string& aUnitOfLength, int i)
//--------------------------------------------------------------------------------------------
{
    double unit_of_length = getUnitOfLength(aUnitOfLength);

    if (i % 2 == 0)
    {
        gVirtualXRay::LOGGER.logWarning("i (") << i << ") must be an odd number, " <<
            i+1 << " will be used instead." << std::endl;
    }

    g_xray_detector.setCubicSource(gVirtualXRay::VEC3(x * unit_of_length, y * unit_of_length, z * unit_of_length),
                      i,
                      l * unit_of_length);

    // The position has been initialised
    g_source_position_initialised = true;

    // The source shape has been initialised
    g_beam_shape_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer(g_mixed_precision_flag);
}


//---------------------------------------------------------
void setFocalSpotAsSquare(float x, float y, float z,
                          float l,
                          const std::string& aUnitOfLength,
                          int i)
//---------------------------------------------------------
{
    double unit_of_length = getUnitOfLength(aUnitOfLength);

    if (i % 2 == 0)
    {
        gVirtualXRay::LOGGER.logWarning("i (") << i << ") must be an odd number, " <<
            i+1 << " will be used instead." << std::endl;
    }

    g_xray_detector.setSquareSource(gVirtualXRay::VEC3(x * unit_of_length, y * unit_of_length, z * unit_of_length),
                      i,
                      l * unit_of_length);

    // The position has been initialised
    g_source_position_initialised = true;

    // The source shape has been initialised
    g_beam_shape_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer(g_mixed_precision_flag);
}


//------------------------------------------------------------
void setFocalSpotAsRectangle(float x, float y, float z,
                             float l, float k,
                             const std::string& aUnitOfLength,
                             int i, int j)
//------------------------------------------------------------
{
    double unit_of_length = getUnitOfLength(aUnitOfLength);

    if (i % 2 == 0)
    {
        gVirtualXRay::LOGGER.logWarning("i (") << i << ") must be an odd number, " <<
            i+1 << " will be used instead." << std::endl;
    }

    if (j % 2 == 0)
    {
        gVirtualXRay::LOGGER.logWarning("j (") << j << ") must be an odd number, " <<
            j+1 << " will be used instead." << std::endl;
    }

    g_xray_detector.setRectangleSource(gVirtualXRay::VEC3(x * unit_of_length, y * unit_of_length, z * unit_of_length),
                      i, j,
                      l * unit_of_length,
                      k * unit_of_length);

    // The position has been initialised
    g_source_position_initialised = true;

    // The source shape has been initialised
    g_beam_shape_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer(g_mixed_precision_flag);
}


//------------------------------------------------------------------------------
void rotateFocalSpot(const double& aRotationAngle, double u, double v, double w)
//------------------------------------------------------------------------------
{
    g_xray_detector.rotateFocalSpot(aRotationAngle, gVirtualXRay::VEC3(u, v, w));
}


//---------------------------------------------
void rotateSource(const double& aRotationAngle,
                  double x, double y, double z,
                  double u, double v, double w)
//---------------------------------------------
{
    g_xray_detector.rotateSource(aRotationAngle,
        gVirtualXRay::VEC3(x, y, z),
        gVirtualXRay::VEC3(u, v, w));
}


//--------------------------------------------------
void resetBeamSpectrum(bool aClearTubeParameterFlag)
//--------------------------------------------------
{
    if (aClearTubeParameterFlag) g_xray_beam.clear();
    else g_xray_beam.clearSpectrumOnly();

    g_beam_energy_initialised = false;
}


//------------------------------------------------------------
void setBinTotalEnergyCutoff(double aThreshold,
                             const std::string& aUnitOfEnergy)
//------------------------------------------------------------
{
    g_xray_beam.setBinTotalEnergyCutoff(aThreshold * getUnitOfEnergy(aUnitOfEnergy));
}


//-----------------------------------------------------
void setMonoChromatic(double anEnergy,
                      const std::string& aUnitOfEnergy,
                      double aNumberOfPhotons)
//-----------------------------------------------------
{
    if (!g_warning_message_counters.count("setMonoChromatic"))
    {
        gVirtualXRay::LOGGER.logWarning(std::string(__FUNCTION__) + 
            " is deprecated. It will be removed in a future release. "
            "Choose setMonoChromaticPerPixelAtSDD or setMonoChromaticPerCm2At1m. "
            "Note that setMonoChromatic and setMonoChromaticPerPixelAtSDD are equivalent.") << std::endl;

        g_warning_message_counters["setMonoChromatic"] = 1;
    }

    setMonoChromaticPerPixelAtSDD(anEnergy, aUnitOfEnergy, aNumberOfPhotons);
}


//------------------------------------------------------------------
void setMonoChromaticPerPixelAtSDD(double anEnergy,
                                   const std::string& aUnitOfEnergy,
                                   double aNumberOfPhotons)
//------------------------------------------------------------------
{
    g_xray_beam.clear();
    addEnergyBinToSpectrumPerPixelAtSDD(anEnergy, aUnitOfEnergy, aNumberOfPhotons);
}


//---------------------------------------------------------------
void setMonoChromaticPerCm2At1m(double anEnergy,
                                const std::string& aUnitOfEnergy,
                                double aNumberOfPhotons)
//---------------------------------------------------------------
{
    g_xray_beam.clear();
    addEnergyBinToSpectrumPerCm2At1m(anEnergy, aUnitOfEnergy, aNumberOfPhotons);
}


//-----------------------------------------------------------
void addEnergyBinToSpectrum(double anEnergy,
                            const std::string& aUnitOfEnergy,
                            double aNumberOfPhotons)
//-----------------------------------------------------------
{
    if (!g_warning_message_counters.count("addEnergyBinToSpectrum"))
    {
        gVirtualXRay::LOGGER.logWarning(std::string(__FUNCTION__) + 
            " is deprecated. It will be removed in a future release. "
            "Choose addEnergyBinToSpectrumPerPixelAtSDD or addEnergyBinToSpectrumPerCm2At1m. "
            "Note that addEnergyBinToSpectrum and addEnergyBinToSpectrumPerPixelAtSDD are equivalent.") << std::endl;

        g_warning_message_counters["addEnergyBinToSpectrum"] = 1;
    }

    addEnergyBinToSpectrumPerPixelAtSDD(anEnergy, aUnitOfEnergy, aNumberOfPhotons);
}


//------------------------------------------------------------------------
void addEnergyBinToSpectrumPerPixelAtSDD(double anEnergy,
                                         const std::string& aUnitOfEnergy,
                                         double aNumberOfPhotons)
//------------------------------------------------------------------------
{
    if (g_xray_beam.addChannelPerPixelAtSDD(aNumberOfPhotons,
        anEnergy * getUnitOfEnergy(aUnitOfEnergy)))
    {
        g_beam_energy_initialised = true;

        // Initialise the X-ray renderer if needed and if possible
        initialiseXRayRenderer(g_mixed_precision_flag);
    }
}


//---------------------------------------------------------------------
void addEnergyBinToSpectrumPerCm2At1m(double anEnergy,
                                      const std::string& aUnitOfEnergy,
                                      double aNumberOfPhotons)
//---------------------------------------------------------------------
{
    if (g_xray_beam.addChannelPerCm2At1m(aNumberOfPhotons,
        anEnergy * getUnitOfEnergy(aUnitOfEnergy)))
    {
        g_beam_energy_initialised = true;

        // Initialise the X-ray renderer if needed and if possible
        initialiseXRayRenderer(g_mixed_precision_flag);
    }
}


//--------------------------------------------------------
void loadSpectrumFromTSV(const std::string& aFileName,
    const std::string& aUnitOfEnergy, bool aNormalisationFlag)
//--------------------------------------------------------
{
    if (!g_warning_message_counters.count("loadSpectrumFromTSV"))
    {
        gVirtualXRay::LOGGER.logWarning(std::string(__FUNCTION__) + 
            " is deprecated. It will be removed in a future release. "
            "Choose loadSpectrumFromTSVPerPixelAtSDD or loadSpectrumFromTSVPerCm2At1m. "
            "Note that loadSpectrumFromTSV and loadSpectrumFromTSVPerPixelAtSDD are equivalent.") << std::endl;

        g_warning_message_counters["loadSpectrumFromTSV"] = 1;
    }

    loadSpectrumFromTSVPerPixelAtSDD(aFileName, aUnitOfEnergy, aNormalisationFlag);
}


//---------------------------------------------------------------------
void loadSpectrumFromTSVPerPixelAtSDD(const std::string& aFileName,
                                      const std::string& aUnitOfEnergy,
                                      bool aNormalisationFlag)
//---------------------------------------------------------------------
{
    g_xray_beam.clear();

    g_xray_beam.initialisePerPixelAtSDD(aFileName.c_str(), 
        getUnitOfEnergy(aUnitOfEnergy));

    if (aNormalisationFlag) {
        g_xray_beam.normalise();
    }

    g_beam_energy_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer(g_mixed_precision_flag);
}


//------------------------------------------------------------------
void loadSpectrumFromTSVPerCm2At1m(const std::string& aFileName,
                                   const std::string& aUnitOfEnergy,
                                   bool aNormalisationFlag)
//------------------------------------------------------------------
{
    g_xray_beam.clear();

    g_xray_beam.initialisePerCm2At1m(aFileName.c_str(), 
        getUnitOfEnergy(aUnitOfEnergy));

    if (aNormalisationFlag) {
        g_xray_beam.normalise();
    }

    g_beam_energy_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer(g_mixed_precision_flag);
}


//-----------------------------------------------------------------
std::vector<double> getEnergyBins(const std::string& aUnitOfEnergy)
//-----------------------------------------------------------------
{
    std::vector<double> p_converted_energy_bins;
    std::vector<double> p_unconverted_energy_bins = g_xray_beam.getEnergySet();

    for (std::vector<double>::const_iterator ite = p_unconverted_energy_bins.begin();
		ite != p_unconverted_energy_bins.end();
		++ite)
	{
        p_converted_energy_bins.push_back(*ite / getUnitOfEnergy(aUnitOfEnergy));
    }

    return (p_converted_energy_bins);
}


//----------------------------------------------------------------------
std::vector<double> getPhotonCountEnergyBins(bool anApplyFiltrationFlag)
//----------------------------------------------------------------------
{
    gVirtualXRay::LOGGER.logWarning(std::string(__FUNCTION__) + 
        " is deprecated. It will be removed in a future release. "
        "Choose getPhotonCountsPerPixelAtSDD or getPhotonCountsPerCm2At1m. "
        "Note that getEnergyChannel and getPhotonCountsPerPixelAtSDD are equivalent.") << std::endl;

    return getPhotonCountsPerPixelAtSDD(anApplyFiltrationFlag);
}


//-----------------------------------------------------------------------
std::vector<double> getPhotonCountsPerCm2At1m(bool anApplyFiltrationFlag)
//-----------------------------------------------------------------------
{
    std::vector<double> p_temp;

    size_t number_of_energy_channels = g_xray_beam.getEnergyChannelNumber();

    for (size_t i = 0; i < number_of_energy_channels; ++i) {
        p_temp.push_back(g_xray_beam.getEnergyChannelPerCm2At1m(i, &g_xray_detector, anApplyFiltrationFlag).getPhotonNumber());
    }

    return (p_temp);
}


//--------------------------------------------------------------------------
std::vector<double> getPhotonCountsPerPixelAtSDD(bool anApplyFiltrationFlag)
//--------------------------------------------------------------------------
{
    std::vector<double> p_temp;

    size_t number_of_energy_channels = g_xray_beam.getEnergyChannelNumber();

    for (size_t i = 0; i < number_of_energy_channels; ++i) {
        p_temp.push_back(g_xray_beam.getEnergyChannelPerPixelAtSDD(i, &g_xray_detector, anApplyFiltrationFlag).getPhotonNumber());
    }

    return (p_temp);
}


//-------------------------------------------------------------------
double getTotalEnergyWithDetectorResponse(bool anApplyFiltrationFlag)
//-------------------------------------------------------------------
{
    return g_xray_renderer.getTotalEnergyWithDetectorResponse(anApplyFiltrationFlag);
}


//-----------------------------------------------------------------------------------
unsigned short getTotalEnergyWithDetectorResponseWithGain(bool anApplyFiltrationFlag)
//-----------------------------------------------------------------------------------
{
    float total_energy_with_gain = g_xray_detector.getGain() * g_xray_renderer.getTotalEnergyWithDetectorResponse(anApplyFiltrationFlag);
    return static_cast<unsigned short>(std::clamp(static_cast<int>(std::round(total_energy_with_gain)), 0, USHRT_MAX));
}


//----------------------------------------------------------------------
double getTotalEnergyWithoutDetectorResponse(bool anApplyFiltrationFlag)
//----------------------------------------------------------------------
{
    return g_xray_renderer.getTotalEnergyWithoutDetectorResponse(anApplyFiltrationFlag);
}


//--------------------------------------------------------------------------------------
unsigned short getTotalEnergyWithoutDetectorResponseWithGain(bool anApplyFiltrationFlag)
//--------------------------------------------------------------------------------------
{
    float total_energy_with_gain = g_xray_detector.getGain() * g_xray_renderer.getTotalEnergyWithoutDetectorResponse(anApplyFiltrationFlag);
    unsigned short clamped_value = std::max(USHRT_MAX, static_cast<int>(std::round(total_energy_with_gain)));

    return clamped_value;
}


//--------------------------------------------------------------------------------
void setVoltage(const float& aVoltage, const std::string& aUnitOfElectriPotential)
//--------------------------------------------------------------------------------
{
    g_xray_beam.setVoltage(aVoltage * getUnitOfElectriPotential(aUnitOfElectriPotential));

    g_beam_energy_initialised = true;

    // Initialise the X-ray renderer if needed and if possible
    initialiseXRayRenderer(g_mixed_precision_flag);
}


//----------------------------------------------------------
float getVoltage(const std::string& aUnitOfElectriPotential)
//----------------------------------------------------------
{
    return g_xray_beam.getVoltage() / getUnitOfElectriPotential(aUnitOfElectriPotential);
}


//------------------------------------------------------
void addInherentFilter(const std::string& aMaterial,
                       const double& aThickness,
                       const std::string& aUnitOfLength)
//------------------------------------------------------
{
    g_xray_beam.addInherentFilter(gVirtualXRay::ElementSet::getInstance().getElement(aMaterial).Z(), 
        aThickness * getUnitOfLength(aUnitOfLength));
}


//------------------------------------------------------
void addInherentFilter(unsigned int aMaterial,
                       const double& aThickness,
                       const std::string& aUnitOfLength)
//------------------------------------------------------
{
    g_xray_beam.addInherentFilter(aMaterial, aThickness * getUnitOfLength(aUnitOfLength));

}


//----------------------------
void clearInherentFiltration()
//----------------------------
{
    g_xray_beam.clearInherentFiltration();
}


//----------------------------------------------
std::vector<int> getInherentFiltrationMaterial()
//----------------------------------------------
{
    std::vector<int> p_filtration;

    for (std::vector<std::pair<unsigned int, gVirtualXRay::RATIONAL_NUMBER> >::const_iterator ite(g_xray_beam.getInherentFiltration().begin());
        ite != g_xray_beam.getInherentFiltration().end();
        ++ite)
    {
        p_filtration.push_back(ite->first);
    }

    return p_filtration;
}


//---------------------------------------------------------------------------------
std::vector<float> getInherentFiltrationThickness(const std::string& aUnitOfLength)
//---------------------------------------------------------------------------------
{
    std::vector<float> p_filtration;

    for (std::vector<std::pair<unsigned int, gVirtualXRay::RATIONAL_NUMBER> >::const_iterator ite(g_xray_beam.getInherentFiltration().begin());
        ite != g_xray_beam.getInherentFiltration().end();
        ++ite)
    {
        p_filtration.push_back(ite->second / getUnitOfLength(aUnitOfLength));
    }

    return p_filtration;
}


//------------------------------
void setmAs(const float& an_mAs)
//------------------------------
{
    g_xray_beam.setmAs(an_mAs);
}


//------------
float getmAs()
//------------
{
    return g_xray_beam.getmAs();
}


//-------------------------------------
void setTubeAngle(const float& anAngle)
//-------------------------------------
{
    g_xray_beam.setTubeAngle(anAngle);
}


//------------------
float getTubeAngle()
//------------------
{
    return g_xray_beam.getTubeAngle();
}


//------------------------------------------------------
void setFirstEnergyBin(const float& aFirstEnergyBin, 
                       const std::string& aUnitOfEnergy)
//------------------------------------------------------
{
    g_xray_beam.setFirstEnergyBin(aFirstEnergyBin * getUnitOfEnergy(aUnitOfEnergy));
}


//------------------------------------------------------
void setEnergyBinSize(const float& anEnergyBinSize, 
                      const std::string& aUnitOfEnergy)
//------------------------------------------------------
{
    g_xray_beam.setEnergyBinSize(anEnergyBinSize * getUnitOfEnergy(aUnitOfEnergy));
}


//------------------------------------------------------
float getEnergyBinSize(const std::string& aUnitOfEnergy)
//------------------------------------------------------
{
    return g_xray_beam.getEnergyBinSize() / getUnitOfEnergy(aUnitOfEnergy);
}


//--------------------
void clearFiltration()
//--------------------
{
    g_xray_beam.clearFiltration();
}


//-----------------------------------------------------------------------------------
void setFiltration(const std::vector<std::pair<unsigned int, float> >& aSetOfFilters,
                    const std::string& aUnitOfLength)
//-----------------------------------------------------------------------------------
{
    std::vector<std::pair<unsigned int, gVirtualXRay::RATIONAL_NUMBER> > p_filtration;

    for (std::vector<std::pair<unsigned int, float> >::const_iterator ite(aSetOfFilters.begin());
        ite != aSetOfFilters.end();
        ++ite)
    {
        p_filtration.push_back(std::pair<unsigned int, float>(ite->first, float(ite->second * getUnitOfLength(aUnitOfLength))));
    }

    g_xray_beam.setFiltration(p_filtration);
}


//----------------------------------------------
void addFilter(const std::string& aMaterial,
               const double& aThickness,
               const std::string& aUnitOfLength)
//----------------------------------------------
{
    g_xray_beam.addFilter(aMaterial, aThickness * getUnitOfLength(aUnitOfLength));
}


//----------------------------------------------
void addFilter(unsigned int aMaterial,
               const double& aThickness,
               const std::string& aUnitOfLength)
//----------------------------------------------
{
    g_xray_beam.addFilter(aMaterial, aThickness * getUnitOfLength(aUnitOfLength));

}


//--------------------------------------
std::vector<int> getFiltrationMaterial()
//--------------------------------------
{
    std::vector<int> p_filtration;

    for (std::vector<std::pair<unsigned int, gVirtualXRay::RATIONAL_NUMBER> >::const_iterator ite(g_xray_beam.getFiltration().begin());
        ite != g_xray_beam.getFiltration().end();
        ++ite)
    {
        p_filtration.push_back(ite->first);
    }

    return p_filtration;
}


//-------------------------------------------------------------------------
std::vector<float> getFiltrationThickness(const std::string& aUnitOfLength)
//-------------------------------------------------------------------------
{
    std::vector<float> p_filtration;

    for (std::vector<std::pair<unsigned int, gVirtualXRay::RATIONAL_NUMBER> >::const_iterator ite(g_xray_beam.getFiltration().begin());
        ite != g_xray_beam.getFiltration().end();
        ++ite)
    {
        p_filtration.push_back(ite->second / getUnitOfLength(aUnitOfLength));
    }

    return p_filtration;
}


//-----------------------
void enablePoissonNoise()
//-----------------------
{
    g_xray_beam.enablePoissonNoise();
}


//------------------------
void disablePoissonNoise()
//------------------------
{
    g_xray_beam.disablePoissonNoise();
}


//--------------------------------------------------------------------------
void setNumberOfPhotons(double aNumberOfPhotons, bool anApplyFiltrationFlag)
//--------------------------------------------------------------------------
{
    setNumberOfPhotonsPerPixelAtSDD(aNumberOfPhotons, anApplyFiltrationFlag);
}


//---------------------------------------------------------------------------------------
void setNumberOfPhotonsPerPixelAtSDD(double aNumberOfPhotons, bool anApplyFiltrationFlag)
//---------------------------------------------------------------------------------------
{
    g_xray_beam.setNumberOfPhotonsPerPixelAtSDD(aNumberOfPhotons, anApplyFiltrationFlag);
}


//------------------------------------------------------------------------------------
void setNumberOfPhotonsPerCm2At1m(double aNumberOfPhotons, bool anApplyFiltrationFlag)
//------------------------------------------------------------------------------------
{
    g_xray_beam.setNumberOfPhotonsPerCm2At1m(aNumberOfPhotons, anApplyFiltrationFlag);
}


//---------------------------------------------------
double getNumberOfPhotons(bool anApplyFiltrationFlag)
//---------------------------------------------------
{
    return getNumberOfPhotonsPerPixelAtSDD(anApplyFiltrationFlag);
}


//----------------------------------------------------------------
double getNumberOfPhotonsPerPixelAtSDD(bool anApplyFiltrationFlag)
//----------------------------------------------------------------
{
	return g_xray_beam.getNumberOfPhotonsPerPixelAtSDD(&g_xray_detector, anApplyFiltrationFlag);
}


//-------------------------------------------------------------
double getNumberOfPhotonsPerCm2At1m(bool anApplyFiltrationFlag)
//-------------------------------------------------------------
{
	return g_xray_beam.getNumberOfPhotonsPerCm2At1m(&g_xray_detector, anApplyFiltrationFlag);
}


//------------------------------
void usePoissonNoise(bool aFlag)
//------------------------------
{
    g_xray_beam.usePoissonNoise(aFlag);
}


//--------------------
bool usePoissonNoise()
//--------------------
{
    return (g_xray_beam.usePoissonNoise());
}


//----------------------------------------
int getLargestPossibleNumberOfEnergyBins()
//----------------------------------------
{
    GLint texture_size = 0;

#ifdef HAS_OPENGL
    glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &texture_size);
    gVirtualXRay::checkOpenGLErrorStatus(__FILE__, __FUNCTION__, __LINE__);
#endif

    return texture_size;
}