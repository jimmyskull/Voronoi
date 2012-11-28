#ifndef __VORONOI_H__
#define	__VORONOI_H__

#include <algorithm>
#include <list>
#include <queue>
#include <dcel/dcel.hh>
#include "voronoiface.hh"
#include "point.hh"
#include "status.hh"
#include "voronoitree.hh"

namespace voronoi {

typedef std::priority_queue<Point*> VoronoiQueue;
typedef dcel::DCEL<Point, Point, FaceInfo> VoronoiDCEL;

class Voronoi {
public:
	Voronoi(std::list<Point*>&);
	Voronoi(const Voronoi&);
	virtual ~Voronoi();
private:
	void HandleSiteEvent(Point* p);
	void HandleCircleEvent(Point* p);

	std::list<Point*> sites;
	VoronoiQueue queue;
	VoronoiDCEL dcel;
	VoronoiTree tree;
};

}

#endif	/* __VORONOI_H__ */

