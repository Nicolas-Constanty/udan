-- premake5.lua
project "udan_core"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    pchheader "corepch.h"
    pchsource "src/corepch.cpp"

    files {
        "src/**.cpp",
        "include/udan/core/**.h"
    }

    links { "udan_ecs", "udan_debug", "udan_utils" }

    includedirs {
        "include/udan/core",
        "../udan_debug/include",
        "../udan_ecs/include",
        "../udan_utils/include",
        "../ThirdParties/Clipp/include", -- Arg parser
        "../ThirdParties/tomlplusplus/include", -- Toml parser
        "../ThirdParties/SpdLog/include"
    }
    