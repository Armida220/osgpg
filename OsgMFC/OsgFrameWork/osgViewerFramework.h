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
#include <osgViewer/Viewer>
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

		void AddEventHandler(osgGA::GUIEventHandler* handler);
		osgViewer::Viewer* GetViewer();

		bool Load(std::string path);
		void Load(osg::Node* node);

		osg::Group* GetRoot();

		void Run();
		void Stop();

		void ReleaseImpl();

	protected:
		ViewerFrameworkImpl* m_impl;
	};

}