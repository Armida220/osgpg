#include <osg/ref_ptr>
#include <osg/Group>
#include <osgViewer/Viewer>
#include <osg/NodeVisitor>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <iostream>
#include "HumanController.h"

//////////////////////////////////////////////////////////////////////////

bool CreateControllerPlugin(void **pobj)
{
	*pobj = new FC::HumanControllerCreator;
	return *pobj != NULL;
}

namespace FC {
	//////////////////////////////////////////////////////////////////////////
	void HumanControllerCreator::Release()
	{
		delete this;
	}

	Controller* HumanControllerCreator::CreateController()
	{
		if(!this->_controller.valid())
			_controller = new HumanController;
		return _controller.get();
	}

	//////////////////////////////////////////////////////////////////////////
	bool HumanController::handle(const osgGA::GUIEventAdapter& ea, 
		osgGA::GUIActionAdapter& aa)
	{
		switch(ea.getEventType())
		{
			case(osgGA::GUIEventAdapter::KEYDOWN):
				osg::Vec3 delta = _left^_up;
				delta.normalize();
				delta *= _velocity;

				if (ea.getKey()==osgGA::GUIEventAdapter::KEY_Up) {
					_eye += delta;
				}
				if (ea.getKey()==osgGA::GUIEventAdapter::KEY_Down) {
					_eye -= delta;
				}
				if (ea.getKey()==osgGA::GUIEventAdapter::KEY_Left) {
					_eye += _left*_velocity;
				}
				if (ea.getKey()==osgGA::GUIEventAdapter::KEY_Right) {
					_eye -= _left*_velocity;
				}
				return true;
		}
		return false;
	}

	void HumanController::setByMatrix(const osg::Matrixd& matrix)
	{
		_eye = matrix.getTrans();
	}

	void HumanController::setByInverseMatrix(const osg::Matrixd& matrix)
	{
		_eye = -matrix.getTrans();
	}

	osg::Matrixd HumanController::getMatrix() const
	{
		return osg::Matrixd::translate(_eye);
	}

	osg::Matrixd HumanController::getInverseMatrix() const
	{
		return osg::Matrixd::translate(-_eye);
	}
}