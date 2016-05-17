#ifndef __DOTEXPORT_HPP__
#define __DOTEXPORT_HPP__

#include "package.hpp"
#include "graph.hpp"
#include "cyclecontainer.hpp"
#include "exporter.hpp"
#include "utils.hpp"
#include <fstream>


class TestExporter : public Exporter
{
protected:
	Graph _tmp;

	int _intcounter;

	bool _slotIgnore;
	bool _markBroken;
	bool _markAllLinkCycles;

	std::string _fl;

	std::string _graphname;


public:
	TestExporter();
	TestExporter(std::string name);
	TestExporter(std::string name, std::string flname);

	void setFile(std::string);

	void setSlotIgnore(bool);
	bool getSlotIgnore();

	void setMarkBroken(bool);
	bool getMarkBroken();

	void setMarkAll(bool);
	bool getMarkAll();

	void setName(std::string);
	std::string getName();

	void generateFromGraph(Graph);
	void markCycles(CycleContainer);

	void save();

	std::string getClassName();
	std::string getClassDescription();

};



#endif