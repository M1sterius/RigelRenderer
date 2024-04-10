#pragma once

#ifdef _DEBUG
	#define ASSERT(cond)									   \
		if (!cond) {										   \
			std::cerr << "Assertion Error! " << "In file: " << \
			__FILE__ << ", at line: " << __LINE__ << '\n';     \
		}													   \
		else { }
#else
	#define ASSERT(cond)
#endif 



