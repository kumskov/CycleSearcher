#include "TestParser.hpp"

//##################################################//
//													//
//						InFile						//
//													//
//##################################################//


//##################################################//
//													//
//						TestParser					//
//													//
//##################################################//

//Constructors
TestParser::TestParser()
{
	_reponame = "BL4H";
	//_repo.open(flname);
	//_pkgs = std::vector<Package>();
}

TestParser::TestParser(std::string flname)
{
	_reponame = flname;
	//_pkgs = std::vector<Package>();
}

TestParser::TestParser(const char* flname)
{
	_reponame = flname;
	//_pkgs = std::vector<Package>();
}

TestParser::~TestParser()
{

}

void TestParser::load(std::string flname)
{

}

//Start working with the file
void TestParser::parse()
{

}

Container TestParser::getContainer()
{
	return _pkgs;
}

Package TestParser::getPackage(int index)
{
	if ((index >= _pkgs.size()) ||
		(index < 0))
	{
		throw std::logic_error("TestParser: Requested package with unreachable index");
	}
	return _pkgs[index];
}

int TestParser::getAmountOfPackages()
{
	return _pkgs.size();
}


std::string TestParser::getClassName()
{
	return "\"Can't Wake Up\" Sample Parser";
}

std::string TestParser::getClassDescription()
{
	return "SAVE ME FROM THE NOTHING I'VE BECOME";
}

//dlopen hook
extern "C" Parser* getParser()
{
	return new TestParser;
}