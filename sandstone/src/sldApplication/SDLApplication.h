#pragma once

#include "core.h"
#include <string>
#include <sstream>
#include <map>
#include "Application.h"
#include <SDL.h>

namespace Sandstone {

	struct Vector2d {
		int x;
		int y;

		Vector2d(int x, int y) {
			this->x = x;
			this->y = y;
		}

		Vector2d(int x) {
			this->x = x;
			this->y = x;
		}

		Vector2d add(Vector2d vec) {
			this->x += vec.x;
			this->y += vec.y;
			return Vector2d(this->x += vec.x, this->y + vec.y);
		}

		Vector2d add(int x, int y) {
			return Vector2d(this->x + x, this->y + y);
		}

		std::string toString() {
			return std::to_string(this->x) + ", " + std::to_string(this->y);
		}

		bool operator == (const Vector2d& o) const {
			if (o.x == this->x)
				return true;
			else if (o.y == this->y)
				return true;
			return false;
		}
	};

	class SDLApplication : public Application
	{
	public:
		SDLApplication();
		virtual ~SDLApplication();

		virtual void Run() override;

		SDL_Window* window = NULL;

		//The surface contained by the window
		SDL_Surface* screenSurface = NULL;

		SDL_Renderer* renderer;
	};

	//	To be defined in client
    Application* CreateApplication();
}
