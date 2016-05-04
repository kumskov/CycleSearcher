#include "parser.hpp"
#include "package.hpp"
#include "container.hpp"
#include "graph.hpp"
#include "cyclesearcher.hpp"
#include "cyclecontainer.hpp"
#include "parser.hpp"
#include "exporter.hpp"
#include "templatefactory.hpp"
//#include "exporterfactory.hpp"
#include <iostream>
#include <ctime>


void usageHelp()
{
	std::cout << "Usage:\t./graphdump import <target file>" << std::endl;
	std::cout << "\t./graphdump export <source file> <target file> <library file>" << std::endl;
	std::cout << "\t./graphdump fix <source file> <target file>" << std::endl;
	std::cout << "\t./graphdump cycle <source file>" << std::endl;
	std::cout << "\t./graphdump dotexport <source file> <target file>" << std::endl;
	std::cout << "\t./graphdump libtest <library file>" << std::endl;
}

void exporter(std::string flsrc, std::string flexp, std::string lib)
{
	LibFactory<Parser> f;
	f.setSymbol("getParser");
	f.load(lib);
	
	Parser* p = f.getObject();

	p->load(flsrc);

	std::time_t result = std::time(nullptr);
	std::cout << "Starting: \t" << std::asctime(std::localtime(&result));

	p->parse();

	int size = p->getAmountOfPackages();

	Container c = p->getContainer();
	std::cout << "Amount of elements:" << size << std::endl;

	Graph test;

	test.buildGraph(p->getContainer());

	test.save(flexp);

	result = std::time(nullptr);
	std::cout << "Finished: \t" << std::asctime(std::localtime(&result));

	delete p;
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
	result = std::time(nullptr);
	std::cout << "Got cycles: \t" << std::asctime(std::localtime(&result));

	CycleContainer data;
	data.loadData(worker);
	data.addFilter("lang");
	data.addFilter("data");
	//data.applyFilter(worker);

	result = std::time(nullptr);
	std::cout << "Filtered: \t" << std::asctime(std::localtime(&result));
	

	std::cout << data.getInfo() << std::endl;

	//std::cout << data.getCycleAmount() << " cycles total" << std::endl;
	//std::cout << data.getSelfCycleAmount() << " selfcycles total" << std::endl;
	while(1)
	{
		std::cout << "Input what cycle to print: ";
		int index;
		std::cin >> index;
		std::cout << data.cycleToString(index) << std::endl;
		//std::cout << worker.cycleToString(index) << std::endl;
	}
}

void dotexporter(std::string flsrc, std::string flexp, std::string fllib)
{
	
	std::time_t result = std::time(nullptr);
	std::cout << "Starting: \t" << std::asctime(std::localtime(&result));

	CycleSearcher worker;

	worker.load(flsrc);

	result = std::time(nullptr);
	std::cout << "Loaded: \t" << std::asctime(std::localtime(&result));

	//int size = worker.getAmount();
	//std::cout << "Total amount of packages: " << size << std::endl;

	worker.findCycles();
	result = std::time(nullptr);
	std::cout << "Got cycles: \t" << std::asctime(std::localtime(&result));

	CycleContainer data;
	data.loadData(worker);
	data.addFilter("lang");
	data.addFilter("data");
	//data.applyFilter(worker);

	result = std::time(nullptr);
	std::cout << "Filtered: \t" << std::asctime(std::localtime(&result));
	
	LibFactory<Exporter> expfact;
	
	expfact.setSymbol("getExporter");
	expfact.load(fllib);

	Exporter* testexport = expfact.getObject();
	//DotExporter testexport("FedoraRepo", flexp);
	testexport->setFile(flexp);
	testexport->setName("FedoraRepo");

	testexport->generateFromGraph(worker);
	std::cout << std::endl;

	result = std::time(nullptr);
	std::cout << "Generated export: \t" << std::asctime(std::localtime(&result));

	testexport->markCycles(data);

	result = std::time(nullptr);
	std::cout << "Marked cycles: \t" << std::asctime(std::localtime(&result));

	testexport->save();

	result = std::time(nullptr);
	std::cout << "Finished: \t" << std::asctime(std::localtime(&result));

	delete testexport;
	
}

void libtest(std::string flname)
{
	//ExpFactory f;
	LibFactory<Exporter> f;
	f.setSymbol("getExporter");
	f.load(flname);

	Exporter* test = f.getObject();
	//std::cout << "Factory says it's name is: " << f.getName(0) << std::endl;
	std::cout << "Class name is: " << test->getClassName() << std::endl;
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
		if (argc != 5)
		{
			std::cout << "Not enough parameters!" << std::endl;
			usageHelp();
			return -1;
		}

		std::string flexp = argv[3];
		std::string fllib = argv[4];
		exporter(fl, flexp, fllib);
		return 0;
	}
	if (option == "dotexport")
	{
		if (argc != 5)
		{
			std::cout << "Not enough parameters!" << std::endl;
			usageHelp();
			return -1;
		}

		std::string flexp = argv[3];
		std::string fllib = argv[4];
		dotexporter(fl, flexp, fllib);
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
	if (option == "libtest")
	{
		libtest(fl);
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