#ifndef LUAF_H_INCLUDED
#define LUAF_H_INCLUDED

#include <SDL.h>
#include <iostream>
#include <vector>
#include <string>
#include <lua.hpp>
#include <stdio.h>
#include <algorithm>
#include "gPlayer.h"
//#include "TGamePlay.h"
#include "LDevConsole.h"

using namespace std;

struct hook
{
	int id;
	std::string functionN;
};

extern gPlayer ply;

extern std::vector<hook> updateHooks;

extern std::vector<hook> initHooks;

extern std::vector<hook> commandHooks;

extern std::vector<hook> renderHooks;

extern SDL_Renderer* luaRen;

extern std::vector<std::string> &_split(const std::string &s, char delim, std::vector<std::string> &elems);

extern std::vector<std::string> _split(const std::string &s, char delim);

extern std::string _stringToUpper(std::string str);

extern std::string _stringToLower(std::string str);

/*void setDevCPos(LDevConsole* _devC)
{

}*/


static int setPlyPosFLua(lua_State* _L)
{
	int numberOfPrams = lua_gettop(_L);

	if (numberOfPrams != 2)
		return luaL_error(_L, "Got %d arguments expected 2", numberOfPrams);

	if (!lua_isnumber(_L, 1))
	{
		return luaL_error(_L, "Argument %d is not a number", 1);
	}
	if (!lua_isnumber(_L, 2))
	{
		return luaL_error(_L, "Argument %d is not a number", 2);
	}

	int x = lua_tonumber(_L, 1);
	int y = lua_tonumber(_L, 2);

	//ply.setPlyPos(x, y);

	return 0;
}

static int addHook(lua_State* _L)
{
	//addHook(nameOfHook, hookId, functionName)

	int numberOfPrams = lua_gettop(_L);

	if (numberOfPrams != 3)
		return luaL_error(_L, "Got %d arguments expected 3", numberOfPrams);

	if (!lua_isstring(_L, 1))
	{
		return luaL_error(_L, "Argument %d is not a string", 1);
	}
	if (!lua_isnumber(_L, 2))
	{
		return luaL_error(_L, "Argument %d is not a number", 2);
	}
	if (!lua_isstring(_L, 3))
	{
		return luaL_error(_L, "Argument %d is not a string", 3);
	}

	std::string firstP = lua_tostring(_L, 1);
	if (firstP == "update")
	{
		hook tempHook;

		tempHook.id = lua_tonumber(_L, 2);
		tempHook.functionN = lua_tostring(_L, 3);

		updateHooks.push_back(tempHook);
	}
	else if (firstP == "init")
	{
		hook tempHook;

		tempHook.id = lua_tonumber(_L, 2);
		tempHook.functionN = lua_tostring(_L, 3);

		initHooks.push_back(tempHook);
	}
	else if (firstP == "command")
	{
		hook tempHook;

		tempHook.id = lua_tonumber(_L, 2);
		tempHook.functionN = lua_tostring(_L, 3);

		commandHooks.push_back(tempHook);
	}
	else if (firstP == "render")
	{
		hook tempHook;

		tempHook.id = lua_tonumber(_L, 2);
		tempHook.functionN = lua_tostring(_L, 3);

		renderHooks.push_back(tempHook);
	}


	return 0;
}

static int removeHook(lua_State* _L)
{
	//removeHook(nameOfHook, hookId)

	int numberOfPrams = lua_gettop(_L);

	if (numberOfPrams != 2)
		return luaL_error(_L, "Got %d arguments expected 2", numberOfPrams);

	if (!lua_isstring(_L, 1))
	{
		return luaL_error(_L, "Argument %d is not a string", 1);
	}
	if (!lua_isnumber(_L, 2))
	{
		return luaL_error(_L, "Argument %d is not a number", 2);
	}

	std::string luaSToCString = lua_tostring(_L, 1);


	if (luaSToCString == "update")
	{
		for (int i = 0; i < updateHooks.size(); i++)
		{
			int id = lua_tonumber(_L, 2);
			if (updateHooks.at(i).id == id)
			{
				updateHooks.erase(updateHooks.begin() + i);
			}
		}
	}
	else if (luaSToCString == "init")
	{
		for (int i = 0; i < initHooks.size(); i++)
		{
			int id = lua_tonumber(_L, 2);
			if (initHooks.at(i).id == id)
			{
				initHooks.erase(initHooks.begin() + i);
			}
		}
	}
	else if (luaSToCString == "command")
	{
		for (int i = 0; i < commandHooks.size(); i++)
		{
			int id = lua_tonumber(_L, 2);
			if (commandHooks.at(i).id == id)
			{
				commandHooks.erase(commandHooks.begin() + i);
			}
		}
	}
	else if (luaSToCString == "render")
	{
		for (int i = 0; i < renderHooks.size(); i++)
		{
			int id = lua_tonumber(_L, 2);
			if (renderHooks.at(i).id == id)
			{
				renderHooks.erase(renderHooks.begin() + i);
			}
		}
	}

	return 0;
}

