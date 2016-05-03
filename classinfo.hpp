#ifndef __CLASS_INFO_HPP__
#define __CLASS_INFO_HPP__

#include <string>

class ClassInfo
{
public:
	virtual std::string getClassName() = 0;
	virtual std::string getClassDescription() = 0;

};

#endif