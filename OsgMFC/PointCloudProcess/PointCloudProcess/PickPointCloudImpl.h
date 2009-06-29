#pragma once
#include "controller.h"

class PickPointCloudImpl :	public ControlImpl
{
public:
	PickPointCloudImpl(ostream& _log=cout);

	bool SetColorPerVertex(osg::Vec4Array& colors);
	osg::ref_ptr<osg::Vec3Array> GetTransformedPoints();

	double GetTriangleArea();

	osg::Vec3Array* GetVertexArray();//not transformed
	const vector<osg::DrawElementsUInt*>* GetTriangleArray();

	bool SetTexture(osg::Vec2Array& textureCoords, string textureName);

	bool operator()(const osgGA::GUIEventAdapter& ea, 
		osgGA::GUIActionAdapter& aa);

protected:
	osgManipulator::Selection* sel;
};
