-- premake5.lua
project "udan_glfw"
    kind "SharedLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    pchheader "glfwpch.h"
    pchsource "src/glfwpch.cpp"

    files {
        "src/**.cpp",
        "include/udan/glfw/**.h"
    }

    includedirs { "include/udan/glfw" }
    