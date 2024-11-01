workspace "fallingSand"
	startproject "fallingSand"

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
IncludeDir["spdlog"] = "%{wks.location}/sandstone/vendor/spdlog/include"
IncludeDir["sdl2"] = "%{wks.location}/sandstone/vendor/SDL2/include"
IncludeDir["ImGui"] = "%{wks.location}/sandstone/vendor/imgui"

group "Dependencies"
	include "sandstone/vendor/imgui"
	include "sandstone/vendor/SDL2/SDL2.lua"
	filter "system:windows"
		include "sandstone/vendor/SDL2/SDL2main.lua"

group ""
	include "sandstone"
	include "fallingSand"