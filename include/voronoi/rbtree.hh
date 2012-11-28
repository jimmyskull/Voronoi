/**
 *  @file
 *  @brief 
 *  @date 27/11/2012
 *  @author Paulo Urio
 *  @copyright FreeBSD License
 */

#ifndef _RBTREE_HH_
#define _RBTREE_HH_

#include <cassert>
#include <cstddef>
#include <tr1/functional>
#include "status.hh"

namespace voronoi {

template<class T>
class RBTree;

template<class T>
class RBTreeNode {
public:
	enum Color {
		kRed = 0, kBlack = 1
	};

	RBTreeNode(T& data, RBTree<T>* tree) :
			_parent(NULL), _left_child(NULL), _right_child(NULL), _color(kRed),
					_data(data), _tree(tree), _id(0)
	{
	}

	~RBTreeNode()
	{
	}

	void set_parent(RBTreeNode* node)
	{
		_parent = node;
	}

	void set_left_child(RBTreeNode* node)
	{
		_left_child = node;
	}

	void set_right_child(RBTreeNode* node)
	{
		_right_child = node;
	}

	void set_color(Color color)
	{
		_color = color;
	}

	void set_data(T& data)
	{
		_data = data;
	}

	RBTreeNode* parent() const
	{
		return _parent;
	}

	RBTreeNode* left_child() const
	{
		return _left_child;
	}

	RBTreeNode* right_child() const
	{
		return _right_child;
	}

	Color color() const
	{
		return _color;
	}

	const T* data() const
	{
		return &_data;
	}

	int id() const
	{
		return _id;
	}

	RBTree<T>* tree() const
	{
		return _tree;
	}

	RBTreeNode* grandparent() const
	{
		return parent()->parent();
	}

	bool isBlack() const
	{
		return color() == kBlack;
	}

	bool isRed() const
	{
		return !isBlack();
	}

	bool isLeaf() const
	{
		return left_child() == NULL && right_child() == NULL;
	}

	void UpdateNodeIDs(int* id)
	{
		/*
		 * This method is called to prepare the tree to be printed in
		 * pre-order.  Useful to draw the tree with "bosque", where only
		 * integers are accepted as key to a node.
		 */
		if (left_child())
			left_child()->UpdateNodeIDs(id);
		set_id((*id)++);
		if (right_child())
			right_child()->UpdateNodeIDs(id);
	}

	void WalkPreOrder(typename RBTree<T>::Callback callback) const
	{
		callback(this);
		if (left_child())
			left_child()->WalkPreOrder(callback);
		if (right_child())
			right_child()->WalkPreOrder(callback);
	}

	void RotateToLeft()
	{
		RBTreeNode* y = right_child();

		set_right_child(y->left_child());
		if (y->left_child() != NULL)
			y->left_child()->set_parent(this);
		y->set_parent(parent());
		if (parent() == NULL)
			tree()->set_root(y);
		else if (isLeftChild())
			parent()->set_left_child(y);
		else
			parent()->set_right_child(y);
		y->set_left_child(this);
		set_parent(y);
	}

	void RotateToRight()
	{
		RBTreeNode* y = left_child();

		set_left_child(y->right_child());
		if (y->right_child() != NULL)
			y->right_child()->set_parent(this);
		y->set_parent(parent());
		if (parent() == NULL)
			tree()->set_root(y);
		else if (isRightChild())
			parent()->set_right_child(y);
		else
			parent()->set_left_child(y);
		y->set_right_child(this);
		set_parent(y);
	}

	void FixUp()
	{
		InternalFixUp();
		tree()->root()->set_color(kBlack);
	}

	void InsertLeftChild(RBTreeNode* node)
	{
		assert(left_child() == NULL);
		set_left_child(node);
		node->set_parent(this);
		node->FixUp();
	}

	void InsertRightChild(RBTreeNode* node)
	{
		assert(right_child() == NULL);
		set_right_child(node);
		node->set_parent(this);
		node->FixUp();
	}

	bool isLeftChild() const
	{
		return parent() != NULL && parent()->left_child() == this;
	}

	bool isRightChild() const
	{
		return parent() != NULL && parent()->right_child() == this;
	}

	bool isRoot() const
	{
		return parent() == NULL;
	}

	/**
	 * Detach child node without freeing.
	 */
	void DetachLeftChild()
	{
		if (left_child() != NULL)
			left_child()->set_parent(NULL);
		set_left_child(NULL);
	}

	/**
	 * Detach child node without freeing.
	 */
	void DetachRightChild()
	{
		if (right_child() != NULL)
			right_child()->set_parent(NULL);
		set_right_child(NULL);
	}

