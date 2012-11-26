#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <cmath>

namespace voronoi {

struct Point {
	Point() : x(0), y(0) {};

	Point(double x, double y) :
		x(x), y(y), site_event(true)
	{
	}

	bool operator <(const Point& b) const
	{
		return (y < b.y) || (y == b.y && x < b.x);
	}

	double x;
	double y;
	bool site_event;
};

struct FaceInfo {
	FaceInfo(int id) : id(id) {}

	int id;
};


/* Círculo formado por a, b e c.
 *                          |ax    ay   ax² + ay²   1|
 *  in_circle(a,b,c,d) = det|bx    by   bx² + by²   1|
 *                          |cx    cy   cx² + cy²   1|
 *                          |dx    dy   dx² + dy²   1|
 * in_circle == 0  se d está no círculo.
 * in_circle >  0  se d está fora do círculo.
 * in_circle <  0  se d está dentro de círculo.
 */
static inline int in_circle(const Point a, const Point b, const Point c,
		const Point d)
{
#define paraboloid(a) (a.x * a.x + a.y * a.y)
	return paraboloid(a)
			* (b.x * c.y + b.y * d.x + c.x * d.y - d.x * c.y - d.y * b.x
					- c.x * b.y)
			+ (-1) * paraboloid(b)
					* (a.x * c.y + a.y * d.x + c.x * d.y - d.x * c.y - d.y * a.x
							- c.x * a.y)
			+ paraboloid(c)
					* (a.x * b.y + a.y * d.x + b.x * d.y - d.x * b.y - d.y * a.x
							- b.x * a.y)
			+ (-1) * paraboloid(d)
					* (a.x * b.y + a.y * c.x + b.x * c.y - c.x * b.y - c.y * a.x
							- b.x * a.y);
#undef paraboloid
}

/* http://blog.ivank.net/fortunes-algorithm-and-implementation.html */
/* Left point |a| and left point |b|. */
double GetXOfEdge(Point* p, Point* q, double y)
{
	/* Centro do círculo de p */
	double dp = 2.0 * (p->y - y);
	double a1 = 1.0 / dp;
	double b1 = -2.0 * p->x / dp;
	double c1 = y + dp / 4 + p->x * p->x / dp;

	/* Centro do círculo de q */
	dp = 2.0 * (q->y - y);
	double a2 = 1.0 / dp;
	double b2 = -2.0 * q->x/dp;
	double c2 = y + dp / 4 + q->x * q->x / dp;

	double a = a1 - a2;
	double b = b1 - b2;
	double c = c1 - c2;

	double disc = b * b - 4 * a * c;
	double x1 = (-b + std::sqrt(disc)) / (2 * a);
	double x2 = (-b - std::sqrt(disc)) / (2 * a);

	double ry;
	if (p->y > q->y)
		ry = std::min(x1, x2);
	else
		ry =  std::max(x1, x2);
	return ry;
}

}

#endif /* __GEOMETRY_H__ */
