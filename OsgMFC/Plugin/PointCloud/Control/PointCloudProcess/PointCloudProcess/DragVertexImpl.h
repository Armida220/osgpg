#pragma once
#include "controller.h"

struct DragVertexInfo {
	osg::Vec3* Point2Change;
	osgUtil::PolytopeIntersector::Intersection intersection;
};

class DragVertexImpl :	public ControlImpl
{
public:
	DragVertexImpl(ostream& _log=cout);

	unsigned int GetDragVertexMode() const { return _dragMode; }
	void SetDragVertexMode(unsigned int m) { _dragMode = m; if(virtualSel.valid()&&m==Controller::FINISH) disconnect(); }

	//TODO 修改顶点之后，法线应该相应的修改
	bool operator()(const osgGA::GUIEventAdapter& ea, 
		osgGA::GUIActionAdapter& aa);

protected:
	void disconnect();
	bool connect(const osgGA::GUIEventAdapter& ea,
		osgGA::GUIActionAdapter& aa);
	bool drag(const osgGA::GUIEventAdapter& ea,
		osgGA::GUIActionAdapter& aa);

	//for drag vertex
	DragVertexInfo dragVertexInfo;
	osg::ref_ptr<osgManipulator::Selection> virtualSel;
	
	unsigned int _dragMode;
	osgManipulator::Dragger* _activeDragger;
	osgManipulator::PointerInfo _pointer;
	osg::ref_ptr<osgManipulator::Dragger> m_pointsDragger;
	osg::ref_ptr<osgManipulator::CommandManager> cmdMgr;
	osgManipulator::Selection* sel;
};
