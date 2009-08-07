#ifndef SIMPLIFY_FUNCTOR_BASE_H_ED1D1072_D18C_40DD_BF81_3FBC6F1F2D9A_INCLUDE
#define SIMPLIFY_FUNCTOR_BASE_H_ED1D1072_D18C_40DD_BF81_3FBC6F1F2D9A_INCLUDE

#include <osg/Referenced>
#include <osg/PagedLOD>

class SimplifyFunctorBase : public osg::Referenced
{
protected:
	explicit SimplifyFunctorBase(void)
	{
	}

public:
	virtual ~SimplifyFunctorBase(void)
	{
	}

protected:
	virtual bool NeedSimplify(osg::PagedLOD &node)
	{
		return false;
	}

public:
	virtual void operator()(osg::PagedLOD &node)
	{
	}
};

#endif