#pragma once
#include "ximage.h"
#include <osg/Vec4>

class PixelGetter
{
protected:
	CxImage img;
public:
	PixelGetter(void);
	~PixelGetter(void);

	bool Load(const char * filename);
	inline bool GetPixel(int x, int y, osg::Vec4& c)
	{
		if(x<0 || y<0 || x>=img.GetWidth() || y>=img.GetHeight()) {
			return false;
		}
		RGBQUAD rgb = img.GetPixelColor(x, img.GetHeight() - y);
		c = osg::Vec4(rgb.rgbRed/255.0, rgb.rgbGreen/255.0, rgb.rgbBlue/255.0, 1);
		return true;
	}
};
