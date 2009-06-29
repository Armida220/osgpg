#pragma once

#include "WorkFlowPluginBase.h"

#include <osg/ref_ptr>
#include <list>
using namespace std;

typedef bool (*CreatePluginCallBack)(void **);

namespace FC {
	typedef struct _PLUG_ST {
		WorkFlowPluginBase* pObj;
		HINSTANCE hIns;
	}PLUG_ST, * LPPLUG_ST;

	typedef list<PLUG_ST> PluginList;

	class PluginManager
	{
	public:
		PluginManager(void);
		~PluginManager(void);

		bool AddPlugin(CString name);

		inline void UnLoadPlugin(PluginList::iterator itr);

		WorkFlowPluginBase* GetWorkFlow(unsigned int i);

	protected:
		PluginList _pluginList;
	};
}
