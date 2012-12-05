/**
 *  @file
 *  @brief 
 *  @date 04/12/2012
 *  @author Paulo Urio
 *  @copyright FreeBSD License
 */

#include <iostream>
#include <voronoi/point.hh>
#include <voronoi/queue.hh>
#include <voronoi/status.hh>
#include <voronoi/tree.hh>

namespace voronoi {

template <class T>
class RBTreeNode;

typedef RBTreeNode<Status> Node;

static void print_node(const RBTreeNode<Status>* v)
{
	if (v->isRoot())
		std::cerr << "root  ";
	else if (v->isLeftChild())
		std::cerr << "left  ";
	else
		std::cerr << "right ";
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

void VoronoiTree::InsertParabola(Point* parabola)
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

	std::cerr << "Mais próximo: " << nearest->data()->str() << " (" << nearest
			<< ")\n";

	if (nearest->data()->hasCircleEvent()) {
		const Status* data = nearest->data();
		Point* circle_event = data->circle_event();
		circle_event->MarkAsFalseAlarm();
		//nearest->set_data(data);
		std::cerr << "\033[31;1mEvento de círculo em " << circle_event->str()
				<< " marcado como falso alarme\n";
	}

	Node* internal_root = CreateBreakpointNode(nearest->data()->arc, s.arc);
	Node* internal2 = CreateBreakpointNode(s.arc, nearest->data()->arc);
	Node* leaf_left = CreateParabolaNode(nearest->data()->arc);
	Node* leaf_middle = CreateParabolaNode(parabola);
	Node* leaf_right = CreateParabolaNode(nearest->data()->arc);
	internal_root->set_color(nearest->color());

	if (nearest->isRoot()) {
		set_root(internal_root);
	} else {
		Node* parent = nearest->parent();
		if (nearest->isLeftChild()) {
			parent->DetachLeftChild();
			parent->AttachLeftChild(internal_root);
		} else {
			parent->DetachRightChild();
			parent->AttachRightChild(internal_root);
		}
	}
	internal_root->InsertRightChild(internal2);
	internal_root->InsertLeftChild(leaf_left);

	internal2->InsertLeftChild(leaf_middle);
	internal2->InsertRightChild(leaf_right);

	// Update start edges
	Point* start = new Point(parabola->x(),
			nearest->data()->arc->GetYOfParabolaInsersection(parabola));

	Status* ll = const_cast<Status*>(internal2->data());
	ll->set_start(start);

	Status* lr = const_cast<Status*>(internal_root->data());
	lr->set_start(start);

	std::cerr << "\033[36;1mAresta começando em " << start->str();
	std::cerr << " (armazenado em " << ll->str() << " e " << lr->str()
			<< ")\033[0m\n";

	delete nearest;

	CheckCircle(leaf_left, parabola->y());
	CheckCircle(leaf_right, parabola->y());

	//PrintTree();
}

/**
 * This method should be called only on leaves (parabolas).
 */
void VoronoiTree::CheckCircle(Node* leaf, double sweepline_y)
{
	Node* left_parent = leaf->GetFirstParentAtLeft();
	Node* right_parent = leaf->GetFirstParentAtRight();

	if (left_parent == NULL || right_parent == NULL)
		return;

	Node* left_neighbor = left_parent->GetPredecessorChild();
	Node* right_neighbor = right_parent->GetSuccessorChild();

	if (left_neighbor == NULL || right_neighbor == NULL)
		return;
	if (*left_neighbor->data() == *right_neighbor->data())
		return;

	Point* a = left_neighbor->data()->arc;
	Point* b = leaf->data()->arc;
	Point* c = right_neighbor->data()->arc;

	Point* circle_bottom = new Point();
	circle_bottom->SetCoordinatesToTheCircleBottom(a, b, c);

	if (circle_bottom->y() >= sweepline_y
			|| circle_bottom->y() != circle_bottom->y()) {
		delete circle_bottom;
		return;
	}

	Status* data = const_cast<Status*>(leaf->data());
	data->set_circle_event(circle_bottom);

	circle_bottom->set_circle_lowest_circle_parabola_node(leaf);

	queue.push(circle_bottom);

	std::cerr << "\033[1;34mEvento de círculo em " << circle_bottom->str()
			<< "\033[0m\n";
}

void VoronoiTree::RemoveParabola(Point* circle_event)
{
	std::cerr << "\033[32;1m==> RemoveParabola(" << circle_event->str()
			<< ")\033[0m" << std::endl;

	Node* leaf = circle_event->lowest_circle_parabola_node();
	std::cerr << "Evento de círculo aponta para " << leaf->data()->str() << " ("
			<< leaf << ")\n";

	Node* left_parent = leaf->GetFirstParentAtLeft();
	Node* right_parent = leaf->GetFirstParentAtRight();
	Node* left_neighbor = left_parent->GetPredecessorChild();
	Node* right_neighbor = right_parent->GetSuccessorChild();

	if (left_neighbor == right_neighbor)
		std::cerr << "Erro? Parábolas esquerda e direitas são iguais.\n";

	Status* lc = const_cast<Status*>(left_neighbor->data());
	lc->RemoveCircleEvent();

	Status* rc = const_cast<Status*>(right_neighbor->data());
	rc->RemoveCircleEvent();

	Node* p = leaf->parent();
	Status* data = const_cast<Status*>(p->parent()->data());
	std::cerr << "Breakpoint atualizado de " << p->parent()->data()->str();
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
	std::cerr << " para " << data->str() << "\n";

	bool is_left = p->isLeftChild();
	while ((p = p->parent()) != NULL && !p->isRoot()) {
		Status* data = const_cast<Status*>(p->parent()->data());
		std::cerr << "-- Breakpoint atualizado de "
				<< p->parent()->data()->str();
		if (p->isLeftChild()) {
			data->i = p->data()->j;
		} else {
			data->j = p->data()->i;
		}
		std::cerr << " para " << data->str() << "\n";
		if (p->isLeftChild() != is_left)
			break;
	}

	Point* center = new Point();

	center->SetCoordinatesToTheCircleCenter(left_neighbor->data()->arc,
			leaf->data()->arc,
			right_neighbor->data()->arc);

	std::cerr << "\033[35;1mNova aresta: "
			<< left_parent->data()->start()->str();
	std::cerr << " ---> " << center->str() << " (" << left_parent
			<< ")\033[0m\n";

	std::cerr << "\033[35;1mNova aresta: "
			<< right_parent->data()->start()->str();
	std::cerr << " ---> " << center->str() << " (" << right_parent
			<< ")\033[0m\n";

	std::cerr << "\033[33;1mVértice de voronoi no centro do ";
	std::cerr << "círculo " << left_neighbor->data()->arc->str() << ", "
			<< leaf->data()->arc->str() << ", "
			<< right_neighbor->data()->arc->str() << " = " << center->str()
			<< ", removendo ";
	std::cerr << leaf->data()->str() << " e " << leaf->parent()->data()->str()
			<< " " << leaf->parent() << "\033[0m\n";

	std::cerr << "\033[35;1mNova aresta deveria começar aqui.\033[0m\n";

	Node* par = leaf;
	Node* higher = NULL;
	while (!par->isRoot()) {
		par = par->parent();
		if (par == left_parent)
			higher = left_parent;
		if (par == right_parent)
			higher = right_parent;
	}
	Status* gpstat = const_cast<Status*>(higher->data());
	gpstat->set_start(center);

	//delete leaf->data()->start();
	leaf->SelfDelete();
	leaf->parent()->SelfDelete();
	delete leaf->parent();
	delete leaf;

	//PrintTree();

	CheckCircle(left_neighbor, circle_event->y());
	CheckCircle(right_neighbor, circle_event->y());
}

void VoronoiTree::PrintTree()
{
	RBTree::PrintTree(print_node);
	std::cout << " . ";
}

Node* VoronoiTree::CreateBreakpointNode(Point* i, Point* j)
{
	Status status(i, j);
	return new Node(status, this);
}

Node* VoronoiTree::CreateParabolaNode(Point* parabola)
{
	Status status(parabola);
	return new Node(status, this);
}

}
