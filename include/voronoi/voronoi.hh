#ifndef __VORONOI_H__
#define	__VORONOI_H__

#include <list>
#include <queue>
#include <dcel/dcel.hh>
#include "geometry.hh"
#include "rbtree.hh"

namespace voronoi {

class FaceData {
public:
	std::string name;
};

class Status {
public:
	Point* i;
	Point* j;
	Point* arc;
};

class Voronoi {
public:
	Voronoi(std::list<Point*>&);
	Voronoi(const Voronoi&);
	virtual ~Voronoi();
private:
	std::priority_queue<Point*> queue;
	std::list<Point*> sites;
	dcel::DCEL<Point, Point, FaceData> dcel;
	RBTree<Status> tree;
};

}

#endif	/* __VORONOI_H__ */

