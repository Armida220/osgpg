#include "PointsDividor.h"
#include "KdTree.h"
#include <fstream>
#include <osg/Shape>
#include <osg/ShapeDrawable>

#include <stdlib.h>
#include <time.h>

using namespace FC;

//////////////////////////////////////////////////////////////////////////
//											PointDividor
//////////////////////////////////////////////////////////////////////////
bool PointsDividor::read_point()
{
	if(!infile)
	{
		_log << "Can not open this file in this path"<<endl;
		return false;
	}

	_log<<"Begin to read data."<<endl;

	int num,tmp;
	infile>>num;
	tmp = num;

	_points.reserve(num);
	_bb.init();

	string str;
	getline(infile, str, '\n');
	double x,y,z;
	double I;
	double r,g,b;
	int flag=0;

	//try
	I = r = g = b = -1;
	getline(infile, str, '\n');
	sscanf(str.c_str(), "%lf %lf %lf %lf %lf %lf %lf", &x, &y, &z, &I, &r, &g, &b);

	if(I==-1) {
		flag = XYZ; // x y z
	} else {
		if(r==-1) {
			flag = XYZI; // x y z i
		} else {
			if(b==-1) {
				flag = XYZRGB; // x y z r g b
			} else {
				flag = XYZIRGB; // x y z i r g b
			}
		}
	}

	_fileType = flag;

	switch(flag){
		case XYZ:
			_points.push_back(PointRec(x,y,z));
			break;
		case XYZI:
			_points.push_back(PointRec(x,y,z,I));
			break;
		case XYZRGB:
			_points.push_back(PointRec(x,y,z,-1, I, r, g));//ATTENTION!
			break;
		case XYZIRGB:
			_points.push_back(PointRec(x,y,z,I,r,g,b));
			break;
	}
	--num;

	do
	{
		switch(flag){
	case XYZ:
		infile >> x >> y >> z;
		_points.push_back(PointRec(x,y,z));
		break;
	case XYZI:
		infile >> x >> y >> z >> I;
		_points.push_back(PointRec(x,y,z,I));
		break;
	case XYZRGB:
		infile >> x >> y >> z >> r >> g >> b;
		_points.push_back(PointRec(x,y,z,-1, r, g, b));
		break;
	case XYZIRGB:
		infile >> x >> y >> z >> I >> r >> g >> b;
		_points.push_back(PointRec(x,y,z, I, r, g, b));
		break;
		}

		_bb.expandBy(x, y, z);

	}while(--num);

	infile.close();
	_log<<"All "<<_points.size()<<" datas have been read successfully!"<<endl;

	return true;
}

osg::Geode* PointsDividor::makePointsInOneNode(PointIdxSet& pis,
																							 osg::BoundingBox bb)
{
	if(pis.size()<=0)
		return 0;

	osg::Geode *geode = new osg::Geode;

#ifdef _DEBUG
	osg::Box* box = new osg::Box(bb.center(), bb.xMax()-bb.xMin(),
		bb.yMax()-bb.yMin(), bb.zMax()-bb.zMin());
	osg::ShapeDrawable* sd = new osg::ShapeDrawable(box);
	sd->setColor(osg::Vec4(rand()%100/100.0, rand()%100/100.0, rand()%100/100.0,0.8));
	geode->addDrawable(sd);
#else
	osg::Geometry *galaxy = new osg::Geometry;
	osg::Vec4Array *colorsArr = new osg::Vec4Array;
	osg::Vec3Array *pointsArr = new osg::Vec3Array;
	int nvertices = pis.size();

	galaxy->setVertexArray(pointsArr);
	galaxy->setColorArray(colorsArr);

	switch(_fileType) {
		case XYZ://x y z
			{
				osg::Vec4 ini(1,1,0,0);
				colorsArr->push_back(ini);
				galaxy->setColorBinding(osg::Geometry::BIND_OVERALL);

				for(PointIdxSet::iterator pi=pis.begin();
					pi!=pis.end(); ++pi)
				{
					PointRec& p = _points[ (*pi) ];
					pointsArr->push_back(osg::Vec3(p.x, p.y, p.z));
				}
			}
			break;
		case XYZI://x y z i
			{
				galaxy->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

				for(PointIdxSet::iterator pi=pis.begin();
					pi!=pis.end(); ++pi)
				{
					PointRec& p = _points[ (*pi) ];
					pointsArr->push_back(osg::Vec3(p.x, p.y, p.z));

					double c = p.i/255.0;
					colorsArr->push_back(osg::Vec4(c, c, c, 0));
				}
			}
			break;
		case XYZRGB://x y z r g b
		case XYZIRGB://x y z i r g b
			{
				galaxy->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

				for(PointIdxSet::iterator pi=pis.begin();
					pi!=pis.end(); ++pi)
				{
					PointRec& p = _points[ (*pi) ];
					pointsArr->push_back(osg::Vec3(p.x, p.y, p.z));

					double r=p.r;
					double g=p.g;
					double b=p.b;
					if(r>1 || g>1 || b>1) {
						r/=255; g/=255; b/=255;
					}
					colorsArr->push_back(osg::Vec4(r, g, b, 0));
				}
			}
			break;
	}

	galaxy->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS, 
		0, nvertices));
	geode->addDrawable(galaxy);
	geode->setInitialBound( geode->getBound() );
#endif

	return geode;
}

bool PointsDividor::process(unsigned int maxLevels,
														float sampleRatio, unsigned int targetPointsNumOnLeaf)
{
	SetMaxLevels(maxLevels);
	SetSampleRatio(sampleRatio);
	SetTargetPointsNumOnLeaf(targetPointsNumOnLeaf);

	time_t startTime, endTime;
	
	{
		startTime = clock();
		if(!read_point())
			_log<<"read point error£¡"<<endl;
		else {
			endTime = clock();
			_log<<"Read Raw Data, Completed!"<<endl;
			_log<<"used time:"<<difftime(endTime, startTime)<<" (ms)"<<endl;
		}
	}

	_root = new osg::LOD;
	FC::KdTree kdTree;
	kdTree._bp._maxLevels = GetMaxLevels();
	kdTree._bp._sampleRatio = GetSampleRatio();
	kdTree._bp._targetPointsNumOnLeaf = GetTargetPointsNumOnLeaf();
	{
		startTime = clock();
		if(!kdTree.Build(this))
			return false;
		else {
			endTime = clock();
			_log<<"Build KdTree and Scene Graph, Completed!"<<endl;
			_log<<"used time:"<<difftime(endTime, startTime)<<" (ms)"<<endl;
		}
	}

	return true;
}