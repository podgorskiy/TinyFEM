#!lua

   project "glfw"
      kind "StaticLib"
      language "C"
      files { "**compileUnit.c" }

      targetdir("lib")
	  
      includedirs {
         "../glew/include"
	  }

	  flags {  "StaticRuntime", "FloatFast", "NoRTTI" }
	  
      configuration "Debug"
         defines { "DEBUG" }
         flags { "Symbols" }

      configuration "Release"
         defines { "NDEBUG" }
         flags { "Optimize" }


