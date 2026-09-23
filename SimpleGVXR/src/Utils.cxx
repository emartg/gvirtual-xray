#include <memory>
#include <map>
#include <vector>
#include <iostream>

#ifndef GVXR_CONSTANT_VALUES_H
#include "gVirtualXRay/ConstantValues.h"
#endif

#ifndef GVXR_UNITS_H
#include "gVirtualXRay/Units.h"
#endif

#ifndef GVXR_XRAY_RENDERER_H
#include "gVirtualXRay/XRayRenderer.h"
#endif

#ifndef GVXR_Detector_h
#include "gVirtualXRay/XRayDetector.h"
#endif

#ifndef GVXR_XRAY_BEAM_H
#include "gVirtualXRay/XRayBeam.h"
#endif

#ifndef GVXR_CONTEXT_H
#include "gVirtualXRay/Context.h"
#endif

#ifndef GVXR_EXCEPTION_H
#include "gVirtualXRay/Exception.h"
#endif

#ifndef GVXR_RESOURCE_MANAGER_H
#include "gVirtualXRay/ResourceManager.h"
#endif

#ifndef GVXR_SHADER_H
#include "gVirtualXRay/Shader.h"
#endif

#ifndef GVXR_SCENEGRAPH_BINDER_H
#include "gVirtualXRay/SceneGraphBinder.h"
#endif

#ifndef GVXR_LOGGER_H
#include "gVirtualXRay/Logger.h"
#endif

#ifndef SGVXR_Window_h
#include "Window.h"
#endif

#ifndef SGVXR_OpenGLWindow_h
#include "OpenGLWindow.h"
#endif

#ifndef SGVXR_VulkanWindow_h
#include "VulkanWindow.h"
#endif

#ifndef SGVXR_Windowless_h
#include "Windowless.h"
#endif

#ifndef SGVXR_internal_h
#include "internal.h"
#endif

#ifndef SGVXR_Utils_h
#include "Utils.h"
#endif


//******************************************************************************
//  Namespace
//******************************************************************************


//******************************************************************************
//  Global variables
//******************************************************************************
extern gVirtualXRay::XRayRenderer g_xray_renderer; // X-ray graphics engine
extern gVirtualXRay::XRayBeam g_xray_beam;
extern gVirtualXRay::XRayDetector g_xray_detector; // X-ray detector
extern std::map<int, std::unique_ptr<SimpleGVXR::Window> > g_p_window_set;
extern std::vector<int> g_p_window_id_stack;           // The stack of window IDs
extern gVirtualXRay::Shader g_display_shader;      // Visualisation shader
extern std::unique_ptr<gVirtualXRay::SceneGraphBinder> g_p_scene_graph;
extern bool g_renderer_initialised;
extern bool g_beam_shape_initialised;
extern bool g_beam_energy_initialised;
extern bool g_source_position_initialised;
extern bool g_detector_position_initialised;
extern bool g_detector_up_vector_initialised;
extern bool g_detector_size_initialised;
extern bool g_detector_pixel_size_initialised;

uint8_t g_mixed_precision_flag = 255;


//******************************************************************************
//  Function declarations
//******************************************************************************
void errorCallback(int error, const char* description);
void quit();


//******************************************************************************
//  Function definitions
//******************************************************************************


//---------
void quit()
//---------
{
    gVirtualXRay::LOGGER.logNow("Quit and destroy all the windows") << std::endl;
    gVirtualXRay::ResourceManager::getInstance().setGraphicsContext(nullptr);
    destroyAllWindows();
}

//----------------------------------------------------
void errorCallback(int error, const char* description)
//----------------------------------------------------
{
    // Display the error in the terminal
    gVirtualXRay::LOGGER.logWarning("GLFW error: ") << description << std::endl;
}


//--------------------------------
float radian2degree(float anAngle)
//--------------------------------
{
    return (180.0 * anAngle / gVirtualXRay::Pi);
}


