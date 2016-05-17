#include "parser.hpp"
#include "exporter.hpp"

#include "package.hpp"
#include "container.hpp"
#include "graph.hpp"
#include "cyclesearcher.hpp"
#include "cyclecontainer.hpp"

#include "templatefactory.hpp"

#include <iostream>
#include <sstream>
#include <ctime>
#include <string>
#include <vector>
#include <limits>


struct MainData
{
	LibFactory<Exporter> 		ExpFactory;
	LibFactory<Parser>			ParseFactory;

	CycleSearcher* 				MainGraph = NULL;
	CycleContainer* 			MainCycles = NULL;

	std::string 				Output;
	std::string 				Prompt = "$ ";
	std::string 				GraphName = "Test";

	std::vector<std::string> 	Command;

	int 						CurrentExporter = -1;
	int 						CurrentParser = -1;

	bool 						MarkCycles = false;
	bool						MarkAll	= false;
	bool 						Exit = false;

} Data;

void parseCommand()
{
	std::string tmp;
	std::getline(std::cin, tmp);
	if (tmp.size() == 0)
	{
		return;
	}
	std::stringstream nstream;
	nstream << tmp;
	tmp = "";

	while(nstream >> tmp)
	{
		Data.Command.push_back(tmp);
	}
}

void generalHelp(std::string option = "")
{
	if (option != "")
	{
		std::cout << "Unknown option or command: \"" << option << "\"" << std::endl;
	}
	
	std::cout << "Type \"help\" to see all available commands" << std::endl;
}

void helpLoad()
{
	std::cout << "load: load shared libraries or graphs" << std::endl;
	std::cout << "Arguments for command \"load\":" << std::endl;
	std::cout << "\tparser <library path>: " << "load parser library" << std::endl;
	std::cout << "\texporter <library path>: " << "load exporter library" << std::endl;
	std::cout << "\tgraph <graph path>: " << "load graph source file" << std::endl;
}

void helpUnload()
{
	std::cout << "unload: remove loaded libraries or graph" << std::endl;
	std::cout << "Arguments for command \"unload\":" << std::endl;
	std::cout << "\tparser <int>: " << "unload parser library" << std::endl;
	std::cout << "\texporter <int>: " << "unload exporter library" << std::endl;
	std::cout << "\tgraph: " << "unload current graph" << std::endl;
}

void helpSet()
{
	std::cout << "set: configure specific options" << std::endl;
	std::cout << "Arguments for command \"set\":" << std::endl;
	std::cout << "\tparser <int>: set what parser to use" << std::endl;
	std::cout << "\texporter <int>: set what exporter to use" << std::endl;
	std::cout << "\tcycles <yes/no>: set whether analyze cycles or not" << std::endl;
	std::cout << "\tmark <all/one>: mark all links from cycled packages or just one" << std::endl;
	std::cout << "\tname <string>: set graph name" << std::endl;
}

void helpList()
{
	std::cout << "list: print specific info" << std::endl;
	std::cout << "Arguments for command \"list\":" << std::endl;
	std::cout << "\tparsers: list all loaded parsers" << std::endl;
	std::cout << "\texporters: list all loaded exporters" << std::endl;
	std::cout << "\tcycles: shows if cycles will be analyzed" << std::endl;
}

void helpHelp()
{
	std::cout << "help: Display a list of commands" << std::endl;
}

void helpPrint()
{
	std::cout << "print: print certain info" << std::endl;
	std::cout << "Arguments for command \"list\":" << std::endl;
	std::cout << "\tsize: print amount of packages" << std::endl;
	std::cout << "\tindex <int>: print package full info at specified index" << std::endl;
	std::cout << "\tsimple <int>: print simple package info at index" << std::endl;
	std::cout << "\tfind <string>: find all packages that have specified string in name" << std::endl;
	std::cout << "\tcycles amount: print how many cycles have been found" << std::endl;
	std::cout << "\tcycles index <int>: print cycle at specified index" << std::endl;
	std::cout << "\tcycles selfindex <int>: print selfcycle at specified index" << std::endl;
}

