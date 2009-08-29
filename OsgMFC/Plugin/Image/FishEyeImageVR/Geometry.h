
#pragma once

typedef double DataType;

struct Point2D {
		DataType x,y;

		Point2D(DataType _x=0, DataType _y=0)
		{
			x=_x; y=_y;
		}

		DataType SquareDistanceTo(const Point2D& rhs) const
		{
			return (x-rhs.x)*(x-rhs.x) + (y-rhs.y)*(y-rhs.y);
		}

		DataType DistanceTo(const Point2D& rhs) const
		{
			return sqrt( (x-rhs.x)*(x-rhs.x) + (y-rhs.y)*(y-rhs.y) );
		}
};
