#ifndef __GRAPH_HPP__
#define __GRAPH_HPP__

#include "package.hpp"
#include "parser.hpp"
#include <iostream>



class Graph
{
protected:
	bool alreadyExistsIn(std::vector<int>, int) const;

	struct GraphNode
	{
		struct ReqSlot
		{
			std::vector<int> _possibleMatch;

			ReqSlot() {};
		};

		std::vector<ReqSlot> _requires;
		std::vector<int> _providesFor;
		Package _pkg;
		bool _brokenDep;

		GraphNode() {};
		GraphNode(Package src)
		{
			_pkg = src;
			_brokenDep = false;
		}
		GraphNode(const GraphNode& src)
		{
			_pkg = src._pkg;
			_brokenDep = src._brokenDep;
			_requires = src._requires;
			_providesFor = src._providesFor;
		}
	};

	//Vector to store all graph links
	std::vector<GraphNode> _ingraph;

	void fillProvidesFor();
	void fillRequires();

	//For fillers
	std::vector<int> findNodesProviding(std::string) const;
	std::vector<int> findNodesRequiring(std::string) const;

	//void debugPrintNodeVectors(int index) const;

	void writeNode(std::ofstream&, const GraphNode&) const;
	GraphNode readNode(std::ifstream&) const;

public:
	Graph();

	//Optimize
	//hash map of table (provides -> pkg)
	void buildGraph(Container);

	int getAmount() const;
	bool inGraph(const Package) const;

	std::vector<int> find(const std::string) const;

	const Package& operator[](int) const;

	std::string printInfo(int index) const;

	//Clean duplicate node links from old built graphs
	void cleanDuplicates();

	void save(std::string flname) const;
	void load(std::string flname);

	std::vector<int> getProvides(int) const;
	std::vector< std::vector<int> > getRequires(int) const;
};


#endif