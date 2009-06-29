#pragma once

#include <osg/Referenced>
#include <osg/PointSprite>
#include <osg/BlendFunc>
#include <osg/StateAttribute>
#include <osg/Point>
#include <osg/Geometry>
#include <osg/GLExtensions>
#include <osg/ref_ptr>
#include <osg/Group>
#include <osg/Geode>
#include <osg/Drawable>
#include <osg/BoundingBox>
#include <osg/lod>

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
using namespace std;

#include "pointRecord.h"
#include "KdTree.h"

namespace FC {
	class PointsDividor
	{
	public:
		enum FileType {
			XYZ=0,
			XYZI,
			XYZRGB,
			XYZIRGB
		};

	protected:
		fstream&			infile;
		int					_fileType;

		//store raw data
		vector<PointRec>			_points;
		osg::BoundingBox			_bb;

		//build parameter
		unsigned int _maxLevels;
		float				 _sampleRatio;
		unsigned int _targetPointsNumOnLeaf;

		osg::ref_ptr<osg::LOD>		_root;

		std::ofstream					_log;

	public:
		PointsDividor(fstream& in, string logname):
				infile(in), _log(logname.c_str())
				{
					setlocale(LC_ALL, "chs");
					_points.clear();
				}

	public:
		//possible max level limited to 32
		inline void SetMaxLevels(unsigned int val) { _maxLevels = val<32?val:32; }
		inline void SetTargetPointsNumOnLeaf(unsigned int val) { _targetPointsNumOnLeaf = val; }
		inline void SetSampleRatio(float val) { _sampleRatio = val; }
		inline unsigned int GetMaxLevels() { return _maxLevels; }
		inline float GetSampleRatio() { return _sampleRatio; }
		inline unsigned int GetTargetPointsNumOnLeaf() { return _targetPointsNumOnLeaf; } 

		inline osg::ref_ptr<osg::LOD> GetRoot() { return _root; }

		bool process(unsigned int maxLevels=32,
			float sampleRatio=0.1, unsigned int targetPointsNumOnLeaf=100);

		osg::Geode* makePointsInOneNode(PointIdxSet& pis, osg::BoundingBox bb);

		inline vector<PointRec>& GetRawPoints() { return _points; }
	protected:

		bool read_point();
	};
}

