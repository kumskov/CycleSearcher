#ifndef __DOTEXPORT_HPP__
#define __DOTEXPORT_HPP__

#include "package.hpp"
#include "graph.hpp"
#include "cyclecontainer.hpp"
#include "exporter.hpp"
#include "utils.hpp"
#include <fstream>


class DotExporter : public Exporter
{
protected:
	struct Link
	{
		std::string _path;
		int _from;
		int _to;
		std::string _dep;
		bool _loop;
	};

	struct SlotLink
	{
		std::vector<std::string> _paths;
		std::string _slotname;
		std::string _slotencname;
		std::string _link;
		int _from;
		std::vector<int> _to;
		std::vector<bool> _loop;
	};

	std::vector<std::string> _names;
	std::vector<std::string> _encodednames;

	std::vector<Link> _paths;
	std::vector<SlotLink> _slotpaths;

	int _intcounter;

	bool _slotIgnore;
	bool _markBroken;

	std::string _fl;

	std::string _graphname;

	int findName(std::string);
	std::string generateName(std::string);

	std::vector<int> transformSingleSlot(std::vector< std::vector<int> >);
	bool checkSingleSlot(std::vector< std::vector<int> >);

	void addPackage(Package);
	void generateLinks(int, std::vector<int>, Graph);
	void generateSlotLinks(int, std::vector< std::vector<int> >, Graph);

	int findSelfLink(int);
	int findLink(int, int);
	int findSlot(int, int);

public:
	DotExporter();
	DotExporter(std::string name);
	DotExporter(std::string name, std::string flname);

	void setFile(std::string);

	void setSlotIgnore(bool);
	bool getSlotIgnore();

	void setMarkBroken(bool);
	bool getMarkBroken();

	void setName(std::string);
	std::string getName();

	void generateFromGraph(Graph);
	void markCycles(CycleContainer);

	void save();

	std::string getClassName();
	std::string getClassDescription();

};



#endif