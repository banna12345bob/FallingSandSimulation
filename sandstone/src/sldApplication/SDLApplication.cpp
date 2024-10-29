#include "SDLApplication.h"
#include "Log.h"


namespace Sandstone {
	SDLApplication::SDLApplication()
	{
		const int SCREEN_WIDTH = 640;
		const int SCREEN_HEIGHT = 480;
		
		//Initialize SDL
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			SS_CORE_FATAL("SDL could not initialize! SDL_Error: %s", SDL_GetError());
			SS_CORE_ASSERT(false, "SDL ERROR");
		}else {
			window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
			if (window == NULL)
			{
				SS_CORE_FATAL("Window could not be created! SDL_Error: %s", SDL_GetError());
				SS_CORE_ASSERT(false, "SDL ERROR");
			}
		}
	}

	SDLApplication::~SDLApplication()
	{}

	void SDLApplication::Run()
	{
		//Get window surface
		screenSurface = SDL_GetWindowSurface(window);

		renderer = SDL_GetRenderer(window);

		//Fill the surface white
		SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00));

		//Update the surface
		SDL_UpdateWindowSurface(window);


		//Hack to get window to stay up
		SDL_Event e; while (running == true) { while (SDL_PollEvent(&e)) { if (e.type == SDL_QUIT) running = false; } }

		//Destroy window
		SDL_DestroyWindow(window);

		//Quit SDL subsystems
		SDL_Quit();
	}

}
