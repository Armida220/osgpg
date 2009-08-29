#ifndef __FISH_EYE_IMAGE_API_INCLUDE__
#define __FISH_EYE_IMAGE_API_INCLUDE__ 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
// OpenCV
#include <cxcore.h>
#include <cv.h>
#include <highgui.h>
#include <cvaux.h>
#include <vector>
#include "Geometry.h"
#include "RansacEstimator.h"
#include "..\ImgInterface.h"
using namespace std;

typedef vector<Point2D> Point2DArr;

#define IF_A_THEN_B(a, b) ( (a) && ((b), true) )

void FindCutPoint(GrayInterface& gi, 
									int xa, int ya, int xb, int yb,
									int& x1, int& y1, int& x2, int& y2,
									uchar thresh_grad=5, uchar thresh_gray=50)
{
	uchar T = thresh_grad;
	uchar T2 = thresh_gray;
	double dx = xa-xb, dy = ya-yb;
	if(abs(dx)>abs(dy)) {//scan X axis
		if(xa>xb) {
			swap(xa, xb);
			swap(ya, yb);
			dx=-dx; dy=-dy;
		}
		x1=xa,y1=ya;
		uchar gray,old;
		old=gray=gi[ya][xa];
		for(x1=xa+1; x1<=xb; ++x1)
		{
			if(abs((int)gray-(int)old)>T || gray>T2) {	break; }

			y1 = (x1-xb)*dy/dx + yb;
			old=gray;
			gray=gi[y1][x1];
		}

		x2=xb,y2=yb;
		old=gray=gi[yb][xb];
		for(x2=xb-1; x2>=xa; --x2)
		{
			if(abs((int)gray-(int)old)>T || gray>T2) {	break; }

			y2 = (x2-xa)*dy/dx + ya;
			old=gray;
			gray=gi[y2][x2];
		}
	} else {//scan Y axis
		if(ya>yb) {
			swap(xa, xb);
			swap(ya, yb);
			dx=-dx; dy=-dy;
		}
		x1=xa,y1=ya;
		uchar gray,old;
		old=gray=gi[ya][xa];
		for(y1=ya+1; y1<=yb; ++y1)
		{
			if(abs((int)gray-(int)old)>T || gray>T2) {	break; }

			x1 = (y1-yb)*dx/dy + xb;
			old=gray;
			gray=gi[y1][x1];
		}

		x2=xb,y2=yb;
		old=gray=gi[yb][xb];
		for(y2=yb-1; y2>=ya; --y2)
		{
			if(abs((int)gray-(int)old)>T || gray>T2) {	break; }

			x2 = (y2-ya)*dx/dy + xa;
			old=gray;
			gray=gi[y2][x2];
		}
	}
}

void FindCirclePoint(IplImage* img, IplImage* gray,
										 double& x0, double& y0, double& r,
										 bool drawLabel=true)
{
	GrayInterface gi(gray);

	int n=24;
	Point2DArr pa;

	int H = gi->height;
	int W = gi->width;
	for(int i=0; i<n; i++)
	{
		//left-right
		int yl = rand()%H;
		int yr = rand()%H;

		int x1,y1,x2,y2;
		FindCutPoint(gi,
			0, yl, W-1, yr,
			x1,y1,x2,y2);

		IF_A_THEN_B(drawLabel, 
			cvLine(img, cvPoint(0,yl), cvPoint(W-1, yr), CV_RGB(0,0,255)) );
		IF_A_THEN_B(drawLabel,
			cvCircle(img, cvPoint(x1,y1), 3, CV_RGB(255,0,0), -1) );
		IF_A_THEN_B(drawLabel,
			cvCircle(img, cvPoint(x2,y2), 3, CV_RGB(0,255,0), -1) );

		pa.push_back(Point2D(x1,y1));
		pa.push_back(Point2D(x2,y2));
		//up-down
		int xu = rand()%W;
		int xd = rand()%W;

		FindCutPoint(gi,
			xu, 0, xd, H-1,
			x1, y1, x2, y2);

		IF_A_THEN_B(drawLabel,
			cvLine(img, cvPoint(xu,0), cvPoint(xd, H-1), CV_RGB(0,0,255)) );
		IF_A_THEN_B(drawLabel,
			cvCircle(img, cvPoint(x1,y1), 3, CV_RGB(255,255,0), -1) );
		IF_A_THEN_B(drawLabel,
			cvCircle(img, cvPoint(x2,y2), 3, CV_RGB(0,255,255), -1) );
		pa.push_back(Point2D(x1,y1));
		pa.push_back(Point2D(x2,y2));
	}

	Ransac::ObserveDataSet ods;
	ods.pts = pa;
	Ransac::CircleModel cm;
	Ransac::CircleRansacor cr;
	cm = cr.Estimate(ods, 3, 0.02);
	x0 = cm.center.x;
	y0 = cm.center.y;
	r = cm.r;

	IF_A_THEN_B(drawLabel, 
		cvCircle(img, cvPoint(x0,y0), 5, CV_RGB(255,0,0), -1) );
	IF_A_THEN_B(drawLabel, 
		cvCircle(img, cvPoint(x0,y0), r, CV_RGB(255,255,255)) );
}

void GetFishEyePhotoParas(RgbInterface& img, 
													double& x0, double& y0, double& r,
													bool drawLabel=true)
{
	IplImage* gray = cvCreateImage( cvGetSize(img.get()), 8, 1 );
	cvCvtColor(img.get(), gray, CV_BGR2GRAY );
	cvSmooth( gray, gray, CV_GAUSSIAN, 9, 9 );

	FindCirclePoint(img.get(), gray, x0, y0, r, drawLabel);

	//cvSaveImage("d:\\ttest.jpg", img.get());

	SAFE_RELEASE_IMAGE(gray);
	printf("%.3lf %.3lf %.3lf\n", x0, y0, r);
}

#endif