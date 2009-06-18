#pragma once
#include <osg/ref_ptr>
#include <osg/Group>
#include <vector>
#include <iostream>
#include <string>

#include "Boid.h"

using namespace std;

namespace osgMA {

	typedef osg::Vec3 Vector3;

	class BoidsManager
	{
	public:
		BoidsManager(void);
		~BoidsManager(void);

	public:
		virtual bool Frame();
		virtual bool Init_positions();

		osg::ref_ptr<osg::Group> GetRoot() { return _root; }

		void SetModelName(string& name) { _modelName = name; }

	protected:
		virtual void Draw_Boids();
		virtual void Move_Boids();

		Vector3 Separation(Vector3 pos);

	protected:
		typedef vector<osg::ref_ptr<Boid>> BoidsList;
		BoidsList _boids;

		osg::ref_ptr<osg::Group> _root;
		string _modelName;

		unsigned int _frames;
	};


}

