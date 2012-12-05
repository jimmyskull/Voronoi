#ifndef __STATUS_H__
#define __STATUS_H__

namespace voronoi {

struct Point;

class Status {
public:
	Point* i;
	Point* j;
	Point* arc;

	Status();
	Status(const Status& status);
	Status(Point* arc);
	Status(Point* i, Point* j);
	~Status();

	bool operator <(const Status& b) const;
	bool operator >(const Status& b) const;
	bool operator ==(const Status& b) const;

	std::string str() const;

	Point* start() const;
	Point* circle_event() const;
	unsigned int face() const;

	void set_start(Point* start);
	void set_circle_event(Point* event);
	void set_face(unsigned int face);
	bool hasCircleEvent() const;
	bool hasFace() const;
	void RemoveCircleEvent();

private:
	Point* _circle_event;
	Point* _start; /**< Edge start point */
	unsigned int _face;
};

}

#endif 
