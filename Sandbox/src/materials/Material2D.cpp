#include "Material2D.hpp"

#include "components/Shader.hpp"

Material2D::Material2D(rgr::Texture* texture, rgr::Shader* shader) : rgr::Material(shader)
{
	m_Texture = texture;
	color = glm::vec4(0.1, 0.2, 0.3, 1.0);
}

void Material2D::SetUniforms()
{
	BindTexture("u_Texture", m_Texture, 6);
	SetUniformVec4("u_Color", color);
}

Material2D::~Material2D()
{

}