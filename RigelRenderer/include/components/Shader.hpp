#pragma once

#include "fwd.hpp"
#include "internal.hpp"

#include <string>
#include <unordered_map>

namespace rgr
{
	static int GetShaderCompileInfo(const unsigned int shaderHandle);
	static std::string ProcessShaderSource(const std::string& sourcePath);

	class Shader
	{
	private:
		unsigned int m_Handle;
		bool m_ShaderHasError;
		int m_UnifromsCallback;
		std::unordered_map<std::string, int> m_UniformsLocationCache;
		
		Shader(const std::string& vertexSource, const std::string& fragmentSource);
	public:
		~Shader();

		static Shader* FromFiles(const std::string& vertexPath, const std::string& fragmentPath);
		static Shader* FromSources(const std::string& vertexSource, const std::string& fragmentSource);
	
		inline bool GetShaderHasError() const { return m_ShaderHasError; }
	INTERNAL:
		void Bind() const;
		void Unbind() const;

		void SetUniform1i(const std::string& name, const int value);
		void SetUniform1i(const std::string& name, const unsigned int value);
		void SetUniform1i(const std::string& name, const size_t value);
		void SetUniform1f(const std::string& name, const float value);
		void SetUniformVec2(const std::string& name, const glm::vec2& value);
		void SetUniformVec3(const std::string& name, const glm::vec3& value);
		void SetUniformVec4(const std::string& name, const glm::vec4& value);
		void SetUniformMat3(const std::string& name, const bool transpose, const glm::mat3& value);
		void SetUniformMat4(const std::string& name, const bool transpose, const glm::mat4& value);

		inline unsigned int GetHandle() const { return m_Handle; }
		int FindUniform(const std::string& name);
		int GetUniformsCallback();

		static Shader* GetPlainColorShader();
		static Shader* GetDepthMapShader();
	};
}

