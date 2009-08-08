// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 VIEWERFRAMEWORKIMPL_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// VIEWERFRAMEWORKIMPL_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef VIEWERFRAMEWORKIMPL_EXPORTS
#define VIEWERFRAMEWORKIMPL_API __declspec(dllexport)
#else
#define VIEWERFRAMEWORKIMPL_API __declspec(dllimport)
#endif


#include "stdafx.h"

#include <osg/ref_ptr>
#include <osg/Group>
#include <osgViewer/CompositeViewer>
#include <iostream>

#include <fstream>
#include <process.h>
#include <memory>
#include <string>

using namespace std;
using namespace osg;
using namespace osgViewer;

namespace FC {
	//default implementation of ViewerFramework
	//scene structure :
	//				Root0              -> for view 0
	//        /  \  \  
	//    Root1 Root2 Nodes...   -> for view 1, view 2, ...
	class VIEWERFRAMEWORKIMPL_API ViewerFrameworkImpl
	{
	public:
		osg::ref_ptr<osgViewer::CompositeViewer>		Viewer;
		osg::ref_ptr<osg::GraphicsContext::Traits>	Traits;
		osg::ref_ptr<osg::GraphicsContext>					Gc;
		osg::ref_ptr<osg::Group>										Root;
		HANDLE																			Thread;

		bool Closed_yet;

	public:
		explicit ViewerFrameworkImpl(void);
		~ViewerFrameworkImpl();

		enum { SLEEP_TIME = 1000 };

		//add model for view i 's root
		bool AddModel(osg::Node* node, int i=0);
		//get view i 's root
		osg::Node* GetRoot(int i=0);
		//get or create view i 's root
		//and make sure it is attached to view i
		osg::Node* GetOrCreateRoot(int i=0);

		//init
		virtual void SetupCamera(HWND hwnd, RECT rect);
		virtual void SetupManipulatorAndEventHandler();

		virtual void Resize(RECT rect);

		//create and relate roots with views
		void InitSceneRoot();

		//setup scene data and begin the DoRendering thread
		virtual void StartRendering();
		static  void DoRendering(void* data);
		virtual void StopRendering();

		//delete self
		virtual void Release();
	};
}