#pragma once

#ifdef _DEBUG
	#define ASSERT(cond, message)											\
		if (!cond) {														\
			std::cerr << "Assertion Error! " << message << " In file: " <<	\
			__FILE__ << ", at line: " << __LINE__ << '\n';					\
			__debugbreak();													\
		}																	\
		else { }
#else
	#define ASSERT(cond, message)
#endif
