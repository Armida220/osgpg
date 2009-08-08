#include "StdAfx.h"
#include "PickTriangleImpl.h"

PickTriangleImpl::PickTriangleImpl(ostream& _log) : ControlImpl(_log)
{
}

bool PickTriangleImpl::operator()(const osgGA::GUIEventAdapter& ea,
																						osgGA::GUIActionAdapter& aa)
{
	osgViewer::View* viewer =
		dynamic_cast<osgViewer::View*>( &aa );
	if (!viewer)
		return false;

	switch(ea.getEventType())
	{
	case osgGA::GUIEventAdapter::RELEASE:

		double x = ea.getXnormalized();
		double y = ea.getYnormalized();

		osgUtil::PolytopeIntersector* picker =
			new osgUtil::PolytopeIntersector(
			osgUtil::Intersector::PROJECTION,
			x-0.01, y-0.01, x+0.01, y+0.01 );

		osgUtil::IntersectionVisitor iv( picker );
		viewer->getCamera()->accept( iv );

		if (picker->containsIntersections())
		{
			osgUtil::PolytopeIntersector::Intersection& intersection = 
				picker->getFirstIntersection();

			osg::NodePath& nodePath =
				intersection.nodePath;
			unsigned int idx = nodePath.size();
			osgManipulator::Selection* sel=0;
			while (idx--)	{
				// 查找交集节点路径中的最后一个osgManipulator::Selection
				sel =	dynamic_cast<osgManipulator::Selection*>( nodePath[ idx ] );
				if (sel == NULL)
					continue;
				if( strcmp(sel->className(), "TriangleMesh") != 0 )
					continue;

				osg::IntArray* iA = dynamic_cast<osg::IntArray*>(
					intersection.drawable->asGeometry()->getColorIndices() );

				if(iA)
					(*iA)[intersection.primitiveIndex] = !(*iA)[intersection.primitiveIndex];

				intersection.drawable->dirtyDisplayList();

				triangle.pickedPoint = intersection.localIntersectionPoint;
				osg::Vec3Array* vArr = dynamic_cast<osg::Vec3Array*>(
					intersection.drawable->asGeometry()->getVertexArray() );
				osg::DrawElementsUInt* drawIdx = dynamic_cast<osg::DrawElementsUInt*>(
					intersection.drawable->asGeometry()->getPrimitiveSet(
					intersection.primitiveIndex) );
				if(vArr && drawIdx && drawIdx->size()==3) {
					GLuint id0 = drawIdx->at(0);
					GLuint id1 = drawIdx->at(1);
					GLuint id2 = drawIdx->at(2);

					triangle.vertexs[0] = vArr->at(id0);
					triangle.vertexs[1] = vArr->at(id1);
					triangle.vertexs[2] = vArr->at(id2);
				}

				return true;
			}
		}

		return true;
	}

	return true;
}
