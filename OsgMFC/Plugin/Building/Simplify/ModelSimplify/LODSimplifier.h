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
	//���ĸ�������ʾ������������geode���֮�ȵ���ֵ,Ĭ��-1��ʾ�����д����޳�
	void	SetThreshold(double dblProb, double dblCenter, double dblRadius, double dblTriVol=-1);

protected:
	double	m_dblProbability;	// �ж�������ɾ��������һ��LOD
	double	m_dblCenterThreshold;
	double	m_dblRadiusThreshold;
	double  m_dblTriangleNumVsVolumeThreshold;
};

#endif