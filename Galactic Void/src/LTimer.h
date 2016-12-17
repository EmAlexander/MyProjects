/*
Version: 0.0
Last changed: 4/27/2014
Author: Herrybiscuit
Copyright: (C) Herrybiscuit 2014
*/

#ifndef LTIMER_H_INCLUDED
#define LTIMER_H_INCLUDED

#include <SDL.h>
#include <iostream>

using namespace std;

class LTimer
{
public:
	LTimer();
	
	void start();
	void stop();
	void pause();
	void unPause();

	Uint32 getTicks();

	bool isStarted();
	bool isPaused();
private:
	bool mPaused;
	bool mStarted;

	Uint32 mStartTicks;
	Uint32 mPausedTicks;
};

#endif
