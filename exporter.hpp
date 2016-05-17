#ifndef __EXPORTER_ABSTRACT_HPP__
#define __EXPORTER_ABSTRACT_HPP__

#include <string>
#include "classinfo.hpp"
#include "cyclecontainer.hpp"
#include "graph.hpp"

class Exporter : public ClassInfo
{
public:
	virtual void setFile(std::string) = 0;

	virtual void setSlotIgnore(bool) = 0;
	virtual bool getSlotIgnore() = 0;

	virtual void setMarkBroken(bool) = 0;
	virtual bool getMarkBroken() = 0;

	virtual void setMarkAll(bool) = 0;
	virtual bool getMarkAll() = 0;

	virtual void setName(std::string) = 0;
	virtual std::string getName() = 0;

	virtual void generateFromGraph(Graph) = 0;
	virtual void markCycles(CycleContainer) = 0;

	virtual void save() = 0;

};

#endif