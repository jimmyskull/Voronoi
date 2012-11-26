///////////////////////////////////////////////////////////////////////////////
//
//  Copyright Leonardo Fischer 2011 - http://coderender.blogspot.com
//
//  Distributed under the licence available in the accompanying file 
//  LICENCE.txt. Please read it before use this code.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __DCEL_Vertex_H__
#define __DCEL_Vertex_H__

#include <cassert>
#include <cstddef>

namespace dcel {

template<class VertexDataT, class HalfEdgeDataT, class FaceDataT>
class HalfEdgeT;

template<class VertexDataT, class HalfEdgeDataT, class FaceDataT>
class VertexT {
	typedef VertexT<VertexDataT, HalfEdgeDataT, FaceDataT> Vertex;
	typedef HalfEdgeT<VertexDataT, HalfEdgeDataT, FaceDataT> HalfEdge;

public:

	VertexT() :
			data(), incidentEdge(NULL)
	{
	}

	inline HalfEdge* getIncidentEdge() const
	{
		assert(this);
		return this->incidentEdge;
	}
	inline void setIncidentEdge(HalfEdge* edge)
	{
		assert(this);
		this->incidentEdge = edge;
	}

	inline VertexDataT& getData()
	{
		return data;
	}

	inline const VertexDataT& getData() const
	{
		return data;
	}

protected:
private:
	VertexDataT data;
	HalfEdge* incidentEdge;
};

}

#endif /* __DCEL_Vertex_H__ */
