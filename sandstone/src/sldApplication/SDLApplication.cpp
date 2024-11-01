#include "SDLApplication.h"
#include "Log.h"

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>

namespace Sandstone {
	SDLApplication::SDLApplication()
	{
		const int SCREEN_WIDTH = 540;
		const int SCREEN_HEIGHT = 540;
		
		//Initialize SDL
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			SS_CORE_FATAL("SDL could not initialize! SDL_Error: %s", SDL_GetError());
			SS_CORE_ASSERT(false, "SDL ERROR");
		}else {
			SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN, &window, &renderer);
			if (window == NULL || renderer == NULL)
			{
				SS_CORE_FATAL("Window or renderer could not be created! SDL_Error: %s", SDL_GetError());
				SS_CORE_ASSERT(false, "SDL ERROR");
			}
		}

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGui::StyleColorsDark();

		//ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
		//ImGui_ImplSDLRenderer3_Init(renderer);
	}

	SDLApplication::~SDLApplication()
	{}

	void SDLApplication::Run()
	{
		
	}

}
