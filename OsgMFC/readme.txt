OsgFramework����˼·��
	
	���������Ϊ���󲿷�
		OsgFrameWork
		Plugins
	�����ò����ʽ�Գ����ܽ�����չ���Ӷ�����������OsgFrameWork�ڲ������������������ڽ���������ģ�黯��
	
	OsgFrameWork������ͨ��PluginManager��������
	ÿһ���ĵ����������һ��PluginManager����ÿһ����ͼ���������һ��ViewerFramework����
	
	Plugin��ʵ�ַ�ʽ���£�
		��Plugin��Ϊ��pluginΪ��׺����dll�ļ���
		�κ�plugin���̳���FC::WorkFlowPluginBase��FC::ControllerPluginBase��
		���̳���WorkFlowPluginBase�ı������ʵ�ֺ���
		bool CreateWorkFlowPlugin(void ** pobj)
		{
			*pobj = new CPlugA;	//CPlugA�̳���WorkFlowPluginBase
			return *pobj != NULL;
		}
		���Ƶģ����̳���ControllerPluginBase�ı������ʵ�ֺ���CreateControllerPlugin
		
	PluginĿǰ��Ҫ��Ϊ���֣������Ը�����Ҫ������µĿ��������࣬������Ҫ��Ӧ�ĸ���������OsgFrameWork��
		WorkFlowPlugin		������һ��������
							���﹤����ָ���ǣ�
								1.����ViewerFrameworkImpl����Ҫ������ʵ�ֲ�ͬ��osgViewer::compositeView
								  ��һ��������ؿ��Բο�Camsimulator�����ʵ��
								2.Ϊviewer�Ĳ�ͬview�ֱ���ӳ������ݣ�SetSceneData����Ŀǰ�󲿷ֲ����Ҫ
								  ʵ����һ���ֹ��ܵ�����
								3.Ϊ��ͬ��view�ֱ�����GUIEventHandler
		ControllerPlugin	�����ڳ�������֮������µĿ��Ƴ����ķ�ʽ������Ĭ�ϳ������Ʒ�ʽ��TrackBall��
							��������ο����ߴ������µĿ�����A���Ϳ����ڳ������غ���������
	
	Ŀǰ����Ĺ��ܽ��ܣ�
		Building�����
			BuildingSimplifier���		���ܣ�����ModelSimplify.exe�����뽨��Ⱥ����������LOD���������
		Controller�����
			CamSimulator���			���ܣ��������+����ļ���������Ӧ��������ӥ�ۣ�
		Convert�����
			ive2off���					���ܣ���ive��osg�ļ�ת����off�ļ�����ʾ
		PointCloud�����
			PointCloudProcessor���		���ܣ�����������ݣ�������ק����ѡ�Ȳ���
			PointCloudClassifyReader���	���ܣ�������Ʒ����ļ�����ʾ
			PointCloudLODGenerator���	���ܣ�ͨ��kdtree��ʽ�����ɵ���lod
		ViewerFrameworkImpl
			Ĭ��ViewerFramework��ʵ��