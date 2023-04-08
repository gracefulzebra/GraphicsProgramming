#pragma once

#include <SDL\SDL.h>

struct Clock {

	Clock() {};

	/// <summary>
	/// Updates Time To The Current Frame
	/// </summary>
	void Tick() 
	{
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();

		//Create ratio between current and previous frame times.
		deltaTime = (double)((NOW - LAST) / (double)SDL_GetPerformanceFrequency());

		//Real world time calculation
		time = time + (1 * deltaTime);
	}

	double GetTime()
	{
		return time;
	}

private:

	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	double deltaTime = 0;
	double time = 0;
};