-- premake5.lua
project "udan_ecs"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    pchheader "ecspch.h"
    pchsource "src/ecspch.cpp"

    files {
        "src/**.cpp",
        "include/udan/ecs/**.h"
    }

    links { "udan_debug" }

    includedirs {
        "include/udan/ecs",
        "../udan_debug/include",
        "../ThirdParties/SpdLog/include"
    }
    