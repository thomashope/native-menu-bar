
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

workspace "Examples"

	location ("build/".._ACTION)
    configurations { "Debug", "Release" }

    filter "configurations:Debug"
        defines { "CONFIG_DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "CONFIG_RELEASE" }
        optimize "On"

-- TODO: find a better way of adding the platform specific projects

if _ACTION == "xcode4" then

ExampleProject "Mac_Cocoa"

    links {
        "Cocoa.framework"
    }

    files {
        "Info.plist",
        "main_mac_cocoa.mm"
    }

ExampleProject "Mac_SDL2"

    links {
        "Cocoa.framework"
    }

    includeThirdPartyMacFramework "third_party/sdl2/mac/SDL2.framework"
    linkThirdPartyMacFramework "third_party/sdl2/mac/SDL2.framework"

    xcodebuildsettings {
        ["LD_RUNPATH_SEARCH_PATHS"] = "$(inherited) @executable_path/../Frameworks", -- tell the executable where to find the frameworks. Path is relative to executable location inside .app bundle
    }

    files {
        "Info.plist",
        "main_mac_sdl2.mm"
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

if _ACTION == "vs2022" then

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

end