//-----------------------------------------------
double getUnitOfEnergy(const std::string& aUnitOfEnergy)
//-----------------------------------------------
{
    double unit_of_energy = 0;

    if (aUnitOfEnergy == "megaelectronvolt" || aUnitOfEnergy == "MeV") {
    unit_of_energy = gVirtualXRay::MeV;
    } else if (aUnitOfEnergy == "kiloelectronvolt" || aUnitOfEnergy == "keV") {
    unit_of_energy = gVirtualXRay::keV;
    } else if (aUnitOfEnergy == "electronvolt" || aUnitOfEnergy == "eV") {
    unit_of_energy = gVirtualXRay::eV;
    } else {
    throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
    "ERROR: Unknown unit of energy.");
    }

    return (unit_of_energy);
}


//-----------------------------------------------
double getUnitOfLength(const std::string& aUnitOfLength)
//-----------------------------------------------
{
    double unit_of_length = 0;

    if ((aUnitOfLength == "kilometer") || (aUnitOfLength == "kilometre")
    || (aUnitOfLength == "km")) {
    unit_of_length = gVirtualXRay::km;
    } else if ((aUnitOfLength == "hectometer")
    || (aUnitOfLength == "hectometre") || (aUnitOfLength == "hm")) {
    unit_of_length = gVirtualXRay::hm;
    } else if ((aUnitOfLength == "decameter") || (aUnitOfLength == "decametre")
    || (aUnitOfLength == "dam")) {
    unit_of_length = gVirtualXRay::dam;
    } else if ((aUnitOfLength == "meter") || (aUnitOfLength == "metre")
    || (aUnitOfLength == "m")) {
    unit_of_length = gVirtualXRay::m;
    } else if ((aUnitOfLength == "decimeter") || (aUnitOfLength == "decimetre")
    || (aUnitOfLength == "dm")) {
    unit_of_length = gVirtualXRay::dm;
    } else if ((aUnitOfLength == "centimeter")
    || (aUnitOfLength == "centimetre") || (aUnitOfLength == "cm")) {
    unit_of_length = gVirtualXRay::cm;
    } else if ((aUnitOfLength == "millimeter")
    || (aUnitOfLength == "millimetre") || (aUnitOfLength == "mm")) {
    unit_of_length = gVirtualXRay::mm;
    } else if ((aUnitOfLength == "micrometer")
    || (aUnitOfLength == "micrometre") || (aUnitOfLength == "um")) {
    unit_of_length = gVirtualXRay::um;
    } else {
    throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
    "ERROR: Unknown unit of length.");
    }

    return (unit_of_length);
}


//---------------------------------------------------------------------------
double getUnitOfElectriPotential(const std::string& aUnitOfElectriPotential)
//---------------------------------------------------------------------------
{
    double unit_of_electric_potential = 0;

    if ((aUnitOfElectriPotential == "V") || (aUnitOfElectriPotential == "volt"))
    {
        unit_of_electric_potential = gVirtualXRay::V;
    }
    else if ((aUnitOfElectriPotential == "kV") || (aUnitOfElectriPotential == "kilovolt"))
    {
        unit_of_electric_potential = gVirtualXRay::kV;
    }
    else if ((aUnitOfElectriPotential == "MV") || (aUnitOfElectriPotential == "megavolt"))
    {
        unit_of_electric_potential = gVirtualXRay::MV;
    }
    else
    {
        throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
            "ERROR: Unknown unit of electric potential.");
    }

    return (unit_of_electric_potential);
}


//-------------------------------------
void enableArtefactFilteringOnGPU(void)
//-------------------------------------
{
    g_xray_renderer.enableArtifactFilteringOnGPU();
}

//-------------------------------------
void enableArtifactFilteringOnGPU(void)
//-------------------------------------
{
    enableArtefactFilteringOnGPU();
}

//-------------------------------------
void enableArtefactFilteringOnCPU(void)
//-------------------------------------
{
    g_xray_renderer.enableArtifactFilteringOnCPU();
}

//-------------------------------------
void enableArtifactFilteringOnCPU(void)
//-------------------------------------
{
    enableArtefactFilteringOnCPU();
}


//---------------------------------
void disableArtefactFiltering(void)
//---------------------------------
{
    g_xray_renderer.disableArtifactFilteringOnGPU();
    g_xray_renderer.disableArtifactFilteringOnCPU();
}

//---------------------------------
void disableArtifactFiltering(void)
//---------------------------------
{
    disableArtefactFiltering();
}


