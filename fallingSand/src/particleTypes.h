#pragma once

#include <sandstone.h>

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

struct Cell : SDL_FRect
{
	Sandstone::Vector2d cellPos = Sandstone::Vector2d(0, 0);

	int windowW;
	int windowH;

	SDL_Window* window;
	Sandstone::Vector2d* cellSize;
	Sandstone::Vector2d* gridSize;

	SDL_Color colour = { 0, 0, 0, 255 };

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
	std::vector<std::vector<Particle*>>* cells;

	// Desnity in kg/m^3: default 10e100
	double density = 10e100;

	// Gravity in m/s^2
	double gravity = 9.8;

	double lastUpdated = -1;

	Particle(SDL_Window* window, Sandstone::Vector2d* cellSize, Sandstone::Vector2d* gridSize, std::vector<std::vector<Particle*>>* cells) :
		Cell(window, cellSize, gridSize) {
		this->cells = cells;
	}

	virtual bool update(double frameIndex) {
		if (std::round(lastUpdated) == std::round(frameIndex)) {
			return false;
		}
		return true;
	}

	bool swap(Particle* destination) {
		if (destination->cellPos.x == -1 && destination->cellPos.y == -1) {
			return false;
		}
		if (destination->density * 0.1 > this->density || destination->density > 1e99) {
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

		cells->at(destination->cellPos.x).at(destination->cellPos.y) = destination;

		cells->at(this->cellPos.x).at(this->cellPos.y) = this;

		return true;
	}

	Particle* getCell(directions direction) {
		if (direction == UP && this->cellPos.y != 0) {
			return cells->at(this->cellPos.x).at(this->cellPos.y - 1);
		} if (direction == DOWN && this->cellPos.y + 1 < gridSize->y) {
			return cells->at(this->cellPos.x).at(this->cellPos.y + 1);
		} if (direction == RIGHT && this->cellPos.x + 1 < gridSize->x) {
			return cells->at(this->cellPos.x + 1).at(this->cellPos.y);
		} if (direction == LEFT && this->cellPos.x != 0) {
			return cells->at(this->cellPos.x - 1).at(this->cellPos.y);
		} if (direction == UPRIGHT && this->cellPos.y != 0 && this->cellPos.x + 1 < gridSize->x) {
			return cells->at(this->cellPos.x + 1).at(this->cellPos.y - 1);
		} if (direction == UPLEFT && this->cellPos.y != 0 && this->cellPos.x != 0) {
			return cells->at(this->cellPos.x - 1).at(this->cellPos.y - 1);
		}  if (direction == DOWNRIGHT && this->cellPos.y + 1 < gridSize->y && this->cellPos.x + 1 < gridSize->x) {
			return cells->at(this->cellPos.x + 1).at(this->cellPos.y + 1);
		} if (direction == DOWNLEFT && this->cellPos.y + 1 < gridSize->y && this->cellPos.x != 0) {
			return cells->at(this->cellPos.x - 1).at(this->cellPos.y + 1);
		}
		return nullptr;
	}

};

struct solid : Particle {
	solid(Particle* particle) :
		Particle(particle->window, particle->cellSize, particle->gridSize, particle->cells) {

		this->x = particle->x;
		this->y = particle->y;
		this->cellPos = particle->cellPos;
	}

	virtual bool update(double frameIndex) override {
		if (!Particle::update(frameIndex)) {
			return false;
		}

		Particle* cell;
		this->lastUpdated = frameIndex;
		if ((cell = this->getCell(DOWN)) && cell != nullptr && cell->density * gravity < this->density * gravity) {
			return this->swap(cell);
		} if ((cell = this->getCell(DOWNLEFT)) && cell != nullptr && cell->density * gravity < this->density * gravity) {
			return this->swap(cell);
		} if ((cell = this->getCell(DOWNRIGHT)) && cell != nullptr && cell->density * gravity < this->density * gravity) {
			return this->swap(cell);
		}
		return false;
	}
};

struct liquid : Particle {
	liquid(Particle* particle) :
		Particle(particle->window, particle->cellSize, particle->gridSize, particle->cells) {
		this->x = particle->x;
		this->y = particle->y;
		this->cellPos = particle->cellPos;
	}

	virtual bool update(double frameIndex) override {
		if (!Particle::update(frameIndex)) {
			return false;
		}

		Particle* cell;
		this->lastUpdated = frameIndex;
		if ((cell = this->getCell(DOWN)) && cell != nullptr && cell->density * gravity < this->density * gravity) {
			return this->swap(cell);
		} if ((cell = this->getCell(DOWNLEFT)) && cell != nullptr && cell->density * gravity < this->density * gravity) {
			return this->swap(cell);
		} if ((cell = this->getCell(DOWNRIGHT)) && cell != nullptr && cell->density * gravity < this->density * gravity) {
			return this->swap(cell);
		} if ((cell = this->getCell(LEFT)) && cell != nullptr && cell->density * gravity < this->density * gravity) {
			return this->swap(cell);
		} if ((cell = this->getCell(RIGHT)) && cell != nullptr && cell->density * gravity < this->density * gravity) {
			return this->swap(cell);
		}
		return false;
	}
};

struct gas : Particle {
	gas(SDL_Window* window, Sandstone::Vector2d* cellSize, Sandstone::Vector2d* gridSize, std::vector<std::vector<Particle*>>* cells) :
		Particle(window, cellSize, gridSize, cells) {
	}

	gas(Particle* particle) :
		Particle(particle->window, particle->cellSize, particle->gridSize, particle->cells) {
		this->x = particle->x;
		this->y = particle->y;
		this->cellPos = particle->cellPos;
	}

	virtual bool update(double frameIndex) override {
		if (!Particle::update(frameIndex)) {
			return false;
		}

		Particle* cell;
		this->lastUpdated = frameIndex;
		if ((cell = this->getCell(UP)) && cell != nullptr && cell->density * gravity > this->density * gravity) {
			return this->swap(cell);
		} if ((cell = this->getCell(UPLEFT)) && cell != nullptr && cell->density * gravity > this->density * gravity) {
			return this->swap(cell);
		} if ((cell = this->getCell(UPRIGHT)) && cell != nullptr && cell->density * gravity > this->density * gravity) {
			return this->swap(cell);
		} if ((cell = this->getCell(LEFT)) && cell != nullptr && cell->density * gravity > this->density * gravity) {
			return this->swap(cell);
		} if ((cell = this->getCell(RIGHT)) && cell != nullptr && cell->density * gravity > this->density * gravity) {
			return this->swap(cell);
		}
		return false;
	}
};