#pragma once

#include <string>

namespace rgr
{
	const std::string PlainColorVertex =
	{
		"#version 440														\n"
		"layout (location = 0) in vec3 a_Pos;								\n"
		"uniform mat4 u_MVP;												\n"
		"void main()														\n"
		"{																	\n"
		"	gl_Position = vec4(a_Pos, 1.0) * u_MVP;							\n"
		"}																	\n"
	};
	const std::string PlainColorFragment =
	{
		"#version 440														\n"
		"uniform vec4 u_Color;												\n"
		"out vec4 outColor;													\n"
		"void main()														\n"
		"{																	\n"
			"outColor = u_Color;											\n"
		"}																	\n"
	};

	const std::string DepthMapVertex =
	{
		"#version 440														\n"
		"layout (location = 0) in vec3 aPos;								\n"
		"uniform mat4 u_LightSpaceMatrix;									\n"
		"uniform mat4 u_Model;												\n"
		"void main()														\n"
		"{																	\n"
			"gl_Position = u_LightSpaceMatrix * u_Model * vec4(aPos, 1.0);	\n"
		"}																	\n"
	};

	const std::string DepthMapFragment =
	{
		"#version 440														\n"
		"void main()														\n"
		"{																	\n"
		"	gl_FragDepth = gl_FragCoord.z;									\n"
		"}																	\n"
	};
}
