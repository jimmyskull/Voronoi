/**
 *  @file
 *  @brief 
 *  @date 04/12/2012
 *  @author Paulo Urio
 *  @copyright FreeBSD License
 */

#ifndef _VORONOITREE_CC_
#define _VORONOITREE_CC_

#include "rbtree.hh"

namespace voronoi {

struct Point;
class Status;
class VoronoiDCEL;
class VoronoiQueue;

typedef RBTreeNode<Status> Node;

class VoronoiTree: public RBTree<Status> {
public:
	VoronoiQueue& queue;
	VoronoiDCEL& dcel;

	VoronoiTree(VoronoiQueue& queue, VoronoiDCEL& dcel) :
			queue(queue), dcel(dcel)
	{
	}

	/**
	 * Find the nearest parabola in the tree and then replace it by a
	 * new subtree with two internal nodes and three leaves.  The nearest
	 * parabola will be splitted into |leaf1| and |leaf3| nodes.  The new
	 * |parabola| will be |leaf3|.  The internal nodes |internal1| and
	 * |internal2| are breakpoints (tuples) for the leaves.
	 */
	void InsertParabola(Point* parabola);

	void RemoveParabola(Point* circle_event);

	void CheckCircle(Node* leaf, double sweepline_y);

	void PrintTree();

private:
	Node* CreateBreakpointNode(Point* i, Point* j);
	Node* CreateParabolaNode(Point* parabola);
};

}

#endif /* _VORONOITREE_CC_ */
