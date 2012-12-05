/**
 *  @file
 *  @brief 
 *  @date 04/12/2012
 *  @author Paulo Urio
 *  @copyright FreeBSD License
 */

#include <algorithm>
#include <iostream>
#include <limits>
#include <sstream>
#include <cmath>
#include <voronoi/point.hh>

namespace voronoi {

static Point GetCircleCenter(const Point* a, const Point* b, const Point* c)
{
	double y_delta_a = b->y() - a->y();
	double x_delta_a = b->x() - a->x();
	double y_delta_b = c->y() - b->y();
	double x_delta_b = c->x() - b->x();

	double a_slope = y_delta_a / x_delta_a;
	double b_slope = y_delta_b / x_delta_b;

	double center_x, center_y;

	center_x = (a_slope * b_slope * (a->y() - c->y()) + b_slope * (a->x() + b->x())
			- a_slope * (b->x() + c->x())) / (2 * (b_slope - a_slope));
	center_y = -1 * (center_x - (a->x() + b->x()) / 2) / a_slope
			+ (a->y() + b->y()) / 2;

	std::cerr << "Círculo " << a->str()
					<< ", " << b->str() << ", "
					<< c->str() << " = "
					<< Point(center_x, center_y).str() << "\n";

	return Point(center_x, center_y);
}

template <class T>
class RBTreeNode;

Point::Point() :
		_x(0.0), _y(0.0), _id(0), _false_alarm(false), _face(-1U),
				_lowest_circle_parabola_node(NULL)
{
}

Point::Point(const Point& p) :
		_false_alarm(p.isFalseAlarm()), _face(-1U)
{
	set_coordinates(p.x(), p.y());
	set_id(p.id());
	set_circle_lowest_circle_parabola_node(p.lowest_circle_parabola_node());
}

Point::Point(double x, double y) :
		_id(0), _false_alarm(false), _face(-1U),
				_lowest_circle_parabola_node(NULL)
{
	set_coordinates(x, y);
}

void Point::set_id(int id)
{
	_id = id;
}

void Point::set_coordinates(double x, double y)
{
	set_x(x);
	set_y(y);
}

void Point::set_x(double x)
{
	_x = x;
}

void Point::set_y(double y)
{
	_y = y;
}

double Point::x() const
{
	return _x;
}

double Point::y() const
{
	return _y;
}

int Point::id() const
{
	return _id;
}

bool Point::operator <(const Point& b) const
{
	double abs_a = std::fabs(y());
	double abs_b = std::fabs(b.y());
	double greater = abs_a < abs_b? abs_b : abs_a;

	return (b.y() - y()) > greater * std::numeric_limits<double>::epsilon();
}

bool Point::operator >(const Point& b) const
{
	return y() > b.y();
}

bool Point::operator ==(const Point& b) const
{
	return x() == b.x() && y() == b.y();
}

void Point::set_face(unsigned int face)
{
	_face = face;
}

unsigned int Point::face() const
{
	return _face;
}

void Point::SetCoordinatesToTheCircleCenter(const Point* a, const Point* b,
		const Point* c)
{
	Point d = GetCircleCenter(a, b, c);

	set_coordinates(d.x(), d.y());
}

void Point::SetCoordinatesToTheCircleBottom(const Point* a, const Point* b,
		const Point* c)
{
	Point d = GetCircleCenter(a, b, c);

	double dx = d.x() - a->x();
	double dy = d.y() - a->y();

	double radius = std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));

	set_coordinates(d.x(), d.y() - radius);
}

RBTreeNode<Status>* Point::lowest_circle_parabola_node() const
{
	return _lowest_circle_parabola_node;
}

void Point::set_circle_lowest_circle_parabola_node(RBTreeNode<Status>* node)
{
	_lowest_circle_parabola_node = node;
}

bool Point::isCircleEvent() const
{
	return lowest_circle_parabola_node() != NULL;
}

bool Point::isFalseAlarm() const
{
	return _false_alarm;
}

void Point::MarkAsFalseAlarm()
{
	_false_alarm = true;
}

double Point::GetYOfParabolaInsersection(Point* other) const
{
	double dp = 2. * (y() - other->y());
	double a1 = 1. / dp;
	double b1 = -2. * x() / dp;
	double c1 = other->y() + dp / 4. + std::pow(x(), 2) / dp;

	return a1 * std::pow(other->x(), 2) + b1 * other->x() + c1;
}

/* http://blog.ivank.net/fortunes-algorithm-and-implementation.html */
double Point::GetXOfParabolaIntersection(const Point* q, double sweep_y) const
{
	/* p's (current) parabola */
	double dp = 2.0 * (y() - sweep_y);
	double a1 = 1.0 / dp;
	double b1 = -2.0 * x() / dp;
	double c1 = sweep_y + dp / 4 + std::pow(x(), 2) / dp;

	/* q's parabola */
	dp = 2.0 * (q->y() - sweep_y);
	double a2 = 1.0 / dp;
	double b2 = -2.0 * q->x() / dp;
	double c2 = sweep_y + dp / 4 + std::pow(q->x(), 2) / dp;

	double a = a1 - a2;
	double b = b1 - b2;
	double c = c1 - c2;

	double disc = b * b - 4 * a * c;
	double x1 = (-b + std::sqrt(disc)) / (2 * a);
	double x2 = (-b - std::sqrt(disc)) / (2 * a);

	if (y() >= q->y())
		return std::min(x1, x2);
	return std::max(x1, x2);
}

std::string Point::str() const
{
	std::stringstream ss;

	ss << "(" << x() * 2  << "," << y() * 2 << ")";
	return ss.str();
}

}
