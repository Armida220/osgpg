#include "LODGenStrategy.h"

#include "PointSetDividor.h"
#include <osg/BoundingBox>
#include <osg/lod>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <fstream>
#include <time.h>
using namespace std;

using namespace FC;

//////////////////////////////////////////////////////////////////////////
//										Global
//////////////////////////////////////////////////////////////////////////
int findMaxAxis(osg::BoundingBox bb)
{
	int axis = -1;
	float xlen = bb.xMax() - bb.xMin();
	float ylen = bb.yMax() - bb.yMin();
	float zlen = bb.zMax() - bb.zMin();
	if(xlen>=ylen) {
		if(xlen>=zlen)
			axis = 0;
		else
			axis = 2;
	} else if(ylen>zlen) {
		axis = 1;
	} else {
		axis = 2;
	}
	return axis;
}

inline void printTab(unsigned int num, std::ofstream& log)
{
	while(num--) { log << "\t"; }
}

osg::Geode* makePointsInOneNode(PointSet& points, PointIdxSet& pis,
														osg::BoundingBox bb, const unsigned int recordType)
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

	switch(recordType) {
		case XYZ://x y z
			{
				osg::Vec4 ini(1,1,1,0);
				colorsArr->push_back(ini);
				galaxy->setColorBinding(osg::Geometry::BIND_OVERALL);

				for(PointIdxSet::iterator pi=pis.begin();
					pi!=pis.end(); ++pi)
				{
					PointRec& p = points.at(*pi);//PointRec& p = *(*pi);
					pointsArr->push_back(osg::Vec3(p.x, p.y, p.z));
				}
			}
			break;
		case XYZI://x y z i
		case XYZIRGB://x y z i r g b
			{
				//linear transform of I [Imin, Imax] -> [0, 255]
				float Imax = PointRec::GetMaxI();
				float Imin = PointRec::GetMinI();
				float a = 255.0/(Imax - Imin);
				float b = -a*Imin;

				galaxy->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

				for(PointIdxSet::iterator pi=pis.begin();
					pi!=pis.end(); ++pi)
				{
					PointRec& p = points.at(*pi);//PointRec& p = *(*pi);
					pointsArr->push_back(osg::Vec3(p.x, p.y, p.z));

					float c = (a*p.i+b)/255.0;
					colorsArr->push_back(osg::Vec4(c, c, 0, 0));
				}
			}
			break;
		case XYZRGB://x y z r g b
			{
#define COLOR_MODE 1
#if COLOR_MODE
				galaxy->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
#else
				osg::Vec4 ini(1,1,1,0);
				colorsArr->push_back(ini);
				galaxy->setColorBinding(osg::Geometry::BIND_OVERALL);
#endif

				for(PointIdxSet::iterator pi=pis.begin();
					pi!=pis.end(); ++pi)
				{
					PointRec& p = points.at(*pi);//PointRec& p = *(*pi);
					pointsArr->push_back(osg::Vec3(p.x, p.y, p.z));

#if COLOR_MODE
					double r=p.r;
					double g=p.g;
					double b=p.b;
					if(r>1 || g>1 || b>1) {
						r/=255; g/=255; b/=255;
					}
					colorsArr->push_back(osg::Vec4(r, g, b, 0));
#endif
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

//////////////////////////////////////////////////////////////////////////
//								TraditionKdTreeGenStrategy
//////////////////////////////////////////////////////////////////////////
bool TraditionKdTreeGenStrategy::Generate(PointSet &pointSet, 
																					osg::ref_ptr<osg::LOD> root, BuildParameters bp)
{
	return false;
}

//////////////////////////////////////////////////////////////////////////
//								SampleKdTreeGenStrategy
//////////////////////////////////////////////////////////////////////////
bool SampleKdTreeGenStrategy::Generate(PointSet &pointSet, 
																			 osg::ref_ptr<osg::LOD> root, BuildParameters bp)
{
	time_t sT, eT;
	sT = clock();

	_bp = bp;
	_points = &pointSet;

	osg::BoundingBox bb;
	bb.init();

	PointIdxSet pis;
	pis.reserve(pointSet.size());
	for(PointIdx i = 0;//pointSet.begin(); 
		i<pointSet.size(); ++i)//i!=pointSet.end(); ++i)
	{
		PointRec& p = pointSet[i];//(*i);
		pis.push_back(i);
		bb.expandBy(p.x, p.y, p.z);
	}

	if(!root.valid()) {
		_log<<"Generate Error! root invalid!"<<endl;
		return false;
	}

	try {
		BuildKdTree(pis, bb, 0, root);
	} catch (...) {
		_log<<"Build Kd tree Error!"<<endl;
		_points = 0;
		return false;
	}
	_points = 0;

	eT = clock();
	_log << "Generate-Time: "<<difftime(eT, sT)<<endl;

	return true;
}

#define DRAW_TREE_IN_LOG 0

void SampleKdTreeGenStrategy::BuildKdTree(PointIdxSet &pis, osg::BoundingBox bb,
																					unsigned int level, osg::ref_ptr<osg::LOD> root)
{
	root->setCenter(bb.center());
	root->setRadius(bb.radius());

	if(level >= _bp._maxLevels ||
		pis.size() <= _bp._targetPointsNumOnLeaf)
	{
#if DRAW_TREE_IN_LOG
		printTab(level, _log);
		_log<<"Leaf : points num = "<<pis.size()<<endl;
#endif
		//build self
		{
			_log<<"RecordType: "<<_bp._recordType<<endl;
			osg::Geode* gd = makePointsInOneNode(*_points ,pis, bb, _bp._recordType);
			if(gd)
#ifdef _DEBUG
				root->addChild( gd, bb.radius()*10, FLT_MAX);
#else
				root->addChild( gd, 0, bb.radius()*20);
#endif
		}

		//ATTENTION! we clear pis here, for it has no use anymore
		pis.clear();

		return;
	}

#if DRAW_TREE_IN_LOG
	printTab(level, _log);
	_log<<"Level "<< level << " : points num = " << pis.size() << " {"<<endl;
#endif

	int axis = findMaxAxis(bb);

	float mid = bb.center()[axis];
	PointIdxSet selfPis;
	PointIdxSet leftPis;
	PointIdxSet rightPis;

	//if the estimated num to sampled is less than leaf target num
	//then there is no need to sample
	unsigned int selfEstimateNum = pis.size()*_bp._sampleRatio;
	unsigned int interval = 1/_bp._sampleRatio;
	bool needSample = selfEstimateNum>_bp._targetPointsNumOnLeaf;

	selfPis.reserve(selfEstimateNum);
	leftPis.reserve(pis.size()/2);
	rightPis.reserve(pis.size()/2);

	osg::BoundingBox leftbb;
	osg::BoundingBox rightbb;
	leftbb.init();
	rightbb.init();

	unsigned int cnt=0;
	for(PointIdxSet::iterator pi=pis.begin();
		pi!=pis.end(); ++pi, --cnt)
	{
		PointRec& p = _points->at(*pi);//*(*pi);

		if(needSample && cnt==0) {
			selfPis.push_back( (*pi) );
			cnt=interval;
		}	else if(p[axis]<mid) {
			leftbb.expandBy(p.x, p.y, p.z);
			leftPis.push_back( (*pi) );
		} else {
			rightbb.expandBy(p.x, p.y, p.z);
			rightPis.push_back( (*pi) );
		}
	}

	//ATTENTION! we clear pis here, for it has no use anymore
	pis.clear();

	//build self
	{
#if DRAW_TREE_IN_LOG
		printTab(level+1, _log);
		_log<<"Self Sample : points num = " << selfPis.size() <<endl;
#endif

		osg::Geode* gd = makePointsInOneNode(*_points, selfPis, bb, _bp._recordType);
		if(gd)
#ifdef _DEBUG
			root->addChild( gd, bb.radius()*10, FLT_MAX);
#else
			root->addChild( gd, 0, bb.radius()*10);
#endif

		selfPis.clear();
	}

	//build child
	{
		osg::ref_ptr<osg::LOD> lcRoot = new osg::LOD;
		BuildKdTree(leftPis, leftbb, level+1, lcRoot);
		if(lcRoot->getNumChildren()>0)
#ifdef _DEBUG
			root->addChild( lcRoot.get(), leftbb.radius()*5, FLT_MAX );
#else
			root->addChild( lcRoot.get(), 0, leftbb.radius()*10 );
#endif

		leftPis.clear();

		osg::ref_ptr<osg::LOD> rcRoot = new osg::LOD;
		BuildKdTree(rightPis, rightbb, level+1, rcRoot);
		if(rcRoot->getNumChildren()>0)
#ifdef _DEBUG
			root->addChild( rcRoot.get(), rightbb.radius()*5, FLT_MAX );
#else
			root->addChild( rcRoot.get(), 0, rightbb.radius()*10 );
#endif

		rightPis.clear();
	}

#if DRAW_TREE_IN_LOG
	printTab(level, _log);
	_log<<"}"<<endl;
#endif
}