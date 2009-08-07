#include "stdafx.h"
#include "Points2Grids.h"
#include <fstream>

bool Points2Grids::read_point()
{
	if(!infile.good())
	{
		_log << "Can not open this file in this path"<<endl;
		return false;
	}

	_log<<"Begin to read data."<<endl;

	int num,tmp;
	infile>>num;
	tmp = num;

	float min_x,min_y,min_z;
	float max_x,max_y,max_z;

	string str;
	getline(infile, str, '\n');
	double x,y,z;
	double I;
	double r,g,b;
	int flag=0;

	//try
	I = r = g = b = -1;
	getline(infile, str, '\n');
	sscanf(str.c_str(), "%lf %lf %lf %lf %lf %lf %lf", &x, &y, &z, &I, &r, &g, &b);

	{
		min_x = max_x = x;
		min_y = max_y = y;
		min_z = max_z = z;
	}

	if(I==-1) {
		flag = 0; // x y z
	} else {
		if(r==-1) {
			flag = 1; // x y z i
		} else {
			if(b==-1) {
				flag = 2; // x y z r g b
			} else {
				flag = 3; // x y z i r g b
			}
		}
	}

	_fileType = flag;

	switch(flag){
		case 0:
			_points.push_back(PointRec(x,y,z));
			break;
		case 1:
			_points.push_back(PointRec(x,y,z,I));
			break;
		case 2:
			_points.push_back(PointRec(x,y,z,-1, I, r, g));//ATTENTION!
			break;
		case 3:
			_points.push_back(PointRec(x,y,z,I,r,g,b));
			break;
	}
	--num;

	do
	{
		switch(flag){
			case 0:
				infile >> x >> y >> z;
				_points.push_back(PointRec(x,y,z));
				break;
			case 1:
				infile >> x >> y >> z >> I;
				_points.push_back(PointRec(x,y,z,I));
				break;
			case 2:
				infile >> x >> y >> z >> r >> g >> b;
				_points.push_back(PointRec(x,y,z,-1, r, g, b));
				break;
			case 3:
				infile >> x >> y >> z >> I >> r >> g >> b;
				_points.push_back(PointRec(x,y,z, I, r, g, b));
				break;
		}

		{
			min_x = min(min_x,x);
			min_y = min(min_y,y);
			min_z = min(min_z,z);

			max_x = max(max_x,x);
			max_y = max(max_y,y);
			max_z = max(max_z,z);
		}

	}while(--num);
	_log<<endl;

	infile.close();
	_log<<"All "<<_points.size()<<" datas have been read successfully!"<<endl;

	_ptInfo.min_x = min_x;
	_ptInfo.min_y = min_y;
	_ptInfo.min_z = min_z;
	_grid = pow(tmp/_grid,1.0/3)+1;//FC 2009-04-04
	_ptInfo.grid = _grid;
	_ptInfo.dx = (max_x-min_x)/_grid;
	_ptInfo.dy = (max_y-min_y)/_grid;
	_ptInfo.dz = (max_z-min_z)/_grid;

	return true;
}

void Points2Grids::put_points_to_grids()
{
	float min_x,min_y,min_z;
	min_x = _ptInfo.min_x;
	min_y = _ptInfo.min_y;
	min_z = _ptInfo.min_z;
	float dx,dy,dz;
	dx = _ptInfo.dx;
	dy = _ptInfo.dy;
	dz = _ptInfo.dz;
	int grid = _ptInfo.grid;
	int grid2 = grid*grid;
	int grid3 = (grid+1)*(grid+1)*(grid+1);

	int* flag = new int[grid3];
	memset(flag, 0, sizeof(flag[0])*grid3);

	int top=0;
	for(vector<PointRec>::iterator ptItr = _points.begin();
		ptItr != _points.end(); ++ptItr)
	{
		float x,y,z;
		PointRec& p = *ptItr;
		x = p.x;
		y = p.y;
		z = p.z;
		
		//int id = int((x-min_x)/dx) + int((y-min_y)/dy)*grid + int((z-min_z)/dz)*grid2;
		int A = dx==0?0:int((x-min_x)/dx);
		int B = dy==0?0:int((y-min_y)/dy)*grid;
		int C = dz==0?0:int((z-min_z)/dz)*grid2;
		int id = A+B+C;

		if(flag[id]==0)
		{
			top++;
			vector<PointRec> pvnew;
			_grids.push_back(pvnew);
			flag[id] = top;
		}
		vector<PointRec>& pv = _grids[ flag[id]-1 ];

		pv.push_back(p);
	}

	delete [] flag;
}