//-----------------------------------------------------------
uint8_t computeMixedPrecisionFlag(bool aUncleanedLBufferFlag,
                                  bool aCleanedLBufferFlag,
                                  bool aCumulatedLBufferFlag,
                                  bool anEnergyFluenceFlag,
                                  bool aSumMuXdXFlag,
                                  bool aZBufferFlag)
//-----------------------------------------------------------
{
    uint8_t flag = 0;
    
    if (aUncleanedLBufferFlag) flag = flag | gVirtualXRay::MixedPrecisionFlags::UncleanedLBuffer;
    if (aCleanedLBufferFlag)   flag = flag | gVirtualXRay::MixedPrecisionFlags::CleanedLBuffer;
    if (aCumulatedLBufferFlag) flag = flag | gVirtualXRay::MixedPrecisionFlags::CumulatedLBuffer;
    if (anEnergyFluenceFlag)   flag = flag | gVirtualXRay::MixedPrecisionFlags::EnergyFluence;
    if (aSumMuXdXFlag)         flag = flag | gVirtualXRay::MixedPrecisionFlags::SumMuXdX;
    if (aZBufferFlag)          flag = flag | gVirtualXRay::MixedPrecisionFlags::ZBuffer;

    return flag;
}


//-----------------------------
uint8_t getMixedPrecisionFlag()
//-----------------------------
{
    return g_mixed_precision_flag;
}


//---------------------------------
bool getPrecisionUncleanedLBuffer()
//---------------------------------
{
    return g_mixed_precision_flag & gVirtualXRay::MixedPrecisionFlags::UncleanedLBuffer;
}


//-------------------------------
bool getPrecisionCleanedLBuffer()
//-------------------------------
{
    return g_mixed_precision_flag & gVirtualXRay::MixedPrecisionFlags::CleanedLBuffer;
}


//---------------------------------
bool getPrecisionCumulatedLBuffer()
//---------------------------------
{
    return g_mixed_precision_flag & gVirtualXRay::MixedPrecisionFlags::CumulatedLBuffer;
}


//------------------------------
bool getPrecisionEnergyFluence()
//------------------------------
{
    return g_mixed_precision_flag & gVirtualXRay::MixedPrecisionFlags::EnergyFluence;
}


//-------------------------
bool getPrecisionSumMuXdX()
//-------------------------
{
    return g_mixed_precision_flag & gVirtualXRay::MixedPrecisionFlags::SumMuXdX;
}


//------------------------
bool getPrecisionZBuffer()
//------------------------
{
    return g_mixed_precision_flag & gVirtualXRay::MixedPrecisionFlags::ZBuffer;
}


//----------------------------------
unsigned int getNumberOfEGLDevices()
//----------------------------------
{
    return gVirtualXRay::Context::getNumberOfEGLDevices();
}


//---------------------------------------
std::vector<std::string> findEGLClients()
//---------------------------------------
{
    return gVirtualXRay::Context::findEGLClients();
}


//----------------------------------------------------------
void createOpenGLContext(int aWindowID,
		                 int aRendererMajorVersion,
		                 int aRendererMinorVersion,
                         unsigned int aMixedPrecisionFlag)
//----------------------------------------------------------
{
    g_mixed_precision_flag = aMixedPrecisionFlag;

    createWindow(aWindowID,
        true,
        "OPENGL",
        aRendererMajorVersion,
        aRendererMinorVersion,
        aMixedPrecisionFlag);
}


//---------------------------------------------------
void createWindow(int aWindowID,
                  int aVisibilityFlag,
                  const std::string& aRenderer,
                  int aRendererMajorVersion,
                  int aRendererMinorVersion,
                  unsigned int aMixedPrecisionFlag)
