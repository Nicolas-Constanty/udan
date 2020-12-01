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


outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "udan_core"
include "udan_debug"
include "udan_ecs"
include "udan_glfw"
include "udan_windows"