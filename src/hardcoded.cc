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

#include <viewer.hh>

int main(void)
{
	using namespace voronoi;

	std::vector<Point*> sites;

	sites.reserve(5);
	sites.push_back(new Point(2, 5));
	sites.push_back(new Point(5, 4));
	sites.push_back(new Point(3, 3));
	sites.push_back(new Point(7, 2));
	sites.push_back(new Point(6, 1));

	Voronoi voronoi(sites);

	voronoi.tree.PrintTree();

	//std::cerr <<  "A merda do caralho do filho da puta do vagabundo da buceta desse lixo de travesti viado filho de uma puta do caralho tem a porra do tamanho igual a porra da mae na zona, "
	//		<< voronoi.tree.dcel->getVertices().size() << std::endl;

	DiagramViewer& dv = DiagramViewer::getInstance();
	dv.set_diagram(&voronoi.dcel);
	dv.set_sites(&voronoi.sites);
	dv.Show();

	return 0;
}
