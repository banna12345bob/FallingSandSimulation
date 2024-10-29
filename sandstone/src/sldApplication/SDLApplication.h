#pragma once

#include "core.h"
#include <string>
#include <sstream>
#include <map>
#include "Application.h"
#include <SDL.h>

namespace Sandstone {

	class SDLApplication : public Application
	{
	public:
		SDLApplication();
		virtual ~SDLApplication();

		virtual void Run() override;

		SDL_Window* window = NULL;

		//The surface contained by the window
		SDL_Surface* screenSurface = NULL;

		SDL_Renderer* renderer;
	};

	//	To be defined in client
    Application* CreateApplication();
}
