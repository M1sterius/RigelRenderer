#pragma once

#include "glm.hpp"
#include "internal.hpp"

#include <string>
#include <vector>

namespace rgr
{	
	class Shader;
	class Texture;

	class Material
	{
	private:
		rgr::Shader* m_Shader;
		bool m_IsLit = false;
	public:
		std::string name = "Material";

		Material(rgr::Shader* shader);
		virtual ~Material();

		virtual void SetUniforms();
	protected:
		/*
		Will make this material affected by light
		Note: A shader attached to material marked as lit must correctly handle all light calculations,
		otherwise compilation or uniforms set errors are possible
		*/
		void MakeLit();

		void BindTexture(const std::string& name, const rgr::Texture* texture, unsigned int slot);
		void SetUniform1i(const std::string& name, const int value);
		void SetUniform1i(const std::string& name, const unsigned int value);
		void SetUniform1f(const std::string& name, const float value);
		void SetUniformVec2(const std::string& name, const glm::vec2& value);
		void SetUniformVec3(const std::string& name, const glm::vec3& value);
		void SetUniformVec4(const std::string& name, const glm::vec4& value);
		void SetUniformMat3(const std::string& name, const bool transpose, const glm::mat3& value);
		void SetUniformMat4(const std::string& name, const bool transpose, const glm::mat4& value);
	INTERNAL:
		void Bind() const;
		void Unbind() const;

		inline bool GetIsLit() const { return m_IsLit; }
		inline rgr::Shader* GetShader() const { return m_Shader; }
	};
}
