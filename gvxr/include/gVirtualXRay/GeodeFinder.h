#ifndef GVXR_GEODE_FINDER_H
#define GVXR_GEODE_FINDER_H


#ifdef HAS_OPENSCENEGRAPH
#include <osg/NodeVisitor>
#include <osg/Transform>
#include <osg/Node>
#include <osg/Geode>


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


class GeodeFinder : public osg::NodeVisitor {
   public:

   // Constructor - sets the traversal mode to TRAVERSE_ALL_CHILDREN
   // and Visitor type to NODE_VISITOR
   GeodeFinder();


   // Constructor - sets the traversal mode to TRAVERSE_ALL_CHILDREN
   // and Visitor type to NODE_VISITOR
   // See if a className() call to searchNode returns "Geode."
   // If so, add this node to our list.
   GeodeFinder(osg::Node &searchNode);

   // The 'apply' method for 'node' type instances.
   // See if a className() call to searchNode returns "Geode."
   // If so, add this node to our list.
   void apply(osg::Node &searchNode);

   // Return a pointer to the first node in the list
   // with a matching name
   osg::Geode* getFirst();

   // return a the list of nodes we found
   std::vector<osg::Geode*>& getNodeList();

private:
   // List of nodes with names that match the searchForName string
   std::vector<osg::Geode*> m_found_geode_set;
   std::vector<osg::NodePath*> m_found_geode_path_set;
};


} // namespace gVirtualXRay

#endif


#endif // GVXR_GEODE_FINDER_H
