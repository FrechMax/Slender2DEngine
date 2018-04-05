#include "..\..\headers\Core\Framework.h"

#include "SDL_image.h"
#include <cstdio>

using namespace S2D::Core;

void Game::init(SDL_WindowFlags wf)
{

	//Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("[Game::Game]: Initialization failed, %s", SDL_GetError());
		internalShutdown();
	}

	//Initialize SDL_image
	int flags = (IMG_INIT_JPG | IMG_INIT_PNG);
	int check = IMG_Init(flags);
	if (check = !flags)
	{
		printf("[Game::Game]: Could not Initialize SDL_image, %s", SDL_GetError());
		internalShutdown();
	}

	// give control to subclass initialize
	initialize();
}

void Game::run()
{
	lastTime = (double)SDL_GetTicks() / 1000.0;
	int updatesPerTick = 0;
	while (!mEndGame)
	{
		curTime = (double)SDL_GetTicks() / 1000.0;
		delta = curTime - lastTime;
		lag += delta;
		lastTime = curTime;

		//count ticks per second and updates per tick
		startTimeCount += delta;
		ticksPerSecond++;
		if (startTimeCount >= 1.0)
		{
			printf("Ticks_Per_Second: %d\n", ticksPerSecond);
			printf("Updates_Per_Tick: %d\n", updatesPerTick);
			ticksPerSecond = 0;
			startTimeCount = 0;
		}


		// update game state
		updatesPerTick = 0;
		while (lag >= MS_PER_TICK)
		{
			// read user input or event input
			processEvents();

			// process non-render events
			updateSystems();

			lag -= MS_PER_TICK;
			updatesPerTick++;
		}

		updateRenderingSystems();
	}
}

void S2D::Core::Game::internalShutdown()
{
	SDL_Quit();
	IMG_Quit();
	
	shutdown();

	printf("\n");
	system("pause");

	exit(0);
}

void S2D::Core::Game::handle(GameExitEvent e)
{
	mEndGame = false;
}

void S2D::Core::Game::initializeGameState()
{
}

void S2D::Core::Game::processEvents()
{
}
