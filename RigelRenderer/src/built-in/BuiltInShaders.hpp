#pragma once

#include <string>

namespace rgr
{
	inline std::string PlainColorVertex =
	{
		"#version 440 \n"
		"layout (location = 0) in vec3 a_Pos; \n"
		"uniform mat4 u_MVP; \n"
		"void main() \n"
		"{ \n"
		"	gl_Position = vec4(a_Pos, 1.0) * u_MVP; \n"
		"} \n"
	};
	inline std::string PlainColorFragment =
	{
		"#version 440 \n"
		"uniform vec4 u_Color;"
		"out vec4 outColor; \n"
		"void main() \n"
		"{ \n"
		"outColor = u_Color; \n"
		"} \n"
	};
}
