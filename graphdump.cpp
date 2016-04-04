#include "parser.hpp"
#include "package.hpp"
#include "container.hpp"
#include "graph.hpp"
#include <iostream>
#include <ctime>


void usageHelp()
{
	std::cout << "Usage:\t./graphdump import <export file>" << std::endl;
	std::cout << "\t./graphdump export <source file> <export file>" << std::endl;
	std::cout << "\t./graphdump fix <source file> <export file>" << std::endl;
}

void exporter(std::string flsrc, std::string flexp)
{
	Parser p(flsrc);

	std::time_t result = std::time(nullptr);
	std::cout << std::asctime(std::localtime(&result)) << ": Starting" << std::endl;

	p.parse();

	int size = p.getAmountOfPackages();

	Container c = p.getContainer();
	std::cout << "Amount of elements:" << size << std::endl;

	Graph test;

	test.buildGraph(p.getContainer());

	test.save(flexp);

	result = std::time(nullptr);
	std::cout << std::asctime(std::localtime(&result)) << ": Finished" << std::endl;
}

void importer(std::string flname)
{
	std::time_t result = std::time(nullptr);
	std::cout << std::asctime(std::localtime(&result)) << ": Starting" << std::endl;

	Graph imported;

	imported.load(flname);

	result = std::time(nullptr);
	std::cout << std::asctime(std::localtime(&result)) << ": Loaded succesfully!" << std::endl;

	int size = imported.getAmount();

	while(1)
	{
		std::cout << "Total amount of packages: " << size << std::endl;
		std::cout << "Print number of package to get info: ";
		int index;
		std::cin >> index;
		std::cout << std::endl << imported.printInfo(index) << std::endl;
	}
}

void fixer(std::string flsrc, std::string flexp)
{
	std::time_t result = std::time(nullptr);
	std::cout << std::asctime(std::localtime(&result)) << ": Starting" << std::endl;

	Graph imported;

	imported.load(flsrc);

	result = std::time(nullptr);
	std::cout << std::asctime(std::localtime(&result)) << ": Loaded succesfully!" << std::endl;

	imported.cleanDuplicates();

	std::cout << std::asctime(std::localtime(&result)) << ": Fixed succesfully!" << std::endl;

	imported.save(flexp);

	result = std::time(nullptr);
	std::cout << std::asctime(std::localtime(&result)) << ": Finished!" << std::endl;
}

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		usageHelp();
		return -1;
	}
	
	std::string option = argv[1];
	std::string fl = argv[2];

	if (option == "export")
	{
		if (argc != 4)
		{
			std::cout << "Not enough parameters!" << std::endl;
			usageHelp();
			return -1;
		}

		std::string flexp = argv[3];
		exporter(fl, flexp);
		return 0;
	}
	if (option == "import")
	{
		importer(fl);
		return 0;
	}
	if (option == "fix")
	{
		if (argc != 4)
		{
			std::cout << "Not enough parameters!" << std::endl;
			usageHelp();
			return -1;
		}

		std::string flexp = argv[3];
		fixer(fl, flexp);
		return 0;
	}

	std::cout << "Unknown option: " << option << std::endl;
	usageHelp();
	
	return -1;
}