void helpDump()
{
	std::cout << "dump <file name>: In case build process takes too long export generated graph" << std::endl;
}

void helpImport()
{
	std::cout << "import <file name>: import generated graph" << std::endl;
}

void helpExport()
{
	std::cout << "export <file name>: export graph to the desired format" << std::endl;
}

void helpQuit()
{
	std::cout << "quit: quit back to terminal. Or just play Ctrl-C" << std::endl;
}

void help()
{
	std::cout << "Available commands: " << std::endl;
	std::cout << "\thelp" << std::endl;
	std::cout << "\tload" << std::endl;
	std::cout << "\tunload" << std::endl;
	std::cout << "\tset" << std::endl;
	std::cout << "\tlist" << std::endl;
	std::cout << "\tprint" << std::endl;
	std::cout << "\tdump" << std::endl;
	std::cout << "\timport" << std::endl;
	std::cout << "\texport" << std::endl;
	std::cout << "\tquit" << std::endl;
	std::cout << "Print \"help\" <command> in order to see help specific to that command" << std::endl;
}

//TODO: fix magic numbers 50
void clearOutput()
{
	std::cout << '\r';
	for (int i = 0; i < 50; ++i)
	{
		std::cout << ' ';
	}
	std::cout << '\r';
}

void helpCommand()
{
	if (Data.Command.size() == 1)
	{
		help();
		return;
	}

	std::string arg = Data.Command[1];

	if (arg == "load")
	{
		helpLoad();
		return;
	}
	if (arg == "unload")
	{
		helpUnload();
		return;
	}
	if (arg == "list")
	{
		helpList();
		return;
	}
	if (arg == "set")
	{
		helpSet();
		return;
	}
	if (arg == "print")
	{
		helpPrint();
		return;
	}
	if (arg == "dump")
	{
		helpDump();
		return;
	}
	if (arg == "import")
	{
		helpImport();
		return;
	}
	if (arg == "export")
	{
		helpExport();
		return;
	}
	if (arg == "quit")
	{
		helpQuit();
		return;
	}
	if (arg == "help")
	{
		helpHelp();
		return;
	}

	generalHelp(arg);
}

void quitCommand()
{
	Data.Exit = true;
	return;
}

void loadParser()
{
	if (Data.Command.size() == 2)
	{
		std::cout << "load: invalid amount of parameters" << std::endl;
		return;
	}

	std::time_t result = std::time(nullptr);
	std::cout << "Loading lib: \t" << std::asctime(std::localtime(&result));

	std::string str = Data.Command[2];

	Data.ParseFactory.load(str);
	Data.CurrentParser = Data.ParseFactory.getAmount() - 1;

	result = std::time(nullptr);
	std::cout << "Loaded!  \t" << std::asctime(std::localtime(&result));
}

void loadExporter()
{
	if (Data.Command.size() == 2)
	{
		std::cout << "load: invalid amount of parameters" << std::endl;
		return;
	}

	std::time_t result = std::time(nullptr);
	std::cout << "Loading lib: \t" << std::asctime(std::localtime(&result));

	std::string str = Data.Command[2];

	Data.ExpFactory.load(str);
	Data.CurrentExporter = Data.ExpFactory.getAmount() - 1;

	result = std::time(nullptr);
	std::cout << "Loaded!  \t" << std::asctime(std::localtime(&result));
}

