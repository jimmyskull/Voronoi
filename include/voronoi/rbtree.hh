/**
 *  @file
 *  @brief 
 *  @date 27/11/2012
 *  @author Paulo Urio
 *  @copyright FreeBSD License
 */

#ifndef _RBTREE_HH_
#define _RBTREE_HH_

#include <limits>
#include <queue>
#include <tr1/functional>
#include <cassert>
#include <cstddef>
#include <dcel/dcel.hh>
#include "status.hh"

namespace voronoi {

template<class T>
class RBTree;

#define RB_ISBLACK(x) (x != NULL? x->isBlack(): true)
#define RB_ISRED(x) (x != NULL? x->isRed(): false)

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
		assert(left_child() == NULL);
		assert(right_child() == NULL);
		assert(color() == kRed);
		assert(isRed());
		assert(!isBlack());
		assert(this->tree() != NULL);
		assert(parent() == NULL);
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

	void set_data(const T* data)
	{
		_data = *data;
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

		assert(!isLeaf());
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

		assert(!isLeaf());
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

	void PosInsertFixUp()
	{
		InternalInsertFixUp();
		tree()->root()->set_color(kBlack);
	}

	void InsertLeftChild(RBTreeNode* node)
	{
		assert(left_child() == NULL);
		set_left_child(node);
		node->set_parent(this);
		node->PosInsertFixUp();
	}

	void InsertRightChild(RBTreeNode* node)
	{
		if (right_child() != NULL)
			std::cerr << right_child()->data()->str() << "\n";
		assert(right_child() == NULL);
		set_right_child(node);
		node->set_parent(this);
		node->PosInsertFixUp();
	}

	void AttachLeftChild(RBTreeNode* node)
	{
		assert(left_child() == NULL);
		set_left_child(node);
		node->set_parent(this);
		//node->PosInsertFixUp();
	}

	void AttachRightChild(RBTreeNode* node)
	{
		if (right_child() != NULL)
			std::cerr << right_child()->data()->str() << "\n";
		assert(right_child() == NULL);
		set_right_child(node);
		node->set_parent(this);
		//node->PosInsertFixUp();
	}

	/**
	 * Special method to delete itself from the tree.
	 * We will never delete a node that is a root of is a internal node with
	 * both left and right childs.
	 * The method returns itself.
	 */
	RBTreeNode* SelfDelete()
	{
		if (isRoot()) {
			std::cerr
					<< "I shouldn't be trying to delete myself when I'm the root.\n";
			return this;
		}
		RBTreeNode* new_son;
		if (left_child() != NULL)
			new_son = left_child();
		else
			new_son = right_child();
		if (new_son != NULL)
			new_son->set_parent(parent());
		if (isLeftChild())
			parent()->set_left_child(new_son);
		else
			parent()->set_right_child(new_son);
		if (isBlack() && new_son != NULL)
			new_son->PosRemoveFixUp();
		return this;
	}

	void PosRemoveFixUp()
	{
		InternalRemoveFixUp();
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
		assert(left_child() == NULL);
	}

	/**
	 * Detach child node without freeing.
	 */
	void DetachRightChild()
	{
		if (right_child() != NULL)
			right_child()->set_parent(NULL);
		set_right_child(NULL);
		assert(right_child() == NULL);
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

	RBTreeNode* GetFirstLeftParent()
	{
		RBTreeNode* last = this;
		RBTreeNode* p = parent();

		while (p->left_child() == last) {
			if (!p->parent())
				return NULL;
			last = p;
			p = p->parent();
		}
		return p;
	}

	RBTreeNode* GetFirstRightParent()
	{
		RBTreeNode* last = this;
		RBTreeNode* p = parent();

		while (p->right_child() == last) {
			if (!p->parent())
				return NULL;
			last = p;
			p = p->parent();
		}
		return p;
	}

	RBTreeNode* GetLeftmostChild() const
	{
		RBTreeNode* par = left_child();

		if (par == NULL)
			return NULL;
		while (!par->isLeaf())
			par = par->right_child();
		return par;
	}

	RBTreeNode* GetRightmostChild()
	{
		RBTreeNode* par = right_child();

		if (par == NULL)
			return NULL;
		while (!par->isLeaf())
			par = par->left_child();
		return par;
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
	void InternalInsertFixUp()
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
			next->InternalInsertFixUp();
	}

	void InternalRemoveFixUp()
	{
		if (isRed() || isRoot()) {
			set_color(kBlack);
			return;
		}
		if (isLeftChild()) {
			RBTreeNode* sibling = parent()->right_child();
			if (RB_ISRED(sibling)) {
				sibling->set_color(kBlack);
				parent()->set_color(kRed);
				parent()->RotateToLeft();
				sibling = parent()->right_child();
			}
			RBTreeNode* left = sibling->left_child();
			RBTreeNode* right = sibling->right_child();
			if (RB_ISBLACK(left) && RB_ISBLACK(right)) {
				sibling->set_color(kRed);
				sibling->InternalRemoveFixUp();
			} else if (RB_ISBLACK(right)) {
				sibling->left_child()->set_color(kBlack);
				sibling->set_color(kRed);
				sibling->RotateToRight();
				sibling = parent()->right_child();
				sibling->set_color(parent()->color());
				parent()->set_color(kBlack);
				sibling->right_child()->set_color(kBlack);
				parent()->RotateToLeft();
				tree()->root()->InternalRemoveFixUp();
			}
		} else {
			assert(isRightChild());
			RBTreeNode* sibling = parent()->left_child();
			if (RB_ISRED(sibling)) {
				sibling->set_color(kBlack);
				parent()->set_color(kRed);
				parent()->RotateToRight();
				sibling = parent()->left_child();
			}
			RBTreeNode* left = sibling->left_child();
			RBTreeNode* right = sibling->right_child();
			if (RB_ISBLACK(left) && RB_ISBLACK(right)) {
				sibling->set_color(kRed);
				sibling->InternalRemoveFixUp();
			} else if (RB_ISBLACK(left)) {
				sibling->right_child()->set_color(kBlack);
				sibling->set_color(kRed);
				sibling->RotateToLeft();
				sibling = parent()->left_child();
				sibling->set_color(parent()->color());
				parent()->set_color(kBlack);
				sibling->left_child()->set_color(kBlack);
				parent()->RotateToRight();
				tree()->root()->InternalRemoveFixUp();
			}
		}
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

struct Point;

class VoronoiDCEL: public dcel::DCEL<Point, Point, FaceInfo> {
public:
	VoronoiDCEL(size_t vertices, size_t edges, size_t faces) :
			DCEL(vertices, edges, faces)
	{
	}

	VoronoiDCEL(const VoronoiDCEL& dcel) :
			DCEL(dcel)
	{
	}
};

class Status;

struct Point {
	Point(const Point& p) :
			_false_alarm(p.isFalseAlarm()), _face(-1U), start(p.start)
	{
		set_coordinates(p.x, p.y);
		set_id(id);
		set_circle_lowest_circle_parabola(p.lowest_circle_parabola());
	}

	Point() :
			x(0), y(0), id(0), _false_alarm(false), _face(-1U), start(NULL),
					_lowest_circle_parabola(NULL)
	{
	}

	Point(double x, double y) :
			id(0), _false_alarm(false), _face(-1U), start(NULL),
					_lowest_circle_parabola(NULL)
	{
		set_coordinates(x, y);
	}

	void set_id(int id)
	{
		this->id = id;
	}

	void set_coordinates(double x, double y)
	{
		set_x(x);
		set_y(y);
	}

	void set_x(double x)
	{
		this->x = x;
	}

	void set_y(double y)
	{
		this->y = y;
	}

	bool operator <(const Point& b) const
	{
		double abs_a = std::fabs(y);
		double abs_b = std::fabs(b.y);
		double greater = abs_a < abs_b? abs_b : abs_a;

		return (b.y - y) > greater * std::numeric_limits<double>::epsilon();
	}

	bool operator >(const Point& b) const
	{
		throw;
		return y > b.y;
	}

	void set_face(unsigned int face)
	{
		_face = face;
	}

	unsigned int face() const
	{
		return _face;
	}

	static Point GetCircleCenter(const Point* a, const Point* b, const Point* c)
	{
		double y_delta_a = b->y - a->y;
		double x_delta_a = b->x - a->x;
		double y_delta_b = c->y - b->y;
		double x_delta_b = c->x - b->x;

		double a_slope = y_delta_a / x_delta_a;
		double b_slope = y_delta_b / x_delta_b;

		double center_x, center_y;

		center_x = (a_slope * b_slope * (a->y - c->y) + b_slope * (a->x + b->x)
				- a_slope * (b->x + c->x)) / (2 * (b_slope - a_slope));
		center_y = -1 * (center_x - (a->x + b->x) / 2) / a_slope
				+ (a->y + b->y) / 2;

		return Point(center_x, center_y);
	}

	void SetCoordinatesToTheCircleBottom(const Point* a, const Point* b,
			const Point* c)
	{
		Point d = GetCircleCenter(a, b, c);

		double dx = d.x - a->x;
		double dy = d.y - a->y;

		double radius = std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));

		set_coordinates(d.x, d.y - radius);
	}

	RBTreeNode<Status>* lowest_circle_parabola() const
	{
		return _lowest_circle_parabola;
	}

	void set_circle_lowest_circle_parabola(RBTreeNode<Status>* node)
	{
		_lowest_circle_parabola = node;
	}

	bool isCircleEvent() const
	{
		return lowest_circle_parabola() != NULL;
	}

	bool isFalseAlarm() const
	{
		return _false_alarm;
	}

	void MarkAsFalseAlarm()
	{
		_false_alarm = true;
	}

	double GetParabolaY(Point* other) const
	{
		double dp = 2. * (y - other->y);
		double a1 = 1. / dp;
		double b1 = -2. * x / dp;
		double c1 = other->y + dp / 4. + std::pow(x, 2) / dp;

		return a1 * std::pow(other->x, 2) + b1 * other->x + c1;
	}

	std::string str() const
	{
		std::stringstream ss;

		ss << "(" << x*2 << "," << y*2 << ")";
		return ss.str();
	}

	double x;
	double y;
	int id;
	bool _false_alarm;
	unsigned int _face;

	// TODO: Clean
	Point* start; /**< Edge start point */
private:
	RBTreeNode<Status>* _lowest_circle_parabola;
};

struct ComparePoint: public std::binary_function<Point*, Point*, bool> {
	bool operator ()(const Point* a, const Point* b) const
	{
		return *a < *b;
	}
};

class VoronoiQueue: public std::priority_queue<Point*, std::vector<Point*>,
		ComparePoint> {

};

class Status {
public:
	Point* i;
	Point* j;
	Point* arc;

	Status() :
			i(NULL), j(NULL), arc(NULL), _circle_event(NULL)
	{
	}

	Status(const Status& status) :
			i(status.i), j(status.j), arc(status.arc),
					_circle_event(status.circle_event())
	{
	}

	Status(Point* arc) :
			i(NULL), j(NULL), arc(arc), _circle_event(NULL)
	{
	}

	Status(Point* i, Point* j) :
			i(i), j(j), arc(NULL), _circle_event(NULL)
	{
	}

	~Status()
	{
	}

	bool operator <(const Status& b) const
	{
		assert(i != NULL);
		assert(j != NULL);
		assert(b.arc != NULL);
		return GetXOfCircle(i, j, b.arc->y) < b.arc->x;
	}

	bool operator >(const Status& b) const
	{
		assert(i != NULL);
		assert(j != NULL);
		assert(b.arc != NULL);
		return GetXOfCircle(i, j, b.arc->y) > b.arc->x;
	}

	bool operator ==(const Status& b) const
	{
		UNUSED(b);
		return false;
	}

	std::string str() const
	{
		std::stringstream ss;

		if (arc == NULL) {
			ss << "<" << i->str() << " " << j->str() << ">";
			return ss.str();
		}
		return arc->str();
	}

	Point* circle_event() const
	{
		return _circle_event;
	}

	void set_circle_event(Point* event)
	{
		_circle_event = event;
	}

	bool hasCircleEvent() const
	{
		return circle_event() != NULL;
	}

	void RemoveCircleEvent()
	{
		if (hasCircleEvent()) {
			circle_event()->MarkAsFalseAlarm();
			set_circle_event(NULL);
		}
	}

private:
	// TODO: Move method to Point class.
	/* http://blog.ivank.net/fortunes-algorithm-and-implementation.html */
	/* Left point |a| and left point |b|. */
	static double GetXOfCircle(Point* p, Point* q, double y)
	{
		/* Centro do círculo de p */
		double dp = 2.0 * (p->y - y);
		double a1 = 1.0 / dp;
		double b1 = -2.0 * p->x / dp;
		double c1 = y + dp / 4 + p->x * p->x / dp;

		/* Centro do círculo de q */
		dp = 2.0 * (q->y - y);
		double a2 = 1.0 / dp;
		double b2 = -2.0 * q->x / dp;
		double c2 = y + dp / 4 + q->x * q->x / dp;

		double a = a1 - a2;
		double b = b1 - b2;
		double c = c1 - c2;

		double disc = b * b - 4 * a * c;
		double x1 = (-b + std::sqrt(disc)) / (2 * a);
		double x2 = (-b - std::sqrt(disc)) / (2 * a);

		double ry;
		if (p->y > q->y)
			ry = std::min(x1, x2);
		else
			ry = std::max(x1, x2);
		return ry;
	}

	Point* _circle_event;
};

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

		if (nearest->data()->hasCircleEvent()) {
			const Status* data = nearest->data();
			Point* circle_event = data->circle_event();
			circle_event->MarkAsFalseAlarm();
			nearest->set_data(data);
			std::cerr << "\033[31;1mEvento de círculo em "
					<< circle_event->str() << " marcado como falso alarme\n";
		}

		Node* internal_root = CreateBreakpointNode(nearest->data()->arc, s.arc);
		Node* internal2 = CreateBreakpointNode(s.arc, nearest->data()->arc);
		Node* leaf_left = CreateParabolaNode(nearest->data()->arc);
		Node* leaf_middle = CreateParabolaNode(parabola);
		Node* leaf_right = CreateParabolaNode(nearest->data()->arc);
		internal_root->set_color(nearest->color());

		assert(nearest->isLeaf());
		assert(internal_root->isLeaf());

		if (nearest->isRoot()) {
			set_root(internal_root);
			assert(internal_root->isLeaf());
		} else {
			assert(nearest->data()->arc != NULL);
			assert(nearest->data()->i == NULL);
			assert(nearest->data()->j == NULL);
			Node* parent = nearest->parent();
			assert(internal_root->isLeaf());
			if (nearest->isLeftChild()) {
				parent->DetachLeftChild();
				parent->AttachLeftChild(internal_root);
				assert(internal_root->isLeaf());
			} else {
				assert(nearest->isRightChild());
				parent->DetachRightChild();
				assert(parent->right_child() == NULL);
				parent->AttachRightChild(internal_root);
				assert(internal_root->isLeaf());
			}
			assert(internal_root->isLeaf());
		}
		assert(internal_root->isLeaf());
		assert(internal_root->right_child() == NULL);
		internal_root->InsertRightChild(internal2);
		internal_root->InsertLeftChild(leaf_left);

		assert(internal2->isLeaf());
		internal2->InsertLeftChild(leaf_middle);
		assert(internal2->right_child() == NULL);
		internal2->InsertRightChild(leaf_right);

		// Update start edges
		parabola->start = new Point(parabola->x, nearest->data()->arc->GetParabolaY(parabola));

		std::cerr << "\033[36;1mAresta começando em " << parabola->start->str() << "\033[0m\n";

		delete nearest;

		CheckCircle(leaf_left, parabola->y);
		CheckCircle(leaf_right, parabola->y);
	}

