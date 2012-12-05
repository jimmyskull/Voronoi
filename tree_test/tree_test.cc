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

static double frand(double fmin, double fmax)
{
    double f = (double) rand() / RAND_MAX;
    return fmin + f * (fmax - fmin);
}

int main(void)
{
	VoronoiQueue queue;
	VoronoiDCEL dcel;
	VoronoiTree tree(&queue, &dcel);

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

	tree.PrintTree();

//	dcel.clear();

//	delete a;
//	delete b;
//	delete c;
//	delete d;
//	delete e;
	return 0;
}
