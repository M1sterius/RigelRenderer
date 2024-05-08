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

	const std::string TextureTestVertex =
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
	const std::string TextureTestFragment =
	{
		"#version 440 \n"

		"in vec2 v_TexCoords; \n"

		"uniform sampler2D u_Texture; \n"

		"out vec4 FragColor; \n"

		"void main() \n"
		"{ \n"
		"	FragColor = vec4(texture(u_Texture, v_TexCoords).rgb, 1.0); \n"
		"} \n"
	};

	const std::string GeometryPassVertex
	{
		"#version 440 \n"

		"layout (location = 0) in vec3 a_Position; \n"
		"layout (location = 1) in vec2 a_TexCoord; \n"
		"layout (location = 2) in vec3 a_Normal; \n"

		"uniform mat4 u_MVP; \n"
		"uniform mat4 u_Model; \n"
		"uniform mat3 u_NormalMatrix; \n"

		"out vec2 v_TexCoords; \n"
		"out vec3 v_Normal; \n"
		"out vec3 v_FragPos; \n"

		"void main() \n"
		"{ \n"
			"gl_Position = vec4(a_Position, 1.0) * u_MVP; \n"
			"v_TexCoords = a_TexCoord; \n"

			"v_FragPos = vec3(u_Model * vec4(a_Position, 1.0)); \n"
			"v_Normal = u_NormalMatrix * a_Normal; \n"
		"} \n"
	}; 

	const std::string GeometryPassFragment
	{
		"#version 440 \n"

		"layout (location = 0) out vec3 gPosition; \n"
		"layout (location = 1) out vec3 gNormal; \n"
		"layout (location = 2) out vec4 gAlbedoSpec; \n"

		"uniform sampler2D u_DiffuseTexture; \n"
		"uniform sampler2D u_SpecularTexture; \n"

		"in vec2 v_TexCoords; \n"
		"in vec3 v_Normal; \n"
		"in vec3 v_FragPos; \n"

		"void main() \n"
		"{ \n"
			"gPosition = v_FragPos; \n"
			"gNormal = normalize(v_Normal); \n"
			"gAlbedoSpec.rgb = texture(u_DiffuseTexture, v_TexCoords).rgb; \n"
			"gAlbedoSpec.a = texture(u_SpecularTexture, v_TexCoords).r; \n"
		"} \n"
	};
}
