-- premake5.lua
project "udan_windows"
    kind "SharedLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    pchheader "pch.h"
    pchsource "src/pch.cpp"

    files {
        "src/**.cpp",
        "includes/**.h"
    }

    includedirs { "includes" }
    