
#ifdef FISHEYEIMAGEVR_EXPORTS
#define FISHEYEIMAGEVR_API __declspec(dllexport)
#else
#define FISHEYEIMAGEVR_API __declspec(dllimport)
#endif

#include "WorkFlowPluginBase.h"
#include <osg/Vec4>
#include <vector>
using namespace std;

namespace FC {

	class FISHEYEIMAGEVR_API FishEyeImageVR :
		public WorkFlowPluginBase {
	protected:
		osg::ref_ptr<osg::Node> sceneRoot;

	public:
		virtual void SetSceneData(osgViewer::CompositeViewer* viewer);
		virtual void Release();
	};

}

extern "C" {
	FISHEYEIMAGEVR_API bool CreateWorkFlowPlugin(void ** pobj);
}