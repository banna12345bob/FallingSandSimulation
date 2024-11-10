#pragma once

#include "particleTypes.h"

struct water : liquid {
	water(Particle* particle) :
		liquid(particle) {
		colour.r = 0;
		colour.g = 0;
		colour.b = 255;

		this->density = 997;
	}
	virtual bool update(double frameIndex) override {
		if (!Particle::update(frameIndex)) {
			return false;
		}
		return liquid::update(frameIndex);
	}
};

struct oil : liquid {
	oil(Particle* particle) :
		liquid(particle) {
		colour.r = 50;
		colour.g = 50;
		colour.b = 50;

		this->density = 950;
	}
	virtual bool update(double frameIndex) override {
		if (!Particle::update(frameIndex)) {
			return false;
		}
		return liquid::update(frameIndex);
	}
};