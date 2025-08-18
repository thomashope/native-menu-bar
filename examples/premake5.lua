
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

function includeThirdPartyMacFramework(relative_path)
    local dir, filename = relative_path:match("^(.-)/([^/]+)$")
    externalincludedirs { relative_path.."/Headers" }
    frameworkdirs { dir }
end

function linkThirdPartyMacFramework(relativePath)
    local dir, filename = relativePath:match("^(.-)/([^/]+)$")
    frameworkdirs { dir }
    links { relativePath }
    embedAndSign { filename }
end


workspace "examples"

	location ("build/".._ACTION)
    configurations { "Debug", "Release" }

    filter "configurations:Debug"
        defines { "CONFIG_DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "CONFIG_RELEASE" }
        optimize "On"


if _TARGET_OS == "windows" then


    ExampleProject "example_win32"

        files {
            "example_win32.c",
            "../native_menu_bar.h",
            "../native_menu_bar.c",
        }


else


    ExampleProject "Mac_Cocoa"

        links {
            "Cocoa.framework"
        }

        files {
            "Info.plist",
            "main_mac_cocoa.mm"
        }

    ExampleProject "Mac_nmb_native"

        links {
            "Cocoa.framework"
        }

        files {
            "Info.plist",
            "main_mac_nmb_native.mm",
            "../native_menu_bar.h",
            "../native_menu_bar.cpp",
        }


end


ExampleProject "example_sdl2"

    files {
        "example_sdl2.c",
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
            "Cocoa.framework"
        }

        includeThirdPartyMacFramework "lib/sdl2/mac/SDL2.framework"
        linkThirdPartyMacFramework "lib/sdl2/mac/SDL2.framework"

        xcodebuildsettings {
            ["LD_RUNPATH_SEARCH_PATHS"] = "$(inherited) @executable_path/../Frameworks", -- tell the executable where to find the frameworks. Path is relative to executable location inside .app bundle
        }

        files {
            "Info.plist",
            "main_mac_sdl2.mm"
        }