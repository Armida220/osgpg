#pragma once

#include "pointRecord.h"
#include "PointsDividor.h"
#include <osg/BoundingBox>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <list>
using namespace std;

namespace FC {

	//forward declare
	class PointsDividor;

	struct BuildParameters	{
		BuildParameters(unsigned int maxLevel=32, 
			unsigned int targetPointsNumOnLeaf=20, float sampleRatio=0.01)
		{
			_maxLevels = maxLevel;
			_targetPointsNumOnLeaf = targetPointsNumOnLeaf;
			_sampleRatio = sampleRatio;
		}
		unsigned int	_maxLevels;
		unsigned int	_targetPointsNumOnLeaf;
		float					_sampleRatio;
	};

	class KdTree
	{
	public:
		KdTree();
		~KdTree(void);

		bool Build(PointsDividor* pd);

		BuildParameters _bp;
	protected:
		//recursive
		void BuildKdTree(PointIdxSet& pis, 
			osg::BoundingBox bb, 
			unsigned int level,
			osg::ref_ptr<osg::LOD> root);

		vector<PointRec>* _points;

		PointsDividor* _pd;
	private:
		inline int findMaxAxis(osg::BoundingBox bb);
	};
}