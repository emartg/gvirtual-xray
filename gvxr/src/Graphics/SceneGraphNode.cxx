#include <cfloat>

#ifndef GVXR_SCENEGRAPH_NODE_H
#include "gVirtualXRay/SceneGraphNode.h"
#endif


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;


SceneGraphNode::SceneGraphNode(const std::string& aLabel,
                               SceneGraphNode* apParent):
        m_label(aLabel),
		m_p_geometry(0),
        m_p_parent(apParent),
		m_world_properties_up_to_date(false)
{
}


SceneGraphNode::SceneGraphNode(const SceneGraphNode& aNode):
		m_label(aNode.m_label),
		m_local_geometry(aNode.m_local_geometry),
		m_p_geometry(0),
		m_local_transformation(aNode.m_local_transformation),
		m_global_transformation(aNode.m_global_transformation),
		m_position(aNode.m_position),
		m_p_parent(aNode.m_p_parent),
		m_p_children(aNode.m_p_children),
		m_world_properties_up_to_date(aNode.m_world_properties_up_to_date)
{
	if (aNode.m_p_geometry != &aNode.m_local_geometry)
	{
		m_p_geometry = aNode.m_p_geometry;
	}
	else
	{
		m_p_geometry = &m_local_geometry;
	}
}


SceneGraphNode::SceneGraphNode(const std::string& aLabel,
                               const PolygonMesh& aPolygonMesh,
                               const VEC3& aPosition,
                               SceneGraphNode* apParent):
        m_label(aLabel),
        m_local_geometry(aPolygonMesh),
		m_p_geometry(&m_local_geometry),
        m_position(aPosition),
        m_p_parent(apParent),
		m_world_properties_up_to_date(false)
{
}


SceneGraphNode::SceneGraphNode(const std::string& aLabel,
                               PolygonMesh* apPolygonMesh,
                               const VEC3& aPosition,
                               SceneGraphNode* apParent):
        m_label(aLabel),
		m_p_geometry(apPolygonMesh),
        m_position(aPosition),
        m_p_parent(apParent),
		m_world_properties_up_to_date(false)
{
}


SceneGraphNode::~SceneGraphNode()
{
    release();

	// The node has a parent
	if (m_p_parent)
	{
		// Remove the child from its parent
		for (std::vector<gVirtualXRay::SceneGraphNode*>::iterator ite = m_p_parent->m_p_children.begin();
			ite != m_p_parent->m_p_children.end();
			++ite)
		{
			// The node has been found
			if (*ite == this)
			{
				// Erase the child
				m_p_parent->m_p_children.erase(ite);
				m_p_parent = nullptr;

				// Exit the function
				return;
			}
		}
	}
}


SceneGraphNode& SceneGraphNode::operator=(const SceneGraphNode& aNode)
{
	m_label = aNode.m_label;
	m_local_geometry = aNode.m_local_geometry;

	if (aNode.m_p_geometry != &aNode.m_local_geometry)
	{
		m_p_geometry = aNode.m_p_geometry;
	}
	else
	{
		m_p_geometry = &m_local_geometry;
	}

	m_local_transformation = aNode.m_local_transformation;
	m_global_transformation = aNode.m_global_transformation;
	m_position = aNode.m_position;
	m_p_parent = aNode.m_p_parent;
	m_p_children = aNode.m_p_children;
	m_world_properties_up_to_date = aNode.m_world_properties_up_to_date;

	return (*this);
}


void SceneGraphNode::release()
{
    m_local_geometry.reset();
    
    // Remove all the children
    while (m_p_children.size())
    {
		// Delete the first child
		delete* (m_p_children.begin());

		// The line below is not needed as the child deregister itself from its parent
    	//m_p_children.erase(m_p_children.begin());
    }
}


void SceneGraphNode::clear()
{
    release();
}


void SceneGraphNode::display()
{
	if (m_p_geometry) m_p_geometry->display();
}


void SceneGraphNode::displayWireFrame()
{
	if (m_p_geometry) m_p_geometry->displayWireFrame();
}

const SceneGraphNode* SceneGraphNode::getParent() const
{
    return (m_p_parent);
}


unsigned int SceneGraphNode::getNumberOfChildren() const
{
    return (m_p_children.size());
}


