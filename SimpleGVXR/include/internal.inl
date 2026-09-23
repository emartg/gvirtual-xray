//******************************************************************************
//  Include
//******************************************************************************
#ifndef GVXR_SCENEGRAPH_BINDER_H
#include "gVirtualXRay/SceneGraphBinder.h"
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
extern std::unique_ptr<gVirtualXRay::SceneGraphBinder> g_p_scene_graph;


//******************************************************************************
//  Function definitions
//******************************************************************************


template<typename T> std::vector<std::vector<std::vector<T> > > getVectorImage3D(
    const gVirtualXRay::Image<T>& anImage)
{
	std::vector < std::vector < std::vector<T> > > output;

    for (unsigned int k = 0; k < anImage.getDepth(); ++k)
    {
    	std::vector < std::vector<T> > slice;
        for (unsigned int j = 0; j < anImage.getHeight(); ++j)
		{
			std::vector<T> row;
	        for (unsigned int i = 0; i < anImage.getWidth(); ++i)
			{
	        	row.push_back(anImage.getPixel(i, j, k));
			}

			slice.push_back(row);
		}

		output.push_back(slice);
    }

    return output;
}


template<typename T> std::vector<std::vector<T> > getVectorImage2D(
    const gVirtualXRay::Image<T>& anImage) {
    std::vector < std::vector<T> > output;

    for (unsigned int j = 0; j < anImage.getHeight(); ++j) {
    std::vector<T> row;
    row.insert(row.end(), anImage.getRawData() + j * anImage.getWidth(),
    anImage.getRawData() + (j + 1) * anImage.getWidth());

    output.push_back(row);
    }

    return output;
}


template<typename T> gVirtualXRay::Image<T> getImageVector3D(const std::vector<std::vector<std::vector<T> > > apVolume)
{
    unsigned int number_of_slices = apVolume.size();

    if (number_of_slices == 0)
    {
        gVirtualXRay::OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    unsigned int number_of_rows = apVolume[0].size();

    if (number_of_rows == 0)
    {
        gVirtualXRay::OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    unsigned int number_of_cols = apVolume[0][0].size();

    if (number_of_cols == 0)
    {
        gVirtualXRay::OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }


    gVirtualXRay::Image<T> temp(number_of_cols, number_of_rows, number_of_slices);

    for (unsigned int k = 0; k < number_of_slices; ++k)
    {
        if (apVolume[k].size() != number_of_rows)
        {
            throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
                "Slices may have different number of rows"
            );
        }

        for (unsigned int j = 0; j < number_of_rows; ++j)
		{
            if (apVolume[k][j].size() != number_of_cols)
            {
                throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
                    "Slices may have different number of cols"
                );
            }

	        for (unsigned int i = 0; i < number_of_cols; ++i)
			{
                temp.setPixel(i, j, k, apVolume[k][j][i]);
			}
		}
    }

    return temp;
}

template<typename T> gVirtualXRay::Image<T> getImageVector2D(const std::vector<std::vector<T> > apImage)
{
    unsigned int number_of_rows = apImage.size();

    if (number_of_rows == 0)
    {
        gVirtualXRay::OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    unsigned int number_of_cols = apImage[0].size();

    if (number_of_cols == 0)
    {
        gVirtualXRay::OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }


    gVirtualXRay::Image<T> temp(number_of_cols, number_of_rows, 1);

    for (unsigned int j = 0; j < number_of_rows; ++j)
    {
        if (apImage[j].size() != number_of_cols)
        {
            throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__,
                "Rows may have different number of cols"
            );
        }

        for (unsigned int i = 0; i < number_of_cols; ++i)
        {
            temp.setPixel(i, j, 0, apImage[j][i]);
        }
    }

    return temp;
}


//------------------------------------------------------------------------------
template<typename T> void _makeIsoSurface(const std::string& aMeshLabel,
                    const std::vector<std::vector<std::vector<T>>>& apVolume,
                    T anIsoValue,
                    float aCentrePositionX,
                    float aCentrePositionY,
                    float aCentrePositionZ,
                    float aVoxelWidth,
                    float aVoxelHeight,
                    float aVoxelDepth,
                    const std::string& aUnitOfLength,
                    const std::string& aParent,
                    unsigned char aVerboseLevel)
