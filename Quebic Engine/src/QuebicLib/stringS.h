#ifndef __STRINGS_H_INCLUDED
#define __STRINGS_H_INCLUDED

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

namespace quebic
{
	namespace stringS
	{
		//converts str to lower and returns
		std::string stringToLower(std::string str);

		//converts str to upper case and returns
		std::string stringToUpper(std::string str);

		//internal function not recommend to use
		std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);

		//returns a vector with each element containing a parced virsion of the string with delim as the buffer
		//example: split("foo bar", ' ') returns a vertor with 
		//at(0) = "foo"
		//at(1) = "bar"
		std::vector<std::string> split(const std::string &s, char delim);
	}
}

#endif //__STRINGS_H_INCLUDED