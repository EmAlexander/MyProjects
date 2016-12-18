#include "stringS.h"

std::vector<std::string> &quebic::stringS::split(const std::string &s, char delim, std::vector<std::string> &elems)
{
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim))
	{
		elems.push_back(item);
	}
	return elems;
}

std::vector<std::string> quebic::stringS::split(const std::string &s, char delim)
{
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

std::string quebic::stringS::stringToLower(std::string str)
{
	std::string _strRef = str;
	std::transform(_strRef.begin(), _strRef.end(), _strRef.begin(), ::tolower);
	return _strRef;
}

std::string quebic::stringS::stringToUpper(std::string str)
{
	std::string _strRef = str;
	std::transform(_strRef.begin(), _strRef.end(), _strRef.begin(), ::toupper);
	return _strRef;
}