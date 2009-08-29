#ifndef __IMG_INTERFACE_INCLUDE__
#define __IMG_INTERFACE_INCLUDE__ 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
// OpenCV
#include <cxcore.h>
#include <cv.h>
#include <highgui.h>
#include <cvaux.h>
#include <list>
using namespace std;

//#define PI 3.14159265358979323846264338327950
#define SAFE_RELEASE_IMAGE(p) if(p) { cvReleaseImage(&(p)); p=0; }

#define B 0
#define G 1
#define R 2
typedef uchar RgbPixel[3];
typedef uchar GrayPixel;

template<class T, class IT> struct ImgInterface
{
private:
	IT* p;
public:
	explicit ImgInterface(IT* _p) : p(_p) {}
	~ImgInterface() { p=0; }
	void operator=(IT* _p) { p = _p; }

	IT* get() { return p; }
	bool valid() { return p!=0; }

	inline T* operator[] (const int rowIndex) {
		return ( (T*)(p->imageData+rowIndex*p->widthStep) );
	}

	inline IT* operator->() { return p; }
};
typedef ImgInterface<RgbPixel, IplImage> RgbInterface;
typedef ImgInterface<RgbPixel, const IplImage> ConstRgbInterface;
typedef ImgInterface<GrayPixel, IplImage> GrayInterface;

//safe box for IplImage
//once destructing, all image registerd will be released
struct ImgFactory
{
	typedef list<IplImage*> ImgLists;
	ImgLists imgLists;

	ImgFactory() {}
	~ImgFactory()
	{
		for(ImgLists::iterator itr=imgLists.begin();
			itr!=imgLists.end(); ++itr) {
			IplImage* p = (*itr);
			SAFE_RELEASE_IMAGE(p);
		}
	}

	void RegisterImage(IplImage* p)
	{
		if(!p) return;
		ImgLists::iterator itr = 
			std::find(imgLists.begin(), imgLists.end(), p);
		if(itr==imgLists.end()) {
			imgLists.push_back(p);
		}
	}

	RgbInterface LoadImage(const char* name, bool doRegister=true)
	{
		IplImage* p = 0;
		p = cvLoadImage(name);
		if(doRegister) RegisterImage(p);
		return RgbInterface(p);
	}

	void DestroyImage(RgbInterface& img)
	{
		ImgLists::iterator itr = 
			std::find(imgLists.begin(), imgLists.end(), img.get());
		if(itr!=imgLists.end()) {
			IplImage* p = *itr;
			SAFE_RELEASE_IMAGE(p);
			imgLists.erase(itr);
		}
	}

	GrayInterface ConvertRGB2Gray(RgbInterface& rgb, bool doRegister=true)
	{
		IplImage* p = 
			cvCreateImage(cvGetSize(rgb.get()), 8, 1);
		cvCvtColor(rgb.get(), p, CV_BGR2GRAY);
		if(doRegister) RegisterImage(p);
		return GrayInterface(p);
	}
};

#endif