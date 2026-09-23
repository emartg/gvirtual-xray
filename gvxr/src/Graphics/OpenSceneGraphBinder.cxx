/**
********************************************************************************
*
*   @file       SceneGraphBinder.cxx
*
*   @brief      Class to handle scenegraphes using The OpenSceneGraph Project,
*   			see http://www.openscenegraph.org/
*
*   @version    1.0
*
*   @date       16/07/2018
*
*   @author     Dr Franck P. Vidal
*
*   License
*   BSD 3-Clause License.
*
*   For details on use and redistribution please refer
*   to http://opensource.org/licenses/BSD-3-Clause
*
*   Copyright
*   (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*   http://www.fpvidal.net/, Jul 2018, 2018, version 1.0, BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifndef GVXR_GVIRTUALXRAY_CONFIG_H
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#ifdef HAS_OPENSCENEGRAPH
#include <osg/ShapeDrawable>
#include <osg/TriangleFunctor>
#endif

#ifndef GVXR_OpenSceneGraphBinder_h
#include "gVirtualXRay/OpenSceneGraphBinder.h"
#endif

// For types such as VEC3, MATRIX4
#ifndef GVXR_TYPES_H
#include "gVirtualXRay/Types.h"
#endif

#ifndef GVXR_NOT_IMPLEMENTED_EXCEPTION_H
#include "gVirtualXRay/NotImplementedException.h"
#endif


//******************************************************************************
//	Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//  Function declarations
//******************************************************************************
#ifdef HAS_OPENSCENEGRAPH
osg::Matrixd* getWorldCoords( osg::Node* node);
#endif

//******************************************************************************
//  Class declarations
//******************************************************************************

#ifdef HAS_OPENSCENEGRAPH

struct NormalPrint
{
public:
	std::vector<float> m_p_vertex_set;
	std::vector<float> m_p_normal_set;


	void Init()
	{
		m_p_vertex_set.clear();
		m_p_normal_set.clear();
	}

	void operator() (const osg::Vec3& v1,const osg::Vec3& v2,const osg::Vec3& v3, bool)
	{
		osg::Vec3 normal_vector = (v2-v1)^(v3-v2);
		normal_vector.normalize();

		m_p_vertex_set.push_back(v1[0]);
		m_p_vertex_set.push_back(v1[1]);
		m_p_vertex_set.push_back(v1[2]);
		m_p_vertex_set.push_back(v2[0]);
		m_p_vertex_set.push_back(v2[1]);
		m_p_vertex_set.push_back(v2[2]);
		m_p_vertex_set.push_back(v3[0]);
		m_p_vertex_set.push_back(v3[1]);
		m_p_vertex_set.push_back(v3[2]);

		m_p_normal_set.push_back(normal_vector[0]);
		m_p_normal_set.push_back(normal_vector[1]);
		m_p_normal_set.push_back(normal_vector[2]);
	}

	void MakePolygonMesh(gVirtualXRay::PolygonMesh& aPolygonMesh)
	{
		aPolygonMesh.setInternalData(GL_TRIANGLES,
	            &m_p_vertex_set,
	            &m_p_normal_set,
	            true,
				GL_STATIC_DRAW);

		aPolygonMesh.computeNormalVectors();
	}
};


class FindNamedNode: public osg::NodeVisitor
{
public:
	FindNamedNode(const char* aName):
		osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN),
		m_name(aName)
	{}


	FindNamedNode( const std::string& aName ):
		osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN),
		m_name(aName)
	{}

    // This method gets called for every node in the scene
    //   graph. Check each node to see if its name matches
    //   out target. If so, save the node's address.
    virtual void apply(osg::Node &aNode)
    {
    	// The node has been found
		if (aNode.getName() == m_name)
		{
			n_p_node = &aNode;
		}
		// Keep traversing the rest of the scene graph.
		else
		{
			traverse(aNode);
		}
    }

    osg::Node* getNode()
    {
    	return n_p_node.get();
    }

    const osg::Node* getNode() const
    {
    	return n_p_node.get();
    }


protected:
    std::string m_name;
    osg::ref_ptr<osg::Node> n_p_node;
};


// Visitor to return the world coordinates of a node.
// It traverses from the starting node to the parent.
// The first time it reaches a root node, it stores the world coordinates of
// the node it started from.  The world coordinates are found by concatenating all
// the matrix transforms found on the path from the start node to the root node.

class getWorldCoordOfNodeVisitor : public osg::NodeVisitor
{
public:
   getWorldCoordOfNodeVisitor():
      osg::NodeVisitor(NodeVisitor::TRAVERSE_PARENTS), done(false)
      {
         wcMatrix= new osg::Matrixd();
      }
      virtual void apply(osg::Node &node)
      {
         if (!done)
         {
            if ( 0 == node.getNumParents() ) // no parents
            {
               wcMatrix->set( osg::computeLocalToWorld(this->getNodePath()) );
               done = true;
            }
            traverse(node);
         }
      }
      osg::Matrixd* giveUpDaMat()
      {
         return wcMatrix;
      }
private:
   bool done;
   osg::Matrix* wcMatrix;
};
#endif

//******************************************************************************
//  Implementation
//******************************************************************************

#ifdef HAS_OPENSCENEGRAPH
//--------------------------------------------
osg::Matrixd* getWorldCoords( osg::Node* node)
//--------------------------------------------
{
   getWorldCoordOfNodeVisitor* ncv = new getWorldCoordOfNodeVisitor();
   if (node && ncv)
   {
      node->accept(*ncv);
      return ncv->giveUpDaMat();
   }
   else
   {
      return NULL;
   }
}

#endif


OpenSceneGraphBinder::OpenSceneGraphBinder():
#ifdef HAS_OPENSCENEGRAPH
		SceneGraphBinder(),
		m_p_scene_graph(0)
#else
		SceneGraphBinder()
#endif
{
#ifndef HAS_OPENSCENEGRAPH
	throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__, "OpenSceneGraph is not supported in your implementation. To enable it, install OpenSceneGraph and re-compile gVirtualXRay to support it.");
#endif
}


OpenSceneGraphBinder::OpenSceneGraphBinder(const OpenSceneGraphBinder& aSceneGraph):
		SceneGraphBinder(aSceneGraph)
{
#ifdef HAS_OPENSCENEGRAPH
	setSceneGraph(aSceneGraph.m_p_scene_graph);
#else
	throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__, "OpenSceneGraph is not supported in your implementation. To enable it, install OpenSceneGraph and re-compile gVirtualXRay to support it.");
#endif
}


OpenSceneGraphBinder::OpenSceneGraphBinder(const char* aFileName,
	    								   RATIONAL_NUMBER aUnitOfLength):
		SceneGraphBinder(aFileName, aUnitOfLength)
{}


OpenSceneGraphBinder::OpenSceneGraphBinder(const std::string& aFileName,
	    								   RATIONAL_NUMBER aUnitOfLength):
		SceneGraphBinder(aFileName, aUnitOfLength)
{}


#ifdef HAS_OPENSCENEGRAPH
OpenSceneGraphBinder::OpenSceneGraphBinder(osg::Group* apSceneGraph):
		SceneGraphBinder()
{
	setSceneGraph(apSceneGraph);
}
#endif


OpenSceneGraphBinder::~OpenSceneGraphBinder()
{}


OpenSceneGraphBinder& OpenSceneGraphBinder::operator=(const OpenSceneGraphBinder& aSceneGraph)
{
	SceneGraphBinder::operator=(aSceneGraph);

#ifdef HAS_OPENSCENEGRAPH
	setSceneGraph(aSceneGraph.m_p_scene_graph);
#endif

	return (*this);
}


void OpenSceneGraphBinder::loadSceneGraph(const char* aFileName, RATIONAL_NUMBER aUnitOfLength)
{
	throw NotImplementedException(__FILE__, __FUNCTION__, __LINE__);
}


#ifdef HAS_OPENSCENEGRAPH
void OpenSceneGraphBinder::setSceneGraph(osg::Group* apSceneGraph)
{
	m_p_scene_graph = apSceneGraph;

	m_geode_finder.apply(*m_p_scene_graph);
	m_p_polygon_mesh_set.clear();

	unsigned short HU = 100;

	for (std::vector<osg::Geode*>::iterator ite = m_geode_finder.getNodeList().begin();
    		ite != m_geode_finder.getNodeList().end();
    		++ite) {

    	if ((*ite)->getNumDrawables() == 1)
    	{
    		if (! strcmp((*ite)->getDrawable(0)->className(), "ShapeDrawable"))
    		{
    	    	osg::ShapeDrawable* p_drawable;
    			osg::TriangleFunctor<NormalPrint> tri;
    			gVirtualXRay::PolygonMesh mesh;

    	    	//the kind of Drawable we have is a Geometry, so lets get a reference to it:
    	    	p_drawable = (osg::ShapeDrawable*) (*ite)->getDrawable(0);


				tri.Init();
				p_drawable->accept(tri);
				tri.MakePolygonMesh(mesh);

				//geometry = (osg::Geometry *) (*ite)->getDrawable(0);
				osg::Vec4 color = p_drawable->getColor();

				gVirtualXRay::VEC3 base_colour(color[0], color[1], color[2]);
				base_colour.normalise();

				mesh.getMaterial().setAmbientColour(
					base_colour.getX() * 0.19225,
					base_colour.getY() * 0.19225,
					base_colour.getZ() * 0.19225,
					color[3]);

				mesh.getMaterial().setDiffuseColour(
					base_colour.getX() * 0.50754,
					base_colour.getY() * 0.50754,
					base_colour.getZ() * 0.50754,
					color[3]);

				mesh.getMaterial().setSpecularColour(
					base_colour.getX() * 0.50827,
					base_colour.getY() * 0.50827,
					base_colour.getZ() * 0.50827,
					color[3]);

				mesh.getMaterial().setShininess(50);

		    	// Add a new polygon mesh
				m_p_polygon_mesh_set[p_drawable->getName()] = mesh;
    		}
    	}
    }
}
#endif


//-----------------------------------------
void OpenSceneGraphBinder::updateMatrices()
//-----------------------------------------
{
#ifdef HAS_OPENSCENEGRAPH
    unsigned int i = 0;
	for (std::vector<osg::Geode*>::iterator ite = m_geode_finder.getNodeList().begin();
    		ite != m_geode_finder.getNodeList().end();
    		++ite) {

    	if ((*ite)->getNumDrawables() == 1)
    	{
    		if (! strcmp((*ite)->getDrawable(0)->className(), "ShapeDrawable") ||
    				! strcmp((*ite)->getDrawable(0)->className(), "Drawable"))
    		{
    	    	osg::Drawable* p_drawable;

    	    	//the kind of Drawable we have is a Geometry, so lets get a reference to it:
    	    	p_drawable = (osg::Drawable*) (*ite)->getDrawable(0);

    	    	osg::Matrixd matrix = *getWorldCoords(p_drawable);

    	    	gVirtualXRay::MATRIX4 modelling_matrix = gVirtualXRay::MATRIX4(matrix.ptr()[0],
    	    			matrix.ptr()[1],
    	    			matrix.ptr()[2],
    	    			matrix.ptr()[3],
    	    			matrix.ptr()[4],
    	    			matrix.ptr()[5],
    	    			matrix.ptr()[6],
    	    			matrix.ptr()[7],
    	    			matrix.ptr()[8],
    	    			matrix.ptr()[9],
    	    			matrix.ptr()[10],
    	    			matrix.ptr()[11],
    	    			matrix.ptr()[12],
    	    			matrix.ptr()[13],
    	    			matrix.ptr()[14],
    	    			matrix.ptr()[15]);

    	    	m_p_polygon_mesh_set[p_drawable->getName()].setTransformationMatrix(modelling_matrix);

    	        ++i;
    		}
    	}
	}
#endif
}


void OpenSceneGraphBinder::scale(const char* aNodeLabel,
		                         const gVirtualXRay::VEC3& aScalingFactorSet)
{
	throw NotImplementedException(__FILE__, __FUNCTION__, __LINE__);
}


//------------------------------------------------------------------------
void OpenSceneGraphBinder::rotate(const char* aNodeLabel,
		                          float aRotationAngleInDegrees,
								  const gVirtualXRay::VEC3& aRotationAxis)
//------------------------------------------------------------------------
{
#ifdef HAS_OPENSCENEGRAPH
	// Find the node
	osg::MatrixTransform* p_node = dynamic_cast<osg::MatrixTransform*>(findNode(aNodeLabel));

	// The node has been found
	if (p_node)
	{
		// Create the rotation matrix
		osg::Matrix rotation_matrix;
		rotation_matrix.makeRotate(osg::PI * aRotationAngleInDegrees /180.0, aRotationAxis[0], aRotationAxis[1], aRotationAxis[2]);

		// Update the node's transformation matrix
		p_node->setMatrix(rotation_matrix * p_node->getMatrix());
	}
	// The node has not been found
	else
	{
		throw gVirtualXRay::Exception(__FILE__, __FUNCTION__, __LINE__, "Unknown node");
	}
#endif
}


void OpenSceneGraphBinder::translate(const char* aNodeLabel,
									 const VEC3& aTranslationVector)
{
	throw NotImplementedException(__FILE__, __FUNCTION__, __LINE__);
}


/*
//--------------------------------------------------
void rotate(float angle,osg::MatrixTransform *joint)
//--------------------------------------------------
{
}*/


