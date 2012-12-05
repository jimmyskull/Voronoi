#ifndef __VORONOI_H__
#define	__VORONOI_H__

#include <algorithm>
#include <list>
#include "diagram.hh"
#include "face.hh"
#include "point.hh"
#include "queue.hh"
#include "status.hh"
#include "tree.hh"

namespace voronoi {

class VoronoiQueue;
class VoronoiDCEL;

class Voronoi {
public:
	Voronoi(std::list<Point*>&);
	Voronoi(const Voronoi&);
	virtual ~Voronoi();

	std::list<Point*> sites;
	VoronoiQueue queue;
	VoronoiDCEL dcel;
	VoronoiTree tree;
private:
	void HandleSiteEvent(Point* p);
	void HandleCircleEvent(Point* p);
};

}

#endif	/* __VORONOI_H__ */

