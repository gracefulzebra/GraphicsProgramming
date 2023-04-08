#include "VisualDisplay.h"

VisualDisplay::VisualDisplay()
{
	appWindow = nullptr; //initialise to generate null access violation for debugging.
	glContext = nullptr;
	screenWidth = 1024.0f;
	screenHeight = 768.0f; 
}

VisualDisplay::~VisualDisplay()
{
	SDL_GL_DeleteContext(glContext); // delete context
	SDL_DestroyWindow(appWindow); // detete window (make sure to delete the context and the window in the opposite order of creation in initDisplay())
	SDL_Quit();
}

float VisualDisplay::GetWidth() { return screenWidth; } //getters
float VisualDisplay::GetHeight() { return screenHeight; }

void VisualDisplay::ReturnError(std::string errorMessage)
{
	std::cout << errorMessage << std::endl;
	std::cout << "Press Any Key To Quit...";
	int val;
	std::cin >> val;
	SDL_Quit();
}

void VisualDisplay::SwapBuffer()
{
	SDL_GL_SwapWindow(appWindow); //swap buffers
}

void VisualDisplay::ClearDisplay(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear colour and depth buffer - set colour to colour defined in glClearColor
}

void VisualDisplay::InitializeDisplay()
{
	SDL_Init(SDL_INIT_EVERYTHING); //initalise everything

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8); //Min no of bits used to diplay colour
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);// set up z-buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // set up double buffer   

	appWindow = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)screenWidth, (int)screenHeight, SDL_WINDOW_OPENGL); // create window

	if (appWindow == nullptr)
	{
		ReturnError("window failed to create");
	}

	glContext = SDL_GL_CreateContext(appWindow);

	if (glContext == nullptr)
	{
		ReturnError("SDL_GL context failed to create");
	}

	GLenum glError = glewInit();
	if (glError != GLEW_OK)
	{
		ReturnError("GLEW failed to initialise");
	}

	glEnable(GL_DEPTH_TEST); //enable z-buffering 
	glEnable(GL_CULL_FACE); //dont draw faces that are not pointing at the Viewport

	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
}