//---------------------------------------------------
{
    g_mixed_precision_flag = aMixedPrecisionFlag;

    gVirtualXRay::LOGGER.logNow("Create window (ID: ") << aWindowID << ")\n";

    // Create the window
    std::string renderer = aRenderer;
    for (auto& c: renderer) c = std::toupper(c);

    if (!g_p_window_set.size())
    {
        // Make sure all the windows are destroyed
        atexit(quit);
    }

    if (aWindowID == -1)
    {
        aWindowID = g_p_window_set.size();
    }

    // The window does not exist
    if (!g_p_window_set[aWindowID])
    {
        // This is a stack (LIFO) not a queue (FIFO)
        std::vector<std::string> backend_priority;

        backend_priority.push_back("OPENMP");

        if (renderer == "OPENGL")
        {
#if defined(_WIN32) || defined(_WIN64)
            // On Windows, prefer the native GLFW/OpenGL path and do not try
            // X11/EGL fallbacks that are not available on the platform.
            backend_priority.push_back(renderer);
#else
            #ifdef HAS_GLX
            backend_priority.push_back("GLX");
            #endif

            #ifdef HAS_EGL
            backend_priority.push_back("EGL");
            #endif

            backend_priority.push_back(renderer);
#endif
        }
        else if (renderer == "EGL")
        {
            backend_priority.push_back("OPENGL");
#if !(defined(_WIN32) || defined(_WIN64))
            #ifdef HAS_GLX
            backend_priority.push_back("GLX");
            #endif
#endif
            backend_priority.push_back(renderer);
        }
        else if (renderer == "GLX")
        {
            backend_priority.push_back("OPENGL");

#if !(defined(_WIN32) || defined(_WIN64))
            #ifdef HAS_EGL
            backend_priority.push_back("EGL");
            #endif
#endif
            backend_priority.push_back(renderer);
        }
        else if (renderer == "VULKAN")
        {
            backend_priority.push_back(renderer);
        }
        // No longer needed as the backend is added to the bottom of the stack (i.e. lowest priority)
        // else if (renderer == "OPENMP")
        // {
        //     backend_priority.push_back(renderer);
        // }
        else if (renderer != "OPENMP")
    	{
    		std::string error_message = "ERROR: Unknown renderer (";
    		error_message += aRenderer;
    		error_message += ")";

    		throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
    				error_message);
    	}

        while (backend_priority.size())
        {
            std::string backend = backend_priority.back();
            backend_priority.pop_back();

            if (backend == "OPENGL")
            {
                gVirtualXRay::LOGGER.logNow("Request an interactive OpenGL context") << std::endl;

                // Use try/catch to prevent crash when the backend fails.
                // It allows us to take advantage of the backend_priority stack.
                try
                {
#ifdef HAS_GLFW
                    // Set an error callback
                    glfwSetErrorCallback(errorCallback);
#endif
                    g_p_window_set[aWindowID].reset(new SimpleGVXR::OpenGLWindow(aVisibilityFlag, aRendererMajorVersion, aRendererMinorVersion));
                    g_xray_renderer.setComputingMode(gVirtualXRay::XRayRenderer::ComputingMode::OPENGL);
                }
                catch (const gVirtualXRay::Exception& err)
                {
                    std::string expected_message("Error: Cannot initialise GLFW.");

                    if (std::string(err.what()).substr(0, expected_message.size()) != expected_message)
                    {
                        throw gVirtualXRay::Exception(err.what());
                    }
                }
            }
            else if (backend == "EGL")
            {
                gVirtualXRay::LOGGER.logNow("Request a EGL context") << std::endl;

#ifdef HAS_EGL
        	    g_p_window_set[aWindowID].reset(new SimpleGVXR::Windowless("EGL", aRendererMajorVersion, aRendererMinorVersion));
                g_xray_renderer.setComputingMode(gVirtualXRay::XRayRenderer::ComputingMode::OPENGL);
#else
                gVirtualXRay::LOGGER.logWarning("An EGL context was requested but it is not available on your platform") << std::endl;
#endif
            }
            else if (backend == "GLX")
            {
                gVirtualXRay::LOGGER.logNow("Request a GLX context") << std::endl;

#ifdef HAS_GLX
        	    g_p_window_set[aWindowID].reset(new SimpleGVXR::Windowless("GLX", aRendererMajorVersion, aRendererMinorVersion));
                g_xray_renderer.setComputingMode(gVirtualXRay::XRayRenderer::ComputingMode::OPENGL);
#else
                gVirtualXRay::LOGGER.logWarning("A GLX context was requested but it is not available on your platform") << std::endl;
#endif
            }
            else if (backend == "VULKAN")
            {
                gVirtualXRay::LOGGER.logNow("Request a Vulkan context") << std::endl;
                g_p_window_set[aWindowID].reset(new SimpleGVXR::VulkanWindow(aVisibilityFlag));
                g_xray_renderer.setComputingMode(gVirtualXRay::XRayRenderer::ComputingMode::VULKAN);
            }

            // CPU backend
            if (backend == "OPENMP")
            {
                g_mixed_precision_flag = 255;
                gVirtualXRay::LOGGER.logNow("Request an OpenMP context") << std::endl;
                g_p_window_set[aWindowID].reset(new SimpleGVXR::Window(false));
                g_xray_renderer.setComputingMode(gVirtualXRay::XRayRenderer::ComputingMode::OPENMP);
            }
            // GPU backend
            else if (!g_p_window_set[aWindowID]->getContext().isReady())
            {
                gVirtualXRay::LOGGER.logWarning("Could not create a context") << std::endl;
                g_p_window_set[aWindowID].reset(nullptr);
            }
            else
            {
                backend_priority.clear();
            }
        }

        // The window has been created
        if (g_p_window_set[aWindowID].get())
        {
            // Register its ID
            g_p_window_id_stack.push_back(aWindowID);

#ifdef HAS_GLFW
            if (g_p_window_set[aWindowID]->getContext().getWindowHandle())
            {
                glfwMakeContextCurrent(g_p_window_set[aWindowID]->getContext().getWindowHandle());
            }
#endif

            // Initialise the X-ray renderer if needed and if possible
            gVirtualXRay::LOGGER.logNow("Initialise the X-ray renderer if needed and if possible") << std::endl;

            initialiseXRayRenderer(g_mixed_precision_flag);
        }
        // The window has not been created
        else
        {
            // Display a warning
            gVirtualXRay::LOGGER.logWarning("Window ") <<
                aWindowID <<
                " cannot be created." <<
                std::endl;

            // Delete the empty record that may have been created
            // in g_p_window_set
            std::map<int, std::unique_ptr<SimpleGVXR::Window> >::iterator it;
            it = g_p_window_set.find(aWindowID);
            if (it != g_p_window_set.end())
                {
                g_p_window_set.erase(it);
                }
        }
    }
    // The window already exist
    // Do nothing
    else
    {
        gVirtualXRay::LOGGER.logWarning("Window ") <<
            aWindowID <<
            " already exists." <<
            std::endl;
    }
}


