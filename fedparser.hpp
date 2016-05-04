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
class InFile
{
protected:
	std::string _flname;
	std::ifstream _fl;
	bool _initialized;
	int _backline;

public:
	InFile();
	InFile(std::string);
	InFile(const char*);
	~InFile();

	int getAmountOfLines();
	std::string getNextLine();
	void rewindLine();

	void open(std::string);
	void open(const char*);

	void close();

	bool isOpened() const;
};


class FedParser : public Parser
{
protected:
	InFile _repo;
	//Isn't needed since packages are kept in the container
	//std::vector<Package> _pkgs;
	Container _pkgs;
	bool _parsed;

	std::vector<std::string> split(std::string, char);
	std::string cut_version(std::string);

public:
	FedParser();
	FedParser(std::string);
	FedParser(const char*);
	~FedParser();

	void load(std::string);

	void parse();

	int getAmountOfPackages();
	Package getPackage(int);

	Container getContainer();

	std::string getClassName();
	std::string getClassDescription();

};


#endif