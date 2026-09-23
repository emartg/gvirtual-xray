//******************************************************************************
//  Include
//******************************************************************************

#ifdef HAS_OPENSCENEGRAPH
#include <osg/Transform>
#endif

#ifndef GVXR_GEODE_FINDER_H
#include "gVirtualXRay/GeodeFinder.h"
#endif


#ifdef HAS_OPENSCENEGRAPH


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;


//******************************************************************************
//  Implementation
//******************************************************************************


GeodeFinder::GeodeFinder ()
   : NodeVisitor (osg::NodeVisitor::TRAVERSE_ALL_CHILDREN) {}


GeodeFinder::GeodeFinder (osg::Node &searchNode)
   : NodeVisitor (osg::NodeVisitor::TRAVERSE_ALL_CHILDREN) {

	apply(searchNode);
}

void GeodeFinder::apply (osg::Node &searchNode) {
   if (! strcmp (searchNode.className(), "Geode"))
   {
      m_found_geode_set.push_back ((osg::Geode*) &searchNode);


      //nodePathes.push_back(searchNode.getNodePath());
   }

   traverse (searchNode);
}

osg::Geode* GeodeFinder::getFirst () {
   if (m_found_geode_set.size() > 0)
      return m_found_geode_set.at(0);
   else
      return NULL;
}

std::vector<osg::Geode*>& GeodeFinder::getNodeList() {
   return m_found_geode_set;
}

#endif
