/**
 *  @file
 *  @brief 
 *  @date 04/12/2012
 *  @author Paulo Urio
 *  @copyright FreeBSD License
 */

#ifndef _QUEUE_HH_
#define _QUEUE_HH_

#include <queue>
#include <vector>
#include "point.hh"

namespace voronoi {

struct Point;
struct ComparePoint;

class VoronoiQueue: public std::priority_queue<Point*, std::vector<Point*>,
		ComparePoint> {
public:
	VoronoiQueue();
	VoronoiQueue(std::vector<Point*>& sites);
};

}

#endif /* _QUEUE_HH_ */
