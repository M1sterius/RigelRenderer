#pragma once

#include <string>

std::string ShaderErrorVertex =
{
	"#version 440 \n"
	"layout (location = 0) a_Pos; \n"
	"uniform mat4 u_MVP; \n"
	"void main() \n"
	"{ \n"
	"	gl_Position = vec4(a_Pos, 1.0) * u_MVP; \n"
	"} \n"
};
std::string ShaderErrorFragment =
{
	"#version 440 \n"
	"out vec4 outColor; \n"
	"void main() \n"
	"{ \n"
	"outColor = vec4(0.96, 0.25, 0.89, 1.0); \n"
	"} \n"
};

