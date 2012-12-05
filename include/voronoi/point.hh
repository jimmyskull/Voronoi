/**
 *  @file
 *  @brief 
 *  @date 28/11/2012
 *  @author Paulo Urio
 *  @copyright FreeBSD License
 */

#ifndef _POINT_HH_
#define _POINT_HH_

#include <functional>
#include <string>

namespace voronoi {

template <class T>
class RBTreeNode;

class Status;

struct Point {
public:
	Point();
	Point(const Point& p);
	Point(double x, double y);

	void set_id(int id);
	void set_coordinates(double x, double y);
	void set_x(double x);
	void set_y(double y);
	void set_face(unsigned int face);
	void set_circle_lowest_circle_parabola(RBTreeNode<Status>* node);

	double x() const;
	double y() const;
	int id() const;
	unsigned int face() const;
	RBTreeNode<Status>* lowest_circle_parabola() const;

	bool operator <(const Point& b) const;
	bool operator >(const Point& b) const;
	bool operator ==(const Point& b) const;

	double GetXOfCircle(const Point* q, double sweep_y) const;
	double GetParabolaY(Point* other) const;

	void SetCoordinatesToTheCircleCenter(const Point* a, const Point* b,
			const Point* c);
	void SetCoordinatesToTheCircleBottom(const Point* a, const Point* b,
			const Point* c);

	bool isCircleEvent() const;
	bool isFalseAlarm() const;
	void MarkAsFalseAlarm();

	std::string str() const;

private:
	double _x;
	double _y;
	int _id;
	bool _false_alarm;
	unsigned int _face;
	RBTreeNode<Status>* _lowest_circle_parabola;
};

struct ComparePoint: public std::binary_function<Point*, Point*, bool> {
	bool operator ()(const Point* a, const Point* b) const
	{
		return *a < *b;
	}
};

}

#endif /* _POINT_HH_ */
