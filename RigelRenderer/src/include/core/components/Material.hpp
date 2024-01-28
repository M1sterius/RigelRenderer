#pragma once

#include "glm.hpp"

#include <string>
#include <vector>

namespace rgr
{	
	class Shader;
	class Texture;

	class Material
	{
	public:
		std::string name = "Material";

		Material();
		virtual ~Material();

		virtual void SetUniforms();
		void Bind() const;
		void Unbind() const;
		inline rgr::Shader* GetShader() const { return m_Shader; }
	protected:
		rgr::Shader* m_Shader;

		void BindTexture(const std::string& name, const rgr::Texture* texture, unsigned int slot);
		void SetUniform1i(const std::string& name, const int value);
		void SetUniform1f(const std::string& name, const float value);
		void SetUniformVec2(const std::string& name, const glm::vec2& value);
		void SetUniformVec3(const std::string& name, const glm::vec3& value);
		void SetUniformVec4(const std::string& name, const glm::vec4& value);
		void SetUniformMat3(const std::string& name, const bool transpose, const glm::mat3& value);
		void SetUniformMat4(const std::string& name, const bool transpose, const glm::mat4& value);
	};
}
