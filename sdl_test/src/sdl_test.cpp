#include <sandstone.h>

class sdl_test : public Sandstone::SDLApplication
{
public:
	sdl_test()
	{
		
	}
	~sdl_test()
	{

	}

	void sdl_test::Run() {
		int mouseX;
		int mouseY;

		SDL_Event e;

		while (this->running == true) {
			while (SDL_PollEvent(&e)) {
				switch (e.type) {
				case SDL_QUIT:
					this->running = false;
					break;
				}
			}

			SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
			SDL_RenderClear(this->renderer);

			SDL_SetRenderDrawColor(this->renderer, 255, 0, 0, 255);

			SDL_GetMouseState(&mouseX, &mouseY);

			SDL_RenderDrawPoint(this->renderer, mouseX, mouseY);

			SDL_RenderPresent(this->renderer);
		}

		// Kill everything
		SDL_DestroyRenderer(this->renderer);
		SDL_DestroyWindow(this->window);

		//Quit SDL subsystems
		SDL_Quit();
	}
};

Sandstone::Application* Sandstone::CreateApplication()
{
	return new sdl_test();
}
