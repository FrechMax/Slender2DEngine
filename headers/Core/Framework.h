#pragma once

#include "SDL.h"

namespace S2D {
	namespace Core {

		struct GameExitEvent 
		{
			
		};

		class Game
		{
		private:
			double curTime = 0, lag = 0, lastTime = 0, delta = 0;
			double MS_PER_TICK = (double)16 / 1000;
			double startTimeCount = 0;
			int ticksPerSecond = 0;
			SDL_Event e;
			bool mEndGame;

		public:
			void init(SDL_WindowFlags wf);
			void run();

			void handle(GameExitEvent e);
			void internalShutdown();
			virtual void shutdown() = 0;

			virtual void initialize() = 0;
			virtual void processEvents() = 0;
			virtual void updateSystems() = 0;
			virtual void updateRenderingSystems() = 0;
		};

	}
}