#pragma once

#include "pointRecord.h"
#include "PointSetDividor.h"
#include <vector>
#include <osg/ref_ptr>
#include <osg/lod>
#include <osg/BoundingBox>

namespace FC {
	class LODGenStrategy
	{
	public:
		virtual bool Generate(PointSet& pointSet, 
			osg::ref_ptr<osg::LOD> root, BuildParameters bp) = 0;
	protected:
		std::ofstream _log;
	};

	class TraditionKdTreeGenStrategy : public LODGenStrategy
	{
	public:
		TraditionKdTreeGenStrategy() { _log.open("TraditionKdTreeGenStrategy.log"); }
		~TraditionKdTreeGenStrategy() { _log.close(); }

		virtual bool Generate(PointSet& pointSet, 
			osg::ref_ptr<osg::LOD> root, BuildParameters bp);
	};

	class SampleKdTreeGenStrategy : public LODGenStrategy
	{
	public:
		SampleKdTreeGenStrategy() { _log.open("SampleKdTreeGenStrategy.log"); }
		~SampleKdTreeGenStrategy() { _log.close(); }

		bool Generate(PointSet& pointSet, 
			osg::ref_ptr<osg::LOD> root, BuildParameters bp);

	protected:
		void BuildKdTree(PointIdxSet& pis, osg::BoundingBox bb, 
			unsigned int level, osg::ref_ptr<osg::LOD> root);

		BuildParameters _bp;
		PointSet* _points;

		double _maxBoundRadius;
		double _minBoundRadius;
	};
}
