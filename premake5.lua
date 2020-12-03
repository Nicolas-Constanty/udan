-- premake5.lua
workspace "Udan"

    architecture "x86_64"

    configurations
    { 
        "Debug",
        "Release",
        "Dist"
    }

    flags {
        "MultiProcessorCompile"
    }

    -- Log library
    defines { "BUILD_LOGURU", "BUILD_SPDLOG" }
    -- Graphic library
    defines { "BUILD_GLFW" }


outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "udan_core"

-- Log lib
include "udan_debug"
filter "BUILD_LOGURU"
    include "udan_loguru"
filter "BUILD_SPDLOG"
    include "udan_spdlog"

include "udan_ecs"

-- Graphic lib
include "udan_window"
filter "BUILD_GLFW"
    include "udan_glfw"
