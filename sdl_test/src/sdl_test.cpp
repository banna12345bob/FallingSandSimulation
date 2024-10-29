#include <sandstone.h>

class sdl_test : public Sandstone::ConsoleApplication
{
public:
	sdl_test()
	{
		while (true)
		{
		}
	}
	~sdl_test()
	{

	}
};

Sandstone::Application* Sandstone::CreateApplication()
{
	return new sdl_test();
}
