#ifndef GVXR_SCENEGRAPH_NODE_H
#define GVXR_SCENEGRAPH_NODE_H


#include <string>
#include <vector>

#ifndef GVXR_TYPES_H
#include "gVirtualXRay/Types.h"
#endif

#ifndef GVXR_POLYGON_MESH_H
#include "gVirtualXRay/PolygonMesh.h"
#endif

#ifndef GVXR_PHOTON_CROSS_SECTION_H
#include "gVirtualXRay/PhotonCrossSection.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


class SceneGraphBinder;


class SceneGraphNode
{
public:
    SceneGraphNode(const std::string& aLabel = "",
			SceneGraphNode* apParent = 0);


    SceneGraphNode(const SceneGraphNode& aNode);


    SceneGraphNode(const std::string& aLabel,
    		const PolygonMesh& aPolygonMesh,
			const VEC3& aPosition,
			SceneGraphNode* apParent);


    SceneGraphNode(const std::string& aLabel,
    		PolygonMesh* apPolygonMesh,
			const VEC3& aPosition,
			SceneGraphNode* apParent);


    ~SceneGraphNode();


    SceneGraphNode& operator= (const SceneGraphNode& aNode);

    void release();
    void clear();

    void display();
    void displayWireFrame();

    const SceneGraphNode* getParent() const;


    unsigned int getNumberOfChildren() const;


    gVirtualXRay::SceneGraphNode& getChild(unsigned int i);


    const gVirtualXRay::SceneGraphNode& getChild(unsigned int i) const;


    gVirtualXRay::SceneGraphNode& getChild(const std::string& aLabel);


    const gVirtualXRay::SceneGraphNode& getChild(const std::string& aLabel) const;


    const std::string& getLabel() const;


    void setGeometry(const PolygonMesh& aPolygonMesh);


    void setGeometry(PolygonMesh* apPolygonMesh);


    PolygonMesh& getPolygonMesh();


    const PolygonMesh& getPolygonMesh() const;


    MATRIX4 getLocalTransformationMatrix() const;


    MATRIX4 getWorldTransformationMatrix();


    void setLocalTransformation(const MATRIX4& aMatrix);


    void applyLocalTransformation(const MATRIX4& aMatrix);


    void addChild(const std::string& aLabel);


    void addChild(const std::string& aLabel, const PolygonMesh& aPolygonMesh, const VEC3& aPosition = VEC3());


    void addChild(const std::string& aLabel, PolygonMesh* apPolygonMesh, const VEC3& aPosition = VEC3());

    // AABB is in local coordinate system
    // aRayOrigin and aRayDirection are in global coordinate system
    std::map<RATIONAL_NUMBER, VEC3> rayIntersectAABB(
        const VEC3& aRayOrigin,
        const VEC3& aRayDirection);


    //VEC3 getLocalMinCorner() const;
    //VEC3 getLocalMaxCorner() const;
    //VEC3 getWorldMinCorner();
    //VEC3 getWorldMaxCorner();
    VEC3 getNodeAndChildrenWorldMinCorner();
    VEC3 getNodeAndChildrenWorldMaxCorner();

    VEC3 getNodeOnlyWorldMinCorner();
    VEC3 getNodeOnlyWorldMaxCorner();

    const PhotonCrossSection& getPhotonCrossSection() const;

    std::map<RATIONAL_NUMBER, VEC3> rayIntersect(
    		const VEC3& aRayOrigin,
			const VEC3& aRayDirection,
			const MATRIX4& aTransformationMatrix);


    //SceneGraphNode& operator=(const SceneGraphNode& aNode);


    friend class SceneGraphBinder;


protected:
    void updateWorldMatrix();

    std::string m_label;

    PolygonMesh   m_local_geometry;
    PolygonMesh*  m_p_geometry;

    MATRIX4     m_local_transformation;
    MATRIX4     m_global_transformation;

    VEC3     m_position;

    gVirtualXRay::SceneGraphNode* m_p_parent;

    std::vector<gVirtualXRay::SceneGraphNode*> m_p_children;

    bool m_world_properties_up_to_date;
};


} // namespace gVirtualXRay


#endif // GVXR_SCENEGRAPH_NODE_H
