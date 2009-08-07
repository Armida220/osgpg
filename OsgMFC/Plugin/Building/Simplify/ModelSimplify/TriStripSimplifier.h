#pragma once
#include "simplifyfunctorbase.h"

/**对所有PagedLOD引用的子结点进行条带化处理
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
