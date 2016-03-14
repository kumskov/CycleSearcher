#include "container.hpp"

//##################################################//
//													//
//					Container						//
//													//
//##################################################//

Container::Container()
{
	_pkgs = std::vector<Package>();
}

int Container::size() const
{
	return _pkgs.size();
}

const Package& Container::operator[] (int index) const
{
	if (index >= _pkgs.size())
	{
		throw std::logic_error("Container: Tried to request index higher than amount of elements");
	}

	return _pkgs[index];
}

Package Container::getPackage(int index) const
{
	if (index >= _pkgs.size())
	{
		throw std::logic_error("Container: Tried to request index higher than amount of elements");
	}

	return _pkgs[index];
}

void Container::addPackage(Package npkg)
{
	_pkgs.push_back(npkg);
}


int Container::findProvidingPackage(std::string prov) const
{
	for (int i = 0; i < _pkgs.size(); ++i)
	{
		if (_pkgs[i].provides(prov))
		{
			return i;
		}
	}

	return -1;
}

int Container::findRequiringPackage(std::string req) const
{
	for (int i = 0; i < _pkgs.size(); ++i)
	{
		if (_pkgs[i].requires(req))
		{
			return i;
		}
	}

	return -1;
}


int Container::find(Package src) const
{
	for (int i = 0; i < _pkgs.size(); ++i)
	{
		if (src == _pkgs[i])
			return i;
	}

	return -1;
}
