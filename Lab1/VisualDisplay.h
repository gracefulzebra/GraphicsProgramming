#pragma once

#include <SDL/SDL.h>
#include <GL\glew.h>
#include <iostream>
#include <string>
using namespace std;


class VisualDisplay
{
public:
	VisualDisplay();
	~VisualDisplay();

	void InitializeDisplay();
	void SwapBuffer();
	void ClearDisplay(float r, float g, float b, float a);

	float GetWidth();
	float GetHeight();

private:
	void ReturnError(std::string errorMessage);
	
	SDL_GLContext glContext = nullptr; //global variable to hold the context
	SDL_Window* appWindow; //holds pointer to out window
	float screenWidth;
	float screenHeight;
};

