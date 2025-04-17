#include <sandstone.h>

#include <SDL3/SDL.h>

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>

class fallingSand : public Sandstone::SDLApplication
{
public:

	fallingSand(const char* windowName, const int width, const int height, const bool fullscreen)
		: Sandstone::SDLApplication(windowName, width, height, fullscreen)
	{

	}
	~fallingSand()
	{
	}

	virtual void RunApp() {
		SDL_Event e;
		
		if (SDL_PollEvent(&e)) {
			ImGui_ImplSDL3_ProcessEvent(&e);
			switch (e.type) {
			case SDL_EVENT_QUIT:
				this->running = false;
				break;
			}
		}
	}

private:
};

Sandstone::Application* Sandstone::CreateApplication()
{
	return new fallingSand("Conway's Game of Life", 540, 540, false);
}
