project "GPU_Bounce"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("%{wks.location}/bin/"..outputdir.."/%{prj.name}")
    objdir ("%{wks.location}/bin-int/"..outputdir.."/%{prj.name}")

    files
    {
        "glad.c",
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "src",
        "rsc",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}"
    }

    libdirs
    {
        "%{LibraryDir.GLFW}"
    }

    links
    {
        "glfw3.lib",
        "opengl32.lib"
    }

    postbuildcommands
    {
        ("XCOPY \"%{wks.location}vendor\\GLFW\\lib-vc2019\\*.dll\" \"%{wks.location}\\bin\\"..outputdir.."\\%{prj.name}\\\" /s/e/k/f/c/y"),
        ("XCOPY \"%{wks.location}GPU_Bounce\\rsc\\\" \"%{wks.location}\\bin\\"..outputdir.."\\%{prj.name}\\rsc\\\" /s/e/k/f/c/y")
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines { "RTS_DEBUG" }
        runtime "Debug"
        symbols "On"

    
    filter "configurations:Release"
        defines { "RTS_RELEASE" }
        runtime "Release"
        optimize "Speed"
        symbols "Off"