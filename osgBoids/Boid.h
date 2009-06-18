#pragma once

#include <osg/MatrixTransform>

namespace osgMA {

	typedef osg::Vec3 Vector3;

	class Boid :
		public osg::MatrixTransform
	{
	public:
		Boid(void);

	public:
		Vector3 GetPos() { return _pos; }
		void		SetPos(Vector3& pos) { _pos = pos; }

		Vector3 GetVel() { return _vel; }
		void		SetVel(Vector3& vel) { _vel = vel; }

		void BoundPosition();

		void UpdateMatrix();

	protected:
		Vector3 _pos;
		Vector3 _vel;
	};

}