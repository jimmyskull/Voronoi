/**
 *  @file
 *  @brief 
 *  @date 04/12/2012
 *  @author Paulo Urio
 *  @copyright FreeBSD License
 */

#ifndef _DIAGRAM_HH_
#define _DIAGRAM_HH_

#include <list>
#include <cstddef>

namespace voronoi {

struct FaceInfo;
struct Point;

struct DiagramEdge {
	const Point* origin;
	const Point* destination;

	DiagramEdge(const Point* origin, const Point* destination) :
		origin(origin), destination(destination)
	{

	}
};

class VoronoiDCEL {
public:
	VoronoiDCEL();

	~VoronoiDCEL();

	void addEdge(const Point* origin, const Point* destination);
	void addVertex(const Point* point);

	const std::list<const DiagramEdge*>& edges() const;
	const std::list<const Point*>& vertices() const;
private:
	std::list<const DiagramEdge*> _edges;
	std::list<const Point*> _vertices;
};

}


#endif /* _DIAGRAM_HH_ */
