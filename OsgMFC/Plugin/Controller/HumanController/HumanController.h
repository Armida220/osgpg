#ifdef HUMANCONTROLLER_EXPORTS
#define HUMANCONTROLLER_API __declspec(dllexport)
#else
#define HUMANCONTROLLER_API __declspec(dllimport)
#endif

#include "ControllerPluginBase.h"
#include <osgGA/TrackballManipulator>
using namespace std;

namespace FC {
	class HUMANCONTROLLER_API HumanControllerCreator :
		public ControllerPluginBase{	
	public:
		virtual Controller* CreateController();
		virtual void Release();
	};

	class HumanController : public osgGA::TrackballManipulator
	{
		osg::Vec3 _eye;
		osg::Vec3 _up;
		osg::Vec3 _left;
		double _velocity;
	public:
		HumanController() { 
			_eye.set(0,0,0); _up.set(0,0,1); _left.set(0,1,0);
			_velocity = 0.5;
		}
		virtual bool handle(const osgGA::GUIEventAdapter&, osgGA::GUIActionAdapter&);
		virtual void setByMatrix(const osg::Matrixd& matrix);
		virtual void setByInverseMatrix(const osg::Matrixd& matrix);
		virtual osg::Matrixd getMatrix() const;
		virtual osg::Matrixd getInverseMatrix() const;
	};
}

extern "C" {
	HUMANCONTROLLER_API bool CreateControllerPlugin(void ** pobj);
}