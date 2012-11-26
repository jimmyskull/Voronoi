#ifndef __VORONOI_H__
#define	__VORONOI_H__

#include <list>
#include <queue>
#include <dcel/dcel.hh>
#include "geometry.hh"
#include "rbtree.hh"

namespace voronoi {

class Voronoi {
public:
	Voronoi(std::list<Point*>&);
	Voronoi(const Voronoi&);
	virtual ~Voronoi();
private:
	std::priority_queue<Point*> queue;
	std::list<Point*> sites;
	dcel::DCEL<Point*, Point*, FaceData*> dcel;
	RBTree tree;

	void HandleSiteEvent(Point* p);
	void HandleCircleEvent(Point* p);
};

}

#endif	/* __VORONOI_H__ */

