#ifndef __LIB_FACTORY_HPP__
#define __LIB_FACTORY_HPP__

#include <dlfcn.h>
#include <vector>
#include <string>

template <class T>
class LibFactory
{
protected:
	typedef T* creat();

	std::vector<creat*> _expTypes;
	std::vector<void*> _handles;
	std::vector<std::string> _libnames;

	int _current = -1;

	bool _handleset = false;
	std::string _symbolHandle;

	bool alreadyOpened(std::string libname) const
	{
		for (int i = 0; i < _libnames.size(); ++i)
		{
			if (libname == _libnames[i])
				return true;
		}

		return false;
	}



public:
	~LibFactory()
	{
		for (int i = 0; i < _handles.size(); ++i)
		{
			dlclose(_handles[i]);
		}
	}

	void load(std::string libname)
	{
		if (!_handleset)
		{
			throw std::runtime_error("LibFactory: Handle is not set!");
		}

		if (alreadyOpened(libname))
		{
			return;
		}

		void* newhandle = dlopen(libname.c_str(), RTLD_NOW);
		if (!newhandle)
		{
			throw std::runtime_error(std::string("LibFactory open: ") + dlerror());
		}

		creat* getSymbol = (creat*) dlsym(newhandle, _symbolHandle.c_str());
		if (!getSymbol)
		{
			throw std::runtime_error(std::string("LibFactory sym: ") + dlerror());
		}

		_expTypes.push_back(getSymbol);
		_handles.push_back(newhandle);
		_libnames.push_back(libname);

		_current = _expTypes.size() - 1;
	}

	void setSymbol(std::string newhandle)
	{
		if (!_handleset)
		{
			_symbolHandle = newhandle;
			_handleset = true;
		}
	}

	T* getObject() const
	{
		if (!_handleset)
		{
			throw std::runtime_error("LibFactory: Library is not set!");
		}

		if ((_current < 0) || (_current > _expTypes.size()))
		{
			throw std::logic_error("LibFactory: Invalid current selection");
		}

		return _expTypes[_current]();
	}

	T* getObject(int sel) const
	{
		if (!_handleset)
		{
			throw std::runtime_error("LibFactory: Library is not set!");
		}

		if ((sel < 0) || (sel > _expTypes.size()))
		{
			throw std::logic_error("LibFactory: Invalid current selection");
		}

		return _expTypes[sel]();
	}

	void setCurrent(int sel)
	{
		if (!_handleset)
		{
			throw std::runtime_error("LibFactory: Library is not set!");
		}

		if ((sel < 0) || (sel > _expTypes.size()))
		{
			throw std::logic_error("LibFactory: Invalid selection");
		}

		_current = sel;
	}

	int getCurrent() const
	{
		return _current;
	}

	int getAmount() const
	{
		return _expTypes.size();
	}
};


#endif