///////////////////////////////////////////////////////////////////////////////
//
//  Copyright Leonardo Fischer 2011 - http://coderender.blogspot.com
//
//  Distributed under the licence available in the accompanying file 
//  LICENCE.txt. Please read it before use this code.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __DCEL_HalfEdge_H__
#define __DCEL_HalfEdge_H__

#include <cassert>
#include <cstddef>

namespace dcel {

template<class VertexDataT, class HalfEdgeDataT, class FaceDataT>
class VertexT;

template<class VertexDataT, class HalfEdgeDataT, class FaceDataT>
class FaceT;

template<class VertexDataT, class HalfEdgeDataT, class FaceDataT>
class HalfEdgeT {
	typedef VertexT<VertexDataT, HalfEdgeDataT, FaceDataT> Vertex;
	typedef HalfEdgeT<VertexDataT, HalfEdgeDataT, FaceDataT> HalfEdge;
	typedef FaceT<VertexDataT, HalfEdgeDataT, FaceDataT> Face;

public:

	HalfEdgeT() :
			data(), twin(NULL), next(NULL), prev(NULL), origin(NULL), face(NULL)
	{
	}

	inline HalfEdge* getTwin() const
	{
		assert(this);
		return this->twin;
	}

	inline void setTwin(HalfEdge* newTwin)
	{
		assert(this);
		this->twin = newTwin;
		newTwin->twin = this;
	}

	inline HalfEdge* getNext() const
	{
		assert(this);
		return this->next;
	}

	inline void setNext(HalfEdge* newNext)
	{
		assert(this);
		assert(newNext != NULL);
		this->next = newNext;
		newNext->prev = this;
	}

	inline HalfEdge* getPrev() const
	{
		assert(this);
		return this->prev;
	}

	inline void setPrev(HalfEdge* newPrev)
	{
		assert(this);
		this->prev = newPrev;
		newPrev->next = this;
	}

	inline Vertex* getOrigin() const
	{
		assert(this);
		return this->origin;
	}

	inline void setOrigin(Vertex* newOrigin)
	{
		assert(this);
		this->origin = newOrigin;
	}

	inline Face* getFace() const
	{
		assert(this);
		return this->face;
	}

	inline void setFace(Face* newFace)
	{
		assert(this);
		this->face = newFace;
	}

	void setData(HalfEdgeDataT& data)
	{
		this->data = data;
	}

	inline HalfEdgeDataT& getData()
	{
		return data;
	}

	inline const HalfEdgeDataT& getData() const
	{
		return data;
	}

protected:
private:
	HalfEdgeDataT data;
	HalfEdge* twin;
	HalfEdge* next;
	HalfEdge* prev;
	Vertex* origin;
	Face* face;
};

}

#endif /* __DCEL_HalfEdge_H__ */