//------------------------------------------------------------------------------
{
    float unit_of_length = getUnitOfLength(aUnitOfLength);
    float scaling = getUnitOfLength(aUnitOfLength) / getUnitOfLength("mm");

    unsigned int number_of_slices = apVolume.size();

    if (number_of_slices == 0)
    {
        gVirtualXRay::OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    unsigned int number_of_rows = apVolume[0].size();

    if (number_of_rows == 0)
    {
        gVirtualXRay::OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    unsigned int number_of_cols = apVolume[0][0].size();

    if (number_of_cols == 0)
    {
        gVirtualXRay::OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    // Create the image
    gVirtualXRay::Image<T> temp(getImageVector3D(apVolume));

    temp.setSpacing(gVirtualXRay::VEC3(aVoxelWidth, aVoxelHeight, aVoxelDepth) * unit_of_length);

    // Create the mesh
    gVirtualXRay::PolygonMesh mesh_data = temp.marchingCubes(anIsoValue, 0, false, aVerboseLevel);
    
    // Translate the mesh
    //  minus half the size of the volume plus half the size of a voxel
    gVirtualXRay::VEC3 translation_vector(
        (-aVoxelWidth * (number_of_cols + 2) / 2.0 + aVoxelWidth / 2.0) * scaling,
        (-aVoxelHeight * (number_of_rows + 2) / 2.0 + aVoxelHeight / 2.0) * scaling,
        (-aVoxelDepth * (number_of_slices + 2) / 2.0 + aVoxelDepth / 2.0) * scaling
    );

    translation_vector[0] += aCentrePositionX * unit_of_length;
    translation_vector[1] += aCentrePositionY * unit_of_length;
    translation_vector[2] += aCentrePositionZ * unit_of_length;

    mesh_data.applyTranslation(translation_vector);

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
        p_node->setGeometry(mesh_data);
    }
    // Create the node in the scene graph and add the mesh
    else
    {
        g_p_scene_graph->addPolygonMesh(
            aMeshLabel,
            mesh_data,
            gVirtualXRay::VEC3(),
            aParent);
    }
}


//------------------------------------------------------------------------------
template<typename T> void _makeIsoSurface(const std::string& aMeshLabel,
                    const std::vector<std::vector<std::vector<T>>>& apVolume,
                    T aLowerIsoValue,
                    T anUpperIsoValue,
                    float aCentrePositionX,
                    float aCentrePositionY,
                    float aCentrePositionZ,
                    float aVoxelWidth,
                    float aVoxelHeight,
                    float aVoxelDepth,
                    const std::string& aUnitOfLength,
                    const std::string& aParent,
                    unsigned char aVerboseLevel)
//------------------------------------------------------------------------------
{
    float unit_of_length = getUnitOfLength(aUnitOfLength);
    float scaling = getUnitOfLength(aUnitOfLength) / getUnitOfLength("mm");

    unsigned int number_of_slices = apVolume.size();

    if (number_of_slices == 0)
    {
        gVirtualXRay::OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    unsigned int number_of_rows = apVolume[0].size();

    if (number_of_rows == 0)
    {
        gVirtualXRay::OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    unsigned int number_of_cols = apVolume[0][0].size();

    if (number_of_cols == 0)
    {
        gVirtualXRay::OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    // Create the image
    gVirtualXRay::Image<T> temp(getImageVector3D(apVolume));

    temp.setSpacing(gVirtualXRay::VEC3(aVoxelWidth, aVoxelHeight, aVoxelDepth) * unit_of_length);

    // Create the mesh
    gVirtualXRay::PolygonMesh mesh_data = temp.marchingCubes(aLowerIsoValue, anUpperIsoValue, true, aVerboseLevel);
    
    // Translate the mesh
    //  minus half the size of the volume plus half the size of a voxel
    gVirtualXRay::VEC3 translation_vector(
        (-aVoxelWidth * (number_of_cols + 2) / 2.0 + aVoxelWidth / 2.0) * scaling,
        (-aVoxelHeight * (number_of_rows + 2) / 2.0 + aVoxelHeight / 2.0) * scaling,
        (-aVoxelDepth * (number_of_slices + 2) / 2.0 + aVoxelDepth / 2.0) * scaling
    );

    translation_vector[0] += aCentrePositionX * unit_of_length;
    translation_vector[1] += aCentrePositionY * unit_of_length;
    translation_vector[2] += aCentrePositionZ * unit_of_length;

    mesh_data.applyTranslation(translation_vector);

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
        p_node->setGeometry(mesh_data);
    }
    // Create the node in the scene graph and add the mesh
    else
    {
        g_p_scene_graph->addPolygonMesh(
            aMeshLabel,
            mesh_data,
            gVirtualXRay::VEC3(),
            aParent);
    }
}


//---------------------------------------------------
template<typename T> void _makeIsoSurface(const std::string& aMeshLabel,
                    const T* apVolume,
                    int aNumberOfVoxelsZ,
                    int aNumberOfVoxelsY,
                    int aNumberOfVoxelsX,
                    T anIsoValue,
                    float aCentrePositionX,
                    float aCentrePositionY,
                    float aCentrePositionZ,
                    float aVoxelWidth,
                    float aVoxelHeight,
                    float aVoxelDepth,
                    const std::string& aUnitOfLength,
                    const std::string& aParent,
                    unsigned char aVerboseLevel)
//---------------------------------------------------
{
    float unit_of_length = getUnitOfLength(aUnitOfLength);
    float scaling = getUnitOfLength(aUnitOfLength) / getUnitOfLength("mm");

    if (aNumberOfVoxelsZ == 0)
    {
        gVirtualXRay::OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    if (aNumberOfVoxelsY == 0)
    {
        gVirtualXRay::OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    if (aNumberOfVoxelsX == 0)
    {
        gVirtualXRay::OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    // Create the image
    gVirtualXRay::Image<T> temp(apVolume,
        aNumberOfVoxelsX, aNumberOfVoxelsY, aNumberOfVoxelsZ,
        gVirtualXRay::VEC3(aVoxelWidth, aVoxelHeight, aVoxelDepth) * unit_of_length);

    // Create the mesh
    gVirtualXRay::PolygonMesh mesh_data = temp.marchingCubes(anIsoValue, 0, false, aVerboseLevel);
    
    // Translate the mesh
    //  minus half the size of the volume plus half the size of a voxel
    gVirtualXRay::VEC3 translation_vector(
        (-aVoxelWidth * (aNumberOfVoxelsX + 2) / 2.0 + aVoxelWidth / 2.0) * scaling,
        (-aVoxelHeight * (aNumberOfVoxelsY + 2) / 2.0 + aVoxelHeight / 2.0) * scaling,
        (-aVoxelDepth * (aNumberOfVoxelsZ + 2) / 2.0 + aVoxelDepth / 2.0) * scaling
    );

    translation_vector[0] += aCentrePositionX * unit_of_length;
    translation_vector[1] += aCentrePositionY * unit_of_length;
    translation_vector[2] += aCentrePositionZ * unit_of_length;

    mesh_data.applyTranslation(translation_vector);

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
        p_node->setGeometry(mesh_data);
    }
    // Create the node in the scene graph and add the mesh
    else
    {
        g_p_scene_graph->addPolygonMesh(
            aMeshLabel,
            mesh_data,
            gVirtualXRay::VEC3(),
            aParent);
    }    
}


//---------------------------------------------------
template<typename T> void _makeIsoSurface(const std::string& aMeshLabel,
                    const T* apVolume,
                    int aNumberOfVoxelsZ,
                    int aNumberOfVoxelsY,
                    int aNumberOfVoxelsX,
                    T aLowerIsoValue,
                    T anUpperIsoValue,
                    float aCentrePositionX,
                    float aCentrePositionY,
                    float aCentrePositionZ,
                    float aVoxelWidth,
                    float aVoxelHeight,
                    float aVoxelDepth,
                    const std::string& aUnitOfLength,
                    const std::string& aParent,
                    unsigned char aVerboseLevel)
//---------------------------------------------------
{
    float unit_of_length = getUnitOfLength(aUnitOfLength);
    float scaling = getUnitOfLength(aUnitOfLength) / getUnitOfLength("mm");

    if (aNumberOfVoxelsZ == 0)
    {
        gVirtualXRay::OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    if (aNumberOfVoxelsY == 0)
    {
        gVirtualXRay::OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    if (aNumberOfVoxelsX == 0)
    {
        gVirtualXRay::OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    // Create the image
    gVirtualXRay::Image<T> temp(apVolume,
        aNumberOfVoxelsX, aNumberOfVoxelsY, aNumberOfVoxelsZ,
        gVirtualXRay::VEC3(aVoxelWidth, aVoxelHeight, aVoxelDepth) * unit_of_length);

    // Create the mesh
    gVirtualXRay::PolygonMesh mesh_data = temp.marchingCubes(aLowerIsoValue, anUpperIsoValue, true, aVerboseLevel);
    
    // Translate the mesh
    //  minus half the size of the volume plus half the size of a voxel
    gVirtualXRay::VEC3 translation_vector(
        (-aVoxelWidth * (aNumberOfVoxelsX + 2) / 2.0 + aVoxelWidth / 2.0) * scaling,
        (-aVoxelHeight * (aNumberOfVoxelsY + 2) / 2.0 + aVoxelHeight / 2.0) * scaling,
        (-aVoxelDepth * (aNumberOfVoxelsZ + 2) / 2.0 + aVoxelDepth / 2.0) * scaling
    );

    translation_vector[0] += aCentrePositionX * unit_of_length;
    translation_vector[1] += aCentrePositionY * unit_of_length;
    translation_vector[2] += aCentrePositionZ * unit_of_length;

    mesh_data.applyTranslation(translation_vector);

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
        p_node->setGeometry(mesh_data);
    }
    // Create the node in the scene graph and add the mesh
    else
    {
        g_p_scene_graph->addPolygonMesh(
            aMeshLabel,
            mesh_data,
            gVirtualXRay::VEC3(),
            aParent);
    }
}
