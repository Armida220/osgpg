/********************************************************************
filename: 	Project\PointCloud\PointCloudProcess\Controller.h
file path:	Project\PointCloud\PointCloudProcess
file base:	Controller

purpose:	控制器实现对控制目标在鼠标和键盘事件发生时的操作，完成
与屏幕交互的动作。
*********************************************************************/
#pragma once
#include "export.h"

#include <iostream>
#include <fstream>

#include <osgGA/GUIEventHandler>

#include <osgUtil/Optimizer>
#include <osgViewer/Viewer>
#include <osgManipulator/CommandManager>
#include <osgManipulator/TabBoxDragger>
#include <osgManipulator/TabPlaneDragger>
#include <osgManipulator/TabPlaneTrackballDragger>
#include <osgManipulator/TrackballDragger>
#include <osgManipulator/Translate1DDragger>
#include <osgManipulator/Translate2DDragger>
#include <osgManipulator/TranslateAxisDragger>
#include <osgManipulator/Command>


#include "SignObject.h"

using namespace std;

class POINTCLOUDPROCESS_API ControlImpl : public osg::Referenced
{
public:
	ControlImpl(ostream& _log=cout) : log(_log) {};
	virtual bool operator()(const osgGA::GUIEventAdapter& ea, 
		osgGA::GUIActionAdapter& aa)	{ return false; }
protected:
	ostream& log;
};

struct POINTCLOUDPROCESS_API Triangle {
	osg::Vec3 vertexs[3];
	osg::Vec3 pickedPoint;
};

class POINTCLOUDPROCESS_API Controller : public osgGA::GUIEventHandler
{
public:
	enum Modes
	{
		VIEW = 0,
		LABEL_POINT,
		PICK_POINT_CLOUD,
		PICK_TRIANGLE,
		DRAG_VERTEX,
		DRAG
	};

	enum DRAG_Modes
	{
		BEGIN = 0,
		FINISH
	};

	Controller(ostream& _log=cout);

	unsigned int GetMode() const { return _mode; }
	void SetMode(unsigned int m) { _mode = m; }

	virtual bool handle( const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa );

public://TODO should use one abstract function to execute
	//label impl
	unsigned int GetCurID();
	void SetCurID(unsigned int id);

	//pick point cloud impl
	bool SetColorPerVertex(osg::Vec4Array& colors);
	osg::ref_ptr<osg::Vec3Array> GetTransformedPoints();
	double GetTriangleArea();
	osg::Vec3Array* GetVertexArray();
	const vector<osg::DrawElementsUInt*>* GetTriangleArray();
	bool SetTexture(osg::Vec2Array& textureCoords, string textureName);
	osg::ref_ptr<osg::Vec3Array> GetSignedPoints();

	//pick triangle impl
	const Triangle& GetTriangle();

	//drag impl
	unsigned int GetDragMode();
	void SetDragMode(unsigned int m);
	void undo();
	void redo();

	//drag vertex impl
	unsigned int GetDragVertexMode();
	void SetDragVertexMode(unsigned int m);

protected:
	ostream& log;
	unsigned int _mode;
	vector<osg::ref_ptr<ControlImpl>> _ctrlImplList;
};

class MyCommandManager : public osgManipulator::CommandManager
{
protected:
	typedef vector<osg::ref_ptr<osgManipulator::MotionCommand>> CMD_GROUP;
	vector<CMD_GROUP> cmdStack;
	unsigned int curCmd;
public:
	MyCommandManager()
	{
		reset();
	}

	void reset()
	{
		cmdStack.clear();
		curCmd = 0;
	}

	virtual void dispatch(osgManipulator::MotionCommand& command)
	{
		command.execute();
		CMD_GROUP cmd;
		switch(command.getStage())
		{
		case	osgManipulator::MotionCommand::START:
			cmd.clear();
			while(curCmd!=cmdStack.size()) {
					cmdStack.pop_back();
			}
			cmdStack.push_back(cmd);
		case osgManipulator::MotionCommand::MOVE:
		case osgManipulator::MotionCommand::FINISH:
			cmdStack[cmdStack.size()-1].push_back(&command);
			break;
		}
		curCmd = cmdStack.size();
	}

	virtual bool disconnect(osgManipulator::Dragger& dragger)
	{
		cmdStack.clear();
		curCmd=0;
		return __super::disconnect(dragger);
	}

	virtual bool undo()
	{
#ifdef _DEBUG
		cout<<"before->"<<endl;
		cout<<"cmdStack size: "<<cmdStack.size()<<endl;
		cout<<"curCmd: "<<curCmd<<endl;
#endif
		bool ret = false;
		if(curCmd>0 && curCmd<=cmdStack.size()) {
			CMD_GROUP& cmd = cmdStack[curCmd-1];
			for(int i=cmd.size()-1; i>=0; --i) {
				cmd[i]->unexecute();
			}
			curCmd--;
			ret = true;
		}
#ifdef _DEBUG
		cout<<"after->"<<endl;
		cout<<"cmdStack size: "<<cmdStack.size()<<endl;
		cout<<"curCmd: "<<curCmd<<endl;
#endif
		return ret;
	}

	virtual bool redo()
	{
#ifdef _DEBUG
		cout<<"before->"<<endl;
		cout<<"cmdStack size: "<<cmdStack.size()<<endl;
		cout<<"curCmd: "<<curCmd<<endl;
#endif
		bool ret = false;
		if(curCmd<cmdStack.size() && curCmd>=0) {
			++curCmd;
			CMD_GROUP& cmd = cmdStack[curCmd-1];
			for(unsigned int i=0; i<cmd.size(); ++i) {
				cmd[i]->execute();
			}
			ret = true;
		}
#ifdef _DEBUG
		cout<<"after->"<<endl;
		cout<<"cmdStack size: "<<cmdStack.size()<<endl;
		cout<<"curCmd: "<<curCmd<<endl;
#endif
		return ret;
	}
};