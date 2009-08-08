#include "StdAfx.h"
#include "LabelPointImpl.h"

LabelPointImpl::LabelPointImpl(ostream& _log) : ControlImpl(_log)
{
	this->curID=0;
}

bool LabelPointImpl::operator()(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa)
{
	osgViewer::View* viewer =
		dynamic_cast<osgViewer::View*>( &aa );
	if (!viewer)
		return false;

	static double x;
	static double y;

	switch(ea.getEventType())
	{
	case osgGA::GUIEventAdapter::PUSH:
		x = ea.getXnormalized();
		y = ea.getYnormalized();
		return true;

	case osgGA::GUIEventAdapter::RELEASE:

		double nx = ea.getXnormalized();
		double ny = ea.getYnormalized();

		if(!(x==nx && y==ny))
			return true;

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

				SignObject* sign = dynamic_cast<SignObject*>(sel->getChild(0));
				if(sign==NULL)
					continue;

				sign->AddOrEditSign(intersection.localIntersectionPoint, curID);
				sign->HighLightSign(curID);
				return true;
			}
		}

		return true;
	}

	return true;
}
