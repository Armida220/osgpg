#pragma once

// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� OSGVIEWERFRAMEWORK_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// OSGVIEWERFRAMEWORK_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
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