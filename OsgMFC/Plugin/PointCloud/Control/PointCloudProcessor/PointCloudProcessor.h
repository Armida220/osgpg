// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 POINTCLOUDPROCESSOR_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// POINTCLOUDPROCESSOR_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
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