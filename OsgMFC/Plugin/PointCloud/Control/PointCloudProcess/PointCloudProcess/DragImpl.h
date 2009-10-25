#pragma once
#include "controller.h"

class DragImpl :	public ControlImpl
{
public:
	DragImpl(ostream& _log=cout);

	unsigned int GetDragMode() const { return _dragMode; }
	void SetDragMode(unsigned int m) { _dragMode = m; if(sel&&m==Controller::FINISH) disconnect(); }

	virtual bool undo() { return cmdMgr->undo(); }
	virtual bool redo() { return cmdMgr->redo(); }

	bool operator()(const osgGA::GUIEventAdapter& ea, 
		osgGA::GUIActionAdapter& aa);

protected:
	void disconnect();
	bool connect(const osgGA::GUIEventAdapter& ea,
		osgGA::GUIActionAdapter& aa);
	bool drag(const osgGA::GUIEventAdapter& ea,
		osgGA::GUIActionAdapter& aa);

	unsigned int _dragMode;
	osgManipulator::Dragger* _activeDragger;
	osgManipulator::PointerInfo _pointer;
	osg::ref_ptr<osgManipulator::Dragger> m_pointsDragger;
	osg::ref_ptr<MyCommandManager> cmdMgr;
	osgManipulator::Selection* sel;
};


class RotateAndTranslate3DDragger : public osgManipulator::CompositeDragger
{
public:
	RotateAndTranslate3DDragger() 
	{
		td = new osgManipulator::TrackballDragger;
		tad = new osgManipulator::TranslateAxisDragger;

		addChild(td.get());
		this->addDragger(td.get());
		addChild(tad.get());
		this->addDragger(tad.get());

		setParentDragger(getParentDragger());
	}

	void setMatrix(float scale, osg::Matrix& trans)
	{
		td->setMatrix(osg::Matrix::scale(scale, scale, scale)*trans);
		scale *= 1.5;
		tad->setMatrix(osg::Matrix::scale(scale, scale, scale) * trans);
	}

	void setupDefaultGeometry()
	{
		td->setupDefaultGeometry();
		tad->setupDefaultGeometry();
	}
protected:
	osg::ref_ptr<osgManipulator::TrackballDragger> td;
	osg::ref_ptr<osgManipulator::TranslateAxisDragger> tad;
};

class CommDragImpl :	public ControlImpl
{
public:
    CommDragImpl(ostream& _log=cout);

    void stop() { disconnect(); }

    //virtual bool undo() { return cmdMgr->undo(); }
    //virtual bool redo() { return cmdMgr->redo(); }

    bool drag(const osgGA::GUIEventAdapter& ea,
        osgGA::GUIActionAdapter& aa);

    osg::Group* createdrag(osg::Node* scene);

    osg::Matrix getTransformMatrix() { return _transformmatrix; }

protected:
    void disconnect();

    osgManipulator::Dragger* _activeDragger;
    osgManipulator::PointerInfo _pointer;
    osg::ref_ptr<osgManipulator::TranslateAxisDragger> m_pointsDragger;
    //osg::ref_ptr<MyCommandManager> cmdMgr;
    osg::ref_ptr<osgManipulator::CommandManager> cmdMgr;
    osgManipulator::Selection* sel;

    bool _isconfgureddrag;
    osg::Matrix _transformmatrix;
};