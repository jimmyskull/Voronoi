/*
 * Use this program with "bosque" tree drawer:
 *
 * $ ./bin/tree_test | desenhar --redblack
 */
#include <iostream>

#include <voronoi/voronoi.hh>

using namespace voronoi;

void print_node(VoronoiTree::Node* v)
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

int main(void)
{
	VoronoiTree tree;

	Point* a = new Point(2, 5);
	Point* b = new Point(5, 4);
	Point* c = new Point(3, 3);
	Point* d = new Point(7, 2);
	Point* e = new Point(6, 1);

	tree.InsertParabola(a);
	tree.InsertParabola(b);
	tree.InsertParabola(c);
	//tree.InsertParabola(d);
	//tree.InsertParabola(e);

	/*std::cout << ".\n";
	tree.UpdateTreeIds();
	tree.root()->WalkPreOrder(print_node);
	*/

	delete a;
	delete b;
	delete c;
	delete d;
	delete e;
	return 0;
}
