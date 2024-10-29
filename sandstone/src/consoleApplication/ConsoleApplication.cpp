#include "ConsoleApplication.h"
#include "Log.h"


namespace Sandstone {
	ConsoleApplication::ConsoleApplication()
	{
	}

	ConsoleApplication::~ConsoleApplication()
	{}

	void ConsoleApplication::Run()
	{
		while (running) {
			SS_CORE_INFO("Running");
		}
	}

}
