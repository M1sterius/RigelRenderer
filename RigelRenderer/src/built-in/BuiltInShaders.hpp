#pragma once

#include <string>

namespace rgr
{
	const std::string PlainColorVertex =
	{
		"#version 440 \n"
		"layout (location = 0) in vec3 a_Pos; \n"
		"uniform mat4 u_MVP; \n"
		"void main() \n"
		"{ \n"
		"	gl_Position = vec4(a_Pos, 1.0) * u_MVP;	\n"
		"} \n"
	};
	const std::string PlainColorFragment =
	{
		"#version 440 \n"
		"uniform vec4 u_Color; \n"
		"out vec4 outColor; \n"
		"void main() \n"
		"{ \n"
			"outColor = u_Color; \n"
		"} \n"
	};

	const std::string DepthMapVertex =
	{
		"#version 440 \n"
		"layout (location = 0) in vec3 aPos; \n"
		"uniform mat4 u_LightSpaceMatrix; \n"
		"uniform mat4 u_Model; \n"
		"void main() \n"
		"{ \n"
			"gl_Position = u_LightSpaceMatrix * u_Model * vec4(aPos, 1.0); \n"
		"} \n"
	};

	const std::string DepthMapFragment =
	{
		"#version 440 \n"
		"void main() \n"
		"{ \n"
		"	gl_FragDepth = gl_FragCoord.z; \n"
		"} \n"
	};

	const std::string DepthTestVertex =
	{
		"#version 440 \n"
		"layout (location = 0) in vec3 a_Pos; \n"
		"layout (location = 1) in vec2 a_TexCoords; \n"
		"out vec2 v_TexCoords; \n"
		"void main() \n"
		"{ \n"
		"	gl_Position = vec4(a_Pos, 1.0); \n"
		"	v_TexCoords = a_TexCoords; \n"
		"} \n"	
	};
	const std::string DepthTestFragment =
	{
		"#version 440 \n"
		"in vec2 v_TexCoords; \n"
		"uniform sampler2D u_DepthMap; \n"
		"out vec4 FragColor; \n"
		"void main() \n"
		"{ \n"
		"	float depthValue = texture(u_DepthMap, v_TexCoords).r; \n"
		"	FragColor = vec4(vec3(depthValue), 1.0); \n"
		"} \n"
	};
}
