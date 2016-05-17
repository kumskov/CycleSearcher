#ifndef __PARSER_HPP__
#define __PARSER_HPP__


//Default libs
#include <fstream>
#include <string>
#include <stdexcept>
#include <vector>

//My libs
#include "package.hpp"
#include "container.hpp"
#include "parser.hpp"

//Temporary
#include <iostream>


//Classes

class TestParser : public Parser
{
protected:
	std::string _reponame;
	//Isn't needed since packages are kept in the container
	//std::vector<Package> _pkgs;
	Container _pkgs;
	bool _parsed;

public:
	TestParser();
	TestParser(std::string);
	TestParser(const char*);
	~TestParser();

	void load(std::string);

	void parse();

	int getAmountOfPackages();
	Package getPackage(int);

	Container getContainer();

	std::string getClassName();
	std::string getClassDescription();

};


#endif