#include "stdafx.h"
#include "TinBuildAPI.h"

#include <iostream>

#define X 0
#define Y 1
#define Z 2

namespace FC {
	bool ReadTinHead(TinDataFile& tdf, std::ifstream& in)
	{
		in>> tdf._pointNum >> tdf._triangleNum >> tdf._textureNum;
		return true;
	}

	bool ReadTinPoints(TinDataFile& tdf, std::ifstream& in)
	{
		tdf._points = new osg::Vec3Array(tdf._pointNum);

		for(long i=0; i<tdf._pointNum; ++i) {
			double x, y, z;
			in>> x >> y >> z;
			osg::Vec3& v = tdf._points->at(i);
			v.set(x, y, z);
		}

		return true;
	}

	bool ReadTinTriangles(TinDataFile& tdf, std::ifstream& in)
	{
		long estimatedSetSize = tdf._triangleNum / tdf._textureNum + 10;

		for(long i=0; i<tdf._textureNum; ++i) {
			TriangleSet ts;
			ts.reserve(estimatedSetSize);

			tdf._tex2TriSet.insert(
				TextureID2TriangleSet::value_type(
				i, ts
				)
				);
		}

		for(long i=0; i<tdf._triangleNum; ++i) {
			TriangleRecord tr;
			long id;

			in >> tr._idx[0] >> tr._idx[1] >> tr._idx[2];
			in >> id;
			in >> tr._tc[0][X] >> tr._tc[0][Y] 
			>> tr._tc[1][X] >> tr._tc[1][Y] 
			>> tr._tc[2][X] >> tr._tc[2][Y] ;

			tdf._tex2TriSet[id].push_back( tr	);
		}

		return true;
	}

	bool ReadTinTexturesPath(TinDataFile& tdf, std::ifstream& in)
	{
		tdf._texturesPath.reserve(tdf._textureNum);

		for(long i=0; i<tdf._textureNum; ++i) {
			string path;
			in >> path;

			tdf._texturesPath.push_back(path);
		}

		return true;
	}
}
