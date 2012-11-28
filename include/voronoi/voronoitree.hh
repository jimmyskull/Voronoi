/**
 *  @file
 *  @brief 
 *  @date 28/11/2012
 *  @author Paulo Urio
 *  @copyright FreeBSD License
 */

#ifndef _VORONOITREE_HH_
#define _VORONOITREE_HH_

#include "status.hh"
#include "rbtree.hh"

namespace voronoi {

class VoronoiTree: public RBTree<Status> {
public:
	typedef VoronoiTree::RBTreeNode Node;

	/**
	 * Find the nearest parabola in the tree and then replace it by a
	 * new subtree with two internal nodes and three leaves.  The nearest
	 * parabola will be splitted into |leaf1| and |leaf3| nodes.  The new
	 * |parabola| will be |leaf3|.  The internal nodes |internal1| and
	 * |internal2| are breakpoints (tuples) for the leaves.
	 */
	void InsertParabola(Point* parabola)
	{
		Status s(parabola);

		std::cerr << "\033[1m==> InsertParabola(" << s.str() << ")\033[0m\n";
		if (isEmpty()) {
			RBTreeNode* new_root = new RBTreeNode(s, this);
			set_root(new_root);
			new_root->set_color(RBTreeNode::kBlack);
			return;
		}
		RBTreeNode* nearest = FindParabola(s);
		RBTreeNode* internal1 = CreateBreakpointNode(nearest->data()->arc, s.arc);
		RBTreeNode* internal2 = CreateBreakpointNode(s.arc, nearest->data()->arc);
		RBTreeNode* leaf1 = CreateParabolaNode(nearest->data()->arc);
		RBTreeNode* leaf2 = CreateParabolaNode(parabola);
		RBTreeNode* leaf3 = CreateParabolaNode(nearest->data()->arc);
		internal1->set_color(nearest->color());

		if (nearest->isRoot()) {
			set_root(internal1);
		} else {
			RBTreeNode* parent = nearest->parent();
			if (nearest->isLeftChild()) {
				parent->DetachLeftChild();
				parent->InsertLeftChild(internal1);
			} else {
				parent->DetachRightChild();
				parent->InsertRightChild(internal1);
			}
		}
		internal1->InsertRightChild(internal2);
		internal1->InsertLeftChild(leaf1);

		internal2->InsertLeftChild(leaf2);
		internal2->InsertRightChild(leaf3);
		delete nearest;
	}

private:
	RBTreeNode* CreateBreakpointNode(Point* i, Point* j)
	{
		Status status(i, j);
		return new RBTreeNode(status, this);
	}

	RBTreeNode* CreateParabolaNode(Point* parabola)
	{
		Status status(parabola);
		return new RBTreeNode(status, this);
	}
};

}

#endif /* _VORONOITREE_HH_ */
