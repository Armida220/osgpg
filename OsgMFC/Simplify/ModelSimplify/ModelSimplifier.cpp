#include "StdAfx.h"
#include <algorithm>
#include "ModelSimplifier.h"


ModelSimplifier::ModelSimplifier(void)
{
	setTraversalMode(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN);
}


ModelSimplifier::~ModelSimplifier(void)
{
	m_vecAllSimplifyFunctors.clear();
}


void ModelSimplifier::AddSimplifyFunctor(SimplifyFunctorBase *pFunctor)
{
	m_vecAllSimplifyFunctors.push_back(pFunctor);
}


bool ModelSimplifier::RemoveSimplifyFunctor(SimplifyFunctorBase *pFunctor)
{
	std::vector< osg::ref_ptr<SimplifyFunctorBase> >::iterator itorFind
		= std::find(m_vecAllSimplifyFunctors.begin(), m_vecAllSimplifyFunctors.end(), pFunctor);
	if(itorFind == m_vecAllSimplifyFunctors.end())
	{
		return false;
	}

	m_vecAllSimplifyFunctors.erase(itorFind);
	return true;
}


unsigned ModelSimplifier::GetSimplifyFunctorCount(void) const
{
	return m_vecAllSimplifyFunctors.size();
}


void ModelSimplifier::apply(osg::PagedLOD &node)
{
	std::vector< osg::ref_ptr<SimplifyFunctorBase> >::iterator itorFunctor
		= m_vecAllSimplifyFunctors.begin();
	while(itorFunctor != m_vecAllSimplifyFunctors.end())
	{
		SimplifyFunctorBase *pFunctor = (*itorFunctor).get();
		(*pFunctor)(node);
		++itorFunctor;
	}
}