SceneGraphNode& SceneGraphNode::getChild(unsigned int i)
{
    if (i >= m_p_children.size())
    {
        throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    return (*m_p_children[i]);
}


const SceneGraphNode& SceneGraphNode::getChild(unsigned int i) const
{
    if (i >= m_p_children.size())
    {
        throw OutOfBoundsException(__FILE__, __FUNCTION__, __LINE__);
    }

    return (*m_p_children[i]);
}



const std::string& SceneGraphNode::getLabel() const
{
    return (m_label);
}


void SceneGraphNode::setGeometry(const PolygonMesh& aPolygonMesh)
{
	m_local_geometry = aPolygonMesh;
	m_p_geometry = &m_local_geometry;
}


void SceneGraphNode::setGeometry(PolygonMesh* apPolygonMesh)
{
	m_p_geometry = apPolygonMesh;
}


PolygonMesh& SceneGraphNode::getPolygonMesh()
{
	if (m_p_geometry)
		{
		return (*m_p_geometry);
		}
	else
		{
		return (m_local_geometry);
		}
}


const PolygonMesh& SceneGraphNode::getPolygonMesh() const
{
	if (m_p_geometry)
		{
		return (*m_p_geometry);
		}
	else
		{
		return (m_local_geometry);
		}
}


MATRIX4 SceneGraphNode::getLocalTransformationMatrix() const
{
    return (m_local_transformation * MATRIX4::buildTranslationMatrix(m_position));
}


MATRIX4 SceneGraphNode::getWorldTransformationMatrix()
{
	if (!m_world_properties_up_to_date)
	{
		updateWorldMatrix();
	}

    return (m_global_transformation);
}


std::map<RATIONAL_NUMBER, VEC3> SceneGraphNode::rayIntersect(
		const VEC3& aRayOrigin,
		const VEC3& aRayDirection,
		const MATRIX4& aTransformationMatrix)
{
	return getPolygonMesh().rayIntersect(
			aRayOrigin,
			aRayDirection,
			getWorldTransformationMatrix() * aTransformationMatrix);
}


void SceneGraphNode::setLocalTransformation(const MATRIX4& aMatrix)
{
	m_local_transformation = aMatrix;


	std::vector<SceneGraphNode*> p_waiting_list;
	p_waiting_list.push_back(this);

	while (p_waiting_list.size())
	{
		SceneGraphNode* p_node = p_waiting_list.back();
		p_node->m_world_properties_up_to_date = false;
		p_waiting_list.pop_back();

		// Add children
		for (std::vector<gVirtualXRay::SceneGraphNode*>::iterator ite = p_node->m_p_children.begin();
				ite != p_node->m_p_children.end();
				++ite)
		{
			p_waiting_list.push_back(*ite);
		}
	}
}


void SceneGraphNode::applyLocalTransformation(const MATRIX4& aMatrix)
{
    m_local_transformation = m_local_transformation * aMatrix;

	std::vector<SceneGraphNode*> p_waiting_list;
	p_waiting_list.push_back(this);

	while (p_waiting_list.size())
	{
		SceneGraphNode* p_node = p_waiting_list.back();
		p_node->m_world_properties_up_to_date = false;
		p_waiting_list.pop_back();

		// Add children
		for (std::vector<gVirtualXRay::SceneGraphNode*>::iterator ite = p_node->m_p_children.begin();
				ite != p_node->m_p_children.end();
				++ite)
		{
			p_waiting_list.push_back(*ite);
		}
	}
}


void SceneGraphNode::addChild(const std::string& aLabel)
{
	m_p_children.push_back(new SceneGraphNode(aLabel, this));
}


void SceneGraphNode::addChild(const std::string& aLabel,
                              const PolygonMesh& aPolygonMesh,
                              const VEC3& aPosition)
{
    m_p_children.push_back(new SceneGraphNode(aLabel, aPolygonMesh, aPosition, this));
}


void SceneGraphNode::addChild(const std::string& aLabel,
		PolygonMesh* apPolygonMesh,
		const VEC3& aPosition)
{
    m_p_children.push_back(new SceneGraphNode(aLabel, apPolygonMesh, aPosition, this));
}


// AABB is in local coordinate system
// aRayOrigin and aRayDirection are in global coordinate system
std::map<RATIONAL_NUMBER, VEC3> SceneGraphNode::rayIntersectAABB(const VEC3& aRayOrigin,
                                                                 const VEC3& aRayDirection)
{
	MATRIX4 global2local = getWorldTransformationMatrix().getInverse();
	VEC3 ray_origin_in_local_system = global2local * aRayOrigin;
	VEC3 ray_direction_in_local_system = global2local * aRayDirection;

	const int NUMDIM = 3;
	const int RIGHT = 0;
	const int LEFT = 1;
	const int MIDDLE = 2;

	char inside = true;
	char quadrant[NUMDIM];
	unsigned int whichPlane;
	double maxT[NUMDIM];
	double candidatePlane[] = {0.0, 0.0, 0.0};
	VEC3 coord;
	VEC3 minB = m_local_geometry.getLocalMinCorner();
	VEC3 maxB = m_local_geometry.getLocalMaxCorner();

	std::map<RATIONAL_NUMBER, VEC3> p_intersection_set;

	/* Find candidate planes; this loop can be avoided if
	rays cast all from the eye(assume perpsective view) */
	for (unsigned int i = 0; i < 3; i++)
		if (ray_origin_in_local_system[i] < minB[i]) {
			quadrant[i] = LEFT;
			candidatePlane[i] = minB[i];
			inside = false;
		}
		else if (ray_origin_in_local_system[i] > maxB[i]) {
			quadrant[i] = RIGHT;
			candidatePlane[i] = maxB[i];
			inside = false;
		}
		else {
			quadrant[i] = MIDDLE;
		}

	/* Ray origin inside bounding box */
	if (inside) {
		coord = ray_origin_in_local_system;
		p_intersection_set[0.0] = coord;
		return p_intersection_set;
	}


	/* Calculate T distances to candidate planes */
	for (unsigned int i = 0; i < 3; i++)
		if (quadrant[i] != MIDDLE && ray_direction_in_local_system[i] != 0.)
			maxT[i] = (candidatePlane[i] - ray_origin_in_local_system[i]) / ray_direction_in_local_system[i];
		else
			maxT[i] = -1.;

	/* Get largest of the maxT's for final choice of intersection */
	whichPlane = 0;
	for (unsigned int i = 1; i < 3; i++)
		if (maxT[whichPlane] < maxT[i])
			whichPlane = i;

	/* Check final candidate actually inside box */
	if (maxT[whichPlane] < 0.) return p_intersection_set;
	for (unsigned int i = 0; i < NUMDIM; i++)
		if (whichPlane != i) {
			coord[i] = ray_origin_in_local_system[i] + maxT[whichPlane] * ray_direction_in_local_system[i];
			if (coord[i] < minB[i] || coord[i] > maxB[i])
				return p_intersection_set;
		}
		else {
			coord[i] = candidatePlane[i];
		}

	p_intersection_set[maxT[whichPlane]] = coord;

	return p_intersection_set;
}


VEC3 SceneGraphNode::getNodeAndChildrenWorldMinCorner()
{
	VEC3 min_bbox(FLT_MAX, FLT_MAX, FLT_MAX);

	std::vector<SceneGraphNode*> p_waiting_list;
	p_waiting_list.push_back(this);

	while (p_waiting_list.size())
	{
		SceneGraphNode* p_node = p_waiting_list.back();
		p_waiting_list.pop_back();


		if (p_node->getPolygonMesh().getVertexNumber())
		{
			// Process the node
			VEC3 node_min_bbox = p_node->getWorldTransformationMatrix() * p_node->getPolygonMesh().getLocalMinCorner();

			if (min_bbox[0] > node_min_bbox[0]) min_bbox[0] = node_min_bbox[0];
			if (min_bbox[1] > node_min_bbox[1]) min_bbox[1] = node_min_bbox[1];
			if (min_bbox[2] > node_min_bbox[2]) min_bbox[2] = node_min_bbox[2];
		}

		// Add children
		for (std::vector<gVirtualXRay::SceneGraphNode*>::iterator ite = p_node->m_p_children.begin();
				ite != p_node->m_p_children.end();
				++ite)
		{
			p_waiting_list.push_back(*ite);
		}
	}

	return (min_bbox);
}


VEC3 SceneGraphNode::getNodeAndChildrenWorldMaxCorner()
{
	VEC3 max_bbox(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	std::vector<SceneGraphNode*> p_waiting_list;
	p_waiting_list.push_back(this);

	while (p_waiting_list.size())
	{
		SceneGraphNode* p_node = p_waiting_list.back();
		p_waiting_list.pop_back();

		if (p_node->getPolygonMesh().getVertexNumber())
		{
			// Process the node
			VEC3 node_max_bbox = p_node->getWorldTransformationMatrix() * p_node->getPolygonMesh().getLocalMaxCorner();

			if (max_bbox[0] < node_max_bbox[0]) max_bbox[0] = node_max_bbox[0];
			if (max_bbox[1] < node_max_bbox[1]) max_bbox[1] = node_max_bbox[1];
			if (max_bbox[2] < node_max_bbox[2]) max_bbox[2] = node_max_bbox[2];
		}

		// Add children
		for (std::vector<gVirtualXRay::SceneGraphNode*>::iterator ite = p_node->m_p_children.begin();
				ite != p_node->m_p_children.end();
				++ite)
		{
			p_waiting_list.push_back(*ite);
		}
	}

	return (max_bbox);
}


VEC3 SceneGraphNode::getNodeOnlyWorldMinCorner()
{
	VEC3 min_bbox(FLT_MAX, FLT_MAX, FLT_MAX);

	if (getPolygonMesh().getVertexNumber())
	{
		// Process the node
		VEC3 node_min_bbox = getWorldTransformationMatrix() * getPolygonMesh().getLocalMinCorner();

		if (min_bbox[0] > node_min_bbox[0]) min_bbox[0] = node_min_bbox[0];
		if (min_bbox[1] > node_min_bbox[1]) min_bbox[1] = node_min_bbox[1];
		if (min_bbox[2] > node_min_bbox[2]) min_bbox[2] = node_min_bbox[2];
    }

	return (min_bbox);
}


VEC3 SceneGraphNode::getNodeOnlyWorldMaxCorner()
{
	VEC3 max_bbox(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	if (getPolygonMesh().getVertexNumber())
	{
		// Process the node
		VEC3 node_max_bbox = getWorldTransformationMatrix() * getPolygonMesh().getLocalMaxCorner();

		if (max_bbox[0] < node_max_bbox[0]) max_bbox[0] = node_max_bbox[0];
		if (max_bbox[1] < node_max_bbox[1]) max_bbox[1] = node_max_bbox[1];
		if (max_bbox[2] < node_max_bbox[2]) max_bbox[2] = node_max_bbox[2];
	}

	return (max_bbox);
}


/*
VEC3 SceneGraphNode::getLocalMinCorner() const
{
	return (getLocalTransformationMatrix() * m_geometry.getLocalMinCorner());
}


VEC3 SceneGraphNode::getLocalMaxCorner() const
{
	return (getLocalTransformationMatrix() * m_geometry.getLocalMaxCorner());
}


VEC3 SceneGraphNode::getWorldMinCorner()
{
	return (getWorldTransformationMatrix() * getLocalMinCorner());
}


VEC3 SceneGraphNode::getWorldMaxCorner()
{
	return (getWorldTransformationMatrix() * getLocalMaxCorner());
}*/


const PhotonCrossSection& SceneGraphNode::getPhotonCrossSection() const
{
	return (getPolygonMesh().getPhotonCrossSection());
}


SceneGraphNode& SceneGraphNode::getChild(const std::string& aLabel)
{
    for (std::vector<SceneGraphNode*>::iterator ite = m_p_children.begin();
    		ite != m_p_children.end();
    		++ite)
    {
    	if ((*ite)->m_label == aLabel)
    	{
    		return **ite;
    	}
    }

    throw Exception(__FILE__, __FUNCTION__, __LINE__, "Node not found");
}


const SceneGraphNode& SceneGraphNode::getChild(const std::string& aLabel) const
{
    for (std::vector<SceneGraphNode*>::const_iterator ite = m_p_children.begin();
    		ite != m_p_children.end();
    		++ite)
    {
    	if ((*ite)->m_label == aLabel)
    	{
    		return **ite;
    	}
    }

    throw Exception(__FILE__, __FUNCTION__, __LINE__, "Node not found");
}


void SceneGraphNode::updateWorldMatrix()
{
	if 	(!m_world_properties_up_to_date)
	{
		m_global_transformation = getLocalTransformationMatrix();

		if (m_p_parent)
		{
			m_global_transformation = m_p_parent->getWorldTransformationMatrix() * m_global_transformation;
		}

		m_world_properties_up_to_date = true;
	}
}




/*
SceneGraphNode& SceneGraphNode::operator=(const SceneGraphNode& aNode)
{
	// The current node has children
	// They need to be deleted
	if (m_p_children.size())
	{
		// Add all the children of the node to the temp list
		std::vector<SceneGraphNode*> p_temp_pool_of_nodes;
		for (std::vector<SceneGraphNode>::iterator ite = m_p_children.begin();
				ite != m_p_children.end();
				++ite)
		{
			p_temp_pool_of_nodes.push_back(&(*ite));
		}

		// Process until the list is empty
		while (p_temp_pool_of_nodes.size())
		{

		}
	}

	return (*this);
}
*/
