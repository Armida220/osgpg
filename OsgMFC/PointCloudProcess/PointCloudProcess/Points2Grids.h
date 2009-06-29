#pragma once

#include "stdafx.h"
#include <osg/Referenced>
#include <osg/PointSprite>
#include <osg/BlendFunc>
#include <osg/StateAttribute>
#include <osg/Point>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/TexEnv>
#include <osg/GLExtensions>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgViewer/Viewer>
#include <osg/ref_ptr>
#include <osg/Group>
#include <osg/Geode>
#include <osg/NodeCallback>
#include <osg/MatrixTransform>
#include <osg/Lod>
#include <osgSim/Impostor>
#include <osg/Drawable>

#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
using namespace std;

class Points2Grids
{
public:
	Points2Grids(fstream& in, int grid):
	  infile(in),_grid(grid), _log("Points2Grids.log")
	  {
		  setlocale(LC_ALL, "chs");
			_points.clear();
			_grids.clear();
	  }

	~Points2Grids(void) {}

	bool process();

	osg::ref_ptr<osg::Group> getRoot() { return _root; }


protected:
	//struct used to store features of the set of those points
	struct PointsInfo
	{
		//axes min
		float min_x,min_y,min_z;
		//num of grids is grid*grid*grid
		int   grid;
		float dx,dy,dz;
	};

	//FC 2009-03-29
	struct PointRec {
		float x,y,z;
		float i;
		float r,g,b;

		PointRec(float _x=0, float _y=0, float _z=0,
			float _i=-1, float _r=-1, float _g=-1, float _b=-1) {
				x=_x;	y=_y;	z=_z;
				i=_i;
				r=_r; g=_g; b=_b;
		}
	};

	bool read_point();

	void put_points_to_grids();

	osg::ref_ptr<osg::Group> makeGridsIntoNodes();

	osg::Geode *makePointsInOneNode(vector<PointRec>& points);

	osg::StateSet* makeStateSet(float size);


private:
	//needed for constructing the object
	fstream&			infile;				//FC 2009-03-19
	int					_grid;
	int					_fileType;			//FC 2009-03-28

	PointsInfo						_ptInfo;
	vector<PointRec>			_points;
	vector< vector<PointRec> > _grids;

	//for write
	osg::ref_ptr<osg::Group>		_root;
	std::ofstream					_log;
};
