/********************************************************************
filename: 	Project\PointCloud\PointCloudProcess\CloudStation.h
file path:	Project\PointCloud\PointCloudProcess
file base:	CloudStation

purpose:	扫描站对象
*********************************************************************/

#pragma once

#include "export.h"

#include <osg/Geometry>
#include <osgManipulator/CommandManager>
#include <osgManipulator/TabBoxDragger>
#include <osgManipulator/TabPlaneDragger>
#include <osgManipulator/TabPlaneTrackballDragger>
#include <osgManipulator/TrackballDragger>
#include <osgManipulator/Translate1DDragger>
#include <osgManipulator/Translate2DDragger>
#include <osgManipulator/TranslateAxisDragger>

#include "SignObject.h"

class POINTCLOUDPROCESS_API CloudStation : public osgManipulator::Selection
{
	//构造函数和析构函数
public:
	CloudStation(void);

	virtual const char* className() const { return "CloudStation"; }

public:
	osg::Vec4    GetColor() const { return m_color; }
	void         SetColor(osg::Vec4 val) { m_color = val; SetColorToCloudStation(); }

	SignObject* GetSignObject() const { return m_signObj.get(); }
	void SetSignObject(SignObject* sign) { m_signObj = sign; }

	virtual osg::ref_ptr<osg::Vec3Array> GetTransformedPointCloud();
	osg::ref_ptr<osg::Vec3Array> GetSignedPoints();

	virtual bool SetColorPerVertex(osg::Vec4Array& colors);

private:
	osg::ref_ptr<SignObject> m_signObj;

	//点云颜色
	osg::Vec4 m_color;

	void SetColorToCloudStation();
};

/*
*	统一修改点云颜色的访问器 TODO 修改颜色太慢，应该使用attribute
*/
class RemapColorVisitor : public osg::NodeVisitor
{
public:
	RemapColorVisitor(osg::Vec4 color) : NodeVisitor(NodeVisitor::TRAVERSE_ALL_CHILDREN),c(color) {}
	virtual void apply(osg::Geode& geode)
	{
		osg::ref_ptr<osg::Vec4Array> colorArray = new osg::Vec4Array;
		colorArray->push_back(c);
		for (unsigned i=0; i<geode.getNumDrawables(); ++i)
		{
			osg::Geometry *geo = dynamic_cast<osg::Geometry *>(geode.getDrawable(i));
			if (!geo) continue;

			geo->setColorArray(colorArray.get());
			geo->setColorBinding(osg::Geometry::BIND_OVERALL);

			/*FC 此段无法更改颜色，注释之
			osg::StateSet *stateset = geo->getStateSet();
			if (!stateset) continue;

			osg::StateAttribute *state = stateset->getAttribute(osg::StateAttribute::MATERIAL);
			if (!state) continue;

			osg::Material *mat = dynamic_cast<osg::Material *>(state);
			if (!mat) continue;
			osg::Material *newmat = (osg::Material *)mat->clone(osg::CopyOp::DEEP_COPY_ALL);
			newmat->setDiffuse(osg::Material::FRONT_AND_BACK, c);
			newmat->setAmbient(osg::Material::FRONT_AND_BACK, c);
			stateset->setAttribute(newmat);*/
		}
		traverse(geode);
	}
	osg::Vec4 c;
};
