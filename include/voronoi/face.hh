/**
 *  @file
 *  @brief 
 *  @date 28/11/2012
 *  @author Paulo Urio
 *  @copyright FreeBSD License
 */

#ifndef _VORONOIFACE_HH_
#define _VORONOIFACE_HH_

namespace voronoi {

struct FaceInfo {
public:
	FaceInfo() :
			id(-1)
	{
	}

	void set_id(int id)
	{
		this->id = id;
	}

	int id;
};

}

#endif /* _VORONOIFACE_HH_ */
