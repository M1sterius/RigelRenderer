#pragma once

#include "fwd.hpp"
#include "Internal.hpp"

#include <string>
#include <unordered_map>
#include <memory>

namespace rgr
{
	class Texture;

	class Shader
	{
	private:
		unsigned int m_Handle;
		bool m_ShaderHasError;
		int m_UniformsCallback = 0;
		std::unordered_map<std::string, int> m_UniformsLocationCache;
        static std::string m_BuildInShadersPath;
		
		Shader(const std::string& vertexSource, const std::string& fragmentSource);
	public:
		~Shader();

		static Shader* FromFiles(const std::string& vertexPath, const std::string& fragmentPath);
		static Shader* FromSources(const std::string& vertexSource, const std::string& fragmentSource);
	
		inline bool GetShaderHasError() const { return m_ShaderHasError; }

        static void SetBuildInShadersPath(const std::string& path);
        static std::string GetBuildInShadersPath();
	INTERNAL:
		void Bind() const;
		void Unbind() const;

		void BindTexture(const std::string& name, const rgr::Texture* texture, unsigned int slot);
		void BindTexture(const std::string& name, const std::shared_ptr<Texture>& texture, unsigned int slot);
		void SetUniform1i(const std::string& name, const int value);
		void SetUniform1i(const std::string& name, const unsigned int value);
		void SetUniform1i(const std::string& name, const size_t value);
		void SetUniform1f(const std::string& name, const float value);
        void SetUniformBool(const std::string& name, const bool value);
		void SetUniformVec2(const std::string& name, const glm::vec2& value);
		void SetUniformVec3(const std::string& name, const glm::vec3& value);
		void SetUniformVec4(const std::string& name, const glm::vec4& value);
		void SetUniformMat3(const std::string& name, const bool transpose, const glm::mat3& value);
		void SetUniformMat4(const std::string& name, const bool transpose, const glm::mat4& value);

		inline unsigned int GetHandle() const { return m_Handle; }
		int FindUniform(const std::string& name);
		int GetUniformsCallback();

        enum class BUILT_IN_SHADERS
        {
            PLAIN_COLOR,
            DEPTH_MAP,
            GEOMETRY_PASS,
            LIGHTING_PASS,
            TEXTURE_TEST
        };

        static Shader* GetBuiltInShader(BUILT_IN_SHADERS type);
	};
}

