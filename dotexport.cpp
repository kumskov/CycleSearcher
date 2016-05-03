#include "dotexport.hpp"

DotExporter::DotExporter() {}

DotExporter::DotExporter(std::string name)
{
	_graphname = name;
	_slotIgnore = false;
	_markBroken = true;
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
	int cnt = 0;
	int i = 0;
	while(cnt < 3)
	{
		if (i >= str.size())
		{
			ret += 'z';
			cnt++;
			continue;
		}

		if (((str[i] >= 'a') && (str[i] <= 'z')) ||
			((str[i] >= 'A') && (str[i] <= 'Z')))
		{
			ret += str[i];
			cnt++;
		}

		i++;
	}

	ret += std::to_string(_intcounter++);

	return ret;
}

int DotExporter::findName(std::string name)
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

bool DotExporter::getSlotIgnore()
{
	return _slotIgnore;
}

void DotExporter::setMarkBroken(bool mark)
{
	_markBroken = mark;
}

bool DotExporter::getMarkBroken()
{
	return _markBroken;
}

void DotExporter::setName(std::string name)
{
	_graphname = name;
}

std::string DotExporter::getName()
{
	return _graphname;
}

bool DotExporter::checkSingleSlot(std::vector< std::vector<int> > reqs)
{
	//for (int i = 0; i < reqs.size(); ++i)
	//{
	//	std::cout << reqs[i].size() << ':' << reqs[i][0] << ' ';
	//}
	//std::cout << std::endl;

	for (int i = 0; i < reqs.size(); ++i)
	{
		if ((reqs[i].size() > 1))
		{
			//std::cout << "Not single" << std::endl;
			return false;
		}
	}
	//std::cout << "Single" << std::endl;
	return true;
}

