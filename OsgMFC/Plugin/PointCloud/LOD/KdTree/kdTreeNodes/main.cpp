#include <osg/ref_ptr>
#include <osg/Group>
#include <osgViewer/Viewer>
#include <osg/NodeVisitor>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osg/Geometry>
#include "PointSetDividor.h"
#include <fstream>
#include <iostream>

using namespace std;

/** Usage:
	appname.exe infile outfile
	*/
int main(int argc, char** argv)
{
	setlocale(LC_ALL, "chs");
	if(argc<3) {
		cout<<"need infile and outfile name"<<endl;
		return -1;
	}

	unsigned int maxLevels = 16;
	float sampleRatio = 0.05;
	unsigned int targetNumOnLeaf = 100;
	double maxBoundVisibleRange = 800;
	double minBoundVisibleRange = 100;

	if(argc>=4)
		maxLevels = atoi(argv[3]);
	if(argc>=5)
		sampleRatio = atof(argv[4]);
	if(argc>=6)
		targetNumOnLeaf = atoi(argv[5]);
	if(argc>=8)
		maxBoundVisibleRange = atof(argv[6]),
		minBoundVisibleRange = atof(argv[7]);

	try {
		FC::PointSetDividor psd(argv[1], argv[2]);
		if(!psd.Process(FC::BuildParameters(maxLevels, targetNumOnLeaf, 
			sampleRatio, maxBoundVisibleRange, minBoundVisibleRange))) {
			cout<<"process error!"<<endl;
			return -1;
		}

		osg::ref_ptr<osg::Group> root = psd.GetRoot();
		osgDB::writeNodeFile(*root, argv[2]);
	} catch (...) {
		cout<<"Error happens!"<<endl;
	}
	return 0;
}
