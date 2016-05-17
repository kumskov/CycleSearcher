#include "utils.hpp"
#include <iostream>

std::string Utils::processString(int current, int max, int width)
{
	double onepercent = max/100.0;
	double percentage = current/onepercent;

	double currentprocess = (width/100.0)*percentage;
	if (current == 0)
	{
		currentprocess = 0;
	}
	//std::cout << currentprocess << std::endl << std::endl;

	std::string ret = "[";
	for (int i = 0; i < width; ++i)
	{
		char toadd = (currentprocess >= i) ? '*' : ' ';
		ret.push_back(toadd);
	}
	ret.push_back(']');

	return ret;
}