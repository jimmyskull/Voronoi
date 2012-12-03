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

void print_node(const voronoi::VoronoiTree::Node* v)
{
	if (v->isRoot())
		std::cerr << "root  ";
	else if (v->isLeftChild())
		std::cerr << "left  ";
	else
		std::cerr << "right ";
	std::cerr << v->id() << " " << v->data()->str();
	std::cout << v->id();
	if (v->isBlack()) {
		std::cout << " BLACK\n";
		std::cerr << " \033[1mBLACK\033[0m " << " (" << v << ")\n";
	} else {
		std::cout << " RED\n";
		std::cerr << " \033[31;1mRED\033[0m " << " (" << v << ")\n";
	}
}

int main(void)
{
	using namespace voronoi;

	VoronoiQueue queue;
	VoronoiDCEL dcel(5, 10, 5);
	VoronoiTree tree(queue, dcel);

	std::list<Point*> sites;

	sites.push_back(new Point(2, 5));
	sites.push_back(new Point(5, 4));
	sites.push_back(new Point(3, 3));
	sites.push_back(new Point(7, 2));
	sites.push_back(new Point(6, 1));

	Voronoi voronoi(sites);

	voronoi.tree.PrintTree(print_node);
	return 0;
}
