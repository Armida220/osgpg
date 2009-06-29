#include "StdAfx.h"
#include "PluginManager.h"

namespace FC {
	PluginManager::PluginManager(void)
	{
	}

	PluginManager::~PluginManager(void)
	{
		while(!_pluginList.empty()) {
			this->UnLoadPlugin(_pluginList.begin());
		}
		_pluginList.clear();
	}

	void PluginManager::UnLoadPlugin(PluginList::iterator itr)
	{
		PLUG_ST& plugin = *itr;
		if(plugin.pObj) {
			plugin.pObj->Release();
			plugin.pObj = 0;

			if(plugin.hIns)
				FreeLibrary(plugin.hIns);

			_pluginList.erase(itr);
		}
	}

	WorkFlowPluginBase* PluginManager::GetWorkFlow(unsigned int i)
	{
		for(PluginList::iterator itr = _pluginList.begin();
			itr!=_pluginList.end(); ++itr, --i)
		{
			if(i==0) {
				PLUG_ST& plugin = *itr;
				return plugin.pObj;
			}
		}
		return 0;
	}

	bool PluginManager::AddPlugin(CString name)
	{
		PLUG_ST stPs;
		ZeroMemory(&stPs, sizeof(stPs));

		stPs.hIns = LoadLibrary(name);
		if(stPs.hIns==0) {
			AfxMessageBox("加载插件错误：无法加载该库！");
			return false;
		}
		
		CreatePluginCallBack pFunc = 
			(CreatePluginCallBack)GetProcAddress(stPs.hIns, "CreateWorkFlowPlugin");
		if(pFunc==0) {
			AfxMessageBox("加载插件错误：找不到CreateWorkFlowPlugin函数！");
			return false;
		}

		WorkFlowPluginBase* pTmp;
		if (pFunc((void **)(&pTmp))) {
			stPs.pObj = pTmp;
			this->_pluginList.push_back(stPs);
		}
		else {
			AfxMessageBox("CreateWorkFlowPlugin生成插件错误！");
			return false;
		}

		return true;
	}
}
