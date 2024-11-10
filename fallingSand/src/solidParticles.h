#pragma once

#include "particleTypes.h"

struct wood : solid {
	wood(Particle* particle) :
		solid(particle) {
		colour.r = 66;
		colour.g = 40;
		colour.b = 14;
	}

	virtual bool update(double frameIndex) override {
		return false;
	}
};

struct sand : solid {
	sand(Particle* particle) :
		solid(particle) {
		colour.r = 255;
		colour.g = 255;
		colour.b = 0;

		this->density = 1602;
	}
	virtual bool update(double frameIndex) override {
		if (!Particle::update(frameIndex)) {
			return false;
		}
		if (solid::update(frameIndex)) {
			return true;
		}
		return true;
	}
};