void loadGraph()
{
	if (Data.Command.size() == 2)
	{
		std::cout << "load: invalid amount of parameters" << std::endl;
		return;
	}

	std::string str = Data.Command[2];

	if (Data.MainGraph != NULL)
	{
		std::cout << "load graph: You need to unload loaded graph first!" << std::endl;
		return;
	}

	if (Data.CurrentParser == -1)
	{
		std::cout << "load graph: You need to set parser first!" << std::endl;
		return;
	}

	std::time_t result = std::time(nullptr);
	std::cout << "Parsing: \t" << std::asctime(std::localtime(&result));

	Parser* p = Data.ParseFactory.getObject(Data.CurrentParser);

	p->load(str);

	p->parse();

	result = std::time(nullptr);
	std::cout << "Parsed!  \t" << std::asctime(std::localtime(&result));

	result = std::time(nullptr);
	std::cout << "Generating:\t" << std::asctime(std::localtime(&result));

	Data.MainGraph = new CycleSearcher;

	Data.MainGraph->buildGraph(p->getContainer());

	clearOutput();

	result = std::time(nullptr);
	std::cout << "Generated!\t" << std::asctime(std::localtime(&result));

	delete p;
}

void loadCommand()
{
	if (Data.Command.size() == 1)
	{
		std::cout << "load: invalid amount of parameters" << std::endl;
		return;
	}

	std::string str = Data.Command[1];

	if (str == "parser")
	{
		loadParser();
		return;
	}
	if (str == "exporter")
	{
		loadExporter();
		return;
	}
	if (str == "graph")
	{
		loadGraph();
		return;
	}

	generalHelp(str);
}

void unloadParser()
{
	if (Data.Command.size() == 2)
	{
		std::cout << "unload: invalid amount of parameters" << std::endl;
		return;
	}

	std::string str = Data.Command[2];

	int index = std::stoi(str, nullptr, 10);

	Data.ParseFactory.unload(index);

	Data.CurrentParser = -1;
}

void unloadExporter()
{
	if (Data.Command.size() == 2)
	{
		std::cout << "unload: invalid amount of parameters" << std::endl;
		return;
	}

	std::string str = Data.Command[2];

	int index = std::stoi(str, nullptr, 10);

	Data.ExpFactory.unload(index);

	Data.CurrentExporter = -1;
}

void unloadGraph()
{
	if (Data.MainGraph == NULL)
	{
		std::cout << "Nothing is loaded as a graph!" << std::endl;
		return;
	}

	delete Data.MainGraph;
	Data.MainGraph = NULL;
}

void unloadCommand()
{
	if (Data.Command.size() == 1)
	{
		std::cout << "unload: invalid amount of parameters" << std::endl;
		return;
	}

	std::string str = Data.Command[1];

	if (str == "parser")
	{
		unloadParser();
		return;
	}
	if (str == "exporter")
	{
		unloadExporter();
		return;
	}
	if (str == "graph")
	{
		unloadGraph();
		return;
	}

	generalHelp(str);
}

void setParser()
{
	if (Data.Command.size() == 2)
	{
		std::cout << "set: invalid amount of parameters" << std::endl;
		return;
	}

	std::string str = Data.Command[2];

	int index = std::stoi(str, nullptr, 10);

	if (((index - 1) < 0) || (index - 1) >= Data.ParseFactory.getAmount())
	{
		std::cout << "set parser: No parser at selected index" << std::endl;
		return;
	}

	Data.CurrentParser = index - 1;
}

void setExporter()
{
	if (Data.Command.size() == 2)
	{
		std::cout << "set: invalid amount of parameters" << std::endl;
		return;
	}

	std::string str = Data.Command[2];

	int index = std::stoi(str, nullptr, 10);

	if (((index - 1) < 0) || (index - 1) >= Data.ExpFactory.getAmount())
	{
		std::cout << "set exporter: No exporter at selected index" << std::endl;
		return;
	}

	Data.CurrentExporter = index - 1;
}

