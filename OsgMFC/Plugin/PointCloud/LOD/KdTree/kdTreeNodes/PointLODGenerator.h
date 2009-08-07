#pragma once

#include "pointRecord.h"
#include "LODGenStrategy.h"
#include <osg/ref_ptr>
#include <osg/lod>
#include <vector>
using namespace std;

namespace FC {
	class PointLODGenerator
	{
	public:
		PointLODGenerator(void);
		~PointLODGenerator(void);

		bool Generate(PointSet& pointSet, 
			osg::ref_ptr<osg::LOD> root, BuildParameters bp);
	protected:
		LODGenStrategy *_genStrategy;
	};
}
