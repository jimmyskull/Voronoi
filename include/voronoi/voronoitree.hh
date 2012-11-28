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

	static void print_node(VoronoiTree::Node* v)
	{
		if (v->isRoot())
			std::cerr << "    root  ";
		else if (v->isLeftChild())
			std::cerr << "    left  ";
		else
			std::cerr << "    right ";
		std::cerr << v->id() << " " << v->data()->str();
		std::cout << v->id();
		if (v->isBlack()) {
			std::cout << " BLACK\n";
			std::cerr << " \033[1mBLACK\033[0m " << " (" << v << ")\n";
		} else {
			std::cout << " RED\n";
			std::cerr << " \033[31;1mRED\033[0m " << " (" << v << ")\n";
		}
	}

	void InsertParabola(Point* parabola)
	{
		Status s(parabola);

		RBTreeNode* nearest = FindParabola(s);

		//if (nearest->data()->arc->hasCircleEvent() == false) {
		//	/* TODO: */
		//}

		//std::cout << "Nó mais próximo: "

		std::cerr << "\033[1m==> InsertParabola(" << s.str() << ")\033[0m\n";

		if (nearest == NULL) {
			Insert(s);

			UpdateTreeIds();
			root()->WalkPreOrder(print_node);
			std::cout << ".\n";

			std::cerr << "\033[1m<== (saindo) InsertParabola(" << s.str()
					<< ") OBS: parabola == raiz\033[0m\n";
			return;
		}

		// verificar se é raiz.
		Status i1, i2, p1, p2, p3;

		i1.i = nearest->data()->arc;
		i1.j = s.arc;
		RBTreeNode* internal1 = new RBTreeNode(i1, this);

		std::cerr << "InsertParabola(): substituindo " << nearest << " por "
				<< internal1 << "\n";

		internal1->set_parent(nearest->parent());
		internal1->set_color(nearest->color());

		i2.i = s.arc;
		i2.j = nearest->data()->arc;
		RBTreeNode* internal2 = new RBTreeNode(i2, this);

		p1.arc = nearest->data()->arc;
		p2.arc = s.arc;
		p3.arc = nearest->data()->arc;
		RBTreeNode* leaf1 = new RBTreeNode(p1, this);
		RBTreeNode* leaf2 = new RBTreeNode(p2, this);
		RBTreeNode* leaf3 = new RBTreeNode(p3, this);

		std::cerr << "Novos nós:\n";
		std::cerr << "\033[34;1m    " << internal1 << ": "
				<< internal1->data()->str() << " (interno 1)\n";
		std::cerr << "    " << internal2 << ": " << internal2->data()->str()
				<< " (interno 2)\n";
		std::cerr << "    " << leaf1 << ": " << leaf1->data()->str()
				<< " (folha à esquerda)\n";
		std::cerr << "    " << leaf2 << ": " << leaf2->data()->str()
				<< " (folha do centro)\n";
		std::cerr << "    " << leaf3 << ": " << leaf3->data()->str()
				<< " (folha à direita)\033[0m\n";

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
			std::cout << ".\n";
			UpdateTreeIds();
			root()->WalkPreOrder(print_node);
		}
		internal1->InsertRightChild(internal2);

		std::cout << ".\n";
		UpdateTreeIds();
		root()->WalkPreOrder(print_node);

		internal1->InsertLeftChild(leaf1);

		std::cout << ".\n";
		UpdateTreeIds();
		root()->WalkPreOrder(print_node);

		internal2->InsertLeftChild(leaf2);

		std::cout << ".\n";
		UpdateTreeIds();
		root()->WalkPreOrder(print_node);

		internal2->InsertRightChild(leaf3);

		std::cout << ".\n";
		UpdateTreeIds();
		root()->WalkPreOrder(print_node);

		//leaf2->InsertFixUp();
		std::cerr << "\033[1m<== (saindo) InsertParabola(" << s.str()
				<< ")\033[0m\n";

		delete nearest;
	}
};

}

#endif /* _VORONOITREE_HH_ */
