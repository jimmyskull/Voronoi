#ifndef __STATUS_H__
#define __STATUS_H__

namespace voronoi {
	
	class FaceData {
public:
	std::string name;
};

class Status {
public:
	Point* i;
	Point* j;
	Point* arc;
};

}

#endif 
