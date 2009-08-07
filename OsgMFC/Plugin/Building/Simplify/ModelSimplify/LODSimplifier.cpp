#include "StdAfx.h"
#include "LODSimplifier.h"
#include "osgDB/ReadFile"
#include "osg/TriangleFunctor"

struct TriangleNumFunctor {
	TriangleNumFunctor() {num=0;}
	unsigned int num;
	void operator() (const osg::Vec3& v1, const osg::Vec3& v2, const osg::Vec3& v3, bool)
	{
		++num;
	}
};

class TriangleNumVisitor : public osg::NodeVisitor
{
public:
	TriangleNumVisitor() {num=0;}

	void ReSet() {num=0;}
	unsigned int GetNum() const { return num; }

	virtual void apply(osg::Geode& node)
	{
		for(unsigned int i=0; i<node.getNumDrawables(); ++i) {
			osg::Drawable* drawable = node.getDrawable(i);
			osg::TriangleFunctor<TriangleNumFunctor> tnf;
			tnf.num = 0;
			drawable->accept(tnf);
			num += tnf.num;
		}
	}
protected:
	unsigned int num;
};


LODSimplifier::LODSimplifier(void)
{
	srand((unsigned)time(NULL));

	m_dblProbability = 0.5;
	m_dblCenterThreshold = 5.0;
	m_dblRadiusThreshold = 10.0;
	m_dblTriangleNumVsVolumeThreshold = -1;
}


LODSimplifier::~LODSimplifier(void)
{
}


void LODSimplifier::SetThreshold(double dblProb, double dblCenter, double dblRadius, double dblTriVol)
{
	if(dblProb > 1.0)		dblProb = 1.0;
	else if(dblProb < 0.0)	dblProb = 0.0;
	m_dblProbability = dblProb;

	m_dblCenterThreshold = dblCenter;
	m_dblRadiusThreshold = dblRadius;

	m_dblTriangleNumVsVolumeThreshold = dblTriVol;
}


bool LODSimplifier::NeedSimplify(osg::PagedLOD &node)
{
	//FC 注意这一段要写成这样才是正确的效果，尹老师原来那段有问题。
	
	//条件1.小于高程阈值且小于半径阈值的，按比例剔除
	if(node.getCenter().z()<m_dblCenterThreshold &&
		node.getRadius()<m_dblRadiusThreshold) {
			if(rand()%100<m_dblProbability*100)
				return true;
	}

	//条件2.三角形数量与boundingbox体积之比大于阈值者，直接剔除
	if(m_dblTriangleNumVsVolumeThreshold != -1) {
		osg::ref_ptr<osg::Node> pSubNode = 
			osgDB::readNodeFile(node.getFileName(0));
		if(!pSubNode.valid()) return false;
		osg::Geode *pGeode = pSubNode->asGeode();
		if(!pGeode)		return false;
		//calc volume
		const osg::BoundingBox& bb = pGeode->getBoundingBox();
		double lenX = bb.xMax() - bb.xMin();
		double lenY = bb.yMax() - bb.yMin();
		double lenZ = bb.zMax() - bb.zMin();
		double bb_volume = lenX * lenY * lenZ;
		//calc num of triangle
		TriangleNumVisitor tnv;
		tnv.ReSet();
		pGeode->accept(tnv);
		unsigned int triNum = tnv.GetNum();
		//judge
		if(triNum/bb_volume > m_dblTriangleNumVsVolumeThreshold) {
			return true;
		}
	}
	
	return false;
}


void LODSimplifier::operator()(osg::PagedLOD &node)
{
	if(!NeedSimplify(node))
	{
		return;
	}

	if(node.getNumFrameNumbers() > 0 || node.getNumRanges() > 0)
	{
		node.removeChildren(0);
	}
}

