#include "SDLApplication.h"
#include "Log.h"

#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_sdlrenderer2.h>

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
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows Doesn't support SDLrenderer2

		ImGui::StyleColorsDark();

		ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
		ImGui_ImplSDLRenderer2_Init(renderer);
	}

	SDLApplication::~SDLApplication()
	{
		// Kill everything
		ImGui_ImplSDLRenderer2_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();

		SDL_DestroyRenderer(this->renderer);
		SDL_DestroyWindow(this->window);

		//Quit SDL
		SDL_Quit();
	}

	void SDLApplication::Run()
	{
		while (this->running) {
			this->RunApp();
		}
	}

}
