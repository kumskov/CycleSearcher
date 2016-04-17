#ifndef __DOTEXPORT_HPP__
#define __DOTEXPORT_HPP__
#include "package.hpp"
#include "graph.hpp"


class DotExporter
{
protected:
	std::vector<std::string> _names;
	std::vector<std::string> _encodednames;
	char _charcounter;
	int _intcounter;

	std::string _fl;

	int findName(std::string) const;

public:
	DotExporter() {}

	void addPackage(Package);
	void setFile(std::string);

	void save();

};



#endif