osg::ref_ptr<osg::Group> Points2Grids::makeGridsIntoNodes()
{
	osg::ref_ptr<osg::Group> ret = new osg::Group;

	for(vector< vector<PointRec> >::iterator itr = _grids.begin();
		itr!=_grids.end(); ++itr)
	{
		ret->addChild( makePointsInOneNode( (*itr) ) );
	}

	return ret;
}

osg::Geode* Points2Grids::makePointsInOneNode(vector<PointRec>& points)
{
	osg::Geode *geode = new osg::Geode;
	osg::Geometry *galaxy = new osg::Geometry;
	osg::Vec4Array *colorsArr = new osg::Vec4Array;
	osg::Vec3Array *pointsArr = new osg::Vec3Array;
	int nvertices = points.size();

	galaxy->setVertexArray(pointsArr);
	galaxy->setColorArray(colorsArr);
	
	switch(_fileType) {
		case 0://x y z
			{
				osg::Vec4 ini(1,1,0,0);
				colorsArr->push_back(ini);
				galaxy->setColorBinding(osg::Geometry::BIND_OVERALL);
				//galaxy->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

				for(unsigned int i=0; i<points.size(); i++) {
					PointRec& p = points[i];
					pointsArr->push_back(osg::Vec3(p.x, p.y, p.z));

					//double c = rand()%256/255.0;
					//colorsArr->push_back(osg::Vec4(c, c, c, 0));
				}
			}
			break;
		case 1://x y z i
			{
				galaxy->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

				for(unsigned int i=0; i<points.size(); i++) {
					PointRec& p = points[i];
					pointsArr->push_back(osg::Vec3(p.x, p.y, p.z));

					double c = p.i/255.0;
					colorsArr->push_back(osg::Vec4(c, c, c, 0));
				}
			}
			break;
		case 2://x y z r g b
		case 3://x y z i r g b
			{
				galaxy->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

				for(unsigned int i=0; i<points.size(); i++) {
					PointRec& p = points[i];
					pointsArr->push_back(osg::Vec3(p.x, p.y, p.z));

					double r=p.r;
					double g=p.g;
					double b=p.b;
					if(r>1 || g>1 || b>1) {
						r/=255; g/=255; b/=255;
					}
					colorsArr->push_back(osg::Vec4(r, g, b, 0));
				}
			}
			break;
	}

	galaxy->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS, 
		0, nvertices));
	geode->addDrawable(galaxy);
	geode->setInitialBound( geode->getBound() );
	
  return geode;
}

osg::StateSet* Points2Grids::makeStateSet(float size)
{
	osg::StateSet *set = new osg::StateSet();

	set->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	return set;
}

bool Points2Grids::process()
{
	//read error
	if( !read_point() )
		return false;

	put_points_to_grids();

	_root = makeGridsIntoNodes();

	_root->setStateSet(makeStateSet(5.0f));	

	return true;
}

//////////////////////////////////////////////////////////////////////////
//下面这段代码可以从geode中的drawable内获得各点坐标数据
/*
osg::Array* nv = geode->getDrawable(0)->asGeometry()->getVertexArray();
osg::Vec3Array* v3a = dynamic_cast<osg::Vec3Array*>(nv);
int num=v3a->size();
_log<<num<<endl;
for(osg::Vec3Array::iterator itr=v3a->begin(); itr!=v3a->end(); ++itr)
{
osg::Vec3 v3 = *itr;
float x=v3[0];
float y=v3[1];
float z=v3[2];
_log<<x<<" "<<y<<" "<<z<<endl;
system("pause");
}*/

//////////////////////////////////////////////////////////////////////////
/// Setup cool blending
/*
set->setMode(GL_BLEND, osg::StateAttribute::ON);
osg::BlendFunc *fn = new osg::BlendFunc();
fn->setFunction(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::DST_ALPHA);
set->setAttributeAndModes(fn, osg::StateAttribute::ON);

/// Setup the point sprites
osg::PointSprite *sprite = new osg::PointSprite();
set->setTextureAttributeAndModes(0, sprite, osg::StateAttribute::ON);

/// Give some size to the points to be able to see the sprite
osg::Point *point = new osg::Point();
point->setSize(size);
set->setAttribute(point);
*/

/// Disable depth test to avoid sort problems and Lighting
//set->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);

/// The texture for the sprites
/*
osg::Texture2D *tex = new osg::Texture2D();
tex->setImage(osgDB::readImageFile("Images/particle.rgb"));
set->setTextureAttributeAndModes(0, tex, osg::StateAttribute::ON);
*/