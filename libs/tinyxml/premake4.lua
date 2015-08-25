#!lua

   project "tinyxml"
      kind "StaticLib"
      language "C++"
      files { "*.h", "*.cpp" }
      excludes { "xmltest*" }

      targetdir("lib")

      configuration "Debug"
         defines { "DEBUG" }
         flags { "Symbols" }

      configuration "Release"
         defines { "NDEBUG" }
         flags { "Optimize" }


