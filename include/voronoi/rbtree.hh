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
#include <iostream>
#include <tr1/functional>
#include <cstddef>

namespace voronoi {

template<class T>
class RBTree {
public:
	RBTree() :
			_root(NULL)
	{
	}

	~RBTree()
	{
		FreeAllNodes();
	}

	class RBTreeNode;
	typedef std::tr1::function<void(RBTreeNode*)> Callback;

	RBTreeNode* root() const
	{
		return _root;
	}

	void set_root(RBTreeNode* root)
	{
		std::cerr << "tree.set_root(" << root << ")\n";
		_root = root;
	}

	class RBTreeNode {
	public:
		enum Color {
			kRed = 0, kBlack = 1
		};

		RBTreeNode(T& data, RBTree* tree) :
				_parent(NULL), _left_child(NULL), _right_child(NULL), _color(kRed), _data(data), _tree(tree), _id(0)
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
			if (_color != color)
				std::cerr << this << ".set_color(" << (color==kBlack?"\033[1mBLACK\033[0m":"\033[31;1mRED\033[0m") << ")\n";
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

		RBTree* tree() const
		{
			return _tree;
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

		void WalkInOrderUpdateID(int* id)
		{
			if (left_child())
				left_child()->WalkInOrderUpdateID(id);
			set_id((*id)++);
			if (right_child())
				right_child()->WalkInOrderUpdateID(id);
		}

		void WalkPreOrder(Callback callback)
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

			std::cerr << "RotateToLeft()\n";
			set_right_child(y->left_child());
			if (y->left_child() != NULL)
				y->left_child()->set_parent(this);
			y->set_parent(parent());
			if (parent() == NULL)
				tree()->set_root(y);
			else if (this->isLeftChild())
				parent()->set_left_child(y);
			else
				parent()->set_right_child(y);
			y->set_left_child(this);
			set_parent(y);
		}

		void RotateToRight()
		{
			RBTreeNode* y = left_child();

			std::cerr << "RotateToRight()\n";
			set_left_child(y->right_child());
			if (y->right_child() != NULL)
				y->right_child()->set_parent(this);
			y->set_parent(parent());
			if (parent() == NULL)
				tree()->set_root(y);
			else if (this->isRightChild())
				parent()->set_right_child(y);
			else
				parent()->set_left_child(y);
			y->set_right_child(this);
			set_parent(y);
		}

		RBTreeNode* grandparent() const
		{
			return parent()->parent();
		}

		/* Do not call this directly. Use InsertFixUp */
		void FixUp()
		{
			RBTreeNode* uncle;
			RBTreeNode* next(this);

			std::cerr << "Fixing node "<< this << "\n";

			if (parent() == NULL || parent()->isBlack())
				return;

			//if (grandparent() == NULL)
			//	return;
			if (parent()->isLeftChild()) {
				uncle = grandparent()->right_child();
				if (uncle != NULL && uncle->isRed()) {
					std::cerr << "\033[36;1mCaso 1 (leftChild)\033[0m\n";
					parent()->set_color(kBlack);
					uncle->set_color(kBlack);
					grandparent()->set_color(kRed);
					next = grandparent();
				} else {
					std::cerr << "\033[36;1mCaso 2 (leftChild)\033[0m\n";
					if (isRightChild()) {
						next = parent();
						next->RotateToLeft();
					}
					next->parent()->set_color(kBlack);
					next->grandparent()->set_color(kRed);
					std::cerr << "A\n";
					next->grandparent()->RotateToRight();
				}
			} else if (parent()->isRightChild()) {
				uncle = grandparent()->left_child();
				if (uncle != NULL && uncle->isRed()) {
					std::cerr << "\033[36;1mCaso 1 (rightChild)\033[0m\n";
					parent()->set_color(kBlack);
					uncle->set_color(kBlack);
					grandparent()->set_color(kRed);
					next = grandparent();
				} else {
					std::cerr << "\033[36;1mCaso 2 (rightChild)\033[0m\n";
					if (isLeftChild()) {
						std::cerr << "   x isLeftChild(), " << parent()<<".RotateToLeft()\n";
						next = parent();
						next->RotateToRight();
					}
					next->parent()->set_color(kBlack);
					next->grandparent()->set_color(kRed);
					std::cerr << "B "<<next->grandparent()<<".RotateToRight()\n";
					next->grandparent()->RotateToLeft();
				}
			}
			if (next != NULL)
				next->FixUp();
		}

