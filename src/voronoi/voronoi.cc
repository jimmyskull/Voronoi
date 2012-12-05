#include <list>
#include <voronoi/voronoi.hh>

namespace voronoi {

Voronoi::Voronoi(std::vector<Point*>& points) :
		sites(points.begin(), points.end()),
		queue(sites),
		tree(&queue, &dcel)
{
	while (!queue.empty()) {
		Point* p = queue.top();
		queue.pop();

		if (p->isCircleEvent())
			HandleCircleEvent(p);
		else
			HandleSiteEvent(p);
	}
	tree.FinishEdges();
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


Voronoi::Voronoi(const Voronoi& /*orig*/) :
		tree(&queue, &dcel)
{
	throw;
}

Voronoi::~Voronoi()
{
	std::vector<Point*>::iterator i = sites.begin();

	while (i != sites.end()) {
		delete *i;
		i++;
	}
}

}
