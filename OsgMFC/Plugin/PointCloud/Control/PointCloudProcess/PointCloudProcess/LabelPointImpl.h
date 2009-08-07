#pragma once
#include "Controller.h"
#include <osgGA/GUIEventHandler>

class LabelPointImpl : public ControlImpl
{
public:
	LabelPointImpl(ostream& _log=cout);

	unsigned int GetCurID() const { return curID; }
	void SetCurID(unsigned int id) { curID = id; }

	bool operator()(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);

protected:
	//for label point
	unsigned int curID;
};
