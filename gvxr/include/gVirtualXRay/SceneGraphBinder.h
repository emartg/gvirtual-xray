#ifndef GVXR_SCENEGRAPH_BINDER_H
#define GVXR_SCENEGRAPH_BINDER_H

#include <map>
#include <string>

#ifndef GVXR_SCENEGRAPH_NODE_H
#include "gVirtualXRay/SceneGraphNode.h"
#endif

// For MATRIX4
#ifndef GVXR_TYPES_H
#include "gVirtualXRay/Types.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


class SceneGraphBinder
{
public:
	SceneGraphBinder();
	SceneGraphBinder(const SceneGraphBinder& aSceneGraph);
	SceneGraphBinder(const std::string& aFileName, RATIONAL_NUMBER aUnitOfLength);
	SceneGraphBinder(const char* aFileName, RATIONAL_NUMBER aUnitOfLength);
	virtual ~SceneGraphBinder();

	SceneGraphBinder& operator=(const SceneGraphBinder& aSceneGraph);


	virtual void loadSceneGraph(const char* aFileName, RATIONAL_NUMBER aUnitOfLength);


	virtual void loadSceneGraph(const std::string& aFileName, RATIONAL_NUMBER aUnitOfLength);

	void clear();


	//const PolygonMesh& getPolygonMesh(const std::string& aLabel) const;
	//const PolygonMesh& getPolygonMesh(const char* aLabel) const;

	//PolygonMesh& getPolygonMesh(const std::string& aLabel);
	//PolygonMesh& getPolygonMesh(const char* aLabel);



	void display(GLint aShaderID);

	void scale(const std::string& aNodeLabel,
			const VEC3& aScalingFactorSet);

	virtual void scale(const char* aNodeLabel,
			const VEC3& aScalingFactorSet);

	void rotate(const std::string& aNodeLabel,
			float aRotationAngleInDegrees,
			const VEC3& aRotationAxis);


	virtual void rotate(const char* aNodeLabel,
			float aRotationAngleInDegrees,
			const VEC3& aRotationAxis);



	void translate(const std::string& aNodeLabel,
			const VEC3& aTranslationVector);

	virtual void translate(const char* aNodeLabel,
			const VEC3& aTranslationVector);


	MATRIX4 getLocalTransformationMatrix(const std::string& aNodeLabel) const;
	MATRIX4 getLocalTransformationMatrix(const char* aNodeLabel) const;
	MATRIX4 getWorldTransformationMatrix(const std::string& aNodeLabel) const;
	MATRIX4 getWorldTransformationMatrix(const char* aNodeLabel) const;

	void moveToCentre(const std::string& aNodeLabel);
	void moveToCentre(const char* aNodeLabel);
	void moveToCenter(const std::string& aNodeLabel);
	void moveToCenter(const char* aNodeLabel);

	void moveToCentre();
	void moveToCenter();

	virtual MATRIX4 getRootNodeTransformationMatrix() const;
	virtual MATRIX4 getNodeLocalTransformationMatrix(const char* aNodeLabel) const;
	virtual MATRIX4 getNodeWorldTransformationMatrix(const char* aNodeLabel) const;

	virtual void setRootNodeTransformationMatrix(const MATRIX4& aTransformationMatrix);
	virtual void setNodeLocalTransformationMatrix(const char* aNodeLabel, const MATRIX4& aTransformationMatrix);

	std::pair<VEC3, VEC3> getBoundingBox();


    void addPolygonMesh(const std::string& aLabel, const PolygonMesh& aPolygonMesh, const VEC3& aPosition, const std::string& aParent = "root");


    SceneGraphNode* getNode(const std::string& aLabel);
    const SceneGraphNode* getNode(const std::string& aLabel) const;

protected:
	SceneGraphNode m_root_node;
	RATIONAL_NUMBER m_unit_of_length;
};


} // namespace gVirtualXRay


#endif // GVXR_SCENEGRAPH_BINDER_H
