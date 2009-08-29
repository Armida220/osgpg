#pragma once

#include <osg/ref_ptr>
#include <osg/Geometry>

#include <fstream>
#include <vector>
#include <string>
#include <map>

using namespace std;

namespace FC {
	typedef long	 TriangleIdx[3];
	typedef double TextureCoord[2];

	struct TriangleRecord {
		TriangleIdx		_idx;
		TextureCoord	_tc[3];
	};

	typedef vector<TriangleRecord> TriangleSet;

	typedef map<int, TriangleSet> TextureID2TriangleSet;

	struct TinDataFile {
		//head
		long _pointNum;
		long _triangleNum;
		long _textureNum;

		//body
		osg::ref_ptr<osg::Vec3Array> _points;
		TextureID2TriangleSet _tex2TriSet;
		vector<string>	_texturesPath;

		TinDataFile() { _points = 0; }
	};

	bool ReadTinHead(TinDataFile& tdf, std::ifstream& in);
	bool ReadTinPoints(TinDataFile& tdf, std::ifstream& in);
	bool ReadTinTriangles(TinDataFile& tdf, std::ifstream& in);
	bool ReadTinTexturesPath(TinDataFile& tdf, std::ifstream& in);
}