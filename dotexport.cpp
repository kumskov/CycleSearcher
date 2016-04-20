#include "dotexport.hpp"

DotExporter::DotExporter() {}

DotExporter::DotExporter(std::string name)
{
	_graphname = name;
}

DotExporter::DotExporter(std::string name, std::string flname)
{
	_fl = flname;
	_graphname = name;
}

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

void DotExporter::setName(std::string name)
{
	_graphname = name;
}

std::string DotExporter::getName() const
{
	return _graphname;
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
		simplelink._loop = false;

		_paths.push_back(simplelink);
	}
}

void DotExporter::generateSlotLinks(int index, std::vector< std::vector<int> > reqs, Graph src)
{
	if (index == -1)
	{
		throw std::logic_error("DotExporter: Invalid index (not found previously)");
	}

	//throw std::logic_error("FIX IT");

	std::string start = _encodednames[index];
	

	for (int i = 0; i < reqs.size(); ++i)
	{
		if (reqs[i].size() == 0)
		{
			//Mark broken here
			continue;
		}
		if (reqs[i].size() == 1)
		{
			int toindex = reqs[i][0];
			std::string end = _encodednames[toindex];

			Link simplelink;
			simplelink._path = start + " -> " + end;
			simplelink._from = index;
			simplelink._to = toindex;
			simplelink._dep = src[reqs[i][0]].getRequires()[i];
			simplelink._loop = false;

			_paths.push_back(simplelink);
		}
		else
		{
			SlotLink newslot;
			newslot._slotname = src[index].getRequires()[i];
			newslot._slotencname = "slot_" + generateName(newslot._slotname);
			newslot._from = index;
			newslot._to = reqs[i];

			//std::string connector = src[reqs[i]].getRequires()[i];
			newslot._link = start + " -> " + newslot._slotname;

			for (int j = 0; j < reqs[i].size(); ++j)
			{
				int toindex = reqs[i][j];
				std::string end = _encodednames[toindex];

				std::string newpath;
				newpath = newslot._slotname + " -> " + end;

				newslot._paths.push_back(newpath);
				newslot._loop.push_back(false);
			}

			_slotpaths.push_back(newslot);
		}
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
		std::vector< std::vector<int> > reqs = src.getRequires(i);

		if (checkSingleSlot(reqs))
		{
			generateLinks(i, transformSingleSlot(reqs), src);
		}
		else
		{
			generateSlotLinks(i, reqs, src);
		}
	}
}

void DotExporter::markCycles(CycleContainer src)
{
	throw std::logic_error("DotExporter: markCycles unfinshed");
}

void DotExporter::save()
{
	std::ofstream dotfile;
	dotfile.open(_fl.c_str(), std::ofstream::out);

	dotfile << "digraph " << _graphname << " {" << std::endl;

	dotfile << std::endl << "// Graph node names" << std::endl;
	for (int i = 0; i < _names.size(); ++i)
	{
		dotfile << "\t";
		dotfile << _encodednames[i] << " ";
		dotfile << "[label=\"" << _names[i] << "\" shape=box]" << std::endl; 
	}

	if (!_slotIgnore)
	{
		dotfile << std::endl << "// Slot connectors" << std::endl;

		for (int i = 0; i < _slotpaths.size(); ++i)
		{
			dotfile << "\t";
			dotfile << _slotpaths[i]._slotencname << " ";
			dotfile << "[label=\"" << _slotpaths[i]._slotname << "\" shape=circle]" << std::endl; 
		}
	}

	dotfile << std::endl << "// Graph links" << std::endl;

	for (int i = 0; i < _paths.size(); ++i)
	{
		dotfile << "\t";
		dotfile << _paths[i]._path;
		dotfile << " [";
		if (_paths[i]._loop)
		{
			dotfile << "color=red ";
		}
		dotfile << "label=\"" << _paths[i]._dep << "\"]";
		dotfile << std::endl;
	}

	if (!_slotIgnore)
	{
		dotfile << std::endl << "// Graph slot links" << std::endl;

		for (int i = 0; i < _slotpaths.size(); ++i)
		{
			dotfile << "//Link from main node to a connector" << std::endl;
			dotfile << "\t";
			dotfile << _slotpaths[i]._link << std::endl;

			dotfile << "//Links from connector to all dependecies" << std::endl;
			dotfile << "\t";
			for (int j = 0; j < _slotpaths[i]._paths.size(); ++j)
			{
				dotfile << "\t" << std::endl;
				dotfile << _slotpaths[i]._paths[j] << std::endl;
				dotfile << " [";
				if (_slotpaths[i]._loop[j])
				{
					dotfile << "color=red ";
				}
				dotfile << "\"]" << std::endl;
			}
			//throw std::logic_error("DotExporter: save is unfinished, slot links");
		}
	}

	dotfile << "}" << std::endl;

	dotfile.close();

}