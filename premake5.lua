workspace "fallingSand"
	startproject "conwaysGame"

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
IncludeDir["SDL2"] = "%{wks.location}/sandstone/vendor/SDL2-VC/include"
IncludeDir["ImGui"] = "%{wks.location}/sandstone/vendor/imgui"

group "Dependencies"
	include "sandstone/vendor/imgui"

group ""
	include "sandstone"
	include "fallingSand"
	include "conwaysGame"