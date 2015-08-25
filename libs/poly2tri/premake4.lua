#!lua

   project "poly2tri"
      kind "StaticLib"
      language "C++"
      files { "poly2tri/**.h", "poly2tri/**.cc" }

      targetdir("lib")

      configuration "Debug"
         defines { "DEBUG" }
         flags { "Symbols" }

      configuration "Release"
         defines { "NDEBUG" }
         flags { "Optimize" }