std::vector<int> DotExporter::transformSingleSlot(std::vector< std::vector<int> > src)
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

	//std::cout << src[index].getRequires().size() << " " << reqs.size() << std::endl;

	if (src[index].getRequires().size() != reqs.size())
	{
		throw std::logic_error("DotExporter: Requirements size mismatch");
	}

	std::string start = _encodednames[index];
	//std::cout << "Single: got encoded" << std::endl;
	for (int i = 0; i < reqs.size(); ++i)
	{
		int toindex = reqs[i];
		if (reqs[i] == -1)
		{
			continue;
		}
		std::string end = _encodednames[toindex];

		//std::cout << src[reqs[i]].getName() << " " << _names[reqs[i]] << std::endl;
		//std::cout << src[reqs[i]].getRequires().size() << " " << reqs.size() << std::endl;

		Link simplelink;
		simplelink._path = start + " -> " + end;
		simplelink._from = index;
		simplelink._to = toindex;
		//std::cout << src[reqs[i]].getRequires().size() << ":" << i << std::endl;
		simplelink._dep = src[index].getRequires()[i];
		//std::cout << i << ":" << reqs.size() << "Got req dep" << std::endl;
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

	if (src[index].getRequires().size() != reqs.size())
	{
		throw std::logic_error("DotExporter: Requirements size mismatch");
	}

	std::string start = _encodednames[index];
	
	for (int i = 0; i < reqs.size(); ++i)
	{
		//std::cout << "Slot size: " << reqs[i].size() << std::endl;
		if (reqs[i].size() == 0)
		{
			//std::cout << "Broken" << std::endl;
			//Mark broken here
			continue;
		}
		if (reqs[i].size() == 1)
		{
			//std::cout << "Working at simple" << std::endl;
			int toindex = reqs[i][0];
			if (reqs[i][0] == -1)
			{
				//Another broken here
				continue;
			}
			std::string end = _encodednames[toindex];

			Link simplelink;
			simplelink._path = start + " -> " + end;
			simplelink._from = index;
			simplelink._to = toindex;
			simplelink._dep = src[index].getRequires()[i];
			simplelink._loop = false;

			_paths.push_back(simplelink);
		}
		else
		{
			//std::cout << "Working at slot" << std::endl;
			SlotLink newslot;
			newslot._slotname = src[index].getRequires()[i];
			newslot._slotencname = "SLOT" + generateName(newslot._slotname);
			newslot._from = index;
			newslot._to = reqs[i];

			//std::string connector = src[reqs[i]].getRequires()[i];
			newslot._link = start + " -> " + newslot._slotencname;

			for (int j = 0; j < reqs[i].size(); ++j)
			{
				int toindex = reqs[i][j];
				std::string end = _encodednames[toindex];

				std::string newpath;
				newpath = newslot._slotencname + " -> " + end;

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
		std::cout << i << " out of " << src.getAmount() << std::endl;
		addPackage(src[i]);
	}

	for (int i = 0; i < src.getAmount(); ++i)
	{
		std::cout << "Working on " << i << " out of " << src.getAmount() << std::endl;
		std::vector< std::vector<int> > reqs = src.getRequires(i);

		if (checkSingleSlot(reqs))
		{
			//std::cout << "Single" << std::endl;
			generateLinks(i, transformSingleSlot(reqs), src);
		}
		else
		{
			//std::cout << "Slot" << std::endl;
			generateSlotLinks(i, reqs, src);
		}
	}
}

int DotExporter::findSelfLink(int index)
{
	for (int i = 0; i < _paths.size(); ++i)
	{
		if ((_paths[i]._from == index) &&
			(_paths[i]._to == index))
		{
			_paths[i]._loop = true;
			return i;
		}
	}

	return -1;
}

int DotExporter::findLink(int start, int end)
{
	for (int i = 0; i < _paths.size(); ++i)
	{
		if ((_paths[i]._from == start) &&
			(_paths[i]._to == end))
		{
			_paths[i]._loop = true;
			return i;
		}
	}

	return -1;
}

int DotExporter::findSlot(int start, int end)
{
	for (int i = 0; i < _slotpaths.size(); ++i)
	{
		if ((_slotpaths[i]._from == start))
		{
			for (int j = 0; j < _slotpaths[i]._to.size(); ++j)
			{
				if (_slotpaths[i]._to[j] == end)
				{
					_slotpaths[i]._loop[j] = true;
					return i;
				}
			}
		}
	}

	return -1;
}


void DotExporter::markCycles(CycleContainer src)
{
	std::vector<int> selfcycles = src.getSelfCycles();
	std::cout << "Working on selfcycles" << std::endl;

	for (int i = 0; i < selfcycles.size(); ++i)
	{	
		std::cout << "Selfcycle: " << i << " out of " << selfcycles.size() << std::endl;
		int found = findSelfLink(selfcycles[i]);
		if (found == -1)
		{
			throw std::logic_error("DotExporter: Could not find selfcycle");
		}
		//_paths[found]._loop = true;
	}

	std::vector< std::vector<int> > cycles = src.getCycles();
	std::cout << "Working on cycles" << std::endl;

	for (int i = 0; i < cycles.size(); ++i)
	{
		std::cout << "Cycle: " << i << " out of " << cycles.size() << std::endl;
		
		for (int j = cycles[i].size() - 1; j > 0; --j)
		{
			int found = findLink(cycles[i][j], cycles[i][j-1]);
			if (found == -1)
			{
				found = findSlot(cycles[i][j], cycles[i][j-1]);
				if (found == -1)
				{
					throw std::logic_error("DotExporter: Could not find cycle");
				}
				//_slotpaths[found]
			}
		}
	}

	int filters = src.getFilterAmount();
	std::cout << "Working on filtered cycles" << std::endl;

	for (int i = 0; i < filters; ++i)
	{
		std::cout << "Filter <" << src.getFilterParameter(i) << ">: " << i << " out of " << filters << std::endl;
		std::vector< std::vector<int> > fcycles = src.getFilteredCycle(i);
		for (int j = 0; j < fcycles.size(); ++j)
		{
			std::cout << "Filter " << i << ": " << j << " out of " << fcycles.size() << std::endl;
			for (int k = cycles[j].size() - 1; k > 0; --k)
			{
				int found = findLink(cycles[j][k], cycles[j][k-1]);
				if (found == -1)
				{
					found = findSlot(cycles[j][k], cycles[j][k-1]);
					if (found == -1)
					{
						throw std::logic_error("DotExporter: Could not find filtered cycle");
					}
				}
			}
		}
	}
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

	//std::cout << "Completed names" << std::endl;

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

	//std::cout << "Completed slot connectors" << std::endl;

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
			//dotfile << "\t";
			for (int j = 0; j < _slotpaths[i]._paths.size(); ++j)
			{
				dotfile << "\t";
				dotfile << _slotpaths[i]._paths[j];
				dotfile << " [";
				if (_slotpaths[i]._loop[j])
				{
					dotfile << "color=red ";
				}
				dotfile << "]" << std::endl;
			}
			//throw std::logic_error("DotExporter: save is unfinished, slot links");
		}
	}

	dotfile << "}" << std::endl;

	dotfile.close();

}

std::string DotExporter::getClassName()
{
	return "GraphViz/Dot Exporter";
}

std::string DotExporter::getClassDescription()
{
	return "Can't work with large graphs";
}

//dlopen hook
extern "C" Exporter* getExporter()
{
	return new DotExporter;
}