	/**
	 * This method should be called only on leaves (parabolas).
	 */
	void CheckCircle(Node* leaf, double sweepline_y)
	{
		assert(leaf->isLeaf());
		Node* left_parent = leaf->GetFirstLeftParent();
		Node* right_parent = leaf->GetFirstRightParent();

		if (left_parent == NULL || right_parent == NULL)
			return;

		Node* left_neighbor = left_parent->GetLeftmostChild();
		Node* right_neighbor = right_parent->GetRightmostChild();

		if (left_neighbor == NULL || right_neighbor == NULL)
			return;
		if (left_neighbor == right_neighbor)
			return;

		Point* a = left_neighbor->data()->arc;
		Point* b = leaf->data()->arc;
		Point* c = right_neighbor->data()->arc;

		Point* circle_bottom = new Point();
		circle_bottom->SetCoordinatesToTheCircleBottom(a, b, c);

		if (circle_bottom->y >= sweepline_y || circle_bottom->y != circle_bottom->y) {
			delete circle_bottom;
			return;
		}

		Status* data = const_cast<Status*>(leaf->data());
		data->set_circle_event(circle_bottom);

		assert(leaf->data()->circle_event() == circle_bottom);

		circle_bottom->set_circle_lowest_circle_parabola(leaf);

		queue.push(circle_bottom);

		std::cerr << "\033[1;34mEvento de círculo em " << circle_bottom->str()
				<< "\033[0m\n";

	}

