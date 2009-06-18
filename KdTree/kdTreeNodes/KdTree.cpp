#include "KdTree.h"

using namespace FC;

//////////////////////////////////////////////////////////////////////////

KdTree::KdTree()
{
	_pd = 0;
	_points = 0;
}

KdTree::~KdTree(void)
{
}

//find the max axis of bb
int KdTree::findMaxAxis(osg::BoundingBox bb)
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

void KdTree::BuildKdTree(PointIdxSet& pis, osg::BoundingBox bb, 
												 unsigned int level, osg::ref_ptr<osg::LOD> root)
{
	root->setCenter(bb.center());
	root->setRadius(bb.radius());

	if(level >= _bp._maxLevels ||
		pis.size() <= _bp._targetPointsNumOnLeaf)
	{
		//build self
		{
			osg::Geode* gd = _pd->makePointsInOneNode(pis, bb);
			if(gd)
#ifdef _DEBUG
				root->addChild( gd, bb.radius()*10, FLT_MAX);
#else
				root->addChild( gd, 0, bb.radius()*10);
#endif
		}
		return;
	}

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
	
	osg::BoundingBox leftbb;
	osg::BoundingBox rightbb;
	leftbb.init();
	rightbb.init();

	unsigned int cnt=0;
	for(PointIdxSet::iterator pi=pis.begin();
		pi!=pis.end(); ++pi, --cnt)
	{
		PointRec& p = _points->at( (*pi) );

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

	//build self
	{
		osg::Geode* gd = _pd->makePointsInOneNode(selfPis, bb);
		if(gd)
#ifdef _DEBUG
			root->addChild( gd, bb.radius()*10, FLT_MAX);
#else
			root->addChild( gd, 0, bb.radius()*10);
#endif
	}

	//build child
	{
		osg::ref_ptr<osg::LOD> lcRoot = new osg::LOD;
		BuildKdTree(leftPis, leftbb, level+1, lcRoot);
		if(lcRoot->getNumChildren()>0)
#ifdef _DEBUG
			root->addChild( lcRoot.get(), leftbb.radius()*5, FLT_MAX );
#else
			root->addChild( lcRoot.get(), 0, leftbb.radius()*5 );
#endif

		osg::ref_ptr<osg::LOD> rcRoot = new osg::LOD;
		BuildKdTree(rightPis, rightbb, level+1, rcRoot);
		if(rcRoot->getNumChildren()>0)
#ifdef _DEBUG
			root->addChild( rcRoot.get(), rightbb.radius()*5, FLT_MAX );
#else
			root->addChild( rcRoot.get(), 0, rightbb.radius()*5 );
#endif
			
	}
}

bool KdTree::Build(PointsDividor* pd)
{
	_pd = pd;
	_points = &(pd->GetRawPoints());

	osg::BoundingBox bb;
	PointIdxSet pis;
	bb.init();
	
	for(unsigned int i=0; i<_points->size(); ++i)
	{
		PointRec& p = _points->at(i);
		pis.push_back(i);
		bb.expandBy(p.x, p.y, p.z);
	}

	osg::ref_ptr<osg::LOD> root = pd->GetRoot();
	BuildKdTree(pis, bb, 0, root);

	_pd = 0;
	_points = 0;

	return true;
}