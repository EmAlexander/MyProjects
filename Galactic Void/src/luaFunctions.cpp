#include "luaFunctions.h"

std::vector<hook> updateHooks;

std::vector<hook> initHooks;

std::vector<hook> commandHooks;

std::vector<hook> renderHooks;

SDL_Renderer* luaRen;

gPlayer ply;

std::vector<std::string> &_split(const std::string &s, char delim, std::vector<std::string> &elems)
{
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim))
	{
		elems.push_back(item);
	}
	return elems;
}


std::vector<std::string> _split(const std::string &s, char delim)
{
	std::vector<std::string> elems;
	_split(s, delim, elems);
	return elems;
}

std::string _stringToLower(std::string str)
{
	std::string _strRef = str;
	std::transform(_strRef.begin(), _strRef.end(), _strRef.begin(), ::tolower);
	return _strRef;
}

std::string _stringToUpper(std::string str)
{
	std::string _strRef = str;
	std::transform(_strRef.begin(), _strRef.end(), _strRef.begin(), ::toupper);
	return _strRef;
}