	/**
	 * Go through the tree until find a leaf.  This is used to find
	 * the nearest parabola on voronoi's algorithm.
	 */
	RBTreeNode* FindNearest(T& value)
	{
		if (isLeaf())
			return this;
		if (*data() < value) {
			if (left_child()->isLeaf())
				return left_child();
			else
				return left_child()->FindNearest(value);
		} else {
			if (right_child()->isLeaf())
				return right_child();
			else
				return right_child()->FindNearest(value);
		}
	}

	/**
	 * Free all child nodes, without detaching them.  You should notice
	 * the node will still have pointers to this free'd memory.  We won't
	 * set the links to left and right to NULL.
	 */
	void FreeChildNodes() const
	{
		if (left_child())
			left_child()->FreeChildNodes();
		delete left_child();
		if (right_child())
			right_child()->FreeChildNodes();
		delete right_child();
	}

private:
	void InternalFixUp()
	{
		/*
		 * Helper method for FixUp().
		 */
		RBTreeNode* uncle;
		RBTreeNode* next(this);

		if (isRoot() || parent()->isBlack())
			return;

		if (parent()->isLeftChild()) {
			uncle = grandparent()->right_child();
			if (uncle != NULL && uncle->isRed()) {
				parent()->set_color(kBlack);
				uncle->set_color(kBlack);
				grandparent()->set_color(kRed);
				next = grandparent();
			} else {
				if (isRightChild()) {
					next = parent();
					next->RotateToLeft();
				}
				next->parent()->set_color(kBlack);
				next->grandparent()->set_color(kRed);
				next->grandparent()->RotateToRight();
			}
		} else if (parent()->isRightChild()) {
			uncle = grandparent()->left_child();
			if (uncle != NULL && uncle->isRed()) {
				parent()->set_color(kBlack);
				uncle->set_color(kBlack);
				grandparent()->set_color(kRed);
				next = grandparent();
			} else {
				if (isLeftChild()) {
					next = parent();
					next->RotateToRight();
				}
				next->parent()->set_color(kBlack);
				next->grandparent()->set_color(kRed);
				next->grandparent()->RotateToLeft();
			}
		}
		if (next != NULL)
			next->InternalFixUp();
	}

	void set_id(int id)
	{
		_id = id;
	}

	RBTreeNode* _parent;
	RBTreeNode* _left_child;
	RBTreeNode* _right_child;
	Color _color;
	T _data;
	RBTree<T>* _tree;
	int _id;
};


/**
 * Implementation of Redblack tree specific for the fortune's algorithm to
 * make voronoi diagrams.
 *
 * Here, the insertion is unusual, more like an "attach" node to one side
 * instead of a real insert method.  The class VoronoiTree implements this.
 */
template<class T>
class RBTree {
public:
	typedef RBTreeNode<T> Node;

	typedef std::tr1::function<void(const RBTreeNode<T>*)> Callback;

	RBTree() :
			_root(NULL)
	{
	}

	~RBTree()
	{
		FreeAllNodes();
	}

	bool isEmpty() const
	{
		return root() == NULL;
	}

	Node* FindParabola(T& value) const
	{
		if (isEmpty())
			return NULL;
		return root()->FindNearest(value);
	}

	void PrintTree(Callback print_callback)
	{
		if (isEmpty())
			return;
		UpdateTreeIds();
		root()->WalkPreOrder(print_callback);
	}

	Node* root() const
	{
		return _root;
	}

	void set_root(Node* root)
	{
		_root = root;
	}

private:
	void FreeAllNodes()
	{
		if (!isEmpty()) {
			root()->FreeChildNodes();
			delete root();
			set_root(NULL);
		}
	}

	/* Workaround to walk tree InOrder updating the nodes. Used before
	 * printing.  The IDs don't mean anything to the node as a real
	 * identifier. */
	void UpdateTreeIds()
	{
		int id(1);
		if (!isEmpty())
			root()->UpdateNodeIDs(&id);
	}

	Node* _root;
};

class VoronoiTree: public RBTree<Status> {
public:
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
			Node* new_root = new Node(s, this);
			set_root(new_root);
			new_root->set_color(Node::kBlack);
			return;
		}
		Node* nearest = FindParabola(s);
		Node* internal1 = CreateBreakpointNode(nearest->data()->arc, s.arc);
		Node* internal2 = CreateBreakpointNode(s.arc, nearest->data()->arc);
		Node* leaf1 = CreateParabolaNode(nearest->data()->arc);
		Node* leaf2 = CreateParabolaNode(parabola);
		Node* leaf3 = CreateParabolaNode(nearest->data()->arc);
		internal1->set_color(nearest->color());

		if (nearest->isRoot()) {
			set_root(internal1);
		} else {
			Node* parent = nearest->parent();
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
	Node* CreateBreakpointNode(Point* i, Point* j)
	{
		Status status(i, j);
		return new Node(status, this);
	}

	Node* CreateParabolaNode(Point* parabola)
	{
		Status status(parabola);
		return new Node(status, this);
	}
};

}

#endif /* _RBTREE_HH_ */
