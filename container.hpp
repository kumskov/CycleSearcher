#ifndef __CONTAINER_HPP__
#define __CONTAINER_HPP__

#include <vector>
#include <string>
#include <stdexcept>
#include "package.hpp"

class Container
{
protected:
	std::vector<Package> _pkgs;

public:
	Container();

	void addPackage(Package);

	int size() const;
	Package getPackage(int) const;

	int findProvidingPackage(std::string) const;
	int findRequiringPackage(std::string) const;

	int find(Package) const;

	const Package& operator[](int) const;

};

#endif