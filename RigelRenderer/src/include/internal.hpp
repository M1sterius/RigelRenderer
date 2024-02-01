#pragma once

/*
Implements alternative to "internal" access modifier available in C#
Class members marked with "INTERNAL" will only be availbale inside the project that defines "INTERNAL_LIB"

Since "INTERNAL_LIB" is defined in RigelRenderer library,
all members marked with "INTERNAL" will be publicly available inside it but not inside projects using it
*/

#ifdef INTERNAL_LIB
	#define INTERNAL public
#else
	#define INTERNAL private
#endif 
