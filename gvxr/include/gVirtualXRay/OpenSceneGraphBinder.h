#ifndef GVXR_OpenSceneGraphBinder_h
#define GVXR_OpenSceneGraphBinder_h

#ifdef HAS_OPENSCENEGRAPH
#include <osg/Node>
#include <osg/Group>
#include <osg/MatrixTransform>
#endif

#ifndef GVXR_GEODE_FINDER_H
#include "gVirtualXRay/GeodeFinder.h"
#endif

#ifndef GVXR_SCENEGRAPH_BINDER_H
#include "gVirtualXRay/SceneGraphBinder.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


class OpenSceneGraphBinder: public SceneGraphBinder
{
public:
	OpenSceneGraphBinder();
	OpenSceneGraphBinder(const OpenSceneGraphBinder& aSceneGraph);
	OpenSceneGraphBinder(const std::string& aFileName, RATIONAL_NUMBER aUnitOfLength);
	OpenSceneGraphBinder(const char* aFileName, RATIONAL_NUMBER aUnitOfLength);

#ifdef HAS_OPENSCENEGRAPH
	OpenSceneGraphBinder(osg::Group* apSceneGraph);
#endif

	virtual ~OpenSceneGraphBinder();

	OpenSceneGraphBinder& operator=(const OpenSceneGraphBinder& aSceneGraph);


	virtual void loadSceneGraph(const char* aFileName, RATIONAL_NUMBER aUnitOfLength);


#ifdef HAS_OPENSCENEGRAPH
	void setSceneGraph(osg::Group* apSceneGraph);
#endif

	virtual void updateMatrices();


	virtual void scale(const char* aNodeLabel,
			const gVirtualXRay::VEC3& aScalingFactorSet);


	virtual void rotate(const char* aNodeLabel,
			float aRotationAngleInDegrees,
			const gVirtualXRay::VEC3& aRotationAxis);

	virtual void translate(const char* aNodeLabel,
			const VEC3& aTranslationVector);

	virtual gVirtualXRay::MATRIX4 getTransformationMatrix(const char* aNodeLabel) const;

	virtual void moveToCentre();
	virtual void moveToCenter();

	virtual MATRIX4 getRootNodeTransformationMatrix() const;
	virtual MATRIX4 getNodeTransformationMatrix(const char* aNodeLabel) const;

	virtual void setRootNodeTransformationMatrix(const MATRIX4& aTransformationMatrix);
	virtual void setNodeTransformationMatrix(const char* aNodeLabel, const MATRIX4& aTransformationMatrix);


#ifdef HAS_OPENSCENEGRAPH
	osg::Node* findNode(const char* aNodeLabel);
	const osg::Node* findNode(const char* aNodeLabel) const;
#endif


private:

#ifdef HAS_OPENSCENEGRAPH
	osg::Group* m_p_scene_graph;
	GeodeFinder m_geode_finder;
#endif
};


} // namespace gVirtualXRay


#endif // GVXR_OpenSceneGraphBinder_h
