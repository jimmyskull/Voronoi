#ifndef __STATUS_H__
#define __STATUS_H__

#include <sstream>
#include <cassert>
#include <cmath>
#include <macros.hh>
#include "point.hh"

namespace voronoi {

class Status {
public:
	Point* i;
	Point* j;
	Point* arc;

	Status() :
			i(NULL), j(NULL), arc(NULL)
	{
	}

	Status(const Status& status) :
			i(status.i), j(status.j), arc(status.arc)
	{
	}

	Status(Point* arc) :
			i(NULL), j(NULL), arc(arc)
	{
	}

	~Status()
	{
	}

	bool operator <(const Status& b) const
	{
		assert(i != NULL);
		assert(j != NULL);
		//assert(b.arc != NULL);
		return GetXOfCircle(i, j, b.arc->y) < b.arc->x;
	}

	bool operator >(const Status& b) const
	{
		assert(i != NULL);
		assert(j != NULL);
		assert(b.arc != NULL);
		return GetXOfCircle(i, j, b.arc->y) > b.arc->x;
	}

	bool operator ==(const Status& b) const
	{
		UNUSED(b);
		return arc != NULL;
	}

	std::string str() const
	{
		std::stringstream ss;

		if (arc == NULL) {
			ss << "<(" << i->x << "," << i->y << ") (";
			ss << j->x << "," << j-> y << ")>";
		} else {
			ss << "(" << arc->x << "," << arc->y  << ")";
		}
		return ss.str();
	}
private:
	/* http://blog.ivank.net/fortunes-algorithm-and-implementation.html */
	/* Left point |a| and left point |b|. */
	static double GetXOfCircle(Point* p, Point* q, double y)
	{
		/* Centro do círculo de p */
		double dp = 2.0 * (p->y - y);
		double a1 = 1.0 / dp;
		double b1 = -2.0 * p->x / dp;
		double c1 = y + dp / 4 + p->x * p->x / dp;

		/* Centro do círculo de q */
		dp = 2.0 * (q->y - y);
		double a2 = 1.0 / dp;
		double b2 = -2.0 * q->x / dp;
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
			ry = std::max(x1, x2);
		return ry;
	}
};

}

#endif 