//-------------------------------------------------------
void createNewContext(const std::string& aRenderer,
                      int aWindowID,
                	  int aVisibilityFlag,
                	  int aRendererMajorVersion,
                	  int aRendererMinorVersion,
                      unsigned int aMixedPrecisionFlag)
//-------------------------------------------------------
{
    g_mixed_precision_flag = aMixedPrecisionFlag;

    createWindow(aWindowID,
        aVisibilityFlag,
        aRenderer,
        aRendererMajorVersion,
        aRendererMinorVersion,
        aMixedPrecisionFlag);
}


//--------------------------------------------------------
void setWindowSize(int aWidth, int aHeight, int aWindowID)
//--------------------------------------------------------
{
    // Resize the last window that has been created
    if (aWindowID == -1)
        {
        // Nothing to do
        if (g_p_window_id_stack.empty() || g_p_window_set.empty())
            {
            gVirtualXRay::LOGGER.logWarning("No active window to resize.") << std::endl;
            }
        // Resize the last created window
        else
            {
            // Set the target window
            aWindowID = g_p_window_id_stack.back();
            }
        }

    // There is a window to resize
    if (aWindowID != -1)
        {
        // Find the window
        auto search = g_p_window_set.find(aWindowID);

        // It has been found
        if (search != g_p_window_set.end())
            {
            if (search->second)
                {
                search->second->setWindowSize(aWidth, aHeight);
                }
            }
        else
            {
            gVirtualXRay::LOGGER.logWarning("Window") <<
                aWindowID <<
                " not found." <<
                std::endl;
            }
        }
}


//-------------------------------------------
std::vector<int> getWindowSize(int aWindowID)
//-------------------------------------------
{
    std::vector<int> return_value(2, 0);

    // Resize the last window that has been created
    if (aWindowID == -1)
        {
        // Nothing to do
        if (g_p_window_id_stack.empty() || g_p_window_set.empty())
            {
            gVirtualXRay::LOGGER.logWarning("No active window to resize.") << std::endl;
            }
        // Resize the last created window
        else
            {
            // Set the target window
            aWindowID = g_p_window_id_stack.back();
            }
        }

    // There is a window to process
    if (aWindowID != -1)
        {
        // Find the window
        auto search = g_p_window_set.find(aWindowID);

        // It has been found
        if (search != g_p_window_set.end())
            {
            if (search->second)
                {
                std::pair<int, int> window_size = search->second->getWindowSize();
                return_value[0] = window_size.first;
                return_value[1] = window_size.second;
                }
            }
        else
            {
            gVirtualXRay::LOGGER.logWarning("Window") <<
                aWindowID <<
                " not found." <<
                std::endl;
            }
        }

    return return_value;
}


