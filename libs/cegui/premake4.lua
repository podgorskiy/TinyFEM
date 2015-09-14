#!lua

   project "cegui"
      kind "StaticLib"
      language "C++"
      files { "cegui/**.h", "cegui/**.cpp" }
	  excludes { "**DevIL**", "**Direct**", "**Irrlicht**", "**Ogre/**", "**OpenGLES**", "**ScriptModules**", "**macPlugins**" , "**Expat/**", "**Libxml2**", "**RapidXML**", "**Xerces**", 
	  "**Iconv**", "**PCRERegex**", "**Minizip**", "**Apple**", "**GLXPBTextureTarget**", "**WGLPBTextureTarget**", "**GLFBOTextureTarget**" , "**Corona**", "**FreeImage**", "**PVR/**" , "**SILLY**" , "**TGA/**"  }
      includedirs {
         "cegui/include",
         "../tinyxml/",
         "../glew/include/",
         "../freetype/include/",
         "../glm/"
      }
	  
      targetdir("lib")
	  
	  flags {  "StaticRuntime", "FloatFast" }

      configuration "Debug"
         defines { "DEBUG", "CEGUI_STATIC", "_LIB",  "GLEW_STATIC" }
         flags { "Symbols" }

      configuration "Release"
         defines { "NDEBUG", "CEGUI_STATIC", "_LIB",  "GLEW_STATIC" }
         flags { "Optimize" }


