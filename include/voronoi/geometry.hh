#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

namespace voronoi {

struct Point {
	Point();

	Point(double y, double x) :
			y(y), x(x)
	{
	}

	bool operator <(const Point& b) const
	{
		return (y < b.y) || (y == b.y && x < b.x);
	}

	double y;
	double x;
};

}

#endif /* __GEOMETRY_H__ */
