#include <cstddef>
#include <voronoi/diagram.hh>
#include <voronoi/point.hh>
#include <voronoi/face.hh>

namespace voronoi {

VoronoiDCEL::VoronoiDCEL()
{
}

VoronoiDCEL::VoronoiDCEL(size_t vertices, size_t edges, size_t faces) :
		DCEL(vertices, edges, faces)
{
}

}
