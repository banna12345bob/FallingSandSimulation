#pragma once

#include "particleTypes.h"

struct air : gas {
	air(SDL_Window* window, Sandstone::Vector2d* cellSize, Sandstone::Vector2d* gridSize, std::vector<std::vector<Particle*>>* cells) :
		gas(window, cellSize, gridSize, cells) {
		this->density = 1.225;
	}

	air(Particle* particle) :
		gas(particle->window, particle->cellSize, particle->gridSize, particle->cells) {
		colour.r = 0;
		colour.g = 0;
		colour.b = 0;

		this->x = particle->x;
		this->y = particle->y;
		this->cellPos = particle->cellPos;

		this->density = 1.225;
	}

	virtual bool update(double frameIndex) override {
		//if (!Particle::update(frameIndex)) {
		//	return false;
		//}
		//return gas::update(frameIndex);
		return false;
	}
};

struct methane : gas {
	methane(Particle* particle) :
		gas(particle) {
		colour.r = 0;
		colour.g = 255;
		colour.b = 0;

		this->density = 0.1786;
	}
	virtual bool update(double frameIndex) override {
		if (!Particle::update(frameIndex)) {
			return false;
		}
		return gas::update(frameIndex);
	}
};