#include <map>

#ifndef GVXR_IMPLICIT_SURFACE_H
#include "gVirtualXRay/ImplicitSurface.h"
#endif

#ifndef GVXR_LOGGER_H
#include "gVirtualXRay/Logger.h"
#endif

// To load a scenegraph
#ifndef GVXR_SCENEGRAPH_BINDER_H
#include "gVirtualXRay/SceneGraphBinder.h"
#endif

#ifndef SGVXR_internal_h
#include "internal.h"
#endif

#ifndef SGVXR_Utils_h
#include "Utils.h"
#endif

#ifndef SGVXR_IMPLICIT_SURFACE_H
#include "ImplicitSurface.h"
#endif


extern std::unique_ptr<gVirtualXRay::SceneGraphBinder> g_p_scene_graph;
std::map<std::string, gVirtualXRay::ImplicitSurface> g_implicit_surface_set;


//----------------------------------------
unsigned int getNumberOfImplicitSurfaces()
//----------------------------------------
{
    return g_implicit_surface_set.size();
}


//---------------------------------------------------
std::vector<std::string> getImplicitSurfaceLabelSet()
//---------------------------------------------------
{
    std::vector<std::string> return_value;

    for (const auto& [key, value] : g_implicit_surface_set)
    {
        return_value.push_back(key);
    }

    return return_value;
}


//---------------------------------------------------
bool implicitSurfaceExists(const std::string& aLabel)
//---------------------------------------------------
{
    return (g_implicit_surface_set.find(aLabel) != g_implicit_surface_set.end());
}

//---------------------------------------------------
void createImplicitSurface(const std::string& aLabel)
//---------------------------------------------------
{
    // The implicit surface label does not exist
    if (!implicitSurfaceExists(aLabel))
    {
        g_implicit_surface_set[aLabel] = gVirtualXRay::ImplicitSurface();
    }
    // Print a message and do nothing
    else
    {
        gVirtualXRay::LOGGER.logWarning("ImplicitSurface ") <<
            aLabel <<
            " already exists in g_implicit_surface_set." <<
            std::endl;
    }
}


//----------------------------------------------------
void destroyImplicitSurface(const std::string& aLabel)
//----------------------------------------------------
{
    // The implicit surface label does not exist
    // Print a message and do nothing
    if (!implicitSurfaceExists(aLabel))
    {
        gVirtualXRay::LOGGER.logWarning("ImplicitSurface ") <<
            aLabel <<
            " not found in g_implicit_surface_set." <<
            std::endl;
    }
    // Erase the implicit surface
    else
    {
        g_implicit_surface_set.erase(aLabel);
    }
}


//-------------------------------
void destroyAllImplicitSurfaces()
//-------------------------------
{
    g_implicit_surface_set.clear();
}


//-------------------------------------------------------
void addControlPoint(const std::string& aLabel,
                     float aPositionX,
                     float aPositionY,
                     float aPositionZ,
                     const std::string& aUnitOfLength,
                     const std::string& aDentityFunction,
                     float aParameter1,
                     float aParameter2)
//-------------------------------------------------------
{
    // The implicit surface label does not exist
    if (!implicitSurfaceExists(aLabel))
    {
        // Print a message
        gVirtualXRay::LOGGER.logWarning("ImplicitSurface ") <<
            aLabel <<
            " not found in g_implicit_surface_set. It will be added." <<
            std::endl;

        // Create a new implicit surface
        createImplicitSurface(aLabel);
    }

    // Add the control point
    auto unit_of_length = static_cast<float>(getUnitOfLength(aUnitOfLength));

    // Make sure the density function name is in capital letters
    std::string density_function_type = aDentityFunction;
    std::transform(density_function_type.begin(), density_function_type.end(), density_function_type.begin(), ::toupper);
    
    // Create the 3D point in the right unit
    gVirtualXRay::VEC3 position(aPositionX, aPositionY, aPositionZ);
    position *= unit_of_length;

    if (density_function_type == "SPHERE")
    {
        g_implicit_surface_set[aLabel].addControlPoint(
            gVirtualXRay::ImplicitSurface::DensityFunctionParameterType(
                gVirtualXRay::ImplicitSurface::SPHERE,
                aParameter1),
            position);
    }
    else if (density_function_type == "BLOBBY_MOLECULE" || density_function_type == "BLOBBY MOLECULE")
    {
        g_implicit_surface_set[aLabel].addControlPoint(
            gVirtualXRay::ImplicitSurface::DensityFunctionParameterType(
                gVirtualXRay::ImplicitSurface::BLOBBY_MOLECULE,
                aParameter1, aParameter2),
            position);
    }
    else if (density_function_type == "META_BALL" || density_function_type == "META BALL" || density_function_type == "METABALL")
    {
        g_implicit_surface_set[aLabel].addControlPoint(
            gVirtualXRay::ImplicitSurface::DensityFunctionParameterType(
                gVirtualXRay::ImplicitSurface::META_BALL,
                aParameter1, aParameter2),
            position);
    }
    else if (density_function_type == "SOFT_OBJECT" || density_function_type == "SOFT OBJECT")
    {
        g_implicit_surface_set[aLabel].addControlPoint(
            gVirtualXRay::ImplicitSurface::DensityFunctionParameterType(
                gVirtualXRay::ImplicitSurface::SOFT_OBJECT,
                aParameter1, aParameter2),
            position);
    }
    else
    {
        gVirtualXRay::LOGGER.logWarning("Unknown type of density function ") << aDentityFunction << 
            R"(Make sure the density function type of is "SPHERE", "BLOBBY_MOLECULE", "META_BALL" or "SOFT_OBJECT".)" << std::endl;
    }
}


