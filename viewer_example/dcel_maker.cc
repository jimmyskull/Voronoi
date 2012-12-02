#include <cassert>
#include <cstddef>
#include "test.hh"
#include <voronoi/voronoi.hh>

using voronoi::VoronoiDCEL;

static VoronoiDCEL::Vertex* createVertex(VoronoiDCEL& d, double x, double y)
{
	VoronoiDCEL::Vertex* v = d.createGetVertex();
	v->getData()->set_coordinates(x, y);
	return v;
}

static void createEdge(VoronoiDCEL& d, VoronoiDCEL::Vertex* origin, VoronoiDCEL::Face* face,
		VoronoiDCEL::Vertex* twinOrigin, VoronoiDCEL::Face* twinFace,
		VoronoiDCEL::HalfEdge** outHalfEdge, VoronoiDCEL::HalfEdge** outTwinHalfEdge)
{
	unsigned int i = d.createEdge(origin, face, twinOrigin, twinFace);

	*outHalfEdge = d.getHalfEdge(i);
	*outTwinHalfEdge = (*outHalfEdge)->getTwin();

	assert((*outHalfEdge)->getOrigin() == origin);
	assert((*outTwinHalfEdge)->getOrigin() == twinOrigin);

	origin->setIncidentEdge(*outHalfEdge);
	twinOrigin->setIncidentEdge(*outTwinHalfEdge);
}

static VoronoiDCEL::Face* createFace(VoronoiDCEL& d, int id)
{
	VoronoiDCEL::Face* face = d.createGetFace(NULL);

	face->getData().set_id(id);
	return face;
}

VoronoiDCEL* createDCEL()
{
	const size_t VERTICES = 4;
	const size_t EDGES = 4;
	const size_t FACES = 2;

	/* Criar DCEL alocando espaço o necessário */
	VoronoiDCEL* ptrd = new VoronoiDCEL(VERTICES, EDGES, FACES);
	VoronoiDCEL& d = *ptrd;

	/* Criar os vértices */
	VoronoiDCEL::Vertex* v1 = createVertex(d, 0, 4);
	VoronoiDCEL::Vertex* v2 = createVertex(d, 2, 4);
	VoronoiDCEL::Vertex* v3 = createVertex(d, 2, 2);
	VoronoiDCEL::Vertex* v4 = createVertex(d, 1, 1);

	/* Criar as faces */
	VoronoiDCEL::Face* f1 = createFace(d, 1);
	VoronoiDCEL::Face* f2 = createFace(d, 2);

	/* Criar as meias arestas */
	VoronoiDCEL::HalfEdge *e11, *e12, *e21, *e22, *e31, *e32, *e41, *e42;
	e11 = e12 = e21 = e22 = e31 = e32 = e41 = e42 = NULL;

	/*  		  Src 	Face 	TwinSrc	TwinFace	HalfEdge	TwinHalfEdge */
	createEdge(d, v1, f1, v2, f2, &e11, &e12);
	createEdge(d, v3, f1, v4, f1, &e21, &e22);
	createEdge(d, v3, f1, v1, f2, &e31, &e32);
	createEdge(d, v3, f2, v2, f1, &e41, &e42);

	e11->setNext(e42);
	e12->setNext(e32);
	e21->setNext(e22);
	e22->setNext(e31);
	e31->setNext(e11);
	e32->setNext(e41);
	e41->setNext(e12);
	e42->setNext(e21);

	f1->setBoundary(e22);
	f2->setBoundary(e32);

	d.checkAllFaces();

	return ptrd;
}

