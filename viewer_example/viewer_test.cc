#include <vector>
#include <viewer.hh>
#include "test.hh"

using namespace voronoi;

int main(void)
{
	DiagramViewer& viewer = DiagramViewer::getInstance();
	VoronoiDCEL* d = createDCEL();
	std::vector<Point*> v;

	v.reserve(5);
	v.push_back(new Point(0, 0));
	v.push_back(new Point(1, 1));
	v.push_back(new Point(2, 2));
	v.push_back(new Point(3, 3));
	v.push_back(new Point(4, 4));

	viewer.set_diagram(d);
	viewer.set_sites(&v);
	viewer.Show();
	//d->clear();
	delete d;
	return 0;
}
