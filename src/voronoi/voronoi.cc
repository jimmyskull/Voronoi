#include <list>
#include <macros.hh>
#include <voronoi/voronoi.hh>

namespace voronoi {

Voronoi::Voronoi(std::list<Point*>& points) :
		dcel(points.size() * 2, points.size() * 2, points.size()),
		tree(queue, dcel)
{
	// Passo 1
	std::list<Point*>::const_iterator it;
	for (it = points.begin(); it != points.end(); it++)
		queue.push(*it);
	// Passo 2
	while (!queue.empty()) {
		Point* p = queue.top();
		queue.pop();

		if (p->isCircleEvent())
			HandleSiteEvent(p);
		else
			HandleCircleEvent(p);
	}

}

Voronoi::Voronoi(const Voronoi& orig) :
		dcel(orig.dcel), tree(queue, dcel)
{
	UNUSED(orig);
}

Voronoi::~Voronoi()
{
}

void Voronoi::HandleSiteEvent(Point* p)
{
	UNUSED(p);
	/*if (tree.lookup(NULL) == NULL) {
	 Status* node = new Status;
	 node->i = p;
	 tree.insert(node);

	 //double dist = p->y
	 }*/
}

void Voronoi::HandleCircleEvent(Point* p)
{
	UNUSED(p);
}

}
