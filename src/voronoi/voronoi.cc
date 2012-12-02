#include <list>
#include <macros.hh>
#include <voronoi/voronoi.hh>

namespace voronoi {

Voronoi::Voronoi(std::list<Point*>& points) :
		sites(points.begin(), points.end()),
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
			HandleCircleEvent(p);
		else
			HandleSiteEvent(p);
	}
}

void Voronoi::HandleSiteEvent(Point* p)
{
	tree.InsertParabola(p);
}

void Voronoi::HandleCircleEvent(Point* p)
{
	if (!p->isFalseAlarm())
		tree.RemoveParabola(p);
	delete p;
}


Voronoi::Voronoi(const Voronoi& orig) :
		dcel(orig.dcel), tree(queue, dcel)
{
	UNUSED(orig);
	throw;
}

Voronoi::~Voronoi()
{
	while (!sites.empty()) {
		delete sites.front();
		sites.pop_front();
	}
}

}
