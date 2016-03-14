#include "parser.hpp"
#include "package.hpp"
#include "container.hpp"
#include "graph.hpp"
#include <iostream>
#include <ctime>


int main()
{
	//Parser p("synthesis.hdlist");
	Parser p("test.hdlist");

	std::cout << "Starting" << std::endl;
	std::time_t result = std::time(nullptr);
	std::cout << std::asctime(std::localtime(&result)) << std::endl;

	p.parse();

	int size = p.getAmountOfPackages();
/*
	for (int i = 0; i < size; ++i)
	{
		Package test = p.getPackage(i);
		if (test.getReqAmount() == 0)
			std::cout << test.getInfo() << std::endl;
	}
*/

	Container c = p.getContainer();
	std::cout << size << std::endl;

	Graph test;

	//test.buildGraph(c);

	result = std::time(nullptr);
	std::cout << std::asctime(std::localtime(&result)) << std::endl;


	while(1)
	{
		std::cout << "Total amount of packages: " << size << std::endl;
		std::cout << "Print number of package to get info: ";
		int index;
		std::cin >> index;
		//Package exportpkg = c[index];
		//Package exportpkg2 = c[index+1];
		//Package importpkg;
		//Package importpkg2;
		//std::string flname = "pkgtest";
		//flname += std::to_string(index);
		//std::ofstream _fli;
		//std::ifstream _flo;
		//_fli.open(flname.c_str(), std::ofstream::out);
		//_fli << exportpkg;
		//_fli << exportpkg2;
		//_fli.close();
		//std::cout << "Source pkg" << std::endl << exportpkg2.getFullInfo() << std::endl;
		//_flo.open(flname.c_str(), std::ifstream::in);
		//_flo >> importpkg;
		//_flo >> importpkg2;
		//_flo.close();
		//std::cout << "Import pkg" << std::endl << importpkg2.getFullInfo() << std::endl;
		//std::cout << std::endl << test.printInfo(index) << std::endl;
		std::cout << std::endl << c[index].getFullInfo() << std::endl;
	}


	return 0;
}