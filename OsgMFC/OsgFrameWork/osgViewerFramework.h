#pragma once

// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 OSGVIEWERFRAMEWORK_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// OSGVIEWERFRAMEWORK_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef OSGVIEWERFRAMEWORK_EXPORTS
#define OSGVIEWERFRAMEWORK_API __declspec(dllexport)
#else
#define OSGVIEWERFRAMEWORK_API __declspec(dllimport)
#endif

#include <osg/ref_ptr>
#include <osg/Group>
#include <osgViewer/CompositeViewer>
#include <osg/NodeVisitor>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgGA/GUIEventHandler>
#include <osgGA/StateSetManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/api/win32/GraphicsWindowWin32>
#include <osgGA/TrackballManipulator>
#include <osgGA/TerrainManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgDB/DatabasePager>
#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>
#include <iostream>

#include <fstream>
#include <process.h>
#include <memory>
#include <string>

namespace FC {

	class ViewerFrameworkImpl;

	class ViewerFramework
	{
	public:
		ViewerFramework(void);
		~ViewerFramework(void);

		void SetupWindow(HWND hwnd, RECT rect);
		void Resize(RECT rect);

		//add eventhandler for view i 
		void AddEventHandler(osgGA::GUIEventHandler* handler, int i=0);
		osgViewer::CompositeViewer* GetViewer();

		//add model for to view i 's root
		bool AddModel(std::string path, int i=0);
		void AddModel(osg::Node* node, int i=0);

		void InitSceneRoot();
		//get view i 's root
		osg::Node* GetRoot(int i=0);

		void Run();
		void Stop();

		void ResetImpl(ViewerFrameworkImpl* newImpl);
		void ReleaseImpl();

	protected:
		ViewerFrameworkImpl* m_impl;
	};

}