		void InsertFixUp()
		{
			std::cerr << "\033[32;1m===> " << this << ".InsertFixUp()\033[0m\n";
			FixUp();
			tree()->root()->set_color(kBlack);
			std::cerr << "\033[32;1m<=== (saindo)" << this << ".InsertFixUp()\033[0m\n";
		}

		void InsertLeftChild(RBTreeNode* node)
		{
			std::cerr << this << ".InsertLeftChild(" << node << ")\n";
			assert(left_child() == NULL);
			set_left_child(node);
			node->set_parent(this);
			node->InsertFixUp();
		}

		void InsertRightChild(RBTreeNode* node)
		{
			std::cerr << this << ".InsertRightChild(" << node << ")\n";
			assert(right_child() == NULL);
			set_right_child(node);
			node->set_parent(this);
			node->InsertFixUp();
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

		void DetachLeftChild()
		{
			if (left_child() != NULL)
				left_child()->set_parent(NULL);
			set_left_child(NULL);
		}

		void DetachRightChild()
		{
			if (right_child() != NULL)
				right_child()->set_parent(NULL);
			set_right_child(NULL);
		}

		void DeleteLeftChild()
		{
			RBTreeNode* node = left_child();
			DetachLeftChild();
			delete node;
		}

		void DeleteRightChild()
		{
			RBTreeNode* node = right_child();
			DetachRightChild();
			delete node;
		}

		RBTreeNode* FindNearest(T& value)
		{
			if (isLeaf())
				return this;
			if (*data() < value) {
				if (left_child()->isLeaf()) {
					return left_child();
				} else {
					return left_child()->FindNearest(value);
				}
			} else {
				if (right_child()->isLeaf()) {
					return right_child();
				} else {
					return right_child()->FindNearest(value);
				}
			}
		}

		void WalkPosOrderFreeingChilds()
		{
			if (left_child())
				left_child()->WalkPosOrderFreeingChilds();
			delete left_child();
			if (right_child())
				right_child()->WalkPosOrderFreeingChilds();
			delete right_child();
		}

	private:
		void set_id(int id)
		{
			_id = id;
		}

		RBTreeNode* _parent;
		RBTreeNode* _left_child;
		RBTreeNode* _right_child;
		Color _color;
		T _data;
		RBTree* _tree;
		int _id;
	};
	/* RBTreeNode */

	bool isEmpty() const
	{
		return root() == NULL;
	}

	/* Called only when the tree is empty! */
	void Insert(T& value)
	{
		RBTreeNode* new_node = new RBTreeNode(value, this);

		if (isEmpty()) {
			set_root(new_node);
			new_node->set_color(RBTreeNode::kBlack);
			return;
		}
		throw;
	}

	RBTreeNode* FindParabola(T& value)
	{
		if (isEmpty())
			return NULL;
		return root()->FindNearest(value);
	}

	/* Workaround to walk tree InOrder updating the nodes. Used before
	 * printing. */
	void UpdateTreeIds()
	{
		int id(1);
		if (root() != NULL)
			root()->WalkInOrderUpdateID(&id);
	}


private:

	void FreeAllNodes()
	{
		if (!isEmpty()) {
			root()->WalkPosOrderFreeingChilds();
			delete root();
			set_root(NULL);
		}
	}

	RBTreeNode* _root;
};

}

#endif /* _RBTREE_HH_ */
