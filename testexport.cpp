#include "testexport.hpp"



TestExporter::TestExporter() 
{
	_slotIgnore = false;
	_markBroken = true;
	_markAllLinkCycles = false;
}

TestExporter::TestExporter(std::string name)
{
	_graphname = name;
	_slotIgnore = false;
	_markBroken = true;
	_markAllLinkCycles = false;
}

TestExporter::TestExporter(std::string name, std::string flname)
{
	_fl = flname;
	_graphname = name;
	_slotIgnore = false;
	_markBroken = true;
	_markAllLinkCycles = false;
}


void TestExporter::setFile(std::string flname)
{
	_fl = flname;
}

void TestExporter::setSlotIgnore(bool mark)
{
	_slotIgnore = mark;
}

bool TestExporter::getSlotIgnore()
{
	return _slotIgnore;
}

void TestExporter::setMarkBroken(bool mark)
{
	_markBroken = mark;
}

bool TestExporter::getMarkBroken()
{
	return _markBroken;
}

void TestExporter::setName(std::string name)
{
	_graphname = name;
}

std::string TestExporter::getName()
{
	return _graphname;
}

void TestExporter::setMarkAll(bool mark)
{
	_markAllLinkCycles = mark;
}

bool TestExporter::getMarkAll()
{
	return _markAllLinkCycles;
}

void TestExporter::generateFromGraph(Graph src)
{
	_tmp = src;
}


void TestExporter::markCycles(CycleContainer src)
{
	
}

void TestExporter::save()
{
	std::ofstream dotfile;
	dotfile.open(_fl.c_str(), std::ofstream::out);

	for (int i = 0; i < _tmp.getAmount(); ++i)
	{
		dotfile << _tmp[i].getFullInfo() << std::endl;
	}

	dotfile.close();

}

std::string TestExporter::getClassName()
{
	return "Simple Package Info printer";
}

std::string TestExporter::getClassDescription()
{
	return "WAKE ME UP INSIDE";
}

//dlopen hook
extern "C" Exporter* getExporter()
{
	return new TestExporter;
}