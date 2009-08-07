#pragma once
#include "simplifyfunctorbase.h"

/**������PagedLOD���õ��ӽ���������������
*/
class TriStripSimplifier :
	public SimplifyFunctorBase
{
public:
	TriStripSimplifier(void);
	~TriStripSimplifier(void);
protected:
	virtual void operator()(osg::PagedLOD &node);
};
