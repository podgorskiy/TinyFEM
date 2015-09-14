#!lua

   project "glew"
      kind "StaticLib"
      language "C"
      files { "**glew.c", "include**" }
      includedirs {
         "include",
      }
	  
      targetdir("lib")

	  flags {  "StaticRuntime", "FloatFast", "NoRTTI" }
	  
      configuration "Debug"
         defines { "DEBUG", "_LIB",  "GLEW_STATIC" }
         flags { "Symbols" }

      configuration "Release"
         defines { "NDEBUG", "_LIB", "GLEW_STATIC"  }
         flags { "Optimize" }


