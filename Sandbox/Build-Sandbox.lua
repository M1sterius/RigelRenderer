project "Sandbox"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "off"

   files { "src/**.hpp", "src/**.cpp" }

   includedirs
   {
      "src",
      "../Vendor/glm",
	  "../RigelRenderer/src",
      "../RigelRenderer/src/include"
   }

   links
   {
       "opengl32.lib",
       "../Vendor/glfw/lib-vc2022/glfw3.lib",
       "../Vendor/glew/lib/Release/x64/glew32s.lib",
       "../Vendor/freetype/lib/freetype.lib",
       "RigelRenderer"
   }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

   defines 
   {
        "GLEW_STATIC"
   }

   filter "system:windows"
       systemversion "latest"
       defines { "WINDOWS" }

   filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "On"

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"

   filter "configurations:Dist"
       defines { "DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"