	void RemoveParabola(Point* circle_event)
	{
		std::cerr << "\033[32;1m==> RemoveParabola(" << circle_event->str() << ")\033[0m"
				<< std::endl;


		Node* leaf = circle_event->lowest_circle_parabola();
		std::cerr << "Evento de círculo aponta para " << leaf->data()->str() << " (" <<leaf<< ")\n";

		Node* left_parent = leaf->GetFirstLeftParent();
		Node* right_parent = leaf->GetFirstRightParent();
		Node* left_neighbor = left_parent->GetLeftmostChild();
		Node* right_neighbor = right_parent->GetRightmostChild();

		if (left_neighbor == right_neighbor)
			std::cerr << "Erro? Parábolas esquerda e direitas são iguais.\n";

		Status* lc = const_cast<Status*>(left_neighbor->data());
		lc->RemoveCircleEvent();

		Status* rc = const_cast<Status*>(right_neighbor->data());
		rc->RemoveCircleEvent();

		Node* p = leaf->parent();
		Status* data = const_cast<Status*>(p->parent()->data());
		//std::cerr << "Breakpoint atualizado de " << p->parent()->data()->str();
		if (p->isLeftChild()) {
			if (leaf->isLeftChild())
				data->i = p->data()->j;
			else
				data->i = p->data()->i;
		} else {
			if (leaf->isLeftChild())
				data->j = p->data()->j;
			else
				data->j = p->data()->i;
		}
		//std::cerr << " para " << data->str() << "\n";
		p->parent()->set_data(data);


//		Point center = Point::GetCircleCenter(left_neighbor->data()->arc,
//				leaf->data()->arc,
//				right_neighbor->data()->arc);

	//	assert( leaf->data()->arc->start != NULL );
	//	std::cerr << "\033[35;1mNova aresta: " << leaf->data()->arc->start->str();
	//	std::cerr << " ---> " << center.str() << "\033[0m\n";

		std::cerr << "\033[33;1mVértice de voronoi no centro do ";
		std::cerr << "círculo " << left_neighbor->data()->arc->str()
				<< ", " << leaf->data()->arc->str() << ", "
				<< right_neighbor->data()->arc->str() << " = "
				<< Point::GetCircleCenter(left_neighbor->data()->arc,
						leaf->data()->arc,
						right_neighbor->data()->arc).str()
				<< ", removendo ";
		std::cerr << leaf->data()->str() << " e "
				<< leaf->parent()->data()->str() << "\033[0m\n";

		std::cerr << "\033[35;1mNova aresta deveria começar aqui.\033[0m\n";

		leaf->SelfDelete();
		leaf->parent()->SelfDelete();
		delete leaf->parent();
		delete leaf;

		CheckCircle(left_neighbor, circle_event->y);
		CheckCircle(right_neighbor, circle_event->y);
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
