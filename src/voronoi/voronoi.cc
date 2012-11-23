#include <list>
#include <macros.hh>
#include <voronoi/geometry.hh>
#include <voronoi/voronoi.hh>

namespace voronoi {

Voronoi::Voronoi(std::list<Point*>& points)
{
	UNUSED(points);
}

Voronoi::Voronoi(const Voronoi& orig)
{
	UNUSED(orig);
}

Voronoi::~Voronoi()
{
}

}
