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
	public:
		~Shader();

		static Shader FromFiles(const std::string& vertexPath, const std::string& fragmentPath);
		static Shader FromSources(const std::string& vertexSource, const std::string& fragmentSource);
	
		inline bool GetShaderHasError() const { return m_ShaderHasError; }
    private:

        unsigned int m_Handle;
        bool m_ShaderHasError;
        mutable std::unordered_map<std::string, int> m_UniformsLocationCache;

        Shader(const std::string& vertexSource, const std::string& fragmentSource);
	INTERNAL:
		void Bind() const;
		void Unbind() const;

		void BindTexture(const std::string& name, const rgr::Texture* texture, const int slot) const;
		void BindTexture(const std::string& name, const std::shared_ptr<Texture>& texture, const int slot) const;
		void SetUniform1is(const std::string& name, const int value) const;
		void SetUniform1i(const std::string& name, const unsigned int value) const;
		void SetUniform1i(const std::string& name, const size_t value) const;
		void SetUniform1f(const std::string& name, const float value) const;
        void SetUniformBool(const std::string& name, const bool value) const;
		void SetUniformVec2(const std::string& name, const glm::vec2& value) const;
		void SetUniformVec3(const std::string& name, const glm::vec3& value) const;
		void SetUniformVec4(const std::string& name, const glm::vec4& value) const;
		void SetUniformMat3(const std::string& name, const bool transpose, const glm::mat3& value) const;
		void SetUniformMat4(const std::string& name, const bool transpose, const glm::mat4& value) const;

		inline unsigned int GetHandle() const { return m_Handle; }
		int FindUniform(const std::string& name) const;

        enum class BUILT_IN_SHADERS
        {
            PLAIN_COLOR,
            DEPTH_MAP,
            GEOMETRY_PASS,
            STENCIL_PASS,
            DEPTH_VISUALIZATION,
            FULLSCREEN_TEXTURE,

            DIR_LIGHT_NO_SHADOWS,
            DIR_LIGHT_SHADOWS_NO_PCF,
            DIR_LIGHT_SHADOWS_PCF,

            POINT_LIGHT_NO_SHADOWS,

            SPOT_LIGHT_NO_SHADOWS,
        };

        static const Shader& GetBuiltInShader(BUILT_IN_SHADERS type);
	};
}
