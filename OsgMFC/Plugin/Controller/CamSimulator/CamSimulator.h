#ifdef CAMSIMULATOR_EXPORTS
#define CAMSIMULATOR_API __declspec(dllexport)
#else
#define CAMSIMULATOR_API __declspec(dllimport)
#endif

#include "WorkFlowPluginBase.h"

namespace FC {
	class CAMSIMULATOR_API CamSimulator :
		public WorkFlowPluginBase{
	protected:
		osg::ref_ptr<osg::Node> sceneRoot;
	public:
		virtual void SetSceneData(osgViewer::CompositeViewer* viewer);
		virtual ViewerFrameworkImpl* CreateViewerFrameworkImpl();
		virtual void Release();
	};

	class CAMSIMULATOR_API PhotoViewerImpl : public ViewerFrameworkImpl
	{
	public:
		int w, h;
		//init
		virtual void SetupCamera(HWND hwnd, RECT rect);
		virtual void Resize(RECT rect);
	};
}

extern "C" {
	CAMSIMULATOR_API bool CreateWorkFlowPlugin(void ** pobj);
}