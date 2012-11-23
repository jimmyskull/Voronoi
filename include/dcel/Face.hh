///////////////////////////////////////////////////////////////////////////////
//
//  Copyright Leonardo Fischer 2011 - http://coderender.blogspot.com
//
//  Distributed under the licence available in the accompanying file 
//  LICENCE.txt. Please read it before use this code.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __DCEL_Face_H__
#define __DCEL_Face_H__

#include <cstddef>
#include <cassert>

namespace dcel {

template<class VertexDataT, class HalfEdgeDataT, class FaceDataT>
class HalfEdgeT;

template<class VertexDataT, class HalfEdgeDataT, class FaceDataT>
class FaceT {
	typedef HalfEdgeT<VertexDataT, HalfEdgeDataT, FaceDataT> HalfEdge;
	typedef FaceT<VertexDataT, HalfEdgeDataT, FaceDataT> Face;

public:

	FaceT() :
			boundary(NULL)
	{
	}

	inline HalfEdge* getBoundary() const
	{
		return this->boundary;
	}

	inline void setBoundary(HalfEdge* bound)
	{
		assert(this);
		this->boundary = bound;
	}

	inline FaceDataT& getData()
	{
		return data;
	}

	inline const FaceDataT& getData() const
	{
		return data;
	}

protected:
private:
	HalfEdge* boundary;
	FaceDataT data;
};

}

#endif /* __DCEL_Face_H__ */