static int drawRect(lua_State* _L)
{
	//drawRect(x,y,w,h,r,g,b,filled)

	int numberOfPrams = lua_gettop(_L);

	if (numberOfPrams != 8)
	{
		return luaL_error(_L, "Got %d arguments expected 8", numberOfPrams);
	}

	for (int i = 1; i <= 7; i++)
	{
		if (!lua_isnumber(_L, i))
		{
			return luaL_error(_L, "Argument %d is not a int", i);
		}
	}

	if (!lua_isboolean(_L, 8))
	{
		return luaL_error(_L, "Argument %d is not a bool", 8);
	}

	bool filled = lua_toboolean(_L, 8);

	if (filled)
	{
		SDL_SetRenderDrawColor(luaRen, lua_tonumber(_L, 5), lua_tonumber(_L, 6), lua_tonumber(_L, 7), 255);
		SDL_Rect _refRect = { lua_tonumber(_L, 1), lua_tonumber(_L, 2), lua_tonumber(_L, 3), lua_tonumber(_L, 4) };
		SDL_RenderFillRect(luaRen, &_refRect);
		SDL_SetRenderDrawColor(luaRen, 255, 255, 255, 255);
	}
	else
	{
		SDL_SetRenderDrawColor(luaRen, lua_tonumber(_L, 5), lua_tonumber(_L, 6), lua_tonumber(_L, 7), 255);
		SDL_Rect _refRect = { lua_tonumber(_L, 1), lua_tonumber(_L, 2), lua_tonumber(_L, 3), lua_tonumber(_L, 4) };
		SDL_RenderDrawRect(luaRen, &_refRect);
		SDL_SetRenderDrawColor(luaRen, 255, 255, 255, 255);
	}

	return 0;
}

static int drawLine(lua_State* _L)
{
	//drawLine(x1,y1,x2,y2,r,g,b)

	int numberOfPrams = lua_gettop(_L);

	if (numberOfPrams != 7)
	{
		return luaL_error(_L, "Got %d arguments expected 7", numberOfPrams);
	}

	for (int i = 1; i <= 7; i++)
	{
		if (!lua_isnumber(_L, i))
		{
			return luaL_error(_L, "Argument %d is not a int", i);
		}
	}

	SDL_SetRenderDrawColor(luaRen, lua_tonumber(_L, 5), lua_tonumber(_L, 6), lua_tonumber(_L, 7), 255);
	SDL_RenderDrawLine(luaRen, lua_tonumber(_L, 1), lua_tonumber(_L, 2), lua_tonumber(_L, 3), lua_tonumber(_L, 4));
	SDL_SetRenderDrawColor(luaRen, 255, 255, 255, 255);

	return 0;
}

static int splitLuaString(lua_State *_L)
{
	int numberOfPrams = lua_gettop(_L);

	if (numberOfPrams != 2)
	{
		return luaL_error(_L, "Got %d arguments expected 2", numberOfPrams);
	}

	for (int i = 1; i <= 2; i++)
	{
		if (!lua_isstring(_L, i))
		{
			return luaL_error(_L, "Argument %d is not a string", i); 
		}
	}

	string firstP = lua_tostring(_L, 1);
	char inc = string(lua_tostring(_L,2))[0];

	vector<string> strV = _split(firstP, inc);

	lua_newtable(_L);

	for (int i = 0; i < strV.size(); i++)
	{
		lua_pushnumber(_L, i);   /* Push the table index */
		lua_pushstring(_L, strV.at(i).c_str()); /* Push the cell value */
		lua_rawset(_L, -3);      /* Stores the pair in the table */
	}

	return 1;
}

static int luaStringToUpper(lua_State *_L)
{
	int numberOfPrams = lua_gettop(_L);

	if (numberOfPrams != 1)
	{
		return luaL_error(_L, "Got %d arguments expected 1", numberOfPrams);
	}

	for (int i = 1; i <= 1; i++)
	{
		if (!lua_isstring(_L, i))
		{
			return luaL_error(_L, "Argument %d is not a string", i);
		}
	}

	string firstP = _stringToUpper(lua_tostring(_L, 1));

	lua_pushstring(_L, firstP.c_str());
	return 1;
}


static int luaStringToLower(lua_State *_L)
{
	int numberOfPrams = lua_gettop(_L);

	if (numberOfPrams != 1)
	{
		return luaL_error(_L, "Got %d arguments expected 1", numberOfPrams);
	}

	for (int i = 1; i <= 1; i++)
	{
		if (!lua_isstring(_L, i))
		{
			return luaL_error(_L, "Argument %d is not a string", i);
		}
	}

	string firstP = _stringToLower(lua_tostring(_L, 1));

	lua_pushstring(_L, firstP.c_str());
	return 1;
}

#endif //__LUAFUNCTIONS_H_INCLUDED