//-------------------------------------------------------------
void addControlLineSegment(const std::string& aLabel,
                           float aPosition1X,
                           float aPosition1Y,
                           float aPosition1Z,
                           float aPosition2X,
                           float aPosition2Y,
                           float aPosition2Z,
                           const std::string& aUnitOfLength,
                           const std::string& aDentityFunction,
                           float aParameter1,
                           float aParameter2)
//-------------------------------------------------------------
{
    // The implicit surface label does not exist
    if (!implicitSurfaceExists(aLabel))
    {
        // Print a message
        gVirtualXRay::LOGGER.logWarning("ImplicitSurface ") <<
            aLabel <<
            " not found in g_implicit_surface_set. It will be added." <<
            std::endl;

        // Create a new implicit surface
        createImplicitSurface(aLabel);
    }

    // Add the control line segment
    auto unit_of_length = static_cast<float>(getUnitOfLength(aUnitOfLength));

    // Make sure the density function name is in capital letters
    std::string density_function_type = aDentityFunction;
    std::transform(density_function_type.begin(), density_function_type.end(), density_function_type.begin(), ::toupper);
    
    // Create the 3D points in the right unit
    gVirtualXRay::VEC3 position1(aPosition1X, aPosition1Y, aPosition1Z);
    position1 *= unit_of_length;

    gVirtualXRay::VEC3 position2(aPosition2X, aPosition2Y, aPosition2Z);
    position2 *= unit_of_length;

    if (density_function_type == "SPHERE")
    {
        g_implicit_surface_set[aLabel].addControlLine(
            gVirtualXRay::ImplicitSurface::DensityFunctionParameterType(
                gVirtualXRay::ImplicitSurface::SPHERE,
                aParameter1),
            position1, position2);
    }
    else if (density_function_type == "BLOBBY_MOLECULE" || density_function_type == "BLOBBY MOLECULE")
    {
        g_implicit_surface_set[aLabel].addControlLine(
            gVirtualXRay::ImplicitSurface::DensityFunctionParameterType(
                gVirtualXRay::ImplicitSurface::BLOBBY_MOLECULE,
                aParameter1, aParameter2),
            position1, position2);
    }
    else if (density_function_type == "META_BALL" || density_function_type == "META BALL" || density_function_type == "METABALL")
    {
        g_implicit_surface_set[aLabel].addControlLine(
            gVirtualXRay::ImplicitSurface::DensityFunctionParameterType(
                gVirtualXRay::ImplicitSurface::META_BALL,
                aParameter1, aParameter2),
            position1, position2);
    }
    else if (density_function_type == "SOFT_OBJECT" || density_function_type == "SOFT OBJECT")
    {
        g_implicit_surface_set[aLabel].addControlLine(
            gVirtualXRay::ImplicitSurface::DensityFunctionParameterType(
                gVirtualXRay::ImplicitSurface::SOFT_OBJECT,
                aParameter1, aParameter2),
            position1, position2);
    }
    else
    {
        throw gVirtualXRay::Exception(__FILE__,
                        __FUNCTION__,
                        __LINE__,
                        R"(Make sure the density function type of is "SPHERE", "BLOBBY_MOLECULE", "META_BALL" or "SOFT_OBJECT".)");
    }    
}


