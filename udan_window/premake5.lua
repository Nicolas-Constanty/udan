-- premake5.lua
project "udan_window"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    pchheader "windowpch.h"
    pchsource "src/windowpch.cpp"

    files {
        "src/**.cpp",
        "include/udan/window/**.h"
    }

    includedirs { "include/udan/window" }
    