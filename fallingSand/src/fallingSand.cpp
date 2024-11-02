#include <sandstone.h>

#include <SDL.h>

#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_sdlrenderer2.h>

#include "fallingSandGUI.h"

class fallingSand : public Sandstone::SDLApplication
{
public:

	enum particleTypes
	{
		NONE,
		AIR,
		SAND,
		WATER,
		STONE
	};

	struct Cell : SDL_Rect
	{
		Sandstone::Vector2d cellPos = Sandstone::Vector2d(0, 0);

		int windowW;
		int windowH;

		SDL_Window* window;
		Sandstone::Vector2d* cellSize;
		Sandstone::Vector2d* gridSize;

		SDL_Colour colour = { 0, 0, 0, 255 };

		particleTypes particleType = NONE;

		Cell(SDL_Window* window, Sandstone::Vector2d* cellSize, Sandstone::Vector2d* gridSize) {
			this->window = window;
			this->cellSize = cellSize;
			this->gridSize = gridSize;

			SDL_GetWindowSize(this->window, &windowW, &windowH);

			w = cellSize->x;
			h = cellSize->y;

			/*colour.r = 0;
			colour.g = 0;
			colour.b = 0;
			colour.a = 255;*/
		}
	};

	struct Particle : Cell {
		std::map<std::pair<int, int>, Particle*>* cells;

		float lastUpdated;

		Particle(SDL_Window* window, Sandstone::Vector2d* cellSize, Sandstone::Vector2d* gridSize, std::map<std::pair<int, int>, Particle*>* cells) :
			Cell(window, cellSize, gridSize) {
			this->cells = cells;
			lastUpdated = 0;
		}

		virtual void update(float frameIndex) {
			this->lastUpdated = frameIndex;
		}

		void swap(Particle* destination) {
			int OldX = this->x;
			int OldY = this->y;
			Sandstone::Vector2d OldPos = this->cellPos;


			this->x = destination->x;
			this->y = destination->y;
			this->cellPos = destination->cellPos;

			destination->x = OldX;
			destination->y = OldY;
			destination->cellPos = OldPos;

			cells->at(std::make_pair(destination->cellPos.x, destination->cellPos.y)) = destination;

			cells->at(std::make_pair(this->cellPos.x, this->cellPos.y)) = this;
		}
	};

	struct air : Particle {
		air(SDL_Window* window, Sandstone::Vector2d* cellSize, Sandstone::Vector2d* gridSize, std::map<std::pair<int, int>, Particle*>* cells) :
			Particle(window, cellSize, gridSize, cells) {
			colour.r = 255;

			this->particleType = AIR;
		}

		air(Particle* particle) :
			Particle(particle->window, particle->cellSize, particle->gridSize, particle->cells) {
			colour.r = 255;
			colour.g = 0;
			colour.b = 0;

			this->x = particle->x;
			this->y = particle->y;
			this->cellPos = particle->cellPos;

			this->particleType = AIR;
		}
	};

	struct sand : Particle {
		sand(Particle* particle) :
			Particle(particle->window, particle->cellSize, particle->gridSize, particle->cells) {
			colour.r = 255;
			colour.g = 255;
			colour.b = 0;

			this->x = particle->x;
			this->y = particle->y;
			this->cellPos = particle->cellPos;

			this->particleType = SAND;
		}

		virtual void update(float frameIndex) override {
			if (std::round(lastUpdated) == std::round(frameIndex)) {
				return;
			}
			this->lastUpdated = frameIndex;
			if (cellPos.y + 1 != gridSize->y) {
				if (cells->at(std::make_pair(this->cellPos.x, this->cellPos.y + 1))->particleType == AIR || cells->at(std::make_pair(this->cellPos.x, this->cellPos.y + 1))->particleType == WATER) {
					return this->swap(cells->at(std::make_pair(this->cellPos.x, this->cellPos.y + 1)));

				} if (cellPos.x != 0 && (cells->at(std::make_pair(this->cellPos.x - 1, this->cellPos.y + 1))->particleType == AIR || cells->at(std::make_pair(this->cellPos.x - 1, this->cellPos.y + 1))->particleType == WATER)) {
					return this->swap(cells->at(std::make_pair(this->cellPos.x - 1, this->cellPos.y + 1)));

				} if (cellPos.x + 1 != gridSize->x && (cells->at(std::make_pair(this->cellPos.x + 1, this->cellPos.y + 1))->particleType == AIR || cells->at(std::make_pair(this->cellPos.x + 1, this->cellPos.y + 1))->particleType == WATER)) {
					return this->swap(cells->at(std::make_pair(this->cellPos.x + 1, this->cellPos.y + 1)));

				}
			}
		}
	};

	struct water : Particle {
		water(Particle* particle) :
			Particle(particle->window, particle->cellSize, particle->gridSize, particle->cells) {
			colour.r = 0;
			colour.g = 0;
			colour.b = 255;

			this->x = particle->x;
			this->y = particle->y;
			this->cellPos = particle->cellPos;

			this->particleType = WATER;
		}

