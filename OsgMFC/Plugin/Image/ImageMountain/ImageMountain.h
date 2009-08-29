#ifdef IMAGEMOUNTAIN_EXPORTS
#define IMAGEMOUNTAIN_API __declspec(dllexport)
#else
#define IMAGEMOUNTAIN_API __declspec(dllimport)
#endif

#include "WorkFlowPluginBase.h"
#include <osg/Vec4>
#include <vector>
using namespace std;

namespace FC {

	class IMAGEMOUNTAIN_API ImageMountain:
		public WorkFlowPluginBase {
	protected:
		osg::ref_ptr<osg::Node> sceneRoot;

	public:
		virtual void SetSceneData(osgViewer::CompositeViewer* viewer);
		virtual void Release();
	};

}

extern "C" {
	IMAGEMOUNTAIN_API bool CreateWorkFlowPlugin(void ** pobj);
}