math.randomseed(1014)
	
solution "fem"
	
	language "C++"

	location ( _ACTION )

	buildoptions { "/MP", "/EHsc"}
	defines {"_SCL_SECURE_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS"}

	flags {"Symbols", "NoMinimalRebuild",  "No64BitChecks", "StaticRuntime" , "NoRTTI" }

	configurations { "Debug", "Release"}

	configuration "Release"
		defines { "NDEBUG", "HB_DEBUG=0" }
		flags{ "OptimizeSize"}
	
	configuration "Debug"
		defines { "DEBUG", "HB_DEBUG=0" }

project "fem"
	files 
	{ 		
		"../sources/main.cpp"
	}
		
	defines {"DEBUG"}

	kind "ConsoleApp"
	
	includedirs 
	{ 
		"../libs/poly2tri", 	
		"../libs/tinyxml",
		"../libs/eigen"	
	}
				   
	targetdir ( "../bin/" )
		
	configuration "Debug"
		targetname( "fem_d" )
		
	configuration "Release"
		targetname( "fem" )
	