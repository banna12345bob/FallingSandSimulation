#include <sandstone.h>

#include <SDL.h>

#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_sdlrenderer2.h>

#include "fallingSandGUI.h"

#include "solidParticles.h"
#include "liquidParticles.h"
#include "gasParticles.h"

class fallingSand : public Sandstone::SDLApplication
{
public:

	enum particles
	{
		NONE,
		AIR,
		SAND,
		WATER,
		WOOD,
		METHANE,
		OIL
	};

	fallingSand(const char* windowName, const int width, const int height)
		: Sandstone::SDLApplication(windowName, width, height)
	{
		SDL_GetWindowSize(window, &windowW, &windowH);

		cellSize = new Sandstone::Vector2d(windowW / gridSize->x, windowH / gridSize->y);


		this->setupGrid();

	}
	~fallingSand()
	{
		cells.clear();
	}

	void fallingSand::Run() {
		double a, b;
		double deltaTime = 0;
		b = SDL_GetTicks();
		bool mouseDown = false;

		particles placing = NONE;


		SDL_Event e;
		SDL_Rect* mouseRect = new SDL_Rect();
		double frameIndex = 0;

		Particle* cell;

		while (this->running == true) {
			a = SDL_GetTicks();

			deltaTime = a - b;

			
			SS_INFO("fps: {0}", 1000 / deltaTime);

			if (SDL_PollEvent(&e)) {
				ImGui_ImplSDL2_ProcessEvent(&e);
				switch (e.type) {
				case SDL_QUIT:
					this->running = false;
					break;
				case SDL_MOUSEBUTTONDOWN:
					mouseDown = true;
					break;
				case SDL_MOUSEBUTTONUP:
					mouseDown = false;
					break;
				case SDL_KEYDOWN:
					switch (e.key.keysym.scancode)
					{
					case SDL_SCANCODE_S:
						placing = SAND;
						break;
					case SDL_SCANCODE_W:
						placing = WATER;
						break;
					case SDL_SCANCODE_C:
						placing = WOOD;
						break;
					case SDL_SCANCODE_M:
						placing = METHANE;
						break;
					case SDL_SCANCODE_O:
						placing = OIL;
						break;
					case SDL_SCANCODE_A:
						placing = AIR;
						break;
					case SDL_SCANCODE_ESCAPE:
						this->setupGrid();
						placing = NONE;
						break;
					default:
						placing = NONE;
						break;
					}
				}
			}

			ImGui_ImplSDLRenderer2_NewFrame();
			ImGui_ImplSDL2_NewFrame();
			ImGui::NewFrame();

			if (mouseDown) {
				for (int x = 0; x < brushSize->x; x++)
				{
					for (int y = 0; y < brushSize->y; y++)
					{
						if ((mouseRect->y * gridSize->y / windowH) + y >= gridSize->y || (mouseRect->y * gridSize->y / windowH) + y < 0) {
							break;
						}
						if ((mouseRect->x * gridSize->x / windowW) + x >= gridSize->x || (mouseRect->x * gridSize->x / windowW) + x < 0) {
							break;
						}
						Sandstone::Vector2d pos = Sandstone::Vector2d((mouseRect->x * gridSize->x / windowW) + x, (mouseRect->y * gridSize->y / windowH) + y);
						switch (placing)
						{
						case AIR:
							cells[pos.x][pos.y] = new air(cells[pos.x][pos.y]);
							break;
						case SAND:
							cells[pos.x][pos.y] = new sand(cells[pos.x][pos.y]);
							break;
						case WATER:
							cells[pos.x][pos.y] = new water(cells[pos.x][pos.y]);
							break;
						case WOOD:
							cells[pos.x][pos.y] = new wood(cells[pos.x][pos.y]);
							break;
						case METHANE:
							cells[pos.x][pos.y] = new methane(cells[pos.x][pos.y]);
							break;
						case OIL:
							cells[pos.x][pos.y] = new oil(cells[pos.x][pos.y]);
							break;
						default:
							break;
						}
					}
				}
			}

			fallingSandGUI::render();

			SDL_GetMouseState(&mouseRect->x, &mouseRect->y);

			SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
			SDL_RenderClear(this->renderer);

			for (int x = 0; x <= gridSize->x; x++)
			{				
				for (int y = 0; y <= gridSize->y; y++)
				{
					cell = cells[x][y];
					if (y % 2) {
						cell = cells[gridSize->x - x][y];
					}

					SDL_SetRenderDrawColor(this->renderer, cell->colour.r, cell->colour.g, cell->colour.b, cell->colour.a);
					SDL_RenderFillRect(this->renderer, cell);

					cell->update(frameIndex);

				}
			}

			mouseRect->w = cellSize->x * brushSize->x;
			mouseRect->h = cellSize->y * brushSize->y;

			SDL_SetRenderDrawColor(this->renderer, 0, 255, 0, 255);
			SDL_RenderFillRect(this->renderer, mouseRect);

			ImGui::Render();
			ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), this->renderer);

			SDL_RenderPresent(this->renderer);

			b = a;
			frameIndex += 0.025 * deltaTime;
		}
	}

	void setupGrid() {
		cells.clear();

		for (int y = 0; y <= gridSize->y; y++)
		{
			for (int x = 0; x <= gridSize->x; x++)
			{
				if (cells.size() != (gridSize->x + 1)) {
					std::vector<Particle*> tempVec;
					tempVec.push_back(new air(this->window, cellSize, gridSize, &cells));
					cells.push_back(tempVec);
				} else {
					cells[x].push_back(new air(this->window, cellSize, gridSize, &cells));
				}

				cells[x][y]->x = cells[x][y]->w * x;
				cells[x][y]->y = cells[x][y]->h * y;

				cells[x][y]->cellPos = Sandstone::Vector2d(x, y);
			}
		}
	}

private:
	std::vector<std::vector<Particle*>> cells;

	Sandstone::Vector2d* gridSize = new Sandstone::Vector2d(60, 60);
	Sandstone::Vector2d* cellSize;

	Sandstone::Vector2d* brushSize = new Sandstone::Vector2d(5);

	int windowW;
	int windowH;
};

Sandstone::Application* Sandstone::CreateApplication()
{
	return new fallingSand("Falling Sand", 540, 540);
}
