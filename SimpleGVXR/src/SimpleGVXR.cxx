/*

Copyright (c) 2019-2023, Dr Franck P. Vidal, Bangor University, All rights reserved.
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
*   @file       SimpleGVXR.cxx
*
*   @brief      SimpleGVXR is an API to the bulk functionalities and classes of
*               gVirtualXRay. SimpleGVXR is used as the basis for
*               the language bindings generated using SWIG.
*
*   @date       18/04/2024
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
*   (c) by Dr Franck P. Vidal, Bangor University, Apr 2019-Sept 2023, version 1.1, BSD 3-Clause License
*   (c) by Prof Franck P. Vidal (franck.vidal@stfc.ac.uk), UK Research and Innovation, Oct 2023-present, version 1.1, BSD 3-Clause License
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
#include <cmath>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <cctype> // Needed for std::toupper
#include <cmath> // Header file for abs
#include <random>


#ifndef USE_EMSCRIPTEN
#define GLFW_INCLUDE_GLCOREARB 1
#endif

#ifdef HAS_VULKAN
#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif
#endif

#ifdef HAS_GLFW
#include <GLFW/glfw3.h>
#endif

// To perform the computations of the X-ray simulation
#ifndef GVXR_XRAY_RENDERER_H
#include "gVirtualXRay/XRayRenderer.h"
#endif

#ifndef GVXR_SCINTILLATOR_H
#include "gVirtualXRay/Scintillator.h"
#endif

// For polygon meshes
#ifndef GVXR_POLYGON_MESH_H
#include "gVirtualXRay/PolygonMesh.h"
#endif

// To load a scenegraph
#ifndef GVXR_SCENEGRAPH_BINDER_H
#include "gVirtualXRay/SceneGraphBinder.h"
#endif

// To load a scenegraph using ASSIMP
#ifndef GVXR_ASSIMP_SCENEGRAPH_BINDER_H
#include "gVirtualXRay/AssimpSceneGraphBinder.h"
#endif

// To load a scenegraph using OpenSceneGraph
#ifndef GVXR_OpenSceneGraphBinder_h
#include "gVirtualXRay/OpenSceneGraphBinder.h"
#endif

#ifndef GVXR_CUBE_MESH_H
#include "gVirtualXRay/CubeMesh.h"
#endif

#ifndef GVXR_CUBOID_MESH_H
#include "gVirtualXRay/CuboidMesh.h"
#endif

#ifndef GVXR_CYLINDER_MESH_H
#include "gVirtualXRay/CylinderMesh.h"
#endif

#ifndef GVXR_OUT_OF_BOUNDS_EXCEPTION_H
#include "gVirtualXRay/OutOfBoundsException.h"
#endif

#ifndef GVXR_NOT_IMPLEMENTED_EXCEPTION_H
#include "gVirtualXRay/NotImplementedException.h"
#endif

#ifndef GVXR_ELEMENT_SET_H
#include "gVirtualXRay/ElementSet.h"
#endif

#ifndef GVXR_PHOTON_CROSS_SECTION_H
#include "gVirtualXRay/PhotonCrossSection.h"
#endif

#ifndef GVXR_RESOURCE_MANAGER_H
#include <gVirtualXRay/ResourceManager.h>
#endif

#ifndef GVXR_LOGGER_H
#include <gVirtualXRay/Logger.h>
#endif

#ifndef GVXR_RESOURCE_MANAGER_H
#include "gVirtualXRay/ResourceManager.h"
#endif

#ifndef GVXR_COORDINATE_SYSTEM_MESH_H
#include "gVirtualXRay/CoordinateSystemMesh.h"
#endif


#ifndef SGVXR_SimpleGVXR_h
#include "SimpleGVXR.h"
#endif

#ifndef SGVXR_Internal_h
#include "internal.h"
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


//******************************************************************************
//  Namespace
//******************************************************************************


//******************************************************************************
//  Function declarations
//******************************************************************************
#ifdef HAS_GLFW
GLFWwindow* initGLFW(int aWindowID, int aVisibilityFlag);
#endif


void computeRotation(gVirtualXRay::MATRIX4& aRotationMatrix,
    int aViewportWidth,
    int aViewportHeight);

gVirtualXRay::Vec3<GLfloat> getArcballVector(int x,
    int y,
    int aViewportWidth,
    int aViewportHeight);

float radian2degree(float anAngle);

// std::vector<std::vector<float> > getVectorImage2D(
//     const gVirtualXRay::Image<float>& anImage);


gVirtualXRay::Image<float> getImage(const std::vector<std::vector<float> >& anImage);

gVirtualXRay::Image<float> getImage(const std::vector<std::vector<std::vector<float> > >& anImage);

gVirtualXRay::MATRIX4 Vector2gvxrMatrix(
    const std::vector<std::vector<float> >& aNumpyMatrix);

std::vector<std::vector<float> > gvxrMatrix2Vector(
    const gVirtualXRay::MATRIX4& aMatrix4);


#ifdef HAS_CUDA
void resampleUsingCUDA();
extern int g_samples_per_pixels;
extern gVirtualXRay::Image<float> g_cpu_memory;
#else
int g_samples_per_pixels = 1;
#endif


//******************************************************************************
//  Global variables
//******************************************************************************
const GLfloat ROTATION_SPEED(2.0);

std::map<int, std::unique_ptr<SimpleGVXR::Window> > g_p_window_set;      // The set of windows
std::vector<int> g_p_window_id_stack;           // The stack of window IDs

double g_initial_fovy(45);                // Field of view along
                                                // the y-axis
double g_initial_near(5.0 * gVirtualXRay::cm);  // Near clipping plane
double g_initial_far(500.0 * gVirtualXRay::cm); // Far clipping plane
GLfloat g_zoom(250.0 * gVirtualXRay::cm);       // How far is the camera from
                                                // the look at point
// State variables
bool g_run_loop = false;
bool g_display_beam(true);
bool g_display_wireframe(false);
bool g_use_lighing(true);
bool g_display_detector(true);
bool g_display_normal_vectors(false);

// bool g_detector_position_initialised = false;
// bool g_detector_up_vector_initialised = false;
// bool g_detector_size_initialised = false;
// bool g_detector_pixel_size_initialised = false;
bool g_renderer_initialised = false;

int g_button(-1);
int g_button_state(-1);
bool g_use_arc_ball(false);
GLint g_last_x_position(0);
GLint g_last_y_position(0);
GLint g_current_x_position(0);
GLint g_current_y_position(0);

extern gVirtualXRay::XRayDetector g_xray_detector; // X-ray detector
gVirtualXRay::XRayRenderer g_xray_renderer; // X-ray graphics engine
extern gVirtualXRay::XRayBeam g_xray_beam;         // X-ray beam

gVirtualXRay::Shader g_display_shader;      // Visualisation shader

// Scenegraph of scanned objects
std::unique_ptr<gVirtualXRay::SceneGraphBinder> g_p_scene_graph = 0;

// Rotate the whole 3-D view
gVirtualXRay::Matrix4x4<GLfloat> g_scene_rotation_matrix;

// Translate the whole 3-D view
gVirtualXRay::Matrix4x4 < GLfloat > g_scene_translation_matrix;

// Rotate the scanned objects
gVirtualXRay::Matrix4x4<GLfloat> g_sample_rotation_matrix;

// Bias/gain of the shift/scale filter for the display of the X-ray image
double g_shift_filter = 0;
double g_scale_filter = 1;

bool g_has_own_gl_context = false;


std::vector<std::vector<std::vector< float> > > lastest_screenshot;

gVirtualXRay::CoordinateSystemMesh g_world_coordinate_system_mesh(2 * gVirtualXRay::cm);

std::random_device g_random_device;  // Will be used to obtain a seed for the random number engine
std::mt19937 g_random_generator(g_random_device()); // Standard mersenne_twister_engine seeded with rd()

std::map<std::string, unsigned int> g_warning_message_counters;


//******************************************************************************
//  Function definitions
//******************************************************************************

//-----------------------------
int getMajorVersionOfCoreGVXR()
//-----------------------------
{
    return (atoi(gVirtualXRay_VERSION_MAJOR));
}

//-----------------------------
int getMinorVersionOfCoreGVXR()
//-----------------------------
{
    return (atoi(gVirtualXRay_VERSION_MINOR));
}

//-----------------------------
int getPatchVersionOfCoreGVXR()
//-----------------------------
{
    return (atoi(gVirtualXRay_VERSION_PATCH));
}

//--------------------------------
std::string getVersionOfCoreGVXR()
//--------------------------------
{
    return (std::string(gvxr_VERSION));
}

//-------------------------------
int getMajorVersionOfSimpleGVXR()
//-------------------------------
{
    return (SimpleGVXR_VERSION_MAJOR);
}

//-------------------------------
int getMinorVersionOfSimpleGVXR()
//-------------------------------
{
    return (SimpleGVXR_VERSION_MINOR);
}

//-------------------------------
int getPatchVersionOfSimpleGVXR()
//-------------------------------
{
    return (SimpleGVXR_VERSION_PATCH);
}

//----------------------------------
std::string getVersionOfSimpleGVXR()
//----------------------------------
{
    return (std::string(SimpleGVXR_VERSION_STRING));
}


//-----------------------------
std::string getOpenGlRenderer()
//-----------------------------
{
    std::string returned_value("Unknown");

#ifdef HAS_OPENGL
    const char* value_c_str = reinterpret_cast<const char *>(glGetString(GL_RENDERER));
    if (value_c_str) returned_value = value_c_str;
#endif

    return returned_value;
}


//----------------------------
std::string getOpenGlVersion()
//----------------------------
{
    std::string returned_value("Unknown");

#ifdef HAS_OPENGL
    const char* value_c_str = reinterpret_cast<const char *>(glGetString(GL_VERSION));
    if (value_c_str) returned_value = value_c_str;
#endif

    return returned_value;
}


//---------------------------
std::string getOpenGlVendor()
//---------------------------
{
    std::string returned_value("Unknown");

#ifdef HAS_OPENGL
    const char* value_c_str = reinterpret_cast<const char *>(glGetString(GL_VENDOR));
    if (value_c_str) returned_value = value_c_str;
#endif

    return returned_value;
}


//-------------------------------------
std::string getShadingLanguageVersion()
//-------------------------------------
{
    std::string returned_value("Unknown");

#ifdef HAS_OPENGL
    const char* value_c_str = reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));
    if (value_c_str) returned_value = value_c_str;
#endif

    return returned_value;
}


//-----------------------------
std::string getNoiseGenerator()
//-----------------------------
{
    std::string returned_value("gVXR's own implementation");

    #ifdef HAS_OPENMP
        returned_value += " with OpenMP (" + std::to_string(omp_get_max_threads()) + " threads)";
    #else
        returned_value += " without OpenMP";
    #endif

    return returned_value;
}


//-------------------------------
std::string getBuildEnvironment()
//-------------------------------
{
    std::stringstream message;
    

    message << "Has NDEBUG:";
#ifdef NDEBUG
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has WIN32:";
#ifdef HAS_WIN32
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has __APPLE__:";
#ifdef __APPLE__
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has GSL:";
#ifdef HAS_GSL
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has OpenMP:";
#ifdef HAS_OPENMP
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has CUDA:";
#ifdef HAS_CUDA
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has Vulkan:";
#ifdef HAS_VULKAN
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has EGL:";
#ifdef HAS_EGL
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has libGBM:";
#ifdef HAS_GBM
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has EGL_DRM_DEVICE_FILE_EXT:";
#ifdef EGL_DRM_DEVICE_FILE_EXT
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has EGL_CUDA_DEVICE_NV:";
#ifdef EGL_CUDA_DEVICE_NV
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has GLU:";
#ifdef HAS_GLU
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has GLEW:";
#ifdef HAS_GLEW
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has GLAD:";
#ifdef HAS_GLAD
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has GLX:";
#ifdef HAS_GLX
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has ASSIMP:";
#ifdef HAS_ASSIMP
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has OpenSceneGraph:";
#ifdef HAS_OPENSCENEGRAPH
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has Built-in Python support:";
#if defined(USE_PYTHON) && defined(HAS_PythonLibs)
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has ITK:";
#ifdef HAS_ITK
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has GCDM:";
#ifdef HAS_GCDM
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has LibTIFF:";
#if defined(HAS_TIFF) && defined(HAS_LIBTIFF)
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    message << "Has FreeType:";
#ifdef HAS_FREETYPE
    message << "true" << std::endl;
#else
    message << "false" << std::endl;
#endif

    return message.str();
}


//------------------------------------
void useLogFile(const char* aFileName)
//------------------------------------
{
    // A filename has been provided
    if (aFileName)
    {
        if (gVirtualXRay::LOGGER.getFilename() != aFileName)
        {
            gVirtualXRay::LOGGER.open(aFileName);
        }
    }
    // A filename has not been provided
    else
    {
        gVirtualXRay::LOGGER.close();
    }
}


//-------------------------------------------
void useLogFile(const std::string& aFileName)
//-------------------------------------------
{
    if (aFileName.empty()) useLogFile(nullptr);
    else useLogFile(aFileName.c_str());
}


//---------------------------------------------
void setSeed(unsigned int aRandomGeneratorSeed)
//---------------------------------------------
{
    g_random_generator = std::mt19937(aRandomGeneratorSeed);
}


//---------------------------
void displayBeam(bool aState)
//---------------------------
{
    g_display_beam = aState;
}


//-------------------------------
void displayDetector(bool aState)
//-------------------------------
{
    g_display_detector = aState;
}


//------------------------------------
void displayNormalVectors(bool aState)
//------------------------------------
{
    g_display_normal_vectors = aState;
}


//----------------------------
void useWireframe(bool aState)
//----------------------------
{
    g_display_wireframe = aState;
}


//--------------------------
void useLighing(bool aState)
//--------------------------
{
    g_use_lighing = aState;
}


//---------------------------
void useNegative(bool aState)
//---------------------------
{
    g_xray_renderer.useNegativeFilteringFlag(aState);
}


//------------------------------------------------------------
void setAxisLength(float aLength, const std::string& aUnitOfLength)
//------------------------------------------------------------
{
    g_world_coordinate_system_mesh.setLength(aLength * getUnitOfLength(aUnitOfLength));
}


//-------------------------------------------------------------------
void setMarkerLength(float aLength, const std::string& aUnitOfLength)
//-------------------------------------------------------------------
{
    g_xray_detector.setWidgetLength(aLength * getUnitOfLength(aUnitOfLength));
}


//--------------------------------------
void autoCreateOpenGLContext(bool aFlag)
//--------------------------------------
{
    g_has_own_gl_context = !aFlag;
}








//-----------------------------------------
void displayNode(const std::string& aLabel)
//-----------------------------------------
{
    std::vector<float> colour(4, 1.0);

    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        p_node->display();
        }
    else
        {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            std::endl;
        }
}


void resetSceneTransformation()
{
    g_sample_rotation_matrix = gVirtualXRay::MATRIX4();
}


void scaleScene(double x,
                double y,
                double z)
{
    g_sample_rotation_matrix *= gVirtualXRay::MATRIX4::buildScaleMatrix(
        x, y, z);
}


void scaleScene(double x,
                double y,
                double z,
                const std::string& aUnitOfLength)
{
    double unit_of_length = getUnitOfLength(aUnitOfLength);
    scaleScene(x * unit_of_length,
               y * unit_of_length,
               z * unit_of_length);
}


void rotateScene(double angle, double x, double y, double z)
{
    // Move the scenegraph
    g_sample_rotation_matrix *= gVirtualXRay::MATRIX4::buildRotationMatrix(
        angle, x, y, z);
}


void rotateScene(double yaw, double pitch, double roll)
{
    // Move the scenegraph
    g_sample_rotation_matrix *= gVirtualXRay::MATRIX4::buildYawPitchRollRotationMatrix(
        yaw, pitch, roll);
}


void translateScene(double x,
                    double y,
                    double z,
                    const std::string& aUnitOfLength)
{
    // Move the scenegraph
    double unit_of_length = getUnitOfLength(aUnitOfLength);
    g_sample_rotation_matrix *= gVirtualXRay::MATRIX4::buildTranslationMatrix(
        x * unit_of_length,
        y * unit_of_length,
        z * unit_of_length);
}


std::vector<std::vector<float> > getSceneTransformationMatrix()
{
    return gvxrMatrix2Vector(g_sample_rotation_matrix);
}


std::vector<std::vector<float> > getRootTransformationMatrix()
{
    if (!g_p_scene_graph.get())
        {
        throw gVirtualXRay::Exception(__FILE__,
                                      __FUNCTION__,
                                      __LINE__,
                                      "The scenegraph is empty.");
        }

    return (gvxrMatrix2Vector(
        g_p_scene_graph->getRootNodeTransformationMatrix()));
}


std::vector<std::vector<float> > getNodeLocalTransformationMatrix(
        const std::string& aLabel)
{
    gVirtualXRay::MATRIX4 transformation;

    if (g_p_scene_graph.get())
        {
        // Find the mesh
        gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

        // It has been found
        if (p_node)
            {
            transformation = p_node->getLocalTransformationMatrix();
            }
        // It has not been found
        else
            {
            gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
                aLabel <<
                " not found in g_p_polygon_mesh_set." <<
                std::endl;
            }
        }
    else
        {
        gVirtualXRay::LOGGER.logWarning("The scenegraph is empty.") << std::endl;
        }

    return (gvxrMatrix2Vector(transformation));
}


std::vector<std::vector<float> > getNodeWorldTransformationMatrix(
        const std::string& aLabel)
{
    gVirtualXRay::MATRIX4 transformation;

    if (g_p_scene_graph.get())
        {
        // Find the mesh
        gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

        // It has been found
        if (p_node)
            {
            transformation = p_node->getWorldTransformationMatrix();
            }
        // It has not been found
        else
            {
            gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
                aLabel <<
                " not found in g_p_polygon_mesh_set." <<
                std::endl;
            }
        }
    else
        {
        gVirtualXRay::LOGGER.logWarning("The scenegraph is empty.") << std::endl;
        }

    return (gvxrMatrix2Vector(transformation));
}


void setSceneTransformationMatrix(
        const std::vector<std::vector<float> >& aMatrix)
{
    g_sample_rotation_matrix = Vector2gvxrMatrix(aMatrix);
}

void setRootTransformationMatrix(
        const std::vector<std::vector<float> >& aMatrix)
{
    if (g_p_scene_graph.get())
        {
        g_p_scene_graph->setRootNodeTransformationMatrix(
        Vector2gvxrMatrix(aMatrix));
        }
    else
        {
        gVirtualXRay::LOGGER.logWarning("The scenegraph is empty.") << std::endl;
        }
}

void setNodeTransformationMatrix(const std::string& aLabel,
        const std::vector<std::vector<float> >& aMatrix)
{
    if (g_p_scene_graph.get())
        {
        // Find the mesh
        gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

        // It has been found
        if (p_node)
            {
            p_node->setLocalTransformation(Vector2gvxrMatrix(aMatrix));
            }
        // It has not been found
        else
            {
            gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
                aLabel <<
                " not found in g_p_polygon_mesh_set." <<
                std::endl;
            }
        }
    else
        {
        gVirtualXRay::LOGGER.logWarning("The scenegraph is empty.") << std::endl;
        }
}

//---------------------------------------------
void setHU(const std::string& aLabel, short HU)
//---------------------------------------------
{
    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Set its properties
        p_node->getPolygonMesh().setHounsfieldValue(HU);
        g_xray_renderer.needUpdateSurfacePerMaterial();
        }
    // It has not been found
    else
        {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            std::endl;
        }
}

//---------------------------------------------------------
void setHounsfieldUnit(const std::string& aLabel, short HU)
//---------------------------------------------------------
{
    setHU(aLabel, HU);
}

//----------------------------------------------------------
void setHounsfieldValue(const std::string& aLabel, short HU)
//----------------------------------------------------------
{
    setHU(aLabel, HU);
}

//----------------------------------------------------------
void setElement(const std::string& aLabel, unsigned short Z)
//----------------------------------------------------------
{
    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Set its properties
        p_node->getPolygonMesh().setElement(Z);
        g_xray_renderer.needUpdateSurfacePerMaterial();
        }
    // It has not been found
    else
        {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            std::endl;
        }
}

//------------------------------------------------------------------
void setElement(const std::string& aLabel, const std::string& aName)
//------------------------------------------------------------------
{
    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Set its properties
        p_node->getPolygonMesh().setElement(aName);
        g_xray_renderer.needUpdateSurfacePerMaterial();
        }
    // It has not been found
    else
        {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            std::endl;
        }
}

//------------------------------------------------------------------
void setMixture(const std::string& aLabel, const std::string& aName)
//------------------------------------------------------------------
{
    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Set its properties
        p_node->getPolygonMesh().setMixture(aName);
        g_xray_renderer.needUpdateSurfacePerMaterial();
        }
    // It has not been found
    else
        {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            std::endl;
        }
}


//----------------------------------------------------
void setMixture(const std::string& aLabel,
                const std::vector<int>& aZNumberSet,
                const std::vector<double>& aWeightSet)
//----------------------------------------------------
{
    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Set its properties
        std::map<int, double> mixture;

        if (aZNumberSet.size() != aWeightSet.size())
            {
            throw gVirtualXRay::Exception(
                __FILE__,
                __FUNCTION__,
                __LINE__,
                "ERROR: The Z number set and weight set don't have the same size.");
            }

        for (unsigned int i = 0; i != aZNumberSet.size(); ++i)
            {
            mixture[aZNumberSet[i]] = aWeightSet[i];
            }

        p_node->getPolygonMesh().setMixture(mixture);
        g_xray_renderer.needUpdateSurfacePerMaterial();
        }
    // It has not been found
    else
        {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            std::endl;
        }
}


//---------------------------------------------------------
void setMixture(const std::string& aLabel,
                const std::vector<std::string>& aSymbolSet,
                const std::vector<double>& aWeightSet)
//---------------------------------------------------------
{
  // Find the mesh
  gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

  // It has been found
  if (p_node)
      {
      // Set its properties
      std::map<std::string, double> mixture;

      if (aSymbolSet.size() != aWeightSet.size())
          {
          throw gVirtualXRay::Exception(
              __FILE__,
              __FUNCTION__,
              __LINE__,
              "ERROR: The symbol set and weight set don't have the same size.");
          }

      for (unsigned int i = 0; i != aSymbolSet.size(); ++i)
          {
          mixture[aSymbolSet[i]] = aWeightSet[i];
          }

      p_node->getPolygonMesh().setMixture(mixture);
      g_xray_renderer.needUpdateSurfacePerMaterial();
      }
  // It has not been found
  else
      {
      gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
          aLabel <<
          " not found in g_p_polygon_mesh_set." <<
          std::endl;
      }
}


//-------------------------------------------------------------------
void setCompound(const std::string& aLabel, const std::string& aName)
//-------------------------------------------------------------------
{
    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Set its properties
        p_node->getPolygonMesh().setCompound(aName);
        g_xray_renderer.needUpdateSurfacePerMaterial();
        }
    // It has not been found
    else
        {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            std::endl;
        }
}

//-----------------------------------------------------------
void setMassAttenuationCoefficient(const std::string& aLabel,
                                   double aCoefficient,
                                   const std::string& aUnit)
//-----------------------------------------------------------
{
    if (aUnit != "cm2/g" && aUnit != "cm2.g-1")
        {
        throw gVirtualXRay::Exception(
            __FILE__,
            __FUNCTION__,
            __LINE__,
            "ERROR: Unknown unit of mass attenuation coefficient.");
        }

    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        // Set its properties
        p_node->getPolygonMesh().setMassAttenuationCoefficient(
        aCoefficient * gVirtualXRay::cm2 / gVirtualXRay::g);
        g_xray_renderer.needUpdateSurfacePerMaterial();
        }
    // It has not been found
    else
        {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            std::endl;
        }
}

//-------------------------------------------------------------
void setLinearAttenuationCoefficient(const std::string& aLabel,
                                     double aCoefficient,
                                     const std::string& aUnit)
//-------------------------------------------------------------
{
    if (aUnit != "cm-1")
        {
        throw gVirtualXRay::Exception(
            __FILE__,
            __FUNCTION__,
            __LINE__,
            "ERROR: Unknown unit of linear attenuation coefficient.");
        }

    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
          if (aUnit.substr(aUnit.length() - 2) != "-1")
          {
              throw gVirtualXRay::Exception(
                  __FILE__,
                  __FUNCTION__,
                  __LINE__,
                  "ERROR: Unknown unit of linear attenuation coefficient.");
          }

          // Set its properties
          double inv_unit_of_length = 1.0 / getUnitOfLength(aUnit.substr(0, aUnit.length() - 2));
          p_node->getPolygonMesh().setLinearAttenuationCoefficient(
            aCoefficient * inv_unit_of_length);
          g_xray_renderer.needUpdateSurfacePerMaterial();
        }
    // It has not been found
    else
        {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            std::endl;
        }
}

//-----------------------------------
void setMu(const std::string& aLabel,
           double aCoefficient,
           const std::string& aUnit)
//-----------------------------------
{
    setLinearAttenuationCoefficient(aLabel, aCoefficient, aUnit);
}

//----------------------------------------
void setDensity(const std::string& aLabel,
                double aDensity,
                const std::string& aUnit)
//----------------------------------------
{
    if (aUnit != "g/cm3" && aUnit != "g.cm-3" &&
        aUnit != "mg/cm3" && aUnit != "mg.cm-3" &&
        aUnit != "kg/m3" && aUnit != "kg.m-3")
    {
        throw gVirtualXRay::Exception(
            __FILE__,
            __FUNCTION__,
            __LINE__,
            "ERROR: Unknown unit of density.");
    }

    double density_unit = 0.0;
    double volume_unit = 0.0;

    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
    {
        if (aUnit == "g/cm3" || aUnit == "g.cm-3")
        {
            density_unit = gVirtualXRay::g;
            volume_unit = gVirtualXRay::cm3;
        }
        else if (aUnit == "mg/cm3" || aUnit == "mg.cm-3")
        {
            density_unit = gVirtualXRay::mg;
            volume_unit = gVirtualXRay::cm3;
        }
        else if (aUnit == "kg/m3" || aUnit == "kg.m-3")
        {
            density_unit = gVirtualXRay::kg;
            volume_unit = gVirtualXRay::m3;
        }

        // Set its properties
        p_node->getPolygonMesh().setDensity(
            aDensity * density_unit / volume_unit);

        g_xray_renderer.needUpdateSurfacePerMaterial();
    }
    // It has not been found
    else
    {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            std::endl;
    }
}

//------------------------------------------
double getDensity(const std::string& aLabel)
//------------------------------------------
{
    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        return p_node->getPolygonMesh().getDensity()
        / (gVirtualXRay::g / gVirtualXRay::cm3);
        }
    // It has not been found
    else
        {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            std::endl;
        }

    return 0;
}


//--------------------------------------------------------------------
double getMassAttenuationCoefficient(const std::string& aLabel,
                                     double anEnergy,
                                     const std::string& aUnitOfEnergy)
//--------------------------------------------------------------------
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        return (p_node->getPolygonMesh().getPhotonCrossSection().
                getMassAttenuationCoefficient(anEnergy *
                        getUnitOfEnergy(aUnitOfEnergy)) /
                (gVirtualXRay::cm2 / gVirtualXRay::g));
        }
    // It has not been found
    else
        {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            std::endl;
        }

    return 0;
}


//----------------------------------------------------------------------
double getLinearAttenuationCoefficient(const std::string& aLabel,
                                       double anEnergy,
                                       const std::string& aUnitOfEnergy)
//----------------------------------------------------------------------
{
    // The scenegraph does not exist
    if (!g_p_scene_graph.get())
        {
        // Create a scenegraph
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        return (p_node->getPolygonMesh().getPhotonCrossSection().
                getLinearAttenuationCoefficient(anEnergy *
                    getUnitOfEnergy(aUnitOfEnergy)) /
                (1.0 / gVirtualXRay::cm));
        }
    // It has not been found
    else
        {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            std::endl;
        }

    return 0;
}

//----------------------------------------------
std::string getMaterialLabel(const std::string& aLabel)
//----------------------------------------------
{
    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        return p_node->getPolygonMesh().getMaterialLabel();
        }
    // It has not been found
    else
        {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            std::endl;
        }

    return std::string();
}


//------------------------------------------------
std::string getCompound(const std::string& aLabel)
//------------------------------------------------
{
    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
        {
        return p_node->getPolygonMesh().getCompound();
        }
    // It has not been found
    else
        {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            std::endl;
        }

    return std::string();
}


//--------------------------------------------------------------
std::vector<int> getMixtureElementSet(const std::string& aLabel)
//--------------------------------------------------------------
{
    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
    {
        return p_node->getPolygonMesh().getMixtureElementSet();
    }
    // It has not been found
    else
    {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            std::endl;
    }

    return std::vector<int>();
}


//----------------------------------------------------------------
std::vector<double> getMixtureWeightSet(const std::string& aLabel)
//----------------------------------------------------------------
{
    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
    {
        return p_node->getPolygonMesh().getMixtureWeightSet();
    }
    // It has not been found
    else
    {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            std::endl;
    }

    return std::vector<double>();
}


//--------------------------------------------------
float getEffectiveZNumber(const std::string& aLabel)
//--------------------------------------------------
{
    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
    {
        return p_node->getPhotonCrossSection().getEffectiveZNumber();
    }
    // It has not been found
    else
    {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            std::endl;
    }

    return 0.0;
}


//-------------------------------------------------------
float getEffectiveAtomicNumber(const std::string& aLabel)
//-------------------------------------------------------
{
    return getEffectiveZNumber(aLabel);
}


//----------------------------------------------------
float getEffectiveMolarMass(const std::string& aLabel)
//----------------------------------------------------
{
    // Find the mesh
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // It has been found
    if (p_node)
    {
        return p_node->getPhotonCrossSection().getEffectiveMolarMass();
    }
    // It has not been found
    else
    {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            std::endl;
    }

    return 0.0;
}


//-------------------------------------------------------
void displayScene(bool aSceneRotationFlag, int aWindowID)
//-------------------------------------------------------
{
    // Display in the last window that has been created
    if (aWindowID == -1 && !g_has_own_gl_context)
        {
        // Nothing to do
        if (g_p_window_id_stack.empty() || g_p_window_set.empty())
            {
            gVirtualXRay::LOGGER.logWarning("No active window to use.") << std::endl;
            }
        // Destroy the last created window
        else
            {
            // Set the target window
            aWindowID = g_p_window_id_stack.back();
            }
        }

    // There is a window to use
    if (aWindowID != -1 || g_has_own_gl_context)
        {
        // Find the window
        auto search_window = g_p_window_set.find(aWindowID);

        // It has been found
        if (search_window != g_p_window_set.end() || g_has_own_gl_context)
            {
            if (search_window->second || g_has_own_gl_context)
                {
                search_window->second->displayScene(aSceneRotationFlag);
                }

            // Poll for and process events
#ifdef HAS_GLFW
            if (!g_has_own_gl_context)
            {
                glfwPollEvents();
            }
#endif
            }
        else
            {
            gVirtualXRay::LOGGER.logWarning("Window") <<
                aWindowID <<
                " not found in g_p_window_set." <<
                std::endl;
            }
        }
}

//----------------------------
void renderLoop(int aWindowID)
//----------------------------
{
    // Display in the last window that has been created
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

    // There is a window to use
    if (aWindowID != -1)
        {
        // Find the window
        auto search_window = g_p_window_set.find(aWindowID);

        // It has been found
        if (search_window != g_p_window_set.end())
            {
            if (search_window->second)
                {
                search_window->second->renderLoop();
                }
            }
        else
            {
            gVirtualXRay::LOGGER.logWarning("Window ") <<
                aWindowID <<
                " not found in g_p_window_set." <<
                std::endl;
            }
        }
}

//----------------------------
void showWindow(int aWindowID)
//----------------------------
{
    // Resize the last window that has been created
    if (aWindowID == -1)
        {
        // Nothing to do
        if (g_p_window_id_stack.empty() || g_p_window_set.empty())
            {
            gVirtualXRay::LOGGER.logWarning("No active window to show.") << std::endl;
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
                search->second->showWindow();
                }
            }
        else
            {
            gVirtualXRay::LOGGER.logWarning("Window ") << aWindowID <<
                " not found." << std::endl;
            }
        }
}

//----------------------------
void hideWindow(int aWindowID)
//----------------------------
{
    // Hide the last window that has been created
    if (aWindowID == -1)
        {
        // Nothing to do
        if (g_p_window_id_stack.empty() || g_p_window_set.empty())
            {
            gVirtualXRay::LOGGER.logWarning("No active window to hide.") << std::endl;
            }
        // Hide the last created window
        else
            {
            // Set the target window
            aWindowID = g_p_window_id_stack.back();
            }
        }

    // There is a window to hide
    if (aWindowID != -1)
        {
        // Find the window
        auto search = g_p_window_set.find(aWindowID);

        // It has been found
        if (search != g_p_window_set.end())
            {
            if (search->second)
                {
                search->second->hideWindow();
                }
            }
        else
            {
            gVirtualXRay::LOGGER.logWarning("Window ") << aWindowID <<
                " not found." << std::endl;
            }
        }
}


//-------------------------------------------------------------------
void setWindowPosition(int aPositionX, int aPositionY, int aWindowID)
//-------------------------------------------------------------------
{
    // Move the last window that has been created
    if (aWindowID == -1)
        {
        // Nothing to do
        if (g_p_window_id_stack.empty() || g_p_window_set.empty())
            {
            gVirtualXRay::LOGGER.logWarning("No active window to hide.") << std::endl;
            }
        // Move the last created window
        else
            {
            // Set the target window
            aWindowID = g_p_window_id_stack.back();
            }
        }

    // There is a window to move
    if (aWindowID != -1)
        {
        // Find the window
        auto search = g_p_window_set.find(aWindowID);

        // It has been found
        if (search != g_p_window_set.end())
            {
            if (search->second)
                {
                search->second->setPosition(aPositionX, aPositionY);
                }
            }
        else
            {
            gVirtualXRay::LOGGER.logWarning("Window ") << aWindowID <<
                " not found." << std::endl;
            }
        }
}


//----------------------------------------------------------------------
void setWindowBackGroundColour(float R, float G, float B, int aWindowID)
//----------------------------------------------------------------------
{
    // Change the colour of the last window that has been created
    if (aWindowID == -1)
        {
        // Nothing to do
        if (g_p_window_id_stack.empty() || g_p_window_set.empty())
            {
            gVirtualXRay::LOGGER.logWarning("No active window to hide.") << std::endl;
            }
        // Change the colour of the last window that has been created
        else
            {
            // Set the target window
            aWindowID = g_p_window_id_stack.back();
            }
        }

    // There is a window to change
    if (aWindowID != -1)
        {
        // Find the window
        auto search = g_p_window_set.find(aWindowID);

        // It has been found
        if (search != g_p_window_set.end())
            {
            if (search->second)
                {
                search->second->setWindowBackGroundColour(R, G, B);
                }
            }
        else
            {
            gVirtualXRay::LOGGER.logWarning("Window ") << aWindowID <<
                " not found." << std::endl;
            }
        }
}

//---------------------------------------------------------------------
void setWindowBackGroundColor(float R, float G, float B, int aWindowID)
//---------------------------------------------------------------------
{
    setWindowBackGroundColour(R, G, B, aWindowID);
}


//----------------------------------------------------------------------------------
const std::vector<std::vector<std::vector< float> > >& takeScreenshot(int aWindowID)
//----------------------------------------------------------------------------------
{
    // Retrieve the window
    if (aWindowID == -1)
        {
        // Nothing to do
        if (g_p_window_id_stack.empty() || g_p_window_set.empty())
            {
            gVirtualXRay::LOGGER.logWarning("No active window to hide.") << std::endl;
            }
        // Change the colour of the last window that has been created
        else
            {
            // Set the target window
            aWindowID = g_p_window_id_stack.back();
            }
        }

    // There is a window to capture
    if (aWindowID != -1)
        {
        // Find the window
        auto search = g_p_window_set.find(aWindowID);

        // It has been found
        if (search != g_p_window_set.end())
            {
            if (search->second)
                {
                search->second->takeScreenshot();
                }
            }
        else
            {
            gVirtualXRay::LOGGER.logWarning("Window ") << aWindowID <<
                " not found." << std::endl;
            }
        }

    // Return the image
    return lastest_screenshot;
}


//--------------------------------------------------------------------------
const std::vector<std::vector<std::vector< float> > >& getLatestScreenshot()
//--------------------------------------------------------------------------
{
    return lastest_screenshot;
}


//-----------------------------------------------
std::vector<std::vector< float> > getWhiteImage(bool anIntegrateEnergyFlag)
//-----------------------------------------------
{
    return (getVectorImage2D<float>(g_xray_renderer.getWhiteImage(anIntegrateEnergyFlag)));
}


//------------------------------------------------------------------------------------------
std::vector<std::vector< unsigned short> > getWhiteImageWithGain(bool anIntegrateEnergyFlag)
//------------------------------------------------------------------------------------------
{
    return (getVectorImage2D<unsigned short>(g_xray_renderer.getWhiteImageWithGain(anIntegrateEnergyFlag)));
}


//---------------------------------------------------------------------
std::vector<std::vector<std::vector< float> > > getWhiteSpectralImage()
//---------------------------------------------------------------------
{
    std::vector<std::vector<std::vector< float> > > p_white_image_set;

    for (unsigned int i = 0; i < g_xray_beam.getNumberOfPhotonCountingBands(); ++i)
    {
        g_xray_beam.activatePhotonCountingBand(i);
        p_white_image_set.push_back(getVectorImage2D<float>(g_xray_renderer.getWhiteImage(false)));
    }

    return p_white_image_set;
}


//---------------------------------------------------------------------
std::vector<std::vector<std::vector< unsigned short> > > getWhiteSpectralImageWithGain()
//---------------------------------------------------------------------
{
    throw gVirtualXRay::NotImplementedException(__FILE__, __FUNCTION__, __LINE__);
    // std::vector<std::vector<std::vector< float> > > p_white_image_set;
    //
    // for (unsigned int i = 0; i < g_xray_beam.getNumberOfPhotonCountingBands(); ++i)
    // {
    //     g_xray_beam.activatePhotonCountingBand(i);
    //     p_white_image_set.push_back(getVectorImage2D<float>(g_xray_renderer.getWhiteImage(false)));
    // }
    //
    // return p_white_image_set;
}


//--------------------------------------------
void saveSTLfile(const std::string& aLabel,
                 const std::string& aFileName,
                 bool aBinaryFlag)
//--------------------------------------------
{
	// Create the file name
	std::string file_name;
	if (aFileName.size() == 0)
	{
		file_name = aLabel + ".stl";
	}
	else
	{
		file_name = aFileName;
	}

	// Get the scenegraph
    gVirtualXRay::SceneGraphBinder* p_temp = g_p_scene_graph.get();
    if (!p_temp)
        {
        g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
        }

    // Get the node
    gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aLabel);

    // The node exists
    if (p_node)
        {
        // Set the file name
    	p_node->getPolygonMesh().setFileName(file_name);

        // save the file
    	p_node->getPolygonMesh().writeSTLFile(aBinaryFlag);
        }
    // The node does not exist
    else
        {
		gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
			aLabel <<
			" not found in g_p_polygon_mesh_set." <<
		    std::endl;
        }
}



//-----------------------------------------------------------
void setPhotonCountingBands(double aMinEnergy,
                            double aMaxEnergy,
                            const std::string& aUnitOfEnergy,
                            unsigned int aNumberOfBands)
//-----------------------------------------------------------
{
    g_xray_beam.setPhotonCountingBands(
        aMinEnergy * getUnitOfEnergy(aUnitOfEnergy),
        aMaxEnergy * getUnitOfEnergy(aUnitOfEnergy),
        aNumberOfBands);
}


//-------------------------------------------
unsigned int getNumberOfPhotonCountingBands()
//-------------------------------------------
{
    return g_xray_beam.getNumberOfPhotonCountingBands();
}


//-----------------------------
void mergePhotonCountingBands()
//-----------------------------
{
    g_xray_beam.mergePhotonCountingBands();
}



//--------------------------------------------------------------------------
std::vector<double> getPhotonCountingBands(const std::string& aUnitOfEnergy)
//--------------------------------------------------------------------------
{
    std::vector<double> p_temp;
    double energy = getUnitOfEnergy(aUnitOfEnergy);

    for (std::vector<gVirtualXRay::RATIONAL_NUMBER>::const_iterator ite = g_xray_beam.getPhotonCountingBands().begin();
        ite != g_xray_beam.getPhotonCountingBands().end();
        ++ite)
    {
        p_temp.push_back(*ite / energy);
    }

    return (p_temp);
}






//------------------------------------------------------------------------------------------
std::vector<std::pair<unsigned int, float> > getFiltration(const std::string& aUnitOfLength)
//------------------------------------------------------------------------------------------
{
    std::vector<std::pair<unsigned int, float> > p_filtration;

    for (std::vector<std::pair<unsigned int, gVirtualXRay::RATIONAL_NUMBER> >::const_iterator ite(g_xray_beam.getFiltration().begin());
        ite != g_xray_beam.getFiltration().end();
        ++ite)
    {
        p_filtration.push_back(std::pair<unsigned int, float>(ite->first, float(ite->second / getUnitOfLength(aUnitOfLength))));
    }

    return p_filtration;
}



//----------------------------------------------
unsigned int getMaxRadiusLbufferArtefactFilter()
//----------------------------------------------
{
    return g_xray_renderer.getMaxRadiusLbufferArtefactFilter();
}


//------------------------------------
bool isArtefactFilteringOnGPUEnabled()
//------------------------------------
{
    return g_xray_renderer.useArtefactFilteringOnGPU();
}


//------------------------------------
bool isArtefactFilteringOnCPUEnabled()
//------------------------------------
{
    return g_xray_renderer.useArtefactFilteringOnCPU();
}


//----------------------------------------------------------
void setMaxRadiusLbufferArtefactFilter(unsigned int aRadius)
//----------------------------------------------------------
{
    g_xray_renderer.setMaxRadiusLbufferArtefactFilter(aRadius);
}




int getImageWidth(const std::vector<std::vector<float> >& anImage) {
    int image_width = 0;

    if (getImageHeght(anImage)) {
    image_width = anImage[0].size();
    }

    return (image_width);
}

int getImageHeght(const std::vector<std::vector<float> >& anImage) {
    return (anImage.size());
}

std::vector<float> getImageRow(const std::vector<std::vector<float> >& anImage,
    int j) {
    if (j >= getImageHeght(anImage)) {
    throw gVirtualXRay::OutOfBoundsException(__FILE__, __FUNCTION__,
    __LINE__);
    }

    return (anImage[j]);
}



void startArcBallRotation(double x, double y) {
    g_last_x_position = x;
    g_last_y_position = y;

    g_current_x_position = x;
    g_current_y_position = y;

    g_use_arc_ball = true;
}

void stopArcBallRotation() {
    g_use_arc_ball = false;
}

bool usingArcBallRotation() {
    return (g_use_arc_ball);
}

void cursorPositionCallback(double x, double y, int aViewportWidth,
    int aViewportHeight) {
    g_current_x_position = x;
    g_current_y_position = y;

    computeRotation(g_scene_rotation_matrix, aViewportWidth, aViewportHeight);
}


//-------------------------------------------------
void scrollCallback(double xoffset, double yoffset)
//-------------------------------------------------
{
    // Scrolling along the Y-axis
    if (fabs(yoffset) > EPSILON)
    {
		g_use_arc_ball = false;
		g_zoom += yoffset * gVirtualXRay::cm;

		gVirtualXRay::loadLookAtModelViewMatrix(0.0, 0.0, g_zoom,
		                                        0.0, 0.0, 0.0,
		                                        0.0, 1.0, 0.0);
    }
}


//----------------------------
void setZoom(float aZoomValue)
//----------------------------
{
    g_zoom = aZoomValue;

    gVirtualXRay::loadLookAtModelViewMatrix(0.0, 0.0, g_zoom,
                                            0.0, 0.0, 0.0,
                                            0.0, 1.0, 0.0);
}


//-------------
float getZoom()
//-------------
{
    return g_zoom;
}


void setSceneRotationMatrix(const std::vector<double>& aRotationMatrix)
{
    g_scene_rotation_matrix = gVirtualXRay::Matrix4x4<GLfloat>(aRotationMatrix);
}

std::vector<double> getSceneRotationMatrix()
{
    return g_scene_rotation_matrix.getAsVector();
}


#ifdef HAS_GLFW
//--------------------------------------------
void mouseButtonCallback(GLFWwindow* apWindow, int aButton, int aButtonState,
    int aModifierKey)
//--------------------------------------------
{
    g_button = aButton;
    g_button_state = aButtonState;

    // Use the arc ball
    if (g_button_state == GLFW_PRESS) {
    double xpos(0);
    double ypos(0);
    glfwGetCursorPos(apWindow, &xpos, &ypos);

    startArcBallRotation(xpos, ypos);
    }
    // Stop using the arc ball
    else {
    stopArcBallRotation();
    }
}


//--------------------------------------------------------------
void cursorPosCallback(GLFWwindow* apWindow, double x, double y)
//--------------------------------------------------------------
{
    int viewport_width  = 0;
    int viewport_height = 0;

    glfwGetWindowSize(apWindow, &viewport_width, &viewport_height);

    // Compute the rotation
    cursorPositionCallback(x, y, viewport_width, viewport_height);
}

//-----------------------------------------------------------------------
void scrollCallback(GLFWwindow* apWindow, double xoffset, double yoffset)
//-----------------------------------------------------------------------
{
    scrollCallback(xoffset, yoffset);
}
#endif


void pushModelViewMatrix() {
    // Store the current transformation matrix
    gVirtualXRay::pushModelViewMatrix();
}

void popModelViewMatrix() {
    // Store the current transformation matrix
    gVirtualXRay::popModelViewMatrix();
}

//----------------------------------------------------------------
void rotateModelView(double anAngle, double x, double y, double z)
//----------------------------------------------------------------
{
    gVirtualXRay::Matrix4x4 < GLfloat > rotation_matrix;
    rotation_matrix.rotate(anAngle, gVirtualXRay::Vec3 < GLfloat > (x, y, z));

    gVirtualXRay::g_current_modelview_matrix *= rotation_matrix;
}


//------------------------------
void orthographic(double left,
                  double right,
                  double bottom,
                  double top,
                  double nearZ,
                  double farZ)
//------------------------------
{
    gVirtualXRay::loadOrthoProjectionMatrix(
        left, right,
        bottom, top,
        nearZ, farZ);
}

//------------------------------------------
void perspective(double initial_fovy,
                 double screen_aspect_ratio,
                 double initial_near,
                 double initial_far)
//------------------------------------------
{
    gVirtualXRay::loadPerspectiveProjectionMatrix(initial_fovy,
    screen_aspect_ratio, initial_near, initial_far);
}

//-------------------------
void lookAt(double eye_x,
            double eye_y,
            double eye_z,
            double lookAtX,
            double lookAtY,
            double lookAtZ,
            double upX,
            double upY,
            double upZ)
//-------------------------
{
	gVirtualXRay::loadLookAtModelViewMatrix(eye_x, eye_y, eye_z,
			lookAtX, lookAtY, lookAtZ,
			upX, upY, upZ);
}


//----------------------------------------------------------
void computeRotation(gVirtualXRay::MATRIX4& aRotationMatrix,
                     int aViewportWidth,
                     int aViewportHeight)
//----------------------------------------------------------
{
    if (g_use_arc_ball)
    {
        if (g_current_x_position != g_last_x_position ||
            g_current_y_position != g_last_y_position)
        {
            gVirtualXRay::Vec3<GLfloat> va(
                getArcballVector(g_last_x_position,
                                 g_last_y_position,
                                 aViewportWidth,
                                 aViewportHeight));

            gVirtualXRay::Vec3<GLfloat> vb(
                getArcballVector(g_current_x_position,
                                 g_current_y_position,
                                 aViewportWidth,
                                 aViewportHeight));

            float angle(ROTATION_SPEED *
                radian2degree(acos(std::min(1.0, va.dotProduct(vb)))));

            gVirtualXRay::Vec3 < GLfloat > axis_in_camera_coord(va ^ vb);
            //axis_in_camera_coord.normalize();

            gVirtualXRay::Matrix4x4<GLfloat> camera2object(
                aRotationMatrix.getInverse());

            gVirtualXRay::Vec3<GLfloat> axis_in_object_coord =
            camera2object * axis_in_camera_coord;
            //axis_in_object_coord.normalize();

            gVirtualXRay::Matrix4x4 < GLfloat > rotation_matrix;
            rotation_matrix.rotate(angle, axis_in_object_coord);
            aRotationMatrix = aRotationMatrix * rotation_matrix;

            g_last_x_position = g_current_x_position;
            g_last_y_position = g_current_y_position;
        }
    }
}

//---------------------------------------------------------------
gVirtualXRay::Vec3<GLfloat> getArcballVector(int x,
                                             int y,
                                             int aViewportWidth,
                                             int aViewportHeight)
//---------------------------------------------------------------
{
    gVirtualXRay::Vec3 < GLfloat
    > P(2.0 * float(x) / float(aViewportWidth) - 1.0,
    2.0 * float(y) / float(aViewportHeight) - 1.0, 0);

    P.setY(-P.getY());

    float OP_squared = P.getX() * P.getX() + P.getY() * P.getY();
    if (OP_squared <= 1.0) {
    P.setZ(sqrt(1.0 - OP_squared));  // Pythagore
    } else {
    P.normalise();  // nearest point
    }

    return (P);
}


gVirtualXRay::Image<float> getImage(const std::vector<std::vector<float> >& anImage)
{
    gVirtualXRay::Image<float> output(anImage[0].size(), anImage.size());

    for (unsigned int j = 0; j < anImage.size(); ++j)
    {
        for (unsigned int i = 0; i < anImage[0].size(); ++i)
        {
            output.setPixel(i, j, 0, anImage[j][i]);
        }
    }

    return output;
}


gVirtualXRay::Image<float> getImage(const std::vector<std::vector<std::vector<float> > >& anImage)
{
    gVirtualXRay::Image<float> output(anImage[0][0].size(), anImage[0].size(), anImage.size());

    for (unsigned int k = 0; k < anImage.size(); ++k)
    {
        for (unsigned int j = 0; j < anImage[0].size(); ++j)
        {
			for (unsigned int i = 0; i < anImage[0][0].size(); ++i)
			{
				output.setPixel(i, j, k, anImage[k][j][i]);
			}
		}
    }

    return output;
}



gVirtualXRay::MATRIX4 Vector2gvxrMatrix(
    const std::vector<std::vector<float> >& aSTLMatrix) {
    if (aSTLMatrix.size() == 4) {
    if (aSTLMatrix[0].size() == 4 && aSTLMatrix[1].size() == 4
    && aSTLMatrix[2].size() == 4 && aSTLMatrix[3].size() == 4) {
    return gVirtualXRay::MATRIX4(aSTLMatrix[0][0], aSTLMatrix[0][1],
    aSTLMatrix[0][2], aSTLMatrix[0][3], aSTLMatrix[1][0],
    aSTLMatrix[1][1], aSTLMatrix[1][2], aSTLMatrix[1][3],
    aSTLMatrix[2][0], aSTLMatrix[2][1], aSTLMatrix[2][2],
    aSTLMatrix[2][3], aSTLMatrix[3][0], aSTLMatrix[3][1],
    aSTLMatrix[3][2], aSTLMatrix[3][3]);
    }
    }

    throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
    "Wrong matrix size");
}

std::vector<std::vector<float> > gvxrMatrix2Vector(
    const gVirtualXRay::MATRIX4& aMatrix4) {
    std::vector < std::vector<float> > output;
    output.push_back(std::vector<float>(4));
    output.push_back(std::vector<float>(4));
    output.push_back(std::vector<float>(4));
    output.push_back(std::vector<float>(4));

    output[0][0] = aMatrix4.get4x4()[0];
    output[0][1] = aMatrix4.get4x4()[1];
    output[0][2] = aMatrix4.get4x4()[2];
    output[0][3] = aMatrix4.get4x4()[3];
    output[1][0] = aMatrix4.get4x4()[4];
    output[1][1] = aMatrix4.get4x4()[5];
    output[1][2] = aMatrix4.get4x4()[6];
    output[1][3] = aMatrix4.get4x4()[7];
    output[2][0] = aMatrix4.get4x4()[8];
    output[2][1] = aMatrix4.get4x4()[9];
    output[2][2] = aMatrix4.get4x4()[10];
    output[2][3] = aMatrix4.get4x4()[11];
    output[3][0] = aMatrix4.get4x4()[12];
    output[3][1] = aMatrix4.get4x4()[13];
    output[3][2] = aMatrix4.get4x4()[14];
    output[3][3] = aMatrix4.get4x4()[15];

    return output;
}

//------------------------------------------
std::string getElementName(unsigned short Z)
//------------------------------------------
{
    return (gVirtualXRay::ElementSet::getInstance().getElement(Z).getName());
}

//--------------------------------------------
std::string getElementSymbol(unsigned short Z)
//--------------------------------------------
{
    return (gVirtualXRay::ElementSet::getInstance().getElement(Z).getSymbol());
}


//----------------------------------------------------
std::string getElementName(const std::string& aSymbol)
//----------------------------------------------------
{
  return (gVirtualXRay::ElementSet::getInstance().getElement(aSymbol).getName());
}


//-----------------------------------------------------------------
unsigned short getElementAtomicNumber(const std::string& anElement)
//-----------------------------------------------------------------
{
    return (gVirtualXRay::ElementSet::getInstance().getElement(anElement).
        getZ());
}

//-----------------------------------------------------
float getElementMolarMass(const std::string& anElement)
//-----------------------------------------------------
{
    return (gVirtualXRay::ElementSet::getInstance().getElement(anElement).
        getMolarMass());
}


double getDensityFromElement(const std::string& anElement) {
    unsigned short Z = getElementAtomicNumber(anElement);
    return (getDensityFromElement(Z));
}

double getDensityFromElement(unsigned short Z) {
    return (gVirtualXRay::ElementSet::getInstance().getElement(Z).getDensity()
    / (gVirtualXRay::g / gVirtualXRay::cm3));
}


//-------------------------------
double getDensityFromHU(double HU)
//-------------------------------
{
    return (gVirtualXRay::XRayRenderer::getDensityFromHU(HU)
    / (gVirtualXRay::g / gVirtualXRay::cm3));
}


//------------------------------------------------------------
double getMassAttenuationFromElement(const std::string& anElement,
		double anEnergy,
				const std::string& aUnitOfEnergy)
//------------------------------------------------------------
{
    unsigned short Z = getElementAtomicNumber(anElement);
    return getMassAttenuationFromElement(Z, anEnergy, aUnitOfEnergy);
}


//------------------------------------------------
double getMassAttenuationFromElement(unsigned short Z,
		double anEnergy,
				const std::string& aUnitOfEnergy)
//------------------------------------------------
{
    return (gVirtualXRay::ElementSet::getInstance().getElement(Z).getMassAttenuationTotal(anEnergy * getUnitOfEnergy(aUnitOfEnergy))
    / (gVirtualXRay::cm2 / gVirtualXRay::g));
}


//-------------------------------------------
double getMassAttenuationFromHU(double HU,
		double anEnergy,
				const std::string& aUnitOfEnergy)
//-------------------------------------------
{
    // Initialise the photon cross-section
    gVirtualXRay::PhotonCrossSection photon_cross_section;
    photon_cross_section.useHU(HU);

	return (photon_cross_section.getMassAttenuationCoefficient(
			anEnergy * getUnitOfEnergy(aUnitOfEnergy)) /
				(gVirtualXRay::cm2 / gVirtualXRay::g));
}


//-----------------------------------------------------------
double getMassAttenuationFromMixture(const std::string& aMixture,
		double anEnergy,
				const std::string& aUnitOfEnergy)
//-----------------------------------------------------------
{
    // Initialise the photon cross-section
    gVirtualXRay::PhotonCrossSection photon_cross_section;
    photon_cross_section.useMixture(aMixture);

	return (photon_cross_section.getMassAttenuationCoefficient(
			anEnergy * getUnitOfEnergy(aUnitOfEnergy)) /
				(gVirtualXRay::cm2 / gVirtualXRay::g));
}


//-------------------------------------------------------------
double getMassAttenuationFromCompound(const std::string& aCompound,
		double anEnergy,
				const std::string& aUnitOfEnergy)
//-------------------------------------------------------------
{
    // Initialise the photon cross-section
    gVirtualXRay::PhotonCrossSection photon_cross_section;
    photon_cross_section.useCompound(aCompound);

	return (photon_cross_section.getMassAttenuationCoefficient(
			anEnergy * getUnitOfEnergy(aUnitOfEnergy)) /
				(gVirtualXRay::cm2 / gVirtualXRay::g));
}


double getMuFromHU(double HU,
		double anEnergy,
		const std::string& aUnitOfEnergy)
{
	return (gVirtualXRay::XRayRenderer::getMuFromHU(HU, anEnergy * getUnitOfEnergy(aUnitOfEnergy)) / (1.0 / gVirtualXRay::cm));
}



std::vector<std::vector<std::vector< float> > > convertHU2mu(const std::vector<std::vector<std::vector< float> > >& anImage,
    double anEnergy,
    const std::string& aUnitOfEnergy)
{
    // Number of slices
    int image_depth = anImage.size();

    // Create an empty image
    std::vector<std::vector<std::vector< float> > > output_image;

    if (image_depth > 0)
    {
        int image_height = anImage[0].size();

        if (image_height > 0)
        {
            int image_width = anImage[0][0].size();

            if (image_width > 0)
            {
                // Allocate the data
                output_image.resize(image_depth);

                for (int z = 0; z < image_depth; ++z)
                {
                    // Allocate the rows
                    output_image[z].resize(image_height);

                    for (int y = 0; y < image_height; ++y)
                    {
                        // Allocate the columns
                        output_image[z][y].resize(image_width);
                    }
                }

#ifdef NDEBUG
#ifdef WIN32
#pragma omp parallel for
#else
#pragma omp parallel for collapse(3)
#endif
#endif
                for (int z = 0; z < image_depth; ++z)
                {
                    for (int y = 0; y < image_height; ++y)
                    {
                        for (int x = 0; x < image_width; ++x)
                        {
                            output_image[z][y][x] = getMuFromHU(anImage[z][y][x],
                                                                anEnergy,
                                                                aUnitOfEnergy);
                        }
                    }
                }
            }
        }
    }

    return output_image;
}



std::vector<float> rayIntersect(const std::string& aLabel,
    double aRayOriginX, double aRayOriginY, double aRayOriginZ,
    double aRayDirectionX, double aRayDirectionY, double aRayDirectionZ)
{
    std::vector<float> p_intersection_set;
    gVirtualXRay::SceneGraphNode* p_source_node = 0;

    // The scenegraph does not exist
    if (g_p_scene_graph.get())
    {
        // Find the node. It must exist
        gVirtualXRay::SceneGraphNode* p_source_node =
            g_p_scene_graph->getNode(aLabel);

        // The node exists
        if (p_source_node)
        {
            // Ray tracing
            std::map<float, gVirtualXRay::VEC3> p_intersection_full_set = p_source_node->rayIntersect(
            		gVirtualXRay::VEC3(aRayOriginX, aRayOriginY, aRayOriginZ),
			        gVirtualXRay::VEC3(aRayDirectionX, aRayDirectionY, aRayDirectionZ),
			        gVirtualXRay::MATRIX4());

			// Iterate through the map
			for (std::map<float, gVirtualXRay::VEC3>::const_iterator ite = p_intersection_full_set.begin();
			         ite != p_intersection_full_set.end();
			         ++ite)
			{
    			p_intersection_set.push_back(ite->first);
			}
        }
    }

    // The source does not exist
    if (!p_source_node)
    {
        gVirtualXRay::LOGGER.logWarning("PolygonMesh ") <<
            aLabel <<
            " not found in g_p_polygon_mesh_set." <<
            std::endl;
    }

    return p_intersection_set;
}















void setShiftFilter(double aValue) {
    g_shift_filter = aValue;
}

void setScaleFilter(double aValue) {
    g_scale_filter = aValue;
}

double getShiftFilter() {
    return (g_shift_filter);
}

double getScaleFilter() {
    return (g_scale_filter);
}


std::vector<std::vector<float> > loadImage2D(const std::string& aFileName)
{
    return getVectorImage2D<float>(gVirtualXRay::Image<float>(aFileName));
}


std::vector<std::vector<std::vector<float> > > loadImage3D(const std::string& aFileName)
{
    return getVectorImage3D<float>(gVirtualXRay::Image<float>(aFileName));
}


double computeZNCC(const std::vector<std::vector<std::vector<float> > >& aReferenceImage, const std::vector<std::vector<std::vector<float> > >& aTestImage)
{
    gVirtualXRay::Image<float> ref = getImage(aReferenceImage);
    gVirtualXRay::Image<float> test = getImage(aTestImage);

    return ref.computeNCC(test);
}


double computeRMSE(const std::vector<std::vector<std::vector<float> > >& aReferenceImage, const std::vector<std::vector<std::vector<float> > >& aTestImage)
{
    gVirtualXRay::Image<float> ref = getImage(aReferenceImage);
    gVirtualXRay::Image<float> test = getImage(aTestImage);

    return ref.computeRMSE(test);
}


double getMean(const std::vector<float>& anImage)
{
    double avg = 0.0;

#pragma omp parallel for reduction( + : avg )
#ifdef _WIN32
    for (int i = 0; i < anImage.size(); ++i)
#else
    for (size_t i = 0; i < anImage.size(); ++i)
#endif
    {
        avg += anImage[i];
    }

    return avg / anImage.size();
}

double getStddev(const std::vector<float>& anImage)
{
    double avg = getMean(anImage);
    double stddev = 0.0;

#pragma omp parallel for reduction( + : stddev )
#ifdef _WIN32
    for (int i = 0; i < anImage.size(); ++i)
#else
    for (size_t i = 0; i < anImage.size(); ++i)
#endif
    {
        stddev += std::pow(anImage[i] - avg, 2);
    }

    return sqrt(stddev / anImage.size());
}


double getStddev(const std::vector<float>& anImage, double avg)
{
    double stddev = 0.0;

#pragma omp parallel for reduction( + : stddev )
#ifdef _WIN32
    for (int i = 0; i < anImage.size(); ++i)
#else
    for (size_t i = 0; i < anImage.size(); ++i)
#endif
    {
        stddev += std::pow(anImage[i] - avg, 2);
    }

    return sqrt(stddev / anImage.size());
}

double computeZNCC(const std::vector<float>& aReferenceImage, const std::vector<float>& aTestImage)
{
    double avg_ref = getMean(aReferenceImage);
    double avg_test = getMean(aTestImage);

    double stddev_ref = getStddev(aReferenceImage, avg_ref);
    double stddev_test = getStddev(aTestImage, avg_test);

    double zncc = 0.0;
#pragma omp parallel for reduction( + : zncc )
#ifdef _WIN32
    for (int i = 0; i < aReferenceImage.size(); ++i)
#else
    for (size_t i = 0; i < aReferenceImage.size(); ++i)
#endif
    {
        zncc += (aReferenceImage[i] - avg_ref) * (aTestImage[i] - avg_test);
    }

    return (zncc / (stddev_ref * stddev_test)) / aReferenceImage.size();
}


double computeRMSE(const std::vector<float>& aReferenceImage, const std::vector<float>& aTestImage, bool normalise)
{
    double metrics = 0.0;

    double avg_ref = getMean(aReferenceImage);

#pragma omp parallel for reduction( + : metrics )
#ifdef _WIN32
    for (int i = 0; i < aReferenceImage.size(); ++i)
#else
    for (size_t i = 0; i < aReferenceImage.size(); ++i)
#endif
    {
        metrics += std::pow(aReferenceImage[i] - aTestImage[i], 2);
    }

    metrics = sqrt(metrics);

    if (normalise) metrics /= avg_ref;

    return metrics;
}


std::vector<float> g_p_ref_image;
std::vector<float> g_p_test_image;

double computeRMSE(bool normalise)
{
    return computeRMSE(g_p_ref_image, g_p_test_image, normalise);
}


double computeZNCC()
{
    return computeZNCC(g_p_ref_image, g_p_test_image);
}


void loadReference(const std::vector<float>& aReferenceImage)
{
    g_p_ref_image = aReferenceImage;
}


void loadTest(const std::vector<float>& aTestImage)
{
    g_p_test_image = aTestImage;
}


void lineariseTest(float aThreshold, float aScalingFactor)
{
#pragma omp parallel for
#ifdef _WIN32
    for (int i = 0; i < g_p_test_image.size(); ++i)
#else
    for (size_t i = 0; i < g_p_test_image.size(); ++i)
#endif
    {
        if (g_p_test_image[i] < aThreshold)
        {
            g_p_test_image[i] = aThreshold;
        }

        g_p_test_image[i] *= aScalingFactor;
        g_p_test_image[i] = -log(g_p_test_image[i]);
    }
}


float getMinValue(const std::vector<std::vector<std::vector<float> > >& aImage)
{
    return getImage(aImage).getMinValue();
}


float getMaxValue(const std::vector<std::vector<std::vector<float> > >& aImage)
{
    return getImage(aImage).getMaxValue();
}

/*
void FBO2Thrust(unsigned int aFBO, void* aThrustVector, int anOffset)
{
#ifndef HAS_CUDA
    throw gVirtualXRay::Exception(__FILE__,
                                  __FUNCTION__,
                                  __LINE__,
                                  "CUDA is not supported.");
#else
    cudaGLSetGLDevice(0);
    struct cudaGraphicsResource *fbo_cuda;
    cudaGraphicsGLRegisterBuffer(&fbo_cuda, g_xray_renderer.getFboId(aFBO), cudaGraphicsMapFlagsReadOnly);


    thrust::device_ptr<float4>* p_device_data = static_cast<thrust::device_ptr<float4>*>(aThrustVector);
#endif
}*/
