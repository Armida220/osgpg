// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� POINTCLOUDPROCESSOR_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// POINTCLOUDPROCESSOR_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef POINTCLOUDPROCESSOR_EXPORTS
#define POINTCLOUDPROCESSOR_API __declspec(dllexport)
#else
#define POINTCLOUDPROCESSOR_API __declspec(dllimport)
#endif

#include "WorkFlowPluginBase.h"
#include "DragModeDialog.h"
#include "DragVertexModeDialog.h"
#include "LablePointModeDialog.h"
#include "PickPointCloudModeDialog.h"
#include "PickTriangleModeDialog.h"
#include "ViewModeDialog.h"
#include <osgGA/GUIEventHandler>
using namespace std;

namespace FC {
	class POINTCLOUDPROCESSOR_API PointCloudProcessor :
		public WorkFlowPluginBase{
	protected:
		osg::ref_ptr<osg::Node> sceneRoot;
		vector<osg::ref_ptr<osgGA::GUIEventHandler>> handlerArr;
		CPropertySheet *setter;
		ViewModeDialog* viewDlg;
		LablePointModeDialog* lableDlg;
		PickPointCloudModeDialog* pickPointDlg;
		PickTriangleModeDialog* pickTriangleDlg;
		DragModeDialog* dragDlg;
		DragVertexModeDialog* dragVertexDlg;
	public:
		virtual void SetSceneData(osgViewer::CompositeViewer* viewer);
		virtual void SetGUIEventHandlers(osgViewer::CompositeViewer* viewer);
		virtual void Release();
	};
}

extern "C" {
	POINTCLOUDPROCESSOR_API bool CreateWorkFlowPlugin(void ** pobj);
}