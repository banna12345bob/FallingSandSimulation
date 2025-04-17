#include <sandstone.h>

#include <SDL.h>

#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_sdlrenderer2.h>

class fallingSand : public Sandstone::SDLApplication
{
public:

	struct Cells {

	};

	fallingSand(const char* windowName, const int width, const int height)
		: Sandstone::SDLApplication(windowName, width, height)
	{
		SDL_GetWindowSize(window, &windowW, &windowH);

		cellSize = new Sandstone::Vector2d(windowW / gridSize->x, windowH / gridSize->y);


		this->setupGrid();

		//Sandstone::SDLApplication::Run();
	}
	~fallingSand()
	{
		cells.clear();
	}

	virtual void RunApp() {
		SDL_Event e;
		
		if (SDL_PollEvent(&e)) {
			ImGui_ImplSDL2_ProcessEvent(&e);
			switch (e.type) {
			case SDL_QUIT:
				this->running = false;
				break;
			}
		}
	}

	void setupGrid() {
		cells.clear();
	}

private:
	std::vector<std::vector<Cells*>> cells;

	Sandstone::Vector2d* gridSize = new Sandstone::Vector2d(90, 90);
	Sandstone::Vector2d* cellSize;

	Sandstone::Vector2d* brushSize = new Sandstone::Vector2d(5);

	int windowW;
	int windowH;
};

Sandstone::Application* Sandstone::CreateApplication()
{
	return new fallingSand("Falling Sand", 540, 540);
}
