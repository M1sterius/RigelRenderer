-- premake5.lua
workspace "RigelEngine"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "Sandbox"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "Engine"
	include "RigelRenderer/Build-Renderer.lua"
group ""

group "Testing"
   include "Sandbox/Build-Sandbox.lua"
group ""
