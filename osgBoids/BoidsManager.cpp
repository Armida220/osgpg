#include "BoidsManager.h"
#include "Boid.h"

#include <osgDB/ReadFile>

using namespace osgMA;

BoidsManager::BoidsManager(void)
{
	_root = new osg::Group;
	_frames = 0;
}

BoidsManager::~BoidsManager(void)
{
}

bool BoidsManager::Frame()
{
	//Draw_Boids();
	Move_Boids();
	++_frames;
	cout<<"Frame "<<_frames<<endl;
	return true;
}

bool BoidsManager::Init_positions()
{
	cout<<"Init"<<endl;
	osg::ref_ptr<osg::Node> model = osgDB::readNodeFile(_modelName);
	cout<<model->getBound().radius()<<endl;
	system("pause");

	for(int i=0; i<20; i++)
	{
		osg::ref_ptr<Boid> boid = new Boid;
		boid->addChild(model.get());
		_boids.push_back(boid);

		_root->addChild(boid.get());
	}
	return true;
}

void BoidsManager::Draw_Boids()
{
	//TODO
}

void BoidsManager::Move_Boids()
{
	Vector3 pos_bias, chilling, vel_bias;
	
	Vector3 aver_pos, aver_vel;
	aver_pos.set(0,0,0);
	aver_vel.set(0,0,0);
	unsigned int num = _boids.size();
	for(unsigned int i=0; i<num; i++)
	{
		Boid* boid = _boids[i].get();
		aver_pos += boid->GetPos();
		aver_vel += boid->GetVel();
	}

	for(unsigned int i=0; i<num; i++)
	{
		Boid* boid = _boids[i].get();

		//Rule 1: Boids try to fly towards the centre of mass of neighbouring boids. 
		pos_bias = ( (aver_pos - boid->GetPos())/(num-1) - boid->GetPos() ) / 100;

		//Rule 2: Boids try to keep a small distance away from other objects (including other boids). 
		chilling = Separation(boid->GetPos()) / 2;

		//Rule 3: Boids try to match velocity with near boids.
		vel_bias = ( (aver_vel - boid->GetVel())/(num-1) - boid->GetVel() ) / 8;

		Vector3 v = boid->GetVel();
		v += pos_bias + vel_bias + chilling;

		float m = max(fabs(v.x()), fabs(v.y()));
		m = max(m, fabs(v.z()));
		if(m>100)
			v *= (100/m);

		boid->SetVel(v);

		Vector3 p = boid->GetPos();
		p += v;
		boid->SetPos(p);

		boid->BoundPosition();

		boid->UpdateMatrix();
	}
}

Vector3 BoidsManager::Separation(Vector3 pos)
{
	Vector3 c;
	c.set(0,0,0);

	unsigned int num = _boids.size();
	for(unsigned int i=0; i<num; i++)
	{
		Boid* boid = _boids[i].get();

		Vector3 p = boid->GetPos();
		Vector3 diff = p-pos;
		
		float m = max(fabs(diff.x()), fabs(diff.y()));
		m = max(m, fabs(diff.z()));
		
		if(m<100)
			c -= diff;
	}

	return c;
}