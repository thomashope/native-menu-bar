
function ExampleProject(name)
    project(name)
    kind "WindowedApp"
    location ("build/".._ACTION)
    targetdir ("bin/".. _ACTION.."/"..name.."/%{cfg.buildcfg}")
    language "C++"
    cppdialect "C++98"
    cdialect "c89"
    architecture "x86_64"
    warnings "Default"
    flags "MultiProcessorCompile"
    warnings "Extra"
end

workspace "examples"

	location ("build/".._ACTION)
    configurations { "Debug", "Release" }

    filter { "configurations:Debug" }
        defines { "CONFIG_DEBUG" }
        symbols "On"

    filter { "configurations:Release" }
        defines { "CONFIG_RELEASE" }
        optimize "On"

    filter { "files:../*.c", "action:xcode*" }
        compileas "Objective-C" 

    filter {}


if _TARGET_OS == "windows" then

    ExampleProject "example_win32"

        files {
            "example_win32.c",
            "common.inl",
            "../native_menu_bar.h",
            "../native_menu_bar.c",
        }

else -- assume target is macos

    ExampleProject "example_cocoa"

        links {
            "Cocoa.framework"
        }

        files {
            "Info.plist",
            "example_cocoa.m",
            "../native_menu_bar.h",
            "../native_menu_bar.c",
        }

end

ExampleProject "example_sdl2"

    files {
        "example_sdl2.c",
        "common.inl",
        "../native_menu_bar.h",
        "../native_menu_bar.c",
    }

    filter "action:vs*"

        externalincludedirs {
            "lib/sdl2/windows/include"
        }

        libdirs {
            "lib/sdl2/windows/lib/x64",
        }

        links {
            "SDL2",
            "SDL2main",
        }

        postbuildcommands {
            "{COPYFILE} " .. _MAIN_SCRIPT_DIR .. "/lib/sdl2/windows/lib/x64/SDL2.dll %{cfg.buildtarget.directory}",
        }

    filter "action:xcode*"

        links {
            "Cocoa.framework",
            "SDL2"
        }

        libdirs {
            "/opt/homebrew/lib"
        }

        externalincludedirs {
            "/opt/homebrew/include/SDL2"
        }

        files {
            "Info.plist"
        }
