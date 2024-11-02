#include "SDLApplication.h"
#include "Log.h"

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>

namespace Sandstone {
	SDLApplication::SDLApplication(const char* windowName, const int width, const int height)
	{		
		//Initialize SDL
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			SS_CORE_FATAL("SDL could not initialize! SDL_Error: %s", SDL_GetError());
			SS_CORE_ASSERT(false, "SDL ERROR");
		}else {
			SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_SHOWN, &window, &renderer);
			if (window == NULL || renderer == NULL)
			{
				SS_CORE_FATAL("Window or renderer could not be created! SDL_Error: %s", SDL_GetError());
				SS_CORE_ASSERT(false, "SDL ERROR");
			}
		}

		SDL_SetWindowTitle(window, windowName);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGui::StyleColorsDark();

		//SDL_version

		//ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
		//ImGui_ImplSDLRenderer3_Init(renderer);
	}

	SDLApplication::~SDLApplication()
	{}

	void SDLApplication::Run()
	{
		
	}

}
