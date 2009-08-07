#include "PointLODGenerator.h"

using namespace FC;

PointLODGenerator::PointLODGenerator(void)
{
	_genStrategy = 0;
}

PointLODGenerator::~PointLODGenerator(void)
{
	if(_genStrategy)
		delete _genStrategy;
}

bool PointLODGenerator::Generate(PointSet &pointSet, 
																 osg::ref_ptr<osg::LOD> root, BuildParameters bp)
{
	if(bp._genType == BuildParameters::TRADITION_KDTREE) {
		_genStrategy = new TraditionKdTreeGenStrategy;
	} else {
		_genStrategy = new SampleKdTreeGenStrategy;
	}

	return _genStrategy->Generate(pointSet, root, bp);
}