//--------------------------------------------------------------
unsigned int getNumberOfControlPoints(const std::string& aLabel)
//--------------------------------------------------------------
{
    unsigned int return_value = 0;

    // The implicit surface label does not exist
    if (!implicitSurfaceExists(aLabel))
    {
        // Print a message
        gVirtualXRay::LOGGER.logWarning("ImplicitSurface ") <<
            aLabel <<
            " not found in g_implicit_surface_set. It will be added." <<
            std::endl;
    }
    // The implicit surface label exists
    else
    {
        return_value = g_implicit_surface_set[aLabel].getNumberOfControlPoints();
    }

    return return_value;
}


//--------------------------------------------------------------------
unsigned int getNumberOfControlLineSegments(const std::string& aLabel)
//--------------------------------------------------------------------
{
    unsigned int return_value = 0;

    // The implicit surface label does not exist
    if (!implicitSurfaceExists(aLabel))
    {
        // Print a message
        gVirtualXRay::LOGGER.logWarning("ImplicitSurface ") <<
            aLabel <<
            " not found in g_implicit_surface_set. It will be added." <<
            std::endl;
    }
    // The implicit surface label exists
    else
    {
        return_value = g_implicit_surface_set[aLabel].getNumberOfControlLineSegments();
    }

    return return_value;
}


//-----------------------------------------------------------------
void setControlPointParameters(const std::string& aLabel,
                               unsigned int aPointID,
                               const std::string& aDentityFunction,
                               float aParameter1,
                               float aParameter2)
//-----------------------------------------------------------------
{
    unsigned int number_of_control_points = getNumberOfControlPoints(aLabel);

    // The control point does not exist
    // Print a message and do nothing
    if (aPointID >= number_of_control_points)
    {
        gVirtualXRay::LOGGER.logWarning("ImplicitSurface ") <<
            aLabel << " has " << number_of_control_points << " control points. Point #" << aPointID << " cannot does not exist." <<
            std::endl;
    }
    // The control point exists
    else
    {
        // Make sure the density function name is in capital letters
        std::string density_function_type = aDentityFunction;
        std::transform(density_function_type.begin(), density_function_type.end(), density_function_type.begin(), ::toupper);

        if (density_function_type == "SPHERE")
        {
            g_implicit_surface_set[aLabel].setControlPointParameters(aPointID,
                gVirtualXRay::ImplicitSurface::SPHERE, aParameter1, aParameter2);
        }
        else if (density_function_type == "BLOBBY_MOLECULE" || density_function_type == "BLOBBY MOLECULE")
        {
            g_implicit_surface_set[aLabel].setControlPointParameters(aPointID,
                gVirtualXRay::ImplicitSurface::BLOBBY_MOLECULE, aParameter1, aParameter2);
        }
        else if (density_function_type == "META_BALL" || density_function_type == "META BALL" || density_function_type == "METABALL")
        {
            g_implicit_surface_set[aLabel].setControlPointParameters(aPointID,
                gVirtualXRay::ImplicitSurface::META_BALL, aParameter1, aParameter2);
        }
        else if (density_function_type == "SOFT_OBJECT" || density_function_type == "SOFT OBJECT")
        {
            g_implicit_surface_set[aLabel].setControlPointParameters(aPointID,
                gVirtualXRay::ImplicitSurface::SOFT_OBJECT, aParameter1, aParameter2);
        }
        else
        {
            gVirtualXRay::LOGGER.logWarning("Unknown type of density function ") << aDentityFunction << 
                R"(Make sure the density function type of is "SPHERE", "BLOBBY_MOLECULE", "META_BALL" or "SOFT_OBJECT".)" << std::endl;
        }
    }
}


//-----------------------------------------------------------------------
void setControlLineSegmentParameters(const std::string& aLabel,
                                     unsigned int aLineSegmentID,
                                     const std::string& aDentityFunction,
                                     float aParameter1,
                                     float aParameter2)
