#ifndef MODEL_SIMPLIFIER_H_4D6072F2_190A_43C6_BE89_6F84B3020873_INCLUDE
#define MODEL_SIMPLIFIER_H_4D6072F2_190A_43C6_BE89_6F84B3020873_INCLUDE

#include <vector>
#include <osg/NodeVisitor>
#include <osg/PagedLOD>
#include "SimplifyFunctorBase.h"

class ModelSimplifier : public osg::NodeVisitor
{
public:
	explicit ModelSimplifier(void);
	virtual ~ModelSimplifier(void);

public:
	virtual void apply(osg::PagedLOD &node);

public:
	void		AddSimplifyFunctor(SimplifyFunctorBase *pFunctor);
	bool		RemoveSimplifyFunctor(SimplifyFunctorBase *pFunctor);
	unsigned	GetSimplifyFunctorCount(void) const;

protected:
	std::vector< osg::ref_ptr<SimplifyFunctorBase> >	m_vecAllSimplifyFunctors;
};

#endif