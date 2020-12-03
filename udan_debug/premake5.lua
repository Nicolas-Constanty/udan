-- premake5.lua
project "udan_debug"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    pchheader "debugpch.h"
    pchsource "src/debugpch.cpp"

    files {
        "src/**.cpp",
        "include/udan/debug/**.h"
    }

    includedirs { 
        "include/udan/debug",
        "../ThirdParties/SpdLog/include" -- string formating python like syntax
     }
    