void setCycles()
{
	if (Data.Command.size() == 2)
	{
		std::cout << "set: invalid amount of parameters" << std::endl;
		return;
	}

	std::string str = Data.Command[2];

	if (str == "yes")
	{
		if (Data.MainGraph == NULL)
		{
			std::cout << "You need to load graph first!" << std::endl;
			return;
		}

		Data.MarkCycles = true;
		if (Data.MainCycles != NULL)
		{
			delete Data.MainCycles;
		}

		CycleSearcher* worker = (CycleSearcher*) Data.MainGraph;
		
		std::time_t result = std::time(nullptr);
		std::cout << "Finding cycles: \t" << std::asctime(std::localtime(&result));

		worker->findCycles();

		result = std::time(nullptr);
		std::cout << "Got cycles: \t\t" << std::asctime(std::localtime(&result));

		Data.MainCycles = new CycleContainer();
		Data.MainCycles->loadData(*worker);
		return;

	}
	if (str == "no")
	{
		Data.MarkCycles = false;
		if (Data.MainCycles != NULL)
		{
			delete Data.MainCycles;
			Data.MainCycles = NULL;
		}
		return;
	}


	generalHelp(str);
}

void setName()
{
	if (Data.Command.size() == 2)
	{
		std::cout << "set: invalid amount of parameters" << std::endl;
		return;
	}

	std::string str = Data.Command[2];

	Data.GraphName = str;
}

void setMark()
{
	if (Data.Command.size() == 2)
	{
		std::cout << "set: invalid amount of parameters" << std::endl;
		return;
	}

	std::string str = Data.Command[2];

	if (str == "all")
	{
		Data.MarkAll = true;
		return;
	}
	if (str == "one")
	{
		Data.MarkAll = false;
		return;
	}

	std::cout << "set mark: invalid option \"" << str << "\"" << std::endl;
}

void setCommand()
{
	if (Data.Command.size() == 1)
	{
		std::cout << "set: invalid amount of parameters" << std::endl;
		return;
	}

	std::string str = Data.Command[1];

	if (str == "parser")
	{
		setParser();
		return;
	}
	if (str == "exporter")
	{
		setExporter();
		return;
	}
	if (str == "cycles")
	{
		setCycles();
		return;
	}
	if (str == "name")
	{
		setName();
		return;
	}
	if (str == "mark")
	{
		setMark();
		return;
	}

	generalHelp(str);
}

void listParsers()
{
	if (Data.ParseFactory.getAmount() == 0)
	{
		std::cout << "There are no parsers loaded!" << std::endl;
		return;
	}

	for (int i = 0; i < Data.ParseFactory.getAmount(); ++i)
	{
		Parser* p = Data.ParseFactory.getObject(i);
		std::cout 	<< "#" << i+1 << " [" \
					<< (i == Data.CurrentParser ? "CURRENT" : "      ") \
					<< "]: " << p->getClassName() << std::endl;
	}
}

void listExporters()
{
	if (Data.ExpFactory.getAmount() == 0)
	{
		std::cout << "There are no exporters loaded!" << std::endl;
		return;
	}

	for (int i = 0; i < Data.ExpFactory.getAmount(); ++i)
	{
		Exporter* e = Data.ExpFactory.getObject(i);
		std::cout 	<< "#" << i+1 << " [" \
					<< (i == Data.CurrentExporter ? "CURRENT" : "       ") \
					<< "]: " << e->getClassName() << std::endl;
	}
}

void listCycles()
{
	std::cout << "Cycles will " << (Data.MarkCycles ? "NOT" : "") << " be marked" << std::endl;
	return;
}

void listCommand()
{
	if (Data.Command.size() == 1)
	{
		std::cout << "list: invalid amount of parameters" << std::endl;
		return;
	}

	std::string str = Data.Command[1];

	if (str == "parsers")
	{
		listParsers();
		return;
	}
	if (str == "exporters")
	{
		listExporters();
		return;
	}
	if (str == "cycles")
	{
		listCycles();
		return;
	}

	generalHelp(str);
}

void printSize()
{
	if (Data.MainGraph == NULL)
	{
		std::cout << "No graph is loaded!" << std::endl;
		return;
	}

	std::cout << "Amount of packages loaded in graph: " << Data.MainGraph->getAmount() << std::endl;
}

