#include "Boid.h"

#include <iostream>
#include <math.h>

using namespace osgMA;
using namespace std;

Boid::Boid(void)
{
	_pos.set(rand()%100, rand()%100, rand()%100);
	_vel.set(rand()%100, rand()%100, rand()%100);
	
	float m = max(fabs(_vel.x()), fabs(_vel.y()));
	m = max(m, fabs(_vel.z()));
	if(m>100)
		_vel *= (100/m);

	BoundPosition();

	UpdateMatrix();
}

void Boid::UpdateMatrix()
{
	Boid& self = *this;

	osg::Matrix mat, trans, rot;
	trans = osg::Matrix::translate(self.GetPos());
	//rot = osg::Matrix::rotate(osg::X_AXIS, self.GetVel());

	mat = trans;

	self.setMatrix(mat);
}

void Boid::BoundPosition()
{
	for(int i=0; i<3; i++) 
	{
		if(_pos[i]<0)
			_pos[i] = 100;
		else if(_pos[i]>100)
			_pos[i] = 0;
	}
}