#include "Texture.hpp"
#include "Shader.hpp"
#include "Material.hpp"

#include <iostream>

rgr::Material::Material(rgr::Shader* shader)
{
	m_Shader = shader;
}

rgr::Material::~Material()
{
	
}
void rgr::Material::SetUniforms()
{
	
}

void rgr::Material::Bind() const
{
	m_Shader->Bind();
}

void rgr::Material::Unbind() const
{
	m_Shader->Unbind();
}

void rgr::Material::BindTexture(const std::string& name, const rgr::Texture* texture, unsigned int slot)
{
    texture->BindToSlot(slot);
	SetUniform1i(name, slot);
}

void rgr::Material::SetUniform1i(const std::string& name, const int value)
{
    m_Shader->SetUniform1is(name, value);
}

void rgr::Material::SetUniform1i(const std::string& name, const unsigned int value)
{
	m_Shader->SetUniform1i(name, value);
}

void rgr::Material::SetUniform1f(const std::string& name, const float value)
{
	m_Shader->SetUniform1f(name, value);
}

void rgr::Material::SetUniformVec2(const std::string& name, const glm::vec2& value)
{
	m_Shader->SetUniformVec2(name, value);
}

void rgr::Material::SetUniformVec3(const std::string& name, const glm::vec3& value)
{
	m_Shader->SetUniformVec3(name, value);
}

void rgr::Material::SetUniformVec4(const std::string& name, const glm::vec4& value)
{
	m_Shader->SetUniformVec4(name, value);
}

void rgr::Material::SetUniformMat3(const std::string& name, const bool transpose, const glm::mat3& value)
{
	m_Shader->SetUniformMat3(name, transpose, value);
}

void rgr::Material::SetUniformMat4(const std::string& name, const bool transpose, const glm::mat4& value)
{
	m_Shader->SetUniformMat4(name, transpose, value);
}
