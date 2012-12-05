/**
 *  @file
 *  @brief 
 *  @date 02/12/2012
 *  @author Paulo Urio
 *  @copyright FreeBSD License
 */
#include <iostream>
#include <list>
#include <voronoi/voronoi.hh>

int main(void)
{
	using namespace voronoi;

	VoronoiQueue queue;
	VoronoiDCEL dcel(5, 10, 5);
	VoronoiTree tree(queue, dcel);

	std::vector<Point*> sites;

	sites.reserve(5);
	sites.push_back(new Point(2, 5));
	sites.push_back(new Point(5, 4));
	sites.push_back(new Point(3, 3));
	sites.push_back(new Point(7, 2));
	sites.push_back(new Point(6, 1));

	Voronoi voronoi(sites);

	voronoi.tree.PrintTree();
	return 0;
}