//-----------------------------------------------------------------------
{
    unsigned int number_of_control_line_segments = getNumberOfControlLineSegments(aLabel);

    // The control line segment does not exist
    // Print a message and do nothing
    if (aLineSegmentID >= number_of_control_line_segments)
    {
        gVirtualXRay::LOGGER.logWarning("ImplicitSurface ") <<
            aLabel << " has " << number_of_control_line_segments << " control line segments. Line segment #" << aLineSegmentID << " cannot does not exist." <<
            std::endl;
    }
    // The line segment exists
    else
    {
        // Make sure the density function name is in capital letters
        std::string density_function_type = aDentityFunction;
        std::transform(density_function_type.begin(), density_function_type.end(), density_function_type.begin(), ::toupper);

        if (density_function_type == "SPHERE")
        {
            g_implicit_surface_set[aLabel].setControlLineSegmentParameters(aLineSegmentID,
                gVirtualXRay::ImplicitSurface::SPHERE, aParameter1, aParameter2);
        }
        else if (density_function_type == "BLOBBY_MOLECULE" || density_function_type == "BLOBBY MOLECULE")
        {
            g_implicit_surface_set[aLabel].setControlLineSegmentParameters(aLineSegmentID,
                gVirtualXRay::ImplicitSurface::BLOBBY_MOLECULE, aParameter1, aParameter2);
        }
        else if (density_function_type == "META_BALL" || density_function_type == "META BALL" || density_function_type == "METABALL")
        {
            g_implicit_surface_set[aLabel].setControlLineSegmentParameters(aLineSegmentID,
                gVirtualXRay::ImplicitSurface::META_BALL, aParameter1, aParameter2);
        }
        else if (density_function_type == "SOFT_OBJECT" || density_function_type == "SOFT OBJECT")
        {
            g_implicit_surface_set[aLabel].setControlLineSegmentParameters(aLineSegmentID,
                gVirtualXRay::ImplicitSurface::SOFT_OBJECT, aParameter1, aParameter2);
        }
        else
        {
            gVirtualXRay::LOGGER.logWarning("Unknown type of density function ") << aDentityFunction << 
                R"(Make sure the density function type of is "SPHERE", "BLOBBY_MOLECULE", "META_BALL" or "SOFT_OBJECT".)" << std::endl;
        }
    }
}


//------------------------------------------------------------
void voxeliseImplicitSurface(const std::string& aLabel,
                             unsigned int aNumberOfVoxelX,
                             unsigned int aNumberOfVoxelY,
                             unsigned int aNumberOfVoxelZ,
                             float aCentrePositionX,
                             float aCentrePositionY,
                             float aCentrePositionZ,
                             float aVoxelWidth,
                             float aVoxelHeight,
                             float aVoxelDepth,
                             const std::string& aUnitOfLength,
                             unsigned char aVerboseLevel)
//------------------------------------------------------------
{
    // The implicit surface label does not exist
    // Print a message and do nothing
    if (!implicitSurfaceExists(aLabel))
    {
        gVirtualXRay::LOGGER.logWarning("ImplicitSurface ") <<
            aLabel <<
            " not found in g_implicit_surface_set." <<
            std::endl;
    }
    // Voxelise the implicit surface
    else
    {
        auto unit_of_length = static_cast<float>(getUnitOfLength(aUnitOfLength));

        g_implicit_surface_set[aLabel].voxelise(gVirtualXRay::Vec3ui(aNumberOfVoxelX, aNumberOfVoxelY, aNumberOfVoxelZ),
            gVirtualXRay::VEC3(aCentrePositionX, aCentrePositionY, aCentrePositionZ) * unit_of_length,
            gVirtualXRay::VEC3(aVoxelWidth, aVoxelHeight, aVoxelDepth) * unit_of_length,
            aVerboseLevel);
    }
}


//------------------------------------------------------------
void voxelizeImplicitSurface(const std::string& aLabel,
                             unsigned int aNumberOfVoxelX,
                             unsigned int aNumberOfVoxelY,
                             unsigned int aNumberOfVoxelZ,
                             float aCentrePositionX,
                             float aCentrePositionY,
                             float aCentrePositionZ,
                             float aVoxelWidth,
                             float aVoxelHeight,
                             float aVoxelDepth,
                             const std::string& aUnitOfLength,
                             unsigned char aVerboseLevel)
//------------------------------------------------------------
{
    voxeliseImplicitSurface(aLabel,
        aNumberOfVoxelX, aNumberOfVoxelY, aNumberOfVoxelZ,
        aCentrePositionX, aCentrePositionY, aCentrePositionZ,
        aVoxelWidth, aVoxelHeight, aVoxelDepth,
        aUnitOfLength,
        aVerboseLevel);
}


//--------------------------------------------------------------------------------------------------------
std::vector<std::vector<std::vector<float>>> getImplicitSurfaceLastVoxelisation(const std::string& aLabel)
//--------------------------------------------------------------------------------------------------------
{
    std::vector<std::vector<std::vector<float>>> volume;

    // The implicit surface label does not exist
    // Print a message and return an empty volume
    if (!implicitSurfaceExists(aLabel))
    {
        gVirtualXRay::LOGGER.logWarning("ImplicitSurface ") <<
            aLabel <<
            " not found in g_implicit_surface_set." <<
            std::endl;
    }
    // Get the last Voxelised implicit surface
    else
    {
        volume = getVectorImage3D(g_implicit_surface_set[aLabel].getVolume());
    }

    return volume;
}


