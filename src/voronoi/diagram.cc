#include <cstddef>
#include <voronoi/diagram.hh>
#include <voronoi/point.hh>
#include <voronoi/face.hh>

#include <iostream>
namespace voronoi {

VoronoiDCEL::VoronoiDCEL()
{
}

VoronoiDCEL::~VoronoiDCEL()
{
	while (!_edges.empty()) {
		delete _edges.front();
		_edges.pop_front();
	}
}

void VoronoiDCEL::addEdge(const Point* origin, const Point* destination)
{
	std::cerr << "push back "<< origin->str() << ", " << destination->str() << std::endl;
	_edges.push_back(new DiagramEdge(new Point(*origin), new Point(*destination)));
}

void VoronoiDCEL::addVertex(const Point* point)
{
	_vertices.push_back(new Point(*point));
}

const std::list<const DiagramEdge*>& VoronoiDCEL::edges() const
{
	return _edges;
}

const std::list<const Point*>& VoronoiDCEL::vertices() const
{
	return _vertices;
}

}