void printIndex()
{
	if (Data.MainGraph == NULL)
	{
		std::cout << "No graph is loaded!" << std::endl;
		return;
	}

	if (Data.Command.size() == 2)
	{
		std::cout << "print: invalid amount of parameters" << std::endl;
		return;
	}

	std::string str = Data.Command[2];

	int index = std::stoi(str, nullptr, 10);
	//std::cout << index << std::endl;

	std::cout << Data.MainGraph->printInfo(index) << std::endl;
}

void printSimple()
{
	if (Data.MainGraph == NULL)
	{
		std::cout << "No graph is loaded!" << std::endl;
		return;
	}

	if (Data.Command.size() == 2)
	{
		std::cout << "print: invalid amount of parameters" << std::endl;
		return;
	}

	std::string str = Data.Command[2];

	int index = std::stoi(str, nullptr, 10);

	std::cout << Data.MainGraph->operator[](index).getFullInfo() << std::endl;	
}

void printFind()
{
	if (Data.MainGraph == NULL)
	{
		std::cout << "No graph is loaded!" << std::endl;
		return;
	}

	if (Data.Command.size() == 2)
	{
		std::cout << "print: invalid amount of parameters" << std::endl;
		return;
	}

	std::string str = Data.Command[2];

	std::vector<int> found = Data.MainGraph->find(str);
	
	for (int j = 0; j < found.size(); ++j)
	{
		//std::cout << found[j] << ": " << imported[found[j]].getName() << std::endl;
		std::cout << found[j] << ": " << Data.MainGraph->operator[](found[j]).getName() << std::endl;
	}
}

void printCycleAmount()
{
	if ((!Data.MarkCycles) || (Data.MainCycles == NULL))
	{
		std::cout << "Cycles have not been marked, type \"set cycles yes\" in order to mark them" << std::endl;
		return;
	}
	std::cout << "There are " << Data.MainCycles->getCycleAmount() << " cycles and " << Data.MainCycles->getSelfCycleAmount() << " selfcycles in this graph" << std::endl;
}

void printCycleIndex()
{
	if ((!Data.MarkCycles) || (Data.MainCycles == NULL))
	{
		std::cout << "Cycles have not been marked, type \"set cycles yes\" in order to mark them" << std::endl;
		return;
	}

	if (Data.Command.size() == 3)
	{
		std::cout << "print cycles index: invalid amount of parameters" << std::endl;
		return;
	}

	std::string str = Data.Command[3];

	int index = std::stoi(str, nullptr, 10);

	std::cout << Data.MainCycles->cycleToString(index) << std::endl;
}

void printCycleSelfindex()
{
	if ((!Data.MarkCycles) || (Data.MainCycles == NULL))
	{
		std::cout << "Cycles have not been marked, type \"set cycles yes\" in order to mark them" << std::endl;
		return;
	}

	if (Data.Command.size() == 3)
	{
		std::cout << "print cycles selfindex: invalid amount of parameters" << std::endl;
		return;
	}

	std::string str = Data.Command[3];

	int index = std::stoi(str, nullptr, 10);

	std::cout << index << ": package " << Data.MainCycles->getSelfCycles()[index] << " is cycling at itself" << std::endl;
}

void printCyclesCommand()
{
	if (Data.Command.size() == 2)
	{
		std::cout << "print cycles: invalid amount of parameters" << std::endl;
		return;
	}

	std::string str = Data.Command[2];

	if (str == "amount")
	{
		printCycleAmount();
		return;
	}
	if (str == "index")
	{
		printCycleIndex();
		return;
	}
	if (str == "selfindex")
	{
		printCycleSelfindex();
		return;
	}

	generalHelp(str);
}

void printCommand()
{
	if (Data.Command.size() == 1)
	{
		std::cout << "print: invalid amount of parameters" << std::endl;
		return;
	}

	std::string str = Data.Command[1];

	if (str == "size")
	{
		printSize();
		return;
	}
	if (str == "index")
	{
		printIndex();
		return;
	}
	if (str == "simple")
	{
		printSimple();
		return;
	}
	if (str == "find")
	{
		printFind();
		return;
	}
	if (str == "cycles")
	{
		printCyclesCommand();
		return;
	}

	generalHelp(str);
}

