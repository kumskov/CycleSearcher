#ifndef __EXPORTER_FACTORY_HPP__
#define __EXPORTER_FACTORY_HPP__

#include "exporter.hpp"
#include <dlfcn.h>
#include <vector>
#include <string>

class ExpFactory
{
protected:
	typedef Exporter* creat();

	std::vector<creat*> _expTypes;
	std::vector<void*> _handles;
	std::vector<std::string> _libnames;

	int _current = -1;

	bool alreadyOpened(std::string) const;



public:
	~ExpFactory();

	void load(std::string);
	Exporter* getExporter() const;
	Exporter* getExporter(int) const;

	void setCurrent(int);
	int getCurrent() const;

	int getAmount() const;

	std::string getName(int) const;
	std::string getDescription(int) const;
	std::string getLibName(int) const;
};


#endif