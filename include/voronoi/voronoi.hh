#ifndef __VORONOI_H__
#define	__VORONOI_H__

#include <algorithm>
#include <list>
#include <queue>
#include <dcel/dcel.hh>
#include "face.hh"
#include "point.hh"
#include "rbtree.hh"
#include "status.hh"

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

