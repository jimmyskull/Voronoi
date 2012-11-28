#include <viewer.hh>
#include "test.hh"

using namespace voronoi;

int main(void)
{
	DiagramViewer& viewer = DiagramViewer::getInstance();
	VoronoiDCEL* d = createDCEL();

	viewer.set_diagram(d);
	viewer.Show();
	d->clear();
	delete d;
	return 0;
}
