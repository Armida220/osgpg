#pragma once

#include "WorkFlowPluginBase.h"
#include "ControllerPluginBase.h"

#include <osg/ref_ptr>
using namespace std;

typedef bool (*CreatePluginCallBack)(void **);

namespace FC {
	typedef struct _WorkFlowPLUG_ST {
		WorkFlowPluginBase* pObj;
		HINSTANCE hIns;
	}WorkFlowPLUG_ST;

	typedef struct _ControllerPLUG_ST {
		ControllerPluginBase* pObj;
		HINSTANCE hIns;
	}ControllerPLUG_ST;

	typedef map<CString, ControllerPLUG_ST> Name2Controller;

	class PluginManager
	{
	public:
		PluginManager(void);
		~PluginManager(void);

		bool SetWorkFlowPlugin(CString name);
		inline void UnLoadWorkFlowPlugin();
		WorkFlowPluginBase* GetWorkFlow();

		ControllerPluginBase* AddControllerPlugin(CString name);
		void UnLoadControllerPlugin(CString name);
		void UnLoadControllerPlugin(Name2Controller::iterator itr);
		ControllerPluginBase* GetControllerCreator(CString name);

	protected:
		WorkFlowPLUG_ST _workFlowPlugin;
		Name2Controller _ctrlMap;
	};
}
