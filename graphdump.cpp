#include "parser.hpp"
#include "package.hpp"
#include "container.hpp"
#include "graph.hpp"
#include <iostream>
#include <ctime>


int main(int argc, char** argv)
{
	if (argc == 1)
	{
		std::cout << "No file to parse specified" << std::endl;
		return -1;
	}
	
	std::string fl = argv[1];

	Parser p(fl);

	std::cout << "Starting" << std::endl;
	std::time_t result = std::time(nullptr);
	std::cout << std::asctime(std::localtime(&result)) << std::endl;

	p.parse();

	int size = p.getAmountOfPackages();

	Container c = p.getContainer();
	std::cout << size << std::endl;

	Graph test;

	//test.buildGraph(p.getContainer());

	//TODO: this
	//test.export();

	result = std::time(nullptr);
	std::cout << std::asctime(std::localtime(&result)) << std::endl;

	return 0;
}