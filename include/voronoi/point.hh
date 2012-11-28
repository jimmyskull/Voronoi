/**
 *  @file
 *  @brief 
 *  @date 28/11/2012
 *  @author Paulo Urio
 *  @copyright FreeBSD License
 */

#ifndef _POINT_HH_
#define _POINT_HH_

#include <cstddef>

namespace voronoi {

struct Point {
	Point() :
			x(0), y(0)
	{
	}

	Point(double x, double y) :
			/*_circle_event(NULL),*/ id(0)
	{
		set_coordinates(x, y);
	}

	void set_id(int id)
	{
		this->id = id;
	}

	void set_coordinates(double new_x, double new_y)
	{
		this->x = new_x;
		this->y = new_y;
	}

	bool operator <(const Point& b) const
	{
		return (y < b.y) || (y == b.y && x < b.x);
	}

	/*VoronoiTree::Node* circle_event() const
	{
		return _circle_event;
	}

	void set_circle_event(Point* event)
	{
		_circle_event = event;
	}*/

	bool hasCircleEvent() const
	{
		return false;//circle_event() != NULL;
	}

	/* Círculo formado por a, b e c.
	 *                          |ax    ay   ax² + ay²   1|
	 *  in_circle(a,b,c,d) = det|bx    by   bx² + by²   1|
	 *                          |cx    cy   cx² + cy²   1|
	 *                          |dx    dy   dx² + dy²   1|
	 * in_circle == 0  se d está no círculo.
	 * in_circle >  0  se d está fora do círculo.
	 * in_circle <  0  se d está dentro de círculo.
	 */
	static int in_circle(const Point a, const Point b, const Point c,
			const Point d)
	{
#define paraboloid(a) (a.x * a.x + a.y * a.y)
		return paraboloid(a)
				* (b.x * c.y + b.y * d.x + c.x * d.y - d.x * c.y - d.y * b.x
						- c.x * b.y)
				+ (-1) * paraboloid(b)
						* (a.x * c.y + a.y * d.x + c.x * d.y - d.x * c.y
								- d.y * a.x - c.x * a.y)
				+ paraboloid(c)
						* (a.x * b.y + a.y * d.x + b.x * d.y - d.x * b.y
								- d.y * a.x - b.x * a.y)
				+ (-1) * paraboloid(d)
						* (a.x * b.y + a.y * c.x + b.x * c.y - c.x * b.y
								- c.y * a.x - b.x * a.y);
#undef paraboloid
	}

	double x;
	double y;
	int id;
private:
	//VoronoiTree::Node* _circle_event;
};


}

#endif /* _POINT_HH_ */
