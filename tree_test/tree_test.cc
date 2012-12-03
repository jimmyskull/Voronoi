/*
 * Use this program with "bosque" tree drawer:
 *
 * $ ./bin/tree_test | desenhar --redblack
 */
#include <iostream>
#include <cstdlib>
#include <ctime>

#include <voronoi/voronoi.hh>

using namespace voronoi;

void print_node(const VoronoiTree::Node* v)
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

double frand(double fmin, double fmax)
{
    double f = (double) rand() / RAND_MAX;
    return fmin + f * (fmax - fmin);
}

int main(void)
{
	VoronoiQueue queue;
	VoronoiDCEL dcel(6, 12, 6);
	VoronoiTree tree(queue, dcel);

/*	Point* a = new Point(2, 5);
	Point* b = new Point(5, 4);
	Point* c = new Point(3, 3);
	Point* d = new Point(7, 2);
	Point* e = new Point(6, 1);

	queue.push(a);
	queue.push(b);
	queue.push(c);
	queue.push(d);
	queue.push(e);
*/
	srand(time(NULL));
	for (int i = 0; i < 500; i++)
		queue.push(new Point(frand(0, 20), frand(0, 20)));

	while (!queue.empty()) {
		Point* x = queue.top();
		queue.pop();

		if (x->isCircleEvent()) {
			if (!x->isFalseAlarm())
				tree.RemoveParabola(x);
			delete x;
		} else {
			tree.InsertParabola(x);
		}
	}

	tree.PrintTree(print_node);

	dcel.clear();

//	delete a;
//	delete b;
//	delete c;
//	delete d;
//	delete e;
	return 0;
}
