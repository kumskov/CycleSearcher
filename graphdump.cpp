#include "parser.hpp"
#include "package.hpp"
#include "container.hpp"
#include "graph.hpp"
#include "cyclesearcher.hpp"
#include <iostream>
#include <ctime>


void usageHelp()
{
	std::cout << "Usage:\t./graphdump import <export file>" << std::endl;
	std::cout << "\t./graphdump export <source file> <export file>" << std::endl;
	std::cout << "\t./graphdump fix <source file> <export file>" << std::endl;
	std::cout << "\t./graphdump cycle <source file>" << std::endl;
}

void exporter(std::string flsrc, std::string flexp)
{
	Parser p(flsrc);

	std::time_t result = std::time(nullptr);
	std::cout << "Starting: \t" << std::asctime(std::localtime(&result));

	p.parse();

	int size = p.getAmountOfPackages();

	Container c = p.getContainer();
	std::cout << "Amount of elements:" << size << std::endl;

	Graph test;

	test.buildGraph(p.getContainer());

	test.save(flexp);

	result = std::time(nullptr);
	std::cout << "Finished: \t" << std::asctime(std::localtime(&result));
}

void importerHelp()
{
	std::cout << "help";
	std::cout << "\t print all available commands" << std::endl;
	std::cout << "index";
	std::cout << "\t print full package and dep info at specified index" << std::endl;
	std::cout << "simple";
	std::cout << "\t print package with primitive deps" << std::endl;
	std::cout << "find";
	std::cout << "\t find all packages with specified string" << std::endl;
	std::cout << "size";
	std::cout << "\t print total amount of packages" << std::endl;
	std::cout << "exit";
	std::cout << "\t finish working with graph" << std::endl;
}

void importer(std::string flname)
{
	std::time_t result = std::time(nullptr);
	std::cout << "Starting: \t" << std::asctime(std::localtime(&result));

	Graph imported;

	imported.load(flname);

	result = std::time(nullptr);
	std::cout << "Loaded: \t" << std::asctime(std::localtime(&result));

	int size = imported.getAmount();
	std::cout << "Total amount of packages: " << size << std::endl;

	while(1)
	{
		std::cout << "cmd: ";
		std::string param;
		std::cin >> param;
		if (param == "help")
		{
			importerHelp();
			continue;
		}
		if (param == "index")
		{
			int index;
			std::cin >> index;
			std::cout << std::endl << imported.printInfo(index) << std::endl;
			continue;
		}
		if (param == "simple")
		{
			int index;
			std::cin >> index;
			std::cout << std::endl << imported[index].getFullInfo() << std::endl;
			continue;
		}
		if (param == "exit")
		{
			return;
		}
		if (param == "size")
		{
			std::cout << "Total amount of packages: " << size << std::endl;
			continue;
		}
		if (param == "find")
		{
			std::string findstr;
			std::cin >> findstr;
			std::vector<int> found = imported.find(findstr);
			for (int j = 0; j < found.size(); ++j)
			{
				std::cout << found[j] << ": " << imported[found[j]].getName() << std::endl;
			}
			continue;
		}
		std::cout << "Unknown command: " << param << std::endl;
		std::cout << "Type \"help\" to list all available commands" << std::endl;
		
	}
}

void fixer(std::string flsrc, std::string flexp)
{
	std::time_t result = std::time(nullptr);
	std::cout << "Starting: \t" << std::asctime(std::localtime(&result));

	Graph imported;

	imported.load(flsrc);

	result = std::time(nullptr);
	std::cout << "Loaded: \t" << std::asctime(std::localtime(&result));

	imported.cleanDuplicates();

	std::cout << "Fixed: \t" << std::asctime(std::localtime(&result));

	imported.save(flexp);

	result = std::time(nullptr);
	std::cout  <<"Finished: \t" << std::asctime(std::localtime(&result));
}

void cycler(std::string flname)
{
	std::time_t result = std::time(nullptr);
	std::cout << "Starting: \t" << std::asctime(std::localtime(&result));

	CycleSearcher worker;

	worker.load(flname);

	result = std::time(nullptr);
	std::cout << "Loaded: \t" << std::asctime(std::localtime(&result));

	//int size = worker.getAmount();
	//std::cout << "Total amount of packages: " << size << std::endl;

	worker.findCycles();
	CycleContainer data = worker.getCycleContainer();

	result = std::time(nullptr);
	std::cout << "Got cycles: \t" << std::asctime(std::localtime(&result));


	std::cout << data.getCycleAmount() << " cycles total" << std::endl;
	std::cout << data.getSelfCycleAmount() << " selfcycles total" << std::endl;
	while(1)
	{
		std::cout << "Input what cycle to print: ";
		int index;
		std::cin >> index;
		std::cout << data.cycleToString(index) << std::endl;
		//std::cout << worker.cycleToString(index) << std::endl;
	}
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
	if (option == "cycle")
	{
		cycler(fl);
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