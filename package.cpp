#include "package.hpp"

//##################################################//
//													//
//						Package						//
//													//
//##################################################//

//Self-explanatory, I guess
Package::Package()
{

}

Package::Package(std::string name, std::string desc): _name(name), _desc(desc)
{
	_provides = std::vector<std::string>();
	_requires = std::vector<std::string>();
}

//Find if package requires package with provided name
bool Package::requires(std::string pkgname) const
{
	if (_requires.size() == 0)
	{
		return false;
	}

	for (int i = 0; i < _requires.size(); ++i)
	{
		if (_requires[i] == pkgname)
		{
			//std::cout << "PKG REQ CMP TRUE:" << _provides[i] << " - " << pkgname << std::endl;
			return true;
		}
	}

	return false;
}

//Find if package provides package with provided name
bool Package::provides(std::string pkgname) const
{
	//std::cout << "ENTER PKG PROV" << std::endl;
	if (_provides.size() == 0)
	{
		//std::cout << "PKG FALSE\n";
		return false;
	}

	for (int i = 0; i < _provides.size(); ++i)
	{
		if (_provides[i] == pkgname)
		{
			//std::cout << "PKG PROV CMP TRUE:" << _provides[i] << " - " << pkgname << std::endl;
			return true;
		}
	}

	//std::cout << "PKG FALSE\n";
	return false;
}

//Self-explanatory
void Package::addRequire(std::string pkg)
{
	_requires.push_back(pkg);
}

//Same
void Package::addProvide(std::string pkg)
{
	_provides.push_back(pkg);
}

//For debugging purposes right now, may be rewritten later
std::string Package::getFullInfo() const
{
	std::string info = "#####################################\n";
	info += "PACKAGE \"" + _name + "\": " + _desc + '\n';
	info += "PACKAGES REQUIRED: " + std::to_string(_requires.size()) + '\n';
	info += "REQUIRES:\n";
	if (_requires.size() != 0)
	{
		for (int i = 0; i < _requires.size(); ++i)
		{
			info += "\t* " + _requires[i] + '\n';
		}
	}
	//info += '\n';
	info += "BINARIES PROVIDED: " + std::to_string(_provides.size()) + "\n";
	info += "PROVIDES:\n";
	if (_provides.size() != 0)
	{
		for (int i = 0; i < _provides.size(); ++i)
		{
			info += "\t* " + _provides[i] + '\n';
		}
	}
	info += "#####################################\n";
	return info;
}

std::vector<std::string> Package::getRequires() const
{
	return _requires;
}

std::vector<std::string> Package::getProvides() const
{
	return _provides;
}

int Package::getReqAmount() const
{
	return _requires.size();
}

int Package::getProvAmount() const
{
	return _provides.size();
}

Package& Package::operator = (const Package &src)
{
	_desc = src._desc;
	_name = src._name;
	for (int i = 0; i < src._provides.size(); ++i)
	{
		_provides.push_back(src._provides[i]);
	}
	for (int i = 0; i < src._requires.size(); ++i)
	{
		_requires.push_back(src._requires[i]);
	}
	
	return *this;
}

bool Package::operator == (const Package& cmp) const
{
	if (this->_desc != cmp._desc)
	{
		return false;
	}

	for (int i = 0; i < _provides.size(); ++i)
	{
		if (!cmp.provides(_provides[i]))
		{
			return false;
		}
	}

	for (int i = 0; i < _requires.size(); ++i)
	{
		if (!cmp.requires(_requires[i]))
		{
			return false;
		}
	}

	return true;
}

std::string Package::getDesc() const
{
	return _desc;
}

std::string Package::getName() const
{
	return _name;
}

//Format as followpkg.s:
//
//pkg
//pkgname
//pkgdesc
//provides 3
//pkgprov1 pkgprov2 pkgprov3
//requires 2
//req1 req2
//
//This same format is used for reading
std::ofstream &operator << (std::ofstream& in, const Package& pkg)
{
	in << "pkg" << std::endl;
	in << pkg._name << std::endl << pkg._desc << std::endl;
	in << "provides" << ' ' << pkg._provides.size() << std::endl;
	for (int i = 0; i < pkg._provides.size(); ++i)
	{
		in << pkg._provides[i] << ' ';
	}
	in << "requires" << ' ' << pkg._requires.size() << std::endl;
	for (int i = 0; i < pkg._requires.size(); ++i)
	{
		in << pkg._requires[i] << ' ';
	}
	in << std::endl;
	return in;
}

std::ifstream &operator >> (std::ifstream& out, Package& pkg)
{
	std::string checker;
	out >> checker;
	if (checker != "pkg")
	{
		throw std::runtime_error("Package import: Invalid header");
	}

	out >> checker;
	pkg._name = checker;
	std::getline(out, checker);
	std::getline(out, checker);
	pkg._desc = checker;

	out >> checker;
	if (checker != "provides")
	{
		throw std::runtime_error("Package import: failed to read provides header");
	}

	int provcount;
	out >> provcount;
	for (int i = 0; i < provcount; ++i)
	{
		out >> checker;
		pkg._provides.push_back(checker);
	}

	out >> checker;
	if (checker != "requires")
	{
		throw std::runtime_error("Package import: failed to read requreq header");
	}

	int reqcount;
	out >> reqcount;
	for (int i = 0; i < reqcount; ++i)
	{
		out >> checker;
		pkg._requires.push_back(checker);
	}


	return out;
}
