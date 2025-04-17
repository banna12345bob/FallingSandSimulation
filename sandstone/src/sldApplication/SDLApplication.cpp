#include "SDLApplication.h"
#include "Log.h"

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>

namespace Sandstone {
	SDLApplication::SDLApplication(const char* windowName, int width, int height, bool fullscreen)
	{		
		//Initialize SDL
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			SS_CORE_FATAL("SDL could not initialize! SDL_Error: %s", SDL_GetError());
			SS_CORE_ASSERT(false, "SDL ERROR");
		}

		// GL 3.0 + GLSL 130
		const char* glsl_version = "#version 130";
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

		if (fullscreen) {
			int displaycount;
			const SDL_DisplayMode* DM = SDL_GetCurrentDisplayMode(SDL_GetDisplays(&displaycount)[0]);
			width = DM->w;
			height = DM->h;
		}

		SDL_CreateWindowAndRenderer(windowName, width, height, SDL_WINDOW_INPUT_FOCUS, &window, &renderer);

		SS_CORE_INFO("Window '{0}' initalised with width: {1} and height: {2}", windowName, width, height);

		if (fullscreen)
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

		if (window == NULL || renderer == NULL)
		{
			SS_CORE_FATAL("Window or renderer could not be created! SDL_Error: %s", SDL_GetError());
			SS_CORE_ASSERT(false, "SDL ERROR");
		}

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows Doesn't support SDLrenderer2

		ImGui::StyleColorsDark();

		ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
		ImGui_ImplSDLRenderer3_Init(renderer);
	}

	SDLApplication::~SDLApplication()
	{
		// Kill everything
		ImGui_ImplSDLRenderer3_Shutdown();
		ImGui_ImplSDL3_Shutdown();
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
