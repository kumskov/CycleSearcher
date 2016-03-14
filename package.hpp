#ifndef __PACKAGE_HPP__
#define __PACKAGE_HPP__

#include <string>
#include <vector>
#include <stdexcept>
#include <fstream>

class Package
{
protected:
	std::string _desc;						//@summary
	std::string _name;						//@info
	std::vector<std::string> _provides;		//@provides
	std::vector<std::string> _requires;		//@requires

public:
	Package();
	//Package(const Package*);
	Package(std::string, std::string);
	//Package(const char*);

	bool requires(std::string) const;
	bool provides(std::string) const;

	int getReqAmount() const;
	int getProvAmount() const;

	void addRequire(std::string);
	void addProvide(std::string);

	std::string getFullInfo() const;

	std::string getDesc() const;
	std::string getName() const;

	std::vector<std::string> getRequires() const;
	std::vector<std::string> getProvides() const;

	Package& operator = (const Package &);
	bool operator == (const Package &) const;

	friend std::ofstream &operator << (std::ofstream &, const Package &);
	friend std::ifstream &operator >> (std::ifstream &, const Package &);
};


#endif