#ifndef __PARSER_ABSTARCT_HPP__
#define __PARSER_ABSTARCT_HPP__

#include <string>
#include "package.hpp"
#include "container.hpp"
#include "classinfo.hpp"

class Parser : public ClassInfo
{
public:
	virtual void load(std::string) = 0;

	virtual void parse() = 0;

	virtual int getAmountOfPackages() = 0;
	virtual Package getPackage(int) = 0;

	virtual Container getContainer() = 0;
};

#endif