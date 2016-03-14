#include "parser.hpp"

//##################################################//
//													//
//						InFile						//
//													//
//##################################################//

//Constructors
InFile::InFile()
{
	_initialized = false;
}

InFile::InFile(std::string flname)
{
	_flname = flname;
	_fl.open(flname.c_str(), std::ifstream::in);
	_initialized = true;
}

InFile::InFile(const char* flname)
{
	_flname = std::string(flname);
	_fl.open(flname, std::ifstream::in);
	_initialized = true;
}

//Need to close file if object is disposed
InFile::~InFile()
{
	_fl.close();
	_initialized = false;
}

//Not the best implementation, but it's a start
int InFile::getAmountOfLines()
{
	if (!_initialized)
	{
		throw std::logic_error("Can\'t work without file!");
	}

	int linecount = 0;
	std::string tmp;
	//Basicly I just count the amount of lines
	while (std::getline(_fl, tmp))
	{
		linecount++;
	}

	//And rewind the reader to the start of the file
	_fl.clear();
	_fl.seekg(_fl.beg);

	return linecount;
}

//Main read method from file
std::string InFile::getNextLine()
{
	if (!_initialized)
	{
		throw std::logic_error("Can\'t work without file!");
	}

	//Since it's used in a cycle it's good for now
	//but for some reason it refuses to work after
	//getAmountOfLines
	//
	//Needs to be fixed later
	_backline = _fl.tellg();

	if (true)//!_fl.good())
	{
		std::string ret;
		std::getline(_fl, ret);
		//std::cout << ret << std::endl;
		return ret;
	}
	else
	{
		throw std::logic_error("Corrupt file stream");
	}
}

void InFile::rewindLine()
{
	_fl.seekg(_backline);
}

//Reopen the file, reset flags
void InFile::open(std::string flname)
{
	_fl.close();
	_fl.open(flname.c_str(), std::ifstream::in);
	_fl.clear();
	_fl.seekg(_fl.beg);
	_initialized = true;
}

void InFile::open(const char* flname)
{
	_fl.close();
	_fl.open(flname, std::ifstream::in);
	_fl.clear();
	_fl.seekg(_fl.beg);
	_initialized = true;
}

//##################################################//
//													//
//						Parser						//
//													//
//##################################################//

//Constructors
Parser::Parser(std::string flname)
{
	_repo.open(flname);
	//_pkgs = std::vector<Package>();
}

Parser::Parser(const char* flname)
{
	_repo.open(flname);
	//_pkgs = std::vector<Package>();
}

Parser::~Parser()
{

}

//Start working with the file
void Parser::parse()
{
	int len = _repo.getAmountOfLines();

	int cnt = 0;
	//Loop for every single line
	while(cnt < len)//(cnt < len)
	{
		bool gotprovides = false;
		bool gotrequires = false;
		bool gotsummary = false;
		bool gotinfo = false;

		std::string provides;
		std::string requires;
		std::string summary;
		std::string info;

		//Loops while we don't get everything needed or meet next summary line
		//May be broken from inside
		while ((
			(!gotprovides) 	|| 
			(!gotrequires) 	|| 
			(!gotinfo)		|| 
			(!gotsummary)) 	&& 
			(cnt < len))
		{
			std::string tmp = _repo.getNextLine();
			cnt++;

			if (tmp.find("@provides@") != std::string::npos)
			{
				//In case we are already in a new package
				//This construction occurs in all 3 tests
				//so all of then are unnesessary to comment
				if (gotprovides)
				{
					//Get back 1 line
					_repo.rewindLine();
					cnt--;
					break;
				}
				provides = tmp.substr(10);
				//std::cout << provides << "prov\n";
				gotprovides = true;
				continue;
			}
			
			if (tmp.find("@requires@") != std::string::npos)
			{
				if (gotrequires)
				{
					_repo.rewindLine();
					cnt--;
					break;
				}
				requires = tmp.substr(10);
				//std::cout << requires << "req\n";
				gotrequires = true;
				continue;
			}

			if (tmp.find("@summary@") != std::string::npos)
			{
				if (gotsummary)
				{
					_repo.rewindLine();
					cnt--;
					break;
				}

				summary = tmp.substr(9);
				//std::cout << summary << "sum\n";
				gotsummary = true;
				continue;
			}

			if (tmp.find("@info@") != std::string::npos)
			{
				if (gotinfo)
				{
					_repo.rewindLine();
					cnt--;
					break;
				}

				info = split(tmp.substr(6), '@')[0];
				//std::cout << info << "info\n";
				gotinfo = true;
				continue;
			}
		}

		//std::cout << "\ngot package\n";

		Package npkg(info, summary);
		//std::cout << summary << std::endl;
		std::vector<std::string> req = split(requires, '@');
		for (int i = 0; i < req.size(); ++i)
		{
			npkg.addRequire(req[i]);
			//std::cout << "Requires: " << req[i] << std::endl;
		}
		std::vector<std::string> prov = split(provides, '@');
		for (int i = 0; i < prov.size(); ++i)
		{
			npkg.addProvide(prov[i]);
			//std::cout << "Provides: " << prov[i] << std::endl;
		}

		_pkgs.addPackage(npkg);
		//std::cout << cnt << std::endl;

		//std::cout << std::endl;
	
	}
}

std::vector<std::string> Parser::split(std::string src, char delim)
{
	std::vector<std::string> ret;

	int i = 0;
	std::string tmp = "";
	while (i < src.length())
	{
		//Second condition is needed in case we have reached end
		//of string
		if ((src[i] != delim) && (i < (src.length()-1)))
		{
			tmp.push_back(src[i]);
		}
		else
		{
			if (i == src.length()-1)
			{
				tmp.push_back(src[i]);
			}
			std::string rdy = cut_version(tmp);
			bool bInVector = false;  
			if (ret.size() != 0)
			{
				for (int j = 0; j < ret.size(); ++j)
				{
					//std::cout << ret[i] << ' ' << rdy << std::endl;
					if (ret[j] == rdy)
					{
						bInVector = true;
					}
				}
			
				if (!bInVector)
				{
					ret.push_back(rdy);
					//std::cout << "added " << rdy << std::endl;
				}
			}
			else
			{
				//std::cout << rdy << std::endl;
				ret.push_back(rdy);
			}
			tmp = "";
		}
		++i;
	}

	return ret;
}

std::string Parser::cut_version(std::string src)
{
	std::string ret;
	for (int i = 0; i < src.length(); ++i)
	{
		/*
		if ((src[i] == '[') ||
			(src[i] == ']') ||
			(src[i] == '(') ||
			(src[i] == ')'))
		{
			return ret;
		}
		*/

		if ((src[i] == '[') ||
			(src[i] == ']'))
		{
			return ret;
		}
		ret.push_back(src[i]);
	}

	return ret;
}

Container Parser::getContainer() const
{
	return _pkgs;
}

Package Parser::getPackage(int index) const
{
	if ((index >= _pkgs.size()) ||
		(index < 0))
	{
		throw std::logic_error("Requested package with unreachable index");
	}
	return _pkgs[index];
}

int Parser::getAmountOfPackages() const
{
	return _pkgs.size();
}

