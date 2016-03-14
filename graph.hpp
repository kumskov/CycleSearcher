#ifndef __GRAPH_HPP__
#define __GRAPH_HPP__

#include "package.hpp"
#include "parser.hpp"


class Graph
{
protected:

	class GraphNode
	{
	public:
		std::vector<int> _requires;
		std::vector<int> _providesFor;
		Package _pkg;
		bool _brokenDep;

		GraphNode(Package src)
		{
			_pkg = src;
			_brokenDep = false;
		}
	};


	//Vector to store all graph links
	std::vector<GraphNode> _ingraph;

	void fillProvidesFor();
	void fillRequires();

	//For later
	int findNodeProviding(std::string) const;
	int findNodeRequiring(std::string) const;

	void debugPrintNodeVectors(int index) const;


public:
	Graph();

	void buildGraph(Container);

	int getAmount() const;
	bool inGraph(const Package) const;

	const Package& operator[](int) const;

	std::string printInfo(int index) const;

};


#endif