		virtual void update(float frameIndex) override {
			if (std::round(lastUpdated) == std::round(frameIndex)) {
				return;
			}
			this->lastUpdated = frameIndex;
			if (cellPos.y + 1 != gridSize->y) {
				if (cells->at(std::make_pair(this->cellPos.x, this->cellPos.y + 1))->particleType == AIR) {
					return this->swap(cells->at(std::make_pair(this->cellPos.x, this->cellPos.y + 1)));

				} if (cellPos.x != 0 && cells->at(std::make_pair(this->cellPos.x - 1, this->cellPos.y + 1))->particleType == AIR) {
					return this->swap(cells->at(std::make_pair(this->cellPos.x - 1, this->cellPos.y + 1)));

				} if (cellPos.x + 1 != gridSize->x && cells->at(std::make_pair(this->cellPos.x + 1, this->cellPos.y + 1))->particleType == AIR) {
					return this->swap(cells->at(std::make_pair(this->cellPos.x + 1, this->cellPos.y + 1)));

				} if (cellPos.x != 0 && cells->at(std::make_pair(this->cellPos.x - 1, this->cellPos.y))->particleType == AIR) {
					return this->swap(cells->at(std::make_pair(this->cellPos.x - 1, this->cellPos.y)));

				} if (cellPos.x + 1 != gridSize->x && cells->at(std::make_pair(this->cellPos.x + 1, this->cellPos.y))->particleType == AIR) {
					return this->swap(cells->at(std::make_pair(this->cellPos.x + 1, this->cellPos.y)));

				}

			}
		}
	};


	struct stone : Particle {
		stone(Particle* particle) :
			Particle(particle->window, particle->cellSize, particle->gridSize, particle->cells) {
			colour.r = 55;
			colour.g = 55;
			colour.b = 55;

			this->x = particle->x;
			this->y = particle->y;
			this->cellPos = particle->cellPos;

			this->particleType = STONE;
		}
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
		Uint64 a, b;
		double deltaTime = 0;
		b = SDL_GetTicks();
		bool mouseDown = false;

		particleTypes placing = NONE;


		SDL_Event e;
		SDL_Rect* mouseRect = new SDL_Rect();
		float frameIndex = 0;

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
						placing = STONE;
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
						if ((mouseRect->y * gridSize->y / windowH) + y < gridSize->y && (mouseRect->y * gridSize->y / windowH) + y >= 0)
							if ((mouseRect->x * gridSize->x / windowW) + x < gridSize->x && (mouseRect->x * gridSize->x / windowW) + x >= 0)
								if (placing == SAND)
									cells[std::make_pair((mouseRect->x * gridSize->x / windowW) + x, (mouseRect->y * gridSize->y / windowH) + y)] = new sand(cells[std::make_pair((mouseRect->x * gridSize->x / windowW) + x, (mouseRect->y * gridSize->y / windowH) + y)]);
								else if (placing == WATER)
									cells[std::make_pair((mouseRect->x * gridSize->x / windowW) + x, (mouseRect->y * gridSize->y / windowH) + y)] = new water(cells[std::make_pair((mouseRect->x * gridSize->x / windowW) + x, (mouseRect->y * gridSize->y / windowH) + y)]);
								else if (placing == STONE)
									cells[std::make_pair((mouseRect->x * gridSize->x / windowW) + x, (mouseRect->y * gridSize->y / windowH) + y)] = new stone(cells[std::make_pair((mouseRect->x * gridSize->x / windowW) + x, (mouseRect->y * gridSize->y / windowH) + y)]);
								else if (placing == AIR)
									cells[std::make_pair((mouseRect->x * gridSize->x / windowW) + x, (mouseRect->y * gridSize->y / windowH) + y)] = new air(cells[std::make_pair((mouseRect->x * gridSize->x / windowW) + x, (mouseRect->y * gridSize->y / windowH) + y)]);

					}
				}
			}

			fallingSandGUI::render();

			SDL_GetMouseState(&mouseRect->x, &mouseRect->y);

			SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
			SDL_RenderClear(this->renderer);

			for (int x = gridSize->x - 1; x >= 0; x--)
			{				
				for (int y = gridSize->y - 1; y >= 0; y--)
				{
					if (cells[std::make_pair(x, y)]->particleType != AIR) {
						SDL_SetRenderDrawColor(this->renderer, cells[std::make_pair(x, y)]->colour.r, cells[std::make_pair(x, y)]->colour.g, cells[std::make_pair(x, y)]->colour.b, cells[std::make_pair(x, y)]->colour.a);
						SDL_RenderFillRect(this->renderer, cells[std::make_pair(x, y)]);
					}
					/*else {
						SDL_SetRenderDrawColor(this->renderer, cells[std::make_pair(x, y)]->colour.r, cells[std::make_pair(x, y)]->colour.g, cells[std::make_pair(x, y)]->colour.b, cells[std::make_pair(x, y)]->colour.a);
						SDL_RenderDrawRect(this->renderer, cells[std::make_pair(x, y)]);
					}*/

					cells[std::make_pair(x, y)]->update(frameIndex);
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
			frameIndex += 0.05 * deltaTime;
		}
	}

	void setupGrid() {
		cells.clear();

		for (int x = 0; x <= gridSize->x; x++)
		{
			for (int y = 0; y < gridSize->y; y++)
			{
				cells[std::make_pair(x, y)] = new air(this->window, cellSize, gridSize, &cells);

				cells[std::make_pair(x, y)]->x = cells[std::make_pair(x, y)]->w * x;
				cells[std::make_pair(x, y)]->y = cells[std::make_pair(x, y)]->h * y;

				cells[std::make_pair(x, y)]->cellPos = Sandstone::Vector2d(x, y);
			}
		}
	}

private:
	std::map<std::pair<int, int>, Particle*> cells;

	Sandstone::Vector2d* gridSize = new Sandstone::Vector2d(60, 60);
	Sandstone::Vector2d* cellSize;

	Sandstone::Vector2d* brushSize = new Sandstone::Vector2d(2);

	int windowW;
	int windowH;
};

Sandstone::Application* Sandstone::CreateApplication()
{
	return new fallingSand("Falling Sand", 540, 540);
}
