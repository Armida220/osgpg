// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 IVE2OFF_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// IVE2OFF_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef IVE2OFF_EXPORTS
#define IVE2OFF_API __declspec(dllexport)
#else
#define IVE2OFF_API __declspec(dllimport)
#endif

#include "WorkFlowPluginBase.h"
using namespace std;

namespace FC {
	class IVE2OFF_API Ive2off :
		public WorkFlowPluginBase{
	protected:
		osg::ref_ptr<osg::Node> sceneRoot;
	public:
		virtual osg::Node* CreateSceneData();
		virtual void Release();
	};

}

extern "C" {
	IVE2OFF_API bool CreateWorkFlowPlugin(void ** pobj);
}