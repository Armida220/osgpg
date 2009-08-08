#include "StdAfx.h"
#include "PickPointCloudImpl.h"
#include "CloudStation.h"
#include "TriangleMesh.h"

PickPointCloudImpl::PickPointCloudImpl(ostream& _log) : ControlImpl(_log)
{
	this->sel=0;
}

bool PickPointCloudImpl::operator()(const osgGA::GUIEventAdapter& ea, 
																							osgGA::GUIActionAdapter& aa)
{
	osgViewer::View* viewer =
		dynamic_cast<osgViewer::View*>( &aa );
	if (!viewer)
		return false;

	switch(ea.getEventType())
	{
	case osgGA::GUIEventAdapter::RELEASE:

		this->sel=0;

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
			while (idx--)	{
				// 查找交集节点路径中的最后一个osgManipulator::Selection
				sel =	dynamic_cast<osgManipulator::Selection*>( nodePath[ idx ] );
				if (sel == NULL)
					continue;

				//TODO 点云全体高亮，应该去掉前一个的高亮
				CloudStation* cs = dynamic_cast<CloudStation*>(sel);
				if(cs==NULL)
					continue;
				cs->SetColor(osg::Vec4(0,1,1,0));
				
				return true;
			}
		}

		return true;
	}

	return true;
}

bool PickPointCloudImpl::SetColorPerVertex(osg::Vec4Array& colors)
{
	TriangleMesh* tm = dynamic_cast<TriangleMesh*>(sel);
	if(tm)
		return tm->SetColorPerVertex(colors);

	CloudStation* cs = dynamic_cast<CloudStation*>(sel);
	if(cs)
		return cs->SetColorPerVertex(colors);
	
	log<<"Error:  have not select any points!"<<endl;
	return false;
}

osg::ref_ptr<osg::Vec3Array> PickPointCloudImpl::GetTransformedPoints()
{
	TriangleMesh* tm = dynamic_cast<TriangleMesh*>(sel);
	if(tm)
		return tm->GetTransformedPointCloud();

	CloudStation* cs = dynamic_cast<CloudStation*>(sel);
	if(cs)
		return cs->GetTransformedPointCloud();

	log<<"Error:  have not select any points!"<<endl;
	return false;
}

double PickPointCloudImpl::GetTriangleArea()
{
	TriangleMesh* tm = dynamic_cast<TriangleMesh*>(sel);
	if(tm==0) {
		log<<"Error:  have not select any triangle mesh!"<<endl;
		return -3;
	}

	return tm->GetTriangleArea();
}

const vector<osg::DrawElementsUInt*>* PickPointCloudImpl::GetTriangleArray()
{
	TriangleMesh* tm = dynamic_cast<TriangleMesh*>(sel);
	if(tm==0) {
		log<<"Error:  have not select any triangle mesh!"<<endl;
		return 0;
	}

	return tm->GetTriangleArray();
}

osg::Vec3Array* PickPointCloudImpl::GetVertexArray()
{
	TriangleMesh* tm = dynamic_cast<TriangleMesh*>(sel);
	if(tm==0) {
		log<<"Error:  have not select any triangle mesh!"<<endl;
		return 0;
	}

	return tm->GetVertexArray();
}

bool PickPointCloudImpl::SetTexture(osg::Vec2Array& textureCoords, string textureName)
{
	TriangleMesh* tm = dynamic_cast<TriangleMesh*>(sel);
	if(tm==0) {
		log<<"Error:  have not select any triangle mesh!"<<endl;
		return 0;
	}

	return tm->SetTexture(textureCoords, textureName);
}