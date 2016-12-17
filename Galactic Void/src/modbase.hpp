#ifndef __MODBASE_HPP_INCLUDED__
#define __MODBASE_HPP_INCLUDED__

#include <memory>
#include <string>
#include <iostream>

using namespace std;

// Test to see if we are building a DLL.
// If we are, specify that we are exporting
// to the DLL, otherwise don't worry (we
// will manually import the functions).
#ifdef BUILD_DLL
#define DLLAPI __declspec(dllexport)
#else
#define DLLAPI
#endif // BUILD_DLL

/*struct gameDat
{
	string sav_name;

	double plyData_speed;
	double plyData_rotationSpeed;
	int plyData_posX;
	int plyData_posY;
	double plyData_velX;
	double plyData_velY;
};*/

// This is the base class for the class
// retrieved from the DLL. This is used simply
// so that I can show how various types should
// be retrieved from a DLL. This class is to
// show how derived classes can be taken from
// a DLL.
class ModBase {
public:
	virtual ~ModBase() = default;

	// Pure virtual function to render dll
	virtual void render(void) = 0;

	// Pure virtual function to update dll
	virtual void update(void) = 0;
};

// DLL export funcs

// Get an instance of the derived class
// contained in the DLL.
DLLAPI unique_ptr<ModBase> getObj(void);

// Get the name of the plugin. This can
// be used in various associated messages.
DLLAPI string getName(void);

//init the dll
DLLAPI void initDLL(void);

#endif // __MODBASE_HPP_INCLUDED__
