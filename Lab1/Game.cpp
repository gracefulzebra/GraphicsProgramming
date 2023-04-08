#include "Game.h"
#include "Viewport.h"
#include <iostream>
#include <string>

Game::Game()
{
	_gameState = GameState::PLAY;
	VisualDisplay* _gameDisplay = new VisualDisplay(); //new display
	ObjectHandler* _objHandler = new ObjectHandler();
}

Game::~Game(){}

void Game::run()
{
	std::cout << "Starting Program..." << "\n" << "\n";

	InitializeSystems();
	SDL_SetRelativeMouseMode(mouseLock);
	GameLoop();
}

void Game::InitializeSystems()
{
	std::cout << "Initializing Systems..." << "\n" << "\n";

	_gameDisplay.InitializeDisplay();
	
	_gameAudio.addAudioTrack("..\\res\\audio\\background.wav");
	_gameAudio.addSoundEffect("..\\res\\audio\\Tap.wav");
	
	
	_mainViewport.InitializeViewport(glm::vec3(0, 0, 5), 70.0f, (float)_gameDisplay.GetWidth()/_gameDisplay.GetHeight(), 0.01f, 1000.0f);

	//Initializes the SkyBoxRenderer for CubeMap generation
	_skyBox.InitializeSkyBoxRenderer();

	//Initializes the ObjectHandler and creates 3 new objects for it to manage
	_objHandler.InitializeObjHandler(_gameAudio);
	_objHandler.CreateObject(_objHandler.textures[0], _objHandler.shaders[0], glm::vec3(5.0, 0.5, 0.0), glm::vec3(0.6, 0.6, 0.6), "..\\res\\objects\\monkey3.obj");
	_objHandler.CreateObject(_objHandler.textures[2], _objHandler.shaders[0], glm::vec3(-5.0, 0.5, 0.0), glm::vec3(0.6, 0.6, 0.6), "..\\res\\objects\\backpack.obj");
	_objHandler.CreateObject(_objHandler.textures[1], _objHandler.shaders[0], glm::vec3(5.0, 0.5, 0.0), glm::vec3(0.6, 0.6, 0.6), "..\\res\\objects\\cube.obj");
}

void Game::GameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		_clock.Tick(); //Update the clock to the current frame's time
		time = (float)_clock.GetTime(); //Get the current frame's time

		_gameAudio.playAudioTrack(); //Loop background audio
		
		_objHandler.CheckCollisions(); //Check for collisions between all the currently managed objects

		ProcessUserInput();
		DrawGame();
	}
}

void Game::ProcessUserInput()
{
	SDL_Event event;
	float xpos = 0;
	float ypos = 0;

	while(SDL_PollEvent(&event)) //get and process events
	{
		switch (event.type)
		{
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;

			case SDL_MOUSEMOTION: //On mouse movement

				xpos += event.motion.xrel; //Increment the x and y axis of movement
				ypos += event.motion.yrel;

				_mainViewport.Pitch(ypos / sensitivity); //Updates the camera's rotation and pitch to the new values
				_mainViewport.RotateY(-xpos / sensitivity);
				break;

			case SDL_KEYDOWN: //On key press
				switch (event.key.keysym.sym) {
					//Camera movement
					case SDLK_w:
						_mainViewport.MoveForward(movementDistance);
						break;

						case SDLK_s:
						_mainViewport.MoveForward(-movementDistance);
						break;

					case SDLK_d:
						_mainViewport.MoveRight(movementDistance);
						break;

					case SDLK_a:
						_mainViewport.MoveRight(-movementDistance);
						break;

					case SDLK_SPACE:
						_mainViewport.MoveUp(movementDistance);
						break;

					case SDLK_LCTRL:
						_mainViewport.MoveUp(-movementDistance);
						break;
					
					//Exit game
					case SDLK_0:
						_gameState = GameState::EXIT;
						break;

						_mainViewport.MoveUp(-movementDistance);
						break;

					//Toggle the cursor lock state
					case SDLK_ESCAPE:
						if (mouseLock == SDL_FALSE) { mouseLock = SDL_TRUE; SDL_SetRelativeMouseMode(mouseLock); break; }
						if (mouseLock == SDL_TRUE) { mouseLock = SDL_FALSE; SDL_SetRelativeMouseMode(mouseLock); break; }	

					default:
						break;
				}
		}
	}
}

void Game::DrawGame()
{
	_gameDisplay.ClearDisplay(0.0f, 0.0f, 0.0f, 1.0f);
	
	//Draw all the currently managed objects
	_objHandler.DrawAllObjects(_mainViewport, time);
	
	//Draw objects that use the CubeMap texture
	_skyBox.DrawReflectionCube(_mainViewport, time);
	_skyBox.DrawRefractionCube(_mainViewport, time);
	_skyBox.DrawSkybox(_mainViewport);
			
	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();

	_gameDisplay.SwapBuffer();
} 
