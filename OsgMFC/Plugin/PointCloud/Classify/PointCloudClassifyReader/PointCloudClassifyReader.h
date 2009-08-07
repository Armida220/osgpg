#ifdef POINTCLOUDCLASSIFYREADER_EXPORTS
#define POINTCLOUDCLASSIFYREADER_API __declspec(dllexport)
#else
#define POINTCLOUDCLASSIFYREADER_API __declspec(dllimport)
#endif

#include "WorkFlowPluginBase.h"
#include <osg/Vec4>
#include <vector>
using namespace std;

namespace FC {

#define WHITE_COLOR (osg::Vec4(0,0,0,0))
#define ORANGE_COLOR (osg::Vec4(216.0/255, 203.0/255, 41.0/255, 0))
#define DEEP_GREEN_COLOR (osg::Vec4(0, 128.0/255, 0, 0))
#define LIGHT_GREEN_COLOR (osg::Vec4(0, 1, 0, 0))
#define RED_COLOR (osg::Vec4(1, 0, 0, 0))

	struct PointRec {
		int		k;
		float	v[3];
	};

	enum PointType {
		WHITE=1,
		ORANGE=2,
		DEEP_GREEN=3,
		LIGHT_GREEN=5,
		RED=6
	};

	typedef vector<PointRec> PointRecArr;

	class POINTCLOUDCLASSIFYREADER_API PointCloudClassifyReader : 
		public WorkFlowPluginBase {
	protected:
		bool ReadPointDataFile(PointRecArr& pointArr);
		osg::Geode* PointArr2Geode(PointRecArr& pointArr);

		osg::ref_ptr<osg::Node> sceneRoot;

	public:
		virtual osg::Node* CreateSceneData();
		virtual void Release();
	};
}

extern "C" {
	POINTCLOUDCLASSIFYREADER_API bool CreateWorkFlowPlugin(void ** pobj);
}