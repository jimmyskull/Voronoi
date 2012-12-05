/**
 *  @file
 *  @brief 
 *  @date 04/12/2012
 *  @author Paulo Urio
 *  @copyright FreeBSD License
 */

#ifndef _DIAGRAM_HH_
#define _DIAGRAM_HH_

#include <cstddef>
#include <dcel/dcel.hh>

namespace voronoi {

struct FaceInfo;
struct Point;

class VoronoiDCEL: public dcel::DCEL<Point, Point, FaceInfo> {
public:
	VoronoiDCEL();

	VoronoiDCEL(size_t vertices, size_t edges, size_t faces);
};

}


#endif /* _DIAGRAM_HH_ */
