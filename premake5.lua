include "Dependencies.lua"

workspace "GPU_Bounce"
    architecture "x86_64"
    startproject "Test_1"

    configurations{
        "Release",
        "Debug"
    }

    flags
    {
        "MultiProcessorCompile"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"

include "GPU_Bounce"
    