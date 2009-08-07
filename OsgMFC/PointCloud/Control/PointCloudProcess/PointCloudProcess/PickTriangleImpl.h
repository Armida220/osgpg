#pragma once
#include "controller.h"

class PickTriangleImpl :	public ControlImpl
{
public:
	PickTriangleImpl(ostream& _log=cout);

	const Triangle& GetTriangle() const { return triangle; }

	bool operator()(const osgGA::GUIEventAdapter& ea, 
		osgGA::GUIActionAdapter& aa);

protected:
	//for select triangle
	Triangle triangle;
};
