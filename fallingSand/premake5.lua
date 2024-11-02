project "fallingSand"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/compile/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/compile/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.SDL2}",
		"%{IncludeDir.ImGui}",
		"%{wks.location}/sandstone/src",
		"%{wks.location}/sandstone/vendor",
	}

	links
	{
		"sandstone",
		-- "ImGui"
	}

	postbuildcommands {
		"{COPY} %{IncludeDir.SDL2}/../lib/x64/SDL2.dll %{cfg.targetdir}"
	}
	
	filter "system:windows"
	systemversion "latest"

	filter "system:linux"
		systemversion "latest"

	filter "system:macosx"
		systemversion "latest"
        xcodebuildsettings { ["ALWAYS_SEARCH_USER_PATHS"] = "YES" }

	filter "configurations:Debug*"
		defines "SS_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release*"
		defines "SS_RELEASE"
		runtime "Debug"
		symbols "on"

	filter "configurations:Dist*"
		defines "SS_DIST"
		runtime "Release"
		optimize "on"
		symbols "off"
