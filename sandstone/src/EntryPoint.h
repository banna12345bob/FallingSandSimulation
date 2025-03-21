#pragma once

#if defined(SS_PLATFORM_WINDOWS) || defined(SS_PLATFORM_MACOS_ARM64) || defined(SS_PLATFORM_MACOS_x86)

extern Sandstone::Application* Sandstone::CreateApplication();

	int main(int argc, char** argv)
	{
		Sandstone::Log::Init();

		SS_CORE_INFO("Sandstone Initialised");

		auto app = Sandstone::CreateApplication();
		app->Run();
		delete app;
		return 0;
	}
#else
    #error "Entry point not built for current OS!"
#endif
