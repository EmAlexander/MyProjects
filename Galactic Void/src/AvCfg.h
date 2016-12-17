#ifndef __AVCFG_H_INCLUDED
#define __AVCFG_H_INCLUDED

#include <iostream>
#include <string>
#include <fstream>
#include <type_traits>

using namespace std;

//unused feature
enum cfgFileTyp
{
	//encrypt file data
	CFG_F_ENCRYP,
	//dont encrypt file data
	CFG_F_UNENCR
};

template <class TYPE>
class AvCfg
{
public:
	AvCfg();
	~AvCfg();

	/*
	open cfg file and define if encryped
	returns -1 on failure
	*/
	int avCfg_start(string fileStr, cfgFileTyp fTyp);

	/*
	gets a line of config file
	*/
	template <class CTYPE>
	CTYPE avCfg_getCfg(string varName, int fileLine)
	{
		throw exception("Must set datatype!");
	}

	template <>
	int avCfg_getCfg<int>(string varName, int fileLine)
	{
		unsigned int currentLine = 0;
		int returnVar;
		if (!failed)
		{
			while (getline(cfg_file, cfg_lineStr))
			{
				if (currentLine == fileLine)
				{
					string tempStr = cfg_lineStr;
					tempStr.erase(0, varName.length());
					if (cfg_lineStr.substr(0, varName.length()) == varName)
					{
						//convert data
						returnVar = atoi(tempStr.c_str());
						return returnVar;
					}
				}
				currentLine++;
			}
		}
		else
		{
			throw exception( "Cant get data from config file!");
			return -1;
		}
		return -1;
	}

	template <>
	bool avCfg_getCfg<bool>(string varName, int fileLine)
	{
		unsigned int currentLine = 0;
		bool returnVar;
		if (!failed)
		{
			while (getline(cfg_file, cfg_lineStr))
			{
				if (currentLine == fileLine)
				{
					string tempStr = cfg_lineStr;
					tempStr.erase(0, varName.length());
					if (cfg_lineStr.substr(0, varName.length()) == varName)
					{
						//convert data
						if (tempStr == "TRUE" || tempStr == "true" || tempStr == "1" || tempStr == "T" || tempStr == "t")
						{
							returnVar = true;
							return returnVar;
						}
						else if (tempStr == "FALSE" || tempStr == "false" || tempStr == "0" || tempStr == "F" || tempStr == "f")
						{
							returnVar = false;
							return returnVar;
						}
						else
						{
							throw exception("Bad input for bool!");
						}
					}
				}
				currentLine++;
			}
		}
		else
		{
			throw exception("Cant get data from config file!");
			return 0;
		}
		return 0;
	}

	template <>
	string avCfg_getCfg<string>(string varName, int fileLine)
	{
		unsigned int currentLine = 0;
		string returnVar;
		if (!failed)
		{
			while (getline(cfg_file, cfg_lineStr))
			{
				if (currentLine == fileLine)
				{
					string tempStr = cfg_lineStr;
					tempStr.erase(0, varName.length());
					if (cfg_lineStr.substr(0, varName.length()) == varName)
					{
						//convert data
						returnVar = tempStr.c_str();
						return returnVar;
					}
				}
				currentLine++;
			}
		}
		else
		{
			throw exception("Cant get data from config file!");
			return -1;
		}
		return -1;
	}

	template <>
	float avCfg_getCfg<float>(string varName, int fileLine)
	{
		unsigned int currentLine = 0;
		float returnVar;
		if (!failed)
		{
			while (getline(cfg_file, cfg_lineStr))
			{
				if (currentLine == fileLine)
				{
					string tempStr = cfg_lineStr;
					tempStr.erase(0, varName.length());
					if (cfg_lineStr.substr(0, varName.length()) == varName)
					{
						//convert data
						returnVar = ::atof(tempStr.c_str());
						return returnVar;
					}
				}
				currentLine++;
			}
		}
		else
		{
			throw exception("Cant get data from config file!");
			return -1;
		}
		return -1;
	}

	template <>
	double avCfg_getCfg<double>(string varName, int fileLine)
	{
		unsigned int currentLine = 0;
		double returnVar;
		if (!failed)
		{
			while (getline(cfg_file, cfg_lineStr))
			{
				if (currentLine == fileLine)
				{
					string tempStr = cfg_lineStr;
					tempStr.erase(0, varName.length());
					if (cfg_lineStr.substr(0, varName.length()) == varName)
					{
						//convert data
						returnVar = atof(tempStr.c_str());
						return returnVar;
					}
				}
				currentLine++;
			}
		}
		else
		{
			throw exception("Cant get data from config file!");
			return -1;
		}
		return -1;
	}
	
	/*
	closes cfg file and frees data
	*/
	void avCfg_stop();
private:
	string cfg_lineStr;
	fstream cfg_file;
	bool encrypt;
	bool failed;
};

#endif //__AVCFG_H_INCLUDED