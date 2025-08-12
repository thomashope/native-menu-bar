workspace "Examples"

	location ("build/".._ACTION)
    configurations { "Debug", "Release" }

    filter "configurations:Debug"
        defines { "CONFIG_DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "CONFIG_RELEASE" }
        optimize "On"

function ExampleProject(name)
	project(name)
    kind "WindowedApp"
	location ("build/".._ACTION)
    targetdir ("bin/".. _ACTION.."/"..name.."/%{cfg.buildcfg}")
    language "C++"
    cppdialect "C++11"
    architecture "x86_64"
    warnings "Default"
	flags "MultiProcessorCompile"
end

ExampleProject "Win32"

    files {
    	"main_win32.cpp"
    }

ExampleProject "Win_SDL2"

    externalincludedirs {
        "third_party/sdl2/windows/include"
    }

    libdirs {
        "third_party/sdl2/windows/lib/x64",
    }

    postbuildcommands {
        "{COPYFILE} " .. _MAIN_SCRIPT_DIR .. "/third_party/sdl2/windows/lib/x64/SDL2.dll %{cfg.buildtarget.directory}",
    }

    links {
        "SDL2",
        "SDL2main",
    }

	files {
		"main_win_sdl2.cpp"
	}