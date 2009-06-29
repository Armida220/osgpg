#ifndef TRIANGLE_SIMPLIFIER_H_4AC5EAAA_71F0_47C0_ACD9_F135E895A2A4_INCLUDE
#define TRIANGLE_SIMPLIFIER_H_4AC5EAAA_71F0_47C0_ACD9_F135E895A2A4_INCLUDE

#include <set>
#include "SimplifyFunctorBase.h"

class TriangleSimplifier : public SimplifyFunctorBase
{
public:
	explicit TriangleSimplifier(void);
protected:
	virtual ~TriangleSimplifier(void);

protected:
	virtual void operator()(osg::PagedLOD &node);
	virtual bool NeedSimplify(osg::PagedLOD &node);

public:
	void	SetThreshold(double dblArea, double dblAngle);

protected:
	double area(osg::Vec3& v1, osg::Vec3& v2, osg::Vec3& v3)
	{
		double l12 = (v1^v2).length();
		double l23 = (v2^v3).length();
		double l31 = (v3^v1).length();
		return abs( l12 + l23 + l31 );
	}

	double minAngle(osg::Vec3& v1, osg::Vec3& v2, osg::Vec3& v3)
	{
		osg::Vec3 v12 = v1-v2;
		osg::Vec3 v23 = v2-v3;
		osg::Vec3 v31 = v3-v1;
		double l12=v12.length();
		double l23=v23.length();
		double l31=v31.length();
		double a2 = (v12^v23).length() / l12 / l23;
		double a3 = (v23^v31).length() / l23 / l31;
		double a1 = (v31^v12).length() / l31 / l12;
		return min(a1, min(a2, a3));
	}

protected:
	std::set<osg::PagedLOD *>	m_setLod;
	double	m_dblAreaThreshold;
	double	m_dblAngleThreshold;

};

#endif