void dumpCommand()
{
	if (Data.MainGraph == NULL)
	{
		std::cout << "dump: Graph has not been loaded yet!" << std::endl;
		return;
	}

	if (Data.Command.size() == 1)
	{
		std::cout << "dump: invalid amount of parameters" << std::endl;
		return;
	}

	std::time_t result = std::time(nullptr);
	std::cout << "Starting: \t" << std::asctime(std::localtime(&result));

	std::string str = Data.Command[1];

	Data.MainGraph->save(str);

	result = std::time(nullptr);
	std::cout << "Saved: \t" << std::asctime(std::localtime(&result));
}

void importCommand()
{
	if (Data.MainGraph != NULL)
	{
		std::cout << "import: Graph has already been loaded!" << std::endl;
		return;
	}

	if (Data.Command.size() == 1)
	{
		std::cout << "import: invalid amount of parameters" << std::endl;
		return;
	}

	std::string str = Data.Command[1];

	Data.MainGraph = new CycleSearcher;

	Data.MainGraph->load(str);
}

void exportCommand()
{
	if (Data.MainGraph == NULL)
	{
		std::cout << "export: Graph has not been loaded yet!" << std::endl;
		return;
	}

	if (Data.Command.size() == 1)
	{
		std::cout << "export: invalid amount of parameters" << std::endl;
		return;
	}

	if (Data.CurrentExporter == -1)
	{
		std::cout << "export: You need to set exporter first!" << std::endl;
		return;
	}

	std::time_t result = std::time(nullptr);
	std::cout << "Starting export: \t" << std::asctime(std::localtime(&result));

	std::string str = Data.Command[1];

	Exporter* e = Data.ExpFactory.getObject(Data.CurrentExporter);

	e->setName(Data.GraphName);
	e->setFile(str);
	e->setMarkAll(Data.MarkAll);
	e->generateFromGraph(*Data.MainGraph);
	clearOutput();

	result = std::time(nullptr);
	std::cout << "Generated graph: \t" << std::asctime(std::localtime(&result));

	if (Data.MainCycles != NULL)
	{
		result = std::time(nullptr);
		std::cout << "Marking cycles: \t" << std::asctime(std::localtime(&result));

		e->markCycles(*Data.MainCycles);
	}
	
	std::cout << "Finished export: \t" << std::asctime(std::localtime(&result));

	e->save();

	delete e;
}

void mainBranch()
{
	parseCommand();

	/*
	for (int i = 0; i < Data.Command.size(); ++i)
	{
		std::cout << Data.Command[i] << ' ';
	}
	std::cout << std::endl;
	*/

	if (Data.Command.size() == 0)
	{
		return;
	}

	std::string first = Data.Command[0];

	if (first == "help")
	{
		helpCommand();
		return;
	}
	if (first == "quit")
	{
		quitCommand();
		return;
	}
	if (first == "load")
	{
		loadCommand();
		return;
	}
	if (first == "unload")
	{
		unloadCommand();
		return;
	}
	if (first == "set")
	{
		setCommand();
		return;
	}
	if (first == "list")
	{
		listCommand();
		return;
	}
	if (first == "print")
	{
		printCommand();
		return;
	}
	if (first == "dump")
	{
		dumpCommand();
		return;
	}
	if (first == "export")
	{
		exportCommand();
		return;
	}
	if (first == "import")
	{
		importCommand();
		return;
	}


	generalHelp(first);
}

void clearInput()
{
	//std::cin.clear();
	//std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main()
{
	Data.ExpFactory.setSymbol("getExporter");
	Data.ParseFactory.setSymbol("getParser");

	while(!Data.Exit)
	{
		try
		{
			std::cout << Data.Prompt;
			mainBranch();
			clearInput();
			Data.Command.clear();
		}
		catch(std::exception& ex)
		{
			std::cout << "An error occured: " << ex.what() << std::endl;
			Data.Command.clear();
		}
	}
	
}