//-------------------------------
void destroyWindow(int aWindowID)
//-------------------------------
{
    // Destroy the last window that has been created
    if (aWindowID == -1)
        {
        // Nothing to do
        if (g_p_window_id_stack.empty() || g_p_window_set.empty())
            {
            gVirtualXRay::LOGGER.logWarning("No active window to destroy.") << std::endl;
            }
        // Destroy the last created window
        else
            {
            // Set the target window
            aWindowID = g_p_window_id_stack.back();
            }
        }

    // There is a window to destroy
    if (aWindowID != -1)
        {
        // Find the window
        auto search_window = g_p_window_set.find(aWindowID);

        // It has been found
        if (search_window != g_p_window_set.end())
            {
            gVirtualXRay::LOGGER.logNow("Destroy window ") << aWindowID <<
                "(" << search_window->second.get() << ")" << std::endl;

            if (search_window->second) search_window->second->destroyWindow();

            g_p_window_set.erase(search_window);
            }
        else
            {
            gVirtualXRay::LOGGER.logWarning("Window ") << aWindowID <<
                " not found in g_p_window_set." << std::endl;
            }

        // Unregister its ID from the stack of IDs
        // Find the ID
        std::vector<int>::iterator search_id = find(
            g_p_window_id_stack.begin(),
            g_p_window_id_stack.end(),
            aWindowID);

        // It has been found
        if (search_id != g_p_window_id_stack.end())
            {
            g_p_window_id_stack.erase(search_id);
            }
        else
            {
            gVirtualXRay::LOGGER.logWarning("Window ") << aWindowID <<
                " not found in g_p_window_id_stack." << std::endl;
            }
        }
}


//----------------------
void destroyAllWindows()
//----------------------
{
    gVirtualXRay::LOGGER.logNow("Destroy all the windows") << std::endl;

    // Process all the windows
    while (g_p_window_set.size())
    {
        // Destroy the window if possible
        destroyWindow(g_p_window_set.begin()->first);
    }
}


//----------------
void destroy(void)
//----------------
{
    gVirtualXRay::LOGGER.logNow("Release all the memory") << std::endl;
    gVirtualXRay::ResourceManager::getInstance().releaseResources();

    g_xray_beam.clear();
    g_xray_detector.release();
    g_xray_renderer.release();
    g_display_shader.release();

    g_p_scene_graph = 0;

    destroyAllWindows();

    g_renderer_initialised = false;
    g_beam_shape_initialised = false;
    g_beam_energy_initialised = false;
    g_source_position_initialised = false;
    g_detector_position_initialised = false;
    g_detector_up_vector_initialised = false;
    g_detector_size_initialised = false;
    g_detector_pixel_size_initialised = false;
}


#ifndef _WIN32
void terminate(void)
{
    gVirtualXRay::LOGGER.logNow("The function `terminate` will be removed in version 2.0.10. Please use `destroy` from now on.") << std::endl;
    destroy();
}
#endif


//---------------------------------------------
std::vector<unsigned int> getMax2DTextureSize()
//---------------------------------------------
{
    gVirtualXRay::Vec2ui max_texture_size = gVirtualXRay::getMax2DTextureSize();
    std::vector<unsigned int> return_value(2);

    return_value[0] = max_texture_size[0];
    return_value[1] = max_texture_size[1];

    return return_value;
}


//---------------------------------------------
std::vector<unsigned int> getMax3DTextureSize()
//---------------------------------------------
{
    gVirtualXRay::Vec3ui max_texture_size = gVirtualXRay::getMax3DTextureSize();
    std::vector<unsigned int> return_value(3);

    return_value[0] = max_texture_size[0];
    return_value[1] = max_texture_size[1];
    return_value[2] = max_texture_size[2];

    return return_value;
}
