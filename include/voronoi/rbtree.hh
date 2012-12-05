/**
 *  @file
 *  @brief 
 *  @date 27/11/2012
 *  @author Paulo Urio
 *  @copyright FreeBSD License
 */

#ifndef _RBTREE_HH_
#define _RBTREE_HH_

#include <tr1/functional>
#include <cassert>
#include <cstddef>

namespace voronoi {

template<class T>
class RBTree;

#define RB_ISBLACK(x) (x != NULL? x->isBlack(): true)
#define RB_ISRED(x) (x != NULL? x->isRed(): false)

template<class T>
class RBTreeNode {
public:
	enum Color {
		kRed = true, kBlack = false
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

	void PosInsertFixUp()
	{
		InternalInsertFixUp();
		tree()->root()->set_color(kBlack);
	}

	void AttachLeftChild(RBTreeNode* node)
	{
		assert(left_child() == NULL);
		set_left_child(node);
		node->set_parent(this);
	}

	void InsertLeftChild(RBTreeNode* node)
	{
		AttachLeftChild(node);
		node->PosInsertFixUp();
	}

	void AttachRightChild(RBTreeNode* node)
	{
		assert(right_child() == NULL);
		set_right_child(node);
		node->set_parent(this);
	}

	void InsertRightChild(RBTreeNode* node)
	{
		AttachRightChild(node);
		node->PosInsertFixUp();
	}

	/**
	 * Special method to delete itself from the tree.
	 * We will never delete a node that is a root of is a internal node with
	 * both left and right childs.
	 * The method returns itself.
	 */
	RBTreeNode* SelfDelete()
	{
		if (isRoot()) /* The root must not delete itself */
			throw;
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
		if (*data() < value)
			return left_child()->FindNearest(value);
		else
			return right_child()->FindNearest(value);
	}

	RBTreeNode* GetFirstParentAtLeft()
	{
		RBTreeNode* p = this;

		while (p->isLeftChild()) {
			if (p->isRoot())
				return NULL;
			p = p->parent();
		}
		return p->parent();
	}

	RBTreeNode* GetFirstParentAtRight()
	{
		RBTreeNode* p = this;

		while (p->isRightChild()) {
			if (p->isRoot())
				return NULL;
			p = p->parent();
		}
		return p->parent();
	}

	RBTreeNode* GetPredecessorChild() const
	{
		RBTreeNode* par = left_child();

		if (par == NULL)
			return NULL;
		while (!par->isLeaf())
			par = par->right_child();
		return par;
	}

	RBTreeNode* GetSuccessorChild()
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
	typedef std::tr1::function<void(const RBTreeNode<T>*)> Callback;

	RBTree() :
			_root(NULL)
	{
	}

	virtual ~RBTree()
	{
		FreeAllNodes();
	}

	bool isEmpty() const
	{
		return root() == NULL;
	}

	RBTreeNode<T>* FindParabola(T& value) const
	{
		if (isEmpty())
			return NULL;
		return root()->FindNearest(value);
	}

	void PrintTree(Callback callback)
	{
		if (isEmpty())
			return;
		UpdateTreeIds();
		root()->WalkPreOrder(callback);
	}

	RBTreeNode<T>* root() const
	{
		return _root;
	}

	void set_root(RBTreeNode<T>* root)
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

	RBTreeNode<T>* _root;
};

}

#endif /* _RBTREE_HH_ */
