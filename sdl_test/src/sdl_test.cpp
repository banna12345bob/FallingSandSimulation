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
};

Sandstone::Application* Sandstone::CreateApplication()
{
	return new sdl_test();
}
