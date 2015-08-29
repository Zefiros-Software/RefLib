local root      = "../../"

solution "reflection"

	location( root .. "reflection/" )
	objdir( root .. "bin/obj/" )
	debugdir( root .. "bin/" )
	
	configurations { "Debug", "Release", "Coverage"  }

	platforms { "x64", "x32" }

	vectorextensions "SSE2"

	floatingpoint "Fast"

	warnings "Extra"

	flags "Unicode"	

    configuration "x32"
		targetdir( root .. "bin/x32/" )
		architecture "x32"

    configuration "x64"
		targetdir( root .. "bin/x64/" )
		architecture "x64"
		
	configuration "Debug"
		targetsuffix "d"
		defines "DEBUG"
		flags "Symbols"
		optimize "Off"

	configuration "Release"		
		flags "LinkTimeOptimization"
		optimize "Speed"
				
	configuration {}
			
	project "reflection-test"
		location(  root .. "test/" )
				
		kind "ConsoleApp"
		flags "WinMain"
		defines "GTEST_HAS_TR1_TUPLE=0"
		
		includedirs {
			root .. "extern/gtest/include/",
			root .. "extern/gtest/",
			
			root .. "reflection/include/",
			root .. "test/"
			}	
		
		files { 
			root .. "extern/gtest/src/gtest-all.cc",
			root .. "test/**.h",
			root .. "test/*.cpp"
			}
			
		configuration "gmake"
			links "pthread"
			
		configuration { "Debug", "x32" }
			defines "PREFIX=X32D_"
		
		configuration { "Debug", "x64" }
			defines "PREFIX=X64D_"
		
		configuration { "Release", "x32" }
			defines "PREFIX=X32R_"
		
		configuration { "Release", "x64" }
			defines "PREFIX=X64R_"
			
	project "reflection"
		targetname "reflection"	 
		kind "StaticLib"

		includedirs {
			root .. "reflection/include/"
			}	
			
		files { 
			root .. "reflection/include/**.h"
			}