/**
 *  @file
 *  @brief 
 *  @date 05/12/2012
 *  @author Paulo Urio
 *  @copyright FreeBSD License
 */

#include <voronoi/queue.hh>
#include <voronoi/point.hh>

namespace voronoi {

VoronoiQueue::VoronoiQueue()
{

}

VoronoiQueue::VoronoiQueue(std::vector<Point*>& sites)
{
	std::vector<Point*>::iterator it;

	for (it = sites.begin(); it != sites.end(); it++)
		push(*it);
}

}