//-----------------------------------------------------------------------------------------------
gVirtualXRay::MATRIX4 OpenSceneGraphBinder::getTransformationMatrix(const char* aNodeLabel) const
//-----------------------------------------------------------------------------------------------
{
	// Delete next line and add code here to update the transformation matrices in m_p_polygon_mesh_set
	throw NotImplementedException(__FILE__, __FUNCTION__, __LINE__);

	return (gVirtualXRay::MATRIX4());
}


void OpenSceneGraphBinder::moveToCentre()
{
	throw NotImplementedException(__FILE__, __FUNCTION__, __LINE__);
}


void OpenSceneGraphBinder::moveToCenter()
{
	moveToCentre();
}


MATRIX4 OpenSceneGraphBinder::getRootNodeTransformationMatrix() const
{
	throw NotImplementedException(__FILE__, __FUNCTION__, __LINE__);
}


MATRIX4 OpenSceneGraphBinder::getNodeTransformationMatrix(const char* aNodeLabel) const
{
	throw NotImplementedException(__FILE__, __FUNCTION__, __LINE__);
}


void OpenSceneGraphBinder::setRootNodeTransformationMatrix(const MATRIX4& aTransformationMatrix)
{
	throw NotImplementedException(__FILE__, __FUNCTION__, __LINE__);
}


void OpenSceneGraphBinder::setNodeTransformationMatrix(const char* aNodeLabel, const MATRIX4& aTransformationMatrix)
{
	throw NotImplementedException(__FILE__, __FUNCTION__, __LINE__);
}


#ifdef HAS_OPENSCENEGRAPH

//---------------------------------------------------------------------------
const osg::Node* OpenSceneGraphBinder::findNode(const char* aNodeLabel) const
//---------------------------------------------------------------------------
{
	FindNamedNode node_finder(aNodeLabel);
	m_p_scene_graph->accept(node_finder);
	return (node_finder.getNode());
}


//---------------------------------------------------------------
osg::Node* OpenSceneGraphBinder::findNode(const char* aNodeLabel)
//---------------------------------------------------------------
{
	FindNamedNode node_finder(aNodeLabel);
	m_p_scene_graph->accept(node_finder);
	return (node_finder.getNode());
}
#endif
