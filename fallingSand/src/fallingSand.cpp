#include <sandstone.h>

#include <SDL.h>

#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_sdlrenderer2.h>

#include "fallingSandGUI.h"

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
		METHANE
	};

	enum directions
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
		UPLEFT,
		UPRIGHT,
		DOWNLEFT,
		DOWNRIGHT
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

		Cell(SDL_Window* window, Sandstone::Vector2d* cellSize, Sandstone::Vector2d* gridSize) {
			this->window = window;
			this->cellSize = cellSize;
			this->gridSize = gridSize;

			SDL_GetWindowSize(this->window, &windowW, &windowH);

			w = cellSize->x;
			h = cellSize->y;
		}
	};

	struct Particle : Cell {
		std::map<std::pair<int, int>, Particle*>* cells;

		// Desnity on kg/m^3: default inf
		double desnsity = INFINITY;
		float lastUpdated;

		Particle(SDL_Window* window, Sandstone::Vector2d* cellSize, Sandstone::Vector2d* gridSize, std::map<std::pair<int, int>, Particle*>* cells) :
			Cell(window, cellSize, gridSize) {
			this->cells = cells;
			lastUpdated = 0;
		}

		virtual bool update(float frameIndex) {
			if (std::round(lastUpdated) == std::round(frameIndex)) {
				return false;
			}
			return true;
		}

		bool swap(Particle* destination) {
			if (destination->cellPos.x == -1 && destination->cellPos.y == -1) {
				return false;
			}
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
			return true;
		}

		Particle* getCell(directions direction) {
			if (direction == UP && this->cellPos.y != 0) {
				return cells->at(std::make_pair(this->cellPos.x, this->cellPos.y - 1));
			} if (direction == DOWN && this->cellPos.y + 1 != gridSize->y) {
				return cells->at(std::make_pair(this->cellPos.x, this->cellPos.y + 1));
			} if (direction == RIGHT && this->cellPos.x + 1 != gridSize->x) {
				return cells->at(std::make_pair(this->cellPos.x + 1, this->cellPos.y));
			} if (direction == LEFT && this->cellPos.x != 0) {
				return cells->at(std::make_pair(this->cellPos.x - 1, this->cellPos.y));
			} if (direction == UPRIGHT && this->cellPos.y != 0 && this->cellPos.x + 1 != gridSize->x) {
				return cells->at(std::make_pair(this->cellPos.x + 1, this->cellPos.y - 1));
			} if (direction == UPLEFT && this->cellPos.y != 0 && this->cellPos.x != 0) {
				return cells->at(std::make_pair(this->cellPos.x - 1, this->cellPos.y - 1));
			} if (direction == DOWNRIGHT && this->cellPos.y + 1 != gridSize->y && this->cellPos.x + 1 != gridSize->x) {
				return cells->at(std::make_pair(this->cellPos.x + 1, this->cellPos.y + 1));
			} if (direction == DOWNLEFT && this->cellPos.y + 1 != gridSize->y && this->cellPos.x != 0) {
				return cells->at(std::make_pair(this->cellPos.x - 1, this->cellPos.y + 1));
			}
			return cells->at(std::make_pair(-1, -1));
		}

	};

	struct wood : Particle {
		wood(Particle* particle) :
			Particle(particle->window, particle->cellSize, particle->gridSize, particle->cells) {
			colour.r = 55;
			colour.g = 55;
			colour.b = 55;

			this->x = particle->x;
			this->y = particle->y;
			this->cellPos = particle->cellPos;

			this->desnsity = 1e100;
		}
	};

	struct solid : Particle {
		solid(Particle* particle) :
			Particle(particle->window, particle->cellSize, particle->gridSize, particle->cells) {

			this->x = particle->x;
			this->y = particle->y;
			this->cellPos = particle->cellPos;
		}

		virtual bool update(float frameIndex) override {
			if (!Particle::update(frameIndex)) {
				return false;
			}

			Particle* cell;
			this->lastUpdated = frameIndex;
			if ((cell = this->getCell(DOWN))->desnsity < this->desnsity) {
				return this->swap(cell);
			} if ((cell = this->getCell(DOWNLEFT))->desnsity < this->desnsity) {
				return this->swap(cell);
			} if ((cell = this->getCell(DOWNRIGHT))->desnsity < this->desnsity) {
				return this->swap(cell);
			}
			return false;
		}
	};

	struct sand : solid {
		sand(Particle* particle) :
			solid(particle) {
			colour.r = 255;
			colour.g = 255;
			colour.b = 0;

			this->desnsity = 1602;
		}
		virtual bool update(float frameIndex) {
			if (!Particle::update(frameIndex)) {
				return false;
			}
			if (solid::update(frameIndex)) {
				return true;
			}
			return true;
		}
	};

	struct liquid : Particle {
		liquid(Particle* particle) :
			Particle(particle->window, particle->cellSize, particle->gridSize, particle->cells) {
			this->x = particle->x;
			this->y = particle->y;
			this->cellPos = particle->cellPos;
		}

		virtual bool update(float frameIndex) override {
			if (!Particle::update(frameIndex)) {
				return false;
			}

			Particle* cell;
			this->lastUpdated = frameIndex;
			if ((cell = this->getCell(DOWN))->desnsity < this->desnsity) {
				return this->swap(cell);
			} if ((cell = this->getCell(DOWNLEFT))->desnsity < this->desnsity) {
				return this->swap(cell);
			} if ((cell = this->getCell(DOWNRIGHT))->desnsity < this->desnsity) {
				return this->swap(cell);
			} if ((cell = this->getCell(LEFT))->desnsity < this->desnsity) {
				return this->swap(cell);
			} if ((cell = this->getCell(RIGHT))->desnsity < this->desnsity) {
				return this->swap(cell);
			}
			return false;
		}
	};

	struct water : liquid {
		water(Particle* particle) :
			liquid(particle) {
			colour.r = 0;
			colour.g = 0;
			colour.b = 255;

			this->desnsity = 997;
		}
		virtual bool update(float frameIndex) {
			if (!Particle::update(frameIndex)) {
				return false;
			}
			return liquid::update(frameIndex);
		}
	};

	struct gas : Particle {
		gas(SDL_Window* window, Sandstone::Vector2d* cellSize, Sandstone::Vector2d* gridSize, std::map<std::pair<int, int>, Particle*>* cells) :
			Particle(window, cellSize, gridSize, cells) {}

		gas(Particle* particle) :
			Particle(particle->window, particle->cellSize, particle->gridSize, particle->cells) {
			this->x = particle->x;
			this->y = particle->y;
			this->cellPos = particle->cellPos;
		}

		virtual bool update(float frameIndex) override {
			if (!Particle::update(frameIndex)) {
				return false;
			}

			Particle* cell;
			this->lastUpdated = frameIndex;
			if ((cell = this->getCell(UP))->desnsity > this->desnsity && cell->desnsity < 10e99) {
				return this->swap(cell);
			} if ((cell = this->getCell(UPLEFT))->desnsity > this->desnsity && cell->desnsity < 10e99) {
				return this->swap(cell);
			} if ((cell = this->getCell(UPRIGHT))->desnsity > this->desnsity && cell->desnsity < 10e99) {
				return this->swap(cell);
			} if ((cell = this->getCell(LEFT))->desnsity > this->desnsity && cell->desnsity < 10e99) {
				return this->swap(cell);
			} if ((cell = this->getCell(RIGHT))->desnsity > this->desnsity && cell->desnsity < 10e99) {
				return this->swap(cell);
			}
			return false;
		}
	};

	struct air : gas {
		air(SDL_Window* window, Sandstone::Vector2d* cellSize, Sandstone::Vector2d* gridSize, std::map<std::pair<int, int>, Particle*>* cells) :
			gas(window, cellSize, gridSize, cells) {
			this->desnsity = 1.225;
		}

		air(Particle* particle) :
			gas(particle->window, particle->cellSize, particle->gridSize, particle->cells) {
			colour.r = 0;
			colour.g = 0;
			colour.b = 0;

			this->x = particle->x;
			this->y = particle->y;
			this->cellPos = particle->cellPos;

			this->desnsity = 1.225;
		}

		virtual bool update(float frameIndex) {
			return false;
		}
	};

	struct methane : gas {
		methane(Particle* particle) :
			gas(particle) {
			colour.r = 0;
			colour.g = 255;
			colour.b = 0;

			this->desnsity = 0.1786;
		}
		virtual bool update(float frameIndex) {
			if (!Particle::update(frameIndex)) {
				return false;
			}
			return gas::update(frameIndex);
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

		particles placing = NONE;


		SDL_Event e;
		SDL_Rect* mouseRect = new SDL_Rect();
		float frameIndex = 0;

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
					case SDL_SCANCODE_G:
						placing = METHANE;
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
						switch (placing)
						{
						case AIR:
							cells[std::make_pair((mouseRect->x * gridSize->x / windowW) + x, (mouseRect->y * gridSize->y / windowH) + y)] = new air(cells[std::make_pair((mouseRect->x * gridSize->x / windowW) + x, (mouseRect->y * gridSize->y / windowH) + y)]);
							break;
						case SAND:
							cells[std::make_pair((mouseRect->x * gridSize->x / windowW) + x, (mouseRect->y * gridSize->y / windowH) + y)] = new sand(cells[std::make_pair((mouseRect->x * gridSize->x / windowW) + x, (mouseRect->y * gridSize->y / windowH) + y)]);
							break;
						case WATER:
							cells[std::make_pair((mouseRect->x * gridSize->x / windowW) + x, (mouseRect->y * gridSize->y / windowH) + y)] = new water(cells[std::make_pair((mouseRect->x * gridSize->x / windowW) + x, (mouseRect->y * gridSize->y / windowH) + y)]);
							break;
						case WOOD:
							cells[std::make_pair((mouseRect->x * gridSize->x / windowW) + x, (mouseRect->y * gridSize->y / windowH) + y)] = new wood(cells[std::make_pair((mouseRect->x * gridSize->x / windowW) + x, (mouseRect->y * gridSize->y / windowH) + y)]);
							break;
						case METHANE:
							cells[std::make_pair((mouseRect->x * gridSize->x / windowW) + x, (mouseRect->y * gridSize->y / windowH) + y)] = new methane(cells[std::make_pair((mouseRect->x * gridSize->x / windowW) + x, (mouseRect->y * gridSize->y / windowH) + y)]);
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
					cell = cells[std::make_pair(x, y)];
					if (y % 2) {
						cell = cells[std::make_pair(gridSize->x - x, y)];
					}

					//if (cell->particleType != GAS) {
					SDL_SetRenderDrawColor(this->renderer, cell->colour.r, cell->colour.g, cell->colour.b, cell->colour.a);
					SDL_RenderFillRect(this->renderer, cell);

					cell->update(frameIndex);
					//} //else {
						//SDL_SetRenderDrawColor(this->renderer, cells[std::make_pair(x, y)]->colour.r, cells[std::make_pair(x, y)]->colour.g, cells[std::make_pair(x, y)]->colour.b, cells[std::make_pair(x, y)]->colour.a);
						//SDL_RenderDrawRect(this->renderer, cells[std::make_pair(x, y)]);
					//}

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
		cells[std::make_pair(-1, -1)] = new Particle(this->window, cellSize, gridSize, &cells);
		cells[std::make_pair(-1, -1)]->cellPos = Sandstone::Vector2d(-1, -1);

		for (int x = 0; x <= gridSize->x; x++)
		{
			for (int y = 0; y <= gridSize->y; y++)
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

	Sandstone::Vector2d* brushSize = new Sandstone::Vector2d(10);

	int windowW;
	int windowH;
};

Sandstone::Application* Sandstone::CreateApplication()
{
	return new fallingSand("Falling Sand", 540, 540);
}
