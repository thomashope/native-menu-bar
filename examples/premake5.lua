workspace "Examples"

	location ("build/".._ACTION)
    configurations { "Debug", "Release" }

    filter "configurations:Debug"
        defines { "CONFIG_DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "CONFIG_RELEASE" }
        optimize "On"

project "Win32"

    kind "WindowedApp"
	location ("build/".._ACTION)
    targetdir ("bin/".. _ACTION.."/Win32/%{cfg.buildcfg}")
    language "C++"
    cppdialect "C++11"
    architecture "x86_64"
    warnings "Default"
	
    files {
    	"main_win32.cpp"
    }