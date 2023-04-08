#pragma once

#include <SDL\SDL.h>
#include <GL/glew.h>
#include <SDL/SDL_mixer.h>
#include "Clock.h"
#include "VisualDisplay.h" 
#include "AudioHandler.h"
#include "ObjectHandler.h"
#include "SkyBoxRenderer.h"

enum class GameState{PLAY, EXIT};

class Game
{
public:
	Game();
	~Game();

	void run();

private:

	void InitializeSystems();
	void GameLoop();
	void ProcessUserInput();
	void DrawGame();

	Clock _clock;
	VisualDisplay _gameDisplay;
	GameState _gameState;
	Viewport _mainViewport;
	AudioHandler _gameAudio;
	ObjectHandler _objHandler;
	SkyBoxRenderer _skyBox;
	
	SDL_bool mouseLock = SDL_TRUE;
	float time = 0.0f;
	float movementDistance = 0.2f;
	float sensitivity = 1500;
};

