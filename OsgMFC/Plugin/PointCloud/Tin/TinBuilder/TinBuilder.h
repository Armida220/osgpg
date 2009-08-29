#ifdef TINBUILDER_EXPORTS
#define TINBUILDER_API __declspec(dllexport)
#else
#define TINBUILDER_API __declspec(dllimport)
#endif

#include "WorkFlowPluginBase.h"
#include <osg/Vec4>
#include <vector>
using namespace std;

namespace FC {

	class TINBUILDER_API TinBuilder : 
		public WorkFlowPluginBase {
	protected:
		osg::ref_ptr<osg::Node> sceneRoot;

	public:
		virtual void SetSceneData(osgViewer::CompositeViewer* viewer);
		virtual void Release();
	};

}

extern "C" {
	TINBUILDER_API bool CreateWorkFlowPlugin(void ** pobj);
}