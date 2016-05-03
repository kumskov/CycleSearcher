#include "exporterfactory.hpp"


bool ExpFactory::alreadyOpened(std::string libname) const
{
	for (int i = 0; i < _libnames.size(); ++i)
	{
		if (libname == _libnames[i])
			return true;
	}

	return false;
}

ExpFactory::~ExpFactory()
{
	for (int i = 0; i < _handles.size(); ++i)
	{
		dlclose(_handles[i]);
	}
}

void ExpFactory::load(std::string libname)
{
	if (alreadyOpened(libname))
		return;

	void* newhandle = dlopen(libname.c_str(), RTLD_NOW);
	if (!newhandle)
	{
		throw std::runtime_error(std::string("ExpFactory open: ") + dlerror());
	}

	ExpFactory::creat* getExporter = (ExpFactory::creat*) dlsym(newhandle, "getExporter");
	if (!getExporter)
	{
		throw std::runtime_error(std::string("ExpFactory sym: ") + dlerror());
	}

	_expTypes.push_back(getExporter);
	_handles.push_back(newhandle);
	_libnames.push_back(libname);

	_current = _expTypes.size() - 1;
}

Exporter* ExpFactory::getExporter() const
{
	if ((_current < 0) || (_current > _expTypes.size()))
	{
		throw std::logic_error("ExpFactory: Invalid current selection");
	}

	return _expTypes[_current]();
}

Exporter* ExpFactory::getExporter(int sel) const
{
	if ((sel < 0) || (sel > _expTypes.size()))
	{
		throw std::logic_error("ExpFactory: Invalid current selection");
	}

	return _expTypes[sel]();
}

void ExpFactory::setCurrent(int sel)
{
	if ((sel < 0) || (sel > _expTypes.size()))
	{
		throw std::logic_error("ExpFactory: Invalid selection");
	}

	_current = sel;
}

int ExpFactory::getCurrent() const
{
	return _current;
}

int ExpFactory::getAmount() const
{
	return _expTypes.size();
}

std::string ExpFactory::getName(int sel) const
{
	if ((sel < 0) || (sel > _expTypes.size()))
	{
		throw std::logic_error("ExpFactory: Invalid selection");
	}

	return _expTypes[sel]()->getClassName();
}

std::string ExpFactory::getDescription(int sel) const
{
	if ((sel < 0) || (sel > _expTypes.size()))
	{
		throw std::logic_error("ExpFactory: Invalid selection");
	}

	return _expTypes[sel]()->getClassDescription();
}

std::string ExpFactory::getLibName(int sel) const
{
	if ((sel < 0) || (sel > _expTypes.size()))
	{
		throw std::logic_error("ExpFactory: Invalid selection");
	}

	return _libnames[sel];
}
