/* -*-c++-*- OpenSceneGraph - Copyright (C) Simbaforrest
 *
 * This application is open source and may be redistributed and/or modified   
 * freely and without restriction, both in commericial and non commericial applications,
 * as long as this copyright notice is maintained.
 * 
 * This application is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#include <osg/ref_ptr>
#include <osg/Group>
#include <osgViewer/Viewer>
#include <osg/NodeVisitor>		//FC
#include <osgDB/ReadFile>		//FC
#include <osgDB/WriteFile>		//FC
#include <iostream>

int main(int argc, char** argv)
{
	osg::ref_ptr<osg::Group> root = new osg::Group;
	
	osgViewer::Viewer viewer;
	viewer.setSceneData( root.get() );
	
	return viewer.run();
}
