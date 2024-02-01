project "RigelRenderer"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "off"

   files { "src/**.hpp", "src/**.cpp" }

   includedirs
   {
      "src",
      "src/include",
      "src/internal",
      "src/built-in",

      "../Vendor/glfw/include/GLFW",
      "../Vendor/glew/include/GL",
      "../Vendor/glm",
      "../Vendor/obj_loader",
      "../Vendor/stb_image",
      "../Vendor/freetype",
      "../Vendor/gltext",
      "../Vendor"
   }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

   defines 
   {
        "GLEW_STATIC",
        "INTERNAL_LIB"
   }

   filter "system:windows"
       systemversion "latest"
       defines { }

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