/**
 *  @file
 *  @brief 
 *  @date 04/12/2012
 *  @author Paulo Urio
 *  @copyright FreeBSD License
 */

#include <sstream>
#include <cassert>
#include <cstddef>
#include <voronoi/status.hh>
#include <voronoi/point.hh>

namespace voronoi {

Status::Status() :
		i(NULL), j(NULL), arc(NULL), _circle_event(NULL), _start(NULL)
{
}

Status::Status(const Status& status) :
		i(status.i), j(status.j), arc(status.arc),
				_circle_event(status.circle_event()), _start(status.start())
{
}

Status::Status(Point* arc) :
		i(NULL), j(NULL), arc(arc), _circle_event(NULL), _start(NULL)
{
}

Status::Status(Point* i, Point* j) :
		i(i), j(j), arc(NULL), _circle_event(NULL), _start(NULL)
{
}

Status::~Status()
{
}

bool Status::operator <(const Status& b) const
{
	assert(i != NULL);
	assert(j != NULL);
	assert(b.arc != NULL);
	return b.arc->x() < i->GetXOfParabolaIntersection(j, b.arc->y());
}

bool Status::operator >(const Status& b) const
{
	assert(i != NULL);
	assert(j != NULL);
	assert(b.arc != NULL);
	return b.arc->x() > i->GetXOfParabolaIntersection(j, b.arc->y());
}

bool Status::operator ==(const Status& b) const
{
	assert(i == NULL);
	assert(b.i == NULL);
	return *arc == *b.arc;
}

std::string Status::str() const
{
	std::stringstream ss;

	if (arc == NULL) {
		ss << "<" << i->str() << " " << j->str() << ">";
		return ss.str();
	}
	return arc->str();
}

Point* Status::start() const
{
	return _start;
}

void Status::set_start(Point* start)
{
	_start = start;
}

Point* Status::circle_event() const
{
	return _circle_event;
}

void Status::set_circle_event(Point* event)
{
	_circle_event = event;
}

bool Status::hasCircleEvent() const
{
	return circle_event() != NULL;
}

void Status::RemoveCircleEvent()
{
	if (hasCircleEvent()) {
		circle_event()->MarkAsFalseAlarm();
		set_circle_event(NULL);
	}
}

unsigned int Status::face() const
{
	return _face;
}


void Status::set_face(unsigned int face)
{
	_face = face;
}

bool Status::hasFace() const
{
	return face() != -1U;
}

}
