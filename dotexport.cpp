#include "dotexport.hpp"

void DotExporter::addPackage(Package pkg)
{
	_names.push_back(pkg.getName());
	_encodednames.push_back(generateName(pkg.getName()));
}

std::string DotExporter::generateName(std::string str)
{
	std::string ret;
	if (str.length() > 3)
	{
		ret += str.substr(0, 3) + "_";
	}
	else
	{
		ret += str + "_";
	}

	ret += std::to_string(_intcounter++);

	return ret;
}

int DotExporter::findName(std::string name) const
{
	for (int i = 0; i < _names.size(); ++i)
	{
		if (_names[i] == name)
		{
			return i;
		}
	}

	return -1;
}

void DotExporter::setFile(std::string flname)
{
	_fl = flname;
}

void DotExporter::setSlotIgnore(bool mark)
{
	_slotIgnore = mark;
}

bool DotExporter::getSlotIgnore() const
{
	return _slotIgnore;
}

void DotExporter::setMarkBroken(bool mark)
{
	_markBroken = mark;
}

bool DotExporter::getMarkBroken() const
{
	return _markBroken;
}

bool DotExporter::checkSingleSlot(std::vector< std::vector<int> > reqs) const
{
	for (int i = 0; i < reqs.size(); ++i)
	{
		if ((reqs.size() > 1))
		{
			return false;
		}
	}
	return true;
}

std::vector<int> DotExporter::transformSingleSlot(std::vector< std::vector<int> > src) const
{
	std::vector<int> ret;
	if (!checkSingleSlot(src))
	{
		throw std::logic_error("DotExporter: Can't convert non-single slot requirements to single-slot");
	}

	for (int i = 0; i < src.size(); ++i)
	{
		ret.push_back(src[i][0]);
	}

	return ret;
}

void DotExporter::generateLinks(int index, std::vector<int> reqs, Graph src)
{
	if (index == -1)
	{
		throw std::logic_error("DotExporter: Invalid index (not found previously)");
	}

	std::string start = _encodednames[index];

	for (int i = 0; i < reqs.size(); ++i)
	{
		int toindex = reqs[i];
		std::string end = _encodednames[toindex];

		Link simplelink;
		simplelink._path = start + " -> " + end;
		simplelink._from = index;
		simplelink._to = toindex;
		simplelink._dep = src[reqs[i]].getRequires()[i];

		_paths.push_back(simplelink);
	}
}

void DotExporter::generateSlotLinks(int index, std::vector< std::vector<int> > reqs, Graph src)
{
	if (index == -1)
	{
		throw std::logic_error("DotExporter: Invalid index (not found previously)");
	}

	std::string start = _encodednames[index];

	for (int i = 0; i < reqs.size(); ++i)
	{
		SlotLink newslot;
		newslot._slotname = src[index].getRequires()[i];
		newslot._from = index;
		newslot._to = reqs[i];

		for (int j = 0; j < reqs[i].size(); ++j)
		{
			int toindex = reqs[i][j];
			std::string end = _encodednames[toindex];

			std::string newpath;
			newpath = start + " -> " + end;

			newslot._paths.push_back(newpath);
		}

		_slotpaths.push_back(newslot);
	}
}

void DotExporter::generateFromGraph(Graph src)
{
	for (int i = 0; i < src.getAmount(); ++i)
	{
		addPackage(src[i]);
	}

	for (int i = 0; i < src.getAmount(); ++i)
	{

	}
}