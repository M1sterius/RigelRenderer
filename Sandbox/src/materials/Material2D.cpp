#include "Material2D.hpp"

#include "core/components/Shader.hpp"

Material2D::Material2D(rgr::Texture* texture)
{
	m_Texture = texture;
	m_Shader = rgr::Shader::FromFiles("resources/shaders/vertex_2d.glsl", "resources/shaders/fragment_2d.glsl");
}

void Material2D::SetUniforms()
{
	BindTexture("u_Texture", m_Texture, 0);
	SetUniformVec4("u_Color", color);
}

Material2D::~Material2D()
{
	delete m_Shader;
}