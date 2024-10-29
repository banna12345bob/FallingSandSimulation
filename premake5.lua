workspace "sdl_test"
	startproject "sdl_test"

	configurations
	{
		"Debug_x86_64",
		"Release_x86_64",
		"Dist_x86_64"
	}
	filter "configurations:*_x86_64"
		architecture "x86_64"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["sdl2"] = "%{wks.location}/sandstone/vendor/SDL2/include"

group "Dependencies"
	include "sandstone/vendor/SDL2/SDL2.lua"
	filter "system:windows"
		include "sandstone/vendor/SDL2/SDL2main.lua"

group ""
	include "sandstone"
	include "sdl_test"