#pragma once

/*
Implements an alternative to the "internal" access modifier available in C#.
Class members marked with "INTERNAL" will have different access levels based on
whether the preprocessor symbol "INTERNAL_LIB" is defined.

If "INTERNAL_LIB" is defined, "INTERNAL" is replaced with "public". Otherwise,
it is replaced with "private". This ensures that certain class members are only
publicly available inside the project that defines "INTERNAL_LIB".
*/

#ifdef INTERNAL_LIB
	#define INTERNAL public
#else
	#define INTERNAL private
#endif 
