#pragma once

#include <osg/ref_ptr>
#include <osg/Group>
#include <osg/lod>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

#include "pointRecord.h"

#define MEM_MAP_THRESHOLD 5000000

namespace FC {

	struct BuildParameters	{
		enum GenType {
			TRADITION_KDTREE=0,
			SAMPLE_KDTREE
		};
		BuildParameters(unsigned int maxLevel=32, 
			unsigned int targetPointsNumOnLeaf=20, 
			float sampleRatio=0.01, unsigned int genType = SAMPLE_KDTREE,
			unsigned int recordType = XYZ)
		{
			_maxLevels = maxLevel;
			_targetPointsNumOnLeaf = targetPointsNumOnLeaf;
			_sampleRatio = sampleRatio;
			_genType = genType;
			_recordType = recordType;
		}
		unsigned int	_maxLevels;
		unsigned int	_targetPointsNumOnLeaf;
		float					_sampleRatio;
		unsigned int	_genType;
		unsigned int	_recordType;
	};

	class PointSetDividor
	{
	protected:
		string _dataFileName;
		string _saveFileName;
		PointSet _pointSet;
		osg::ref_ptr<osg::Group> _root;
		std::ofstream _log;
	public:
		PointSetDividor(string dataFileName, string saveFileName) : _dataFileName(dataFileName), 
			_saveFileName(saveFileName), _pointSet(0), _log("PointSetDividor.log") {}
		void SetDataFileName(string dataFileName) { _dataFileName = dataFileName; }
		void SetSaveFileName(string saveFilename) { _saveFileName = saveFilename; }
		osg::ref_ptr<osg::Group> GetRoot() { return _root; }

		bool Process(BuildParameters bp = BuildParameters(16, 20, 0.1));
	};
}

