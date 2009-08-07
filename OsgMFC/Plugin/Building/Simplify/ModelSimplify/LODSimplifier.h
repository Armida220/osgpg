#ifndef LOD_SIMPLIFIER_H_0C7B601A_4195_4AD4_9D8C_70307B67233F_INCLUDE
#define LOD_SIMPLIFIER_H_0C7B601A_4195_4AD4_9D8C_70307B67233F_INCLUDE

#include "SimplifyFunctorBase.h"

class LODSimplifier : public SimplifyFunctorBase
{
public:
	explicit LODSimplifier(void);
protected:
	virtual ~LODSimplifier(void);

protected:
	virtual void operator()(osg::PagedLOD &node);
	virtual bool NeedSimplify(osg::PagedLOD &node);

public:
	//第四个参数表示三角形数量与geode体积之比的阈值,默认-1表示不进行此项剔除
	void	SetThreshold(double dblProb, double dblCenter, double dblRadius, double dblTriVol=-1);

protected:
	double	m_dblProbability;	// 有多大可能性删除粗纹理一级LOD
	double	m_dblCenterThreshold;
	double	m_dblRadiusThreshold;
	double  m_dblTriangleNumVsVolumeThreshold;
};

#endif