#pragma once

#include "core.h"
#include <string>
#include <sstream>
#include <map>
#include "Application.h"

namespace Sandstone {

	class ConsoleApplication : public Application
	{
	public:
		ConsoleApplication();
		virtual ~ConsoleApplication();

		virtual void Run() override;
	};

	//	To be defined in client
    Application* CreateApplication();
}
