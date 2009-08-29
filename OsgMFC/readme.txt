OsgFramework构架思路：
	
	程序主体分为两大部分
		OsgFrameWork
		Plugins
	即采用插件方式对程序功能进行扩展，从而减少主程序OsgFrameWork内部代码量，而且有助于将各个功能模块化。
	
	OsgFrameWork主程序通过PluginManager管理插件，
	每一个文档对象均附有一个PluginManager对象，每一个视图对象均附有一个ViewerFramework对象
	
	Plugin的实现方式如下：
		各Plugin均为以plugin为后缀名的dll文件，
		任何plugin均继承自FC::WorkFlowPluginBase或FC::ControllerPluginBase，
		凡继承自WorkFlowPluginBase的必须各自实现函数
		bool CreateWorkFlowPlugin(void ** pobj)
		{
			*pobj = new CPlugA;	//CPlugA继承自WorkFlowPluginBase
			return *pobj != NULL;
		}
		类似的，凡继承自ControllerPluginBase的必须各自实现函数CreateControllerPlugin
		
	Plugin目前主要分为两种：（可以根据需要，添加新的控制器种类，不过需要相应的更改主程序OsgFrameWork）
		WorkFlowPlugin		负责创立一个工作流
							这里工作流指的是：
								1.创立ViewerFrameworkImpl，主要功能是实现不同的osgViewer::compositeView
								  这一步骤的重载可以参考Camsimulator插件的实现
								2.为viewer的不同view分别添加场景数据（SetSceneData），目前大部分插件主要
								  实现这一部分功能的重载
								3.为不同的view分别设置GUIEventHandler
		ControllerPlugin	用于在场景加载之后添加新的控制场景的方式，比如默认场景控制方式是TrackBall，
							如果而二次开发者创立了新的控制器A，就可以在场景加载后变更控制器
	
	目前插件的功能介绍：
		Building插件组
			BuildingSimplifier插件		功能：调用ModelSimplify.exe，读入建筑群场景，进行LOD与三角面简化
		Controller插件组
			CamSimulator插件			功能：读入点云+相机文件，生成相应场景（含鹰眼）
		Convert插件组
			ive2off插件					功能：将ive、osg文件转换成off文件并显示
		PointCloud插件组
			PointCloudProcessor插件		功能：读入点云数据，进行拖拽，点选等操作
			PointCloudClassifyReader插件	功能：读入点云分类文件并显示
			PointCloudLODGenerator插件	功能：通过kdtree方式，生成点云lod
		ViewerFrameworkImpl
			默认ViewerFramework的实现