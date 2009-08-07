#include "StdAfx.h"
#include "PluginManager.h"

namespace FC {
	PluginManager::PluginManager(void)
	{
		_workFlowPlugin.pObj = 0;
		_workFlowPlugin.hIns = 0;
	}

	PluginManager::~PluginManager(void)
	{
		this->UnLoadWorkFlowPlugin();
		
		while(!_ctrlMap.empty()) {
			UnLoadControllerPlugin(_ctrlMap.begin());
		}
	}

	void PluginManager::UnLoadWorkFlowPlugin()
	{
		WorkFlowPLUG_ST& plugin = this->_workFlowPlugin;
		if(plugin.pObj) {
			plugin.pObj->Release();
			plugin.pObj = 0;

			if(plugin.hIns)
				FreeLibrary(plugin.hIns);
		}
	}

	WorkFlowPluginBase* PluginManager::GetWorkFlow()
	{
		return _workFlowPlugin.pObj;
	}

	bool PluginManager::SetWorkFlowPlugin(CString name)
	{
		WorkFlowPLUG_ST stPs;
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
			_workFlowPlugin = stPs;
		}
		else {
			AfxMessageBox("CreateWorkFlowPlugin生成插件错误！");
			return false;
		}

		return true;
	}

	ControllerPluginBase* PluginManager::AddControllerPlugin(CString name)
	{
		if(_ctrlMap.find(name) != _ctrlMap.end()) {
			AfxMessageBox("该插件已经加载！请勿重复加载！");
			return 0;
		}

		ControllerPLUG_ST cPs;
		ZeroMemory(&cPs, sizeof(cPs));

		cPs.hIns = LoadLibrary(name);
		if(cPs.hIns==0) {
			AfxMessageBox("加载插件错误：无法加载该库！");
			return 0;
		}

		CreatePluginCallBack pFunc = 
			(CreatePluginCallBack)GetProcAddress(cPs.hIns, "CreateControllerPlugin");
		if(pFunc==0) {
			AfxMessageBox("加载插件错误：找不到CreateControllerPlugin函数！");
			return 0;
		}

		ControllerPluginBase* pTmp;
		if (pFunc((void **)(&pTmp))) {
			cPs.pObj = pTmp;
			_ctrlMap.insert( Name2Controller::value_type(name, cPs) );
		}
		else {
			AfxMessageBox("CreateControllerPlugin生成插件错误！");
			return 0;
		}

		return cPs.pObj;
	}

	void PluginManager::UnLoadControllerPlugin(CString name)
	{
		Name2Controller::iterator itr = _ctrlMap.find(name);
		UnLoadControllerPlugin(itr);
	}

	void PluginManager::UnLoadControllerPlugin(Name2Controller::iterator itr)
	{
		if( itr==_ctrlMap.end() ) return;

		ControllerPLUG_ST& plugin = itr->second;
		if(plugin.pObj) {
			plugin.pObj->Release();
			plugin.pObj = 0;

			if(plugin.hIns)
				FreeLibrary(plugin.hIns);
		}

		_ctrlMap.erase(itr);
	}

	ControllerPluginBase* PluginManager::GetControllerCreator(CString name)
	{
		Name2Controller::iterator itr = _ctrlMap.find(name);
		if(itr!=_ctrlMap.end()) {
			return itr->second.pObj;
		} else {
			return 0;
		}
	}
}