//--------------------------------------------------------------------------------------------------------
std::vector<std::vector<std::vector<float>>> getImplicitSurfaceLastVoxelization(const std::string& aLabel)
//--------------------------------------------------------------------------------------------------------
{
    return getImplicitSurfaceLastVoxelisation(aLabel);
}


//--------------------------------------------------------------------------
void createMeshFromImplicitSurface(const std::string& aImplicitSurfaceLabel,
                                   const std::string& aMeshLabel,
                                   float anIsoValue,
                                   const std::string& aParent,
                                   unsigned char aVerboseLevel)
//--------------------------------------------------------------------------
{
    // The implicit surface label does not exist
    // Print a message and do nothing
    if (!implicitSurfaceExists(aImplicitSurfaceLabel))
    {
        gVirtualXRay::LOGGER.logWarning("ImplicitSurface ") <<
            aImplicitSurfaceLabel <<
            " not found in g_implicit_surface_set." <<
            std::endl;
    }
    // Apply the marching cubes
    else
    {
        // The voxelisation does not exist
        // Print a message and do nothing
        if (g_implicit_surface_set[aImplicitSurfaceLabel].getVolume().getWidth() == 0)
        {
            gVirtualXRay::LOGGER.logWarning("ImplicitSurface ") <<
                aImplicitSurfaceLabel <<
                " exists in g_implicit_surface_set, but you must voxelise it before creating an iso-surface." <<
                std::endl;
        }
        // The voxelisation exists
        else
        {
            // Create the mesh
            g_implicit_surface_set[aImplicitSurfaceLabel].extractIsoSurface(anIsoValue, aVerboseLevel);

            // The scenegraph does not exist
            if (!g_p_scene_graph.get())
                {
                // Create a scenegraph
                g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
                }

            gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aMeshLabel);

            // The node is already in the scene graph and replace the mesh
            if (p_node)
            {
                p_node->setGeometry(g_implicit_surface_set[aMeshLabel].getPolygonMesh());
            }
            // Create the node in the scene graph and add the mesh
            else
            {
                g_p_scene_graph->addPolygonMesh(
                    aMeshLabel,
                    g_implicit_surface_set[aImplicitSurfaceLabel].getPolygonMesh(),
                    gVirtualXRay::VEC3(),
                    aParent);
            }
        }
    }
}


//--------------------------------------------------------------------------
void createMeshFromImplicitSurface(const std::string& aImplicitSurfaceLabel,
                                   const std::string& aMeshLabel,
                                   float aLowerIsoValue,
                                   float anUpperIsoValue,
                                   const std::string& aParent,
                                   unsigned char aVerboseLevel)
//--------------------------------------------------------------------------
{
    // The implicit surface label does not exist
    // Print a message and do nothing
    if (!implicitSurfaceExists(aImplicitSurfaceLabel))
    {
        gVirtualXRay::LOGGER.logWarning("ImplicitSurface ") <<
            aImplicitSurfaceLabel <<
            " not found in g_implicit_surface_set." <<
            std::endl;
    }
    // Apply the marching cubes
    else
    {
        // The voxelisation does not exist
        // Print a message and do nothing
        if (g_implicit_surface_set[aImplicitSurfaceLabel].getVolume().getWidth() == 0)
        {
            gVirtualXRay::LOGGER.logWarning("ImplicitSurface ") <<
                aImplicitSurfaceLabel <<
                " exists in g_implicit_surface_set, but you must voxelise it before creating an iso-surface." <<
                std::endl;
        }
        // The voxelisation exists
        else
        {
            // Create the mesh
            g_implicit_surface_set[aImplicitSurfaceLabel].extractIsoSurface(aLowerIsoValue, anUpperIsoValue, aVerboseLevel);

            // The scenegraph does not exist
            if (!g_p_scene_graph.get())
                {
                // Create a scenegraph
                g_p_scene_graph.reset(new gVirtualXRay::SceneGraphBinder());
                }

            gVirtualXRay::SceneGraphNode* p_node = g_p_scene_graph->getNode(aMeshLabel);

            // The node is already in the scene graph and replace the mesh
            if (p_node)
            {
                p_node->setGeometry(g_implicit_surface_set[aImplicitSurfaceLabel].getPolygonMesh());
            }
            // Create the node in the scene graph and add the mesh
            else
            {
                g_p_scene_graph->addPolygonMesh(
                    aMeshLabel,
                    g_implicit_surface_set[aImplicitSurfaceLabel].getPolygonMesh(),
                    gVirtualXRay::VEC3(),
                    aParent);
            }
        }
    }
}
